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
