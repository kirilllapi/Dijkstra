#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "adjacency_list.h"


/// <summary>
/// Инициализация пустого графа.
/// </summary>
/// <param name="size">- количество узлов графа</param>
adjacency_list init_adjacency_list(int node_count, int arc_count, int* matrix)
{
    int size = node_count + arc_count;

    adjacency_list graph = (adjacency_list)malloc(sizeof(*graph));
    if (graph == NULL)
    {
        std::cout << "Memory allocation for graph error!" << std::endl;
        return NULL;
    }

    graph->nodes = (int*)calloc(size, sizeof(int));
    if (graph->nodes == NULL)
    {
        std::cout << "Memory allocation for nodes error!" << std::endl;
        free(graph->nodes);
        free(graph);

        return NULL;
    }

    graph->next = (int*)calloc(size, sizeof(int));
    if (graph->next == NULL)
    {
        std::cout << "Memory allocation for next error!" << std::endl;
        free(graph->nodes);
        free(graph->next);
        free(graph);

        return NULL;
    }


    graph->weight = (MY_TYPE*)calloc(size, sizeof(MY_TYPE));
    if (graph->weight == NULL)
    {
        std::cout << "Memory allocation for weight error!" << std::endl;
        free(graph->nodes);
        free(graph->next);
        free(graph->weight);
        free(graph);

        return NULL;
    }


    int edge_index = node_count;

    for (int i = 0; i < node_count; i++)
    {
        int last_edge = 0;

        for (int j = 0; j < node_count; j++)
        {
            int weight = matrix[i * node_count + j];
            if (weight != 0)
            {
                graph->nodes[edge_index] = j + 1;
                graph->weight[edge_index] = weight;
                graph->next[edge_index] = 0;

                if (graph->next[i] == 0)
                {
                    graph->next[i] = edge_index + 1;
                }
                else
                {
                    graph->next[last_edge] = edge_index + 1; 
                }

                last_edge = edge_index;
                edge_index++;
            }
        }
    }

    return graph;
}