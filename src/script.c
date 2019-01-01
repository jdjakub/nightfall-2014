#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "include/core.h"
#include "include/script.h"
#include "include/rsrc.h"
#include "include/glue.h"

static const char *setup_filename = "rsrc/core.zip";
static const char *script_names[] = {
	"oop.lua", "rsrc.lua", "display.lua", "gfx.lua", "setup.lua", NULL
};

static lua_State 	*l	= NULL;

static void default_error_func( const char *error );
static void init_core( void );
static void glue_together( void );

void SCR_init( void )
{	
	l = lua_open();
	luaL_openlibs( l );	
	init_core();
	glue_together();
}

void SCR_cleanup( void )
{
	lua_close( l );
}

int SCR_exec_string( const char *source, SCR_error_func error_func )
{
	if ( luaL_dostring( l, source ) != 0 ) {
		const char *error_msg = lua_tostring( l, -1 );
		if ( error_func ) {
			error_func( error_msg );
		} else {
			default_error_func( error_msg );
		}
		lua_pop( l, 1 );
		return 0;
	} else {
		return 1;
	}
}

int SCR_exec_buf( const void *buf, size_t size, SCR_error_func error_func )
{
	if ( luaL_dobuf( l, buf, size ) != 0 ) {
		const char *error_msg = lua_tostring( l, -1 );
		if ( error_func ) {
			error_func( error_msg );
		} else {
			default_error_func( error_msg );
		}
		lua_pop( l, 1 );
		return 0;
	} else {
		return 1;
	}
}

void SCR_notify_input( void *scr, unsigned group_id, unsigned cmpt_id, void *data)
{
	lua_getglobal( l, "Gfx" );
	lua_getfield( l, -1, "dispatchInput" );
	lua_pushlightuserdata( l, scr );
	lua_pushinteger( l, group_id );
	lua_pushinteger( l, cmpt_id );
	lua_pushnil( l );
	if ( lua_pcall( l, 4, 0, 0 ) != 0 ) {
		const char *error_msg = lua_tostring( l, -1 );
		
		default_error_func( error_msg );
		lua_pop( l, 1 );
	}
}

void init_core( void )
{
	struct RES_file *setup = RES_load_resfile( setup_filename );
	if ( setup ) {
		int i = 0;
		const char *name = script_names[i];
		
		while ( name ) {
			size_t size = 0;
			void *script = RES_get_data( setup, name, &size );
			
			if ( script ) {
				SCR_exec_buf( script, size, NULL );
				RES_free_data( script );
			} else {
				COR_fatal_error( "Fatal error: Couldn't load core script %s in %s\n", name, setup_filename );
			}
			name = script_names[++i];
		}
		RES_unload_resfile( setup );
	} else {
		COR_fatal_error( "Fatal error: error with essential file %s\n", setup_filename );
	}
}

void glue_together( void )
{
	glue_display( l );
	glue_rsrc( l );
	glue_event( l );
}

static void default_error_func( const char *error )
{
	COR_fatal_error( "Fatal Lua script error:\n%s\n", error );
}