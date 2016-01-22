#ifndef _EMGL_IMPL_H_
#define _EMGL_IMPL_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
* Memory allocator function
*/
#define emgl_malloc(x) malloc(x)

/*
* Memory clear allocator function
*/
#define emgl_calloc(x) calloc(x)

/*
* Memory deallocator function
*/
#define emgl_free(x) free(x)

/*
 * Logging function.
 * expects the same format as printf
 */
#define EMGL_LOG_IMPL(x) do{ printf(x);} while(0)

/*
 * Assert function log
 */
#define EMGL_LOGASSERT_IMPL(message) \
	do { printf("Assertion \"%s\" failed at line %d in %s\n", message, \
	__LINE__, __FILE__); fflush(NULL); abort(); } while(0)

#endif