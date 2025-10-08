#include "adjacency_list.h"
#include "dijkstra.h"
#include "input_output.h"

int main()
{
    int arc_count, node_count, from, to;

    int* matrix = read_adjacency_matrix_from_file("matrix.txt", from, to, node_count, arc_count);

    if (matrix == NULL) return 1;

    print_adjacency_matrix_in_file(matrix, node_count);

    adjacency_list graph = init_adjacency_list(node_count, arc_count, matrix);

    print_arrays_in_file(graph, node_count, arc_count);

    find_short_path(graph, node_count, arc_count, from, to);
    
    free(graph);
    free(matrix);
    return 0;
}