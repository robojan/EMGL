#ifndef _EMGL_FONT_H_
#define _EMGL_FONT_H_


#ifdef __cplusplus
extern "C" {
#endif

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

void emgl_drawTextA(const EMGL_font_t *font, const char *str, emgl_coord_t x, emgl_coord_t y, 
	emgl_color_t foregroundColor, emgl_color_t backgroundColor);
void emgl_drawTextU32(const EMGL_font_t *font, const emgl_U32 *str, emgl_coord_t x, emgl_coord_t y, 
	emgl_color_t foregroundColor, emgl_color_t backgroundColor);
const EMGL_codePage_t *emgl_getCodePage(const EMGL_font_t *font, emgl_U32 code);
const EMGL_glyph_t *emgl_getGlyph(const EMGL_font_t *font, emgl_U32 code);
emgl_color_t emgl_getColorFromPixelValue(emgl_U8 value, emgl_U8 bpp, emgl_color_t foregroundColor, 
	emgl_color_t backgroundColor);
void emgl_getDecompressedGlyphColoredBitmap(const emgl_U8 *in, emgl_U32 inSize, emgl_U8 inBpp, emgl_U16 width, emgl_U16 height,
	emgl_color_t foregroundColor, emgl_color_t backgroundColor, emgl_color_t *out);
void emgl_getGlyphColoredBitmap(const emgl_U8 *in, emgl_U32 inSize, emgl_U8 inBpp, emgl_U16 width, emgl_U16 height,
	emgl_color_t foregroundColor, emgl_color_t backgroundColor, emgl_color_t *out);
void emgl_drawGlyph(const EMGL_font_t *font, emgl_U32 glyph, emgl_coord_t *x, emgl_coord_t *y, 
	emgl_color_t foregroundColor, emgl_color_t backgroundColor);


#ifdef __cplusplus
}
#endif

#endif
