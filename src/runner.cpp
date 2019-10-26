//
// Created by sandeep on 14/8/19.
//
#include <iostream>
#include <intersection.h>
#include <timer.h>
#include "graph.h"
#include "experiments/naive_triangle_counting.h"
#include "experiments/array_exp.h"
using namespace std;

/* Main entry point */
int main(int argc, char *argv[]) {

    cout << "No. of arguments recieved " << argc <<"\n";
    Graph * graph = new Graph("data/com-lj.ungraph.txt.ro");
//    Graph * graph = new Graph("data/com-youtube.ungraph.txt.ro");
//    Graph * graph = new Graph("data/data1.graph");
//    graph->debugGraph();
    triangle_counting(graph);
//    batched_triangle_counting(graph);
//    triangle_counting(graph);
//      test_array_intersection();
}