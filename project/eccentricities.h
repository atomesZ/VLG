#ifndef ECCENTRICITIES_H
#define ECCENTRICITIES_H

#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "linked_list.h"

#define MY_INF 9223372036854775807

void custom_eccentricities(igraph_t* graph,
                            unsigned long int num_vertices,
                            FILE* file_eccentricities);

long int tactique_1_hasard(W_list* W_head, unsigned long int size);

// Retourne une node de depart selon la tactique choisie
long int select_from(long int (*tactique)(W_list*, unsigned long int), W_list* W_head, unsigned long int size);

// Recupere l'excentricite du noeud v et par la meme occasion, la distance
// de v aux autres noeuds
// Contient 1 BFS
long int get_vertice_eccentricity(igraph_t* graph, long int v, igraph_vector_t* distance);

//trouver la plus grande Composante connexe
igraph_t* get_largest_connected_component(igraph_t* graph);

// BOUNDING ECCENTRICITIES : Algorithme de Takes et Kosters
unsigned long int* get_eccentricities(igraph_t* graph, int delta, unsigned long int* num_bfs, long int (*tactique)(W_list*, unsigned long int));
#endif /* end of include guard: ECCENTRICITIES_H */
