#include <emgl/emgl.h>
#include <emgl/pcdriver.h>

#include "images.h"
#include "arial12.h"
#include "arial8.h"
#include "segoescript10.h"
#include "Sylfaen12.h"

int main(int argc, char **argv)
{
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
	x[0] += 40;
	x[1] += 20;
	x[2] += 30;
	x[3] += 30;
	y[2] -= 5;
	y[1] += 5;
	emgl_drawFilledPolygon(x, y, 4, COLOR_GREEN);

	emgl_drawTextA(&font_arial12, "Hello world!!!", 220, 170, COLOR_BLACK);
	emgl_drawTextA(&font_arial8, "Test 123", 220, 160, COLOR_GREEN);
	emgl_drawTextA(&font_segoescript10, "HHHTTT :D", 220, 145, COLOR_PURPLE);
	emgl_drawTextA(&font_segoescript10, "the quick brown fox jumped over the lazy dog. 1234567890", 0, 130, COLOR_RED);
	emgl_drawTextA(&font_Sylfaen12, "the quick brown fox jumped over the lazy dog. 1234567890", 0, 120, COLOR_NAVY);

	//emgl_drawFilledArcP(165, 50, 191, 65, 142, 30, COLOR_YELLOW);

	//pcdrv_drawBitmap(drv->api, 0, 0, images_colors.width, images_colors.height, images_colors.data);
	//pcdrv_drawBitmap(drv->api, 128, 128, images_parrot.width, images_parrot.height, images_parrot.data);
	//pcdrv_drawBitmap(api, 0, 0, images_blackjack.width, images_blackjack.height, images_blackjack.data);
	//pcdrv_drawBitmap(api, 150, 0, images_luna.width, images_luna.height, images_luna.data);

	

	while (1)
	{
		pcdrv_poll(drv->api);
	}
	return 0;
}