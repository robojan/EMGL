#ifndef _PCDRIVER_H_
#define _PCDRIVER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <emgl/emgl.h>

#define LCD_WIDTH                 320
#define LCD_HEIGHT                240
#define PCDRV_ZOOM                2
#define PCDRV_WINDOW_TITLE        "EMGL - LCD"
#define PCDRV_MONO_FORE           0xFFEDE742
#define PCDRV_MONO_BACK           0xFF615F1B

const emgl_driverAPI_t *pcdrv_CreateDriver();

void pcdrv_init(void *api);
void pcdrv_poll(void *api);
void pcdrv_fillRect(void *api, emgl_coord_t x1, emgl_coord_t y1, emgl_coord_t x2,
		emgl_coord_t y2, emgl_color_t lcdColor);
void pcdrv_drawLineH(void *api, emgl_coord_t x1, emgl_coord_t x2, emgl_coord_t y,
		emgl_color_t lcdColor);
void pcdrv_drawLineV(void *api, emgl_coord_t y1, emgl_coord_t y2, emgl_coord_t x,
		emgl_color_t lcdColor);
emgl_color_t pcdrv_getPixel(void *api, emgl_coord_t x, emgl_coord_t y);
void pcdrv_setPixel(void *api, emgl_coord_t x, emgl_coord_t y, emgl_color_t lcdColor);
void pcdrv_drawBitmap(void *api, emgl_coord_t x, emgl_coord_t y, emgl_coord_t width,
	emgl_coord_t height, const emgl_color_t *data);
void pcdrv_getSize(void *api, emgl_coord_t *width, emgl_coord_t *height);

#ifdef __cplusplus
}
#endif
#endif
