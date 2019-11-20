//
// Created by sandeep on 19/11/19.
//
#include <cstdlib>

#ifndef V3_BRANCH_EVAL_H
#define V3_BRANCH_EVAL_H

#endif //V3_BRANCH_EVAL_H
#define LIKELY(condition) __builtin_expect((condition), 1)


/* Experiment:
 * The objective of this experiment is to measure the cost of branch mis-prediction.
 *  for(int i=0;i<1;i++){
        long a = 100000000;
        miss_branches(a);
        no_branch_miss(a);
    }
 * Conclusion: There was a 2 x performance improvement.
 * Although not conclusive and not universal.
 * However, there is a substantial drop in branch mis-predictions.
 *
 */
void miss_branches(int max){
    int j=0;

    std::cout << "miss branches loops:" << max <<"\n";
    for(int i = 0;i<max;i++){
        int s = i;
        if((s + (s*s))%2==0){
          j++;
        }else{
          j++;
        }
    }
}

void no_branch_miss(int max){
    int i=0; int j=0;
    std::cout << "no miss branches loops:" << max <<"\n";
    for(int i = 0;i<max;i++){
        int s = i;
        s = (s + (s*s))%2;
        j++;
    }
}