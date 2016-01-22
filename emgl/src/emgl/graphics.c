#include <emgl/graphics.h>
#include <emgl/debug.h>
#include <emgl/driver.h>

#define DRIVER g_emgl_activeDriver

void emgl_clear(emgl_color_t clearColor)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	coord_t width,  height;
	DRIVER->getSize(DRIVER->api, &width, &height);
	DRIVER->fillRect(DRIVER->api, 0, 0, width, height, clearColor);
}

void emgl_drawRectangle(coord_t x, coord_t y, coord_t width, coord_t height, 
	emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	EMGL_ASSERT("width >= 0 && height >= 0", width >= 0 && height >= 0);
	if (width == 0 || height == 0)
	{
		return;
	}
	DRIVER->drawLineH(DRIVER->api, x, x + width - 1, y, color);
	DRIVER->drawLineH(DRIVER->api, x, x + width - 1, y + height - 1, color);
	DRIVER->drawLineV(DRIVER->api, y + 1, y + height - 2, x, color);
	DRIVER->drawLineV(DRIVER->api, y + 1, y + height - 2, x + width -1, color);
}

void emgl_drawFilledRectangle(coord_t x, coord_t y, coord_t width, coord_t height, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	EMGL_ASSERT("width >= 0 && height >= 0", width >= 0 && height >= 0);
	if (width == 0 || height == 0)
	{
		return;
	}
	DRIVER->fillRect(DRIVER->api, x, y, x+width-1, y+height-1, color);
}

void emgl_drawCircle(coord_t xo, coord_t yo, coord_t r, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	EMGL_ASSERT("r >= 0", r >= 0);
	if (r == 0)
	{
		return;
	}
	coord_t x = r;
	coord_t y = 0;
	coord_t re = 1-x;

	while (x >= y)
	{
		DRIVER->setPixel(DRIVER->api, x + xo, y + yo, color);
		DRIVER->setPixel(DRIVER->api, y + xo, x + yo, color);
		DRIVER->setPixel(DRIVER->api, -x + xo, y + yo, color);
		DRIVER->setPixel(DRIVER->api, -y + xo, x + yo, color);
		DRIVER->setPixel(DRIVER->api, -x + xo, -y + yo, color);
		DRIVER->setPixel(DRIVER->api, -y + xo, -x + yo, color);
		DRIVER->setPixel(DRIVER->api, x + xo, -y + yo, color);
		DRIVER->setPixel(DRIVER->api, y + xo, -x + yo, color);
		y++;
		if (re < 0)
		{
			re += 2 * y + 1;
		}
		else {
			x--;
			re += 2 * (y - x) + 1;
		}
	}
}

void emgl_drawFilledCircle(coord_t xo, coord_t yo, coord_t r, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	EMGL_ASSERT("r >= 0", r >= 0);
	if (r == 0)
	{
		return;
	}
	coord_t x = r;
	coord_t y = 0;
	coord_t re = 1 - x;

	while (x >= y)
	{
		DRIVER->drawLineH(DRIVER->api, -x + xo, x + xo, y + yo, color);
		DRIVER->drawLineH(DRIVER->api, -y + xo, y + xo, x + yo, color);
		DRIVER->drawLineH(DRIVER->api, -x + xo, x + xo, -y + yo, color);
		DRIVER->drawLineH(DRIVER->api, -y + xo, y + xo, -x + yo, color);
		y++;
		if (re < 0)
		{
			re += 2 * y + 1;
		}
		else {
			x--;
			re += 2 * (y - x) + 1;
		}
	}

}

