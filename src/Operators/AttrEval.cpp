//
// Created by sandeep on 9/3/20.
//

#include "AttrEval.h"
#include<cstring>
#include "assert.h"

AttrEval::AttrEval(AttrEval::TYPE type, neighbourhood_plus nbs, int inEmbeddingSize, Graph *graph){
    this->type = type;
    this->inEmbeddingSize = inEmbeddingSize;
    this->outEmbeddingSize = inEmbeddingSize + 1;
    this->graph = graph;
    inChannelCapacity = CHANNER_SIZE_IN_BYTES/sizeof(struct TrieElement);
    outChannelCapacity = CHANNER_SIZE_IN_BYTES/sizeof(struct TrieElement);
    inChannel = (struct TrieElement *)malloc(inChannelCapacity);
    outChannel = (struct TrieElement *)malloc(outChannelCapacity);
    nextTrieElement = 0;
    outTrieElementId = 0;
    inChannelSize = 0;
}
/* Evaluation problem:*/
void AttrEval::evaluate(){
    if(TYPE::HEAD ==type) {
        while(nextTrieElement < graph->getNoVertexes() && outTrieElementId < outChannelCapacity){
            TrieElement *t = &outChannel[outTrieElementId];
            t->id = nextTrieElement;
            t->nextLevelOffset = -1;
            t->nextLevelSize = -1;
            outTrieElementId ++;
            nextTrieElement ++;
            }
        return ;
    }
    if(TYPE::TAIL == type){
        if(nbs.size==1){

        }
        assert(false);
    }
    assert(false);
//  Build dumb version of th
//  is first
}


bool AttrEval::isEmpty(){
    if(type == TYPE::HEAD){
        return nextTrieElement == graph->getNoVertexes();
    }
    return nextTrieElement == inChannelSize;
}


void AttrEval::transferResult(AttrEval *nextAttrEval){
    assert(type != TYPE::TAIL);
    memcpy(outChannel, nextAttrEval->inChannel, outEmbeddingSize * sizeof(TrieElement) * outTrieElementId);
    nextAttrEval->inChannelSize = outTrieElementId;
}

