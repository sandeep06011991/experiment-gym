//
// Created by sandeep on 24/2/20.
//

#ifndef V3_TILE_H
#define V3_TILE_H

#include <definitions.h>
#include <unordered_set>
using namespace std;

/* A tile which fits into memory
 * Capacity l1_cache_capacity = 64 * 1024;*/

const int TILE_CACHE_CAPACITY = 32*1024;

const  int taskBufferCapacity = 9000000;

struct task {
    NODETYPE id1;
    NODETYPE id2;
    bool isDone;
};

task taskbuffer[taskBufferCapacity];

struct freq_tuple{
    NODETYPE id;
    NODETYPE f;
//  offset of associated instructions
};

struct vertex_task_offset{
    NODETYPE  id;
    NODETYPE offset;
};

struct vertex_task{
    NODETYPE vid;
    NODETYPE task_id;
};

vertex_task vertex_task_list[taskBufferCapacity*2];

const int MAX_TASKS_PERTILE = 20000;

const int MAX_VERTEXES = 2000;
class Tile {
//  No. of vertexes that are a part of this tile.
//  The adjacency lists of all this vertexes should fit in memory.
    int noVertexes;

    unordered_set<NODETYPE> *vertexes =  new unordered_set<NODETYPE>();

    int tileCapacity = 0;

    int cPartialEmbeddings = 0;
    int noVertices = 0;

    NODETYPE partialEmbeddings[MAX_TASKS_PERTILE];

public:

    void addTaskToTile(NODE nd1, NODE nd2, NODETYPE taskId){
        partialEmbeddings[cPartialEmbeddings] = taskId;
        cPartialEmbeddings ++ ;
        if(cPartialEmbeddings >= MAX_TASKS_PERTILE){
            cout << cPartialEmbeddings <<" \n";
        }
        assert(cPartialEmbeddings < MAX_TASKS_PERTILE);
        if(vertexes->find(nd1.id)==vertexes->end()){
            tileCapacity = tileCapacity + nd1.size_plus;
            vertexes->insert(nd1.id);
            noVertexes ++;
//            cout << "inserting \n";
        }
        if(vertexes->find(nd2.id)==vertexes->end()){
            tileCapacity = tileCapacity + nd2.size_plus;
            vertexes->insert(nd2.id);
            noVertexes ++;
//            cout << "inserting \n";
        }
        assert(noVertexes <MAX_VERTEXES);
    }

    bool doesTileHaveSpace(int cost){
        if(noVertexes > MAX_VERTEXES-5)return false;
        if(cPartialEmbeddings < MAX_TASKS_PERTILE-1)return (cost + tileCapacity < TILE_CACHE_CAPACITY);
        return false;
    }

    int  costOfAdditionToTile(NODE nd1, NODE nd2){
        int cost = 0;
        if(vertexes->find(nd1.id)==vertexes->end()){
            cost  = cost + nd1.size_plus;
        }
        if(vertexes->find(nd2.id)==vertexes->end()){
            cost  = cost + nd2.size_plus;
        }
        return cost;
    }

    int process(NODE *ndArray, NODETYPE *edgeArray){
        int s = 0;
        for(int i=0;i<cPartialEmbeddings;i++){
            task t = taskbuffer[partialEmbeddings[i]];
            NODE nd1 = ndArray[t.id1];
            NODE nd2 = ndArray[t.id2];
            s = s + naive_intersect(&edgeArray[nd1.offset_plus], nd1.size_plus, &edgeArray[nd2.offset_plus], nd2.size_plus);

        }
        return s;
    }

    void statistics(){
        cout << "Num Embeddings:" << cPartialEmbeddings << "Tile Capacity: " << tileCapacity << "N(Vertex): "<<vertexes->size() <<"\n";
    }
};

const int MAXTILENO = 10000;

Tile tiles[MAXTILENO];

#endif //V3_TILE_H
