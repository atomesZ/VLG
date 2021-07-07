#include <stdlib.h>
#include "linked_list.h"

W_list* linked_list_init(igraph_t* graph, unsigned long int num_vertices)
{
    W_list* head = NULL;
    for (long int i = num_vertices - 1; i >= 0; --i)
    {
        W_list* current_node = malloc(sizeof(W_list));
        current_node->val = i;
        current_node->next = head;

        head = current_node;
    }

    return head;
}

void linked_list_pop(W_list* prev_node, W_list** current_node, W_list** W_head)
{
    if (prev_node == NULL)
        *W_head = (*current_node)->next;
    else
        prev_node->next = (*current_node)->next;

    W_list* tmp = (*current_node)->next;
    free(*current_node);
    *current_node = tmp;
}
