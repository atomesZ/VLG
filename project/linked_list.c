#include <stdlib.h>
#include "linked_list.h"

W_list* linked_list_init(graph_stats_t* graph_stats)
{
    W_list* head = NULL;
    // for (long int i = num_vertices - 1; i >= 0; --i)
    // {
    //     W_list* current_node = malloc(sizeof(W_list));
    //     current_node->val = i;
    //     current_node->next = head;
    //
    //     head = current_node;
    // }

    commu_stats_t* list_commus = graph_stats->list_commus;
    unsigned long int len_list_commus = graph_stats->len_list_commus;

    for (unsigned long int i_commu = 0; i_commu < len_list_commus; ++i_commu)
    {
        unsigned long int len_list_vertice = list_commus[i_commu].len_list_vertice;
        vertice_stats_t* list_vertice = list_commus[i_commu].list_vertice;

        for (unsigned long int i_vertice = 0; i_vertice < len_list_vertice; ++i_vertice)
        {
            W_list* current_node = malloc(sizeof(W_list));
            current_node->val = list_vertice[i_vertice].value;
            current_node->next = head;
            current_node->my_commu = &(list_commus[i_commu]);
            current_node->i_vertices_not_treated = i_vertice;

            head = current_node;
        }
    }

    return head;
}

void linked_list_pop(W_list* prev_node, W_list** current_node, W_list** W_head)
{
    if (prev_node == NULL)
        *W_head = (*current_node)->next;
    else
        prev_node->next = (*current_node)->next;

    // We pop in vertices_not_treated
    commu_stats_t* my_commu = (*current_node)->my_commu;
    my_commu->num_known_eccentricities += 1;
    my_commu->vertices_not_treated[(*current_node)->i_vertices_not_treated] = my_commu->vertices_not_treated[--my_commu->len_vertices_not_treated];


    W_list* tmp = (*current_node)->next;
    free(*current_node);
    *current_node = tmp;
}
