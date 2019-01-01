#include "include/glue_rsrc.h"
#include "include/rsrc.h"
#include "include/display.h"

void *check_data( lua_State *l, int arg )
{
	luaL_argcheck( l, lua_isuserdata( l, arg ), arg, "expected user data (C pointer)" );
	return lua_touserdata( l, arg );
}

LUA_GLUE( loadResFile ) /* filename */
{
	const char *filename = luaL_checkstring( l, 1 );
	struct RES_file *f = RES_load_resfile( filename );
	if ( f ) {
		lua_pushlightuserdata( l, f );
		return 1;
	} else {
		luaL_error( l, "Couldn't load resource file %s", filename );
		return 0;
	}
}

LUA_GLUE( unloadResFile ) /* file */
{
	struct RES_file *f = NULL;
	
	f = check_data( l, 1 );
	RES_unload_resfile( f );
	return 0;
}

LUA_GLUE( getResData ) /* file, resId */
{
	struct RES_file *f = NULL;
	void *data = NULL;
	const char *resId = NULL;
	size_t size;
	
	resId = luaL_checkstring( l, 2 );
	f = check_data( l, 1 );
	data = RES_get_data( f, resId, &size );
	if ( data ) {
		lua_pushlightuserdata( l, data );
		lua_pushinteger( l, size );
		return 2;
	} else {
		luaL_error( l, "Couldn't obtain resource %s", resId );
		return 0;
	}
}

LUA_GLUE( freeResData ) /* data */
{
	void *data = NULL;
	
	data = check_data( l, 1 );
	RES_free_data( data );
	return 0;
}

LUA_GLUE( resToTexture ) /* data, size */
{
	void *data = NULL;
	SDL_Texture *tex = NULL;
	size_t size;
	
	data = check_data( l, 1 );
	size = luaL_checkinteger( l, 2 );
	tex = DSP_img_to_texture( data, size );
	if ( tex ) {
		lua_pushlightuserdata( l, tex );
		return 1;
	} else {
		luaL_error( l, "Couldn't convert texture" );
		return 0;
	}
}

LUA_GLUE( resToString ) /* data, size */
{
	void *data = NULL;
	size_t size;
	
	data = check_data( l, 1 );
	size = luaL_checkinteger( l, 2 );
	if ( data ) {
		lua_pushlstring( l, (const char*) data, size );
		return 1;
	} else {
		luaL_error( l, "Couldn't obtain script" );
		return 0;
	}
}