#ifndef _EMGL_COLOR_H_
#define _EMGL_COLOR_H_

#include <emgl/emgl_config.h>
#include <emgl/colormode.h>
#include <emgl/types.h>


/*
 * Define the color converter macros
 * and types
 * For color to gray scale approximately the ratios 0.2 0.7 0.1 are used
 * for RGB.
 */
#if EMGL_INTERNALCOLORMODE == CM_1BPP
#define EMGL_COLOR_BPP (1)
typedef emgl_U8 emgl_color_t;
#define emgl_colorBlend emgl_colorBlend1BPP

#define emgl_colorModeGetPixel(data, n) (((data)[(n)/8] >> ((n) & 7)) & 0x1)
#define emgl_colorModeSetPixel(data, n, value) do { (data)[(n)/8] = ((data)[(n)/8] & ~(1 << ((n) & 7))) | (((value) & 1) << ((n) & 7));} while(0)

#define emgl_colorConvFrom1BPP(x) (x)
#define emgl_colorConvFrom2BPP(x) ((x)>>1)
#define emgl_colorConvFrom8BPP(x) ((x)>>7)
#define emgl_colorConvFromRGB111(x) (((((x) & 4) != 0 ? 3 : 0) + \
		(((x) & 2) != 0 ? 11 : 0) + (((x) & 1) != 0 ? 1 : 0))>>3)
#define emgl_colorConvFromRGB222(x) ((((((x)>>4) & 3) * 2) + \
		((((x)>>2) & 3) * 7) + (((x) & 3) * 1))>>4)
#define emgl_colorConvFromRGB332(x) ((((((x)>>5) & 7) * 2) + \
		((((x)>>2) & 7) * 6) + (((x) & 3) * 1))>>5)
#define emgl_colorConvFromRGB565(x) ((((((x)>>11) & 31) * 1) + \
		((((x)>>5) & 63) * 3) + (((x) & 31) * 1))>>7)
#define emgl_colorConvFromRGBA5551(x) ((((((x)>>10) & 31) * 1) + \
		((((x)>>5) & 31) * 6) + (((x) & 31) * 1))>>7)
#define emgl_colorConvFromRGBA8888(x) ((((((x)>>18) & 63) * 1) + \
		((((x)>>9) & 127) * 1) + (((x) & 63) * 1))>>7)
#define emgl_colorConvTo1BPP(x)   (x)
#define emgl_colorConvTo2BPP(x)   (((x) & 1) != 0 ? 0x3 : 0)
#define emgl_colorConvTo8BPP(x)   (((x) & 1) != 0 ? 0xFF : 0)
#define emgl_colorConvToRGB111(x)   (((x) & 1) != 0 ? 0x7 : 0)
#define emgl_colorConvToRGB222(x)   (((x) & 1) != 0 ? 0x3F : 0)
#define emgl_colorConvToRGB332(x)   (((x) & 1) != 0 ? 0xFF : 0)
#define emgl_colorConvToRGB565(x)   (((x) & 1) != 0 ? 0xFFFF : 0)
#define emgl_colorConvToRGBA5551(x)   (((x) & 1) != 0 ? 0xFFFF : 0)
#define emgl_colorConvToRGBA8888(x)   (((x) & 1) != 0 ? 0xFFFFFFFF : 0)
#elif EMGL_INTERNALCOLORMODE == CM_8BPP
#define EMGL_COLOR_BPP (8)
typedef emgl_U8 emgl_color_t;
#define emgl_colorBlend emgl_colorBlend8BPP

#define emgl_colorModeGetPixel(data, n) ((data)[n])
#define emgl_colorModeSetPixel(data, n, value) do { (data)[n] = (value);} while(0)

#define emgl_colorConvFrom1BPP(x) ((x)<<7)
#define emgl_colorConvFrom2BPP(x) ((x)<<6)
#define emgl_colorConvFrom8BPP(x) (x)
#define emgl_colorConvFromRGB111(x) (((((x) & 4) != 0 ? 54 : 0) + \
		(((x) & 2) != 0 ? 181 : 0) + (((x) & 1) != 0 ? 18 : 0)))
#define emgl_colorConvFromRGB222(x) ((((((x)>>4) & 3) * 18) + \
		((((x)>>2) & 3) * 60) + (((x) & 3) * 6)))
#define emgl_colorConvFromRGB332(x) ((((((x)>>5) & 7) * 7) + \
		((((x)>>2) & 7) * 26) + (((x) & 3) * 3)))
