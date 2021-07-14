#include <string.h>
#include "eccentricities.h"

long int tactique_1_random(commu_stats_t* community)
{
    const unsigned long int len_vertices_not_treated = community->len_vertices_not_treated;

    int i = rand() % len_vertices_not_treated;
    long int res = community->vertices_not_treated[i]->value;

    return res;
}

long int tactique_2_high_degree(commu_stats_t* community)
{
    const unsigned long int len_vertices_not_treated = community->len_vertices_not_treated;

    int index_max_degree = 0;
    for (unsigned long int i = 1; i < len_vertices_not_treated; i++)
    {
        if (community->vertices_not_treated[index_max_degree]->degree < community->vertices_not_treated[i]->degree)
            index_max_degree = i;
    }

    return community->vertices_not_treated[index_max_degree]->value;
}

long int tactique_3_low_degree(commu_stats_t* community)
{
    const unsigned long int len_vertices_not_treated = community->len_vertices_not_treated;

    int index_min_degree = 0;
    for (unsigned long int i = 1; i < len_vertices_not_treated; i++)
    {
        if (community->vertices_not_treated[index_min_degree]->degree > community->vertices_not_treated[i]->degree)
            index_min_degree = i;
    }

    return community->vertices_not_treated[index_min_degree]->value;
}

long int tactique_4_big_delta(commu_stats_t* community)
{
    const unsigned long int len_vertices_not_treated = community->len_vertices_not_treated;

    int big_delta_index = 0;
    for (unsigned long int i = 1; i < len_vertices_not_treated; ++i)
    {
        long int big_delta_value_i = community->vertices_not_treated[i]->borne_sup - community->vertices_not_treated[i]->borne_inf;
        long int big_delta_value_index = community->vertices_not_treated[big_delta_index]->borne_sup - community->vertices_not_treated[big_delta_index]->borne_inf;

        if (big_delta_value_index < big_delta_value_i)
            big_delta_index = i;
    }

    return community->vertices_not_treated[big_delta_index]->value;
}

int communities_size_sort_asc_comp(const void* C, const void* D)
{
    commu_stats_t** A = (commu_stats_t**)C;
    commu_stats_t** B = (commu_stats_t**)D;

    long double A_pourcentage_traite_excentricites = (long double)((*A)->num_known_eccentricities) / (long double)(*A)->len_list_vertice;
    long double B_pourcentage_traite_excentricites = (long double)((*B)->num_known_eccentricities) / (long double)(*B)->len_list_vertice;

    long double a_score = (1 - A_pourcentage_traite_excentricites) / (long double)(*A)->len_list_vertice;
    long double b_score = (1 - B_pourcentage_traite_excentricites) / (long double)(*B)->len_list_vertice;

    return (a_score < b_score) - (a_score > b_score);
}

int communities_size_sort_dsc_comp(const void* C, const void* D)
{
    commu_stats_t** A = (commu_stats_t**)C;
    commu_stats_t** B = (commu_stats_t**)D;

    long double A_pourcentage_traite_excentricites = (long double)((*A)->num_known_eccentricities) / (long double)(*A)->len_list_vertice;
    long double B_pourcentage_traite_excentricites = (long double)((*B)->num_known_eccentricities) / (long double)(*B)->len_list_vertice;


    long double a_score = (long double)(*A)->len_list_vertice * (1 - A_pourcentage_traite_excentricites);
    long double b_score = (long double)(*B)->len_list_vertice * (1 - B_pourcentage_traite_excentricites);

    return (a_score < b_score) - (a_score > b_score);
}

void tactique_communities_size(graph_stats_t* graph_stats)
{
    const unsigned long int len_commus_not_treated = graph_stats->len_commus_not_treated;
    commu_stats_t** commus_not_treated = graph_stats->commus_not_treated;

    if (graph_stats->tactique < COMMUNITY_SIZE_DSC)
        qsort(commus_not_treated, len_commus_not_treated, sizeof(commu_stats_t*), communities_size_sort_asc_comp);
    else if (graph_stats->tactique > COMMUNITY_SIZE_DSC)
        qsort(commus_not_treated, len_commus_not_treated, sizeof(commu_stats_t*), communities_size_sort_dsc_comp);

    graph_stats->commus_not_treated = commus_not_treated;
}

