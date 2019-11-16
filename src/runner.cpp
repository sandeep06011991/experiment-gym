//
// Created by sandeep on 14/8/19.
//
#include <iostream>
#include <intersection.h>
#include <timer.h>
#include "graph.h"
#include "experiments/naive_triangle_counting.h"
#include "experiments/square_counting.h"
#include "experiments/array_exp.h"
#include "operators.h"
using namespace std;

/* Main entry point */
int main(int argc, char *argv[]) {

    cout << "No. of arguments recieved " << argc <<"\n";
//    Graph * graph = new Graph("data/com-amazon.ungraph.txt.ro");
    Graph * graph = new Graph("data/com-youtube.ungraph.txt.ro");

//    Graph * graph = new Graph("data/com-lj.ungraph.txt.ro");
//    Graph * graph = new Graph("data/data1.graph");
//    graph->debugGraph();

    naive_square_counting(graph);
//    graph->countNodesWithPosEdgesGreater(100);
//    graph->countNodesWithPosEdgesGreater(1000);
//    graph->countNodesWithPosEdgesGreater(10000);
//    not_naive_square(graph);
//        sort_square(graph);
//    hybrid_square(graph,15);
//         bit_vector_based(graph);
//    hybrid_square(graph,20);
    //        compare_naive_vs_not_naive(graph);
//    batched_triangle_counting(graph);
//    triangle_counting(graph);
//      test_array_intersection();

//  Experiment 1
//    compare_naive_vs_not_naive(graph);
//    test_power_of_2_operator(1000000);
//    test_division_operator(1000000);

}