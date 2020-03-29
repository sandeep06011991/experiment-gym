//
// Created by sandeep on 27/3/20.
//

#include <iostream>
#include "BitMatrixEvaluator.h"
#include "timer.h"
BitMatrixEvaluator::BitMatrixEvaluator(Graph *graph,GHDNode *ghdNode, Trie * trie){
    b = new BitMatrix(graph);
    this->ghdNode = ghdNode;
    this->trie = trie;
    partials = (struct PEmbedding *)malloc(MAX_PEMBEDDINGS * sizeof(struct PEmbedding));
    VETuples = (struct V_E_tuple *)malloc(MAX_V_E_TUPLE * sizeof(struct V_E_tuple));
    VFTuples = (struct V_F_tuple *)malloc(MAX_V_F_TUPLE * sizeof(struct V_F_tuple));
    candidateSets = (NODETYPE *)malloc(sizeof(NODETYPE)*10000);
}

void BitMatrixEvaluator::checkIfFitsInDatastructures( int levelNo, int startMetaBlock, int noBlocks) {
    TrieLevel * level = trie->levels[levelNo];
    int countEmbeddings = 0;
    for(int bid = 0;bid < noBlocks;bid ++ ){
        countEmbeddings = countEmbeddings + level->meta_blocks[startMetaBlock + bid].block_size;
    }
    int vertexCount = countEmbeddings * ghdNode->getNoIncidentAttributes(levelNo+1);
    assert(vertexCount < MAX_V_F_TUPLE);
    assert(vertexCount < MAX_V_E_TUPLE);
    assert(countEmbeddings < MAX_PEMBEDDINGS);
    assert( ghdNode->getNoIncidentAttributes(levelNo+1) < MAX_NEIGHBOURS );
}

int BitMatrixEvaluator::process( int level, int startMetaBlock, int noBlocks){
    checkIfFitsInDatastructures(level, startMetaBlock, noBlocks);
    // populate key datastructures.
    NODETYPE nbs[MAX_NEIGHBOURS];
    totalNoPartials = 0;
    noVETuples = 0;
    noVFTuples = 0;
    int s = 0;
    start_timer(BITMATRIXCONSTRUCTION);
    int nbSize = ghdNode->getNoIncidentAttributes(level+1);
    for(int bid=startMetaBlock;bid < startMetaBlock + noBlocks; bid ++){
        Level_Meta *lm =  &trie->levels[level]->meta_blocks[bid];
        for(int offset = 0;offset < lm->block_size; offset ++){
//            trie->debugEmbedding(level,*lm, offset);
            assert(lm!= nullptr);
            partials[totalNoPartials].meta = lm;
            partials[totalNoPartials].offset = offset;
            partials[totalNoPartials].isDone = false;
            trie->getIncidentNbs(nbs, ghdNode->getNPlusNeighbourhood(level+1), level, *lm, offset);
            for(int i=0;i<nbSize;i++){
                VETuples[noVETuples].vid = nbs[i];
                VETuples[noVETuples].pid = totalNoPartials;
                noVETuples ++;
            }
            totalNoPartials ++;
        }
    }

    sort(VETuples, VETuples + noVETuples, [&](struct V_E_tuple a, struct V_E_tuple b){
        return a.vid < b.vid;
    });

    VFTuples[0].vid = VETuples[0].vid;
    VFTuples[0].start = 0;
    VFTuples[0].freq = 1;
    noVFTuples ++;
    for(int i=1;i < noVETuples;i++){
        if(VFTuples[noVFTuples-1].vid == VETuples[i].vid){
            VFTuples[noVFTuples-1].freq ++;
        }else{
            VFTuples[noVFTuples].vid = VETuples[i].vid;
            VFTuples[noVFTuples].start = i;
            VFTuples[noVFTuples].freq = 1;
            noVFTuples ++;
        }
    }

    sort(VFTuples,VFTuples + noVFTuples, [&](struct V_F_tuple a, struct V_F_tuple b){
        return a.freq > b.freq;
    });
//    cout << noVFTuples << " ";
//    std::cout << "Unique vertex/ total tasks (should be as low as possible):" << (noVFTuples*1.0)/noVETuples <<"\n";
//  Process tuples using bitMatrix.
    int size;
    stop_timer(BITMATRIXCONSTRUCTION);

    for(int i=0; i < noVFTuples;i++){
        start_timer(BITSIMDINTERSECTIONTIME);
        b->setAnchor(VFTuples[i].vid);
        stop_timer(BITSIMDINTERSECTIONTIME);
        for(int j=0; j < VFTuples[i].freq;j++){
            PEmbedding* pe = &partials[VETuples[VFTuples[i].start + j].pid];
            assert(pe->meta != nullptr);
            if(pe->isDone)continue;
            pe->isDone = true;
//            trie->debugEmbedding(level,*pe->meta, pe->offset);
//            trie->debugEmbedding(level,*pe->meta,pe->offset);
            trie->getIncidentNbs(nbs, ghdNode->getNPlusNeighbourhood(level+1), level, *pe->meta , pe->offset);
            size = b->bitIntersect(nbs,ghdNode->getNoIncidentAttributes(level+1), candidateSets);
//            cout << size <<"\n";
//            cout << "size" << size << "\n";
            if(level == ghdNode->getNoAttributes()-2){
                s = s + size;
            }else{
                s = s + size;
                struct Level_Meta lm;
                lm.parent = pe->meta;
                assert(lm.parent != nullptr);
                lm.parent_offset = pe->offset;
//                cout <<"inserting " << lm.parent <<"\n";
                trie->appendTrieBlockToLevel(level+1, &lm, candidateSets, size);
            }
        }

    }
    return  s;
}

