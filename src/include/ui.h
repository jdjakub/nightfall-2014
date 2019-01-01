#ifndef UI_H
#define UI_H
#include <SDL2/SDL.h>
#include "core.h"
#include "ui_cmpts.h"

void UI_init( SDL_Renderer *rnd );
void UI_cleanup( void );

struct cmpt_list *UI_new_screen( void );
struct cmpt_list *UI_get_screen( int idx );
void UI_set_screen( int idx, struct cmpt_list *scr );
void UI_del_screen( int idx );

void UI_draw_frame( int delta );

UI_cmpt_id UI_add_cmpt( struct cmpt_list *scr, UI_cmpt_type type, int layer );
void UI_del_cmpt( struct cmpt_list *scr, UI_cmpt_id id );

struct cmpt_node *UI_find_closest_containing( int idx, struct SDL_Point p );

#endif