void emgl_drawEllipse(coord_t xo, coord_t yo, coord_t width, coord_t height, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	if (width == 0 || height == 0)
	{
		return;
	}
	if (width < 0) width = -width;
	if (height < 0) height = -height;
	width /= 2;
	height /= 2;

	coord_t twoASquare = 2 * width * width;
	coord_t twoBSquare = 2 * height * height;
	coord_t error = width * width * height;
	coord_t x = 0;
	coord_t y = height;
	coord_t stopy = 0;
	coord_t stopx = twoASquare*height;

	while (stopy <= stopx)
	{
		DRIVER->setPixel(DRIVER->api, x + xo, y + yo, color);
		DRIVER->setPixel(DRIVER->api, -x + xo, y + yo, color);
		DRIVER->setPixel(DRIVER->api, -x + xo, -y + yo, color);
		DRIVER->setPixel(DRIVER->api, x + xo, -y + yo, color);
		x++;
		error -= twoBSquare*(x - 1);
		stopy += twoBSquare;
		if (error <= 0)
		{
			error += twoASquare * (y - 1);
			y--;
			stopx -= twoASquare;
		}
	}

	error = height * height * width;
	x = width;
	y = 0;
	stopy = twoBSquare*width;
	stopx = 0;
	while (stopy >= stopx)
	{
		DRIVER->setPixel(DRIVER->api, x + xo, y + yo, color);
		DRIVER->setPixel(DRIVER->api, -x + xo, y + yo, color);
		DRIVER->setPixel(DRIVER->api, -x + xo, -y + yo, color);
		DRIVER->setPixel(DRIVER->api, x + xo, -y + yo, color);
		y++;
		error -= twoASquare * (y - 1);
		stopx += twoASquare;
		if (error < 0)
		{
			error += twoBSquare * (x-1);
			x--;
			stopy -= twoBSquare;
		}
	}
}

void emgl_drawFilledEllipse(coord_t xo, coord_t yo, coord_t width, coord_t height, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	if (width == 0 || height == 0)
	{
		return;
	}
	if (width < 0) width = -width;
	if (height < 0) height = -height;
	width /= 2;
	height /= 2;

	coord_t twoASquare = 2 * width * width;
	coord_t twoBSquare = 2 * height * height;
	coord_t error = width * width * height;
	coord_t x = 0;
	coord_t y = height;
	coord_t stopy = 0;
	coord_t stopx = twoASquare*height;

	while (stopy <= stopx)
	{
		DRIVER->drawLineH(DRIVER->api, -x + xo, x + xo, y + yo, color);
		DRIVER->drawLineH(DRIVER->api, -x + xo, x + xo, -y + yo, color);
		x++;
		error -= twoBSquare*(x - 1);
		stopy += twoBSquare;
		if (error <= 0)
		{
			error += twoASquare * (y - 1);
			y--;
			stopx -= twoASquare;
		}
	}

	error = height * height * width;
	x = width;
	y = 0;
	stopy = twoBSquare*width;
	stopx = 0;
	while (stopy >= stopx)
	{
		DRIVER->drawLineH(DRIVER->api, -x + xo, x + xo, y + yo, color);
		DRIVER->drawLineH(DRIVER->api, -x + xo, x + xo, -y + yo, color);
		y++;
		error -= twoASquare * (y - 1);
		stopx += twoASquare;
		if (error < 0)
		{
			error += twoBSquare * (x - 1);
			x--;
			stopy -= twoBSquare;
		}
	}
}

void emgl_drawArcP(coord_t xo, coord_t yo, coord_t startx, coord_t starty, coord_t endx, coord_t endy, emgl_color_t color)
{
	EMGL_ASSERT("emgl_drawArcP: g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	EMGL_ASSERT("emgl_drawArcP: radius must be constant between start and end", 
		(startx - xo)*(startx - xo) + (starty - yo)*(starty - yo) == 
		(endx - xo)*(endx - xo) + (endy - yo)*(endy - yo));
	coord_t x = startx - xo;
	coord_t y = starty - yo;
	coord_t r = x * x + y * y;
	coord_t re = r;
	endx -= xo;
	endy -= yo;
	while (x != endx || y != endy)
	{
		DRIVER->setPixel(DRIVER->api, x + xo, y + yo, color);
		coord_t rx, ry, rd;
		coord_t dx, dy;
		if (x < 0)
		{
			ry = re - 2 * y + 1;
			dy = -1;
		}
		else {
			ry = re + 2 * y + 1;
			dy = 1;
		}
		if (y < 0)
		{
			rx = re + 2 * x + 1;
			dx = 1;
		}
		else {
			rx = re - 2 * x + 1;
			dx = -1;
		}
		rd = rx + ry - re;
		coord_t ex = r - rx;
		coord_t ey = r - ry;
		coord_t ed = r - rd;
		if (ey < 0) ey = -ey;
		if (ex < 0) ex = -ex;
		if (ed < 0) ed = -ed;
		if (ex < ey && ex < ed)
		{
			// Step in x
			x += dx;
			re = rx;
		}
		else if (ey < ex && ey < ed) {
			// Step in y
			y += dy;
			re = ry;
		}
		else {
			// Step in d
			x += dx;
			y += dy;
			re = rd;
		}
	}
}