#define emgl_colorConvFromRGB565(x) ((((((x)>>11) & 31) * 1) + \
		((((x)>>5) & 63) * 3) + (((x) & 31) * 1)))
#define emgl_colorConvFromRGBA5551(x) ((((((x)>>10) & 31) * 1) + \
		((((x)>>5) & 31) * 6) + (((x) & 31) * 1)))
#define emgl_colorConvFromRGBA8888(x) ((((((x)>>18) & 63) * 1) + \
		((((x)>>9) & 127) * 1) + (((x) & 63) * 1)))
#define emgl_colorConvTo1BPP(x)   ((x)>>7)
#define emgl_colorConvTo2BPP(x)   ((x)>>6)
#define emgl_colorConvTo8BPP(x)   (x)
#define emgl_colorConvToRGB111(x)   (((x) & 0x80) != 0 ? 0 : 0x7)
#define emgl_colorConvToRGB222(x)   (((x) >> 6) | (((x) >> 4)&0xC) | \
		(((x) >> 2) &0x30))
#define emgl_colorConvToRGB332(x)   ((((x) >> 6) & 0x3) | (((x) >> 3)&0x1C) | \
		((x) & 0xE0))
#define emgl_colorConvToRGB565(x)   ((((x) >> 3) & 0x1F) | \
		(((x) << 3)&0x7E0) | (((x) << 8) & 0xF800))
#define emgl_colorConvToRGBA5551(x) ((((x) >> 3) & 0x1F) | \
		(((x) << 3)&0x3E0) | (((x) << 7) & 0x7C00) | 0x8000)
#define emgl_colorConvToRGBA8888(x) (((x)&0xFF) | (((x)<<8)&0xFF00) | \
		(((x)<<16)&0xFF0000)| 0xFF000000) 

#elif EMGL_INTERNALCOLORMODE == CM_RGB332
#define EMGL_COLOR_BPP (8)
typedef emgl_U8 emgl_color_t;
#define emgl_colorBlend emgl_colorBlendRGB332

#define emgl_colorModeGetPixel(data, n) ((data)[n])
#define emgl_colorModeSetPixel(data, n, value) do { (data)[n] = (value);} while(0)

#define emgl_colorConvFrom1BPP(x) (((x) & 1) != 0 ? 0xFF : 0)
#define emgl_colorConvFrom2BPP(x) (((x) & 3) | (((x) & 3) << 3) | \
		(((x) & 3) << 6))
#define emgl_colorConvFrom8BPP(x) ((((x) >> 6) & 0x3) | (((x) >> 3)&0x1C) | \
		((x) & 0xE0))
#define emgl_colorConvFromRGB111(x) ((((x) & 1) != 0 ? 0x3 : 0) |\
		(((x) & 2) != 0 ? 0x1C : 0) | (((x) & 4) != 0 ? 0xE0 : 0))
#define emgl_colorConvFromRGB222(x) ((((x) & 3)) | (((x) & 0xC)<<1) |\
		(((x) & 0x30)<<1))
#define emgl_colorConvFromRGB332(x) (x)
#define emgl_colorConvFromRGB565(x) ((((x)>>3)&3) | (((x)>>6)&0x1C) | \
		(((x)>>8)&0xE0))
#define emgl_colorConvFromRGBA5551(x) ((((x)>>3)&3) | (((x)>>6)&0x1C) | \
		(((x)>>7)&0xE0))
#define emgl_colorConvFromRGBA8888(x) ((((x)>>6)&3) | (((x)>>11)&0x1C) | \
		(((x)>>16)&0xE0))
#define emgl_colorConvTo1BPP(x)   ((((((x)>>5) & 7) * 2) + \
		((((x)>>2) & 7) * 6) + (((x) & 3) * 1))>>5)
#define emgl_colorConvTo2BPP(x)   ((((((x)>>5) & 7) * 2) + \
		((((x)>>2) & 7) * 6) + (((x) & 3) * 1))>>4)
#define emgl_colorConvTo8BPP(x)   ((((((x)>>5) & 7) * 7) + \
		((((x)>>2) & 7) * 26) + (((x) & 3) * 3)))
