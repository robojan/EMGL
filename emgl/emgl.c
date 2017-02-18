#include <emgl/emgl.h>

const emgl_driverAPI_t *g_emgl_activeDriver = NULL;

void emgl_registerDriver(const emgl_driverAPI_t *driver)
{
	g_emgl_activeDriver = driver; 
	driver->init(driver->api);
}