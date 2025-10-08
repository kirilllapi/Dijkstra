#include "input_output.h"


std::ofstream outfile("report.txt");


/// <summary>
/// 
/// </summary>
/// <param name="filename"></param>
/// <param name="from"></param>
/// <param name="to"></param>
/// <param name="node_count"></param>
/// <param name="arc_count"></param>
/// <returns></returns>
int* read_adjacency_matrix_from_file(const char* filename, int& from, int& to, int& node_count, int& arc_count)
{
    std::ifstream infile(filename);
    if (!infile)
    {
        outfile << "Failed to open file!" << std::endl;
        return NULL;
    }

    infile >> node_count;
    outfile << "Size matrix: " << node_count << "x" << node_count << std::endl;
    if (node_count <= 0)
    {
        outfile << "Invalid graph size!" << std::endl;
        return NULL;
    }

    int* matrix = (int*)malloc(node_count * node_count * sizeof(int));
    if (matrix == NULL)
    {
        outfile << "Memory allocation error!" << std::endl;
        return NULL;
    }


    infile >> from;
    infile >> to;
    outfile << "Path from: " << from << std::endl;
    outfile << "Path to: " << to << std::endl;
    outfile << "\n " << std::endl;


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
/// <param name="matrix"></param>
/// <param name="node_count"></param>
void print_adjacency_matrix_in_file(int* matrix, int node_count)
{
    outfile << "Adjacency matrix:" << std::endl;

    for (int i = 0; i < node_count; ++i) 
    {
        for (int j = 0; j < node_count; ++j) outfile << matrix[i * node_count + j] << " ";
        outfile << std::endl;
    }
}


/// <summary>
/// 
/// </summary>
/// <param name="graph"></param>
/// <param name="size"></param>
void print_arrays_in_file(adjacency_list graph, int node_count, int arc_count)
{
    int size = node_count + arc_count;

    outfile << "\n\nAdjacency_list: " << std::endl;

    outfile << "Nodes:  ";
    for (int i = 0; i < size; i++) outfile << graph->nodes[i] << "\t";
    outfile << "\n";

    outfile << "Next:   ";
    for (int i = 0; i < size; i++) outfile << graph->next[i] << "\t";
    outfile << "\n";

    outfile << "Weight: ";
    for (int i = 0; i < size; i++) outfile << graph->weight[i] << "\t";
    outfile << "\n";
}


