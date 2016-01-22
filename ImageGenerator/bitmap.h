#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <color.h>

#ifndef __BITMAP_DATA_T_
#define __BITMAP_DATA_T_
typedef struct _bitmap_data {
	coord_t width;
	coord_t height;
	emgl_color_t const *const data;
} bitmap_data_t;
#endif

extern const bitmap_data_t bitmap_blackjack_wallpaper_by_brisineo-d62139e;
#endif
