
#include <stdlib.h>
#include <stdarg.h>
#include <emgl/emgl.h>
#include <emgl/debug.h>

const emgl_driverAPI_t *g_emgl_activeDriver = NULL;
emgl_support_t g_emgl_support = { NULL, NULL, NULL, NULL, NULL };

void emgl_registerDriver(const emgl_driverAPI_t *driver)
{
	g_emgl_activeDriver = driver; 
	driver->init(driver->api);
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
	va_list args;
	va_start(args, format);
	g_emgl_support.log(format, args);
	va_end(args);
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
	
}