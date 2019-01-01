#include "include/uidata.h"

struct cmpt_list *new_list( void )
{
	struct cmpt_list *list = COR_alloc( sizeof *list, 1 );
	memset( list, 0, sizeof *list );
	return list;
}

void del_list( struct cmpt_list *list )
{
	COR_free( list );
}

struct cmpt_node *alloc_cmpt( void )
{
	struct cmpt_node *c = COR_alloc( sizeof *c, 1 );
	return c;
}

void free_cmpt( struct cmpt_node *c ) {
	COR_free( c );
}

struct cmpt_node *new_cmpt( UI_cmpt_type type, int layer )
{
	static UI_cmpt_id next_id = 1;
	struct cmpt_node *c = alloc_cmpt();
	
	if ( c ) {
		memset( (void*) c, 0, sizeof *c );
		c->type = type;
		c->id = next_id++;
		c->layer = layer;
		
		cmpt_funcs[type].init( c );
		printf( "Cmpt #%d created\n", c->id );
	}
	
	return c;
}

void del_cmpt( struct cmpt_node *c )
{
	printf( "Cmpt #%d deleted\n", c->id );
	cmpt_funcs[c->type].free( c );
	free_cmpt( c );
}

/* Inserts after given node. To insert at beginning, affterThis = list */
void insert_cmpt( struct cmpt_list *list, struct cmpt_node *afterThis, struct cmpt_node *c )
{
	c->next = afterThis->next;
	if ( c->next ) {
		c->next->prev = c;
	} else {
		list->last = c;
	}
	afterThis->next = c;
	c->prev = afterThis;
}

/* Removes and returns given node. */
struct cmpt_node *remove_cmpt( struct cmpt_list *list, struct cmpt_node *c )
{
	struct cmpt_node *prev = c->prev;
	
	prev->next = c->next;
	if ( c->next ) {
		c->next->prev = prev;
	} else {
		list->last = prev;
	}
}

void del_cmpts( struct cmpt_node *afterThis )
{
	struct cmpt_node *c = afterThis->next;
	struct cmpt_node *tmp = NULL;
	
	afterThis->next = NULL; /* Remove link */
	while ( c ) {
		tmp = c->next;
		del_cmpt( c );
		c = tmp;
	}
}

void insert_layer_sorted( struct cmpt_list *list, struct cmpt_node *c )
{
	struct cmpt_node *curr = (struct cmpt_node*) list;
	
	while ( curr->next ) {
		if ( curr->next->layer > c->layer ) {
			insert_cmpt( list, curr, c );
			return;
		} else {
			curr = curr->next;
		}
	}
	insert_cmpt( list, curr, c );
}

struct cmpt_node *find_cmpt( struct cmpt_node *start, UI_cmpt_id id )
{
	struct cmpt_node *curr = start;
	
	while ( curr ) {
		if ( curr->id == id ) {
			return curr;
		}
		curr = curr->next;
	}
	
	return NULL;
}

void cmpt_get_data( struct cmpt_node *c, struct cmpt_node *out )
{
	memcpy( out, c, sizeof( *out ) );
}

void cmpt_set_data( struct cmpt_node *c, struct cmpt_node *in )
{
	memcpy( c, in, sizeof( *in ) );
}