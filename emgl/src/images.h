#ifndef _IMAGES_H_
#define _IMAGES_H_

#include  <emgl/color.h>

#ifndef __BITMAP_DATA_T_
#define __BITMAP_DATA_T_
typedef struct _bitmap_data {
	coord_t width;
	coord_t height;
	emgl_color_t const *const data;
} bitmap_data_t;
#endif

extern const bitmap_data_t images_colors;
extern const bitmap_data_t images_parrot;
#endif