#define emgl_colorConvToRGB111(x)   ((((x)>>1)&1)|(((x)>>4)&2)|(((x)>>5)&4))
#define emgl_colorConvToRGB222(x)   (((x)&3) | (((x)>>1)&0xC) | (((x)>>2)&0x30))
#define emgl_colorConvToRGB332(x)   (x)
#define emgl_colorConvToRGB565(x)   ((((x)<<3)&0x18) | (((x)<<6)&0x700) |\
		(((x)<<8)&0xE000))
#define emgl_colorConvToRGBA5551(x) ((((x)<<3)&0x18) | (((x)<<5)&0x380) |\
		(((x)<<7)&0x7000) | 0x8000)
#define emgl_colorConvToRGBA8888(x) ((((x)<<6)&0xC0) | (((x)<<11) & 0xE000) \
		| (((x)<<16)&0xE00000) | 0xFF000000) 

#elif EMGL_INTERNALCOLORMODE == CM_RGB565
#define EMGL_COLOR_BPP (16)
typedef emgl_U16 emgl_color_t;
#define emgl_colorBlend emgl_colorBlendRGB565

#define emgl_colorModeGetPixel(data, n) ((data)[n])
#define emgl_colorModeSetPixel(data, n, value) do { (data)[n] = (value);} while(0)

#define emgl_colorConvFrom1BPP(x) (((x) & 1) != 0 ? 0xFFFF : 0)
#define emgl_colorConvFrom2BPP(x) ((((x) & 3)<<3) | (((x) & 3) << 9) | \
		(((x) & 3) << 14))
#define emgl_colorConvFrom8BPP(x) ((((x) >> 3) & 0x1F) | \
		(((x) << 3)&0x7E0) | (((x) << 8) & 0xF800))
#define emgl_colorConvFromRGB111(x) ((((x) & 1) != 0 ? 0x1F : 0) |\
		(((x) & 2) != 0 ? 0x07E0 : 0) | (((x) & 4) != 0 ? 0xF800 : 0))
#define emgl_colorConvFromRGB222(x) ((((x) & 3)<<3) | (((x) & 0xC)<<7) |\
		(((x) & 0x30)<<10))
#define emgl_colorConvFromRGB332(x) ((((x) & 3)<<3) | (((x) & 0x1C)<<6) |\
		(((x) & 0xE0)<<8))
#define emgl_colorConvFromRGB565(x) (x)
#define emgl_colorConvFromRGBA5551(x) (((x)&0x1F) | (((x)<<1)&0x7C0) | \
		(((x)<<1)&0xF000))
#define emgl_colorConvFromRGBA8888(x) ((((x)>>3)&0x1F) | (((x)>>5)&0x7C0) | \
		(((x)>>8)&0xF000))
#define emgl_colorConvTo1BPP(x)   ((((((x)>>11) & 31) * 1) + \
		((((x)>>5) & 63) * 3) + (((x) & 31) * 1))>>7)
#define emgl_colorConvTo2BPP(x)   ((((((x)>>11) & 31) * 1) + \
		((((x)>>5) & 63) * 3) + (((x) & 31) * 1))>>6)
#define emgl_colorConvTo8BPP(x)   ((((((x)>>11) & 31) * 1) + \
		((((x)>>5) & 63) * 3) + (((x) & 31) * 1)))
#define emgl_colorConvToRGB111(x)   ((((x)>>4)&1)|(((x)>>9)&2)|(((x)>>13)&4))
#define emgl_colorConvToRGB222(x)   ((((x)>>3)&0x3)|(((x)>>7)&0xC)| \
		(((x)>>10)&0x30))
#define emgl_colorConvToRGB332(x)   ((((x)>>3)&3) | (((x)>>6)&0x1C) | \
		(((x)>>8)&0xE0))
#define emgl_colorConvToRGB565(x)   (x)
#define emgl_colorConvToRGBA5551(x) (((x)&0x1F) | (((x)>>1)&0x3E0) |\
		(((x)>>1)&0x7C00) | 0x8000)
#define emgl_colorConvToRGBA8888(x) ((((x)<<3)&0xF8) | (((x)<<5) & 0xFC00) \
		| (((x)<<8)&0xF80000) | 0xFF000000) 

#elif EMGL_INTERNALCOLORMODE == CM_RGBA5551
#define EMGL_COLOR_BPP (16)
typedef U16 emgl_color_t;
#define emgl_colorBlend emgl_colorBlendRGBA5551

#define emgl_colorModeGetPixel(data, n) ((data)[n])
#define emgl_colorModeSetPixel(data, n, value) do { (data)[n] = (value);} while(0)

