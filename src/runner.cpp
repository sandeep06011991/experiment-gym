//
// Created by sandeep on 14/8/19.
//
#include <iostream>
#include <intersection.h>
#include "graph.h"

using namespace std;

int triangle_counting(Graph *graph){

    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd1 = ndArray[i];
        NODETYPE* aNeighourArray = &edgeArray[nd1.offset_plus];
        NODETYPE aSize = nd1.size_plus;
        for(int j=0;j<nd1.size_plus;j++){
            NODETYPE bNode = aNeighourArray[j];
            NODE nd2 =  ndArray[bNode];
            NODETYPE* bNeighbourArray = &edgeArray[nd2.offset_plus];
            NODETYPE bSize = nd2.size_plus;
            s = s + naive_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
        }
    }
    cout << "triangles found:" <<  s << "\n";
}

int main(int argc, char *argv[]) {

    cout << "No. of arguments recieved " << argc <<"\n";
    Graph * graph = new Graph("data/com-amazon.ungraph.txt.ro");
//    Graph * graph = new Graph("data/data1.graph");
//    graph->debugGraph();
    triangle_counting(graph);
}