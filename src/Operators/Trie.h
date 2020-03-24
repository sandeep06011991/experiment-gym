//
// Created by sandeep on 24/3/20.
//

#ifndef V3_TRIE_H
#define V3_TRIE_H

#include <definitions.h>

struct Level_Meta{
    struct Level_Meta * parent;
    int parent_offset;
    int block_size;
    int block_start;
};


class TrieLevel{

    int level;
    int noMetaBlocks;
    int noDataBlocks;

public:
    NODETYPE * data;


    struct Level_Meta * meta_blocks;

    TrieLevel(int level);

    static const int DATACAPACITY = 1000000;
    static const int METACAPACITY = 1000000;
    static const int CHUNK = DATACAPACITY / 1000;

    void appendTrieBlockToLevel(struct Level_Meta meta ,NODETYPE * data, int size);

    void clear();

    inline NODETYPE getElement(int start, int offset);

    inline int getNoMetaBlocks(){
        return noMetaBlocks;
    }

//  getNo continuos meta blocks starting from s having total payload less than chunk
    int getNoContinuosMetaBlocksFromStart(int start);

    inline struct Level_Meta getLevelMeta(int blockId);

};

class Trie {

    int noLevels;

public:

    TrieLevel ** levels;

    Trie(int noLevels);

    void loadGraph();

    void debugBFS();

    void appendTrieBlockToLevel(int level, struct Level_Meta * meta ,NODETYPE * data, int size);

    void getIncidentNbs(NODETYPE *nbs, neighbourhood_plus nb_plus, int level, Level_Meta lm, int offset);

    void clearLevel(int level);

    void debugEmbedding(int level, Level_Meta lm, int offset);

};

#endif //V3_TRIE_H
