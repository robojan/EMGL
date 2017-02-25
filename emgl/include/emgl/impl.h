#ifndef _EMGL_IMPL_H_
#define _EMGL_IMPL_H_

/*
* Memory allocator function
*/
#define emgl_malloc(x) g_emgl_support.malloc(x)

/*
* Memory clear allocator function
*/
#define emgl_calloc(x) g_emgl_support.calloc(x)

/*
* Memory deallocator function
*/
#define emgl_free(x) g_emgl_support.free(x)

/*
 * Logging function.
 * expects the same format as printf
 */
#define EMGL_LOG_IMPL(format, ...) do { if(g_emgl_support.log != NULL) g_emgl_support.log(format, ##__VA_ARGS__); } while(0)

/*
 * Assert function log
 */
#define EMGL_LOGASSERT_IMPL(message) \
	do { if(g_emgl_support.fatal != NULL) g_emgl_support.fatal("[EMGL] Assertion \"%s\" failed at line %d in %s\n", message, \
	__LINE__, __FILE__); } while(0)

#endif