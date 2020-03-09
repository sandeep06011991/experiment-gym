//
// Created by sandeep on 25/10/19.
//

#ifndef V3_TIMER_H
#define V3_TIMER_H

#endif //V3_TIMER_H

//
// Created by sandeep on 27/8/19.
//

#ifndef V2_TIMER_H
#define V2_TIMER_H

bool const DEBUG = true;


enum TIMERS{
    TOTALNODEPROCESSTIME,
    BITMATRIXCONSTRUCTION,
    REDMATRIXCONSTRUCTION,
    ADGLISTINTERSECTION,
    SIMDINTERSECTION,
    BLACKMATRIXCONSTRUCTION,
    BASECONSTRUCTION,
    INTEGRATEDCONSTRUCTION,
    BINARYSEARCH,
    SORTING,
    PROCESSING,
    BITSIMDINTERSECTIONTIME,
    BITCHARINTERSECTIONTIME,
    EXTEND,
    INTERSECT,
    HIGHSKEW,
    LOWSKEW,
    MAX,
};

enum METRICS{
    V1,
    V2,
    SIMDGALLOPPING,
    END
} ;

void start_timer(TIMERS tname);
void stop_timer(TIMERS tname);
void reset_timer(TIMERS tname);
void print_statistics();
void print_metrics();
void printMemoryUsage();
void init_metrics() ;
void increment_metric(METRICS m);
int get_metric(METRICS m);
double get_timer(TIMERS tname);


bool isSkewed(int sizea,int sizeb,bool isA);

void skew_timer(int sizea, int sizeb,bool isStart);
#endif //V2_TIMER_H
