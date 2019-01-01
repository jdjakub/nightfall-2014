#include "include/ui_cmpts.h"

PROTO_INIT(sprite_init);

PROTO_DRAW(sprite_draw);

PROTO_FREE(sprite_free);

PROTO_UDATA(sprite_udata_get);

PROTO_UDATA(sprite_udata_set);

void fill_func_lists( void )
{
	/* Horrible, horrible way of doing things. But I know no alternatives. */
	/* INIT */
	cmpt_funcs[UI_SPRITE] = (struct cmpt_reg){sprite_init, sprite_draw, sprite_free, sprite_udata_get, sprite_udata_set};
}