#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "include/glue_display.h"
#include "include/display.h"
#include "include/ui.h"
#include "include/uidata.h"

/* LUA_SETTOP!! */

static struct cmpt_node *get_set_base( lua_State *l )
{
	/* Stack:
	 * 3 Table of data [?]
	 * 2 ID
	 * 1 Screen
	*/
	int idx;
	UI_cmpt_id id;
	struct cmpt_list *scr;
	struct cmpt_node *comp;
	
	scr = check_data( l, 1 );
	id = luaL_checkinteger( l, 2 );
	comp = find_cmpt( scr->first, id );
	luaL_argcheck( l, comp, 2, "no such component" );
	
	return comp;
}

static void fill_rect( lua_State *l, SDL_Rect *r, int tableIndex )
{
	lua_getfield( l, tableIndex, "x" );
	if ( lua_isnumber( l, -1 ) ) {
		r->x = lua_tointeger( l, -1 );
	}
	lua_getfield( l, tableIndex, "y" );
	if ( lua_isnumber( l, -1 ) ) {
		r->y = lua_tointeger( l, -1 );
	}
	lua_getfield( l, tableIndex, "w" );
	if ( lua_isnumber( l, -1 ) ) {
		r->w = lua_tointeger( l, -1 );
	}
	lua_getfield( l, tableIndex, "h" );
	if ( lua_isnumber( l, -1 ) ) {
		r->h = lua_tointeger( l, -1 );
	}
}

LUA_GLUE( getSettings )
{
	struct DSP_settings s = { 0 };
	
	DSP_get_settings( &s );
	lua_createtable( l, 0, 8 );
	set_field_bool( l, "fullscreen", s.fullscreen );
	set_field_str( l, "title", s.title );
	set_field_int( l, "w", s.w );
	set_field_int( l, "h", s.h );
	set_field_int( l, "r", s.r );
	set_field_int( l, "g", s.g );
	set_field_int( l, "b", s.b );
	set_field_int( l, "a", s.a );
	
	return 1;
}

LUA_GLUE( setSettings ) /* settings */
{
	int w = 0;
	int h = 0;
	unsigned char cols[4] = { 0 };
	
	luaL_checktype( l, 1, LUA_TTABLE );
	
	lua_getfield( l, 1, "fullscreen" );
	if ( lua_isboolean( l, -1 ) ) {
		DSP_set_fullscreen( lua_toboolean( l, -1 ) );
	}
	lua_getfield( l, 1, "w" );
	if ( lua_isnumber( l, -1 ) ) {
		w = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 1, "h" );
	if ( lua_isnumber( l, -1 ) ) {
		h = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 1, "r" );
	if ( lua_isnumber( l, -1 ) ) {
		cols[0] = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 1, "g" );
	if ( lua_isnumber( l, -1 ) ) {
		cols[1] = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 1, "b" );
	if ( lua_isnumber( l, -1 ) ) {
		cols[2] = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 1, "a" );
	if ( lua_isnumber( l, -1 ) ) {
		cols[3] = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 1, "title" );
	if ( lua_isstring( l, -1 ) ) {
		DSP_set_title( lua_tostring( l, -1 ) );
	}
	DSP_set_resolution( w, h );
	DSP_set_clear_col( cols );
	return 0;
}

LUA_GLUE( newScreen )
{
	struct cmpt_list *scr = UI_new_screen();
	
	if ( scr ) {
		lua_pushlightuserdata( l, scr );
		return 1;
	} else {
		luaL_error( l, "Couldn't create new screen" );
		return 0;
	}
}

LUA_GLUE( setScreen ) /* idx, screen */
{
	int idx = luaL_checkinteger( l, 1 );
	struct cmpt_list *scr = check_data( l, 2 );
	
	UI_set_screen( idx, scr );
	return 0;
}

LUA_GLUE( delScreen ) /* idx */
{
	int idx = luaL_checkinteger( l, 1 );
	
	UI_del_screen( idx );
	return 0;
}

