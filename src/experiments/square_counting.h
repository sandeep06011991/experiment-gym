//
// Created by sandeep on 23/11/19.
//

#include <graph.h>

#ifndef V3_SQUARE_COUNTING_H
#define V3_SQUARE_COUNTING_H

#endif //V3_SQUARE_COUNTING_H

NODETYPE naive_vertex_explore(int i, Graph *graph);

int naive_square_counting(Graph *graph);

NODETYPE not_naive_vertex_explore(int i, Graph *graph);

void bit_vector_based(Graph *graph);

void hybrid_bit_vector_based(Graph *graph);

void dual_bit_matrix_based(Graph *graph);

void redundant_Intersection(Graph *graph);

void traversal_based_sort(Graph *graph);