#ifndef UI_DATA_H
#define UI_DATA_H
#define SCR_STACK_CAPACITY 8 
#include <SDL2/SDL.h>
#include "ui_cmpts.h"

struct cmpt_list {
	struct cmpt_node *first;
	struct cmpt_node *last;
};

struct cmpt_list *new_list( void );

void del_list( struct cmpt_list *list );

struct cmpt_node *alloc_cmpt( void );

void free_cmpt( struct cmpt_node *c );

struct cmpt_node *new_cmpt( UI_cmpt_type type, int layer );

void del_cmpt( struct cmpt_node *c );

void insert_cmpt( struct cmpt_list *list, struct cmpt_node *afterThis, struct cmpt_node *c );

struct cmpt_node *remove_cmpt( struct cmpt_list *list, struct cmpt_node *c );

void del_cmpts( struct cmpt_node *afterThis );

void insert_layer_sorted( struct cmpt_list *list, struct cmpt_node *c );

struct cmpt_node *find_cmpt( struct cmpt_node *start, UI_cmpt_id id );

/* ARE THESE TWO EVEN NEEDED?? */
void cmpt_get_data( struct cmpt_node *c, struct cmpt_node *out );
void cmpt_set_data( struct cmpt_node *c, struct cmpt_node *in );

#endif