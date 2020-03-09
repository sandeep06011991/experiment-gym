//
// Created by sandeep on 9/3/20.
//

#ifndef V3_PIPELINE_H
#define V3_PIPELINE_H


#include <graph.h>
#include <GHDNode.h>
#include "AttrEval.h"
#include "../Graph/GHDNode.h"


class Pipeline {

    AttrEval * pipe;

    Graph * graph;

    GHDNode * ghdNode;


    void initialize();

public:

    Pipeline(Graph *graph,GHDNode * ghdNode);

    int run();


};


#endif //V3_PIPELINE_H
