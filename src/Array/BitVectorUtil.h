//
// Created by sandeep on 21/11/19.
//

#ifndef V3_BITVECTORUTIL_H
#define V3_BITVECTORUTIL_H

#include <definitions.h>

const int maxArraySize = 1000000;


int  computeNaiveBaseArrayAndReturnSize(NODETYPE **arrays, NODETYPE *arrsizes,
        NODETYPE noArrays, NODETYPE* baseArray);

int  computeNaiveBaseArraySkew(NODETYPE **arrays, NODETYPE *arrsizes,
                                        NODETYPE noArrays, NODETYPE* baseArray);

int computeBaseArrayWithMinHeap(NODETYPE **arrays, NODETYPE *arrsizes,
                                NODETYPE noArrays, NODETYPE * baseArray);

int computeNaiveBaseArrayWithDuplicates(NODETYPE **arrays, NODETYPE *arrsizes,
            NODETYPE noArrays, NODETYPE * baseArray);

int computeBaseArrayAndBitMatrixReturnSize(NODETYPE **arrays, NODETYPE *arrsizes,
                                           NODETYPE noArrays, NODETYPE * baseArray);

int computeBaseArrayUsingBinaryRecursion(NODETYPE **arrays, NODETYPE *arrsizes, NODETYPE noArrays, NODETYPE * baseArray);


int bloomfilter_based_baseArray(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE * baseArray);

int usingTrivialSort(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE * baseArray);

int sortNeighbourhood(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE * baseArray);

#endif //V3_BITVECTORUTIL_H
