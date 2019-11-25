//
// Created by sandeep on 14/8/19.
//
#include <iostream>
#include <intersection.h>
#include <timer.h>
#include <branch_eval.h>
#include "graph.h"
#include "experiments/naive_triangle_counting.h"
#include "square_counting.cpp"
#include "experiments/array_exp.h"
#include "operators.h"
using namespace std;

/* Main entry point */
int main(int argc, char *argv[]) {

//    cout << "No. of arguments recieved " << argc <<"\n";
//    Graph * graph = new Graph("data/com-amazon.ungraph.txt.ro");
    Graph * graph = new Graph("data/com-youtube.ungraph.txt.ro");

//    Graph * graph = new Graph("data/com-lj.ungraph.txt.ro");
//    Graph * graph = new Graph("data/data1.graph");
//    graph->debugGraph();
//  468774021
//    4700600
//   35877389

//    generateGraphNothingCommon(1000,1000,"data/data1.graph");
//    Graph * graph = new Graph("data/data1.graph");
//    std::cout << "Total no nodes :" << graph->getNoVertexes() <<"\n";
//    generateGraphSkewNothingCommon(10,10,1,10000,"data/data1.graph");
    bit_vector_based(graph);
    naive_square_counting(graph);

//    graph->countNodesWithPosEdgesGreater(100);
//    graph->countNodesWithPosEdgesGreater(1000);
//    graph->countNodesWithPosEdgesGreater(10000);
//    not_naive_square(graph);
//        sort_square(graph);
//    hybrid_square(graph,15);
//
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