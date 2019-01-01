#include <ctype.h>
#include <SDL2/SDL.h>
#include <FreeType2/ft2build.h>
#include FT_FREETYPE_H
#include "include/core.h"
#include "include/display.h"

extern FT_Library ft;

struct glyph_info {
	unsigned ax; /* Advance width */
	unsigned ay; /* Advance height */
	
	unsigned bw; /* Bitmap width */
	unsigned bh; /* Bitmap height */
	
	int bl; /* Left bearing */
	unsigned bt; /* Top bearing */
	
	unsigned tx; /* x-offset in atlas */
	unsigned ty; /* y-offset in atlas */
};

#define CHAR_START 0x20
#define CHAR_END 0x80

struct DSP_glyph_atlas {
	SDL_Surface *surf;
	unsigned line_height;
	unsigned lowest_descender;
	struct glyph_info info[CHAR_END];
};

struct DSP_glyph_atlas *DSP_create_glyph_atlas( void *fontdata, size_t size, int px_size )
{
	FT_Face face;
	struct DSP_glyph_atlas *atlas;
	FT_GlyphSlot g;
	int error;
	unsigned w, h, code, x;
	unsigned rmask, gmask, bmask, amask;
	
	if ( error = FT_New_Memory_Face( ft, fontdata, size, 0, &face ) ) {
		COR_fatal_error( "Couldn't create new font face from memory\n" );
	}
	
	FT_Set_Pixel_Sizes( face, 0, px_size );
	g = face->glyph;
	w=h=x=0;
	
	for ( code=CHAR_START; code<CHAR_END; code++ ) {
		if ( FT_Load_Char( face, code, FT_LOAD_RENDER ) == 0 ) {
			int rows = 0;
			
			w += g->bitmap.width;
			rows = g->bitmap.rows;
			h = h < rows ? rows : h;
			//printf( "%c: w=%u h=%u\n", code, w, h );
		}
	}
	//printf( "w=%u, h=%u\n", w, h );
	atlas = COR_alloc( sizeof( *atlas ), 1 );
	memset( atlas, 0, sizeof( *atlas ) );
	
	if ( !( atlas->surf = DSP_create_rgba_surf( w, h ) ) ) {
		goto free_atlas;
	} else {
		unsigned sw = atlas->surf->w;
		
		atlas->line_height = h;
		atlas->lowest_descender = 0;
		SDL_SetSurfaceBlendMode( atlas->surf, SDL_BLENDMODE_NONE );
		
		for ( code=CHAR_START; code<CHAR_END; code++ ) {
			//getc( stdin );
			if ( FT_Load_Char( face, code, FT_LOAD_RENDER ) == 0 ) {
				unsigned char *spix = g->bitmap.buffer;
				unsigned *dpix = (unsigned*) atlas->surf->pixels;
				unsigned row, col;
				struct glyph_info *info = &atlas->info[code];
				int descender;
				
				w = g->bitmap.width;
				h = g->bitmap.rows;
				//printf( "Processing '%c'. w=%u h=%u\n", code, w, h );
				for ( row=0; row<h; row++ ) {
					for ( col=0; col<w; col++ ) {
						unsigned grey = (unsigned) spix[row*w + col];
						unsigned surf_col = x+col;
						unsigned dst = row*sw+surf_col;
						//if ( row == 0 ) printf( "%x ", pix );
						//printf( "%c", grey == 0 ? ' ' : '#' );
						dpix[dst] = 0xFFFFFFFF;
						((unsigned char*)(dpix+dst))[3] &= (unsigned char) grey;
					}
					//printf( "\n" );
				}
				
				info->ax = g->advance.x >> 6;
				info->ay = g->advance.y >> 6;
				info->bw = w;
				info->bh = h;
				info->bl = g->bitmap_left;
				info->bt = g->bitmap_top;
				info->tx = x;
				info->ty = 0;
				//printf( "%c : ax=%u (%x) ay=%u (%x) bw=%u bh=%u bl=%u bt=%u tx=%u\n", code, info->ax, g->advance.x, info->ay, g->advance.y, w, h, info->bl, info->bt, info->tx );
				//getc(stdin);
				descender = h - info->bt;
				if ( descender < 0 ) descender = 0;
				if ( descender > atlas->lowest_descender) atlas->lowest_descender = descender;
				
				x += w;
			}
		}
	}
	goto free_face;
	
free_atlas:
	COR_free( atlas );
	atlas = NULL;
free_face:
	FT_Done_Face( face );
	return atlas;
}

struct text_ctx {
	struct DSP_glyph_atlas *atlas;
	SDL_Surface *surf;
	SDL_Rect *bounds;
	unsigned pen_x;
	unsigned pen_y;
	unsigned curr_w;
	unsigned curr_h;
	const char *c;
	const char *line_start;
	const char *ls; /* Last space on line */
};

/* Renders line up to but not including 'to', putting a line break before 'to' */
static void render_and_break( struct text_ctx *t, const char *to ) {
	const char *p = t->line_start;
	struct DSP_glyph_atlas *atlas = t->atlas;
	SDL_Rect *bounds = t->bounds;
	unsigned orig_pen_x = t->pen_x;
	unsigned baseline = t->pen_y + atlas->line_height;
	//printf( "Rendering from %p ('%c') and breaking before %p ('%c')\n", p, *p, to, *to );
	for ( ; p < to; p++ ) {
		char c = *p;
		struct glyph_info *info = &atlas->info[c];
		SDL_Rect src, dst;
		
		if ( info->bw != 0 ) { /* Skip actual render if it's empty eg space */
			unsigned pen_y = baseline - info->bt;
			
			src.x = info->tx;
			src.y = info->ty;
			src.w = info->bw;
			src.h = info->bh;
			dst.x = t->pen_x + info->bl;
			dst.y = pen_y;
		
			SDL_BlitSurface( atlas->surf, &src, t->surf, &dst );
		}
		t->pen_x += info->ax;
	}
	
	t->pen_x = orig_pen_x;
	t->pen_y += atlas->line_height;
	t->line_start = to;
	t->ls = NULL;
	t->curr_w = 0;
	t->curr_h += atlas->line_height;
}

/* Yep, why use SDL_ttf when you can roll your own complicated text renderer? */
void DSP_draw_text( struct DSP_glyph_atlas *atlas, SDL_Surface *surf, const char *str, SDL_Rect *bounds )
{
	struct text_ctx t = { atlas, surf, bounds, bounds->x, bounds->y, 0, 0, str, str, NULL };
	
	while ( t.c[0] ) {
		char c = t.c[0];
		struct glyph_info *info = &atlas->info[c];
		
		t.curr_w += info->ax; /* Advance */
		//printf( "On '%c'. ax=%u, curr_w=%u\n", c, info->ax, t.curr_w );
		//getc(stdin);
		if ( c == '\n' ) {
			render_and_break( &t, t.c+1 );
			t.c++;
		} else if ( t.curr_w > bounds->w ) {	
			//printf( "Overflown...\n" );
			if ( isspace( c ) ) {
				//printf( "Breaking here...\n" );
				render_and_break( &t, t.c+1 );
			} else if ( t.ls == NULL ) {
				render_and_break( &t, t.c );
			} else {
				//printf( "Breaking Bad...\n" );
				render_and_break( &t, t.ls+1 );
			}
			t.c = t.line_start;
		} else {
			if ( isspace( c ) ) {
				t.ls = t.c;
			}
			t.c++;
		}
	}
	render_and_break( &t, t.c ); /* Render last line with NULL terminator */
	bounds->h = t.curr_h + atlas->lowest_descender;
}