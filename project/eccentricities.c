#include "eccentricities.h"

long int tactique_1_hasard(W_list* W_head, unsigned long int size)
{
    srand(time(NULL));   // Initialization, should only be called once.
    int i = rand() % size;
    return W_head->val;
}

long int select_from(long int (*tactique)(W_list*, unsigned long int), W_list* W_head, unsigned long int size)
{
    return tactique(W_head, size);
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

unsigned long int* get_eccentricities(igraph_t* graph, int delta, unsigned long int* num_bfs, long int (*tactique)(W_list*, unsigned long int))
{
    //trouver la plus grande Composante connexe
    //graph = get_largest_connected_component(graph);
    // On a commenté ce bout de code car les graphes fournis sont connexes

    unsigned long int num_vertices = igraph_vcount(graph); // actually is igraph_integer_t but we try to sneaky cast

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
        long int v = select_from(tactique, W_head, num_vertices);

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
