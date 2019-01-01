#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <FreeType2/ft2build.h>
#include FT_FREETYPE_H
#include "include/core.h"
#include "include/display.h"
#include "include/ui.h"
#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

FT_Library				ft				= NULL;
static SDL_Window		*wnd			= NULL;
static SDL_Renderer		*rnd			= NULL;
static unsigned char	clear_col[4]	= {0, 0, 0, 0};
static int				fullscreen		= 0;

void DSP_init( void )
{
	if ( !( wnd = SDL_CreateWindow( "Nightfall",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									DEFAULT_WIDTH, DEFAULT_HEIGHT,
									SDL_WINDOW_SHOWN ) ) ) {
		COR_fatal_error( "Failed to create SDL window: %s\n", SDL_GetError() );
	}
	if ( !( rnd = SDL_CreateRenderer( wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ) ) ) {
		COR_fatal_error( "Failed to create SDL renderer: %s\n", SDL_GetError() );
	}
	if ( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG != IMG_INIT_PNG ) {
		COR_fatal_error( "Couldn't init SDL_image: %s\n", IMG_GetError() );
	}
	if ( FT_Init_FreeType( &ft ) != 0 ) {
		COR_fatal_error( "Couldn't init FreeType\n" );
	}
	UI_init( rnd );
}

void DSP_cleanup( void )
{
	UI_cleanup();
	FT_Done_FreeType( ft );
	IMG_Quit();
	if (wnd) SDL_DestroyRenderer( rnd );
	if (rnd) SDL_DestroyWindow( wnd );
}

void DSP_get_settings( struct DSP_settings *s )
{
	s->fullscreen = fullscreen;
	SDL_GetWindowSize( wnd, &s->w, &s->h );
	s->r = clear_col[0];
	s->g = clear_col[1];
	s->b = clear_col[2];
	s->a = clear_col[3];
	s->title = SDL_GetWindowTitle( wnd );
}

void DSP_set_fullscreen( int yes )
{
	fullscreen = yes;
	SDL_SetWindowFullscreen( wnd, fullscreen ? SDL_WINDOW_FULLSCREEN : 0 );
}

void DSP_set_resolution( unsigned w, unsigned h )
{
	if ( w && h ) {
		SDL_SetWindowSize( wnd, w, h );
	}
}

void DSP_set_clear_col( unsigned char *components )
{
	clear_col[0] = components[0];
	clear_col[1] = components[1];
	clear_col[2] = components[2];
	clear_col[3] = components[3];
}

void DSP_set_title( const char *title )
{
	SDL_SetWindowTitle( wnd, title );
}

void DSP_begin_frame( void )
{
	SDL_SetRenderDrawColor( rnd, clear_col[0], clear_col[1], clear_col[2], clear_col[3] );
	SDL_RenderClear( rnd );
}

void DSP_draw_frame( int delta ) {
	UI_draw_frame( delta );
	//printf( "Drew a frame. Delta = %dms\n", delta );
}

void DSP_end_frame( void )
{
	SDL_RenderPresent( rnd );
}

SDL_Texture *DSP_img_to_texture( void *imgdata, size_t size )
{
	SDL_Surface *surf = IMG_Load_RW( SDL_RWFromMem( imgdata, size ), 1 );
	if ( surf ) {
		SDL_Texture *tex = SDL_CreateTextureFromSurface( rnd, surf );
		
		SDL_FreeSurface( surf );
		return tex;
	} else {
		return NULL;
	}
}

SDL_Surface *DSP_create_rgba_surf( unsigned w, unsigned h )
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	return SDL_CreateRGBSurface( 0, w, h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF );
#else
	return SDL_CreateRGBSurface( 0, w, h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 );
#endif
}

SDL_Texture *DSP_tex_from_surf( SDL_Surface *surf )
{
	return SDL_CreateTextureFromSurface( rnd, surf );
}