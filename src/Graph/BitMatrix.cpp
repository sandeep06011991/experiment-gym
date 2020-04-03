//
// Created by sandeep on 27/3/20.
//

#include <cstring>
#include <intersection.h>
#include <iostream>
#include <timer.h>
#include "BitMatrix.h"
#include "cwchar"



BitMatrix::BitMatrix(Graph *graph, GHDNode * node){
    this->graph = graph;
    ndArray = graph->getNodeArray();
    edgeArray = graph->getEdgeArray();
    this->node = node;
    bitDictionary = new BitDictionary();
    tempCandidateSets = (NODETYPE *)malloc(sizeof(NODETYPE)*10000);
    tempCandidateSets2 = (NODETYPE *)malloc(sizeof(NODETYPE)*10000);

    remapping = (NODETYPE *)malloc(graph->getNoVertexes() * sizeof(NODETYPE));
    defaultBitArray = (unsigned char *)malloc(sizeof(unsigned char ) * MAX_BIT_ARRAYSIZE);
    bitMatrix = (unsigned char *)malloc(sizeof(unsigned char) * MAX_BITMATRIX_SIZE);
    memset((NODETYPE *)remapping, 0 ,graph->getNoVertexes() *sizeof(NODETYPE));
//    defaultBitArray
}

void BitMatrix::setAnchor(NODETYPE id, int level){
    this->anchor = id;
    this->level = level;
    count = level == node->getNoAttributes() -2 ;
    anchorNode = ndArray[id];
    prevOffset = prevOffset + noElementsInBitMatrix;
    noElementsInBitMatrix = 0;
    bitArraySizeInChar = (anchorNode.size_plus/8) + 1;
//    cout << anchorNode.id << " " << anchorNode.size_plus << " " << bitArraySizeInChar <<"\n";
    if(prevOffset > 10000){
        memset((NODETYPE *)remapping, 0 ,graph->getNoVertexes() *sizeof(NODETYPE));
        prevOffset = 0;
    }
    assert(anchorNode.size_plus < (MAX_BIT_ARRAYSIZE/8)+1);
}


// v1 Have a naive array
int BitMatrix::naiveIntersect(NODETYPE *nds, int noNodes, NODETYPE* resultArray){
    NODE nd1 = ndArray[nds[0]];
    int size = nd1.size_plus;
    if(size == 0) return 0;
//    cout << "intersecting" << noNodes << " ";
//    for(int i=0;i<noNodes;i++){
//        cout << nds[i] <<" ";
//    }
//    cout <<"\n";
    memcpy(tempCandidateSets, &edgeArray[nd1.offset_plus], sizeof(NODETYPE) * nd1.size_plus);

    for(int j=1;j<noNodes;j++){
//              perform intersection and get final list of candidates.
        NODE nd= ndArray[nds[j]];
        size = intersectAndReturnCandidateSets( tempCandidateSets, size,
                                                &edgeArray[nd.offset_plus], nd.size_plus, tempCandidateSets2);
        NODETYPE *t = tempCandidateSets2;
        tempCandidateSets2 = tempCandidateSets;
        tempCandidateSets = t;
    }
    memcpy(resultArray, tempCandidateSets, sizeof(NODETYPE) * size);
    return size;

}

