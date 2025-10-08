#include <iostream>
#include "dijkstra.h"
#include "input_output.h"


/// <summary>
/// 
/// </summary>
/// <param name="graph"></param>
/// <param name="node_count"></param>
/// <param name="arc_count"></param>
/// <param name="from"></param>
/// <param name="to"></param>
void find_short_path(adjacency_list graph, int node_count, int arc_count, int from, int to)
{
	int next_node, adjacency_node;

	int* labels = (int*)calloc(node_count, sizeof(int));
	char* visited = (char*)calloc(node_count, sizeof(char));
	int* path = (int*)calloc(node_count, sizeof(int));


	if (labels == NULL || visited == NULL)
	{
		outfile << "Memory allocation error!" << std::endl;
		free(labels);
		free(visited);
		return;
	}

	for (int i = 0; i < node_count; i++)
	{
		path[i] = INT_MAX;
		labels[i] = INT_MAX;
	}

	labels[from - 1] = 0;

	outfile << "\n================ Path from " << from << " to " << to << " =================" << std::endl;

	for (int i = 0; i < node_count; i++)
	{

		// Поиск минимальной пометки из всех
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

		// Обход списков смежности
		next_node = graph->next[node_min_label];


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
				
				path[i] = node_min_label + 1;

				outfile << "Node " << node_min_label + 1 << " -> " << adjacency_node + 1
					<< " (weight = " << graph->weight[next_node - 1] << ")\n";
			}
			next_node = graph->next[next_node - 1];
		}	
	}

	outfile << "\nPath: ";
	char first_node = 1;
	for (int i = 0; i < node_count; i++)
	{
		if (!(path[i] == INT_MAX))
		{
			
			outfile << path[i] << " -> ";
			
		}
	}
	outfile << to;

	outfile << "\n\nLabels: ";
	for (int i = 0; i < node_count; i++)
	{
		outfile << (labels[i] == INT_MAX ? -1 : labels[i]) << "\t";
	}


	outfile << "\n\nShortest distance to target (" << to << "): "
		<< (labels[to - 1] == INT_MAX ? -1 : labels[to - 1]) << "\n";

	free(labels);
	free(visited);
	free(path);
}