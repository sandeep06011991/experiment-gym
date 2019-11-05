//
// Created by sandeep on 1/11/19.
//

#include <graph.h>
#include <iostream>
#include <intersection.h>
#include <MinHeap.h>
#include <ArrayUtilities.h>
#include <cstring>
#include "timer.h"

#ifndef V3_SQUARE_COUNTING_H
#define V3_SQUARE_COUNTING_H

#endif //V3_SQUARE_COUNTING_H
/*  Find all squares in a pattern graph
 * */
int naive_square_counting(Graph *graph){
    init_metrics();
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd1 = ndArray[i];
        NODETYPE* aNeighourArray = &edgeArray[nd1.offset_plus];
        NODETYPE aSize = nd1.size_plus;
        NODETYPE offset_plus[nd1.size_plus];
        for(int ii=0;ii<nd1.size_plus;ii++){
            NODETYPE bNode = aNeighourArray[ii];
            NODE nd2 =  ndArray[bNode];
            NODETYPE* bNeighbourArray = &edgeArray[nd2.offset];
//          Neighbours of nd2 && greater than nd1
            unsigned int nd2_plus_offset = binarySearchFirstElementGreaterTarget
                    (bNeighbourArray,0, nd2.size, nd1.id);
            offset_plus[ii] = nd2_plus_offset;
        }
        for(int j=0; j<nd1.size_plus ;j++){
            NODETYPE bNode = aNeighourArray[j];
            NODE nd2 =  ndArray[bNode];
            NODETYPE* bNeighbourArray = &edgeArray[nd2.offset];
//          Neighbours of nd2 && greater than nd1
            unsigned int nd2_plus_offset = offset_plus[j];
//            bNeighbourArray = &edgeArray[nd2.offset + nd2_plus_offset];
            NODETYPE bSize = nd2.size - nd2_plus_offset;
            for(int k=j+1; k<nd1.size_plus; k++){
                NODETYPE cNode = aNeighourArray[k];
                NODE nd3 =  ndArray[cNode];
                NODETYPE* cNeighbourArray = &edgeArray[nd3.offset];
                //          Neighbours of nd3 && greater than nd1
                unsigned int nd3_plus_offset = offset_plus[k];
//                cNeighbourArray = &edgeArray[nd3.offset + nd3_plus_offset];
                assert(nd3_plus_offset <= nd3.size);
                NODETYPE cSize = nd3.size - nd3_plus_offset;

                s = s + naive_intersect
                        ( &cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);
            }
//            s = s + hybrid_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
        }
    }

    stop_timer(TOTALNODEPROCESSTIME);
    print_statistics();
    cout << "square found:" <<  s << "\n";
    return s;
}


int not_naive_square(Graph *graph){
    init_metrics();
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){

        NODE nd1 = ndArray[i];
        NODETYPE* nd1NeighourArray = &edgeArray[nd1.offset_plus];
        NODE nd1NeighbourNodes[nd1.size_plus];

//      Heap support structures
        HeapElement array[nd1.size_plus];
        MinHeap heap(array,nd1.size_plus);
        int curr_p[nd1.size_plus];

//        Heap creation
        for(int ii=0;ii<nd1.size_plus;ii++){
            nd1NeighbourNodes[ii] = ndArray[nd1NeighourArray[ii]];
//          offset of neighbours of neiigbours of nd1 greater than nd1
            unsigned int offset = binarySearchFirstElementGreaterTarget
                    (&edgeArray[nd1NeighbourNodes[ii].offset],0, nd1NeighbourNodes[ii].size, nd1.id);
            curr_p[ii] = offset;
            if(offset!=nd1NeighbourNodes[ii].size){
                NODETYPE addNode = edgeArray[nd1NeighbourNodes[ii].offset + offset];
                heap.addElement(addNode, ii);
                curr_p[ii] ++;
            }
        }

//        Heap Unionify

        NODETYPE  lastSeen;

        if(heap.currentSize>0){
            HeapElement prev;
            heap.popElement(&prev);
            lastSeen = prev.value;
            if(curr_p[prev.arrayId] != nd1NeighbourNodes[prev.arrayId].size){
                heap.addElement(edgeArray[nd1NeighbourNodes[prev.arrayId].offset + curr_p[prev.arrayId]], prev.arrayId);
                curr_p[prev.arrayId] ++;
            }
        }
        int g_count = 0;
        int l_count = 1;
        while(heap.currentSize > 0){
            HeapElement prev;
            heap.popElement(&prev);
            if(curr_p[prev.arrayId] != nd1NeighbourNodes[prev.arrayId].size){
                heap.addElement(edgeArray[nd1NeighbourNodes[prev.arrayId].offset + curr_p[prev.arrayId]], prev.arrayId);
                curr_p[prev.arrayId] ++;
            }

            if(prev.value == lastSeen)l_count++;

            if(prev.value != lastSeen){
                if(l_count >= 2){
                    g_count = g_count + ((l_count*(l_count-1))>>1);
                }
                l_count = 1;
                lastSeen = prev.value;
            }
        }
        if(l_count >= 2){
            g_count = g_count + ((l_count*(l_count-1))>>1);
        }

        s = s + g_count;
    }

    stop_timer(TOTALNODEPROCESSTIME);
    print_statistics();
    cout << "square found:" <<  s << "\n";
    return s;

}