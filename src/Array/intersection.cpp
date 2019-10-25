//
// Created by sandeep on 24/10/19.
//

#include "intersection.h"

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