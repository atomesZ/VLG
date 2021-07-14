#ifndef GRAPH_STATS_H
#define GRAPH_STATS_H

#include <stdlib.h>
#include <igraph.h>


enum tactique {
    //Without communities
    RANDOM = 0,
    HIGH_DEGREE,
    LOW_DEGREE,
    BIG_DELTA,

    COMMUNITY, // This is a marker

    //Community sorted by size and filled
    COMMUNITY_SIZE, // This is a marker
    COMMUNITY_SIZE_ASC_RANDOM,
    COMMUNITY_SIZE_ASC_HIGH_DEGREE,
    COMMUNITY_SIZE_ASC_LOW_DEGREE,
    COMMUNITY_SIZE_ASC_BIG_DELTA,

    COMMUNITY_SIZE_DSC, // This is a marker
    COMMUNITY_SIZE_DSC_RANDOM,
    COMMUNITY_SIZE_DSC_HIGH_DEGREE,
    COMMUNITY_SIZE_DSC_LOW_DEGREE,
    COMMUNITY_SIZE_DSC_BIG_DELTA,

    //RANDOM community
    COMMUNITY_RANDOM, // This is a marker
    COMMUNITY_RANDOM_RANDOM,
    COMMUNITY_RANDOM_HIGH_DEGREE,
    COMMUNITY_RANDOM_LOW_DEGREE,
    COMMUNITY_RANDOM_BIG_DELTA
};

typedef struct vertice_stats {
    long int value;
    unsigned int degree;
    long int borne_inf;
    long int borne_sup;
} vertice_stats_t;

typedef struct commu_stats {
    vertice_stats_t* list_vertice;
    long int id_community;
    unsigned long int len_list_vertice;
    //for construction only
    unsigned long int fill_list_vertice;

    vertice_stats_t** vertices_not_treated;
    unsigned long int len_vertices_not_treated;

    //STATS
    double mean_degree;
    unsigned long int min_degree;
    unsigned long int max_degree;
    unsigned long int i_min_degree;
    unsigned long int i_max_degree;
    long int mean_borne_inf; // Not implemented yet
    long int mean_borne_sup; // Not implemented yet

    unsigned long int num_known_eccentricities;

} commu_stats_t;

typedef struct graph_stats {
    igraph_t* graph; // largest connected component

    commu_stats_t* list_commus; // if not using commus, then this list will have 1 value : the graph
    unsigned long int len_list_commus;
    unsigned long int num_vertices;

    commu_stats_t** commus_not_treated;
    unsigned long int len_commus_not_treated;

    enum tactique tactique;

    //STATS
    double modularity;
    unsigned long int min_len_list_vertice;
    unsigned long int max_len_list_vertice;
    unsigned long int i_min_len_list_vertice;
    unsigned long int i_max_len_list_vertice;
} graph_stats_t;

#endif /* end of include guard: GRAPH_STATS_H */
