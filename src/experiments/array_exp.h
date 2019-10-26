//
// Created by sandeep on 25/10/19.
//

#include <definitions.h>
#include <intersection.h>

#ifndef V3_ARRAY_EXP_H
#define V3_ARRAY_EXP_H

#endif //V3_ARRAY_EXP_H

/* Experiment
 * Assume 2 arrays
 * arr1: 1-100000
 * and another array with 3 elements less than 100000 */
void test_naive_intersection(NODETYPE *a, NODETYPE aSize,
        NODETYPE bSize, NODETYPE *bArr, NODETYPE  noArrays){
    init_metrics();
    start_timer(TOTALNODEPROCESSTIME);
    int s=0;
    for(int i=0;i<noArrays;i++){
        s = s + naive_intersect(a,aSize,&bArr[bSize*i],bSize);
    }
    stop_timer(TOTALNODEPROCESSTIME);
    print_statistics();
    cout << "Naive Intersection Total:" <<  s <<"\n";
}

void test_batch_intersection(NODETYPE *a, NODETYPE aSize,
                             NODETYPE bSize, NODETYPE *bArr, NODETYPE  noArrays){
    MinHeap *h = new MinHeap(noArrays);
    init_metrics();
    start_timer(TOTALNODEPROCESSTIME);

    NODETYPE *b_arr[noArrays];
    NODETYPE size_arr[noArrays];
    for(int i=0;i<noArrays;i++){
        b_arr[i] = &bArr[i * bSize];
        size_arr[i] = bSize;
    }
//    (NODETYPE* arrA, NODETYPE sizeA,
//            NODETYPE** bArrys, NODETYPE *bsizes, int batchsize, MinHeap * utilityHeap);
    int r = batched_intersection(a, aSize, b_arr, size_arr, noArrays, h);
    stop_timer(TOTALNODEPROCESSTIME);
    print_statistics();

    cout << "Batched Intersection:" << r << "\n";
}

int test_array_intersection(){
    int aSize = 1000000;
    const int bSize = 1000;
    const int noArrays = 128;

    NODETYPE a[aSize];
    NODETYPE bArr[noArrays * bSize];
    for(int i=0;i<aSize;i++){
        a[i] = i;
    }
    int start = aSize - bSize;
    for(int i=0;i<noArrays;i++){
        for(int j=0;j<bSize;j++){
            bArr[i*bSize+j] = start + j;
        }
    }

    test_naive_intersection(a, aSize, bSize, bArr, noArrays);
    test_batch_intersection(a, aSize, bSize, bArr, noArrays);
}