void tactique_communities_random(graph_stats_t* graph_stats)
{
    const unsigned long int len_commus_not_treated = graph_stats->len_commus_not_treated;
    commu_stats_t** commus_not_treated = graph_stats->commus_not_treated;

    //shuffle
    size_t i;
    for (i = 0; i < len_commus_not_treated - 1; i++)
    {
        size_t j = i + rand() / (RAND_MAX / (len_commus_not_treated - i) + 1);
        commu_stats_t* t = commus_not_treated[j];
        commus_not_treated[j] = commus_not_treated[i];
        commus_not_treated[i] = t;
    }
    graph_stats->commus_not_treated = commus_not_treated;
}

void sort_communities(graph_stats_t* graph_stats)
{
    switch (graph_stats->tactique)
    {
        case COMMUNITY_SIZE_ASC_RANDOM:
        case COMMUNITY_SIZE_DSC_RANDOM:
        case COMMUNITY_SIZE_ASC_HIGH_DEGREE:
        case COMMUNITY_SIZE_DSC_HIGH_DEGREE:
        case COMMUNITY_SIZE_ASC_LOW_DEGREE:
        case COMMUNITY_SIZE_DSC_LOW_DEGREE:
        case COMMUNITY_SIZE_ASC_BIG_DELTA:
        case COMMUNITY_SIZE_DSC_BIG_DELTA:
        {
            tactique_communities_size(graph_stats);
            break;
        }
        case COMMUNITY_RANDOM_RANDOM:
        case COMMUNITY_RANDOM_HIGH_DEGREE:
        case COMMUNITY_RANDOM_LOW_DEGREE:
        case COMMUNITY_RANDOM_BIG_DELTA:
        {
            tactique_communities_random(graph_stats);
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
        return *(graph_stats->commus_not_treated); // graph_stats->list_commus + i

    sort_communities(graph_stats);
    return *(graph_stats->commus_not_treated); // We return the address of the first element
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
    igraph_decompose(graph, &components, IGRAPH_WEAK, -1, 2);

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

void init_communities(graph_stats_t* graph_stats, igraph_vector_t* membership)
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
            ++j;

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

    igraph_vit_t vit;
    igraph_vit_create(graph_stats->graph, igraph_vss_all(), &vit);

    if ((long int)len_membership != IGRAPH_VIT_SIZE(vit))
    {
        fprintf(stderr, "Error in code: the len of 'membership' should be equal to the number of vertices here\n");
        exit( EXIT_FAILURE );
    }

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
            list_commus[j].vertices_not_treated = calloc(list_commus[j].len_list_vertice, sizeof(vertice_stats_t*));
            list_commus[j].len_vertices_not_treated = list_commus[j].len_list_vertice;

            list_commus[j].list_vertice[0].value = IGRAPH_VIT_GET(vit);
            list_commus[j].vertices_not_treated[0] = &(list_commus[j].list_vertice[0]);

            list_commus[j].list_vertice[0].degree = VECTOR(*degrees)[i];

            list_commus[j].list_vertice[0].borne_inf = -MY_INF;
            list_commus[j].list_vertice[0].borne_sup = MY_INF;

            list_commus[j].fill_list_vertice = 1;
        }
        else if (list_commus[j].fill_list_vertice < list_commus[j].len_list_vertice)
        {
            list_commus[j].list_vertice[list_commus[j].fill_list_vertice].value = IGRAPH_VIT_GET(vit);
            list_commus[j].vertices_not_treated[list_commus[j].fill_list_vertice] = &(list_commus[j].list_vertice[list_commus[j].fill_list_vertice]);;
            list_commus[j].fill_list_vertice += 1;
        }
        else
        {
            //PAS CENSE ETRE LA
            printf("%s\n", "aaaaa PAS CENSE ETRE LA");
            exit( EXIT_FAILURE );
        }

        IGRAPH_VIT_NEXT(vit);
    }

    igraph_vit_destroy(&vit);

    graph_stats->list_commus = list_commus;
}