void emgl_drawFilledArcP(coord_t xo, coord_t yo, coord_t startx, coord_t starty, coord_t endx, coord_t endy, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	/*EMGL_ASSERT("emgl_drawArcP: radius must be constant between start and end",
		(startx - xo)*(startx - xo) + (starty - yo)*(starty - yo) ==
		(endx - xo)*(endx - xo) + (endy - yo)*(endy - yo));*/
	coord_t x = startx - xo;
	coord_t y = starty - yo;
	coord_t r = x * x + y * y;
	coord_t re = r;
	endx -= xo;
	endy -= yo;
	while (x != endx || y != endy)
	{
		DRIVER->drawLineH(DRIVER->api, xo, xo + x, yo + y, color);
		coord_t rx, ry, rd;
		coord_t dx, dy;
		if (x < 0)
		{
			ry = re - 2 * y + 1;
			dy = -1;
		}
		else {
			ry = re + 2 * y + 1;
			dy = 1;
		}
		if (y < 0)
		{
			rx = re + 2 * x + 1;
			dx = 1;
		}
		else {
			rx = re - 2 * x + 1;
			dx = -1;
		}
		rd = rx + ry - re;
		coord_t ex = r - rx;
		coord_t ey = r - ry;
		coord_t ed = r - rd;
		if (ey < 0) ey = -ey;
		if (ex < 0) ex = -ex;
		if (ed < 0) ed = -ed;
		if (ex < ey && ex < ed)
		{
			// Step in x
			x += dx;
			re = rx;
		}
		else if (ey < ex && ey < ed) {
			// Step in y
			y += dy;
			re = ry;
		}
		else {
			// Step in d
			x += dx;
			y += dy;
			re = rd;
		}
	}

}

void emgl_drawRoundedRectangle(coord_t x, coord_t y, coord_t width, coord_t height, coord_t r, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	EMGL_ASSERT("width >= 0 && height >= 0", width >= 0 && height >= 0);
	if (width == 0 || height == 0)
	{
		return;
	}
	DRIVER->drawLineH(DRIVER->api, x + r, x + width - r - 1, y, color);
	DRIVER->drawLineH(DRIVER->api, x + r, x + width - r - 1, y + height - 1, color);
	DRIVER->drawLineV(DRIVER->api, y + r, y + height - r - 1, x, color);
	DRIVER->drawLineV(DRIVER->api, y + r, y + height - r - 1, x + width - 1, color);
	coord_t xc = r;
	coord_t yc = 0;
	coord_t re = 1 - xc;

	while (xc >= yc)
	{
		coord_t xo = x + width - r - 1;
		coord_t yo = y + height - r - 1;
		DRIVER->setPixel(DRIVER->api, xc + xo, yc + yo, color);
		DRIVER->setPixel(DRIVER->api, yc + xo, xc + yo, color);
		yo = y + r;
		DRIVER->setPixel(DRIVER->api, xc + xo, -yc + yo, color);
		DRIVER->setPixel(DRIVER->api, yc + xo, -xc + yo, color);
		xo = x + r;
		DRIVER->setPixel(DRIVER->api, -xc + xo, -yc + yo, color);
		DRIVER->setPixel(DRIVER->api, -yc + xo, -xc + yo, color);
		yo = y + height - r - 1;
		DRIVER->setPixel(DRIVER->api, -xc + xo, yc + yo, color);
		DRIVER->setPixel(DRIVER->api, -yc + xo, xc + yo, color);
		yc++;
		if (re < 0)
		{
			re += 2 * yc + 1;
		}
		else {
			xc--;
			re += 2 * (yc - xc) + 1;
		}
	}
}

