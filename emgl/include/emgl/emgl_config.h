#ifndef _EMGL_CONFIG_H_
#define _EMGL_CONFIG_H_

/* 
 * Chose the internal color mode:
 * Available color modes:
 *   - CM_1BPP
 *   - CM_8BPP
 *   - CM_RGB332
 *   - CM_RGB565
 *   - CM_RGBA5551
 *   - CM_RGBA8888
*/
#define EMGL_INTERNALCOLORMODE CM_RGBA8888
/*
 * Scratch buffer size in pixels
 * To use no scratch buffer set to 1
 */
#define EMGL_SCRATCHBUFFER_SIZE 1

/*
 * Uncomment to disable assertions
 */
//#define EMGL_NOASSERT

/*
 * Make use of stack for dynamic size buffers
 * Note: MSVC dpes not support this
 */
//#define EMGL_USE_VLA

/*
 * Use alpha blending
 */
//#define EMGL_USE_ALPHA_BLENDING

/*
 * Use compressed fonts
 */
#define EMGL_COMPRESSED_FONTS

/*
 * Enabled loglevel
 * Options are:
 * EMGL_LOGLVL_TODBG
 * EMGL_LOGLVL_TOINFO
 * EMGL_LOGLVL_TOWARN
 * EMGL_LOGLVL_TOERR
 * or an bitmask of:
 * EMGL_LOGLVL_DBG, EMGL_LOGLVL_INFO, 
 * EMGL_LOGLVL_WARN, EMGL_LOGLVL_ERR
 */
#define EMGL_LOGLVL EMGL_LOGLVL_TODBG

/*
 * Comment to disable logging
 */
#define EMGL_LOG_ENABLED

/*
 * Enable logging for modules by uncommenting the corresponding lines
 */
#define EMGL_LOGMOD_DRIVER_ENABLE
#define EMGL_LOGMOD_GRAPHICS_ENABLE
#define EMGL_LOGMOD_FONT_ENABLE

/* 
 * Use standard c library memory allocation functions and printf functions
 */
//#define EMGL_USE_STANDARD_IMPL 

#endif