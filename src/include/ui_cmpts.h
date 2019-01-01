#ifndef UI_CMPTS_H
#define UI_CMPTS_H
#include <SDL2/SDL.h>
#include <lua.h>
#include "core.h"
#define PROTO_INIT(func) void func( struct cmpt_node* )
#define PROTO_DRAW(func) void func( struct cmpt_node*, SDL_Renderer* )
#define PROTO_FREE(func) void func( struct cmpt_node* )
#define PROTO_UDATA(func) int func( lua_State *, struct cmpt_node* )

typedef enum {
	UI_SPRITE,
	UI_LAST /* Do not use - simply here for enum count */
} UI_cmpt_type;

typedef enum {
	UIM_NULL,
	UIM_GET_DATA,
	UIM_SET_DATA,
	UIM_GET_UDATA,
	UIM_SET_UDATA,
} UI_msg;

typedef unsigned UI_cmpt_id;

struct cmpt_node {
	struct cmpt_node *next; /* Next cmpt in list */
	struct cmpt_node *prev;	/* Prev node in list */
	UI_cmpt_type type; 	/* Type of cmpt used to select appropriate funcs */
	UI_cmpt_id id;		/* UNIQUE ID for this specific component */
	int group_id;		/* ID of the component group to which this one belongs */
	int layer;			/* Cmpts with lower values are drawn [FIRST/LAST??] */
	int visible;		/* Drawn if visible, skipped over if not */
	SDL_Rect bounds;    /* Dims / click bounds */
	void *user_data;	/* Component-specific data */
};

typedef void (*cmpt_init_func)( struct cmpt_node *c );
typedef void (*cmpt_draw_func)( struct cmpt_node *c, SDL_Renderer *r );
typedef void (*cmpt_free_func)( struct cmpt_node *c );
typedef int (*cmpt_udata_func)( lua_State *l, struct cmpt_node *c );

struct cmpt_reg {
	cmpt_init_func init;
	cmpt_draw_func draw;
	cmpt_free_func free;
	cmpt_udata_func udata_get;
	cmpt_udata_func udata_set;
};
struct cmpt_reg cmpt_funcs[UI_LAST+1];

#endif