void fill_commus_not_treated(graph_stats_t* graph_stats)
{
    unsigned long int len_list_commus = graph_stats->len_list_commus;
    graph_stats->len_commus_not_treated = len_list_commus;
    graph_stats->commus_not_treated = calloc(len_list_commus, sizeof(void*));

    for (unsigned long int i = 0; i < len_list_commus; ++i)
    {
        graph_stats->commus_not_treated[i] = &(graph_stats->list_commus[i]);
    }
}

void fill_without_communities(graph_stats_t* graph_stats, igraph_vector_t* degrees)
{
    commu_stats_t* list_commus = calloc(1, sizeof(commu_stats_t));
    graph_stats->len_list_commus = 1;
    graph_stats->modularity = 0;

    list_commus->list_vertice = calloc(graph_stats->num_vertices, sizeof(vertice_stats_t));
    list_commus->vertices_not_treated = calloc(graph_stats->num_vertices, sizeof(vertice_stats_t*));
    list_commus->len_list_vertice = 0;

    igraph_vit_t vit;
    igraph_vit_create(graph_stats->graph, igraph_vss_all(), &vit);

    const unsigned long int num_vertices = graph_stats->num_vertices;

    if ((long int)num_vertices != IGRAPH_VIT_SIZE(vit))
    {
        fprintf(stderr, "Error in code: the len of 'membership' should be equal to the number of vertices here\n");
        exit( EXIT_FAILURE );
    }

    for (long unsigned int i = 0; i < num_vertices; ++i)
    {
        list_commus->list_vertice[i].value = IGRAPH_VIT_GET(vit);
        list_commus->vertices_not_treated[i] = &(list_commus->list_vertice[i]);
        list_commus->list_vertice[i].degree = VECTOR(*degrees)[i];

        list_commus->list_vertice[i].borne_inf = -MY_INF;
        list_commus->list_vertice[i].borne_sup = MY_INF;
        list_commus->len_list_vertice += 1;

        IGRAPH_VIT_NEXT(vit);
    }

    igraph_vit_destroy(&vit);

    list_commus->len_vertices_not_treated = list_commus->len_list_vertice;
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
            mean_degree += (current_degree - mean_degree) / (j + 1); //FIXME for example : 0.98 shouldnt be possible
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
        printf("MODULARITY : %lf\n", graph_stats->modularity);
        //igraph_real_t *modularityy = malloc(sizeof(igraph_real_t *));
        //igraph_modularity(graph, &membership, NULL, 1, 0, modularityy);
        //printf("\nMODULARITY : %lf\n", *modularityy);


        //COMMUNITIES INTO GRAPH STATS
        init_communities(graph_stats, &membership);
        fill_communities(graph_stats, &membership, &degrees);
        fill_commus_not_treated(graph_stats);

        //FREE
        igraph_vector_destroy(&modularity);
        igraph_vector_destroy(&membership);


        //SORT
        sort_communities(graph_stats);
    }
    else
    {
        fill_without_communities(graph_stats, &degrees);
        fill_commus_not_treated(graph_stats);
    }

    igraph_vector_destroy(&degrees);

    //STATS
    set_commus_stats(graph_stats);
    return graph_stats->list_commus;
}

graph_stats_t* get_graph_stats(igraph_t* whole_graph, enum tactique tactique)
{
    graph_stats_t* graph_stats = calloc(1, sizeof(graph_stats_t));

    graph_stats->whole_graph = whole_graph;

    graph_stats->graph = whole_graph;

    //graph_stats->graph = get_largest_connected_component(whole_graph);
    igraph_t* graph = graph_stats->graph;

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
    {
        free(list_commus[index_commu].list_vertice);
        free(list_commus[index_commu].vertices_not_treated);
    }
    free(list_commus);
    free(graph_stats->commus_not_treated);
    free(graph_stats);
}

