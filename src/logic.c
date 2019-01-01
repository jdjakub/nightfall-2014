#include <SDL2/SDL.h>
#include "include/core.h"
#include "include/logic.h"
#include "include/rsrc.h"
#include "include/script.h"
#include "include/display.h"
#include "include/event.h"
#define DEFAULT_FPS 60.0

static int keepRunning = 1;
static float ms_per_frame = 1000.0/DEFAULT_FPS;
static unsigned last_tick = 0;
static unsigned delta = 0;

static void load_settings( void );
static void exec_main_script( void );
static void main_loop_begin( void );

void LGC_init( void )
{
	load_settings();
	exec_main_script();
}

/* All systems initialised and ready to use when this is called. */
void LGC_begin( void )
{
	last_tick = SDL_GetTicks();
	
	while ( keepRunning ) {
		main_loop_begin();
		EVT_poll_and_handle_usr_evts( &keepRunning );
		DSP_begin_frame();
		DSP_draw_frame( delta );
		DSP_end_frame();
	}
}

static void load_settings( void )
{
	char col[] = {0xFF, 0x00, 0xFF, 0xFF};
	DSP_set_clear_col( col );
	DSP_set_title( "Nightfall" );
}

void exec_main_script( void )
{
	/*struct RES_file *main_file = RES_load_resfile( "rsrc/main.zip" );
	
	if ( main_file ) {
		size_t size;
		void *main_script = RES_get_data( main_file, "main.lua", &size );
		
		RES_unload_resfile( main_file );
		if ( main_script ) {
			SCR_exec_buf( main_script, size, NULL );
			RES_free_data( main_script );
		} else {
			COR_fatal_error( "Fatal error: couldn't load essential script main.lua\n" );
		}
	} else {
		COR_fatal_error( "Fatal error: couldn't load main.zip\n" );
	}*/
	SCR_exec_string( "nightfallMain()", NULL );
}

void main_loop_begin( void )
{
	int curr_tick, sleep;
	curr_tick = SDL_GetTicks();
	delta = curr_tick - last_tick;
	sleep = ms_per_frame - delta;
	
	/* If the last frame ended early, make up for it */
	if ( sleep > 0 ) {
		SDL_Delay( sleep );
	}
	last_tick = SDL_GetTicks();
	/* Otherwise - continue as soon as possible */
}