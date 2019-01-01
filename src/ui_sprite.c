#include <SDL2/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "include/ui.h"
#include "include/glue_aux.h"

struct sprite_data {
	SDL_Texture *tex;
	SDL_Rect src;
	SDL_Color col;
};

void sprite_init( struct cmpt_node *c )
{
	struct sprite_data *d = (struct sprite_data*) COR_alloc( sizeof( *d ), 1 );
	
	memset( (void*) d, 0, sizeof( *d ) );
	d->col.r = 255;
	d->col.g = 255;
	d->col.b = 255;
	d->col.a = 255;
	c->user_data = d;
}

void sprite_draw( struct cmpt_node *c, SDL_Renderer *r )
{
	struct sprite_data *d = (struct sprite_data*) c->user_data;
	/*printf( "viz: %d tex: %d sx: %d sy: %d sw: %d sh: %d dx: %d dy: %d dw: %d dh: %d\n", c->visible, d->tex, d->src.x, d->src.y, d->src.w, d->src.h, c->bounds.x, c->bounds.y, c->bounds.w, c->bounds.h );*/
	
	if ( d->tex ) {
		SDL_SetTextureColorMod( d->tex, d->col.r, d->col.g, d->col.b );
		SDL_SetTextureAlphaMod( d->tex, d->col.a );
		SDL_RenderCopy( r, d->tex, &d->src,  &c->bounds );
	}
	/*printf( "SRC: %d,%d,%d,%d\n", d->src.x, d->src.y, d->src.w, d->src.h );
	printf( "DST: %d,%d,%d,%d\n", c->bounds.x, c->bounds.y, c->bounds.w, c->bounds.h );
	if ( !d->tex ) {
		printf( "Warning: Texture NULL!\n" );
	}*/
}

void sprite_free( struct cmpt_node *c )
{
	COR_free( (void*) c->user_data );
}

int sprite_udata_get( lua_State *l, struct cmpt_node *c )
{
	/* Stack:
	 * 2 ID
	 * 1 Screen Index
	*/
	struct sprite_data *d = (struct sprite_data*) c->user_data;

	lua_createtable( l, 0, 2 );
	lua_pushlightuserdata( l, d->tex );
	lua_setfield( l, -2, "tex" );
	set_field_int( l, "sx", d->src.x );
	set_field_int( l, "sy", d->src.y );
	set_field_int( l, "sw", d->src.w );
	set_field_int( l, "sh", d->src.h );
	set_field_int( l, "r", d->col.r );
	set_field_int( l, "g", d->col.g );
	set_field_int( l, "b", d->col.b );
	set_field_int( l, "a", d->col.a );
	return 1;
}

int sprite_udata_set( lua_State *l, struct cmpt_node *c )
{
	/* Stack:
	 * 3 Data table (6Or nOMOrNTE HAC...)
	 * 2 ID
	 * 1 Screen Index
	*/
	struct sprite_data *d = (struct sprite_data*) c->user_data;
	
	luaL_checktype( l, 3, LUA_TTABLE );
	lua_getfield( l, 3, "tex" );
	if ( lua_isuserdata( l, -1 ) ) {
		d->tex = (SDL_Texture*) lua_touserdata( l, -1 );
		printf( "Got userdata, it's %d\n", d->tex );
	}
	lua_getfield( l, 3, "sx" );
	if ( lua_isnumber( l, -1 ) ) {
		d->src.x = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 3, "sy" );
	if ( lua_isnumber( l, -1 ) ) {
		d->src.y = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 3, "sw" );
	if ( lua_isnumber( l, -1 ) ) {
		d->src.w = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 3, "sh" );
	if ( lua_isnumber( l, -1 ) ) {
		d->src.h = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 3, "r" );
	if ( lua_isnumber( l, -1 ) ) {
		d->col.r = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 3, "g" );
	if ( lua_isnumber( l, -1 ) ) {
		d->col.g = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 3, "b" );
	if ( lua_isnumber( l, -1 ) ) {
		d->col.b = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 3, "a" );
	if ( lua_isnumber( l, -1 ) ) {
		d->col.a = lua_tointeger( l, -1 );
	}
	lua_settop( l, 3 );
	return 0;
}