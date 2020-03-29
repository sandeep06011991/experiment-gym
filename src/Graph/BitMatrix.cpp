//
// Created by sandeep on 27/3/20.
//

#include <cstring>
#include <intersection.h>
#include <iostream>
#include "BitMatrix.h"
#include "cwchar"



BitMatrix::BitMatrix(Graph *graph){
    this->graph = graph;
    ndArray = graph->getNodeArray();
    edgeArray = graph->getEdgeArray();

    tempCandidateSets = (NODETYPE *)malloc(sizeof(NODETYPE)*10000);
    tempCandidateSets2 = (NODETYPE *)malloc(sizeof(NODETYPE)*10000);

    remapping = (NODETYPE *)malloc(graph->getNoVertexes() * sizeof(NODETYPE));
    defaultBitArray = (unsigned char *)malloc(sizeof(unsigned char ) * MAX_BIT_ARRAYSIZE);
    bitMatrix = (unsigned char *)malloc(sizeof(unsigned char) * MAX_BITMATRIX_SIZE);
    memset((NODETYPE *)remapping, 0 ,graph->getNoVertexes() *sizeof(NODETYPE));
//    defaultBitArray
}

void BitMatrix::setAnchor(NODETYPE id){
    this->anchor = id;
    anchorNode = ndArray[id];
    prevOffset = prevOffset + noElementsInBitMatrix;
    noElementsInBitMatrix = 0;
    bitArraySizeInChar = (anchorNode.size_plus/8) + 1;
    if(prevOffset > 10000){
        memset((NODETYPE *)remapping, 0 ,graph->getNoVertexes() *sizeof(NODETYPE));
        prevOffset = 0;
    }
    assert(anchorNode.size_plus < (MAX_BIT_ARRAYSIZE/8)+1);
}

void BitMatrix::insertIntoBitMatrix(NODETYPE id){
    assert(remapping[id] <= prevOffset);
    if(remapping[id]<=prevOffset){
        noElementsInBitMatrix ++;
        assert(noElementsInBitMatrix * bitArraySizeInChar < MAX_BITMATRIX_SIZE);
        NODE nd = ndArray[id];
        remapping[id] = prevOffset + noElementsInBitMatrix;
        int pos = remapping[id] - prevOffset;
        unsigned  char * bitMatrixArray = &bitMatrix[bitArraySizeInChar * pos];
        memset(bitMatrixArray, 0, sizeof(unsigned char) * bitArraySizeInChar);
        naivebitMark_intersect(&edgeArray[anchorNode.offset_plus],
                anchorNode.size_plus, &edgeArray[nd.offset_plus], nd.size_plus,
                               bitMatrixArray);
    }
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

int BitMatrix::expandBitArrayIntoResultVector(NODETYPE *resultArray){
    int i=0;
    int size = 0;
    while(i<anchorNode.size_plus){
        int pos = i/8;
        int offset = 7-(i%8);
//        bitV[bp/8] = bitV[bp/8] | 1U << (7-(bp%8));
        if(defaultBitArray[pos] & 1U << (offset)){
            resultArray[size] = edgeArray[anchorNode.offset_plus + i];
            size ++;
        }
        i++;
    }
    return size;
}

int BitMatrix::bitIntersect(NODETYPE *nds, int noNodes, NODETYPE *resultArray) {
    NODE nd1 = ndArray[nds[0]];
    int size = nd1.size_plus;
    if(size == 0) return 0;
    assert(noNodes>1);
    memset(defaultBitArray, 0xFF, bitArraySizeInChar);
    for(int j=0;j<noNodes;j++){
//      perform intersection and get final list of candidates.
        NODE nd= ndArray[nds[j]];
        if(nd.id == anchor)continue;
        if(remapping[nd.id] <= prevOffset){
            insertIntoBitMatrix(nd.id);
        }
        int offset = remapping[nd.id]-prevOffset;
//      offload this line to another entity.
        unsigned char * bitArray = &bitMatrix[offset*bitArraySizeInChar];
        for(int i=0; i < bitArraySizeInChar;i++){
            defaultBitArray[i] = defaultBitArray[i] & bitArray[i];
        }
    }
    int s = expandBitArrayIntoResultVector(resultArray);
//    cout <<"aa" <<  anchorNode.id << " " << nds[1] <<" " <<  s <<"\n";
    return s;
}