//
// Created by sandeep on 14/8/19.
//
#include <iostream>
#include <intersection.h>
#include <timer.h>
#include <branch_eval.h>
#include <square_counting.h>
#include <4clique.h>
#include <charecteristics.h>
#include <GHDNode.h>
#include <Pipeline.h>
#include <NaiveGHDEvaluator.h>
#include "graph.h"
#include "experiments/naive_triangle_counting.h"
#include "square_counting.cpp"
#include "experiments/array_exp.h"
#include "operators.h"
#include "memory_experiments.h"

using namespace std;


/* New entry point. */
// Does not work for anything other than cliques

int main(int argc, char *argv[]){
//    Graph *    graph = new Graph("data/data1.graph");
//    Graph *graph = new Graph("data/clique");
//    Graph * graph = new Graph("data/com-youtube.ungraph.txt.ro");
    Graph * graph = new Graph("data/com-lj.ungraph.txt.ro");
//    Graph *graph = new Graph("data/com-amazon.ungraph.txt.ro");
//    GHDNode * node = getTriangleGHDNode();
//        int a =  graph->getNoVertexes();

//    GHDNode * node = get6Clique();
//    GHDNode * node = get5Clique();
//    GHDNode * node = getSquareNode();
    GHDNode * node = get4Clique();

//    NaiveGHDEvaluator * n = new NaiveGHDEvaluator(graph,node);
//    cout << "result" << n->evaluate() <<"\n";
    Pipeline * pipeline = new Pipeline(graph, node);
    pipeline->run();


    cout << "Sorting and preprocessing" << get_timer(BITMATRIXCONSTRUCTION)<<"\n";
    cout << "Bit intersection" << get_timer(SIMDINTERSECTION)<<"\n";
//    cout << "Bit Intersection" << get_timer(ADGLISTINTERSECTION) << "\n";
    cout << " Intersection " << get_timer(BITSIMDINTERSECTIONTIME) <<"\n";
//    cout << "neighbour traversal time " << get_timer(REDMATRIXCONSTRUCTION) << "\n";
}

/* Old entry point */
int oldTests(int argc, char *argv[]) {

//    cout << "No. of arguments recieved " << argc <<"\n";
//    Graph * graph = new Graph("data/com-amazon.ungraph.txt.ro");
//    Graph * graph = new Graph("data/com-youtube.ungraph.txt.ro");
//    Graph * graph = new Graph("data/com-lj.ungraph.txt.ro");
//    Graph * graph = new Graph("data/data1.graph");
//    Graph *graph  = new Graph("data/com-orkut.ungraph.txt.ro");
//    graph->debugGraph();
//    triangle_counting(graph);
//    measure_cost_of_random_reads(graph);
//    test_cost_of_read_and_write();
//    test_cost_of_page_faulting();
//    extend_intersect_version(graph);
//    graphPartition(graph);
//    refetchNdArray(graph);
//    generateGraphNothingCommon(1000,1000,"data/data1.graph");
//    graphPartition(graph);
//    tiled_triangle_counting(graph);
//    naive_square_counting(graph);
//    dual_bit_matrix_based(graph);
//    bit_vector_based(graph);
//    traversal_based_sort(graph);
//    naive4cliqueCounting(graph);
//    v3cliqueCounting(graph);
//    redundant_Intersection(graph);
//    hybrid_bit_vector_based(graph);
//    graph->countNodesWithPosEdgesGreater(100);
//    graph->countNodesWithPosEdgesGreater(1000);
//    graph->countNodesWithPosEdgesGreater(10000);
//    not_naive_square(graph);
//    sort_square(graph);
//    hybrid_square(graph,15);
//    hybrid_square(graph,20);
//    compare_naive_vs_not_naive(graph);
//    batched_triangle_counting(graph); 
//    triangle_counting(graph);
//    test_array_intersection();


}