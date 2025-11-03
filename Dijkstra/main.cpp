#include "adjacency_list.h"
#include "dijkstra.h"
#include "input_output.h"
#include "UI.h"

int main()
{
    int arc_count, node_count, from, to;

    int* matrix = read_adjacency_matrix_from_file("matrix.txt", from, to, node_count, arc_count);

    if (matrix == NULL)
    {
        std::cerr << "The matrix contains a negative weight!";
        return 1;
    }

    print_adjacency_matrix_in_file(matrix, node_count);

    adjacency_list graph = init_adjacency_list(node_count, arc_count, matrix);

    print_arrays_in_file(graph, node_count, arc_count);

    int* path = find_short_path(graph, node_count, arc_count, from, to);
    if (path == NULL)
    {
        std::cerr << "Path is invalid!";
        return 1;
    }

    drawWindow(matrix, node_count, arc_count, path, from, to);
    
    free(graph);
    free(matrix);
    free(path);
    return 0;
}