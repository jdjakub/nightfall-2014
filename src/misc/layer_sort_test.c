#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cmpt_node {
    int layer;
	struct cmpt_node *next;
};

struct cmpt_node *alloc_cmpt( void );
void free_cmpt( struct cmpt_node *c );
struct cmpt_node *new_cmpt( int layer );
void insert_cmpt( struct cmpt_node **ptr, struct cmpt_node *c );
void insert_layer_sorted( struct cmpt_node **ptr, struct cmpt_node *c );

int main(void) {
	struct cmpt_node *list = NULL;
	int layers[] = {10,9,8,7,6,5,4,3,2,1};
	int i;
    struct cmpt_node *c = NULL;
	
	for ( i=0; i<10; i++ ) {
        printf( "%d\n", i );
		insert_layer_sorted( &list, new_cmpt( layers[i] ) );
	}
    
    for ( c=list; c!=NULL; c=c->next ) {
        printf( "[%d]->", c->layer );
    }
	return 0;
}

struct cmpt_node *alloc_cmpt( void )
{
	struct cmpt_node *c = malloc( sizeof(*c) );
	return c;
}

void free_cmpt( struct cmpt_node *c ) {
	free( c );
}


struct cmpt_node *new_cmpt( int layer )
{
	struct cmpt_node *c = alloc_cmpt();
	
	if ( c ) {
		memset( (void*) c, 0, sizeof(*c) );
		c->layer = layer;
	}
	
	return c;
}

void insert_cmpt( struct cmpt_node **ptr, struct cmpt_node *c )
{
	struct cmpt_node *next = *ptr;
	*ptr = c;
	c->next = next;
}

void insert_layer_sorted( struct cmpt_node **ptr, struct cmpt_node *c )
{
	struct cmpt_node *curr = NULL;
	
	while ( 1 ) {
		curr = *ptr;
		
		if ( !curr ) { /* Insert at end if we're at the end */
			*ptr = c;
			return;
		} else if ( curr->layer > c->layer ) {
			insert_cmpt( ptr, c );
			return;
		} else {
			ptr = &curr->next;
		}
	}
}
