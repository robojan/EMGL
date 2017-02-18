#ifndef _EMGL_H_
#define _EMGL_H_

#include <emgl/emgl_config.h>
#include <emgl/types.h>
#include <emgl/color.h>
#include <emgl/graphics.h>
#include <emgl/font.h>

typedef struct {
	void *(*malloc)(emgl_U32 size);
	void *(*calloc)(emgl_U32 num, emgl_U32 size);
	void (*free)(void *ptr);
	void (*log)(const char *, ...);
	void (*fatal)(const char *, ...);
} emgl_support_t;

typedef struct {
	void *api;
	void(*poll)(void *api);
	emgl_color_t(*getPixel)(void *api, emgl_coord_t x, emgl_coord_t y);
	void(*setPixel)(void *api, emgl_coord_t x, emgl_coord_t y, emgl_color_t color);
	void(*fillRect)(void *api, emgl_coord_t x1, emgl_coord_t y1, emgl_coord_t x2, emgl_coord_t y2,
		emgl_color_t color);
	void(*drawLineH)(void *api, emgl_coord_t x1, emgl_coord_t x2, emgl_coord_t y,
		emgl_color_t color);
	void(*drawLineV)(void *api, emgl_coord_t y1, emgl_coord_t y2, emgl_coord_t x,
		emgl_color_t color);
	void(*drawBitmap)(void *api, emgl_coord_t x, emgl_coord_t y, emgl_coord_t width,
		emgl_coord_t height, const emgl_color_t *data);
	void(*init)(void *api);
	void(*getSize)(void *api, emgl_coord_t *width, emgl_coord_t *height);
} emgl_driverAPI_t;

void emgl_init(const emgl_support_t *support);

void emgl_registerDriver(const emgl_driverAPI_t *driver);

#endif