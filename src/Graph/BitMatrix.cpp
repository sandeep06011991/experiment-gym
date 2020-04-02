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

void inline BitMatrix::insertIntoBitMatrix(NODETYPE id){
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

inline int BitMatrix::expandBitArrayIntoResultVector(NODETYPE *resultArray){
    int size = 0;
    assert(anchorNode.size_plus<10000);
    for(int i=0; i< (anchorNode.size_plus/8)+1; i++){
        if(defaultBitArray[i] == 0)continue;
        for(int j=7;j>=0;j--){
            if(i*8+7-j > anchorNode.size_plus)continue;
            if(defaultBitArray[i] & 1U << (j)){
                resultArray[size] = edgeArray[anchorNode.offset_plus + i*8+7-j];
                size ++;
            }
        }
    }
//    for(int i=anchorNode.size_plus/8; i< (anchorNode.size_plus/8)-1; i++){
//        if(defaultBitArray[i] == 0)continue;
//        for(int j=7;j>=0;j--){
//            if(defaultBitArray[i] & 1U << (j)){
//                resultArray[size] = edgeArray[anchorNode.offset_plus + i];
//                size ++;
//            }
//        }
//    }
//    int i=0;
//    while(i<anchorNode.size_plus){
//        if(i%8==0 && defaultBitArray[i/8]==0){
//            i=i+8;
//            continue;
//        }
//        int pos = i/8;
//        int offset = 7-(i%8);
////        bitV[bp/8] = bitV[bp/8] | 1U << (7-(bp%8));
//        if(defaultBitArray[pos] & 1U << (offset)){
//            resultArray[size] = edgeArray[anchorNode.offset_plus + i];
//            size ++;
//        }
//        i++;
//    }
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
//            start_timer(SIMDINTERSECTION);
            insertIntoBitMatrix(nd.id);
//            stop_timer(SIMDINTERSECTION);
        }
        int offset = remapping[nd.id]-prevOffset;
//      offload this line to another entity.
        unsigned char * bitArray = &bitMatrix[offset*bitArraySizeInChar];
        unsigned int * a = (unsigned int *)defaultBitArray;
        unsigned int * b = (unsigned int *)bitArray;
        char zero = 0x00;
        int zeroI = 0;
//        if(bitArraySizeInChar > 64){
//            cout << bitArraySizeInChar << " ";
//        }
        for(int i=0;i < 4 && i < bitArraySizeInChar;i++){
            defaultBitArray[i] = defaultBitArray[i] & bitArray[i];
            zero = zero | bitArray[i];
        }
        for(int i=1;i<bitArraySizeInChar/4;i++){
            a[i] = a[i] & b[i];
            zeroI = zeroI | b[i];
        }
        for(int i=(bitArraySizeInChar/4)*4; i < bitArraySizeInChar;i++){
            defaultBitArray[i] = defaultBitArray[i] & bitArray[i];
            zero = zero | bitArray[i];
        }
//        for(int i=0;i < bitArraySizeInChar;i++){
//            defaultBitArray[i] = defaultBitArray[i] & bitArray[i];
//            zero = zero | defaultBitArray[i];
//        }
        if(!zero && !zeroI){
            return  0;
        }
    }
//    start_timer(ADGLISTINTERSECTION);
    int s = 0;
    if(count){
        for(int i=0;i<bitArraySizeInChar;i++){
           s = s + bitDictionary->dic[(int)defaultBitArray[i]];
        }
        return s;
    }

//    int s = expandBitArrayIntoResultVector(resultArray);
//    stop_timer(ADGLISTINTERSECTION);
//    cout <<"aa" <<  anchorNode.id << " " << nds[1] <<" " <<  s <<"\n";

    return s;
}