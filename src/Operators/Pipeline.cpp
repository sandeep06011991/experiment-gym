//
// Created by sandeep on 9/3/20.
//

#include <cstring>
#include <intersection.h>
#include <iostream>
#include <timer.h>
#include "Pipeline.h"
#include "../Graph/GHDNode.h"
#include "Trie.h"

/* Do not split the granularity of meta blocks. Might have to do this in later versions.
 * There are other alternatives such as skipping those high degree vertexes.
 * */
int Pipeline::evaluate_level_blocks(int level, int startMetaBlock, int noMetaBlocks){
    neighbourhood_plus nbs = ghdNode->getNPlusNeighbourhood(level+1);
    int s = 0;
    NODETYPE incidentNodeId[level];
    struct Level_Meta lm;
    assert(nbs.size >0);

    if(level != ghdNode->getNoAttributes()-2)trie->clearLevel(level+1);

    for(int blockid = startMetaBlock;blockid < startMetaBlock + noMetaBlocks; blockid++){
        lm = trie->levels[level]->meta_blocks[blockid];
        trie->getIncidentNbs(incidentNodeId, nbs, level, lm, 0);
        for(int i=0;i<lm.block_size;i++){
            incidentNodeId[level] = trie->levels[level]->data[lm.block_start + i];
//            trie->debugEmbedding(level, lm, i);
//            trie->getIncidentNbs(incidentNodeId, nbs, level, lm, i);
            NODE nd1 = ndArray[incidentNodeId[0]];
            assert(nd1.size_plus < MAXTEMPCANDIDATES);
            int size = nd1.size_plus;
            if(size == 0) continue;
            memcpy(tempCandidateSets, &edgeArray[nd1.offset_plus], sizeof(NODETYPE) * nd1.size_plus);
            for(int j=1;j<nbs.size;j++){
//              perform intersection and get final list of candidates.
                NODE nd= ndArray[incidentNodeId[j]];
                size = intersectAndReturnCandidateSets( tempCandidateSets, size,
                             &edgeArray[nd.offset_plus], nd.size_plus, tempCandidateSets2);

                NODETYPE *t = tempCandidateSets2;
                tempCandidateSets2 = tempCandidateSets;
                tempCandidateSets = t;
            }
            if(size == 0) continue;
            if(level == ghdNode->getNoAttributes()-2){
                s = s + size;
            }else{
                struct Level_Meta child;
                child.parent = &trie->levels[level]->meta_blocks[blockid];
                child.parent_offset = i;
                trie->appendTrieBlockToLevel(level+1, &child, tempCandidateSets, size);
            }
        }
    }
    if(level == ghdNode->getNoAttributes()-2){
        return s;
    }else{
        s = s + recursive_evaluate(level+1);
        return s;
    }

}

// Evaluate only a small portion
int Pipeline::recursive_evaluate(int levelNo){
    // split range into blocks and for each block
    TrieLevel * level = this->trie->levels[levelNo];
    int start = 0;
    int noMetaBlocks = level->getNoMetaBlocks();
    int s = 0;
    int t = 0;

    while(start < noMetaBlocks){
        int noBlocks = level->getNoContinuosMetaBlocksFromStart(start);
//        s = s + evaluate_level_blocks(levelNo, start, noBlocks);
//        FixME: Is this a triangle
        if(ghdNode->getNoIncidentAttributes(levelNo+1)<3){
            s = s + evaluate_level_blocks(levelNo, start, noBlocks);
        }else{
            t = bitMatrixEvaluator->process(levelNo, start, noBlocks);
            if(levelNo == ghdNode->getNoAttributes()-2){
                s = s + t;
            }else{
                s = s + recursive_evaluate(levelNo + 1);
                trie->clearLevel(levelNo +1);
            }
        }
//      cout << "BitMatrix evaluator\n";
        start = start + noBlocks;
    }
    return s;
}

/* Blocked v1 DFS */
int Pipeline::run(){
    Level_Meta root;
    memset(&root,0,sizeof(Level_Meta));
//  FixMe: Could this be moved outside ?
    NODETYPE * batch = (NODETYPE *)malloc(sizeof(NODETYPE) * TrieLevel::CHUNK);
    int batchSize = 0;
    int levelSize = 0;
    int s = 0;
    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
        if(batchSize == TrieLevel::CHUNK){
            trie->appendTrieBlockToLevel(0, &root ,batch, batchSize);
            levelSize = levelSize + batchSize;
            batchSize = 0;
        }
        if(levelSize >= .8 * TrieLevel::DATACAPACITY){
            s = s + recursive_evaluate(0);
            trie->clearLevel(0);
            levelSize = 0;
        }
        batch[batchSize] = ndArray[i].id;
        batchSize ++ ;
    }
    trie->appendTrieBlockToLevel(0, &root ,batch, batchSize);
    s = s + recursive_evaluate(0);
    trie->clearLevel(0);
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total GHDNode Processing time " << get_timer(TOTALNODEPROCESSTIME) <<"\n";
    cout << "Total Query Count:" << s <<"\n";
}


Pipeline::Pipeline(Graph *graph,GHDNode * ghdNode){
    this->graph = graph;
    this->ndArray = graph->getNodeArray();
    this->edgeArray = graph->getEdgeArray();
    this->ghdNode = ghdNode;
//  Assuming we do only counts.
    tempCandidateSets = (NODETYPE *)malloc(sizeof(NODETYPE) * MAXTEMPCANDIDATES);
    tempCandidateSets2 = (NODETYPE *)malloc(sizeof(NODETYPE) * MAXTEMPCANDIDATES);
    this->trie = new Trie(ghdNode->getNoAttributes()-1);
    bitMatrixEvaluator = new BitMatrixEvaluator(graph, ghdNode, trie);
}