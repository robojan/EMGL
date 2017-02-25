
#include <emgl/font.h>
#include <emgl/driver.h>
#include <emgl/debug.h>
#include <emgl/graphics.h>

#if defined(EMGL_USE_VLA) && defined(_MSC_VER)
#error Microsoft Visual Compiler does not support VLA disable EMGL_USE_VLA
#endif

#define DRIVER g_emgl_activeDriver

void emgl_drawTextA(const EMGL_font_t *font, const char *str, emgl_coord_t x, emgl_coord_t y, 
	emgl_color_t foregroundColor, emgl_color_t backgroundColor)
{
	EMGL_ASSERT("font != NULL", font != NULL);
	// Move to origin
	y += font->ascender;

	// Draw draw each glyph
	while (*str != '\0') {
		emgl_drawGlyph(font, *str, &x, &y, foregroundColor, backgroundColor);
		str++;
	}
}

void emgl_drawTextU32(const EMGL_font_t *font, const emgl_U32 *str, emgl_coord_t x, emgl_coord_t y, 
	emgl_color_t foregroundColor, emgl_color_t backgroundColor)
{
	EMGL_ASSERT("font != NULL", font != NULL);
	// Move to origin
	y += font->ascender;

	// Draw draw each glyph
	while (*str != 0) {
		emgl_drawGlyph(font, *str, &x, &y, foregroundColor, backgroundColor);
		str++;
	}
}

const EMGL_codePage_t *emgl_getCodePage(const EMGL_font_t *font, emgl_U32 code) 
{
	EMGL_ASSERT("font != NULL", font != NULL);
	for (emgl_U8 i = 0; i < font->numCodepages; ++i) {
		if (code >= font->codePagePtrs[i].startCode && code <= font->codePagePtrs[i].endCode) {
			return &font->codePagePtrs[i];
		}
	}
	return NULL;
}

const EMGL_glyph_t *emgl_getGlyph(const EMGL_font_t *font, emgl_U32 code) 
{
	EMGL_ASSERT("font != NULL", font != NULL);
	const EMGL_codePage_t *page = emgl_getCodePage(font, code);
	if (page == NULL) return NULL;
	return page->glyphPtrs[code - page->startCode];
}

void emgl_drawGlyph(const EMGL_font_t *font, emgl_U32 code, emgl_coord_t *x, emgl_coord_t *y, 
	emgl_color_t foregroundColor, emgl_color_t backgroundColor)
{
	EMGL_ASSERT("font != NULL", font != NULL);
	EMGL_ASSERT("x != NULL", x != NULL);
	EMGL_ASSERT("y != NULL", y != NULL);
	const EMGL_glyph_t *glyph = emgl_getGlyph(font, code);
	if (glyph == NULL) {
		EMGL_LOG(EMGL_LOGLVL_DBG | EMGL_LOGMOD_FONT, "emgl_drawGlyph: No glyph found\n");
		return;
	}
	emgl_U32 size = (glyph->bitmapWidth * glyph->bitmapHeight * EMGL_COLOR_BPP + 7) / (8 * sizeof(emgl_color_t));
	if (size > 0) {
		EMGL_ASSERT("Bitmap size is > 0, but bitmapData == NULL", glyph->bitmapData != NULL);
#ifdef EMGL_USE_VLA
		emgl_color_t bitmap[size];
#else
		emgl_color_t *bitmap = (emgl_color_t *)emgl_malloc(size * sizeof(emgl_color_t));
#endif
		// Get the bitmap
		if ((glyph->bitmapSize & (1 << 31)) != 0) {
			// compressed
#ifndef EMGL_COMPRESSED_FONTS
			EMGL_ASSERT("Compressed font found when EMGL_COMPRESSED_FONTS == 0", 0);
#endif
			emgl_getDecompressedGlyphColoredBitmap(glyph->bitmapData, glyph->bitmapSize & ~(1 << 31),
				font->bpp, glyph->bitmapWidth, glyph->bitmapHeight, foregroundColor, backgroundColor, bitmap);
		}
		else {
			emgl_getGlyphColoredBitmap(glyph->bitmapData, glyph->bitmapSize, font->bpp,
				glyph->bitmapWidth, glyph->bitmapHeight, foregroundColor, backgroundColor, bitmap);
		}
		// draw the bitmap
		DRIVER.drawBitmap(DRIVER.api, *x + glyph->bearingX, *y - glyph->bitmapHeight + glyph->bearingY,
			glyph->bitmapWidth, glyph->bitmapHeight, bitmap);

#ifndef EMGL_USE_VLA
		emgl_free(bitmap);
#endif
	}
	*x = *x + glyph->advanceX;
	*y = *y - glyph->advanceY;
}

