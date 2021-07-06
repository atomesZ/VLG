#include "eccentricities.h"



unsigned int* get_eccentricities(igraph_t* graph, int delta)
{
    delta += 1;
    
    igraph_integer_t size = igraph_vcount(graph);


    igraph_vs_t vs = igraph_vss_all();
    igraph_vit_t vit;

    int error_code = igraph_vit_create(graph, vs, &vit);

    if (error_code)
    {
        fprintf(stderr, "Could not create vertex iterator\n");
        exit( EXIT_FAILURE );
    }


    unsigned int* eccentricities = calloc(size, sizeof(unsigned int));



    igraph_vit_destroy(&vit);

    return eccentricities;
}
