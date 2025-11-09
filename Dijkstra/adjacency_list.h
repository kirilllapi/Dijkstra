#pragma once
#include <stdlib.h>  
#include <stdio.h>  

// ================== Структура списков смежности ==============
/// <summary>
/// Структура списков смежности на основе массивов
/// </summary>
typedef struct adjacency_list_s {
	int* nodes;
	int* next;
	int* weight;
} *adjacency_list;


// ================== Прототипы функций ===============
adjacency_list init_adjacency_list(int, int, int*);