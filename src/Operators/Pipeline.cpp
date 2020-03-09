//
// Created by sandeep on 9/3/20.
//

#include "Pipeline.h"
#include "../Graph/GHDNode.h"

void Pipeline::initialize(){
    int noAttr = ghdNode->getNoAttributes();
    assert(noAttr > 2);
    AttrEval ** pipe = (AttrEval **)malloc(sizeof(AttrEval *) * noAttr);
    pipe[0] = new AttrEval(AttrEval::TYPE::HEAD, ghdNode->getNPlusNeighbourhood(0), -1, graph);
    for(int i=1;i<noAttr-1; i++){
        pipe[i] = new AttrEval(AttrEval::TYPE::MIDDLE, ghdNode->getNPlusNeighbourhood(i), i ,graph);
    }
    pipe[noAttr-1] = new AttrEval(AttrEval::TYPE::TAIL, ghdNode->getNPlusNeighbourhood(noAttr-1),
                 noAttr-2, graph);
}

/* Blocked v1 DFS */
int Pipeline::run(){
//    pipeSize = gh
    int cAttrEval = 0;
    int acc = 0;
    initialize();
    while(!pipe[cAttrEval].isHead() || pipe[cAttrEval].isEmpty()){
        AttrEval * curr = &pipe[cAttrEval];
        if(curr->isEmpty()){
            cAttrEval --;
            continue;
        }
        curr->evaluate();
        if(curr->isTail()){
            cAttrEval --;
        }else{
            curr->transferResult(&pipe[cAttrEval+1]);
            cAttrEval ++;
        }
    }
    return acc;
}

Pipeline::Pipeline(Graph *graph,GHDNode * ghdNode){
    this->graph = graph;
    this->ghdNode = ghdNode;
}