long int select_from(graph_stats_t* graph_stats)
{
    int selected_vertice = 0;

    switch (graph_stats->tactique) {
        case RANDOM:
        {
            //selected_vertice = tactique_1_random_old(NULL, 0); // FIXME TODO WARNING
            selected_vertice = tactique_1_random(graph_stats->list_commus);
            break;
        }
        case HIGH_DEGREE:
        {
            //selected_vertice = tactique_1_random_old(NULL, 0); // FIXME TODO WARNING
            selected_vertice = tactique_2_high_degree(graph_stats->list_commus);
            break;
        }
        case LOW_DEGREE:
        {
            selected_vertice = tactique_3_low_degree(graph_stats->list_commus);
            break;
        }
        case BIG_DELTA:
        {
            selected_vertice = tactique_4_big_delta(graph_stats->list_commus);
            break;
        }
        case COMMUNITY_SIZE_ASC_RANDOM:
        case COMMUNITY_SIZE_DSC_RANDOM:
        case COMMUNITY_RANDOM_RANDOM:
        {
            commu_stats_t* community = get_a_community(graph_stats);
            selected_vertice = tactique_1_random(community);
            break;
        }
        case COMMUNITY_SIZE_ASC_HIGH_DEGREE:
        case COMMUNITY_SIZE_DSC_HIGH_DEGREE:
        case COMMUNITY_RANDOM_HIGH_DEGREE:
        {
            commu_stats_t* community = get_a_community(graph_stats);
            selected_vertice = tactique_2_high_degree(community);
            break;
        }
        case COMMUNITY_SIZE_ASC_LOW_DEGREE:
        case COMMUNITY_SIZE_DSC_LOW_DEGREE:
        case COMMUNITY_RANDOM_LOW_DEGREE:
        {
            commu_stats_t* community = get_a_community(graph_stats);
            selected_vertice = tactique_3_low_degree(community);
            break;
        }
        case COMMUNITY_SIZE_ASC_BIG_DELTA:
        case COMMUNITY_SIZE_DSC_BIG_DELTA:
        case COMMUNITY_RANDOM_BIG_DELTA:
        {
            commu_stats_t* community = get_a_community(graph_stats);
            selected_vertice = tactique_4_big_delta(community);
            break;
        }
        default:
            fprintf(stderr, "We tried to apply an unknown tactic\n");
            exit( EXIT_FAILURE );

    }

    return selected_vertice;
}

int are_all_commu_treated(commu_stats_t* list_commus, unsigned long int len_list_commus)
{
    for (unsigned long int i_commu = 0; i_commu < len_list_commus; ++i_commu)
    {
        if (list_commus[i_commu].len_vertices_not_treated != 0)
            return 0;
    }

    return 1;
}

