//
// Created by sandeep on 27/3/20.
//

#ifndef V3_BITMATRIXEVALUATOR_H
#define V3_BITMATRIXEVALUATOR_H

#include <GHDNode.h>
#include <graph.h>
#include <BitMatrix.h>
#include "Trie.h"

// Partial Embedding
struct PEmbedding{
    Level_Meta *meta;
    int offset;
    bool isDone;
};

// Vertex Embedding Id Tuple
struct V_E_tuple{
    NODETYPE vid;
    int pid;
};

struct V_F_tuple{
    NODETYPE vid;
    int freq;
    int start;
};

class BitMatrixEvaluator {

    static const int MAX_PEMBEDDINGS = 10000;

    static const int MAX_V_E_TUPLE = 10000;

    static const int MAX_V_F_TUPLE = 10000;

    int totalNoPartials = 0;

    int noVETuples = 0;

    int noVFTuples = 0;

    const int MAX_NEIGHBOURS = 10;

    struct PEmbedding * partials;

    struct V_E_tuple * VETuples;

    struct V_F_tuple * VFTuples;

    NODETYPE * candidateSets;

    Graph *graph;

    GHDNode *ghdNode;

    Trie *trie;

    void checkIfFitsInDatastructures( int level, int startMetaBlock, int noBlocks);

public:

    BitMatrix *b;

    BitMatrixEvaluator(Graph *graph, GHDNode *ghdNode, Trie * trie);

    int process(int level, int startMetaBlock, int noBlocks);


};


#endif //V3_BITMATRIXEVALUATOR_H
