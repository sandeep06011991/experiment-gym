//
// Created by sandeep on 9/8/19.
//

#include "gtest/gtest.h"

TEST(IndependentMethod, ResetsToZero) {
    EXPECT_TRUE(0==0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}