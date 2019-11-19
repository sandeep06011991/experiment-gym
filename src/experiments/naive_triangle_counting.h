//
// Created by sandeep on 25/10/19.
//

#include <graph.h>
#include <iostream>
#include <intersection.h>
#include <MinHeap.h>
#include "timer.h"

#ifndef V3_NAIVE_TRIANGLE_COUNTING_H
#define V3_NAIVE_TRIANGLE_COUNTING_H

bool isSkewed(NODE a, NODE b){
    int skew = 2;
    if(a.size_plus << skew < b.size_plus)return true;
    if(b.size_plus << skew < a.size_plus)return true;
    return false;
}
/* Experiment Results
 * NOTE: All experiments are performed on reordered graphs.
 * If id1 < id2 => degree(id1) < degree(id2)
 * 1. Description: Triangle counting using a naive intersection without any gallopping
 * using function naive_intersect()
 * ---------------------------
 * Graph Name | RunTime
 * ---------------------------
 * Amazon     |     .103s
 * Youtube    |     .483s
 * LiveJournal|     16s
 *
 *
 * 2. Description: Triangle Counting with gallopping using intersection function
 * binary_intersect();
 * ---------------------------
 * Graph Name | RunTime
 * ---------------------------
 * Amazon     |     .07s
 * Youtube    |     .55s
 * LiveJournal|     19.9s
 * This is an increase in all cases relative to naive intersection.
 *
 * 3. Hybrid Intersection
 * using the function
 * ---------------------------
 * Graph Name | RunTime  | Skew Tolerance
 * ---------------------------
 * Youtube    |     .46s | 3
 * LiveJournal|    14.06s| 4
 * LiveJournal|    14.079s| 4.5
 * LiveJournal|    17.01s| 5
 * Conclusion for my current test sets hybrid algorithms are useless
 *  s = s + hybrid_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
 * */

/* Triangle counting using the various intersection implementations */
int triangle_counting(Graph *graph){
    init_metrics();
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    int skip =0;
    int total = 0;
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd1 = ndArray[i];
        NODETYPE* aNeighourArray = &edgeArray[nd1.offset_plus];
        NODETYPE aSize = nd1.size_plus;
        cout << i <<"\n";
        for(int j=0;j<nd1.size_plus;j++){
            NODETYPE bNode = aNeighourArray[j];
            NODE nd2 =  ndArray[bNode];
            NODETYPE* bNeighbourArray = &edgeArray[nd2.offset_plus];
            NODETYPE bSize = nd2.size_plus;
            s = s + naive_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
//            s = s + hybrid_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
        }
    }

    stop_timer(TOTALNODEPROCESSTIME);
    print_statistics();
    cout << "Fraction of nodes skipped:" << skip *1.0 / total  << "\n";
    cout << "triangles found:" <<  s << "\n";
    return s;
}


/* Exploration of batched triangle counting */
int batched_triangle_counting(Graph *graph){
    init_metrics();
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    int batchSize = 10;
    MinHeap *heap = new MinHeap(batchSize);
    NODETYPE *bArr[batchSize];
    NODETYPE bSize[batchSize];
    int noB = 0;
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd1 = ndArray[i];
        NODETYPE* aNeighourArray = &edgeArray[nd1.offset_plus];
        NODETYPE aSize = nd1.size_plus;
        int noBatches = aSize/batchSize;
        if(noBatches>0){ noB ++; }
        for(int b = 0; b < noBatches; b++){
            for(int j=0;j< batchSize; j++){
                int curr_b= b*batchSize + j;
                NODETYPE bNode = aNeighourArray[curr_b];
                NODE nd2 =  ndArray[bNode];
                bArr[j] = &edgeArray[nd2.offset_plus];
                bSize[j] = nd2.size_plus;
            }
            s = s + batched_intersection(aNeighourArray, aSize,
                        bArr, bSize, batchSize, heap);
//        NODETYPE* arrA, NODETYPE sizeA,
//                NODETYPE** bArrys, NODETYPE *bsizes, int batchsize, MinHeap * utilityHeap
        }
        int offset = noBatches * batchSize;
        for(int j=0; j < aSize%batchSize; j++){
            int curr_b= offset + j;
            NODETYPE bNode = aNeighourArray[curr_b];
            NODE nd2 =  ndArray[bNode];
            NODETYPE *bNeighbourArray = &edgeArray[nd2.offset_plus];
            NODETYPE bSize = nd2.size_plus;
            s = s + naive_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
        }
    }

    stop_timer(TOTALNODEPROCESSTIME);
    print_statistics();
    cout << "batched triangles found:" <<  s << "\n";
    cout << "No batched nodes" << noB << "\n";
}


#endif //V3_NAIVE_TRIANGLE_COUNTING_H
