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

	int* labels = graph->weight;
	int* visited = graph->nodes;
	int* path = (int*)calloc(node_count, sizeof(int));

	if (path == NULL)
	{
		std::cout << "Memory allocation for path error!" << std::endl;
		return NULL;
	}


	for (int i = 0; i < node_count; i++) 
	{
		path[i] = -1;        
		labels[i] = INT_MAX;  
	}

	labels[from - 1] = 0;
	int p = from - 1;

	outfile << "\n================ Path from " << from << " to " << to << " =================" << std::endl;

	while(true)
	{
		// Поиск минимальной пометки из всех
		int min_label = INT_MAX;

		for (int k = 0; k < node_count; k++)
		{
			if (!visited[k] && labels[k] < min_label)
			{
				min_label = labels[k];
				p = k;
			}
		}
		if (min_label == INT_MAX || p == to - 1) break;

		visited[p] = true;

		// Обход списков смежности
		next_node = graph->next[p];

		while (next_node != 0)
		{
			adjacency_node = graph->nodes[next_node - 1] - 1;

			// Пропускаем дугу в саму себя
			if (adjacency_node == p)
			{
				next_node = graph->next[next_node - 1];
				continue;
			}

			if (!visited[adjacency_node] && labels[p] + graph->weight[next_node - 1] < labels[adjacency_node])
			{
				labels[adjacency_node] = labels[p] + graph->weight[next_node - 1];

				path[adjacency_node] = p + 1;
			}
			next_node = graph->next[next_node - 1];
		}

		outfile << "\nLabels: for node " << p + 1 << "\n";
		for (int i = 0; i < node_count; i++)
		{
			if (labels[i] == INT_MAX)
			{
				outfile << "inf" << "\t";
			}
			else
			{
				outfile << labels[i] << "\t";
			}
		}
		outfile << "\n";
	}


	
	int* result_path = (int*)calloc(node_count, sizeof(int));
	if (!result_path) 
	{
		outfile << "Memory allocation error for result_path!" << std::endl;
		free(labels);
		free(visited);
		return NULL;
	}

	int count = 0;
	int current = to - 1;

	// Записываем в новый массив до точки откуда 
	while (current != -1)
	{
		result_path[count++] = current + 1;

		if (current == from - 1) break;

		// если нет предшественника — пути не существует
		if (path[current] == -1)
		{
			outfile << "\nPath is not available!\n";
			count = 0; 
			break;
		}

		current = path[current] - 1;
	}

	// Переворачиваем result_path для правильного порядка
	for (int i = 0; i < count / 2; i++)
	{
		int t = result_path[i];
		result_path[i] = result_path[count - 1 - i];
		result_path[count - 1 - i] = t;
	}

	// Выводим все в файл
	if (count != 0)
	{
		outfile << "\nPath: ";
		for (int i = 0; i < count; i++)
		{
			if (i > 0) outfile << " -> ";
			outfile << result_path[i];
		}
	}

	outfile << "\n\nResult labels: ";
	for (int i = 0; i < node_count; i++)
	{
		if (labels[i] == INT_MAX)
		{
			outfile << "inf" << "\t";
		}
		else
		{
			outfile << labels[i] << "\t";
		}
	}

	outfile << "\n\nShortest distance to target (" << to << "): " << (labels[to - 1] == INT_MAX ? -1 : labels[to - 1]) << "\n";

	free(path);

	return result_path;
}