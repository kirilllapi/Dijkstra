#pragma once

#include <iostream>
#include <fstream>
#include "adjacency_list.h"

extern std::ofstream outfile;

int* read_adjacency_matrix_from_file(const char*, int&, int&, int&, int&);
void print_arrays_in_file(adjacency_list, int, int);
void print_adjacency_matrix_in_file(int*, int);
