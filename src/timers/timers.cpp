//
// Created by sandeep on 25/10/19.
//

//
// Created by sandeep on 27/8/19.
//

#include <ctime>
#include <iostream>
#include "timer.h"

using namespace std;

double timers[100];
double save[100];
double metrics[100];


void init_metrics() {
    for (int i = 0; i<100; i++){
        timers[i] = 0;
        save[i] = 0;
        metrics[i] = 0;
    }
}

void reset_timer(TIMERS tname){
    if(DEBUG){
        save[tname] = 0;
    }
}

void start_timer(TIMERS tname){
    if(DEBUG) {
        timers[tname] = std::clock();
    }
}

void stop_timer(TIMERS tname){
    if(DEBUG) {
        timers[tname] = (std::clock() - timers[tname]) / 1000000;
        save[tname] = save[tname] + timers[tname];
    }
}

double get_timer(TIMERS tname){
    return timers[tname];
}

void print_statistics(){
//            BITINTERSECTIONTIME,
//            COSTOFCANONICALITY,
//            MAX,
    for(int i=0;i<TIMERS::MAX;i++){
        switch(i){
            case TIMERS::TOTALNODEPROCESSTIME:cout << "Total Node Process time";
                break;
            case TIMERS::BITMATRIXCONSTRUCTION:cout << "=== BitMatrix Construct";
                break;
            case TIMERS::REDMATRIXCONSTRUCTION:cout << "====== Red Matrix Construct";
                break;
            case TIMERS::BLACKMATRIXCONSTRUCTION:cout << "====== Black Matrix Construct";
                break;
            case TIMERS::BASECONSTRUCTION: cout << "========= Base Construction"; break;
            case TIMERS::BINARYSEARCH: cout << "=========== Binary search";break;
            case TIMERS::ADGLISTINTERSECTION:cout << "========= Adj List Intersection";
                break;
            case TIMERS::SIMDINTERSECTION:cout << "========= SIMDINTERSECTION";
                break;
            case TIMERS::INTEGRATEDCONSTRUCTION:cout << "======= Integrated timers";break;
            case TIMERS::BITSIMDINTERSECTIONTIME:cout << "=== BitSIMDIntersectionTime";
                break;
            case TIMERS::BITCHARINTERSECTIONTIME:cout << "=== BitCharIntersectionTime";
                break;
            case TIMERS::HIGHSKEW:cout << "highskew";break;
            case TIMERS::LOWSKEW:cout << "lowskew";break;


        }
        cout << fixed <<  save[i] << "s\n";
    }
}

void increment_metric(METRICS m){
    metrics[m]++;
}

void print_metrics(){
    for(int i=0;i<METRICS::END;i++) {
        switch (i) {
            case V1:
                cout << "v1 ";
                break;
            case V2:
                cout << "v2";
                break;
            case SIMDGALLOPPING:
                cout << "simdgallop";
        }
        cout << metrics[i] <<"\n";
    }
}

int get_metric(METRICS m){
    return metrics[m];
}

void skew_timer(int sizea, int sizeb,bool isStart){
    int minS = min(sizea,sizeb);
    int maxS = max(sizea, sizeb);
    if(maxS/minS > 50){
//        cout<< maxS/minS <<'\n';
        if(isStart)start_timer(HIGHSKEW);else stop_timer(HIGHSKEW);
    }else{
        if(isStart)start_timer(LOWSKEW);else stop_timer(LOWSKEW);
    }
}

bool isSkewed(int sizea, int sizeb){
    int minS = min(sizea, sizeb);
    int maxS = max(sizea, sizeb);
//    cout << maxS/minS<<"\n";
//    return false;
    if(maxS/minS > 50){
        return true;
    }else{
        return false;
    }
}
bool isSkewed(int sizea, int sizeb,bool isA){
    if(!isSkewed(sizea,sizeb))return false;
    if(isA)return sizea > sizeb;
    return sizeb > sizea;
}
