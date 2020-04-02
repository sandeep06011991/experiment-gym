//
// Created by sandeep on 27/3/20.
//

#ifndef V3_BITMATRIX_H
#define V3_BITMATRIX_H


#include <BitDictionary.h>
#include "graph.h"
#include "GHDNode.h"

class BitMatrix {

    Graph *graph;

    NODE *ndArray;

    NODETYPE *edgeArray;

    NODETYPE anchor;

    NODE anchorNode;

    GHDNode *node;

    const int MAX_BITMATRIX_SIZE = 100000;

    const int MAX_BIT_ARRAYSIZE = 10000;

    NODETYPE * tempCandidateSets;

    NODETYPE * tempCandidateSets2;

    NODETYPE * remapping;

    bool count;

    int prevOffset = 0;

    unsigned char * defaultBitArray;

    unsigned char * bitMatrix;

    int bitArraySizeInChar;

    int noElementsInBitMatrix;

    int level;

    BitDictionary *bitDictionary;

    void insertIntoBitMatrix(NODETYPE id);

public:

    BitMatrix(Graph *graph, GHDNode *node);

    inline int expandBitArrayIntoResultVector(NODETYPE *resultArray);

    void setAnchor(NODETYPE anchor, int level);

    inline void bitArrayIntersection(char * result, char * bitArray);

    // v1 Have a naive array
    int bitIntersect(NODETYPE *nds, int noNodes, NODETYPE* resultArray);

    int naiveIntersect(NODETYPE *nds, int noNodes, NODETYPE* resultArray);

};


#endif //V3_BITMATRIX_H
