#include <emgl/math.h>

U8 emgl_lineSectionIntersection(emgl_coord_t x1, emgl_coord_t y1, emgl_coord_t x2, emgl_coord_t y2, 
	emgl_coord_t x3, emgl_coord_t y3, emgl_coord_t x4, emgl_coord_t y4, 
	emgl_coord_t *xOut, emgl_coord_t *yOut)
{
	emgl_coord_t A1 = y2 - y1;
	emgl_coord_t B1 = x1 - x2;
	emgl_coord_t C1 = A1*x1 + B1*y1;
	emgl_coord_t A2 = y4 - y3;
	emgl_coord_t B2 = x3 - x4;
	emgl_coord_t C2 = A2*x3 + B2*y3;
	emgl_coord_t det = A1 * B2 - A2 * B1;
	if (det == 0) {
		// parallel lines
		return 0;
	}
	else {
		emgl_coord_t x = (B2*C1 - B1*C2) / det;
		emgl_coord_t y = (A1*C2 - A2*C1) / det;
		emgl_coord_t minx = x1 < x2 ? x1 : x2;
		emgl_coord_t maxx = x1 < x2 ? x2 : x1;
		emgl_coord_t miny = y1 < y2 ? y1 : y2;
		emgl_coord_t maxy = y1 < y2 ? y2 : y1;
		if (minx <= x && x <= maxx && miny <= y && y <= maxy) {
			if (xOut != NULL) *xOut = x;
			if (yOut != NULL) *yOut = y;
			return 1;
		}
		else {
			// No intersection in segment
			return 0;
		}
	}
}