#define emgl_colorConvFrom1BPP(x) (((x) & 1) != 0 ? 0xFFFF : 0)
#define emgl_colorConvFrom2BPP(x) ((((x) & 3)<<3) | (((x) & 3) << 8) | \
		(((x) & 3) << 13) | 0x8000)
#define emgl_colorConvFrom8BPP(x) ((((x) >> 3) & 0x1F) | \
		(((x) << 2)&0x3E0) | (((x) << 7) & 0x8C00) | 0x8000)
#define emgl_colorConvFromRGB111(x) ((((x) & 1) != 0 ? 0x1F : 0) |\
		(((x) & 2) != 0 ? 0x03E0 : 0) | (((x) & 4) != 0 ? 0xFC00 : 0x8000))
#define emgl_colorConvFromRGB222(x) ((((x) & 3)<<3) | (((x) & 0xC)<<6) |\
		(((x) & 0x30)<<9) | 0x8000)
#define emgl_colorConvFromRGB332(x) ((((x) & 3)<<3) | (((x) & 0x1C)<<5) |\
		(((x) & 0xE0)<<7) | 0x8000)
#define emgl_colorConvFromRGB565(x) (((x)&0x1F) | (((x)>>1)&0x3E0) |\
		(((x)>>1)&0x7C00) | 0x8000)
#define emgl_colorConvFromRGBA5551(x) (x)
#define emgl_colorConvFromRGBA8888(x) ((((x)>>3)&0x1F) | (((x)>>6)&0x3C0) | \
		(((x)>>9)&0x7C00) | 0x8000)
#define emgl_colorConvTo1BPP(x)   ((((((x)>>10) & 31) * 1) + \
		((((x)>>5) & 31) * 6) + (((x) & 31) * 1))>>7)
#define emgl_colorConvTo2BPP(x)   ((((((x)>>10) & 31) * 1) + \
		((((x)>>5) & 31) * 6) + (((x) & 31) * 1))>>6)
#define emgl_colorConvTo8BPP(x)   ((((((x)>>10) & 31) * 1) + \
		((((x)>>5) & 31) * 6) + (((x) & 31) * 1)))
#define emgl_colorConvToRGB111(x)   ((((x)>>4)&1)|(((x)>>8)&2)|(((x)>>12)&4))
#define emgl_colorConvToRGB222(x)   ((((x)>>3)&0x3)|(((x)>>6)&0xC)| \
		(((x)>>9)&0x30))
#define emgl_colorConvToRGB332(x)   ((((x)>>3)&3) | (((x)>>5)&0x1C) | \
		(((x)>>7)&0xE0))
#define emgl_colorConvToRGB565(x)   (((x)&0x1F) | (((x)<<1)&0x7C0) | \
		(((x)<<1)&0xF000))
#define emgl_colorConvToRGBA5551(x) (x)
#define emgl_colorConvToRGBA8888(x) ((((x)<<3)&0xF8) | (((x)<<6) & 0xF800) \
		| (((x)<<9)&0xF80000) | (((x)&0x8000)!= 0 ? 0xFF000000: 0)) 

#elif EMGL_INTERNALCOLORMODE == CM_RGBA8888
#define EMGL_COLOR_BPP (32)
typedef emgl_U32 emgl_color_t;
#define emgl_colorBlend emgl_colorBlendRGBA8888

#define emgl_colorModeGetPixel(data, n) ((data)[n])
#define emgl_colorModeSetPixel(data, n, value) do { (data)[n] = (value);} while(0)

#define emgl_colorConvFrom1BPP(x) (((x) & 1) != 0 ? 0xFFFFFFFF : 0)
#define emgl_colorConvFrom2BPP(x) ((((x) & 3)<<6) | (((x) & 3) << 14) | \
		(((x) & 3) << 22) | 0xFF000000)
#define emgl_colorConvFrom8BPP(x) (((x)&0xFF) | (((x)<<8)&0xFF00) | \
		(((x)<<16)&0xFF0000)| 0xFF000000) 
#define emgl_colorConvFromRGB111(x) ((((x) & 1) != 0 ? 0xFF : 0) |\
		(((x) & 2) != 0 ? 0xFF00 : 0) | (((x) & 4) != 0 ? 0xFFFF0000 : 0xFF000000))
