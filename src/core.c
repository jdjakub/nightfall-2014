#include <SDL2/SDL.h>
#include "include/core.h"
#include "include/rsrc.h"
#include "include/script.h"
#include "include/display.h"
#include "include/logic.h"

void COR_init( int argc, char *argv[] )
{
	if ( SDL_Init( SDL_INIT_EVERYTHING != 0 ) ) {
		COR_fatal_error( "SDL Initialisation failed: %s\n", SDL_GetError() );
	}
	SCR_init();
	DSP_init();
	LGC_init();
}

void COR_begin( void )
{
	LGC_begin();
}

void COR_cleanup( void )
{
	DSP_cleanup();
	SCR_cleanup();
	SDL_Quit();
}

void COR_fatal_error( const char *format, ... )
{
	va_list args;
	va_start( args, format );
	vfprintf( stderr, format, args );
	va_end( args );
	
	COR_cleanup();
	exit( -1 );
}

void *COR_alloc( size_t size, int critical )
{
	void *ptr = malloc( size );
	if ( !ptr ) {
		if ( critical ) {
			COR_fatal_error( "Fatal error: critical allocation for %u bytes failed\n", size );
		} else {
			fprintf( stderr, "Warning: allocation returned 0\n" );
		}
	}
	return ptr;
}

void COR_free( void *data )
{
	free( data );
}