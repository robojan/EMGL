#ifndef _EMGL_MATH_H
#define _EMGL_MATH_H


#ifdef __cplusplus
extern "C" {
#endif

#include <emgl/types.h>

emgl_U8 emgl_lineSectionIntersection(emgl_coord_t x1, emgl_coord_t y1, emgl_coord_t x2, emgl_coord_t y2,
	emgl_coord_t x3, emgl_coord_t y3, emgl_coord_t x4, emgl_coord_t y4,
	emgl_coord_t *xOut, emgl_coord_t *yOut);


#ifdef __cplusplus
}
#endif
#endif