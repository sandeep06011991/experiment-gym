//
// Created by sandeep on 24/10/19.
//

#ifndef V3_INTERSECTION_H
#define V3_INTERSECTION_H

#include <definitions.h>
#include "MinHeap.h"

int naive_intersect(NODETYPE* arrA, NODETYPE sizeA, NODETYPE* arrB, NODETYPE sizeB);

int binary_intersect(NODETYPE* arrA, NODETYPE sizeA, NODETYPE* arrB, NODETYPE sizeB);

int hybrid_intersect(NODETYPE* arrA, NODETYPE sizeA, NODETYPE* arrB, NODETYPE sizeB);

int naivebitMark_intersect(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB, NODETYPE sizeB ,unsigned char *pos);

int binaryBitMark_intersect(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB, NODETYPE sizeB ,unsigned char *pos);

int hybridBitMark_intersect(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB, NODETYPE sizeB ,unsigned char *pos);

int batched_intersection(NODETYPE* arrA, NODETYPE sizeA,
                         NODETYPE** bArrys, NODETYPE *bsizes, int batchsize, MinHeap * utilityHeap);

int  intersectSets4IntegerAttime(NODETYPE *base, int baseSize, NODETYPE *A, int ASize);


int intersectSets8IntegerAttime(NODETYPE *base, int baseSize, NODETYPE *A, int ASize);

#endif //V3_INTERSECTION_H
