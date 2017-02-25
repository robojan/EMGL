/*
 * emgl_ui.h
 *
 * Created: 25-2-2017 21:13:21
 *  Author: Robojan
 */ 


#ifndef EMGL_UI_H_
#define EMGL_UI_H_

#include <emgl/emgl.h>

typedef struct emgl_ui_element {
	emgl_coord_t x;
	emgl_coord_t y;
	emgl_coord_t width;
	emgl_coord_t height;
	int numChildren;
	struct emgl_ui_element *children;
	void (*update)(emgl_ui_element *self);
} emgl_ui_element_t;

typedef struct emgl_textbox {
	emgl_ui_element_t element;
	char *text;
} emgl_textbox_t;



#endif /* EMGL_UI_H_ */