emgl_color_t emgl_getColorFromPixelValue(emgl_U8 value, emgl_U8 bpp, emgl_color_t foregroundColor, emgl_color_t backgroundColor)
{
	emgl_U8 strength;
	switch (bpp) {
	case 1:
		strength = value ? 0xFF : 0x00;
		break;
	case 2:
		strength = 85 * value;
		break;
	case 4:
		strength = 17 * value;
		break;
	case 8:
		strength = value;
		break;
	default:
		EMGL_LOG(EMGL_LOGLVL_ERR | EMGL_LOGMOD_FONT, "Unknown bpp in font");
		return COLOR_BLACK;
	}
	return emgl_colorBlend(foregroundColor, backgroundColor, strength);
}

void emgl_getGlyphColoredBitmap(const emgl_U8 *in, emgl_U32 inSize, emgl_U8 inBpp, emgl_U16 width, emgl_U16 height, 
	emgl_color_t foregroundColor, emgl_color_t backgroundColor, emgl_color_t *out)
{
	EMGL_ASSERT("in != NULL", in != NULL);
	EMGL_ASSERT("out != NULL", in != NULL);
	emgl_U8 mask = 0xFF >> (8 - inBpp);
	emgl_U32 bitPos = 0;
	for (emgl_U16 y = 0; y < height; ++y) {
		for (emgl_U16 x = 0; x < width; ++x) {
			emgl_U32 idx = bitPos / 8;
			emgl_U8 bit = bitPos & 7;
			emgl_U8 value = (in[idx] >> bit) & mask;
			emgl_color_t valueColor = emgl_getColorFromPixelValue(value, inBpp, foregroundColor, backgroundColor);
			emgl_colorModeSetPixel(out, (height - y - 1) * width + x, valueColor);
			bitPos += inBpp;
		}
	}
}

void emgl_getDecompressedGlyphColoredBitmap(const emgl_U8 *in, emgl_U32 inSize, emgl_U8 inBpp, emgl_U16 width, emgl_U16 height, 
	emgl_color_t foregroundColor, emgl_color_t backgroundColor, emgl_color_t *out)
{
	EMGL_ASSERT("in != NULL", in != NULL);
	EMGL_ASSERT("out != NULL", in != NULL);
	emgl_U8 mask = 0xFF >> (8 - inBpp);

	emgl_U32 bitPos = 0;
	emgl_U32 elementSize = 4 + inBpp;
	emgl_U16 x = 0, y = 0;
	while(y < height) {
		emgl_U32 idx = bitPos / 8;
		emgl_U8 bit = bitPos & 7;
		emgl_U8 elementSizeBytes = (bit + elementSize + 7) / 8;
		emgl_U16 element = 0;
		emgl_U8 elementBitsRead = 0;
		for (emgl_U8 j = 0; j < elementSizeBytes; ++j) {
			emgl_U8 bitsFree = 8 - bit;
			emgl_U8 bitsToRead = (elementSize - elementBitsRead) > bitsFree ? bitsFree : (elementSize - elementBitsRead);
			emgl_U8 elementMask = 0xFF >> (8 - bitsToRead);
			emgl_U8 elementPart = (in[idx] & (elementMask << bit)) >> bit;
			element |= elementPart << elementBitsRead;
			bitPos += bitsToRead;
			elementBitsRead += bitsToRead;
			idx = bitPos / 8;
			bit = bitPos & 7;
		}
		emgl_U8 repeat = (element & 0xF) + 1;
		emgl_U8 value = (element >> 4) & mask;
		emgl_color_t valueColor = emgl_getColorFromPixelValue(value, inBpp, foregroundColor, backgroundColor);
		for (emgl_U8 j = 0; y < height && j < repeat; ++j) {
			emgl_colorModeSetPixel(out, (height - y - 1) * width + x, valueColor);
			x++;
			if (x >= width) {
				x = 0;
				y++;
			}
		}		
	}
}
