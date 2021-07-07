#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>

#include "eccentricities.h"
#include "igraph_eccentricities.h"

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
    fclose(fptr);


    if (graph_error_code == IGRAPH_PARSEERROR)
    {
        fprintf(stderr, "Could not parse file correctly\n");
        exit( EXIT_FAILURE );
    }

    printf("Graph loading of '%s': OK\n", argv[1]);


    // Get diameter and average degree
    igraph_real_t diameter;
    unsigned long int num_edges = igraph_ecount(&graph);
    unsigned long int num_vertices = igraph_vcount(&graph);

    printf("Number of edges: %ld\n", num_edges);
    printf("Number of vertices: %ld\n", num_vertices);



    igraph_diameter(&graph, &diameter, 0, 0, 0, IGRAPH_UNDIRECTED, 1);


    printf("Diameter: %ld\n", (long int) diameter);
    printf("Average degree: %g\n", 2.0 * num_edges / num_vertices);

    printf("Eccentricities: ");

    char* res_filename =  "eccentricities_teexgraph.txt";
    FILE* file_eccentricities = fopen(res_filename, "w");


    /////////////////////////////////////////////////////
    int bool_custom_eccentricities = 1;

    if (bool_custom_eccentricities)
        custom_eccentricities(&graph, num_vertices, file_eccentricities);
    else // igraph_eccentricities
        igraph_eccentricities(&graph, num_vertices, file_eccentricities);



////////////////////////////////////////:
    printf("We outputed the eccentricities in the file '%s'\n", res_filename);
    fclose(file_eccentricities);

    igraph_destroy(&graph);
    printf("Destroy: OK\n");

    return 0;
}
