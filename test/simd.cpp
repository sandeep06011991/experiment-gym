//
// Created by sandeep on 15/11/19.
//

#include "gtest/gtest.h"
#include "definitions.h"
#include "intersection.h"

#include <iostream>
using namespace std;
/*
 * Profiling different components
 * 1. running without any function calls : .020s
 * 2. call main intersection but do not go into simd-inter : .06
 * 3. call main intersect , go into dummy simd-inter(cost of fcall): .08
 * 4. load into simd vectors and shuffle = .11
 * 5. Compare and mask .194
 * -------------- TO HIGH (INVESTIGATE)
 *      a. Use 4 compare and 1 mask .183
 *
 * 6. Everything running: .25
 * */
TEST(simd, four_int_intersections) {
    alignas(32) NODETYPE base[] = {1,2,3,4,5,6,7,8};
    alignas(32) NODETYPE arr[] ={1,2,4,5,6,9,19,11};
    double start = std::clock();
    int r;
   r = intersectSets4IntegerAttime(base,8,arr,8);
    double end = std::clock();
    cout << "Total time" << (end-start)/1000000 <<"\n";
    cout << r << "\n";
}


TEST(simd, eight_int_intersections) {
    alignas(32) NODETYPE base[] = {1,2,3,4,5,6,7,8};
    alignas(32) NODETYPE arr[] =  {1,2,4,5,6,9,19,11};
    double start = std::clock();
    int r;
    r = intersectSets8IntegerAttime(base,8,arr,8);
    double end = std::clock();
    cout << "Total time" << (end-start)/1000000 <<"\n";
    cout << r << "\n";
}