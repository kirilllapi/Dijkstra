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
int* find_short_path(adjacency_list graph, int node_count, int arc_count, int from, int to)
{
	int next_node, adjacency_node;

	int* labels = (int*)calloc(node_count, sizeof(int));
	char* visited = (char*)calloc(node_count, sizeof(char));
	int* path = (int*)calloc(node_count, sizeof(int));

	if (path == NULL)
	{
outfile << "Memory allocation error!" << std::endl;
		free(labels);
		free(visited);
		return NULL;
	}


	if (labels == NULL || visited == NULL)
	{
		outfile << "Memory allocation error!" << std::endl;
		free(labels);
		free(visited);
		return NULL;
	}


	for (int i = 0; i < node_count; i++) 
	{
		path[i] = -1;        
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

				path[adjacency_node] = node_min_label + 1; // <-- исправлено

				outfile << "Node " << node_min_label + 1 << " -> " << adjacency_node + 1
					<< " (weight = " << graph->weight[next_node - 1] << ")\n";
			}
			next_node = graph->next[next_node - 1];
		}	
	}

	int current = to - 1;

	int* tmp_path = (int*)calloc(node_count, sizeof(int));

	if (!tmp_path) 
	{
		outfile << "Memory allocation error for tmp_path!" << std::endl;
		free(labels);
		free(visited);
		return NULL;
	}

	int count = 0;

	while (current != -1)
	{
		tmp_path[count++] = current + 1;
		if (current == from - 1) break;
		current = path[current] - 1; // идём по предшественникам
	}

	// переворачиваем tmp_path для правильного порядка
	for (int i = 0; i < count / 2; i++)
	{
		int t = tmp_path[i];
		tmp_path[i] = tmp_path[count - 1 - i];
		tmp_path[count - 1 - i] = t;
	}

	// выводим
	outfile << "\nPath: ";
	for (int i = 0; i < count; i++)
	{
		if (i > 0) outfile << " -> ";
		outfile << tmp_path[i];
	}

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

	return tmp_path;
}