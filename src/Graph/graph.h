//
// Created by sandeep on 12/8/19.
//

#ifndef V2_GRAPH_H
#define V2_GRAPH_H
#include<string>
#include <unordered_map>

#include "definitions.h"

using namespace std;



class Graph{
    NODETYPE * edges;
    NODE * nodes;
    int no_vertexes;
    int no_edges;

//    unordered_map < NODETYPE, NODETYPE>  * nodemap = new unordered_map<NODETYPE ,NODETYPE >();

public:
    Graph(string dbName);

    NODE * getNodeArray();

    NODETYPE * getEdgeArray();

    int getNoVertexes();

    NODE* getNode(NODETYPE nodeID);

    void calculatePlusNeighbourHood();

    void debugGraph();

    int countNodesWithPosEdgesGreater(int target);
};
#endif //V2_GRAPH_H
