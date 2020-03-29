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

//
//void test(){
//    int max = 100;
//    int noArrays = 100;
//    NODETYPE *arrAddr[noArrays];
//    NODETYPE arrSizes[noArrays];
//    NODETYPE arr[max*noArrays];
//    NODETYPE base[max*noArrays];
//    for(int i=0;i<noArrays;i++){
//    arrAddr[i] = &arr[max*i];
//    arrSizes[i] = max;
//    }
//    for(int i=0;i<max*noArrays;i++){
//    arr[i] = i;
//    }
//
//    start_timer(TOTALNODEPROCESSTIME);
//    computeNaiveBaseArrayWithDuplicates(arrAddr, arrSizes, noArrays,base);
////    computeBaseArrayUsingBinaryRecursion(arrAddr, arrSizes, noArrays,base);
//    stop_timer(TOTALNODEPROCESSTIME);
//    cout << "total run  time " << get_timer(TOTALNODEPROCESSTIME) <<"\n";
//}
//


void test(){

}

/* New entry point. */
int main(int argc, char *argv[]){
//    Graph *    graph = new Graph("data/data1.graph");

    Graph * graph = new Graph("data/com-youtube.ungraph.txt.ro");
//    Graph * graph = new Graph("data/com-lj.ungraph.txt.ro");
//    Graph *graph = new Graph("data/com-amazon.ungraph.txt.ro");
    GHDNode * node = getTriangleGHDNode();
//    GHDNode * node = get4Clique();
//    GHDNode * node = get5Clique();
//    GHDNode *node = getEdgeGHDNode();
//    NaiveGHDEvaluator * n = new NaiveGHDEvaluator(graph,node);
//    cout << "result" << n->evaluate() <<"\n";
    Pipeline * pipeline = new Pipeline(graph, node);
    pipeline->run();
    cout << "construction" << get_timer(BITMATRIXCONSTRUCTION)<<"\n";
    cout << "Bit Intersection Evaluation " << get_timer(BITSIMDINTERSECTIONTIME) <<"\n";
    cout << "neighbour traversal time " << get_timer(REDMATRIXCONSTRUCTION) << "\n";
}

/* Old entry point */
int main1(int argc, char *argv[]) {

//    cout << "No. of arguments recieved " << argc <<"\n";
//    Graph * graph = new Graph("data/com-amazon.ungraph.txt.ro");
//    Graph * graph = new Graph("data/com-youtube.ungraph.txt.ro");
    Graph * graph = new Graph("data/com-lj.ungraph.txt.ro");
//        Graph *    graph = new Graph("data/data1.graph");
//    Graph *graph  = new Graph("data/com-orkut.ungraph.txt.ro");
//    graph->debugGraph();
//  468774021
//    4700600
//   35877389
//    triangle_counting(graph);
//      measure_cost_of_random_reads(graph);
//    test_cost_of_read_and_write();
//    test_cost_of_page_faulting();
//    extend_intersect_version(graph);
//      graphPartition(graph);
        prefetchNdArray(graph);
//    generateGraphNothingCommon(1000,1000,"data/data1.graph");
//        graphPartition(graph);
//    tiled_triangle_counting(graph);
//    Graph * graph = new Graph("data/data1.graph");
//    std::cout << "Total no nodes :" << graph->getNoVertexes() <<"\n";
//    generateGraphSkewNothingCommon(100,10,10,1000,"data/data1.graph");
// Graph * graph = new Graph("data/data1.graph");
//  Hypothesis one impact of skew
//    naive_square_counting(graph);
//    dual_bit_matrix_based(graph);
//     bit_vector_based(graph);
//        traversal_based_sort(graph);
//  naive4cliqueCounting(graph);
//  v3cliqueCounting(graph);
//graph->debugGraph();
//  v1_4cliqueCounting(graph);
//test();
//        redundant_Intersection(graph);
//    hybrid_bit_vector_based(graph);
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


}