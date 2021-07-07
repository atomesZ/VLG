#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>

#include "eccentricities.h"

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


    unsigned long int num_bfs = 0;
    unsigned long int num_vertices = igraph_vcount(&graph);

    printf("Number of vertices : %ld\n", num_vertices);

    unsigned long int* res_eccentricities = get_eccentricities(&graph, 0, &num_bfs, tactique_1_hasard);


    if (graph_error_code)
    {
        fprintf(stderr, "Could not get eccentricity\n");
        exit( EXIT_FAILURE );
    }

    printf("Eccentricities : OK\n");
    printf("Number of BFS used : %ld\n", num_bfs);


    char* res_filename =  "eccentricities_teexgraph.txt";
    FILE* file_eccentricities = fopen(res_filename, "w");

    for (unsigned long int i = 0; i < num_vertices; ++i)
        fprintf(file_eccentricities, "%ld ", (long) res_eccentricities[i]);

    printf("We outputed the ecentricities in the file '%s'\n", res_filename);



    free(res_eccentricities);

    igraph_destroy(&graph);

    fclose(fptr);
    fclose(file_eccentricities);

    printf("Destroy : OK\n");


    return 0;
}
