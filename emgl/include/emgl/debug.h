#ifndef _EMGL_LOG_H_
#define _EMGL_LOG_H_

#include <emgl/emgl_config.h>
#include <emgl/impl.h>

#ifdef EMGL_NOASSERT
#define EMGL_ASSERT(msg, cond) 
#else 
#define EMGL_ASSERT(msg, cond) do{if(!(cond)) { \
	EMGL_LOGASSERT_IMPL(msg); while(1); }} while(0)
#endif

#define EMGL_LOGLVL_DBG           0x01
#define EMGL_LOGLVL_INFO          0x02
#define EMGL_LOGLVL_WARN          0x04
#define EMGL_LOGLVL_ERR           0x08
#define EMGL_LOGLVL_MASK          0x0F
#define EMGL_LOGLVL_TODBG         0x0F
#define EMGL_LOGLVL_TOINFO        0x0E
#define EMGL_LOGLVL_TOWARN        0x0C
#define EMGL_LOGLVL_TOERR         0x08
#define EMGL_LOGLVL_NOTHING       0x00

#define EMGL_LOGMOD_DRIVER        0x10
#define EMGL_LOGMOD_GRAPHICS      0x20
#define EMGL_LOGMOD_FONT          0x40

#ifdef EMGL_LOGMOD_DRIVER_ENABLE
#define EMGL_LOGMOD_OPTDRIVER EMGL_LOGMOD_DRIVER
#else 
#define EMGL_LOGMOD_OPTDRIVER 0x00
#endif
#ifdef EMGL_LOGMOD_GRAPHICS_ENABLE
#define EMGL_LOGMOD_OPTGRAPHICS EMGL_LOGMOD_GRAPHICS
#else 
#define EMGL_LOGMOD_OPTGRAPHICS 0x00
#endif
#ifdef EMGL_LOGMOD_FONT_ENABLE
#define EMGL_LOGMOD_OPTFONT EMGL_LOGMOD_FONT
#else 
#define EMGL_LOGMOD_OPTFONT 0x00
#endif

#define EMGL_LOGMOD_ENABLED (EMGL_LOGMOD_OPTDRIVER | EMGL_LOGMOD_OPTGRAPHICS | EMGL_LOGMOD_OPTFONT)

#ifdef EMGL_LOG_ENABLED
#define EMGL_LOG(level, x) do {if((((level)&EMGL_LOGLVL_MASK&EMGL_LOGLVL) != 0) && \
	((level)&(~EMGL_LOGLVL_MASK)&EMGL_LOGMOD_ENABLED)!= 0) {EMGL_LOG_IMPL(x);} } \
	while(0)
#else
#define EMGL_LOG(level, x) 
#endif

#endif