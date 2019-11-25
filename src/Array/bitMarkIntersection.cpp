//
// Created by sandeep on 21/11/19.
//

#include "intersection.h"
#include "ArrayUtilities.h"

int naivebitMark_intersect(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB,
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


int binaryBitMark_intersectLargeBase(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB,
        NODETYPE sizeB ,unsigned char *bitV){
    // sizea >= sizeB
    int s = 0;
    int curr_a = 0;
    int curr_b = 0;
    while (curr_a < baseSize && curr_b < sizeB) {
        if (baseA[curr_a] == arrB[curr_b]) {
            bitV[curr_a/8] = bitV[curr_a/8] | 1U << (7-(curr_a%8));
            s ++ ;
            curr_a ++ ;
            curr_b ++ ;
            continue;
        }
        if (baseA[curr_a] < arrB[curr_b]) {
            curr_a = searchFirstElementGreaterOrEqualTarget(baseA,curr_a,baseSize,arrB[curr_b]);
//            curr_a = gallopingSearchFirstElementGreaterOrEqualTarget(arrA, curr_a, sizeA, arrB[curr_b]);
//            curr_a =  binarySearchFirstElementGreaterOrEqualTarget(arrA, curr_a, sizeA, arrB[curr_b]);
        } else {
            curr_b ++;
        }
    }
    return s;
}

int binaryBitMark_intersectLargeArray(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB,
                                     NODETYPE sizeB ,unsigned char *bitV){
    // sizea >= sizeB
    int s = 0;
    int curr_a = 0;
    int curr_b = 0;
    while (curr_a < baseSize && curr_b < sizeB) {
        if (baseA[curr_a] == arrB[curr_b]) {
            bitV[curr_a/8] = bitV[curr_a/8] | 1U << (7-(curr_a%8));
            s ++ ;
            curr_a ++ ;
            curr_b ++ ;
            continue;
        }
        if (baseA[curr_a] < arrB[curr_b]) {
            curr_a ++;
//            curr_a = gallopingSearchFirstElementGreaterOrEqualTarget(arrA, curr_a, sizeA, arrB[curr_b]);
//            curr_a =  binarySearchFirstElementGreaterOrEqualTarget(arrA, curr_a, sizeA, arrB[curr_b]);
        } else {
            curr_b = searchFirstElementGreaterOrEqualTarget(arrB,curr_b,sizeB,baseA[curr_a]);
        }
    }
    return s;
}


/* Hybrid-20 : 56s
 * Naive :   65s
 * Shaved around: 10 seconds.  */
int hybridBitMark_intersect(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB, NODETYPE sizeB ,unsigned char *pos){
    int tolerance =20;
    if(baseSize > 20*sizeB){
        return binaryBitMark_intersectLargeBase(baseA,baseSize,arrB,sizeB,pos);
    }
    if(sizeB > 20*sizeB){
        return binaryBitMark_intersectLargeArray(baseA,baseSize,arrB,sizeB,pos);
    }

    return naivebitMark_intersect(baseA,baseSize,arrB,sizeB,pos);
}
