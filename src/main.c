#include "include/core.h"

int main( int argc, char *argv[] ) {
	COR_init( argc, argv );
	COR_begin();
	COR_cleanup();
	return 0;
}