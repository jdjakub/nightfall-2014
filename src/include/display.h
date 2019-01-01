#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>

struct DSP_glyph_atlas;

struct DSP_settings {
	int fullscreen;
	int w;
	int h;
	int r, g, b, a;
	const char *title;
};

struct DSP_box_desc {
	SDL_Rect tl, tr, bl, br;
	SDL_Rect t, l, r, b;
	SDL_Rect centre;
};

void DSP_init( void );
void DSP_cleanup( void );

void DSP_get_settings( struct DSP_settings *s );
void DSP_set_fullscreen( int fullscreen );
void DSP_set_resolution( unsigned w, unsigned h );
void DSP_set_clear_col( unsigned char *components );
void DSP_set_title( const char *title );

void DSP_begin_frame( void );
void DSP_draw_frame( int delta );
void DSP_end_frame( void );

SDL_Texture *DSP_img_to_texture( void *imgdata, size_t size );
/* UNFORTUNATELY only ASCII for now... MUST put in simple UTF-8 support */
struct DSP_glyph_atlas *DSP_create_glyph_atlas( void *fontdata, size_t size, int px_size );
//void DSP_get_text_dims( struct DSP_glyph_atlas *atlas, const char *str, int *w, int *h );
SDL_Surface *DSP_create_rgba_surf( unsigned w, unsigned h );
SDL_Texture *DSP_tex_from_surf( SDL_Surface *surf );
void DSP_draw_text( struct DSP_glyph_atlas *atlas, SDL_Surface *surf, const char *str, SDL_Rect *bounds );
void DSP_draw_box( SDL_Surface *dst, SDL_Surface *src, struct DSP_box_desc *box, SDL_Rect *bounds );

#endif