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

long int get_vertice_eccentricity(long int v, long int* distance)
{
    return 42;
}

unsigned long int* get_eccentricities(igraph_t* graph, int delta, long int (*tactique)(W_list*, unsigned long int))
{
    unsigned long int num_vertices = igraph_vcount(graph); // actually is igraph_integer_t but we try to sneaky cast

    //INIT
    W_list* head = NULL;
    for (long int i = num_vertices - 1; i >= 0; --i)
    {
        W_list* current_node = malloc(sizeof(W_list));
        current_node->val = i;
        current_node->next = head;

        head = current_node;
    }

    W_list* W_head = head;

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
        // TACTIQUE : nous choisissons par quelle methode nous allons commencer
        // notre calcul d'excentricites
        long int v = select_from(tactique, W_head, num_vertices);

        // on va calculer les distance de v par rapport aux w en meme temps que
        // le calcul d'excentricites (grâce aux BFS)
        long int* distance = calloc(num_vertices, sizeof(long int));

        //calcul d'excentricites
        eccentricities[v] = get_vertice_eccentricity(v, distance);

        W_list* prev_node = NULL;
        W_list* current_node = W_head;
        while (current_node != NULL)
        {
            long int w = current_node->val;

            //LES BORNES
            long int d = distance[w];
            eccentricities_L[w] = fmax(eccentricities_L[w], fmax(eccentricities[v] - d, d));
            eccentricities_U[w] = fmin(eccentricities_U[w], eccentricities[v] + d);//inf

            // Si les deux bornes sont egales, alors nous avons trouve la
            // bonne excentricite
            if (eccentricities_L[w] == eccentricities_U[w])
            {
                eccentricities[w] = eccentricities_L[w];

                // Pop the value
                if (prev_node == NULL)
                    W_head = current_node->next;
                else
                    prev_node->next = current_node->next;

                W_list* tmp = current_node->next;
                free(current_node);
                current_node = tmp;
            }
            else
            {
                prev_node = current_node;
                current_node = current_node->next;
            }
        }
        free(distance);
    }

    free(eccentricities_L);
    free(eccentricities_U);

    return eccentricities;
}
