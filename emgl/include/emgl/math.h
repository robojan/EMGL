#ifndef _EMGL_MATH_H
#define _EMGL_MATH_H

#include <emgl/types.h>

U8 emgl_lineSectionIntersection(emgl_coord_t x1, emgl_coord_t y1, emgl_coord_t x2, emgl_coord_t y2,
	emgl_coord_t x3, emgl_coord_t y3, emgl_coord_t x4, emgl_coord_t y4,
	emgl_coord_t *xOut, emgl_coord_t *yOut);

#endif