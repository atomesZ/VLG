#include "eccentricities.h"

void custom_eccentricities(igraph_t* graph,
                            unsigned long int num_vertices,
                            FILE* file_eccentricities)
{
    unsigned long int num_bfs = 0;
    int delta = 0;
    unsigned long int* res_eccentricities = get_eccentricities(graph, delta, &num_bfs, tactique_5_community);

    printf("OK\n");
    printf("Value of delta: %d\n", delta);
    printf("Number of BFS used: %ld\n", num_bfs);

    for (unsigned long int i = 0; i < num_vertices; ++i)
        fprintf(file_eccentricities, "%ld ", (long) res_eccentricities[i]);

    free(res_eccentricities);
}

long int tactique_5_community(igraph_t* graph, W_list* W_head, unsigned long int size)
{
    igraph_vector_t modularity, membership;
    igraph_matrix_t memberships;
    igraph_vector_init(&modularity, 0);
    igraph_vector_init(&membership, 0);
    igraph_matrix_init(&memberships, 0, 0);

    igraph_community_multilevel(graph, NULL, 1, &membership, &memberships, &modularity);

    printf("Modularities:\n");
    igraph_vector_print(&modularity);

    //Prend la meilleure Modularite
    long int j = igraph_vector_which_max(&modularity);
    unsigned long int membership_size = igraph_vector_size(&membership);

    //igraph_vector_destroy(&modularity);
    //igraph_vector_destroy(&membership);
    //igraph_matrix_destroy(&memberships);
    long int tactique = rand() % membership_size;

    return (long int)MATRIX(memberships, j, tactique);
}

long int tactique_1_hasard(igraph_t* graph, W_list* W_head, unsigned long int size)
{
    W_list* current_node = W_head; // Useless but cleaner
    int i = rand() % size;
    for (int j = 0; j < i; ++j)
    {
        current_node = current_node->next;
        if (current_node == NULL)
        {
            fprintf(stderr, "If this code is reached, then we miscalculated the size of the list somehow\n");
            exit( EXIT_FAILURE );
        }
    }

    return current_node->val;
}

long int select_from(long int (*tactique)(igraph_t*, W_list*, unsigned long int), igraph_t* graph, W_list* W_head, unsigned long int size)
{
    return tactique(graph, W_head, size);
}

long int get_vertice_eccentricity(igraph_t* graph, long int v, igraph_vector_t* distance)
{
// int igraph_bfs(const igraph_t *graph,
//                 igraph_integer_t root,
//                 const igraph_vector_t *roots,
//                 igraph_neimode_t mode,
//                 igraph_bool_t unreachable,
//                 const igraph_vector_t *restricted,
//                 igraph_vector_t *order,
//                 igraph_vector_t *rank,
//                 igraph_vector_t *father,
//                 igraph_vector_t *pred,
//                 igraph_vector_t *succ,
//                 igraph_vector_t *dist,
//                 igraph_bfshandler_t *callback,
//                 void *extra);

    int error_code = igraph_bfs(graph, v, NULL, IGRAPH_ALL, 0, NULL, NULL, NULL, NULL, NULL, NULL, distance, NULL, NULL);

    if (error_code)
    {
        fprintf(stderr, "Could not apply bfs\n");
        exit( EXIT_FAILURE );
    }

    return igraph_vector_max(distance);
}

igraph_t* get_largest_connected_component(igraph_t* graph)
{
    igraph_vector_ptr_t components;
    igraph_vector_ptr_init(&components, 0);
    igraph_decompose(graph, &components, IGRAPH_STRONG, -1, 2);

    // a voir si il existe pas un max pour igraph_vector_ptr_t
    igraph_vector_ptr_sort(&components, igraph_vector_colex_cmp);
    //le premier element est le plus grand
    igraph_t* largest_component = NULL;
    largest_component = igraph_vector_ptr_e(&components, 0);

    //faut faire attention a ne pas free le largest_component
    //igraph_decompose_destroy(&components);
    //free_complist(&components);
    //igraph_vector_ptr_destroy(&components);

    return largest_component;
}

unsigned long int* get_eccentricities(igraph_t* graph, int delta, unsigned long int* num_bfs, long int (*tactique)(igraph_t*, W_list*, unsigned long int))
{
    //trouver la plus grande Composante connexe
    //graph = get_largest_connected_component(graph);
    // On a commenté ce bout de code car les graphes fournis sont connexes

    unsigned long int num_vertices = igraph_vcount(graph); // actually is igraph_integer_t but we try to sneaky cast

    unsigned int len_W_list = num_vertices;
    //INIT
    W_list* W_head = linked_list_init(graph, num_vertices);
    unsigned long int* eccentricities = calloc(num_vertices, sizeof(unsigned long int));
    long int* eccentricities_L = calloc(num_vertices, sizeof(long int));
    long int* eccentricities_U = calloc(num_vertices, sizeof(long int));



    // Algorithme de Takes et Kosters
    // Init
    for (W_list* current_node = W_head; current_node; current_node = current_node->next)
    {
        long int w = current_node->val;
        //eccentricities[w] = 0; // Unrequired since we used calloc
        eccentricities_L[w] = -MY_INF;
        eccentricities_U[w] = MY_INF;
    }

    // Nous calculons les excentricites de tout le graphe
    while (W_head != NULL)
    {
        // Nous selectionnons le point initial par lequel on va commencer nos
        // calculs d'excentricite
        const long int v = select_from(tactique, graph, W_head, len_W_list);

        // on va calculer les distance de v par rapport aux w en meme temps que
        // le calcul d'excentricites (grâce aux BFS)
        igraph_vector_t distance; // = calloc(num_vertices, sizeof(long int));
        igraph_vector_init(&distance, num_vertices);

        //calcul d'excentricites
        eccentricities[v] = get_vertice_eccentricity(graph, v, &distance);
        *num_bfs += 1;

        W_list* prev_node = NULL;
        W_list* current_node = W_head;
        while (current_node != NULL)
        {
            long int w = current_node->val;

            //LES BORNES
            long int d = VECTOR(distance)[w];
            eccentricities_L[w] = fmax(eccentricities_L[w], fmax(eccentricities[v] - d, d));
            eccentricities_U[w] = fmin(eccentricities_U[w], eccentricities[v] + d);//inf

            // Si l'écart entre les deux bornes est inférieur à delta,
            // alors nous avons trouve la bonne excentricite
            if (eccentricities_U[w] - eccentricities_L[w] <= delta)
            {
                eccentricities[w] = eccentricities_L[w];

                // Pop the value
                linked_list_pop(prev_node, &current_node, &W_head);

                len_W_list -= 1;
            }
            else
            {
                prev_node = current_node;
                current_node = current_node->next;
            }
        }
        igraph_vector_destroy(&distance);
    }

    free(eccentricities_L);
    free(eccentricities_U);

    return eccentricities;
}