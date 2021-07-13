#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <igraph.h>


enum tactique {
    RANDOM = 0,
    HIGH_DEGREE,

    COMMUNITY, // This is a marker
    COMMUNITY_SIZE, // This is a marker
    COMMUNITY_SIZE_ASC_RANDOM,
    COMMUNITY_SIZE_DSC_RANDOM,
    COMMUNITY_SIZE_ASC_HIGH_DEGREE,
    COMMUNITY_SIZE_DSC_HIGH_DEGREE,

    COMMUNITY_RANDOM, // This is a marker
    COMMUNITY_RANDOM_RANDOM,
    COMMUNITY_RANDOM_HIGH_DEGREE,
    COMMUNITY_RANDOM_
};

typedef struct vertice_stats {
    long int value;
    unsigned int degree;
    long int borne_inf; // TODO
    long int borne_sup; // TODO

} vertice_stats_t;

typedef struct commu_stats {
    vertice_stats_t* list_vertice;
    long int id_community;
    unsigned long int len_list_vertice;
    //for construction only
    unsigned long int fill_list_vertice;

    // FIXME Maybe use this for commu as well (commu_not_treated) to be faster
    long int* vertices_not_treated; // FIXME Maybe change this for an array of pointer to vertice_stats
    unsigned long int len_vertices_not_treated;

    //STATS
    double mean_degree;
    unsigned long int min_degree;
    unsigned long int max_degree;
    unsigned long int i_min_degree;
    unsigned long int i_max_degree;
    long int mean_borne_inf; // TODO
    long int mean_borne_sup; // TODO

    unsigned long int num_known_eccentricities; //FIXME je suis en trop avec len_vertices_not_treated

} commu_stats_t;

typedef struct graph_stats {
    igraph_t* graph;

    commu_stats_t* list_commus; // if not using commus, then this list will have 1 value : the graph
    unsigned long int len_list_commus;
    unsigned long int num_vertices;

    enum tactique tactique;

    //STATS
    double modularity; // FIXME (always 0 somehow)
    unsigned long int min_len_list_vertice;
    unsigned long int max_len_list_vertice;
    unsigned long int i_min_len_list_vertice;
    unsigned long int i_max_len_list_vertice;
} graph_stats_t;

typedef struct node {
    long int val;
    struct node* next;
    commu_stats_t* my_commu;
    unsigned long int i_vertices_not_treated;
} W_list;


//FIXME
W_list* linked_list_init(graph_stats_t* graph_stats);

void linked_list_pop(W_list* prev_node, W_list** current_node, W_list** W_head);

#endif /* end of include guard: LINKED_LIST_H */
