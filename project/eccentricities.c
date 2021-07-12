#include "eccentricities.h"

long int tactique_1_random_old(W_list* W_head, unsigned long int len_W_list)
{
    W_list* current_node = W_head; // Useless but cleaner
    int i = rand() % len_W_list;
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

long int tactique_1_random(commu_stats_t* community)
{
    const unsigned long int len_list_vertice = community->len_list_vertice;
    int i = rand() % len_list_vertice;

    return community->list_vertice[i].value;
}

int communities_size_sort_asc_comp(const void* C, const void* D)
{
    const commu_stats_t* A = C;
    const commu_stats_t* B = D;

    long double A_pourcentage_traite_excentricites = (long double)(A->num_known_eccentricities) / (long double)A->len_list_vertice;
    long double B_pourcentage_traite_excentricites = (long double)(B->num_known_eccentricities) / (long double)B->len_list_vertice;
    double a_size = A->len_list_vertice * (1 - A_pourcentage_traite_excentricites);
    double b_size = B->len_list_vertice * (1 - B_pourcentage_traite_excentricites);
    return (a_size > b_size) - (a_size < b_size);
}

int communities_size_sort_dsc_comp(const void* C, const void* D)
{
    const commu_stats_t* A = C;
    const commu_stats_t* B = D;

    long double A_pourcentage_traite_excentricites = (long double)(A->num_known_eccentricities) / (long double)A->len_list_vertice;
    long double B_pourcentage_traite_excentricites = (long double)(B->num_known_eccentricities) / (long double)B->len_list_vertice;
    double a_size = A->len_list_vertice * (1 - A_pourcentage_traite_excentricites);
    double b_size = B->len_list_vertice * (1 - B_pourcentage_traite_excentricites);
    return (a_size < b_size) - (a_size > b_size);
}

void tactique_communities_size(graph_stats_t* graph_stats)
{
    const unsigned long int len_list_commus = graph_stats->len_list_commus;
    commu_stats_t* list_commus = graph_stats->list_commus;

    if (graph_stats->tactique == COMMUNITY_SIZE_ASC_RANDOM || graph_stats->tactique == COMMUNITY_SIZE_ASC_HIGH_DEGREE)
        qsort(list_commus, len_list_commus, sizeof(commu_stats_t), communities_size_sort_asc_comp);
    else if (graph_stats->tactique == COMMUNITY_SIZE_DSC_RANDOM || graph_stats->tactique == COMMUNITY_SIZE_DSC_HIGH_DEGREE)
        qsort(list_commus, len_list_commus, sizeof(commu_stats_t), communities_size_sort_dsc_comp);

     graph_stats->list_commus = list_commus;
}

void sort_communities(graph_stats_t* graph_stats)
{
    switch (graph_stats->tactique)
    {
        case COMMUNITY_SIZE_ASC_RANDOM:
        case COMMUNITY_SIZE_DSC_RANDOM:
        case COMMUNITY_SIZE_ASC_HIGH_DEGREE:
        case COMMUNITY_SIZE_DSC_HIGH_DEGREE:
        {
            tactique_communities_size(graph_stats);
            break;
        }

        default:
            fprintf(stderr, "No community tactic applied\n");
    }
}

commu_stats_t* get_a_community(graph_stats_t* graph_stats)
{
    if (graph_stats->tactique < COMMUNITY)
        return graph_stats->list_commus;

    if (graph_stats->tactique > COMMUNITY_RANDOM)
    {
        int i = rand() % graph_stats->len_list_commus;
        return &(graph_stats->list_commus[i]); // graph_stats->list_commus + i
    }

    sort_communities(graph_stats);

    return graph_stats->list_commus;
}


long int get_vertice_eccentricity(igraph_t* graph, long int v, igraph_vector_t* distance)
{
    int error_code = igraph_bfs(graph, // const igraph_t *graph
                                v, // igraph_integer_t root
                                NULL, // const igraph_vector_t *roots
                                IGRAPH_ALL, // igraph_neimode_t mode
                                0, // igraph_bool_t unreachable
                                NULL, // const igraph_vector_t *restricted
                                NULL, // igraph_vector_t *order
                                NULL, // igraph_vector_t *rank
                                NULL, // igraph_vector_t *father
                                NULL, // igraph_vector_t *pred
                                NULL, // igraph_vector_t *succ
                                distance, // igraph_vector_t *dist
                                NULL, // igraph_bfshandler_t *callback
                                NULL); // void *extra

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

void init_size_communities(graph_stats_t* graph_stats, igraph_vector_t* membership)
{
    const unsigned long int len_membership = igraph_vector_size(membership);

    commu_stats_t* list_commus = calloc(len_membership, sizeof(commu_stats_t));
    graph_stats->len_list_commus = 0;

    for (unsigned long int i = 0; i < len_membership; i++)
        list_commus[i].id_community = -1;

    for (unsigned long int i = 0; i < len_membership; i++)
    {
        unsigned long int j = 0;
        while (list_commus[j].id_community != -1
                              && list_commus[j].id_community != VECTOR(*membership)[i])
            j++;

        if (list_commus[j].id_community == -1)
        {
            list_commus[j].id_community = VECTOR(*membership)[i];
            list_commus[j].len_list_vertice = 1;
            graph_stats->len_list_commus += 1;
            list_commus[j].fill_list_vertice = 0;
        }
        else
            list_commus[j].len_list_vertice += 1;
    }

    //ALLOCATION
    list_commus = realloc(list_commus, graph_stats->len_list_commus * sizeof(commu_stats_t));
    if (list_commus == NULL)
    {
        printf("%s\n", "Couldnt realloc for list_commus");
        exit(EXIT_FAILURE);
    }
    graph_stats->list_commus = list_commus;
}

void fill_communities(graph_stats_t* graph_stats, igraph_vector_t* membership, igraph_vector_t* degrees)
{
    commu_stats_t* list_commus = graph_stats->list_commus;
    const unsigned long int len_membership = igraph_vector_size(membership);
    for (unsigned long int i = 0; i < len_membership; i++)
    {
        unsigned long int j = 0;
        while (list_commus[j].id_community != VECTOR(*membership)[i])
            ++j;

        if (list_commus[j].id_community == -1)
        {
            //PAS CENSE ETRE LA
            printf("%s\n", "aaaaa");
        }

        if (list_commus[j].list_vertice == NULL)
        {
            list_commus[j].list_vertice = calloc(list_commus[j].len_list_vertice, sizeof(vertice_stats_t));
            list_commus[j].list_vertice[0].value = i;

            list_commus[j].list_vertice[0].degree = VECTOR(*degrees)[i];

            list_commus[j].list_vertice[0].borne_inf = -MY_INF;
            list_commus[j].list_vertice[0].borne_sup = MY_INF;

            list_commus[j].fill_list_vertice = 1;
        }
        else if (list_commus[j].fill_list_vertice < list_commus[j].len_list_vertice)
        {
            list_commus[j].list_vertice[list_commus[j].fill_list_vertice].value = i;
            list_commus[j].fill_list_vertice += 1;
        }
        else
        {
            //PAS CENSE ETRE LA
            printf("%s\n", "aaaaa");
        }
    }

    graph_stats->list_commus = list_commus;
}


void fill_without_communities(graph_stats_t* graph_stats, igraph_vector_t* degrees)
{
    commu_stats_t* list_commus = calloc(1, sizeof(commu_stats_t));
    graph_stats->len_list_commus = 1;
    graph_stats->modularity = 0;

    list_commus->list_vertice = calloc(graph_stats->num_vertices, sizeof(vertice_stats_t));
    list_commus->len_list_vertice = 0;

    for (long unsigned int i = 0; i < graph_stats->num_vertices; ++i)
    {
        list_commus->list_vertice[i].value = i;
        list_commus->list_vertice[i].degree = VECTOR(*degrees)[i];

        list_commus->list_vertice[i].borne_inf = -MY_INF;
        list_commus->list_vertice[i].borne_sup = MY_INF;
        list_commus->len_list_vertice += 1;
    }

    graph_stats->list_commus = list_commus;
}

void set_commus_stats(graph_stats_t* graph_stats)
{
    //STATS
    const unsigned long int len_list_commus = graph_stats->len_list_commus;
    commu_stats_t* list_commus = graph_stats->list_commus;
    // TODO for loop to retrieve those infos
    // for each commus
    for (unsigned long int i = 0; i < len_list_commus; ++i)
    {
        commu_stats_t* current_commu = &(list_commus[i]);
        unsigned long int len_list_vertice = current_commu->len_list_vertice;
        vertice_stats_t* list_vertice = current_commu->list_vertice;

        double mean_degree = 0;
        unsigned long int min_degree = list_vertice[0].degree;
        unsigned long int max_degree = list_vertice[0].degree;
        unsigned long int i_min_degree = 0;
        unsigned long int i_max_degree = 0;

        for (unsigned long int j = 0; j < len_list_vertice; ++j)
        {
            const unsigned int current_degree = list_vertice[j].degree;


            if (current_degree < min_degree) // min degree
            {
                min_degree = current_degree;
                i_min_degree = j;
            }
            else if (current_degree > max_degree) // max degree
            {
                max_degree = current_degree;
                i_max_degree = j;
            }

            // mean degree without overflow nor underflow
            mean_degree += (current_degree - mean_degree) / (j + 1);
        }
        //STATS
        current_commu->mean_degree = mean_degree;
        current_commu->min_degree = min_degree;
        current_commu->max_degree = max_degree;
        current_commu->i_min_degree = i_min_degree;
        current_commu->i_max_degree = i_max_degree;
        current_commu->mean_borne_inf = -MY_INF;
        current_commu->mean_borne_sup = MY_INF; //TODO, may cause problems

        current_commu->num_known_eccentricities = 0;
    }
    graph_stats->list_commus = list_commus;
}

void get_communities(igraph_t* graph, igraph_vector_t* membership, igraph_vector_t* modularity)
{
    igraph_vector_init(modularity, 0);
    igraph_vector_init(membership, 0);

    igraph_community_multilevel(graph, NULL, 1, membership, NULL, modularity);
}

commu_stats_t* constructor_list_commus(graph_stats_t* graph_stats)
{
    enum tactique tactique = graph_stats->tactique;
    igraph_t* graph = graph_stats->graph;

    igraph_vector_t degrees;
    igraph_vector_init(&degrees, graph_stats->num_vertices);
    igraph_vs_t vs = igraph_vss_all();

    igraph_degree(graph, &degrees, vs, IGRAPH_ALL, 0);

    // Si commu, on fait les commus
    if (tactique > COMMUNITY)
    {
        igraph_vector_t membership, modularity;
        get_communities(graph, &membership, &modularity);

        //MODULARITY
        graph_stats->modularity = VECTOR(modularity)[igraph_vector_size(&modularity) - 1];
        printf("MODULARITY : %ld\n", graph_stats->modularity);

        //COMMUNITIES INTO GRAPH STATS
        init_size_communities(graph_stats, &membership);
        fill_communities(graph_stats, &membership, &degrees);

        //FREE
        igraph_vector_destroy(&modularity);
        igraph_vector_destroy(&membership);

        //SORT
        sort_communities(graph_stats);
    }
    else
    {
        fill_without_communities(graph_stats, &degrees);
    }

    igraph_vector_destroy(&degrees);

    //STATS
    set_commus_stats(graph_stats);
    return graph_stats->list_commus;
}

graph_stats_t* get_graph_stats(igraph_t* graph, enum tactique tactique)
{
    graph_stats_t* graph_stats = calloc(1, sizeof(graph_stats_t));
    graph_stats->graph = graph;

    graph_stats->num_vertices = igraph_vcount(graph);
    graph_stats->tactique = tactique;

    const commu_stats_t* list_commus = constructor_list_commus(graph_stats);


    unsigned long int min_len_list_vertice = list_commus[0].len_list_vertice;
    unsigned long int max_len_list_vertice = list_commus[0].len_list_vertice;
    unsigned long int i_min_len_list_vertice = 0;
    unsigned long int i_max_len_list_vertice = 0;

    unsigned long int len_list_commus = graph_stats->len_list_commus;
    for (unsigned long int i = 1; i < len_list_commus; ++i)
    {
        const commu_stats_t current_commu = list_commus[i];

        if (current_commu.len_list_vertice < min_len_list_vertice)
        {
            min_len_list_vertice = current_commu.len_list_vertice;
            i_min_len_list_vertice = i;
        }
        else if (current_commu.len_list_vertice > max_len_list_vertice)
        {
            max_len_list_vertice = current_commu.len_list_vertice;
            i_max_len_list_vertice = i;
        }
    }

    graph_stats->min_len_list_vertice = min_len_list_vertice;
    graph_stats->max_len_list_vertice = max_len_list_vertice;
    graph_stats->i_min_len_list_vertice = i_min_len_list_vertice;
    graph_stats->i_max_len_list_vertice = i_max_len_list_vertice;

    return graph_stats;
}

void free_graph_stats(graph_stats_t* graph_stats)
{
    commu_stats_t* list_commus = graph_stats->list_commus;
    const unsigned long int len_list_commus = graph_stats->len_list_commus;

    for (unsigned long int index_commu = 0; index_commu < len_list_commus; index_commu++)
        free(list_commus[index_commu].list_vertice);

    free(list_commus);
    free(graph_stats);
}

long int select_from(graph_stats_t* graph_stats, W_list* W_head, unsigned long int len_W_list)
{
    int selected_vertice = 0;
    commu_stats_t* community = get_a_community(graph_stats);

    switch (graph_stats->tactique) {
        case RANDOM:
            selected_vertice = tactique_1_random_old(W_head, len_W_list);
            break;
        case COMMUNITY_SIZE_ASC_RANDOM:
        case COMMUNITY_SIZE_DSC_RANDOM:
        case COMMUNITY_RANDOM_RANDOM:
        {
            selected_vertice = tactique_1_random(community);
            break;
        }
        default:
            fprintf(stderr, "We tried to apply an unknown tactic\n");
            exit( EXIT_FAILURE );

    }

    return selected_vertice;
}

unsigned long int* get_eccentricities(igraph_t* graph, int delta, unsigned long int* num_bfs, enum tactique tactique)
{
    //trouver la plus grande Composante connexe
    //graph = get_largest_connected_component(graph);
    // On a commenté ce bout de code car les graphes fournis sont connexes
    graph_stats_t* graph_stats = get_graph_stats(graph, tactique);

    unsigned long int len_W_list = graph_stats->num_vertices;
    //INIT
    W_list* W_head = linked_list_init(len_W_list);
    unsigned long int* eccentricities = calloc(len_W_list, sizeof(unsigned long int));
    long int* eccentricities_L = calloc(len_W_list, sizeof(long int));
    long int* eccentricities_U = calloc(len_W_list, sizeof(long int));

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
        const long int v = select_from(graph_stats, W_head, len_W_list);

        // on va calculer les distance de v par rapport aux w en meme temps que
        // le calcul d'excentricites (grâce aux BFS)
        igraph_vector_t distance; // = calloc(num_vertices, sizeof(long int));
        igraph_vector_init(&distance, graph_stats->num_vertices);

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

    free_graph_stats(graph_stats);
    free(eccentricities_L);
    free(eccentricities_U);

    return eccentricities;
}

void custom_eccentricities(igraph_t* graph,
                            unsigned long int num_vertices,
                            FILE* file_eccentricities)
{
    unsigned long int num_bfs = 0;
    int delta = 0;


    unsigned long int* res_eccentricities = get_eccentricities(graph, delta, &num_bfs, COMMUNITY_SIZE_ASC_RANDOM);
    printf("Eccentricities: OK\n");
    printf("Value of delta: %d\n", delta);
    printf("Number of BFS used: %ld\n", num_bfs);

    for (unsigned long int i = 0; i < num_vertices; ++i)
        fprintf(file_eccentricities, "%ld ", (long) res_eccentricities[i]);

    free(res_eccentricities);
}
