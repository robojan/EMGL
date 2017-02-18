#ifndef _EMGL_GRAPHICS_H_
#define _EMGL_GRAPHICS_H_

#include <emgl/color.h>
#include <emgl/types.h>

void emgl_clear(emgl_color_t clearColor);
void emgl_drawRectangle(emgl_coord_t x, emgl_coord_t y, emgl_coord_t width, emgl_coord_t height, emgl_color_t color);
void emgl_drawFilledRectangle(emgl_coord_t x, emgl_coord_t y, emgl_coord_t width, emgl_coord_t height, emgl_color_t color);
void emgl_drawCircle(emgl_coord_t x, emgl_coord_t y, emgl_coord_t r, emgl_color_t color);
void emgl_drawFilledCircle(emgl_coord_t x, emgl_coord_t y, emgl_coord_t r, emgl_color_t color);
void emgl_drawEllipse(emgl_coord_t x, emgl_coord_t y, emgl_coord_t width, emgl_coord_t height, emgl_color_t color);
void emgl_drawFilledEllipse(emgl_coord_t x, emgl_coord_t y, emgl_coord_t width, emgl_coord_t height, emgl_color_t color);
void emgl_drawArc(emgl_coord_t x, emgl_coord_t y, emgl_I16 startAngle, emgl_I16 stopAngle, emgl_color_t color);
void emgl_drawFilledArc(emgl_coord_t x, emgl_coord_t y, emgl_I16 startAngle, emgl_I16 stopAngle, emgl_color_t color);
void emgl_drawArcP(emgl_coord_t x, emgl_coord_t y, emgl_coord_t startx, emgl_coord_t starty, emgl_coord_t endx, emgl_coord_t endy, emgl_color_t color);
void emgl_drawFilledArcP(emgl_coord_t x, emgl_coord_t y, emgl_coord_t startx, emgl_coord_t starty, emgl_coord_t endx, emgl_coord_t endy, emgl_color_t color);
void emgl_drawRoundedRectangle(emgl_coord_t x, emgl_coord_t y, emgl_coord_t width, emgl_coord_t height, emgl_coord_t r, emgl_color_t color);
void emgl_drawFilledRoundedRectangle(emgl_coord_t x, emgl_coord_t y, emgl_coord_t width, emgl_coord_t height, emgl_coord_t r, emgl_color_t color);
void emgl_drawPolygon(emgl_coord_t *x, emgl_coord_t *y, emgl_U8 count, emgl_color_t color);
void emgl_drawFilledPolygon(emgl_coord_t *x, emgl_coord_t *y, emgl_U8 count, emgl_color_t color);
void emgl_drawBitmap(emgl_coord_t x, emgl_coord_t y, emgl_coord_t width, emgl_coord_t height, emgl_color_t *data);
void emgl_drawLine(emgl_coord_t x1, emgl_coord_t y1, emgl_coord_t x2, emgl_coord_t y2, emgl_color_t color);
void emgl_drawBezier(emgl_coord_t *x, emgl_coord_t *y, emgl_U8 count, emgl_color_t color);


#endif