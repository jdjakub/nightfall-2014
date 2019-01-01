#include "include/miniz.h"
#include "include/core.h"
#include "include/rsrc.h"

struct RES_file {
	mz_zip_archive ar;
};

static void *cvt_texture( void *raw, size_t size );

struct RES_file *RES_load_resfile( const char *filename )
{
	struct RES_file *f = (struct RES_file*) COR_alloc( sizeof( *f ), 0 );
	
	memset( f, 0, sizeof( *f ) );
	if ( !f || mz_zip_reader_init_file( &f->ar, filename, 0 ) == MZ_FALSE ) {
		COR_free( f );
		perror("RES_load_resfile");
		return NULL;
	}
	printf( "Resource file %d loaded\n", f );
	return f;
}

void RES_unload_resfile( struct RES_file *file )
{
	mz_zip_reader_end( &file->ar );
	COR_free( file );
	printf( "Resource file %d unloaded\n", file );
}

void *RES_get_data( struct RES_file *f, const RES_id id, size_t *datasize )
{
	size_t size;
	void *raw = mz_zip_reader_extract_file_to_heap( &f->ar, id, &size, 0 );
	if ( datasize ) {
		*datasize = size;
	}
	printf( "Raw resource data %d obtained\n", raw );
	return raw;
}

void RES_free_data( void *data )
{
	COR_free( data ); /* HACK which only works because default dealloc is free() */
	printf( "Raw resource data %d freed\n", data );
}

/*void *RES_get_data_copy( struct RES_file *f, const RES_id id, size_t *datasize)
{
	size_t size;
	void *data = RES_get_data( f, id, &size );
	
	if ( data ) {
		
	}
}*/