unsigned long int* get_eccentricities(igraph_t* whole_graph, int delta, unsigned long int* num_bfs, enum tactique tactique)
{
    //trouver la plus grande Composante connexe
    //graph = get_largest_connected_component(graph);
    // On a commenté ce bout de code car les graphes fournis sont connexes
    graph_stats_t* graph_stats = get_graph_stats(whole_graph, tactique);

    igraph_t* graph = graph_stats->graph;

    unsigned long int num_vertices = graph_stats->num_vertices;
    //INIT
    //W_list* W_head = linked_list_init(graph_stats);
    unsigned long int* eccentricities = calloc(num_vertices, sizeof(unsigned long int));
    long int* eccentricities_L = calloc(num_vertices, sizeof(long int));
    long int* eccentricities_U = calloc(num_vertices, sizeof(long int));

    // Algorithme de Takes et Kosters
    // Init
    for (unsigned long int i = 0; i < num_vertices; ++i)
    {
        //eccentricities[w] = 0; // Unrequired since we used calloc
        eccentricities_L[i] = -MY_INF;
        eccentricities_U[i] = MY_INF;
    }

    while (graph_stats->len_commus_not_treated != 0)
    {
        // Nous selectionnons le point initial par lequel on va commencer nos
        // calculs d'excentricite
        const long int v = select_from(graph_stats);

        // on va calculer les distance de v par rapport aux w en meme temps que
        // le calcul d'excentricites (grâce aux BFS)
        igraph_vector_t distance; // = calloc(num_vertices, sizeof(long int));
        igraph_vector_init(&distance, graph_stats->num_vertices);

        //calcul d'excentricites
        eccentricities[v] = get_vertice_eccentricity(graph, v, &distance);
        *num_bfs += 1;

        for (unsigned long int i_commu = 0; i_commu < graph_stats->len_commus_not_treated; ++i_commu)
        {
            if (graph_stats->commus_not_treated[i_commu]->len_vertices_not_treated != 0) // removable TODO FIXME
            {
                vertice_stats_t** vertices_not_treated = graph_stats->commus_not_treated[i_commu]->vertices_not_treated;
                for (unsigned long int i_vertices_not_treated = 0;
                    i_vertices_not_treated < graph_stats->commus_not_treated[i_commu]->len_vertices_not_treated;
                    ++i_vertices_not_treated)
                {
                    unsigned long int w = vertices_not_treated[i_vertices_not_treated]->value;
                    // LES BORNES
                    long int d = VECTOR(distance)[w];

                    vertices_not_treated[i_vertices_not_treated]->borne_inf = fmax(vertices_not_treated[i_vertices_not_treated]->borne_inf, fmax(eccentricities[v] - d, d));
                    vertices_not_treated[i_vertices_not_treated]->borne_sup = fmin(vertices_not_treated[i_vertices_not_treated]->borne_sup, eccentricities[v] + d);//inf

                    eccentricities_L[w] = fmax(eccentricities_L[w], fmax(eccentricities[v] - d, d));
                    eccentricities_U[w] = fmin(eccentricities_U[w], eccentricities[v] + d); // inf

                    // Si l'écart entre les deux bornes est inférieur à delta,
                    // alors nous avons trouve la bonne excentricite
                    //if (vertices_not_treated[i_vertices_not_treated]->borne_sup - vertices_not_treated[i_vertices_not_treated]->borne_inf <= delta)
                    if (eccentricities_U[w] - eccentricities_L[w] <= delta)
                    {
                        eccentricities[w] = vertices_not_treated[i_vertices_not_treated]->borne_inf;
                        eccentricities[w] = eccentricities_L[w];

                        // Pop the value;
                        // We pop in vertices_not_treated
                        graph_stats->commus_not_treated[i_commu]->num_known_eccentricities += 1;
                        vertices_not_treated[i_vertices_not_treated] = vertices_not_treated[--(graph_stats->commus_not_treated[i_commu]->len_vertices_not_treated)];
                        --i_vertices_not_treated; // We do this trick to re evaluate current vertice since we swapped

                        if (graph_stats->commus_not_treated[i_commu]->num_known_eccentricities == graph_stats->commus_not_treated[i_commu]->len_list_vertice)
                        {
                            graph_stats->commus_not_treated[i_commu] = graph_stats->commus_not_treated[graph_stats->len_commus_not_treated - 1];
                            --graph_stats->len_commus_not_treated;
                            --i_commu; // We do this trick to re evaluate current commu since we swapped
                            break;
                        }
                    }
                }
            }
        }
        igraph_vector_destroy(&distance); // FIXME, maybe use the same everytimes
        if (*num_bfs % 10 == 0)
        {
            commu_stats_t** commus_not_treated = graph_stats->commus_not_treated;
            unsigned long int len_commus_not_treated = graph_stats->len_commus_not_treated;
            unsigned long int num_not_treated_vertices = 0;
            for (unsigned long int i = 0; i < len_commus_not_treated; ++i)
            {
                num_not_treated_vertices += commus_not_treated[i]->len_vertices_not_treated;
            }
            printf("\rProgress bar eccentricities calculated: %Lf%%   \e[?25l", (1 - (long double)num_not_treated_vertices / graph_stats->num_vertices) * 100);
            fflush(stdout);
        }
    }

    free_graph_stats(graph_stats);
    free(eccentricities_L);
    free(eccentricities_U);

    printf("\e[?25h\n");

    return eccentricities;
}

