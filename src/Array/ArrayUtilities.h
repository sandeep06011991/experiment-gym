//
// Created by sandeep on 15/9/19.
//

#include "definitions.h"

#ifndef V2_ARRAYUTILITIES_H
#define V2_ARRAYUTILITIES_H


int binary_search(NODETYPE* nd, int start, int end, NODETYPE target);

int binarySearchFirstElementGreaterOrEqualTarget(NODETYPE *arr,int start, NODETYPE size, int target);

int binarySearchFirstElementLessOrEqualTarget(NODETYPE *arr,int start, NODETYPE size, int target);

int binarySearchFirstElementGreaterTarget(NODETYPE *arr,int start, NODETYPE size, int target);

int gallopingSearchFirstElementGreaterOrEqualTarget(NODETYPE *arr,int start, NODETYPE size, int target) ;

#endif //V2_ARRAYUTILITIES_H
