#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "include/core.h"
#include "include/ui.h"
#include "include/uidata.h"
#include "include/display.h"
#define MAX_SCREENS 2

static SDL_Renderer *rnd = NULL;
struct cmpt_list *screens[MAX_SCREENS] = { 0 };

void fill_func_lists( void );

void UI_init( SDL_Renderer *r )
{
	rnd = r;
	fill_func_lists();
}

void UI_cleanup( void )
{
	int i;
	
	for ( i=0; i<MAX_SCREENS; i++ ) {
		struct cmpt_list *scr = screens[i];
		
		if ( scr ) {
			UI_del_screen( i );
		}
	}
}

struct cmpt_list *UI_new_screen()
{
	return new_list();
}

struct cmpt_list *UI_get_screen( int idx )
{
	return screens[idx];
}

void UI_set_screen( int idx, struct cmpt_list *scr )
{
	screens[idx] = scr;
}

void UI_del_screen( int idx )
{
	struct cmpt_list *scr = screens[idx];
	
	screens[idx] = NULL;
	del_list( scr );
}

void UI_draw_frame( int delta )
{
	int i;
	
	for ( i=0; i<MAX_SCREENS; i++ ) {
		if ( screens[i] ) {
			struct cmpt_node *curr = screens[i]->first;
			
			while ( curr ) {
				if ( curr->visible ) {
					cmpt_funcs[curr->type].draw( curr, rnd );
				}
				//printf( "On cmpt #%u! (%d) - group %u, %s\n", curr->id, curr->layer, curr->group_id, curr->visible ? "visible" : "not visible" );
				SDL_SetRenderDrawColor( rnd, 0, 255, 0, 255 );
				SDL_RenderDrawRect( rnd, &curr->bounds );
				curr = curr->next;
			}
		}
	}
}

UI_cmpt_id UI_add_cmpt( struct cmpt_list *scr, UI_cmpt_type type, int layer )
{
	struct cmpt_node *cmpt = new_cmpt( type, layer );
	
	if ( !cmpt ) {
		return 0;
	} else {	
		insert_layer_sorted( scr, cmpt );
		return cmpt->id;
	}
}

void UI_del_cmpt( struct cmpt_list *scr, UI_cmpt_id id )
{
	struct cmpt_node *c = NULL;
	
	for ( c=scr->first; c!=NULL; c=c->next ) {
		if ( c->id == id ) {
			del_cmpt( remove_cmpt( scr, c ) );
			printf( "Deleted component #%u", id );
			return;
		}
	}
}

struct cmpt_node *UI_find_closest_containing( int idx, SDL_Point p )
{
	struct cmpt_list *scr = screens[idx];
	struct cmpt_node *c = scr->last;
	
	/* Now traverse backwards until finding an intersection */
	while ( c ) {
		if ( SDL_EnclosePoints( &p, 1, &c->bounds, NULL ) ) {
			return c;
		} else if ( c == scr->first ) {
			break;
		} else {
			c = c->prev;
		}
	}
	return NULL;
}