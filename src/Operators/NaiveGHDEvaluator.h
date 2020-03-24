//
// Created by sandeep on 24/3/20.
//

#ifndef V3_NAIVEGHDEVALUATOR_H
#define V3_NAIVEGHDEVALUATOR_H


#include <GHDNode.h>
#include <graph.h>

class NaiveGHDEvaluator {

    Graph *graph;
    GHDNode *ghdNode;
    NODE * ndArray;
    NODETYPE *edgeArray;

    NODETYPE ** candidateSets;
//  Used for temporary datastructures
    NODETYPE ** dummySets;
    NODETYPE * candidateSetSizes;

    NODETYPE * currentEmbedding;

    const int MAX_CANDIDATE_SET_SIZE = 10000;


    int recursiveEvaluate(int currEmbeddingSize);

public:
    NaiveGHDEvaluator(Graph *graph, GHDNode *ghd);

    int evaluate();
};


#endif //V3_NAIVEGHDEVALUATOR_H
