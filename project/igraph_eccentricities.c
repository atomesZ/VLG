#include "igraph_eccentricities.h"

void igraph_eccentricities(igraph_t* graph,
                            unsigned long int num_vertices,
                            FILE* file_eccentricities)
{
    igraph_vector_t res_eccentricities;

    int graph_error_code = igraph_vector_init(&res_eccentricities, num_vertices);
    if (graph_error_code)
    {
        fprintf(stderr, "Could not init vector for eccentricities\n");
        exit( EXIT_FAILURE );
    }


    igraph_vs_t all_vs = igraph_vss_all();


    graph_error_code = igraph_eccentricity(graph, &res_eccentricities, all_vs, IGRAPH_ALL);

    if (graph_error_code)
    {
        fprintf(stderr, "Could not get eccentricities\n");
        exit( EXIT_FAILURE );
    }

    printf("Eccentricities: OK\n");

    igraph_real_t el;

    // Iterate over vector of eccentricities and write to file
    long int len_vec_ecc = igraph_vector_size(&res_eccentricities);

    for (long int i = 0; i < len_vec_ecc; ++i)
    {
        el = igraph_vector_e(&res_eccentricities, i);

        fprintf(file_eccentricities, "%ld ", (long) el);
    }

    igraph_vector_destroy(&res_eccentricities);
    igraph_vs_destroy(&all_vs);
}
