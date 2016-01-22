#ifndef _EMGL_H_
#define _EMGL_H_

#include <emgl/emgl_config.h>
#include <emgl/types.h>
#include <emgl/color.h>
#include <emgl/graphics.h>

typedef struct {
	void *api;
	void(*poll)(void *api);
	emgl_color_t(*getPixel)(void *api, coord_t x, coord_t y);
	void(*setPixel)(void *api, coord_t x, coord_t y, emgl_color_t color);
	void(*fillRect)(void *api, coord_t x1, coord_t y1, coord_t x2, coord_t y2,
		emgl_color_t color);
	void(*drawLineH)(void *api, coord_t x1, coord_t x2, coord_t y,
		emgl_color_t color);
	void(*drawLineV)(void *api, coord_t y1, coord_t y2, coord_t x,
		emgl_color_t color);
	void(*drawBitmap)(void *api, coord_t x, coord_t y, coord_t width,
		coord_t height, const emgl_color_t *data);
	void(*init)(void *api);
	void(*getSize)(void *api, coord_t *width, coord_t *height);
} emgl_driverAPI_t;

void emgl_init();

void emgl_registerDriver(const emgl_driverAPI_t *driver);

#endif