#define emgl_colorConvFromRGB222(x) ((((x) & 3)<<6) | (((x) & 0xC)<<12) |\
		(((x) & 0x30)<<18) | 0xFF000000)
#define emgl_colorConvFromRGB332(x) ((((x)<<6)&0xC0) | (((x)<<11) & 0xE000) \
		| (((x)<<16)&0xE00000) | 0xFF000000) 
#define emgl_colorConvFromRGB565(x) ((((x)<<3)&0xF8) | (((x)<<3) & 0xFC00) \
		| (((x)<<8)&0xF80000) | 0xFF000000) 
#define emgl_colorConvFromRGBA5551(x) ((((x)<<3)&0xF8) | (((x)<<6) & 0xF800) \
		| (((x)<<9)&0xF80000) | (((x)&0x8000)!= 0 ? 0xFF000000: 0)) 
#define emgl_colorConvFromRGBA8888(x) (x)
#define emgl_colorConvTo1BPP(x)   ((((((x)>>18) & 63) * 1) + \
		((((x)>>9) & 127) * 1) + (((x) & 63) * 1))>>7)
#define emgl_colorConvTo2BPP(x)   ((((((x)>>18) & 63) * 1) + \
		((((x)>>9) & 127) * 1) + (((x) & 63) * 1))>>6)
#define emgl_colorConvTo8BPP(x)   ((((((x)>>18) & 63) * 1) + \
		((((x)>>9) & 127) * 1) + (((x) & 63) * 1)))
#define emgl_colorConvToRGB111(x)   ((((x)>>7)&1)|(((x)>>14)&2)|(((x)>>21)&4))
#define emgl_colorConvToRGB222(x)   ((((x)>>6)&0x3)|(((x)>>12)&0xC)| \
		(((x)>>18)&0x30))
#define emgl_colorConvToRGB332(x)   ((((x)>>6)&3) | (((x)>>11)&0x1C) | \
		(((x)>>16)&0xE0))
#define emgl_colorConvToRGB565(x)   ((((x)>>3)&0x1F) | (((x)>>5)&0x7C0) | \
		(((x)>>8)&0xF000))
#define emgl_colorConvToRGBA5551(x) ((((x)>>3)&0x1F) | (((x)>>6)&0x3C0) | \
		(((x)>>9)&0x7C00) | 0x8000)
#define emgl_colorConvToRGBA8888(x) (x)

#else
#error Unknown internal color mode
#endif

/*
* Color blending
*/
emgl_color_t emgl_colorBlend1BPP(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix);
emgl_color_t emgl_colorBlend8BPP(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix);
emgl_color_t emgl_colorBlendRGB332(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix);
emgl_color_t emgl_colorBlendRGB565(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix);
emgl_color_t emgl_colorBlendRGBA5551(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix);
emgl_color_t emgl_colorBlendRGBA8888(emgl_color_t color1, emgl_color_t color2, emgl_U8 mix);


#define COLOR_BLACK           emgl_colorConvFromRGBA8888(0xFF000000)
#define COLOR_SILVER          emgl_colorConvFromRGBA8888(0xFFC0C0C0)
#define COLOR_GRAY            emgl_colorConvFromRGBA8888(0xFF808080)
#define COLOR_WHITE           emgl_colorConvFromRGBA8888(0xFFFFFFFF)
#define COLOR_MAROON          emgl_colorConvFromRGBA8888(0xFF800000)
#define COLOR_RED             emgl_colorConvFromRGBA8888(0xFFFF0000)
#define COLOR_PURPLE          emgl_colorConvFromRGBA8888(0xFF800080)
#define COLOR_FUCHSIA         emgl_colorConvFromRGBA8888(0xFFFF00FF)
#define COLOR_GREEN           emgl_colorConvFromRGBA8888(0xFF008000)
#define COLOR_LIME            emgl_colorConvFromRGBA8888(0xFF00FF00)
#define COLOR_OLIVE           emgl_colorConvFromRGBA8888(0xFF808000)
#define COLOR_YELLOW          emgl_colorConvFromRGBA8888(0xFFFFFF00)
#define COLOR_NAVY            emgl_colorConvFromRGBA8888(0xFF000080)
#define COLOR_BLUE            emgl_colorConvFromRGBA8888(0xFF0000FF)
#define COLOR_TEAL            emgl_colorConvFromRGBA8888(0xFF008080)
#define COLOR_AQUA            emgl_colorConvFromRGBA8888(0xFF00FFFF)



#endif