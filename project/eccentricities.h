#ifndef ECCENTRICITIES_H
#define ECCENTRICITIES_H

#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MY_INF 9223372036854775807

typedef struct node {
    long int val;
    struct node* next;
} W_list;


long int tactique_1_hasard(W_list* W_head, unsigned long int size);

// Retourne une node de depart selon la tactique choisie
long int select_from(long int (*tactique)(W_list*, unsigned long int), W_list* W_head, unsigned long int size);

// Recupere l'excentricite du noeud v et par la meme occasion, la distance
// de v aux autres noeuds
// Contient 1 BFS
long int get_vertice_eccentricity(long int v, long int* distance);

// BOUNDING ECCENTRICITIES : Algorithme de Takes et Kosters
unsigned long int* get_eccentricities(igraph_t* graph, int delta, long int (*tactique)(W_list*, unsigned long int));
#endif /* end of include guard: ECCENTRICITIES_H */
