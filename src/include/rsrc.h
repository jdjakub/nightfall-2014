#ifndef RSRC_H
#define RSRC_H
#include "core.h"

typedef const char *RES_id;

struct RES_file;

struct RES_file *RES_load_resfile( const char *filename );
void RES_unload_resfile( struct RES_file *file );

void *RES_get_data( struct RES_file *f, const RES_id id, size_t *datasize );
void RES_free_data( void *data );

/*void *RES_get_data_copy( struct RES_file *f, const RES_id id, size_t *datasize ); /* Alloc'ed with COR_alloc() */

#endif