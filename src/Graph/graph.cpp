//
// Created by sandeep on 15/8/19.
//

#include <fstream>
#include <assert.h>
#include "ArrayUtilities.h"
#include <iostream>
#include "graph.h"
#include<cstring>

void Graph::calculatePlusNeighbourHood(){
    for(int i=0;i<no_vertexes;i++){
        NODE *nd = &nodes[i];
        if(nd->size == 0) continue;
        int plus = binarySearchFirstElementGreaterOrEqualTarget(&edges[nd->offset],0,nd->size,nd->id);
        assert(plus<=nd->size);
        if(plus == nd->size)continue;
        nd->offset_plus = nd->offset + plus;
        nd->size_plus = nd->size - plus;
        assert(edges[nd->offset_plus]>nd->id);
    }
}

Graph::Graph(string dbName){
    std::ifstream infile(dbName);
    int a, b;
    infile >> no_vertexes >> no_edges;
    no_edges ++;
    no_vertexes ++;
    edges = (NODETYPE *)malloc(no_edges * sizeof(NODETYPE));
    nodes = (NODE *)malloc(no_vertexes * sizeof (NODE));
    memset( nodes, 0, sizeof (NODE));

    assert(no_edges > 1);
    infile >> a >> b;
    int n = 0;
    int t = 1;
    int d = 1;
    NODETYPE prev = a;
    edges[0] = b;
    while(infile >> a >> b){
        if(t>no_edges){
            cout << "EDGE OVERFLOW \n";
            assert(false);
        }
        edges[t] = b;
        if(prev != a){
            if(prev >= no_vertexes){
                cout << "VERTEX OVERFLOW \n";
                assert(false);
            }
            nodes[prev].id = prev;
            nodes[prev].offset = t-d;
            nodes[prev].size = d;
            prev = a;
            n++;
            d = 0;
        }
        t++;
        d++;
    }
    nodes[prev].id = prev;
    nodes[prev].offset = t-d;
    nodes[prev].size = d;
    this->calculatePlusNeighbourHood();
}

NODE * Graph::getNodeArray(){
    return nodes;
}

NODETYPE * Graph::getEdgeArray(){
    return edges;
}

int Graph::getNoVertexes(){
    return no_vertexes;
}

NODE* Graph::getNode(NODETYPE nodeID){
    return &nodes[nodeID];
//    auto it = nodemap->find(nodeID);
//    if(it == nodemap->end())return nullptr;
//    return &nodes[it->second];
}

void Graph::debugGraph(){
    cout << "####### GRAPH #### \n";
    for(int i=0;i<no_vertexes;i++){
        cout << nodes[i].id << ":";
        for(int j=0;j<nodes[i].size;j++){
            cout << edges[nodes[i].offset+j] << " ";
        }
        cout << nodes[i].size_plus << "\n";
        cout << "\n";
    }
}