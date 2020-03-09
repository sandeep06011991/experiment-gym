//
// Created by sandeep on 9/3/20.
//

#ifndef V3_ATTREVAL_H
#define V3_ATTREVAL_H


#include <graph.h>
#include <GHDNode.h>
#include "../definitions/definitions.h"

// 4MB Channel capacity.
const int CHANNER_SIZE_IN_BYTES = 4000000;

struct TrieElement{
    NODETYPE id;
    NODETYPE nextLevelOffset;
    NODETYPE nextLevelSize;
} ;
/* could split into three classes later on. */
class AttrEval {

    struct TrieElement * inChannel;

    struct TrieElement * outChannel;

    int inChannelCapacity;

    int outChannelCapacity;

    int inChannelSize;

    int outChannelSize;

    int inEmbeddingSize;

    int outEmbeddingSize;

//  id of next unprocessed element in TrieLevel in in channel.
    int nextTrieElement;

//  no. of TrieElements in out channel
    int outTrieElementId;

    Graph *graph;

    neighbourhood_plus nbs;

    int countAccumulator = 0;

public:

    enum class TYPE{
        HEAD,
        MIDDLE,
        TAIL
    };

    TYPE type;

    AttrEval(TYPE type,neighbourhood_plus nbs, int inEmbeddingSize, Graph *graph);

    void evaluate();

    void transferResult(AttrEval *nextChannel);

    bool isEmpty();

    inline bool isHead(){return type==TYPE ::HEAD;}

    inline bool isTail(){return type==TYPE ::TAIL;}


};


#endif //V3_ATTREVAL_H
