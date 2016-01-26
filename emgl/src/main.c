#include <emgl/emgl.h>
#include <emgl/pcdriver.h>

#include "images.h"

int main(int argc, char **argv)
{
	const emgl_driverAPI_t *drv = pcdrv_CreateDriver();
	emgl_registerDriver(drv);
	emgl_clear(COLOR_WHITE);
	emgl_drawRectangle(20, 200, 20, 20, COLOR_BLACK);
	emgl_drawRectangle(20, 180, 20, 20, COLOR_MAROON);
	emgl_drawRectangle(20, 160, 20, 20, COLOR_GREEN);
	emgl_drawRectangle(20, 140, 20, 20, COLOR_NAVY);
	emgl_drawRectangle(40, 200, 20, 20, COLOR_SILVER);
	emgl_drawRectangle(40, 180, 20, 20, COLOR_RED);
	emgl_drawRectangle(40, 160, 20, 20, COLOR_LIME);
	emgl_drawRectangle(40, 140, 20, 20, COLOR_BLUE);
	emgl_drawRectangle(60, 200, 20, 20, COLOR_GRAY);
	emgl_drawRectangle(60, 180, 20, 20, COLOR_PURPLE);
	emgl_drawRectangle(60, 160, 20, 20, COLOR_OLIVE);
	emgl_drawRectangle(60, 140, 20, 20, COLOR_TEAL);
	emgl_drawRectangle(80, 200, 20, 20, COLOR_WHITE);
	emgl_drawRectangle(80, 180, 20, 20, COLOR_FUCHSIA);
	emgl_drawRectangle(80, 160, 20, 20, COLOR_YELLOW);
	emgl_drawRectangle(80, 140, 20, 20, COLOR_AQUA);

	emgl_drawFilledRectangle(120, 200, 20, 20, COLOR_BLACK);
	emgl_drawFilledRectangle(120, 180, 20, 20, COLOR_MAROON);
	emgl_drawFilledRectangle(120, 160, 20, 20, COLOR_GREEN);
	emgl_drawFilledRectangle(120, 140, 20, 20, COLOR_NAVY);
	emgl_drawFilledRectangle(140, 200, 20, 20, COLOR_SILVER);
	emgl_drawFilledRectangle(140, 180, 20, 20, COLOR_RED);
	emgl_drawFilledRectangle(140, 160, 20, 20, COLOR_LIME);
	emgl_drawFilledRectangle(140, 140, 20, 20, COLOR_BLUE);
	emgl_drawFilledRectangle(160, 200, 20, 20, COLOR_GRAY);
	emgl_drawFilledRectangle(160, 180, 20, 20, COLOR_PURPLE);
	emgl_drawFilledRectangle(160, 160, 20, 20, COLOR_OLIVE);
	emgl_drawFilledRectangle(160, 140, 20, 20, COLOR_TEAL);
	emgl_drawFilledRectangle(180, 200, 20, 20, COLOR_WHITE);
	emgl_drawFilledRectangle(180, 180, 20, 20, COLOR_FUCHSIA);
	emgl_drawFilledRectangle(180, 160, 20, 20, COLOR_YELLOW);
	emgl_drawFilledRectangle(180, 140, 20, 20, COLOR_AQUA);

	emgl_drawCircle(50, 50, 25, COLOR_OLIVE);
	emgl_drawCircle(55, 50, 25, COLOR_TEAL);
	emgl_drawCircle(60, 50, 25, COLOR_FUCHSIA);
	emgl_drawCircle(65, 50, 25, COLOR_AQUA);

	emgl_drawFilledCircle(50, 110, 25, COLOR_AQUA);
	emgl_drawFilledCircle(55, 110, 25, COLOR_FUCHSIA);
	emgl_drawFilledCircle(60, 110, 25, COLOR_TEAL);
	emgl_drawFilledCircle(65, 110, 25, COLOR_OLIVE);

	emgl_drawEllipse(150, 50, 50, 50, COLOR_OLIVE);
	emgl_drawEllipse(155, 50, 60, 50, COLOR_TEAL);
	emgl_drawEllipse(160, 50, 70, 50, COLOR_FUCHSIA);
	emgl_drawEllipse(165, 50, 80, 50, COLOR_AQUA);

	emgl_drawFilledEllipse(165, 110, 80, 50, COLOR_OLIVE);
	emgl_drawFilledEllipse(160, 110, 70, 50, COLOR_TEAL);
	emgl_drawFilledEllipse(155, 110, 60, 50, COLOR_FUCHSIA);
	emgl_drawFilledEllipse(150, 110, 50, 50, COLOR_AQUA);

	emgl_drawRoundedRectangle(220, 140, 30, 30, 5, COLOR_BLACK);
	emgl_drawRoundedRectangle(220, 180, 30, 30, 5, COLOR_MAROON);
	emgl_drawFilledRoundedRectangle(260, 140, 30, 30, 10, COLOR_GREEN);
	emgl_drawFilledRoundedRectangle(260, 180, 30, 30, 10, COLOR_NAVY);	

	emgl_coord_t x [] = {280, 310, 315, 300};
	emgl_coord_t y [] = {30, 40, 65, 70};

	emgl_drawPolygon(x, y, 4, COLOR_GREEN);
	x[0] -= 50;
	x[1] -= 50;
	x[2] -= 50;
	x[3] -= 50;
	emgl_drawFilledPolygon(x, y, 4, COLOR_GREEN);

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