#ifndef SCRIPT_H
#define SCRIPT_H
#include "core.h"
#define luaL_dobuf(l, buf, size) (luaL_loadbuffer(l, buf, size, NULL) || lua_pcall(l, 0, LUA_MULTRET, 0))

typedef void ( SCR_error_func )( const char *error );

typedef enum {
	SCR_INT,
	SCR_STRING,
	SCR_BOOLEAN,
} SCR_data_type;

struct SCR_val_desc {
	SCR_data_type type;
	size_t size;
	const char *name;
};

void SCR_init( void );
void SCR_cleanup( void );

int SCR_exec_buf( const void *buf, size_t size, SCR_error_func error_func );
int SCR_exec_string( const char *source, SCR_error_func error_func );
void SCR_notify_input( void *scr, unsigned group_id, unsigned cmpt_id, void *data);
/*
int SCR_fill_struct_from_table( const char *table, struct SCR_val_desc *descs, size_t nvals, void *s );
*/
#endif