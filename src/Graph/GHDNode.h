//
// Created by sandeep on 8/3/20.
//


#include <assert.h>
#include <algorithm>
#include <unordered_map>


#ifndef V3_GHDNODE_H
#define V3_GHDNODE_H

struct relation{
    int attr1;
    int attr2;
};

struct neighbourhood_plus{
    int *attrs;
    int size;
};

/* Captures all the attributes of a query plan */
/* Attributes are numbered (0,noAttributes-1)
 * Relations are listed in increasing order.
 * */
class GHDNode {

    int noAttributes;

    int noRelations;

    struct relation * rels;

    neighbourhood_plus * nbs;
//    std::unordered_map<int,neighbourhood_plus> * nmap;

public:

    GHDNode(int noAttributes, int noRelations, relation * rels);

    neighbourhood_plus getNPlusNeighbourhood(const int attribute);

    int getNoIncidentAttributes(int attribute);

    int getNoAttributes();

};

GHDNode * getTriangleGHDNode();

GHDNode * getEdgeGHDNode();

GHDNode * getSquareGHDNode();

GHDNode * getNClique();

GHDNode * get4Clique();


#endif //V3_GHDNODE_H
