#include <SDL2/SDL.h>
#include "include/core.h"
#include "include/event.h"
#include "include/ui.h"

void EVT_poll_and_handle_usr_evts( int *keepRunning )
{
	SDL_Event e;
	/* TODO [low-p] - BINDINGS */
	while ( SDL_PollEvent( &e ) ) {
		switch ( e.type ) {
			case SDL_QUIT:
				*keepRunning = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if ( e.button.button == SDL_BUTTON_LEFT ) {
					SDL_Point p = {e.button.x, e.button.y};
					struct cmpt_node *cmpt = UI_find_closest_containing( 0, p );
					if ( cmpt ) {
						SDL_Point rel = {p.x - cmpt->bounds.x, p.y-cmpt->bounds.y};
						
						printf( "Component #%d, group %d deemed clicked with the left mouse button.\n", cmpt->id, cmpt->group_id );
						
						SCR_notify_input( UI_get_screen( 0 ), cmpt->group_id, cmpt->id, NULL );
					}
				}
				break;
			case SDL_KEYDOWN:
				printf( "'ey up, got a key down event...\n" );
		}
	}
}