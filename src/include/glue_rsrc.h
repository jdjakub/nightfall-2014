#ifndef GLUE_RSRC_H
#define GLUE_RSRC_H
#include "glue_aux.h"

LUA_GLUE( loadResFile ); /* filename */
LUA_GLUE( unloadResFile ); /* file */
LUA_GLUE( getResData ); /* resId */
LUA_GLUE( freeResData ); /* file, data */
LUA_GLUE( resToTexture ); /* data, size */
LUA_GLUE( resToString ); /* data, size */

#endif