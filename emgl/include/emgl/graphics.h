#ifndef _EMGL_GRAPHICS_H_
#define _EMGL_GRAPHICS_H_

#include <emgl/color.h>
#include <emgl/types.h>

void emgl_clear(emgl_color_t clearColor);
void emgl_drawRectangle(coord_t x, coord_t y, coord_t width, coord_t height, emgl_color_t color);
void emgl_drawFilledRectangle(coord_t x, coord_t y, coord_t width, coord_t height, emgl_color_t color);
void emgl_drawCircle(coord_t x, coord_t y, coord_t r, emgl_color_t color);
void emgl_drawFilledCircle(coord_t x, coord_t y, coord_t r, emgl_color_t color);
void emgl_drawEllipse(coord_t x, coord_t y, coord_t width, coord_t height, emgl_color_t color);
void emgl_drawFilledEllipse(coord_t x, coord_t y, coord_t width, coord_t height, emgl_color_t color);
void emgl_drawArc(coord_t x, coord_t y, I16 startAngle, I16 stopAngle, emgl_color_t color);
void emgl_drawFilledArc(coord_t x, coord_t y, I16 startAngle, I16 stopAngle, emgl_color_t color);
void emgl_drawArcP(coord_t x, coord_t y, coord_t startx, coord_t starty, coord_t endx, coord_t endy, emgl_color_t color);
void emgl_drawFilledArcP(coord_t x, coord_t y, coord_t startx, coord_t starty, coord_t endx, coord_t endy, emgl_color_t color);
void emgl_drawRoundedRectangle(coord_t x, coord_t y, coord_t width, coord_t height, coord_t r, emgl_color_t color);
void emgl_drawFilledRoundedRectangle(coord_t x, coord_t y, coord_t width, coord_t height, coord_t r, emgl_color_t color);
void emgl_drawPolygon(coord_t *x, coord_t *y, U8 count, emgl_color_t color);
void emgl_drawFilledPolygon(coord_t *x, coord_t *y, U8 count, emgl_color_t color);
void emgl_drawBitmap(coord_t x, coord_t y, coord_t width, coord_t height, emgl_color_t *data);
void emgl_drawLine(coord_t x1, coord_t y1, coord_t x2, coord_t y2, emgl_color_t color);
void emgl_drawBezier(coord_t *x, coord_t *y, U8 count, emgl_color_t color);


#endif