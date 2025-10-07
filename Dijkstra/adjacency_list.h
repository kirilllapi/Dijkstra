#pragma once
#include <stdlib.h>  
#include <stdio.h>  

#define MY_TYPE int

// ================== Структура списков смежности ==============
/// <summary>
/// Структура списков смежности на основе массивов
/// </summary>
typedef struct adjacency_list_s {
	int* nodes;
	int* next;
	MY_TYPE* weight;
} *adjacency_list;


// ================== Прототипы функций ===============
adjacency_list init_adjacency_list(int, int, int*);