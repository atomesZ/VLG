#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <igraph.h>

typedef struct node {
    long int val;
    struct node* next;
} W_list;

//FIXME
W_list* linked_list_init(igraph_t* graph, unsigned long int num_vertices);

void linked_list_pop(W_list* prev_node, W_list** current_node, W_list** W_head);


#endif /* end of include guard: LINKED_LIST_H */