LUA_GLUE( addComponent ) /* screen, type, layer */
{
	UI_cmpt_type type;
	int layer, id;
	struct cmpt_list *scr = check_data( l, 1 );
	
	type = luaL_checkinteger( l, 2 );
	layer = luaL_checkinteger( l, 3 );
	luaL_argcheck( l, type < UI_LAST, 1, "invalid component type" );
	
	id = UI_add_cmpt( scr, type, layer );
	
	lua_pushinteger( l, id );
	return 1;
}

LUA_GLUE( delComponent ) /* screen, id */
{
	struct cmpt_list *scr = check_data( l, 1 );
	int id = luaL_checkinteger( l, 2 );
	
	UI_del_cmpt( scr, id );
	return 0;
}

LUA_GLUE( getCmptBaseData ) /* idx, id */
{
	struct cmpt_node c = { 0 };
	struct cmpt_node *comp = get_set_base( l );
	/* Need to send back type, id, groupId, layer, visible, bounds */
	cmpt_get_data( comp, &c );
	lua_createtable( l, 0, 9 );
	set_field_int( l, "type", c.type );
	set_field_int( l, "id", c.id );
	set_field_int( l, "groupId", c.group_id );
	set_field_int( l, "layer", c.layer );
	set_field_bool( l, "visible", c.visible );
	set_field_int( l, "x", c.bounds.x );
	set_field_int( l, "y", c.bounds.y );
	set_field_int( l, "w", c.bounds.w );
	set_field_int( l, "h", c.bounds.h );
	
	return 1;
}

LUA_GLUE( setCmptBaseData ) /* screen, id, data */
{
	struct cmpt_node c = { 0 };
	struct cmpt_node *comp = get_set_base( l );
	
	cmpt_get_data( comp, &c );
	luaL_checktype( l, 3, LUA_TTABLE);
	
	lua_getfield( l, 3, "groupId" );
	if ( lua_isnumber( l, -1 ) ) {
		c.group_id = lua_tointeger( l, -1 );
	}
	lua_getfield( l, 3, "visible" );
	if ( lua_isboolean( l, -1 ) ) {
		c.visible = lua_toboolean( l, -1 );
	}
	fill_rect( l, &c.bounds, 3 );
	
	cmpt_set_data( comp, &c );
	
	return 0;
}

LUA_GLUE( getCmptUserData ) /* screen, id */
{
	struct cmpt_node *comp = get_set_base( l );
	return cmpt_funcs[comp->type].udata_get( l, comp );
}

LUA_GLUE( setCmptUserData ) /* screen, id, data */
{
	struct cmpt_node *comp = get_set_base( l );
	return cmpt_funcs[comp->type].udata_set( l, comp );
}

LUA_GLUE( getTextureDims ) /* tex */
{
	SDL_Texture *tex = check_data( l, 1 );
	int w, h;
	
	SDL_QueryTexture( tex, NULL, NULL, &w, &h );
	lua_pushinteger( l, w );
	lua_pushinteger( l, h );
	
	return 2;
}

LUA_GLUE( createGlyphAtlas ) /* fontData, size, pxSize */
{
	void *data = check_data( l, 1 );
	size_t size = luaL_checkinteger( l, 2 );
	size_t px_size = luaL_checkinteger( l, 3 );
	struct DSP_glyph_atlas *atlas = DSP_create_glyph_atlas( data, size, px_size );
	
	if ( atlas ) {
		lua_pushlightuserdata( l, atlas );
		return 1;
	} else {
		luaL_error( l, "Couldn't create glyph atlas" );
		return 0;
	}
}

/*LUA_GLUE( getTextDims ); /* atlas, str 
{
	struct DSP_glyph_atlas *atlas = check_data( l );
	const char *str = luaL_checkstring( l, 2 );
	int w, h;
	
	DSP_get_text_dims( atlas, str, &w, &h );
	
	lua_pushinteger( l, w );
	lua_pushinteger( l, h );
	return 2;
}*/

LUA_GLUE( createSurface ) /* w, h */
{
	unsigned w = luaL_checkinteger( l, 1 );
	unsigned h = luaL_checkinteger( l, 2 );
	SDL_Surface *surf = DSP_create_rgba_surf( w, h );
	
	if ( surf ) {
		lua_pushlightuserdata( l, surf );
		return 1;
	} else {
		luaL_error( l, "Could not create surface." );
		return 0;
	}
}

