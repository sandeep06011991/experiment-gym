//
// Created by sandeep on 9/3/20.
//

#ifndef V3_PIPELINE_H
#define V3_PIPELINE_H


#include <graph.h>
#include <GHDNode.h>
#include "AttrEval.h"
#include "../Graph/GHDNode.h"
#include "Trie.h"


class Pipeline {

    Graph * graph;

    GHDNode * ghdNode;

    Trie * trie;

    NODE * ndArray;

    NODETYPE * edgeArray;

    NODETYPE * tempCandidateSets;

    NODETYPE * tempCandidateSets2;

    const int MAXTEMPCANDIDATES = 10000;

    int evaluate_level_blocks(int level, int startMetaBlock, int noBlocks);

    int recursive_evaluate(int level);

public:

    Pipeline(Graph *graph,GHDNode * ghdNode);

    int run();


};


#endif //V3_PIPELINE_H
