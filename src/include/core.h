#ifndef CORE_H
#define CORE_H
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <bstrlib.h>
#define ARR_LEN(x) ( sizeof( x ) / sizeof( x[0] ) )

/* CODE RULES:	- Functions returning bstrings are assumed to return new bstrings by default
 *				- 
 */

void COR_init( int argc, char *argv[] );
void COR_begin( void );
void COR_cleanup( void );

void COR_fatal_error( const char *format, ... );
void *COR_alloc( size_t size, int critical );
void COR_free( void *data );

#endif