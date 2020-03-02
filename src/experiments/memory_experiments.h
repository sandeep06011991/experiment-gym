//
// Created by sandeep on 1/3/20.
//

#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

using namespace std;
#ifndef V3_MEMORY_EXPERIMENTS_H
#define V3_MEMORY_EXPERIMENTS_H

#endif //V3_MEMORY_EXPERIMENTS_H
/*############
 * size     | READ      | Write
 * 100000   | .0002     | .00185
 * 1000000  | .0024     | .01821
 * 10000000 | .0306     | .19  */
void test_cost_of_read_and_write(){
    int size = 10000000;
    int * rd = (int *)malloc(sizeof(int) * size);
    double start_read_time = std::clock();
    for(int i=0;i<size;i++){
        int s = rd[i];
        s++;
    }
    double stop_read_time = std::clock();
    vector<int> *v = new vector<int>(100000);
    double start_write_time = std::clock();
    for(int i=0;i<size;i++){
        v->push_back(i);
    }
    double stop_write_time = std::clock();
    free(rd);
    v->clear();
    cout << "read_time" << (stop_read_time-start_read_time) / 1000000<<"\n";
    cout << "write_time" << (stop_write_time-start_write_time) / 1000000 <<"\n";

}

const int PAGESIZE = 10;
struct element{
    int a[PAGESIZE];
};

const int KB = 1000;
const int MB = 1000 * 1000 ;
const int GB = 1000 * 1000 * 1000;

/* Experiment
 * KB = 1000
 * MB = 1000 * 1000
 * GB = 1000 * 1000 * 1000
 * Allocated Array = 4 GB
 * STRIDE       | Time(s)
 * 1            | 2e-6
 * 10           | 2e-6
 * 100          | 2e-5
 * 1KB          | .000259
 * 10KB         | .000331
 * 100KB        | .000261
 * 1MB          | .000246
 * 10MB         | .000313
* */
void test_cost_of_page_faulting(){
    cout <<"Allocated 4GB" <<"\n";
    int * a = (int *)malloc(1*GB * sizeof(int));
    cout << "Allocation success" <<"\n";

//    int s = 0;
    int STRIDE = 1*KB;
    int HOPS = 1000;
//    struct element * es = (struct element *)malloc(STRIDE * HOPS * sizeof(struct element));
//
//    cout << "STRIDE "<<STRIDE    <<"\n";
    double start_page = std::clock();
    for(int r=0;r<1000;r++){
    for(int i=0;i<HOPS;i++){
         a[i*STRIDE] ++   ;
    }
    }
    double stop_page = std::clock();
    cout  << (stop_page - start_page)/1000000 << "\n";

}