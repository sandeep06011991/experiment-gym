//
// Created by sandeep on 24/3/20.
//

#include <cstdlib>
#include <assert.h>
#include <cstring>
#include <GHDNode.h>
#include <iostream>
#include "Trie.h"

int TrieLevel::getNoContinuosMetaBlocksFromStart(int start){
    int totalDataBlocks = 0;
    int totalMetaBlocks = 0;
//  atleast one block must be added
    assert(meta_blocks[start].block_size <= CHUNK);
    while(start<noMetaBlocks && totalDataBlocks <CHUNK){
        totalDataBlocks = totalDataBlocks + meta_blocks[start].block_size;
        start++;
        totalMetaBlocks++;
    }
    return totalMetaBlocks;
}

inline struct Level_Meta TrieLevel::getLevelMeta(int blockId){
    assert(blockId < noMetaBlocks);
    return meta_blocks[blockId];
}

TrieLevel::TrieLevel(int level){
    meta_blocks = (struct Level_Meta *)malloc(sizeof(struct Level_Meta) * METACAPACITY);
    data = (NODETYPE *) malloc(sizeof(NODETYPE) * DATACAPACITY);
    this->level = level;
    noMetaBlocks = 0;
    noDataBlocks = 0;
}

void TrieLevel::clear(){
    noMetaBlocks = 0;
    noDataBlocks = 0;
}

inline NODETYPE TrieLevel::getElement(int start, int offset){
    return this->data[start+offset];
}

void TrieLevel::appendTrieBlockToLevel(struct Level_Meta meta, NODETYPE * data, int size){
    assert(noMetaBlocks < METACAPACITY);
    assert(noDataBlocks + size < DATACAPACITY);
    meta.block_start = noDataBlocks;
    meta.block_size = size;
    meta_blocks[noMetaBlocks] =meta;
    noMetaBlocks ++ ;
    memcpy(&this->data[noDataBlocks], data, sizeof(NODETYPE) * size);
    noDataBlocks = noDataBlocks + size;
}

Trie::Trie(int noLevels){
   levels = (TrieLevel **)malloc(sizeof(TrieLevel *) * noLevels);
   this->noLevels = noLevels;
   for(int i=0; i<noLevels; i++){
       levels[i] = new TrieLevel(i);
   }
}

void Trie::clearLevel(int level){
    assert(level < noLevels);
    this->levels[level]->clear();
}

void Trie::appendTrieBlockToLevel(int level, struct Level_Meta *meta,NODETYPE * data, int size){
    this->levels[level]->appendTrieBlockToLevel(*meta, data, size);
}

/* Think of a better position or break into smaller more precise functions.
 * Too many low level details of child class are being passed. */
void Trie::getIncidentNbs(NODETYPE *nd, neighbourhood_plus nb_plus, int level, Level_Meta lm1, int offset){
    int clevel = level;
    int* nbs_ids = nb_plus.attrs;
    int nbIndex = nb_plus.size-1;
    Level_Meta *lm = &lm1;
    while((clevel >= 0) && (nbIndex >=0)){
        if(clevel == nbs_ids[nbIndex]){
            nd[nbIndex] = this->levels[clevel]->getElement(lm->block_start,offset);
            nbIndex--;
        }
        offset = lm->parent_offset;
        lm = lm->parent;
        clevel --;
    }
}

void Trie::debugEmbedding(int level, Level_Meta lm, int offset){
    NODETYPE embedding[level+1];
    int max = level+1;
    while(level >=0){
        embedding[level] = levels[level]->getElement(lm.block_start,offset);
        level --;
        if(level == -1)break;
        offset = lm.parent_offset;
        lm = *lm.parent;
        }
        std::cout <<"E:";
        for(int i=0;i<max;i++){
        std::cout << embedding[i] << " ";
    }
    std::cout <<"\n";
}
