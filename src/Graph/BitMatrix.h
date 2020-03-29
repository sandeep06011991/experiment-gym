//
// Created by sandeep on 27/3/20.
//

#ifndef V3_BITMATRIX_H
#define V3_BITMATRIX_H


#include "graph.h"

class BitMatrix {

    Graph *graph;

    NODE *ndArray;

    NODETYPE *edgeArray;

    NODETYPE anchor;

    NODE anchorNode;

    const int MAX_BITMATRIX_SIZE = 100000;

    const int MAX_BIT_ARRAYSIZE = 10000;

    NODETYPE * tempCandidateSets;

    NODETYPE * tempCandidateSets2;

    NODETYPE * remapping;

    int prevOffset = 0;

    unsigned char * defaultBitArray;

    unsigned char * bitMatrix;

    int bitArraySizeInChar;

    int noElementsInBitMatrix;

    void insertIntoBitMatrix(NODETYPE id);

public:

    BitMatrix(Graph *graph);

    int expandBitArrayIntoResultVector(NODETYPE *resultArray);

    void setAnchor(NODETYPE anchor);

    inline void bitArrayIntersection(char * result, char * bitArray);

    // v1 Have a naive array
    int bitIntersect(NODETYPE *nds, int noNodes, NODETYPE* resultArray);

    int naiveIntersect(NODETYPE *nds, int noNodes, NODETYPE* resultArray);

};


#endif //V3_BITMATRIX_H
