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

    int getNoEdges(){return no_edges;}

    NODE* getNode(NODETYPE nodeID);

    void calculatePlusNeighbourHood();

    void debugGraph();

    int countNodesWithPosEdgesGreater(int target);
};

void generateGraphAllCommon(int a_n, int d_n, string outfile);


void generateGraphNothingCommon(int a_n, int d_n, string outfile);

void generateGraphSkewNothingCommon(int a_small, int d_small,
                                    int a_large, int d_large, string outfile);

#endif //V2_GRAPH_H
