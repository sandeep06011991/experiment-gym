//
// Created by sandeep on 4/3/20.
//

#ifndef V3_CHARECTERISTICS_H
#define V3_CHARECTERISTICS_H

#include <graph.h>
#include <cstring>
#include <iostream>


void printGraphCharecteristics(Graph *graph){
    NODE *ndArray = graph->getNodeArray();
    const int BUCKETSIZE = 10;
    const int MAXDEGREE = 200;
    const int NOB = MAXDEGREE/BUCKETSIZE;
    const int V = graph->getNoVertexes();
    NODETYPE *f =(NODETYPE *)malloc((NOB + 1) * sizeof(NODETYPE));
    memset(f, 0, sizeof(NODETYPE) * (NOB + 1));
    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd = ndArray[i];
        int bid = nd.size_plus/BUCKETSIZE;
        if(bid >= NOB)bid = NOB;
        f[bid]++;
    }
    for(int i=0;i<NOB+1;i++){
       std::cout << "Less than " << BUCKETSIZE  * (i + 1)<< ":" <<
                f[i] * 100.0/ V  <<"% \n";
    }
}

// A variant of the shares algorithm.
// Optimize graph reads.
struct tuples{
    NODETYPE id;
    NODETYPE degree;
    NODETYPE new_id;
    NODETYPE partitionId;
} ;

struct triangle_tuple{
   NODETYPE nd1;
   NODETYPE nd2;
   NODETYPE cost;
};

#include <algorithm>
inline int process_batch(struct triangle_tuple *work, int worksize, Graph *graph){
    start_timer(SORTING);
    std::sort(work, work+worksize, [&](triangle_tuple a,triangle_tuple b){
        return a.cost < b.cost;
    });
    stop_timer(SORTING);
    NODE *ndArray = graph->getNodeArray();
    int t = 0;
    NODETYPE *edgeArray = graph->getEdgeArray();
    start_timer(PROCESSING);
    for(int i=0;i<worksize;i++){
        NODE nd1 = ndArray[work[i].nd1];
        NODE nd2 = ndArray[work[i].nd2];
        t = t + naive_intersect(&edgeArray[nd1.offset_plus], nd1.size_plus, &edgeArray[nd2.offset_plus], nd2.size_plus);
    }
    stop_timer(PROCESSING);
    return t;
}

void graphPartition(Graph *graph) {
    const int PARTITIONCAPACITY = 2 * 1000;
//  create partition and measure cost of compute across different categories.
    int totalDegree = 0;
    int currentPartition = 0;
    int capacity = 0;

    NODE *ndArray = graph->getNodeArray();
    int noVertexPerPartition = 0;
    NODETYPE * partitionId = (NODETYPE *)malloc(sizeof(int) * graph->getNoVertexes());
    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd = ndArray[i];
        if(capacity + nd.size > PARTITIONCAPACITY){
            currentPartition ++;
            capacity = 0;
//            cout << noVertexPerPartition <<" ";
            noVertexPerPartition = 0;
        }
        partitionId[i] = currentPartition;
        capacity = capacity + nd.size;
        noVertexPerPartition ++;
    }
    cout << "Total number of parititions " << currentPartition <<"\n";//
    int totalParititons = currentPartition;
//    int * partitionFMap = (int *)malloc(sizeof(int) * totalParititons *totalParititons);
//    memset(partitionFMap, 0, totalParititons * totalParititons);
    NODETYPE * edgeArray = graph->getEdgeArray();
//    for (int i = 0; i < graph->getNoVertexes(); i++) {
//        NODE nd = ndArray[i];
//        for(int j=0;j<nd.size_plus;i++){
//            NODETYPE nd2 = edgeArray[nd.offset_plus + j];
//            partitionFMap[partitionId[nd.id] + totalParititons * partitionId[nd2]] ++;
//        }
//    }

    const int BATCH = 1000000;
    int p = 0;
    int sum = 0;
    struct triangle_tuple * work = (struct triangle_tuple *)malloc(BATCH  * sizeof(triangle_tuple));
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0; i < graph->getNoVertexes();i++){
        NODE  nd = ndArray[i];
        for(int j=0;j<nd.size_plus;j++){
            work[p].nd1 = nd.id;
            work[p].nd2 = edgeArray[nd.offset_plus + j];
            work[p].cost = partitionId[work[p].nd1] + totalParititons * partitionId[work[p].nd2];
            p++;
            if(p==BATCH){
                sum = sum + process_batch(work, p, graph);
                p = 0;
            }
        }
    }
    if(p!=0) sum = sum + process_batch(work, p, graph);
     stop_timer(TOTALNODEPROCESSTIME);

    cout << "total_time" <<get_timer(TOTALNODEPROCESSTIME)<<"\n";
    cout << "total_processing" << get_timer(PROCESSING) <<"\n";
    cout << "sorting time" << get_timer(SORTING) <<"\n";

    int avgDegree;
    avgDegree = totalDegree/graph->getNoVertexes();
    cout << "Triangles " << sum <<"\n";
    cout << "Avg Degree:" << avgDegree <<"\n";
}



#endif //V3_CHARECTERISTICS_H
