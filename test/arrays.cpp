//
// Created by sandeep on 25/10/19.
//



#include <definitions.h>
#include "ArrayUtilities.h"
#include "gtest/gtest.h"


TEST(search, binary_search_first_Greater_Equal) {
    int max = 100;
    NODETYPE a[max];
    for(int i=0;i<max;i++){
        a[i] = i;
    }
    for(int i=0;i<max;i++){
        int r = binarySearchFirstElementGreaterOrEqualTarget(a,0,max,i);
        EXPECT_EQ(i,r);
    }
    for(int i=0;i<max;i++){
        int r = binarySearchFirstElementGreaterOrEqualTarget(a,i,max,i);
        EXPECT_EQ(i,r);
    }
}

TEST(search, binary_search_first_Greater) {
    int max = 100;
    NODETYPE a[max];
    for(int i=0;i<max;i++){
        a[i] = i;
    }
    for(int i=0;i<max;i++){
        int r = binarySearchFirstElementGreaterTarget(a,0,max,i);
        EXPECT_EQ(i+1,r);
    }
    for(int i=0;i<max;i++){
        int r = binarySearchFirstElementGreaterTarget(a,i,max,i);
        EXPECT_EQ(i+1,r);
    }
}

TEST(searchg, gallopping_search_first_Greater) {
    int max = 100;
    NODETYPE a[max];
    for(int i=0;i<max;i++){
        a[i] = i;
    }
    for(int i=0;i<max;i++){
        int r = gallopingSearchFirstElementGreaterOrEqualTarget(a,0,max,i);
        EXPECT_EQ(i,r);
    }
    for(int i=0;i<max;i++){
        int r = gallopingSearchFirstElementGreaterOrEqualTarget(a,i,max,i);
        EXPECT_EQ(i,r);
    }
}

TEST(perfunion, perf) {
    int max = 100;
    int noArrays = 100;
    int *arrAddr[noArrays];
    int arrSizes[noArrays];
    int arr[max*noArrays];
    for(int i=0;i<noArrays;i++){
        arrAddr[i] = &arr[max*i];
        arrSizes[i] = max;
    }
    for(int i=0;i<max*noArrays;i++){
        arr[i] = i;
    }
    EXPECT_EQ(1,2);
}

