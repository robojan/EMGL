#ifndef _FONT_ARIAL12_H_
#define _FONT_ARIAL12_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Choose between local declaration or remote declaration 
#include <emgl/font.h>
/*
typedef struct {
	int16_t advanceX;
	int16_t advanceY;
	int16_t bitmapLeft;
	int16_t bitmapTop;
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
*/

extern const EMGL_font_t font_arial12;

#ifdef __cplusplus
}
#endif

#endif
