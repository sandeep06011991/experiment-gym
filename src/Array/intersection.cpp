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
            curr_a = binarySearchFirstElementGreaterOrEqualTarget(arrA,curr_a,sizeA,arrB[curr_b]);
//            curr_a = gallopingSearchFirstElementGreaterOrEqualTarget(arrA, curr_a, sizeA, arrB[curr_b]);
//            curr_a =  binarySearchFirstElementGreaterOrEqualTarget(arrA, curr_a, sizeA, arrB[curr_b]);
        } else {
            curr_b ++;
        }
    }
    return s;
}

/* Exp:
 * Performance on youtube square:
 * 128 bit (intersect sets 4 integers at a time) -> 28.19, 31.31, 29.4
 * 256 bit (intersect sets of 8 integers at a time) -> 26.97, 25.5, 26.45
 * LIGHT-> 26.03, 30.66, 27.66*/
int hybrid_intersect(NODETYPE* arrA, NODETYPE sizeA, NODETYPE* arrB, NODETYPE sizeB){
    int tolerance = 20;
    assert(tolerance > 1);
    if(sizeA > tolerance * sizeB){
//        return 0;
        return binary_intersect(arrA,sizeA,arrB,sizeB);
    }
    if(sizeB > tolerance * sizeA){
//        return 0;
        return binary_intersect(arrB,sizeB,arrA,sizeA);
    }
//    return 0;
//    return intersectSets8IntegerAttime(arrA,sizeA,arrB,sizeB);
//    return intersectSets4IntegerAttime(arrA,sizeA,arrB,sizeB);

    return naive_intersect(arrA,sizeA,arrB,sizeB);
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

int naive_intersection_and_store(NODETYPE *nd2,NODETYPE nd2Size, NODETYPE *nd3, int nd3Size,
                                 NODETYPE *str){
    int j=0;
    int k=0;
    int i=0;
    while((j<nd2Size) && (k<nd3Size)){
        if(nd2[j] == nd3[k]){
            str[i] = nd2[j];
            i++;
            j++;
            k++;
            continue;
        }
        if(nd2[j]<nd3[k]){
            j++;
        }else{
            k++;
        }
    }
    return i;
}

int binary_intersection_and_store(NODETYPE *nd2,NODETYPE nd2Size, NODETYPE *nd3, int nd3Size,
                                 NODETYPE *str){
    assert(nd2Size > nd3Size);
    int j=0;
    int k=0;
    int i=0;
    while((j<nd2Size) && (k<nd3Size)){
        if(nd2[j] == nd3[k]){
            str[i] = nd2[j];
            i++;
            j++;
            k++;
            continue;
        }
        if(nd2[j]<nd3[k]){
            j = binarySearchFirstElementGreaterOrEqualTarget(nd2,j+1,nd2Size,nd3[k]);
//            j++;
        }else{
            k++;
        }
    }
    return i;
}
/* Naive version*/
int intersectTrie4Square(NODETYPE *nd2,NODETYPE nd2Size, NODETYPE *nd3, int nd3Size,
                         NODETYPE *nd1, NODETYPE nd1Size){
    int resSize = nd3Size;
    if(nd2Size <nd3Size) resSize = nd2Size;
    int skewFactor = 20;
    NODETYPE interResults[resSize];
    int j=0;
    int i=0;
    int total_results;
    if(nd2Size * skewFactor < nd3Size){
        total_results = binary_intersection_and_store(nd3, nd3Size,nd2,nd2Size,  interResults);
    }else{
        if(nd3Size * skewFactor < nd2Size){
            total_results = binary_intersection_and_store(nd2,nd2Size,nd3, nd3Size,interResults);
        }else{
            total_results = naive_intersection_and_store(nd2,nd2Size, nd3, nd3Size, interResults);
        }
    }
    int s = 0;
    while((i < nd1Size) && (j < total_results)){
        if(nd1[i] < interResults[j]){
            s = s + (total_results - j);
            i++;
            continue;
        }
        if(nd1[i] >= interResults[j]){
            j++;
        }
    }
    return s;
}


int intersectAndReturnCandidateSets(NODETYPE *nd1,NODETYPE nd1Size, NODETYPE *nd2, int nd2Size,
                                    NODETYPE *returnbuffer){
    int i=0;
    int j=0;
    int k=0;
    while((i < nd1Size) && (j < nd2Size)){
        if(nd1[i]==nd2[j]){
            returnbuffer[k]=nd1[i] ;
            i++;
            j++;
            k++;
            continue;
        }
        if(nd1[i] < nd2[j]){
            i++;
            continue;
        }
        if(nd1[i] > nd2[j]){
            j++;
            continue;
        }
    }
    return k;
}

/* return buffer now contains ids indexed by nd1 */
int intersectAndReturnCandidateIds(NODETYPE *nd1,NODETYPE nd1Size, NODETYPE *nd2, int nd2Size,
                                    NODETYPE *returnbufferCandidates, NODETYPE *returnbufferIds){
    int i=0;
    int j=0;
    int k=0;
    while((i < nd1Size) && (j < nd2Size)){
        if(nd1[i]==nd2[j]){
            returnbufferCandidates[k] = nd1[i];
            returnbufferIds[k]=i ;
            i++;
            j++;
            k++;
            continue;
        }
        if(nd1[i] < nd2[j]){
            i++;
            continue;
        }
        if(nd1[i] > nd2[j]){
            j++;
            continue;
        }
    }
    return k;
}

inline void markBit(unsigned char *bitVector, int bitID){
    int position = bitID / 8;
    int offset = 7 - (bitID%8);
    bitVector[position] = bitVector[position] | 1U << offset;
}

int intersectAndMarkBits(NODETYPE *nd1,NODETYPE nd1Size, NODETYPE *nd2, int nd2Size,
                         unsigned char *bitVector, NODETYPE * results){
    int i=0;
    int j=0;
    int k=0;
    memset(bitVector,0,nd1Size);
    memset(results,0,nd1Size);
    while((i < nd1Size) && (j < nd2Size)){
        if(nd1[i]==nd2[j]){
            markBit(bitVector, i);
            results[k] = i;
            i++;
            j++;
            k++;
            continue;
        }
        if(nd1[i] < nd2[j]){
            i++;
            continue;
        }
        if(nd1[i] > nd2[j]){
            j++;
            continue;
        }
    }
    return k;
}