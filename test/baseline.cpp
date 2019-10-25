//
// Created by sandeep on 25/10/19.
//

#include "gtest/gtest.h"
#include "naive_triangle_counting.h"

#include "iostream"
#include "../src/Graph/graph.h"

using namespace std;

TEST(test2A, test2B) {
    Graph * graph = new Graph("data/com-youtube.ungraph.txt.ro");
    int r = triangle_counting(graph);
    EXPECT_TRUE(r==305686);
}
