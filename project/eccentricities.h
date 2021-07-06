#ifndef ECCENTRICITIES_H
#define ECCENTRICITIES_H

#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int* get_eccentricities(igraph_t* graph, int delta);
#endif /* end of include guard: ECCENTRICITIES_H */
