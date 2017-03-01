#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <emgl/emgl.h>
#include <emgl/emgl_ui.h>
#include "pcdriver.h"

#include "resources/images.h"
#include "resources/arial12.h"
#include "resources/arial8.h"
#include "resources/segoescript10.h"
#include "resources/Sylfaen12.h"

const EMGL_font_t *emgl_default_font_ptr = &font_Sylfaen12;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	emgl_support_t support;
	support.malloc = malloc;
	support.calloc = calloc;
	support.free = free;
	support.log = (void(__cdecl *)(const char *, ...))printf;
	support.fatal = NULL;

	emgl_init(&support);
	const emgl_driverAPI_t *drv = pcdrv_CreateDriver();
	emgl_registerDriver(drv);
	emgl_clear(COLOR_WHITE);
	emgl_drawRectangle(00, 230, 10, 10, COLOR_BLACK);
	emgl_drawRectangle(00, 220, 10, 10, COLOR_MAROON);
	emgl_drawRectangle(00, 210, 10, 10, COLOR_GREEN);
	emgl_drawRectangle(00, 200, 10, 10, COLOR_NAVY);
	emgl_drawRectangle(10, 230, 10, 10, COLOR_SILVER);
	emgl_drawRectangle(10, 220, 10, 10, COLOR_RED);
	emgl_drawRectangle(10, 210, 10, 10, COLOR_LIME);
	emgl_drawRectangle(10, 200, 10, 10, COLOR_BLUE);
	emgl_drawRectangle(20, 230, 10, 10, COLOR_GRAY);
	emgl_drawRectangle(20, 220, 10, 10, COLOR_PURPLE);
	emgl_drawRectangle(20, 210, 10, 10, COLOR_OLIVE);
	emgl_drawRectangle(20, 200, 10, 10, COLOR_TEAL);
	emgl_drawRectangle(30, 230, 10, 10, COLOR_WHITE);
	emgl_drawRectangle(30, 220, 10, 10, COLOR_FUCHSIA);
	emgl_drawRectangle(30, 210, 10, 10, COLOR_YELLOW);
	emgl_drawRectangle(30, 200, 10, 10, COLOR_AQUA);

	emgl_drawFilledRectangle(50, 230, 10, 10, COLOR_BLACK);
	emgl_drawFilledRectangle(50, 220, 10, 10, COLOR_MAROON);
	emgl_drawFilledRectangle(50, 210, 10, 10, COLOR_GREEN);
	emgl_drawFilledRectangle(50, 200, 10, 10, COLOR_NAVY);
	emgl_drawFilledRectangle(60, 230, 10, 10, COLOR_SILVER);
	emgl_drawFilledRectangle(60, 220, 10, 10, COLOR_RED);
	emgl_drawFilledRectangle(60, 210, 10, 10, COLOR_LIME);
	emgl_drawFilledRectangle(60, 200, 10, 10, COLOR_BLUE);
	emgl_drawFilledRectangle(70, 230, 10, 10, COLOR_GRAY);
	emgl_drawFilledRectangle(70, 220, 10, 10, COLOR_PURPLE);
	emgl_drawFilledRectangle(70, 210, 10, 10, COLOR_OLIVE);
	emgl_drawFilledRectangle(70, 200, 10, 10, COLOR_TEAL);
	emgl_drawFilledRectangle(80, 230, 10, 10, COLOR_WHITE);
	emgl_drawFilledRectangle(80, 220, 10, 10, COLOR_FUCHSIA);
	emgl_drawFilledRectangle(80, 210, 10, 10, COLOR_YELLOW);
	emgl_drawFilledRectangle(80, 200, 10, 10, COLOR_AQUA);

	emgl_drawCircle(115, 220, 19, COLOR_OLIVE);
	emgl_drawCircle(120, 220, 19, COLOR_TEAL);
	emgl_drawCircle(125, 220, 19, COLOR_FUCHSIA);
	emgl_drawCircle(130, 220, 19, COLOR_AQUA);

	emgl_drawFilledCircle(175, 220, 19, COLOR_AQUA);
	emgl_drawFilledCircle(180, 220, 19, COLOR_FUCHSIA);
	emgl_drawFilledCircle(185, 220, 19, COLOR_TEAL);
	emgl_drawFilledCircle(190, 220, 19, COLOR_OLIVE);

	emgl_drawEllipse(230, 220, 33, 38, COLOR_OLIVE);
	emgl_drawEllipse(232, 220, 38, 38, COLOR_TEAL);
	emgl_drawEllipse(234, 220, 43, 38, COLOR_FUCHSIA);
	emgl_drawEllipse(237, 220, 48, 38, COLOR_AQUA);

	emgl_drawFilledEllipse(290, 220, 50, 38, COLOR_AQUA);
	emgl_drawFilledEllipse(290, 220, 40, 38, COLOR_FUCHSIA);
	emgl_drawFilledEllipse(290, 220, 30, 38, COLOR_TEAL);
	emgl_drawFilledEllipse(290, 220, 20, 38, COLOR_OLIVE);

	emgl_drawRoundedRectangle(0, 170, 20, 20, 5, COLOR_BLACK);
	emgl_drawRoundedRectangle(20, 170, 20, 20, 5, COLOR_MAROON);
	emgl_drawFilledRoundedRectangle(40, 170, 20, 20, 7, COLOR_GREEN);
	emgl_drawFilledRoundedRectangle(60, 170, 20, 20, 7, COLOR_NAVY);	

	emgl_coord_t x [] = {80, 110, 115, 100};
	emgl_coord_t y [] = {170, 180, 185, 190};

	emgl_drawPolygon(x, y, 4, COLOR_GREEN);
	x[0] += 30;
	x[1] += 30;
	x[2] += 30;
	x[3] += 30;
	emgl_drawFilledPolygon(x, y, 4, COLOR_GREEN);
	x[0] = 150;
	x[1] = 175;
	x[2] = 200;
	x[3] = 175;
	y[0] = 170;
	y[1] = 190;
	y[2] = 170;
	y[3] = 180;
	emgl_drawFilledPolygon(x, y, 4, COLOR_GREEN);

	emgl_drawTextA(&font_arial12, "Hello world!!!", 220, 170, COLOR_BLACK, COLOR_WHITE);
	emgl_drawTextA(&font_arial8, "Test 123", 220, 160, COLOR_GREEN, COLOR_WHITE);
	emgl_drawTextA(&font_segoescript10, "HHHTTT :D", 220, 145, COLOR_PURPLE, COLOR_WHITE);
	emgl_drawTextA(&font_segoescript10, "the quick brown fox jumped over the lazy dog. 1234567890", 0, 130, COLOR_RED, COLOR_WHITE);
	emgl_drawTextA(&font_Sylfaen12, "the quick brown fox jumped over the lazy dog. 1234567890", 0, 120, COLOR_NAVY, COLOR_WHITE);

	//emgl_drawFilledArcP(165, 50, 191, 65, 142, 30, COLOR_YELLOW);

	emgl_drawFilledRectangle(220, 90, 10, 10, COLOR_BLACK);
	emgl_drawFilledRectangle(230, 90, 10, 10, COLOR_MAROON);
	emgl_drawFilledRectangle(240, 90, 10, 10, COLOR_GREEN);
	emgl_drawFilledRectangle(250, 90, 10, 10, COLOR_NAVY);
	emgl_drawFilledRectangle(260, 90, 10, 10, COLOR_SILVER);
	emgl_drawFilledRectangle(270, 90, 10, 10, COLOR_RED);
	emgl_drawFilledRectangle(280, 90, 10, 10, COLOR_LIME);
	emgl_drawFilledRectangle(290, 90, 10, 10, COLOR_BLUE);
	emgl_drawFilledRectangle(220, 100, 10, 10, emgl_colorLighter(COLOR_BLACK));
	emgl_drawFilledRectangle(230, 100, 10, 10, emgl_colorLighter(COLOR_MAROON));
	emgl_drawFilledRectangle(240, 100, 10, 10, emgl_colorLighter(COLOR_GREEN));
	emgl_drawFilledRectangle(250, 100, 10, 10, emgl_colorLighter(COLOR_NAVY));
	emgl_drawFilledRectangle(260, 100, 10, 10, emgl_colorLighter(COLOR_SILVER));
	emgl_drawFilledRectangle(270, 100, 10, 10, emgl_colorLighter(COLOR_RED));
	emgl_drawFilledRectangle(280, 100, 10, 10, emgl_colorLighter(COLOR_LIME));
	emgl_drawFilledRectangle(290, 100, 10, 10, emgl_colorLighter(COLOR_BLUE));
	emgl_drawFilledRectangle(220, 110, 10, 10, emgl_colorDoubleLighter(COLOR_BLACK));
	emgl_drawFilledRectangle(230, 110, 10, 10, emgl_colorDoubleLighter(COLOR_MAROON));
	emgl_drawFilledRectangle(240, 110, 10, 10, emgl_colorDoubleLighter(COLOR_GREEN));
	emgl_drawFilledRectangle(250, 110, 10, 10, emgl_colorDoubleLighter(COLOR_NAVY));
	emgl_drawFilledRectangle(260, 110, 10, 10, emgl_colorDoubleLighter(COLOR_SILVER));
	emgl_drawFilledRectangle(270, 110, 10, 10, emgl_colorDoubleLighter(COLOR_RED));
	emgl_drawFilledRectangle(280, 110, 10, 10, emgl_colorDoubleLighter(COLOR_LIME));
	emgl_drawFilledRectangle(290, 110, 10, 10, emgl_colorDoubleLighter(COLOR_BLUE));
	emgl_drawFilledRectangle(220, 80, 10, 10, emgl_colorDarker(COLOR_BLACK));
	emgl_drawFilledRectangle(230, 80, 10, 10, emgl_colorDarker(COLOR_MAROON));
	emgl_drawFilledRectangle(240, 80, 10, 10, emgl_colorDarker(COLOR_GREEN));
	emgl_drawFilledRectangle(250, 80, 10, 10, emgl_colorDarker(COLOR_NAVY));
	emgl_drawFilledRectangle(260, 80, 10, 10, emgl_colorDarker(COLOR_SILVER));
	emgl_drawFilledRectangle(270, 80, 10, 10, emgl_colorDarker(COLOR_RED));
	emgl_drawFilledRectangle(280, 80, 10, 10, emgl_colorDarker(COLOR_LIME));
	emgl_drawFilledRectangle(290, 80, 10, 10, emgl_colorDarker(COLOR_BLUE));
	emgl_drawFilledRectangle(220, 70, 10, 10, emgl_colorDoubleDarker(COLOR_BLACK));
	emgl_drawFilledRectangle(230, 70, 10, 10, emgl_colorDoubleDarker(COLOR_MAROON));
	emgl_drawFilledRectangle(240, 70, 10, 10, emgl_colorDoubleDarker(COLOR_GREEN));
	emgl_drawFilledRectangle(250, 70, 10, 10, emgl_colorDoubleDarker(COLOR_NAVY));
	emgl_drawFilledRectangle(260, 70, 10, 10, emgl_colorDoubleDarker(COLOR_SILVER));
	emgl_drawFilledRectangle(270, 70, 10, 10, emgl_colorDoubleDarker(COLOR_RED));
	emgl_drawFilledRectangle(280, 70, 10, 10, emgl_colorDoubleDarker(COLOR_LIME));
	emgl_drawFilledRectangle(290, 70, 10, 10, emgl_colorDoubleDarker(COLOR_BLUE));
	emgl_drawFilledRectangle(220, 40, 10, 10, COLOR_GRAY);
	emgl_drawFilledRectangle(230, 40, 10, 10, COLOR_PURPLE);
	emgl_drawFilledRectangle(240, 40, 10, 10, COLOR_OLIVE);
	emgl_drawFilledRectangle(250, 40, 10, 10, COLOR_TEAL);
	emgl_drawFilledRectangle(260, 40, 10, 10, COLOR_WHITE);
	emgl_drawFilledRectangle(270, 40, 10, 10, COLOR_FUCHSIA);
	emgl_drawFilledRectangle(280, 40, 10, 10, COLOR_YELLOW);
	emgl_drawFilledRectangle(290, 40, 10, 10, COLOR_AQUA);
	emgl_drawFilledRectangle(220, 50, 10, 10, emgl_colorLighter(COLOR_GRAY));
	emgl_drawFilledRectangle(230, 50, 10, 10, emgl_colorLighter(COLOR_PURPLE));
	emgl_drawFilledRectangle(240, 50, 10, 10, emgl_colorLighter(COLOR_OLIVE));
	emgl_drawFilledRectangle(250, 50, 10, 10, emgl_colorLighter(COLOR_TEAL));
	emgl_drawFilledRectangle(260, 50, 10, 10, emgl_colorLighter(COLOR_WHITE));
	emgl_drawFilledRectangle(270, 50, 10, 10, emgl_colorLighter(COLOR_FUCHSIA));
	emgl_drawFilledRectangle(280, 50, 10, 10, emgl_colorLighter(COLOR_YELLOW));
	emgl_drawFilledRectangle(290, 50, 10, 10, emgl_colorLighter(COLOR_AQUA));
	emgl_drawFilledRectangle(220, 60, 10, 10, emgl_colorDoubleLighter(COLOR_GRAY));
	emgl_drawFilledRectangle(230, 60, 10, 10, emgl_colorDoubleLighter(COLOR_PURPLE));
	emgl_drawFilledRectangle(240, 60, 10, 10, emgl_colorDoubleLighter(COLOR_OLIVE));
	emgl_drawFilledRectangle(250, 60, 10, 10, emgl_colorDoubleLighter(COLOR_TEAL));
	emgl_drawFilledRectangle(260, 60, 10, 10, emgl_colorDoubleLighter(COLOR_WHITE));
	emgl_drawFilledRectangle(270, 60, 10, 10, emgl_colorDoubleLighter(COLOR_FUCHSIA));
	emgl_drawFilledRectangle(280, 60, 10, 10, emgl_colorDoubleLighter(COLOR_YELLOW));
	emgl_drawFilledRectangle(290, 60, 10, 10, emgl_colorDoubleLighter(COLOR_AQUA));
	emgl_drawFilledRectangle(220, 30, 10, 10, emgl_colorDarker(COLOR_GRAY));
	emgl_drawFilledRectangle(230, 30, 10, 10, emgl_colorDarker(COLOR_PURPLE));
	emgl_drawFilledRectangle(240, 30, 10, 10, emgl_colorDarker(COLOR_OLIVE));
	emgl_drawFilledRectangle(250, 30, 10, 10, emgl_colorDarker(COLOR_TEAL));
	emgl_drawFilledRectangle(260, 30, 10, 10, emgl_colorDarker(COLOR_WHITE));
	emgl_drawFilledRectangle(270, 30, 10, 10, emgl_colorDarker(COLOR_FUCHSIA));
	emgl_drawFilledRectangle(280, 30, 10, 10, emgl_colorDarker(COLOR_YELLOW));
	emgl_drawFilledRectangle(290, 30, 10, 10, emgl_colorDarker(COLOR_AQUA));
	emgl_drawFilledRectangle(220, 20, 10, 10, emgl_colorDoubleDarker(COLOR_GRAY));
	emgl_drawFilledRectangle(230, 20, 10, 10, emgl_colorDoubleDarker(COLOR_PURPLE));
	emgl_drawFilledRectangle(240, 20, 10, 10, emgl_colorDoubleDarker(COLOR_OLIVE));
	emgl_drawFilledRectangle(250, 20, 10, 10, emgl_colorDoubleDarker(COLOR_TEAL));
	emgl_drawFilledRectangle(260, 20, 10, 10, emgl_colorDoubleDarker(COLOR_WHITE));
	emgl_drawFilledRectangle(270, 20, 10, 10, emgl_colorDoubleDarker(COLOR_FUCHSIA));
	emgl_drawFilledRectangle(280, 20, 10, 10, emgl_colorDoubleDarker(COLOR_YELLOW));
	emgl_drawFilledRectangle(290, 20, 10, 10, emgl_colorDoubleDarker(COLOR_AQUA));

	emgl::Panel mainpanel(nullptr, emgl::Point(10, 10), emgl::Size(200, 110));
	emgl::Panel *panel2 = new emgl::Panel(&mainpanel, emgl::Point(5, 5), emgl::Size(50, 25));
	emgl::Text *testText = new emgl::Text(&mainpanel, emgl::Point(70, 25), emgl::Size(100, 11),
		"Hello world", emgl::ALIGN_LEFT, emgl::ALIGN_BOTTOM, true);
	emgl::Text *testText2 = new emgl::Text(&mainpanel, emgl::Point(70, 13), emgl::Size(100, 11),
		"Hello world", emgl::ALIGN_CENTER, emgl::ALIGN_BOTTOM, true);
	emgl::Text *testText3 = new emgl::Text(&mainpanel, emgl::Point(70, 1), emgl::Size(100, 11),
		"Hello world", emgl::ALIGN_RIGHT, emgl::ALIGN_BOTTOM, true);
	emgl::Text *testText4 = new emgl::Text(&mainpanel, emgl::Point(5, 40), emgl::Size(100, 80),
		"\nkl\nTest 1 2 3 4 5 6 7 8 Hello world\nDe\tasdf\nRJ", emgl::ALIGN_CENTER, emgl::ALIGN_TOP, true);
	testText->setBackground(COLOR_FUCHSIA);
	testText2->setBackground(COLOR_FUCHSIA);
	testText3->setBackground(COLOR_FUCHSIA);
	testText4->setBackground(COLOR_FUCHSIA);
	panel2->setBackground(COLOR_RED);
	mainpanel.Refresh();

	for (int y = 0; y < LCD_HEIGHT; y += 10)
	{
		emgl_setPixel(0, y, COLOR_BLACK);
	}

	while (1)
	{
		pcdrv_poll(drv->api);
	}
	return 0;
}