#ifndef GLUE_DISPLAY_H
#define GLUE_DISPLAY_H
#include "glue_aux.h"

LUA_GLUE( getSettings );
LUA_GLUE( setSettings ); /* settings */
LUA_GLUE( newScreen );
LUA_GLUE( setScreen ); /* idx, screen */
LUA_GLUE( delScreen ); /* idx */
LUA_GLUE( addComponent ); /* screen, type, layer */
LUA_GLUE( delComponent ); /* screen, id */
LUA_GLUE( getCmptBaseData ); /* screen, id */
LUA_GLUE( setCmptBaseData ); /* screen, id, data */
LUA_GLUE( getCmptUserData ); /* screen, id */
LUA_GLUE( setCmptUserData ); /* screen, id, data */
LUA_GLUE( getTextureDims ); /* tex */
LUA_GLUE( createGlyphAtlas ); /* fontData, size, pxSize */
//LUA_GLUE( getTextDims ); /* atlas, str, w */
LUA_GLUE( createSurface ); /* w, h */
LUA_GLUE( createTexture ); /* surf */
LUA_GLUE( freeSurface ); /* surf */
LUA_GLUE( freeTexture ); /* tex */
LUA_GLUE( updateTexture ); /* tex, surf */
LUA_GLUE( drawText ); /* atlas, surf, str, bounds */
LUA_GLUE( drawBox ); /* dst, src, desc, bounds */
#endif