//
// Created by sandeep on 20/3/20.
//

#include "GHDNode.h"
#include <unordered_map>
#include <iostream>

GHDNode::GHDNode(int noAttributes, int noRelations, relation *rels){
//    nmap = new std::unordered_map<int,neighbourhood_plus>();

    this->noAttributes = noAttributes;
    this->noRelations = noRelations;
    std::sort(rels,rels + noRelations, [&](relation a, relation b){
        if(a.attr1 == b.attr1) return a.attr2 < b.attr2;
        return a.attr1 < b.attr1;
    });
    this->rels = rels;

    int * nattr = (int *)malloc(sizeof(int) * noAttributes * noAttributes);
    nbs = (neighbourhood_plus *)malloc(sizeof(neighbourhood_plus) * noAttributes);

    for(int i=0;i<noAttributes;i++){
        nbs[i].attrs = &nattr[i*noAttributes];
        nbs[i].size = 0;
    }
    for(int i=0;i<this->noRelations;i++){
        relation r = rels[i];
        assert(r.attr1 < r.attr2);
//          Enforce order of enumeration
//        if(r.attr2 < i) continue;
        int p = nbs[r.attr2].size;
        nbs[r.attr2].attrs[p] = r.attr1;
        nbs[r.attr2].size ++;
    }

}

neighbourhood_plus GHDNode::getNPlusNeighbourhood(const int attribute){
    assert(attribute < noAttributes);
    return nbs[attribute];
}

int GHDNode::getNoIncidentAttributes(int attribute){
    assert(attribute < noAttributes);
    return nbs[attribute].size;
}

int GHDNode::getNoAttributes(){
    return noAttributes;
}


GHDNode * getTriangleGHDNode(){
    const int noA = 3;
    const int noR = 3;
    struct relation * rels = (relation *)malloc((noR) * sizeof(struct relation));
    rels[0] = {0,1};
    rels[1] = {0,2};
    rels[2] = {1,2};
    return new GHDNode(noA, noR,rels);
}

GHDNode * get4Clique(){
    const int noA = 4;
    const int noR = 6;
    struct relation * rels = (relation *)malloc((noR) * sizeof(struct relation));
    rels[0] = {0,1};
    rels[1] = {0,2};
    rels[2] = {0,3};
    rels[3] = {1,2};
    rels[4] = {1,3};
    rels[5] = {2,3};
    return new GHDNode(noA, noR,rels);
}
GHDNode * get5Clique(){
    const int noA = 5;
    const int noR = 10;
    struct relation * rels = (relation *)malloc((noR) * sizeof(struct relation));
    rels[0] = {0,1};
    rels[1] = {0,2};
    rels[2] = {0,3};
    rels[3] = {0,4};
    rels[4] = {1,2};
    rels[5] = {1,3};
    rels[6] = {1,4};
    rels[7] = {2,3};
    rels[8] = {2,4};
    rels[9] = {3,4};
    return new GHDNode(noA, noR,rels);
}

GHDNode * getSquareNode(){
    const int noA = 4;
    const int noR = 4;
    struct relation * rels = (relation *)malloc((noR) * sizeof(struct relation));
    rels[0] = {0,1};
    rels[1] = {0,2};
    rels[2] = {1,3};
    rels[3] = {2,3};
    return new GHDNode(noA, noR,rels);
}



GHDNode * getEdgeGHDNode(){
    const int noA = 2;
    const int noR = 1;
    struct relation * rels = (relation *)malloc((noR) * sizeof(struct relation));
    rels[0] = {0,1};
    return new GHDNode(noA, noR,rels);
}


GHDNode * get6Clique(){
    const int noA = 6;
    const int noR = 14;
    struct relation * rels = (relation *)malloc((noR) * sizeof(struct relation));
    rels[0] = {0,1};
    rels[1] = {0,2};
    rels[2] = {0,3};
    rels[3] = {0,4};
    rels[4] = {0,5};
    rels[5] = {1,2};
    rels[6] = {1,3};
    rels[7] = {1,4};
    rels[8] = {1,5};
    rels[9] = {2,3};
    rels[10] = {2,4};
    rels[11] = {2,5};
    rels[12] = {3,4};
    rels[13] = {3,5};
    rels[14] = {4,5};
    return new GHDNode(noA, noR,rels);
}
