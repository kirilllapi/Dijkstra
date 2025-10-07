#include <iostream>
#include <fstream>
#include "adjacency_list.h"
#include "dijkstra.h"

int* read_adjacency_matrix_from_file(const char* filename, int& node_count, int& arc_count)
{
    std::ifstream infile(filename);
    if (!infile)
    {
        std::cout << "Failed to open file!" << std::endl;
        return NULL;
    }

    infile >> node_count;
    if (node_count <= 0)
    {
        std::cout << "Invalid graph size!" << std::endl;
        return NULL;
    }

    int* matrix = (int*)malloc(node_count * node_count * sizeof(int));
    if (matrix == NULL)
    {
        std::cout << "Memory allocation error!" << std::endl;
        return NULL;
    }

    arc_count = 0;
    int weight;

    for (int i = 0; i < node_count; i++)
    {
        for (int j = 0; j < node_count; j++)
        {
            infile >> weight;

            if (weight != 0) arc_count++;

            matrix[i * node_count + j] = weight;
        }
    }

    infile.close();
    return matrix;
}


/// <summary>
/// 
/// </summary>
/// <param name="graph"></param>
/// <param name="size"></param>
void print_arrays(adjacency_list graph, int size)
{
    std::cout << "nodes:  ";
    for (int i = 0; i < size; i++) 
        std::cout << graph->nodes[i] << "\t";
    std::cout << "\n";

    std::cout << "next:   ";
    for (int i = 0; i < size; i++) 
        std::cout << graph->next[i] << "\t";
    std::cout << "\n";

    std::cout << "weight: ";
    for (int i = 0; i < size; i++) 
        std::cout << graph->weight[i] << "\t";
    std::cout << "\n";
}



int main()
{
    int arc_count, node_count;

    int* matrix = read_adjacency_matrix_from_file("matrix.txt", node_count, arc_count);

    if (matrix == NULL) return 1;

    std::cout << "Adjacency matrix:" << std::endl;
    for (int i = 0; i < node_count; ++i) {
        for (int j = 0; j < node_count; ++j)
            std::cout << matrix[i * node_count + j] << " ";
        std::cout << std::endl;
    }

    adjacency_list graph = init_adjacency_list(node_count, arc_count, matrix);
    int size = node_count +  arc_count;
    print_arrays(graph, size);

    find_short_path(graph, node_count, arc_count, 2, 5);
    
    free(graph);
    return 0;
}