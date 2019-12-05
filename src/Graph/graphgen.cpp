//
// Created by sandeep on 24/11/19.
//

#include "graph.h"
#include<cstring>
#include <fstream>
#include <assert.h>

/* These functions are used to explore the cost of of using a bitvectors
 * Experiment.
 * (Node 1, neighbours [a1....a_n], each having a degree d_n
 * all connected to the same elements {d1....d_n}
 * Time to find out all squares starting from node 1.
 * Prove that there exists a cartesian product which can be avoided.
 * a_n | d_n | Bit      | naive + simd | Speedup of Bit
 * 10  | 10  | 9e-6     | 7e-6         | ~1
 * 100 | 10  | .000132  | .0005        | x5
 * 1000| 10  | .009     | .045         | x5
 * 10  | 100 | 1.7e-5   | 3.1e-5       | x2
 * 10  | 1000| .000151  | .000321      | x2
 * 100 | 100 | .000481  | .00339       | x10
 * 100 | 1000| .003     | .029         | x10
 * Conclusion there is definetely a speed up in the best case x10
 */
void generateGraphAllCommon(int a_n, int d_n, string outfile){
    int total_vertex = a_n + 1 + d_n;
    int total_edges = a_n + a_n*d_n;
    std::ofstream of(outfile);
    of << total_vertex << " " << total_edges << "\n";
    for(int i=1;i<=a_n;i++){
        of << 0 << " " << i <<"\n";
    }
    for(int i=1;i<=a_n;i++){
        for(int j=1;j<=d_n;j++){
            of << i << " " << a_n+j<< "\n";
        }
    }
    of.close();
}

/*  Experiment:
 *  Cost of finding out all square starting from vertex 0.
 *  Adj(0) -> {a_1,a_2.....a_n}
 *  Degree(a_i)= d_n
 *  However a_i /\ a_j = null set. Nothing in common
 *  a_n | d_n | Naive |BitVector * Time to find out all squares starting from node 1.
 * Prove that there exists a cartesian product which can be avoided.
 * Evaluate various costs of bit matrix construction.
 * a_n | d_n  | Naive+simd | Bit-Naive    | Bit-MinHeap
 * 10  | 1000 | .00029     | .00016 [2x]  | .000449
 * 100 | 10   | .000335    | .000263      | .000266 [.5x]
 * 100 | 100  | .0002      | .04          | .0016  [ .2x]
 * 100 | 1000 | .016       | .038  [BAD]  | .0043 [4x]
 * 1000| 10   | .034       | 2.47  [BAD]  | .0125 [2x]
 * Worst case:
 * Isolate cases where a_n > 100
 * Conclusion there is definetely a speed up in the best case x10 worst case 2x
 * 1. a_n is less<20 do not use a minheap for base construction
 * 2. When a_n is more than say 20 use a min heap */
void generateGraphNothingCommon(int a_n, int d_n, string outfile){
    int total_vertex = a_n + 1 + d_n;
    int total_edges = a_n + a_n*d_n;
    std::ofstream of(outfile);
    of << total_vertex << " " << total_edges << "\n";
    for(int i=1;i<=a_n;i++){
        of << 0 << " " << i <<"\n";
    }
    for(int i=1;i<=a_n;i++){
        for(int j=1;j<=d_n;j++){
            of << i << " " << a_n*i+j<< "\n";
        }
    }
    of.close();
}

/*
 * Lot of small degree vertexes and some large degree vertexes.
 * a_small | d_small | a_large | d_large | Naive | Bit | Performance
 * 100     |    100  |      5  |  1000   |  e-5  | e-6 | Bad
 * 100     |    100  |      5  |  10000  |  e-5  | e-6 | Bad
 *
 * Conclusion.
 * Highly skewed case has a 10 order detriment.
 *
 * */
void generateGraphSkewNothingCommon(int a_small, int d_small,
        int a_large, int d_large, string outfile){
    int total_vertex = a_small + a_large + 1 + a_small * d_small + a_large * d_large;
    int total_edges = a_small + a_large + a_small*d_small + a_large * d_large;
    std::ofstream of(outfile);
    of << total_vertex << " " << total_edges << "\n";
    for(int i=1;i<=a_small+a_large;i++){
        of << 0 << " " << i <<"\n";
    }
    for(int i=1;i<=a_small;i++){
        for(int j=1;j<=d_small;j++){
            of << i << " " << a_small*i+j<< "\n";
        }
    }

    for(int i=a_small+1;i<=a_small+a_large;i++){
        for(int j=1;j<=d_large;j++){
            of << i << " " << (a_small+a_large*i+j)<< "\n";
        }
    }
    of.close();
}

void experiment_code_to_compute_cost_model(){
//    int i = 100;
//    int j = 1000;
//    cout << "neighbours: " << i << " avg_degree:" << j <<"\n";
//    generateGraphNothingCommon(i,j,"data/data1.graph");
//    Graph * graph = new Graph("data/data1.graph");
//    naive_square_counting(graph);
//    bit_vector_based(graph);
}

