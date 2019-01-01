#ifndef GLUE_AUX_H
#define GLUE_AUX_H
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "ui_cmpts.h"
#define LUA_GLUE(func) int glue_##func( lua_State *l )

void set_field_int( lua_State *l, const char *kname, int val );

void set_field_bool( lua_State *l, const char *kname, int val );

void set_field_str( lua_State *l, const char *kname, const char *val );

void *check_data( lua_State *l, int arg );

#endif