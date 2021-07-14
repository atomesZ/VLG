#ifndef ECCENTRICITIES_H
#define ECCENTRICITIES_H

#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph_stats.h"

// We could use LONG_MAX from limits.h instead
#define MY_INF 9223372036854775807

//TACTIQUES
long int tactique_1_random(commu_stats_t* community);

long int tactique_2_high_degree(commu_stats_t* community);

long int tactique_3_low_degree(commu_stats_t* community);

long int tactique_4_big_delta(commu_stats_t* community);

//Tactiques communautes
int communities_size_sort_asc_comp(const void* A, const void* B);
int communities_size_sort_dsc_comp(const void* A, const void* B);
void tactique_communities_size(graph_stats_t* graph_stats);

void tactique_communities_random(graph_stats_t* graph_stats);

void sort_communities(graph_stats_t* graph_stats);

commu_stats_t* get_a_community(graph_stats_t* graph_stats);

long int get_vertice_eccentricity(igraph_t* graph, long int v, igraph_vector_t* distance);

igraph_t* get_largest_connected_component(igraph_t* graph, igraph_vector_ptr_t* components);

void init_communities(graph_stats_t* graph_stats, igraph_vector_t* membership);

void fill_communities(graph_stats_t* graph_stats, igraph_vector_t* membership, igraph_vector_t* degrees);

void fill_without_communities(graph_stats_t* graph_stats, igraph_vector_t* degrees);

void fill_commus_not_treated(graph_stats_t* graph_stats);

void set_commus_stats(graph_stats_t* graph_stats);

void get_communities(igraph_t* graph, igraph_vector_t* membership, igraph_vector_t* modularity);

commu_stats_t* constructor_list_commus(graph_stats_t* graph_stats);

graph_stats_t* get_graph_stats(igraph_t* graph, enum tactique tactique);

void free_graph_stats(graph_stats_t* graph_stats);

long int select_from(graph_stats_t* graph_stats);

unsigned long int* get_eccentricities(igraph_t* graph, int delta, unsigned long int* num_bfs, enum tactique tactique);

void custom_eccentricities(igraph_t* graph,
                            unsigned long int num_vertices,
                            FILE* file_eccentricities,
                            char* tactique_str,
                            int delta);


#endif /* end of include guard: ECCENTRICITIES_H */
