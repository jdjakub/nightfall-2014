#include <SDL2/SDL.h>
#include "include/core.h"
#include "include/display.h"

void DSP_draw_box( SDL_Surface *dst, SDL_Surface *src, struct DSP_box_desc *box, SDL_Rect *bounds )
{
	struct DSP_box_desc d = { 0 };
	SDL_Rect *r, *p;
	int dx, dy;
	
	memcpy( &d, box, sizeof d );
	dx = d.tl.x;
	dy = d.tl.y;
	/* Translate them all */
	for ( r=&d.tl; r<=&d.b; r++ ) {
		r->x += bounds->x - dx;
		r->y += bounds->y - dy;
	}
	d.centre.x += bounds->x - dx;
	d.centre.y += bounds->y - dy;
	/* Resize */
	d.t.w = bounds->w - d.tl.w - d.tr.w;
	d.b.w = bounds->w - d.bl.w - d.br.w;
	d.l.h = bounds->h - d.bl.h - d.tl.h;
	d.r.h = bounds->h - d.br.h - d.tr.h;
	d.tr.x = d.t.x + d.t.w;
	d.bl.y = d.l.y + d.l.h;
	d.br.x = d.b.x + d.b.w;
	d.br.y = d.r.y + d.r.h;
	d.centre.w = d.t.w;
	d.centre.h = d.l.h;
	
	/* Draw */
	r = &d.tl;
	p = &box->tl;
	
	SDL_SetSurfaceColorMod( src, 255,  255, 255 );
	while ( r <= &d.br ) {
		SDL_BlitSurface( src, p, dst, r );
		r++;
		p++;
	}
	while ( r <= &d.b ) {
		SDL_BlitScaled( src, p, dst, r );
		r++;
		p++;
	}
	
	SDL_BlitScaled( src, &box->centre, dst, &d.centre );
	
	memcpy( box, &d, sizeof *box );
}