#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "include/glue.h"
#include "include/glue_display.h"
#include "include/glue_rsrc.h"
/*#include "glue_event.h"*/

static const luaL_reg display_exports[] =
{
	{"getSettings", glue_getSettings},
	{"setSettings", glue_setSettings},
	{"newScreen", glue_newScreen},
	{"setScreen", glue_setScreen},
	{"delScreen", glue_delScreen},
	{"addComponent", glue_addComponent},
	{"delComponent", glue_delComponent},
	{"getCmptBaseData", glue_getCmptBaseData},
	{"setCmptBaseData", glue_setCmptBaseData},
	{"getCmptUserData", glue_getCmptUserData},
	{"setCmptUserData", glue_setCmptUserData},
	{"getTextureDims", glue_getTextureDims},
	{"createGlyphAtlas", glue_createGlyphAtlas},
	//{"getTextDims", glue_getTextDims},
	{"createSurface", glue_createSurface},
	{"createTexture", glue_createTexture},
	{"freeSurface", glue_freeSurface},
	{"freeTexture", glue_freeTexture},
	{"updateTexture", glue_updateTexture},
	//{"clearRect", glue_clearRect},
	{"drawText", glue_drawText},
	{"drawBox", glue_drawBox},
	{0, 0}
};

static const luaL_reg rsrc_exports[] =
{
	{"loadResFile", glue_loadResFile},
	{"unloadResFile", glue_unloadResFile},
	{"getResData", glue_getResData},
	{"freeResData", glue_freeResData},
	{"resToTexture", glue_resToTexture},
	{"resToString", glue_resToString},
	{0, 0}
};

static const luaL_reg event_exports[] =
{
	{0, 0}
};

void glue_display( lua_State *l )
{
	lua_getglobal( l, "Display" );
	luaL_register( l, NULL, display_exports );
	lua_pop( l, 1 );
}

void glue_rsrc( lua_State *l )
{
	lua_getglobal( l, "Rsrc" );
	luaL_register( l, NULL, rsrc_exports );
	lua_pop( l, 1 );
}

void glue_event( lua_State *l )
{
}