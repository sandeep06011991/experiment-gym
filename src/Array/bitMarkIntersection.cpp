//
// Created by sandeep on 21/11/19.
//

#include "intersection.h"

int bitMark_intersect(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB,
        NODETYPE sizeB ,unsigned char *bitV){
    int bp = 0;
    int ap = 0;
    int t = 0;
    while((bp < baseSize) && (ap < sizeB)){
        if (baseA[bp] == arrB[ap]) {
            bitV[bp/8] = bitV[bp/8] | 1U << (7-(bp%8));
            t++;
            bp ++ ;
            ap ++ ;
            continue;
        }
        if (baseA[bp] < arrB[ap]) {
            bp ++;
        } else {
            ap ++;
        }
    }
    return t;
}
