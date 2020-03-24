//
// Created by sandeep on 24/3/20.
//

#include <cstring>
#include <intersection.h>
#include <iostream>
#include <timer.h>
#include "NaiveGHDEvaluator.h"

NaiveGHDEvaluator::NaiveGHDEvaluator(Graph *graph, GHDNode *ghd){
    this->graph = graph;
    this->ghdNode = ghd;
    ndArray = graph->getNodeArray();
    edgeArray = graph->getEdgeArray();

    candidateSets = (NODETYPE **)malloc(sizeof(NODETYPE *) * ghdNode->getNoAttributes());
    dummySets = (NODETYPE **)malloc(sizeof(NODETYPE *) * ghdNode->getNoAttributes());

    for(int i=0;i<ghdNode->getNoAttributes();i++){
        candidateSets[i] = (NODETYPE *)malloc(sizeof(NODETYPE) * MAX_CANDIDATE_SET_SIZE);
        dummySets[i] = (NODETYPE *)malloc(sizeof(NODETYPE) * MAX_CANDIDATE_SET_SIZE);
    }
    currentEmbedding = (NODETYPE *)malloc(sizeof(NODETYPE) * ghdNode->getNoAttributes());
    candidateSetSizes = (NODETYPE *)malloc(sizeof(NODETYPE) * ghdNode->getNoAttributes());

}

int NaiveGHDEvaluator::evaluate(){
    int s = 0;
    NODE * ndArray = graph->getNodeArray();
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
        currentEmbedding[0] = ndArray[i].id;
        s = s + recursiveEvaluate(1);
    }
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total NAIVE processing time = " << get_timer(TOTALNODEPROCESSTIME) << "\n";
    return s;
}

// Uses a recursive version of generic join.
int NaiveGHDEvaluator::recursiveEvaluate(int currEmbeddingSize) {
//   if currSize = 1;
//    int newSize = currEmbeddingSize + 1;
    neighbourhood_plus nb = ghdNode->getNPlusNeighbourhood(currEmbeddingSize);
    assert(nb.size > 0);
//  Must be a connected graph
    NODE nd1= ndArray[currentEmbedding[nb.attrs[0]]];
    assert(nd1.size_plus < MAX_CANDIDATE_SET_SIZE);

    memcpy(candidateSets[currEmbeddingSize], &edgeArray[nd1.offset_plus], sizeof(NODETYPE) * nd1.size_plus);
    candidateSetSizes[currEmbeddingSize] = nd1.size_plus;
    for(int i=1;i<nb.size;i++){
        NODE nd= ndArray[currentEmbedding[nb.attrs[i]]];
        candidateSetSizes[currEmbeddingSize] = intersectAndReturnCandidateSets
                (candidateSets[currEmbeddingSize],candidateSetSizes[currEmbeddingSize] ,
                        &edgeArray[nd.offset_plus], nd.size_plus, dummySets[currEmbeddingSize]);

        NODETYPE *t = candidateSets[currEmbeddingSize];
        candidateSets[currEmbeddingSize] = dummySets[currEmbeddingSize];
        dummySets[currEmbeddingSize] = t;

    }
    if(candidateSetSizes[currEmbeddingSize]==0)return 0;
    int s = 0;
    if(currEmbeddingSize == ghdNode->getNoAttributes()-1){
//        for(int a=0;a<currEmbeddingSize;a++){
//            cout << currentEmbedding[a] <<" " ;
//        }
//        cout << candidateSetSizes[currEmbeddingSize] << "\n";
//        assert(false);
        s = s + candidateSetSizes[currEmbeddingSize];
        return s;
    }

    for(int i=0;i<candidateSetSizes[currEmbeddingSize];i++){
        currentEmbedding[currEmbeddingSize] = candidateSets[currEmbeddingSize][i];
        s = s + recursiveEvaluate(currEmbeddingSize + 1);
    }
    return s;
}