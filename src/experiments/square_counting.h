//
// Created by sandeep on 1/11/19.
//

#include <graph.h>
#include <iostream>
#include <intersection.h>
#include <MinHeap.h>
#include <ArrayUtilities.h>
#include "timer.h"

#ifndef V3_SQUARE_COUNTING_H
#define V3_SQUARE_COUNTING_H

#endif //V3_SQUARE_COUNTING_H
/*
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
        for(int j=0; j<nd1.size_plus ;j++){
            NODETYPE bNode = aNeighourArray[j];
            NODE nd2 =  ndArray[bNode];
            NODETYPE* bNeighbourArray = &edgeArray[nd2.offset];
//          Neighbours of nd2 && greater than nd1
            unsigned int nd2_plus_offset = binarySearchFirstElementGreaterTarget
            (bNeighbourArray,0, nd2.size, nd1.id);
            assert(nd2_plus_offset <= nd2.size);
//            bNeighbourArray = &edgeArray[nd2.offset + nd2_plus_offset];
            NODETYPE bSize = nd2.size - nd2_plus_offset;
            for(int k=j+1; k<nd1.size_plus; k++){
                NODETYPE cNode = aNeighourArray[k];
                NODE nd3 =  ndArray[cNode];
                NODETYPE* cNeighbourArray = &edgeArray[nd3.offset];
                //          Neighbours of nd3 && greater than nd1
                unsigned int nd3_plus_offset = binarySearchFirstElementGreaterTarget(
                        cNeighbourArray,0,nd3.size,nd1.id);
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