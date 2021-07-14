#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eccentricities.h"
#include "igraph_eccentricities.h"

int main(int argc, char** argv)
{

    char* default_tactic = (argc == 3) ? argv[2] : "RANDOM";
    if (argc < 2 || argc >= 4)
    {
        fprintf(stderr, "We need the name of the file containing the graph\n");
        exit( EXIT_FAILURE );
    }
    if (argc < 3)
    {
        fprintf(stderr, "We need the name of a tactic, if not given, the random tactic is chosen\n");
    }

    FILE* fptr = fopen(argv[1], "r");

    if (!fptr)
    {
        fprintf(stderr, "Could not open file\n");
        exit( EXIT_FAILURE );
    }

    //On initialise le random
    srand(time(NULL));   // Initialization, should only be called once.
    igraph_rng_seed(igraph_rng_default(), 42);

    igraph_t graph;

    int graph_error_code = igraph_read_graph_edgelist(&graph, fptr, 0, 0);
    fclose(fptr);


    if (graph_error_code == IGRAPH_PARSEERROR)
    {
        fprintf(stderr, "Could not parse file correctly\n");
        exit( EXIT_FAILURE );
    }

    printf("Graph loading of '%s': OK\n", argv[1]);


    // Get diameter and average degree
    //igraph_real_t diameter;
    //igraph_diameter(&graph, &diameter, 0, 0, 0, IGRAPH_UNDIRECTED, 1);

    unsigned long int num_edges = igraph_ecount(&graph);
    unsigned long int num_vertices = igraph_vcount(&graph);

    printf("Number of edges: %ld\n", num_edges);
    printf("Number of vertices: %ld\n", num_vertices);


    //printf("Diameter: %ld\n", (long int) diameter);
    printf("Average degree: %g\n", 2.0 * num_edges / num_vertices);

    char* res_filename =  "eccentricities_teexgraph.txt";
    FILE* file_eccentricities = fopen(res_filename, "w");

    if (strcmp(default_tactic, "IGRAPH"))
        custom_eccentricities(&graph, num_vertices, file_eccentricities, default_tactic);
    else // igraph_eccentricities
        igraph_eccentricities(&graph, num_vertices, file_eccentricities);

    printf("We outputed the eccentricities in the file '%s'\n", res_filename);
    fclose(file_eccentricities);

    igraph_destroy(&graph);
    printf("Destroy: OK\n");

    return 0;
}
