#include "dijkstra.h"
#include <iostream>

void find_short_path(adjacency_list graph, int node_count, int arc_count, int source, int target)
{
	int next_node, adjacency_node;

	int* labels = (int*)calloc(node_count, sizeof(int));
	char* visited = (char*)calloc(node_count, sizeof(char));

	if (labels == NULL || visited == NULL)
	{
		std::cout << "Memory allocation error!" << std::endl;
		free(labels);
		free(visited);
		return;
	}

	for (int i = 0; i < node_count; i++)
	{

		labels[i] = INT_MAX;
	}

	labels[source - 1] = 0;

	

	for (int i = 0; i < node_count; i++)
	{

		// Поиск минимальной пометки из 
		int min_label = INT_MAX;
		int node_min_label = -1;

		for (int k = 0; k < node_count; k++)
		{
			if (!visited[k] && labels[k] < min_label)
			{
				min_label = labels[k];
				node_min_label = k;
			}
		}

		if (node_min_label == -1) break;
	
		visited[node_min_label] = true;

		std::cout << "New view label" << std::endl;


		// Обход списков смежности
		next_node = graph->next[node_min_label];

		std::cout << "For node " << node_min_label + 1 << ": " << next_node << std::endl;

		while (next_node != 0)
		{
			adjacency_node = graph->nodes[next_node - 1] - 1;

			// Пропускаем дугу в саму себя
			if (adjacency_node == node_min_label)
			{
				next_node = graph->next[next_node - 1];
				continue;
			}

			if (!visited[adjacency_node] &&
				labels[node_min_label] + graph->weight[next_node - 1] < labels[adjacency_node])
			{
				labels[adjacency_node] = labels[node_min_label] + graph->weight[next_node - 1];
			}


			std::cout << "Node " << node_min_label + 1 << " -> " << adjacency_node + 1
				<< " (weight = " << graph->weight[next_node - 1] << ")\n";

			next_node = graph->next[next_node - 1];
		}	
	}

	for (int i = 0; i < node_count; i++) std::cout << labels[i] << "\t";
}