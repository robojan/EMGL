#ifndef __H_
#define __H_

#include <color.h>

#ifndef __BITMAP_DATA_T_
#define __BITMAP_DATA_T_
typedef struct _bitmap_data {
	coord_t width;
	coord_t height;
	emgl_color_t const *const data;
} bitmap_data_t;
#endif

extern const bitmap_data_t _colors;
extern const bitmap_data_t _parrot;
#endif
