#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eccentricities.h"
#include "igraph_eccentricities.h"

int main(int argc, char** argv)
{
    // We create a simple help menu to be user friendly
    if ((argc < 2 || argc >= 5) || (argc == 2 && strcmp(argv[1], "--help") == 0))
    {
        char* optional = "If tactic and delta are not precised then the default values will be 'RANDOM' and '0'";
        char* tactics_message = "The different tactics are:";
        printf("HELP MENU:\n./get_eccentricities {graph_path} {tactic} {delta}\n%s\n%s\n", optional, tactics_message);

        char* tactics_list[] = {
        "RANDOM",
        "HIGH_DEGREE",
        "LOW_DEGREE",
        "BIG_DELTA",
        "COMMUNITY_SIZE_ASC_RANDOM",
        "COMMUNITY_SIZE_ASC_HIGH_DEGREE",
        "COMMUNITY_SIZE_ASC_LOW_DEGREE",
        "COMMUNITY_SIZE_ASC_BIG_DELTA",
        "COMMUNITY_SIZE_DSC_RANDOM",
        "COMMUNITY_SIZE_DSC_HIGH_DEGREE",
        "COMMUNITY_SIZE_DSC_LOW_DEGREE",
        "COMMUNITY_SIZE_DSC_BIG_DELTA",
        "COMMUNITY_RANDOM_RANDOM",
        "COMMUNITY_RANDOM_HIGH_DEGREE",
        "COMMUNITY_RANDOM_LOW_DEGREE",
        "COMMUNITY_RANDOM_BIG_DELTA",
        NULL};

        for (int i = 0; tactics_list[i] != NULL; ++i)
            printf("  %s\n", tactics_list[i]);

        return 0;
    }

    char* default_tactic = (argc >= 3) ? argv[2] : "RANDOM";
    int delta = (argc == 4) ? atoi(argv[3]) : 0;

    if (argc < 3)
        fprintf(stderr, "Since we did not mention a tactic, the random tactic is chosen\n");

    if (argc < 4)
        fprintf(stderr, "Since we did not mention a delta, the default chosen delta is 0 (exact eccentricities)\n");

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
        custom_eccentricities(&graph, num_vertices, file_eccentricities, default_tactic, delta);
    else // igraph_eccentricities
        igraph_eccentricities(&graph, num_vertices, file_eccentricities);

    printf("We outputed the eccentricities in the file '%s'\n", res_filename);
    fclose(file_eccentricities);

    igraph_destroy(&graph);
    printf("Destroy: OK\n");

    return 0;
}
