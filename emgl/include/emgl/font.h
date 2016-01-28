#ifndef _EMGL_FONT_H_
#define _EMGL_FONT_H_

#include <emgl/color.h>
#include <emgl/types.h>

typedef struct {
	int16_t advanceX;
	int16_t advanceY;
	int16_t bearingX;
	int16_t bearingY;
	uint16_t bitmapWidth;
	uint16_t bitmapHeight;
	uint32_t bitmapSize;
	const uint8_t * const bitmapData;
} EMGL_glyph_t;

typedef struct {
	uint32_t startCode;
	uint32_t endCode;
	char name[17];
	const EMGL_glyph_t * const * const glyphPtrs;
} EMGL_codePage_t;

typedef struct {
	int16_t ascender;
	uint8_t bpp;
	uint8_t numCodepages;
	const EMGL_codePage_t * const codePagePtrs;
} EMGL_font_t;

void emgl_drawTextA(const EMGL_font_t *font, const char *str, emgl_coord_t x, emgl_coord_t y, emgl_color_t color);
void emgl_drawTextU32(const EMGL_font_t *font, const U32 *str, emgl_coord_t x, emgl_coord_t y, emgl_color_t color);
const EMGL_codePage_t *emgl_getCodePage(const EMGL_font_t *font, U32 code);
const EMGL_glyph_t *emgl_getGlyph(const EMGL_font_t *font, U32 code);
emgl_color_t emgl_getColorFromPixelValue(U8 value, U8 bpp, emgl_color_t color);
void emgl_getDecompressedGlyphColoredBitmap(const U8 *in, U32 inSize, U8 inBpp, U16 width, U16 height,
	emgl_color_t color, emgl_color_t *out);
void emgl_getGlyphColoredBitmap(const U8 *in, U32 inSize, U8 inBpp, U16 width, U16 height,
	emgl_color_t color, emgl_color_t *out);
void emgl_drawGlyph(const EMGL_font_t *font, U32 glyph, emgl_coord_t *x, emgl_coord_t *y, emgl_color_t color);

#endif
