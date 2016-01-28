
#include <emgl/font.h>
#include <emgl/driver.h>
#include <emgl/debug.h>
#include <emgl/graphics.h>

#if defined(EMGL_USE_VLA) && defined(_MSC_VER)
#error Microsoft Visual Compiler does not support VLA disable EMGL_USE_VLA
#endif

#define DRIVER g_emgl_activeDriver

void emgl_drawTextA(const EMGL_font_t *font, const char *str, emgl_coord_t x, emgl_coord_t y, emgl_color_t color)
{
	EMGL_ASSERT("font != NULL", font != NULL);
	// Move to origin
	y += font->ascender;

	// Draw draw each glyph
	while (*str != '\0') {
		emgl_drawGlyph(font, *str, &x, &y, color);
		str++;
	}
}

void emgl_drawTextU32(const EMGL_font_t *font, const U32 *str, emgl_coord_t x, emgl_coord_t y, emgl_color_t color)
{
	EMGL_ASSERT("font != NULL", font != NULL);
	// Move to origin
	y += font->ascender;

	// Draw draw each glyph
	while (*str != 0) {
		emgl_drawGlyph(font, *str, &x, &y, color);
		str++;
	}
}

const EMGL_codePage_t *emgl_getCodePage(const EMGL_font_t *font, U32 code) 
{
	EMGL_ASSERT("font != NULL", font != NULL);
	for (U8 i = 0; i < font->numCodepages; ++i) {
		if (code >= font->codePagePtrs[i].startCode && code <= font->codePagePtrs[i].endCode) {
			return &font->codePagePtrs[i];
		}
	}
	return NULL;
}

const EMGL_glyph_t *emgl_getGlyph(const EMGL_font_t *font, U32 code) 
{
	EMGL_ASSERT("font != NULL", font != NULL);
	const EMGL_codePage_t *page = emgl_getCodePage(font, code);
	if (page == NULL) return NULL;
	return page->glyphPtrs[code - page->startCode];
}

void emgl_drawGlyph(const EMGL_font_t *font, U32 code, emgl_coord_t *x, emgl_coord_t *y, emgl_color_t color)
{
	EMGL_ASSERT("font != NULL", font != NULL);
	EMGL_ASSERT("x != NULL", x != NULL);
	EMGL_ASSERT("y != NULL", y != NULL);
	const EMGL_glyph_t *glyph = emgl_getGlyph(font, code);
	if (glyph == NULL) {
		EMGL_LOG(EMGL_LOGLVL_DBG | EMGL_LOGMOD_FONT, "emgl_drawGlyph: No glyph found\n");
		return;
	}
	U32 size = (glyph->bitmapWidth * glyph->bitmapHeight * EMGL_COLOR_BPP + 7) / (8 * sizeof(emgl_color_t));
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
				font->bpp, glyph->bitmapWidth, glyph->bitmapHeight, color, bitmap);
		}
		else {
			emgl_getGlyphColoredBitmap(glyph->bitmapData, glyph->bitmapSize, font->bpp,
				glyph->bitmapWidth, glyph->bitmapHeight, color, bitmap);
		}
		// draw the bitmap
		DRIVER->drawBitmap(DRIVER->api, *x + glyph->bearingX, *y - glyph->bitmapHeight + glyph->bearingY,
			glyph->bitmapWidth, glyph->bitmapHeight, bitmap);

#ifndef EMGL_USE_VLA
		emgl_free(bitmap);
#endif
	}
	*x = *x + glyph->advanceX;
	*y = *y - glyph->advanceY;
}

emgl_color_t emgl_getColorFromPixelValue(U8 value, U8 bpp, emgl_color_t color)
{
	U32 colorRGBA = emgl_colorConvToRGBA8888(color);
	U8 strength;
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
#ifdef EMGL_USE_ALPHA_BLENDING
	strength = (colorRGBA >> 24) * strength / 255; // TODO: optimize
	return emgl_colorConvFromRGBA8888((colorRGBA & 0xFFFFFF) | (strength << 24));
#else
	U8 r = colorRGBA & 0xFF;
	U8 g = (colorRGBA >> 8) & 0xFF;
	U8 b = (colorRGBA >> 16) & 0xFF;
	r = (r*strength + 0xFF * (0xFF - strength)) / 255;
	g = (g*strength + 0xFF * (0xFF - strength)) / 255;
	b = (b*strength + 0xFF * (0xFF - strength)) / 255;
	return emgl_colorConvFromRGBA8888(0xFF000000 | r | (g << 8) | (b << 16)); // TODO not working yet
#endif
}

void emgl_getGlyphColoredBitmap(const U8 *in, U32 inSize, U8 inBpp, U16 width, U16 height, emgl_color_t color, emgl_color_t *out)
{
	EMGL_ASSERT("in != NULL", in != NULL);
	EMGL_ASSERT("out != NULL", in != NULL);
	U8 mask = 0xFF >> (8 - inBpp);
	U32 bitPos = 0;
	for (U16 y = 0; y < height; ++y) {
		for (U16 x = 0; x < width; ++x) {
			U32 idx = bitPos / 8;
			U8 bit = bitPos & 7;
			U8 value = (in[idx] >> bit) & mask;
			emgl_color_t valueColor = emgl_getColorFromPixelValue(value, inBpp, color);
			emgl_colorModeSetPixel(out, (height - y - 1) * width + x, valueColor);
			bitPos += inBpp;
		}
	}
}

void emgl_getDecompressedGlyphColoredBitmap(const U8 *in, U32 inSize, U8 inBpp, U16 width, U16 height, emgl_color_t color, emgl_color_t *out)
{
	EMGL_ASSERT("in != NULL", in != NULL);
	EMGL_ASSERT("out != NULL", in != NULL);
	U8 mask = 0xFF >> (8 - inBpp);

	U32 bitPos = 0;
	U32 elementSize = 4 + inBpp;
	U16 x = 0, y = 0;
	while(y < height) {
		U32 idx = bitPos / 8;
		U8 bit = bitPos & 7;
		U8 elementSizeBytes = (bit + elementSize + 7) / 8;
		U16 element = 0;
		U8 elementBitsRead = 0;
		for (U8 j = 0; j < elementSizeBytes; ++j) {
			U8 bitsFree = 8 - bit;
			U8 bitsToRead = (elementSize - elementBitsRead) > bitsFree ? bitsFree : (elementSize - elementBitsRead);
			U8 elementMask = 0xFF >> (8 - bitsToRead);
			U8 elementPart = (in[idx] & (elementMask << bit)) >> bit;
			element |= elementPart << elementBitsRead;
			bitPos += bitsToRead;
			elementBitsRead += bitsToRead;
			idx = bitPos / 8;
			bit = bitPos & 7;
		}
		U8 repeat = (element & 0xF) + 1;
		U8 value = (element >> 4) & mask;
		emgl_color_t valueColor = emgl_getColorFromPixelValue(value, inBpp, color);
		for (U8 j = 0; y < height && j < repeat; ++j) {
			emgl_colorModeSetPixel(out, (height - y - 1) * width + x, valueColor);
			x++;
			if (x >= width) {
				x = 0;
				y++;
			}
		}		
	}
}
