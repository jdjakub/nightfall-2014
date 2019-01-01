#include "include/glue_aux.h"

void set_field_int( lua_State *l, const char *kname, int val )
{
	lua_pushinteger( l, val );
	lua_setfield( l, -2, kname );
}

void set_field_bool( lua_State *l, const char *kname, int val )
{
	lua_pushboolean( l, val );
	lua_setfield( l, -2, kname );
}

void set_field_str( lua_State *l, const char *kname, const char *val )
{
	lua_pushstring( l, val );
	lua_setfield( l, -2, kname );
}