void emgl_drawFilledRoundedRectangle(coord_t x, coord_t y, coord_t width, coord_t height, coord_t r, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	EMGL_ASSERT("width >= 0 && height >= 0", width >= 0 && height >= 0);
	if (width == 0 || height == 0)
	{
		return;
	}
	DRIVER->fillRect(DRIVER->api, x, y + r, x + width - 1, y + height - r - 1, color);
	DRIVER->fillRect(DRIVER->api, x + r, y, x + width - r - 1, y + r, color);
	DRIVER->fillRect(DRIVER->api, x + r, y + height - r - 1, x + width - r - 1, y + height - 1, color);

	coord_t xc = r;
	coord_t yc = 0;
	coord_t re = 1 - xc;

	while (xc >= yc)
	{
		coord_t xo = x + width - r - 1;
		coord_t yo = y + height - r - 1;
		DRIVER->drawLineH(DRIVER->api, xo, xc +xo, yc +yo, color);
		DRIVER->drawLineH(DRIVER->api, xo, yc + xo, xc + yo, color);
		yo = y + r;
		DRIVER->drawLineH(DRIVER->api, xo, xc + xo, -yc + yo, color);
		DRIVER->drawLineH(DRIVER->api, xo, yc + xo, -xc + yo, color);
		xo = x + r;
		DRIVER->drawLineH(DRIVER->api, xo, -xc + xo, -yc + yo, color);
		DRIVER->drawLineH(DRIVER->api, xo, -yc + xo, -xc + yo, color);
		yo = y + height - r - 1;
		DRIVER->drawLineH(DRIVER->api, xo, -xc + xo, yc + yo, color);
		DRIVER->drawLineH(DRIVER->api, xo, -yc + xo, xc + yo, color);
		yc++;
		if (re < 0)
		{
			re += 2 * yc + 1;
		}
		else {
			xc--;
			re += 2 * (yc - xc) + 1;
		}
	}
}

void emgl_drawPolygon(coord_t *x, coord_t *y, U8 count, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);

	for (U8 n = 0; n < count-1; n++)
	{
		emgl_drawLine(x[n], y[n], x[n+1], y[n+1], color);
	}
	emgl_drawLine(x[count-1], y[count-1], x[0], y[0],color);
}

void emgl_drawFilledPolygon(coord_t *x, coord_t *y, U8 count, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);

	EMGL_LOG(EMGL_LOGLVL_WARN, "emgl_drawFilledPolygon not implemented");
}

void emgl_drawBitmap(coord_t x, coord_t y, coord_t width, coord_t height, emgl_color_t *data)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	DRIVER->drawBitmap(DRIVER->api, x, y, width, height, data);
}

void emgl_drawLine(coord_t x1, coord_t y1, coord_t x2, coord_t y2, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);
	if (x1 == x2)
	{
		DRIVER->drawLineV(DRIVER->api, y1, y2, x1, color);
		return;
	}
	if (y1 == y2)
	{
		DRIVER->drawLineH(DRIVER->api, x1, x2, y1, color);
		return;
	}

	coord_t w = x2-x1;
	coord_t h = y2-y1;
	coord_t dx1 = 0;
	coord_t dy1 = 0;
	coord_t dy2 = 0;
	if (w < 0) dx1 = -1; else if (w > 0) dx1 = 1;
	if (h < 0) dy1 = -1; else if (h > 0) dy1 = 1;
	coord_t dx2 = dx1;
	coord_t longest = w < 0 ? -w : w;
	coord_t shortest = h < 0 ? -h : h;
	if (longest <= shortest)
	{
		coord_t t = longest;
		longest = shortest;
		shortest = t;
		if (h<0) dy2 = -1; else if (h > 0) dy2 = 1;
		dx2 = 0;
	}
	coord_t numerator = longest / 2;
	for (coord_t i = 0; i <= longest; i++)
	{
		DRIVER->setPixel(DRIVER->api, x1, y1, color);
		numerator += shortest;
		if (numerator >= longest)
		{
			numerator -= longest;
			x1 += dx1;
			y1 += dy1;
		}
		else {
			x1 += dx2;
			y1 += dy2;
		}
	}
}

void emgl_drawBezier(coord_t *x, coord_t *y, U8 count, emgl_color_t color)
{
	EMGL_ASSERT("g_emgl_activeDriver != NULL", g_emgl_activeDriver != NULL);

}
