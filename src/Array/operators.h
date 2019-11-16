//
// Created by sandeep on 5/11/19.
//

#include <ctime>
#include <iostream>

#ifndef V3_OPERATORS_H
#define V3_OPERATORS_H

#endif //V3_OPERATORS_H


void test_division_operator(int noTimes){
    int a = 123456;
    int b = 255;
    double start = std::clock();
    for(int i=0;i<noTimes;i++){
        int c = a/b;
        int d = a%b;
    }
    double end = std::clock();
    std::cout << "Total division_operator time taken" << (end-start)/1000000 <<"\n";
}

void test_power_of_2_operator(int noTimes){
    int a = 123456;
    int b = 256;
    double start = std::clock();
    for(int i=0;i<noTimes;i++){
        int c = a>>4;
        int d = a & 1>>4;
    }
    double end = std::clock();
    std::cout << "Total power_of_2_ time taken" << (end-start)/1000000 <<"\n";

}