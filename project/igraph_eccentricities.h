#ifndef IGRAPH_ECCENTRICITIES_H
#define IGRAPH_ECCENTRICITIES_H

#include <igraph.h>

void igraph_eccentricities(igraph_t* graph,
                            unsigned long int num_vertices,
                            FILE* file_eccentricities);

#endif /* end of include guard: IGRAPH_ECCENTRICITIES_H */