LUA_GLUE( createTexture ) /* surf */
{
	SDL_Surface *surf = check_data( l, 1 );
	SDL_Texture *tex = DSP_tex_from_surf( surf );
	
	if ( tex ) {
		lua_pushlightuserdata( l, tex );
		return 1;
	} else {
		luaL_error( l, "Could not create texture from surface." );
		return 0;
	}
}

LUA_GLUE( freeSurface ) /* surf */
{
	SDL_Surface *surf = check_data( l, 1 );
	
	SDL_FreeSurface( surf );
	return 0;
}

LUA_GLUE( freeTexture ) /* tex */
{
	SDL_Texture *tex = check_data( l, 1 );
	
	SDL_DestroyTexture( tex );
}

LUA_GLUE( updateTexture ) /* tex, surf */
{
	SDL_Texture *tex = check_data( l, 1 );
	SDL_Surface *surf = check_data( l, 2 );
	
	SDL_SaveBMP( surf, "surface.bmp" );
	SDL_UpdateTexture( tex, NULL, surf->pixels, surf->pitch );
	return 0;
}

LUA_GLUE( drawText ) /* atlas, surf, str, bounds */
{
	struct DSP_glyph_atlas *atlas = check_data( l, 1 );
	SDL_Surface *surf = check_data( l, 2 );
	const char *str = luaL_checkstring( l, 3 );
	SDL_Rect bounds = { 0 };
	
	luaL_argcheck( l, str[1] != '\0', 3, "str cannot be empty" );
	
	luaL_checktype( l, 4, LUA_TTABLE );
	fill_rect( l, &bounds, 4 );
	
	luaL_argcheck( l, bounds.w > 0, 4, "text width cannot be 0" );
	
	DSP_draw_text( atlas, surf, str, &bounds );
	
	lua_pushinteger( l, bounds.h );
	return 1;
}

LUA_GLUE( drawBox ) /* dst, src, desc, bounds */
{
	SDL_Surface *dst = check_data( l, 1 );
	SDL_Surface *src = check_data( l, 2 );
	struct DSP_box_desc d = { 0 };
	SDL_Rect bounds = { 0 };
	SDL_Rect *r = &d.tl;
	static const char *rects[] = {
		"tl", "tr", "bl", "br",
		"t", "l", "r", "b",
		"centre"
	};
	int i;
	
	luaL_checktype( l, 3, LUA_TTABLE );
	for ( i=0; i<ARR_LEN( rects ); i++ ) {
		const char *curr = rects[i];
		
		lua_getfield( l, 3, curr );
		luaL_checktype( l, 5, LUA_TTABLE );
		lua_getfield( l, 5, "x" );
		lua_getfield( l, 5, "y" );
		lua_getfield( l, 5, "w" );
		lua_getfield( l, 5, "h" );
		r->x = luaL_checkinteger( l, 6 );
		r->y = luaL_checkinteger( l, 7 );
		r->w = luaL_checkinteger( l, 8 );
		r->h = luaL_checkinteger( l, 9 );
		lua_settop( l, 4 );
		r++;
	}
	
	luaL_checktype( l, 4, LUA_TTABLE );
	lua_getfield( l, 4, "x" );
	lua_getfield( l, 4, "y" );
	lua_getfield( l, 4, "w" );
	lua_getfield( l, 4, "h" );
	bounds.x = luaL_checkinteger( l, 5 );
	bounds.y = luaL_checkinteger( l, 6 );
	bounds.w = luaL_checkinteger( l, 7 );
	bounds.h = luaL_checkinteger( l, 8 );
	lua_settop( l, 4 );
	
	DSP_draw_box( dst, src, &d, &bounds );
	
	lua_createtable( l, 0, 9 );
	r = &d.tl;
	for ( i=0; i<ARR_LEN( rects ); i++ ) {
		const char *curr = rects[i];
		
		lua_createtable( l, 0, 4 );
		set_field_int( l, "x", r->x );
		set_field_int( l, "y", r->y );
		set_field_int( l, "w", r->w );
		set_field_int( l, "h", r->h );
		lua_setfield( l, -2, curr );
		r++;
	}
	return 1;
}