enum tactique string_to_tactique(char* tactique_str)
{
    if (strcmp(tactique_str, "RANDOM") == 0)
        return RANDOM;
    if (strcmp(tactique_str, "HIGH_DEGREE") == 0)
        return HIGH_DEGREE;
    if (strcmp(tactique_str, "LOW_DEGREE") == 0)
        return LOW_DEGREE;
    if (strcmp(tactique_str, "BIG_DELTA") == 0)
        return BIG_DELTA;

    if (strcmp(tactique_str, "COMMUNITY_SIZE_ASC_RANDOM") == 0)
        return COMMUNITY_SIZE_ASC_RANDOM;
    if (strcmp(tactique_str, "COMMUNITY_SIZE_DSC_RANDOM") == 0)
        return COMMUNITY_SIZE_DSC_RANDOM;
    if (strcmp(tactique_str, "COMMUNITY_SIZE_ASC_HIGH_DEGREE") == 0)
        return COMMUNITY_SIZE_ASC_HIGH_DEGREE;
    if (strcmp(tactique_str, "COMMUNITY_SIZE_DSC_HIGH_DEGREE") == 0)
        return COMMUNITY_SIZE_DSC_HIGH_DEGREE;
    if (strcmp(tactique_str, "COMMUNITY_SIZE_ASC_LOW_DEGREE") == 0)
        return COMMUNITY_SIZE_ASC_LOW_DEGREE;
    if (strcmp(tactique_str, "COMMUNITY_SIZE_DSC_LOW_DEGREE") == 0)
        return COMMUNITY_SIZE_DSC_LOW_DEGREE;
    if (strcmp(tactique_str, "COMMUNITY_SIZE_ASC_BIG_DELTA") == 0)
        return COMMUNITY_SIZE_ASC_BIG_DELTA;
    if (strcmp(tactique_str, "COMMUNITY_SIZE_DSC_BIG_DELTA") == 0)
        return COMMUNITY_SIZE_DSC_BIG_DELTA;

    if (strcmp(tactique_str, "COMMUNITY_RANDOM_RANDOM") == 0)
        return COMMUNITY_RANDOM_RANDOM;
    if (strcmp(tactique_str, "COMMUNITY_RANDOM_HIGH_DEGREE") == 0)
        return COMMUNITY_RANDOM_HIGH_DEGREE;
    if (strcmp(tactique_str, "COMMUNITY_RANDOM_LOW_DEGREE") == 0)
        return COMMUNITY_RANDOM_LOW_DEGREE;
    if (strcmp(tactique_str, "COMMUNITY_RANDOM_BIG_DELTA") == 0)
        return COMMUNITY_RANDOM_BIG_DELTA;
    return RANDOM;
}

void custom_eccentricities(igraph_t* graph,
                            unsigned long int num_vertices,
                            FILE* file_eccentricities,
                            char* tactique_str)
{
    unsigned long int num_bfs = 0;
    int delta = 0;

    enum tactique tactique = string_to_tactique(tactique_str);

    printf("Tactique: %s\n", tactique_str);

    unsigned long int* res_eccentricities = get_eccentricities(graph, delta, &num_bfs, tactique);
    printf("Eccentricities: OK\n");
    printf("Value of delta: %d\n", delta);
    printf("Number of BFS used: %ld\n", num_bfs);

    for (unsigned long int i = 0; i < num_vertices; ++i)
        fprintf(file_eccentricities, "%ld ", (long) res_eccentricities[i]);

    free(res_eccentricities);
}
