//
// Created by sandeep on 12/8/19.
//

#ifndef V2_DEFINITIONS_H
#define V2_DEFINITIONS_H

typedef unsigned int NODETYPE;

struct NODE {
    NODETYPE id;
    NODETYPE offset;
    NODETYPE size;
    NODETYPE offset_plus;
    NODETYPE size_plus;
};

const int MAX_QUERY_SIZE = 10;

#endif //V2_DEFINITIONS_H
