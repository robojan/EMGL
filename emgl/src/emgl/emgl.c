
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <emgl/emgl.h>
#include <emgl/debug.h>

emgl_driverAPI_t g_emgl_activeDriver = {
	.api = NULL,
	.poll = NULL,
	.getPixel = NULL,
	.setPixel = NULL,
	.fillRect = NULL, 
	.drawLineH = NULL,
	.drawLineV = NULL,
	.drawBitmap = NULL,
	.init = NULL,
	.getSize = NULL,
};
emgl_support_t g_emgl_support = { 
	.malloc=NULL, 
	.calloc=NULL,
	.free=NULL,
	.log=NULL,
	.fatal=NULL
};

static void emgl_default_fillRect(void *api, emgl_coord_t x1, emgl_coord_t y1, emgl_coord_t x2, emgl_coord_t y2,
	emgl_color_t color)
{
	if (x1 > x2)
	{
		emgl_coord_t x = x1;
		x1 = x2;
		x2 = x;
	}
	if (y1 > y2)
	{
		emgl_coord_t y = y1;
		y1 = y2;
		y2 = y;
	}
	
	if (x1 < 0)
	{
		x1 = 0;
	}
	if (y1 < 0)
	{
		y1 = 0;
	}

	for (emgl_coord_t x = x1; x <= x2; x++)
	{
		for (emgl_coord_t y = y1; y <= y2; y++)
		{
			g_emgl_activeDriver.setPixel(api, x, y, color);
		}
	}
}

static void emgl_default_drawBitmap(void *api, emgl_coord_t x, emgl_coord_t y, emgl_coord_t width,
	emgl_coord_t height, const emgl_color_t *data)
{
	EMGL_ASSERT("emgl_default_drawBitmap: width >= 0 && height >= 0", width >= 0 && height >= 0);
	emgl_coord_t startx = x;
	emgl_coord_t starty = y;
	emgl_coord_t endx = x + width;
	emgl_coord_t endy = y + height;

	if (x < 0)
	{
		startx = -x;
	}
	if (y < 0)
	{
		starty = -y;
	}

	for (emgl_coord_t iy = 0; iy < height; iy++)
	{
		for (emgl_coord_t ix = 0; ix < width; ix++)
		{
			g_emgl_activeDriver.setPixel(api, ix+startx, iy+starty, data[iy*width+ix]);
		}
	}
}

static void emgl_default_drawLineH(void *api, emgl_coord_t x1, emgl_coord_t x2, emgl_coord_t y,
	emgl_color_t color)
{
	if(x1 > x2)
	{
		emgl_coord_t tmp = x1;
		x1 = x2;
		x2 = tmp;					
	}
	for(emgl_coord_t x = x1; y <= x2; x++){
		g_emgl_activeDriver.setPixel(api, x, y, color);
	}
}

static void emgl_default_drawLineV(void *api, emgl_coord_t y1, emgl_coord_t y2, emgl_coord_t x, emgl_color_t color)
{
	if(y1 > y2)
	{
		emgl_coord_t tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	for(emgl_coord_t y = y1; y <= y2; y++){
		g_emgl_activeDriver.setPixel(api, x, y, color);
	}
}

static void emgl_stub_setPixel(void *api, emgl_coord_t x, emgl_coord_t y, emgl_color_t color)
{
	(void)api;
	(void)x;
	(void)y;
	(void)color;
}

static emgl_color_t emgl_stub_getPixel(void *api, emgl_coord_t x, emgl_coord_t y)
{
	(void) api;
	(void) x; 
	(void) y;
	return COLOR_BLACK;
}

static void emgl_stub_poll(void *api)
{
	(void)api;
}

static void emgl_stub_init(void *api)
{
	(void)api;
}

static void emgl_stub_getSize(void *api, emgl_coord_t *width, emgl_coord_t *height)
{
	(void)api;
	*width = 0;
	*height = 0;
}

void emgl_registerDriver(const emgl_driverAPI_t *driver)
{
	EMGL_LOG(EMGL_LOGLVL_DBG | EMGL_LOGMOD_GRAPHICS, "Register driver: %p\n\r", driver);
	if(driver == NULL)
	{
		memset(&g_emgl_activeDriver, 0, sizeof(g_emgl_activeDriver));
	} else {
		memcpy(&g_emgl_activeDriver, driver, sizeof(emgl_driverAPI_t));
	}
	if(g_emgl_activeDriver.drawBitmap == NULL) 
		g_emgl_activeDriver.drawBitmap = emgl_default_drawBitmap;
	if(g_emgl_activeDriver.drawLineH == NULL)
		g_emgl_activeDriver.drawLineH = emgl_default_drawLineH;
	if(g_emgl_activeDriver.drawLineV == NULL)
		g_emgl_activeDriver.drawLineV = emgl_default_drawLineV;
	if(g_emgl_activeDriver.fillRect == NULL)
		g_emgl_activeDriver.fillRect = emgl_default_fillRect;
	if(g_emgl_activeDriver.poll == NULL)
		g_emgl_activeDriver.poll = emgl_stub_poll;
	if(g_emgl_activeDriver.init == NULL)
		g_emgl_activeDriver.init = emgl_stub_init;
	if(g_emgl_activeDriver.getPixel == NULL)
		g_emgl_activeDriver.getPixel = emgl_stub_getPixel;
	if(g_emgl_activeDriver.setPixel == NULL)
		g_emgl_activeDriver.setPixel = emgl_stub_setPixel;
	if(g_emgl_activeDriver.getSize == NULL) 
		g_emgl_activeDriver.getSize = emgl_stub_getSize;
	g_emgl_activeDriver.init(g_emgl_activeDriver.api);
}

void *emgl_impl_calloc(emgl_U32 count, emgl_U32 size)
{
	void *buffer = g_emgl_support.malloc(size * count);
	emgl_U8 *ptr = (emgl_U8 *)buffer;
	for(emgl_U32 i = 0; i < count; i++) {
		for(emgl_U32 j = 0; j < size; j++) {
			*ptr++ = 0;
		}
	}
	return buffer;
}

void emgl_impl_fatal(const char *format, ...)
{
	if(g_emgl_support.log != NULL)
	{
		va_list args;
		va_start(args, format);
		g_emgl_support.log(format, args);
		va_end(args);
	}
	abort();
}

void emgl_init(const emgl_support_t *support)
{
	g_emgl_support.fatal = emgl_impl_fatal;
	g_emgl_support.calloc = emgl_impl_calloc;
#ifdef EMGL_USE_STANDARD_IMPL
	g_emgl_support.malloc = malloc;
	g_emgl_support.calloc = calloc;
	g_emgl_support.free = free;
	g_emgl_support.log = printf;
#else
	EMGL_ASSERT("support != NULL", support != NULL);
	EMGL_ASSERT("support->malloc != NULL", support->malloc);
	EMGL_ASSERT("support->free != NULL", support->free);
	EMGL_ASSERT("support->log != NULL", support->log);
#endif
	EMGL_LOG(EMGL_LOGLVL_DBG | EMGL_LOGMOD_GRAPHICS, "Initializing\n\r");
	if(support)
	{
		g_emgl_support.malloc = support->malloc;
		if(support->calloc)
			g_emgl_support.calloc = support->calloc;
		g_emgl_support.free = support->free;
		g_emgl_support.log = support->log;
		if(support->fatal)
			g_emgl_support.fatal = support->fatal;
	}
	
	emgl_registerDriver(NULL);
}