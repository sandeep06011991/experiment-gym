//
// Created by sandeep on 21/11/19.
//

#ifndef V3_BITVECTORUTIL_H
#define V3_BITVECTORUTIL_H

#include <definitions.h>

const int maxArraySize = 1000000;


int  computeNaiveBaseArrayAndReturnSize(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE* baseArray);

int computeBaseArrayWithBinaryAndReturnSize(NODETYPE **arrays, NODETYPE *arrsizes,
        NODETYPE noArrays, NODETYPE * baseArray);

int computeBaseArrayWithMinHeap(NODETYPE **arrays, NODETYPE *arrsizes,
                                NODETYPE noArrays, NODETYPE * baseArray);

int computeNaiveBaseArrayWithoutDuplicates(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE * baseArray);

#endif //V3_BITVECTORUTIL_H
