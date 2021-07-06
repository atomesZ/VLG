#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{

    if (argc != 2)
    {
        fprintf(stderr, "We need the name of the file containing the graph\n");
        exit( EXIT_FAILURE );
    }

    FILE* fptr = fopen(argv[1], "r");

    if (!fptr)
    {
        fprintf(stderr, "Could not open file\n");
        exit( EXIT_FAILURE );
    }


    igraph_t graph;

    int graph_error_code = igraph_read_graph_edgelist(&graph, fptr, 0, 0);

    if (graph_error_code == IGRAPH_PARSEERROR)
    {
        fprintf(stderr, "Could not parse file correctly\n");
        exit( EXIT_FAILURE );
    }

    printf("Graph loading : OK\n");


    // Get diameter and average degree
    igraph_real_t diameter;

    igraph_diameter(&graph, &diameter, 0, 0, 0, IGRAPH_UNDIRECTED, 1);
    printf("Diameter of the graph %s with average degree %g: %g\n",
            argv[1],
            2.0 * igraph_ecount(&graph) / igraph_vcount(&graph),
            (double) diameter);

    printf("Diameter : OK\n");

    // Get eccentricities
    igraph_vector_t res_eccentricities;

    long int size_res_eccentricities = igraph_vcount(&graph);

    graph_error_code = igraph_vector_init(&res_eccentricities, size_res_eccentricities);
    if (graph_error_code)
    {
        fprintf(stderr, "Could not init vector for eccentricities\n");
        exit( EXIT_FAILURE );
    }


    igraph_vs_t all_vs = igraph_vss_all();
    //graph_error_code = igraph_vs_all(&all_vs);

    if (graph_error_code)
    {
        fprintf(stderr, "Could not get all vertices\n");
        exit( EXIT_FAILURE );
    }

    printf("Vertices : OK\n");

    //TODO
    graph_error_code = igraph_eccentricity(&graph, &res_eccentricities, all_vs, IGRAPH_ALL);

    if (graph_error_code)
    {
        fprintf(stderr, "Could not get eccentricity\n");
        exit( EXIT_FAILURE );
    }

    printf("Eccentricities : OK\n");

    FILE* file_eccentricities = fopen("eccentricities_teexgraph.txt", "w");


    igraph_real_t el;

    // Iterate over vector of eccentricities and write to file
    long len_vec_ecc = igraph_vector_size(&res_eccentricities);
    for (long i = 0; i < len_vec_ecc; ++i)
    {
        el = igraph_vector_e(&res_eccentricities, i);

        fprintf(file_eccentricities, "%ld ", (long) el);
    }


    igraph_vector_destroy(&res_eccentricities);
    igraph_vs_destroy(&all_vs);


    igraph_destroy(&graph);

    fclose(fptr);

    printf("Destroy : OK\n");

    return 0;
}
