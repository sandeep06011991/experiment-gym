//
// Created by sandeep on 24/10/19.
//

#include <assert.h>
#include <cstring>
#include <iostream>
#include "intersection.h"
#include "ArrayUtilities.h"
#include "MinHeap.h"

/*  Naive intersection of sets, no gallopping
 * Experiment: What is the contribution of skew to total intersection runtime
 * Reproduce: Naive triangle counting function calls naive intersection
 * skip over vertex marked as skew accoding to the following function
 * if((sizeA > sizeB)){
        if(sizeA > [TOLERANCE  * sizeB )return 0;
    }else{
        if(sizeB > [TOLERANCE * sizeA )return 0;
    }
    For LiveJournal Triangle counting runtime for tolerance levels
    Tolerance | Runtime
    -----------------------------------
    1.5       | 8.7
    2         | 11.5
    3         | 16.31
    4         | 15.04
    5         | 15.45
    Conclusion: Skew has an impact but since tolerance levels are low. I cant
    make a conclusion of its effects *
 * */
int naive_intersect(NODETYPE* arrA, NODETYPE sizeA, NODETYPE* arrB, NODETYPE sizeB) {
    int curr_a = 0;
    int curr_b = 0;
    int s = 0;
    while (curr_a < sizeA && curr_b < sizeB) {
        if (arrA[curr_a] == arrB[curr_b]) {
            s ++ ;
            curr_a ++ ;
            curr_b ++ ;
            continue;
        }
        if (arrA[curr_a] < arrB[curr_b]) {
            curr_a ++;
        } else {
            curr_b ++;
        }
    }
    return s;
}

int binary_intersect(NODETYPE* arrA, NODETYPE sizeA, NODETYPE* arrB, NODETYPE sizeB){
    if(sizeA < sizeB){
        NODETYPE *t =arrA;
        NODETYPE s = sizeA;
        arrA = arrB;
        sizeA = sizeB;
        arrB = t;
        sizeB = s;
    }
    // sizea >= sizeB
    int s = 0;
    int curr_a = 0;
    int curr_b = 0;
    while (curr_a < sizeA && curr_b < sizeB) {
        if (arrA[curr_a] == arrB[curr_b]) {
            s ++ ;
            curr_a ++ ;
            curr_b ++ ;
            continue;
        }
        if (arrA[curr_a] < arrB[curr_b]) {
            curr_a =  binarySearchFirstElementGreaterOrEqualTarget(arrA, curr_a, sizeA, arrB[curr_b]);
        } else {
            curr_b ++;
        }
    }
    return s;
}

int hybrid_intersect(NODETYPE* arrA, NODETYPE sizeA, NODETYPE* arrB, NODETYPE sizeB){
    int tolerance = 10;
    assert(tolerance > 1);
    if(sizeA > tolerance * sizeB){
        return binary_intersect(arrA,sizeA,arrB,sizeB);
    }
    if(sizeB > tolerance * sizeA){
        return binary_intersect(arrA,sizeA,arrB,sizeB);
    }
    return 0;
    return intersectSets4IntegerAttime(arrA,sizeA,arrB,sizeB);

//    return naive_intersect(arrA,sizeA,arrB,sizeB);
}

int batched_intersection(NODETYPE* arrA, NODETYPE sizeA,
        NODETYPE** bArrys, NODETYPE *bsizes, int batchsize, MinHeap * utilityHeap){
    utilityHeap->clear();
    NODETYPE curr_a = 0;
    NODETYPE curr_p[batchsize];
    memset(curr_p,0,batchsize*sizeof(NODETYPE));
    for(int i=0; i < batchsize; i++){
        if(bsizes[i] == 0){
            continue;
        }else{
            utilityHeap->addElement(bArrys[i][0], i);
            curr_p[i] = 1;
        }
    }
    HeapElement temp;
    int k=0;
    while((utilityHeap->currentSize > 0) && (curr_a < sizeA)) {
        temp = utilityHeap->peekTopElement();
        if (temp.value == arrA[curr_a]) {
            if (curr_p[temp.arrayId] != bsizes[temp.arrayId]) {
                utilityHeap->popTopAndInsertNewElement(bArrys[temp.arrayId][curr_p[temp.arrayId]],
                                                       temp.arrayId);
                curr_p[temp.arrayId]++;
            } else {
                utilityHeap->popElement(&temp);
            }
            k++;
            continue;
        }
        if (temp.value > arrA[curr_a]) {
            curr_a++;
            continue;
        } else {
            int csize = curr_p[temp.arrayId];
            while((csize < bsizes[temp.arrayId]) && (bArrys[temp.arrayId][csize] < arrA[curr_a])){
                csize ++;
            }
            curr_p[temp.arrayId] = csize;
            if (curr_p[temp.arrayId] != bsizes[temp.arrayId]) {

                utilityHeap->popTopAndInsertNewElement(bArrys[temp.arrayId][curr_p[temp.arrayId]],
                                                       temp.arrayId);
                curr_p[temp.arrayId]++;
            } else {
                utilityHeap->popElement(&temp);
            }
        }
    }
    return k;
}