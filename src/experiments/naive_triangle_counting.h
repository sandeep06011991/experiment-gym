//
// Created by sandeep on 25/10/19.
//

#include <graph.h>
#include <iostream>
#include <intersection.h>
#include <MinHeap.h>
#include <vector>
#include "timer.h"
#include "Tile.h"

#ifndef V3_NAIVE_TRIANGLE_COUNTING_H
#define V3_NAIVE_TRIANGLE_COUNTING_H

bool isSkewed(NODE a, NODE b){
    int skew = 2;
    if(a.size_plus << skew < b.size_plus)return true;
    if(b.size_plus << skew < a.size_plus)return true;
    return false;
}
/* Experiment Results
 * NOTE: All experiments are performed on reordered graphs.
 * If id1 < id2 => degree(id1) < degree(id2)
 * 1. Description: Triangle counting using a naive intersection without any gallopping
 * using function naive_intersect()
 * ---------------------------
 * Graph Name | RunTime
 * ---------------------------
 * Amazon     |     .103s
 * Youtube    |     .483s
 * LiveJournal|     16s
 *
 *
 * 2. Description: Triangle Counting with gallopping using intersection function
 * binary_intersect();
 * ---------------------------
 * Graph Name | RunTime
 * ---------------------------
 * Amazon     |     .07s
 * Youtube    |     .55s
 * LiveJournal|     19.9s
 * This is an increase in all cases relative to naive intersection.
 *
 * 3. Hybrid Intersection
 * using the function
 * ---------------------------
 * Graph Name | RunTime  | Skew Tolerance
 * ---------------------------
 * Youtube    |     .46s | 3
 * LiveJournal|    14.06s| 4
 * LiveJournal|    14.079s| 4.5
 * LiveJournal|    17.01s| 5
 * Conclusion for my current test sets hybrid algorithms are useless
 *  s = s + hybrid_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
 *                          Cache Misses & Cache References & Time
 *  Just Loading          3.81 M         & 11.89            &
 *  Naive Triangle Count 48.81 M         &  69              & 24
 *  JUST READ GRAPH         52 M         & 77               & 17
 * */

/* Triangle counting using the various intersection implementations */
int triangle_counting(Graph *graph){
    init_metrics();
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    int skip =0;
    int total = 0;
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
//    for(int i=0;i<1000;i++){
        NODE nd1 = ndArray[i];
        NODETYPE* aNeighourArray = &edgeArray[nd1.offset_plus];
        NODETYPE aSize = nd1.size_plus;
//        cout << i <<"\n";
//        if(aSize >10)continue;
        for(int j=0;j<nd1.size_plus;j++){
            NODETYPE bNode = aNeighourArray[j];
            NODE nd2 =  ndArray[bNode];
//            if(nd1.size_plus <50)continue;
//            if(nd2.size_plus <50)continue;

//            if(nd1.size_plus + nd2.size_plus > 0){
//                s =s  + naive_intersect(&edgeArray[nd1.offset_plus], nd1.size_plus, &edgeArray[nd2.offset_plus], nd2.size_plus);
//                continue;
////              TODO: Add a naive executor for the thread.
//            }
//            s = s + naive_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
            s = s + hybrid_intersect(aNeighourArray, aSize, &edgeArray[nd2.offset_plus], nd2.size_plus);
        }
    }

    stop_timer(TOTALNODEPROCESSTIME);
//    print_statistics();
    cout << "Total time taken" << get_timer(TOTALNODEPROCESSTIME) <<"\n";
    cout << "Fraction of nodes skipped:" << skip *1.0 / total  << "\n";
    cout << "triangles found:" <<  s << "\n";
    return s;
}

int measure_cost_of_random_reads(Graph *graph){
    init_metrics();
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    long long total = 0;
//
//    NODETYPE *f = (NODETYPE *)malloc(sizeof(NODETYPE) * graph->getNoVertexes());
//    for(int i=0;i<graph->getNoVertexes();i++) {
//        NODE nd1 = ndArray[i];
//        NODETYPE *aNeighourArray = &edgeArray[nd1.offset_plus];
//        NODETYPE aSize = nd1.size_plus;
//        f[nd1.id]++;
//        for (int j = 0; j < nd1.size_plus; j++) {
//            NODETYPE bNode = aNeighourArray[j];
//            NODE nd2 = ndArray[bNode];
//            f[nd2.id]++;
//        }
//    }
////  Gain with sequential scan
////    cout << "start exp\n";
//    start_timer(TOTALNODEPROCESSTIME);
//    for(int i=0;i<graph->getNoVertexes();i++){
//        NODE nd = ndArray[i];
//        for(int j=0;j<f[nd.id];j++){
//            for(int k=0;k<nd.size_plus;k++){
//                s = edgeArray[nd.offset_plus + k];
//            }
//        }
//    }
//    stop_timer(TOTALNODEPROCESSTIME);

////      Random Reads
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++) {
        NODE nd1 = ndArray[i];
        NODETYPE *aNeighourArray = &edgeArray[nd1.offset_plus];
        NODETYPE aSize = nd1.size_plus;
        for (int j = 0; j < nd1.size_plus; j++) {
            NODETYPE bNode = aNeighourArray[j];
            NODE nd2 = ndArray[bNode];
            for(int k=0;k<nd2.size_plus;k++){
                s = edgeArray[nd2.offset_plus + k];
            }
        }
    }
    stop_timer(TOTALNODEPROCESSTIME);

    cout << "Total time taken for random reads" << get_timer(TOTALNODEPROCESSTIME) <<"\n";
    cout << total <<"\n";
//    cout << "Fraction of nodes skipped:" << skip *1.0 / total  << "\n";
//    cout << "triangles found:" <<  s << "\n";
    return s;
}

int test_impact_tiling(){
//   L1 cache size 256 * 1024 byte
//   Integer to test cache overflow : 64*1024
    int arrsize=8*1024;
    int arrperblock = 8;
    int blocks =24;
    int blocksize = arrperblock * arrsize;
    int s=0;
    int a[arrsize * arrperblock * blocks];
//    for(int i=0;i<blocks;i++){
//        for(int j=0;j<arrperblock;j++){
//            for(int k=0;k<arrperblock;k++){
//                for(int l=0;l<arrsize;l++){
//                    s = s + a[(blocksize)*i+j*arrsize+l] + a[(blocksize)*i+k*arrsize+l];
//                }
//                s=0;
//            }
//        }
//    }
    for(int j=0;j<arrperblock;j++){
        for(int k=0;k<arrperblock;k++){
            for(int i=0;i<blocks;i++){
                for(int l=0;l<arrsize;l++){
                    s = s + a[(blocksize)*i+j*arrsize+l] + a[(blocksize)*i+k*arrsize+l];
                }
                s=0;
            }
        }
    }
}

/* Experiment:
 * Impact of cache missing
 *          # Cache Missing | # Cache references| Time
 * No read:     10 M        | 21 M              | 4.4
 * Read:        52 M        | 78 M              | 9
 * Total Node:  46 M        | 69 M              | 13 s
 * With both arrays: 52, 76
 * tiling array a:
 * */
int cache_aware_intersection(Graph *graph){
    init_metrics();
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    int skip =0;
    int total = 0;
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<100000;i++){
//    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd1 = ndArray[i];
        NODETYPE* aNeighourArray = &edgeArray[nd1.offset_plus];
        NODETYPE aSize = nd1.size_plus;
//      cout << i <<"\n";
//      block size
//        for(int k=0;k<nd1.size_plus;k++){
//                s = aNeighourArray[k];
//            }
        for(int j=0;j<nd1.size_plus;j++){
            NODETYPE bNode = aNeighourArray[j];
            NODE nd2 =  ndArray[bNode];
            NODETYPE* bNeighbourArray = &edgeArray[nd2.offset_plus];
            NODETYPE bSize = nd2.size_plus;
            NODETYPE x=0; NODETYPE y=0;
            for(int k=0;k<bSize;k++){
                s = bNeighbourArray[k];
            }
            for(int k=0;k<nd1.size_plus;k++){
                s = aNeighourArray[k];
            }
//
//            while((x<aSize)&&(y<bSize)){
//                if (aNeighourArray[x] == bNeighbourArray[y]) {
//                    s ++ ;
//                    x ++ ;
//                    y ++ ;
//                    continue;
//                }
//                if (aNeighourArray[x] < bNeighbourArray[y]) {
//                    x ++;
//                } else {
//                    y ++;
//                }
//            }
//          s = s + naive_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
//          s = s + hybrid_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
        }
    }
    stop_timer(TOTALNODEPROCESSTIME);
    print_statistics();
    cout << "Fraction of nodes skipped:" << skip *1.0 / total  << "\n";
    cout << "triangles found:" <<  s << "\n";
    return s;
}

/* Exploration of batched triangle counting */
int batched_triangle_counting(Graph *graph){
    init_metrics();
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    int batchSize = 10;
    MinHeap *heap = new MinHeap(batchSize);
    NODETYPE *bArr[batchSize];
    NODETYPE bSize[batchSize];
    int noB = 0;
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd1 = ndArray[i];
        NODETYPE* aNeighourArray = &edgeArray[nd1.offset_plus];
        NODETYPE aSize = nd1.size_plus;
        int noBatches = aSize/batchSize;
        if(noBatches>0){ noB ++; }
        for(int b = 0; b < noBatches; b++){
            for(int j=0;j< batchSize; j++){
                int curr_b= b*batchSize + j;
                NODETYPE bNode = aNeighourArray[curr_b];
                NODE nd2 =  ndArray[bNode];
                bArr[j] = &edgeArray[nd2.offset_plus];
                bSize[j] = nd2.size_plus;
            }
            s = s + batched_intersection(aNeighourArray, aSize,
                        bArr, bSize, batchSize, heap);
//        NODETYPE* arrA, NODETYPE sizeA,
//                NODETYPE** bArrys, NODETYPE *bsizes, int batchsize, MinHeap * utilityHeap
        }
        int offset = noBatches * batchSize;
        for(int j=0; j < aSize%batchSize; j++){
            int curr_b= offset + j;
            NODETYPE bNode = aNeighourArray[curr_b];
            NODE nd2 =  ndArray[bNode];
            NODETYPE *bNeighbourArray = &edgeArray[nd2.offset_plus];
            NODETYPE bSize = nd2.size_plus;
            s = s + naive_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
        }
    }

    stop_timer(TOTALNODEPROCESSTIME);
    print_statistics();
    cout << "batched triangles found:" <<  s << "\n";
    cout << "No batched nodes" << noB << "\n";
}

/* Finish skeleton of a tiled approach to graph mining. */
/* Approach:
 * Phase 1: working code.
 * Phase 2: small triangle working prototype
 * Phase 3: Large Triangle */

/* Do an experiment of connectedness vs triangle counting
 * Untiled version running at .46s*/
int tiled_triangle_counting(Graph *graph){
//  L1 cache capacity constants
    start_timer(TOTALNODEPROCESSTIME);
    cout << "cuurent graph no. vertex " << graph->getNoVertexes() <<" \n";
    unsigned int l1_cache_capacity = 64 * 1024;
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();

    int taskCount = 0;
    freq_tuple * f_table=(freq_tuple *)malloc(sizeof(freq_tuple) *graph->getNoVertexes());
    vertex_task_offset * vtf = (vertex_task_offset *)malloc(sizeof(vertex_task_offset) * graph->getNoVertexes());
    int vt_counter = 0;
    for(int i=0;i<graph->getNoVertexes();i++){
        f_table[i].id = i;
        f_table[i].f = 0;
        vtf[i].id = i;
        vtf[i].offset = 0;
    }
//    memset(f_table,0, sizeof(freq_tuple) * graph->getNoVertexes());

/*  Populate a list of tasks */
//  FixMe: Tunable parameter.
    start_timer(BITMATRIXCONSTRUCTION);
    int cost = 100;
    int partial= 0;
    int total_savings = 0;
    int total_cost = 100;
//    for(int i=0;i<100000;i++){
    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd1 = ndArray[i];
        f_table[i].id = i;
        vtf[i].id = i;
        for(int j=0;j<nd1.size_plus;j++){
            NODE nd2 = ndArray[edgeArray[nd1.offset_plus+j]];
            if(nd1.size_plus + nd2.size_plus < cost){
                partial = partial + naive_intersect(&edgeArray[nd1.offset_plus], nd1.size_plus, &edgeArray[nd2.offset_plus], nd2.size_plus);
                continue;
//              TODO: Add a naive executor for the thread.
            }
//            cout << nd1.size_plus + nd2.size_plus <<" ";
            taskbuffer[taskCount].id1 = nd1.id;
            taskbuffer[taskCount].id2 = nd2.id;
            taskbuffer[taskCount].isDone = false;
            vertex_task_list[vt_counter].vid = nd1.id;
            vertex_task_list[vt_counter].task_id = taskCount;
            vt_counter++;
            vertex_task_list[vt_counter].vid = nd2.id;
            vertex_task_list[vt_counter].task_id = taskCount;
            vt_counter++;
            taskCount ++;
            f_table[nd1.id].f ++;
            f_table[nd2.id].f ++;

            assert(vt_counter < taskBufferCapacity*2);
            assert(taskCount < taskBufferCapacity);
//            cout << taskCount <<" ";
        }
    }
    cout << "Total tasks being handled are:" << vt_counter <<"\n";
//    cout << "########### DEBUG task table \n";
//     for(int i=0;i<taskCount;i++){
//        cout << i <<" "<< taskbuffer[i].id1 << " " << taskbuffer[i].id2 <<"\n";
//    }
//    cout << "########### DEBUG vertex task table \n";
//    for(int i=0;i<vt_counter;i++){
//        cout << i <<" "<< vertex_task_list[i].vid << " " << vertex_task_list[i].task_id <<"\n";
//    }
/*  sort list by frequency */
    sort(f_table,f_table+graph->getNoVertexes(),[&](freq_tuple a,freq_tuple b){
        if(a.f == b.f) return a.id < b.id;
        return a.f > b.f;
    });
//    cou


    sort(vertex_task_list, vertex_task_list+vt_counter, [&](vertex_task a, vertex_task b){
       return a.vid < b.vid;
    });
//    cout << "############# DEBUG vertex_task \n";
//    for(int i=0;i<vt_counter;i++){
//        cout << vertex_task_list[i].task_id << " " << vertex_task_list[i].vid <<"\n";
//    }

    NODETYPE id = vertex_task_list[0].vid;
    vtf[id].offset = 0;
    for(int i=0;i<vt_counter;i++){

        assert(vertex_task_list[i].vid == taskbuffer[vertex_task_list[i].task_id].id1 ||
               vertex_task_list[i].vid == taskbuffer[vertex_task_list[i].task_id].id2);
        if(id != vertex_task_list[i].vid){
            vtf[vertex_task_list[i].vid].offset = i;
            if(vtf[vertex_task_list[i].vid].id != vertex_task_list[i].vid){
                cout << "error \n";
            }
            assert(vertex_task_list[i].vid == taskbuffer[vertex_task_list[i].task_id].id1 ||
                           vertex_task_list[i].vid == taskbuffer[vertex_task_list[i].task_id].id2);
            id = vertex_task_list[i].vid;
        }
    }
//    cout << "######### DEBUG FTABLE \n";
//    for(int i=0;i<graph->getNoVertexes();i++){
//        cout << f_table[i].id <<" "<< f_table[i].offset <<" "<< f_table[i].f <<"\n";
//    }

    int total_tiles = 0;
    int s = 0;
    cout << "start tiling \n";
//    return 0;
    for(int i=0;i<graph->getNoVertexes();i++){
//        if(i%1000==0)cout << i <<"\n";
        freq_tuple ft = f_table[i];
        vertex_task_offset vt = vtf[ft.id];
        for(int tp = 0; tp <ft.f ; tp ++){
            NODETYPE taskId = vertex_task_list[vt.offset + tp].task_id;
            task *t = &taskbuffer[taskId];
            if(!(t->id1 == ft.id || t->id2 == ft.id)){
                cout << taskId << " " << t->id1 << " " << t->id2 <<" " <<ft.id << "\n";
            }
            assert(t->id1 == ft.id || t->id2 == ft.id);
            if(t->isDone){continue;}
            t->isDone = true;
            NODE nd1 = ndArray[t->id1];
            NODE nd2 = ndArray[t->id2];
            int mincost = 10000000;
            int mintileid = -1;
//            cout << "total tiles " << total_tiles <<"\n";
            for(int j=0;j<total_tiles;j++){
                Tile *tile = &tiles[j];
                if(tile->isTileNearlyFull()){
//                    cout << "overlapp suycces \n";
                    start_timer(ADGLISTINTERSECTION);
                    s = s + tile->process(ndArray, edgeArray);
                    stop_timer(ADGLISTINTERSECTION);
                }
                int cost = tile->costOfAdditionToTile(nd1,nd2);
                if(!tile->doesTileHaveSpace(cost))continue;
                if(cost < mincost){
                    mintileid = j;
                    mincost = cost;
                    if(mincost == 0)break;
                }
            }
//          add to tile
            if(mincost == 0 )total_savings = total_savings + nd1.size_plus + nd2.size_plus;
            total_cost = total_cost + nd1.size_plus + nd2.size_plus;
            if(mintileid != -1){
                tiles[mintileid].addTaskToTile(nd1,nd2,taskId);
            }else{
                total_tiles ++;
                assert(total_tiles<MAXTILENO);;
                tiles[total_tiles-1].addTaskToTile(nd1, nd2,taskId);
            }
        }
    }
    cout << "Total tiles  " << total_tiles <<"\n";
    stop_timer(BITMATRIXCONSTRUCTION);
    start_timer(ADGLISTINTERSECTION);

    for(int i=0;i<total_tiles;i++){
        s  = s + tiles[i].process(ndArray,edgeArray);
        tiles[i].statistics();
//      Naive process
    }
    s  = s+ partial;
    stop_timer(ADGLISTINTERSECTION);
    stop_timer(TOTALNODEPROCESSTIME);
    print_statistics();
    cout << "total savings " << total_savings << " " << total_cost << "=" << total_savings*100.0 /total_cost << "\n";
    cout << "total triangels found" << s <<"\n";
}

struct extend_tuple{
    NODETYPE extend;
    NODETYPE filter;
};

struct filter_tuple{
    NODETYPE extend;
    NODETYPE filter;
    NODETYPE extension_offset;
    NODETYPE extension_size;
};
const int CHUNKSIZE = 1000;

/* Batched execution of extend_intersect to allow larger graphs to be processed using
 * pure extend intersect*/
int extend_intersect_version2(Graph *graph){
    int totalNoChunks = graph->getNoVertexes()/CHUNKSIZE + 1;
//    cout << "Graphs Details:\n" ;
//    cout << "Vertex:" << graph->getNoVertexes() <<"\n";
//    cout << "Edges:" << graph->getNoEdges() <<"\n";
//    cout << "Edges SPLIT OVER Pages" << graph->getNoEdges()/10000 <<"\n";
    cout << "CHUNK SIZE "<< CHUNKSIZE <<"\n";
    cout <<"Total no chunks" << totalNoChunks <<"\n";
     filter_tuple * toFilter[totalNoChunks];
     NODETYPE toFilterSizes[totalNoChunks];
     NODETYPE *extensions[totalNoChunks];
     NODETYPE extSizes[totalNoChunks];
     const int MAX_FILTER_SIZES = 1000000;
     const int MAX_EXT_SIZES = 10000000;
//  Print chunk statistics
    NODETYPE chunkCapacity[totalNoChunks];
    NODE * ndArray = graph->getNodeArray();
//    for(int i=0;i<totalNoChunks;i++){
//        chunkCapacity[i] = 0;
//    }
//    for(int i=0;i<graph->getNoVertexes();i++){
//        int chunkId = ndArray[i].id/CHUNKSIZE;
//        chunkCapacity[chunkId] = chunkCapacity[chunkId] + ndArray[i].size_plus;
//    }
//    cout << "chunk capacity\n";
//    for(int i=0;i<totalNoChunks;i++){
//        cout << chunkCapacity[i] <<" ";
//    }
    for(int i=0;i<totalNoChunks;i++){
        toFilter[i] =  (filter_tuple *)malloc(sizeof(filter_tuple) * MAX_FILTER_SIZES);
        toFilterSizes[i] = 0;
        extensions[i] = (NODETYPE *)malloc(sizeof(NODETYPE) * MAX_EXT_SIZES);
        extSizes[i] = 0;
    }

    NODETYPE * edgeArray = graph->getEdgeArray();

    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);


    int s = 0;
//    int *arr = (int *)malloc(1000*1000*3);
    const int BATCH_SIZE = 100000;
    for(int ii=0;ii<graph->getNoVertexes();ii=ii+BATCH_SIZE) {
        cout << ii <<":" << graph->getNoVertexes()<<"\n";
        int exit = ii + BATCH_SIZE;
        int insert = 0;
        if (exit > graph->getNoVertexes())exit = graph->getNoVertexes();
        start_timer(EXTEND);
        for (int i = ii; i < exit; i++) {
            NODE nd1 = ndArray[i];
            if(nd1.size_plus > 50){
                for(int j=0;j<nd1.size_plus;j++){
                    NODETYPE bNode = edgeArray[nd1.offset_plus + j];
                    NODE nd2 =  ndArray[bNode];
                    s =s  + naive_intersect(&edgeArray[nd1.offset_plus], nd1.size_plus, &edgeArray[nd2.offset_plus], nd2.size_plus);
                }
                continue;
            }
            // By pass for high degree vertexes
            for (int j = 0; j < nd1.size_plus; j++) {
                NODETYPE nd2ID = edgeArray[nd1.offset_plus + j];
                if(ndArray[nd2ID].size_plus > 50){
                    continue;
                }
                struct filter_tuple f;
                f.extend = nd1.id;
                f.filter = nd2ID;
                int chunkID = f.filter / CHUNKSIZE;
                f.extension_offset = extSizes[chunkID];
                f.extension_size = nd1.size_plus - (j + 1);
                for (int k = j + 1; k < nd1.size_plus; k++) {
                    NODETYPE nd3ID = edgeArray[nd1.offset_plus + k];
                    //                arr[insert] = nd3.id;
                    assert(extSizes[chunkID] < MAX_EXT_SIZES);
                    extensions[chunkID][extSizes[chunkID]] = nd3ID;
                    extSizes[chunkID]++;
                    insert ++ ;
                }
                if (f.extension_size != 0) {
                    assert(toFilterSizes[chunkID] < MAX_FILTER_SIZES);
                    toFilter[chunkID][toFilterSizes[chunkID]] = f;
                    toFilterSizes[chunkID]++;
                }
            }
        }
        stop_timer(EXTEND);
//        cout << "Amount of DATA written" << insert << ":" << s << "\n";

        start_timer(INTERSECT);
        for (int i = 0; i < totalNoChunks; i++) {
            filter_tuple *filter = toFilter[i];
            NODETYPE *extensionlist = extensions[i];
            //        sort(filter->begin(),filter->end(),[&](filter_tuple a, filter_tuple b){
            //           if(a.filter == b.filter)return a.extension < b.extension;
            //            return a.filter <b.filter;
            //        });

            for (int l = 0; l < toFilterSizes[i]; l++) {
                filter_tuple f = filter[l];
                NODE nd = ndArray[f.filter];
                int j = 0;
                int k = 0;
                NODETYPE *ndEdgeArray = &edgeArray[nd.offset_plus];
                NODETYPE *extenArray = &extensionlist[f.extension_offset];
                while ((j < nd.size_plus) && (k < f.extension_size)) {
                    if (ndEdgeArray[j] == extenArray[k]) {
                        s++;
                        j++;
                        k++;
                        continue;
                    }
                    if (ndEdgeArray[j] < extenArray[k]) {
                        j++;
                        continue;
                    }
                    if (ndEdgeArray[j] > extenArray[k]) {
                        k++;
                        continue;
                    }

                }

            }
            toFilterSizes[i] = 0;
            extSizes[i] = 0;
        }

        stop_timer(INTERSECT);
    }
        cout << "Extend part" << get_timer(EXTEND) <<"\n";
        cout << "intersection time" <<get_timer(INTERSECT)<<"\n";
        stop_timer(TOTALNODEPROCESSTIME);
        cout << "total processing time" << get_timer(TOTALNODEPROCESSTIME) << "\n";
        cout << "total triangles " << s <<"\n";

    }


/* V1 AMAZON IMPLEMENTATION:
* Naive Implementation: .07
* No optimizations    : .25
* 1 Phase extend and delete: .09
* 2 Phase movement of intersection: .13
* Amazon -> baseline -> .089
* Amazon -> Version 1-> .059
* Percentage = 1.5 x
 * Youtube -> .45
 * Filter   | >100 | >50  | >20 |
 * Extend   | .21  | .22  | .46 |
 * Intersect| .375 | .237 | .09 |
 * Total    | .584 | .458 | .55 |
 * Live Journal - 12.38s baseline 15s
 * Snapshot at 50
* */
int extend_intersect_version(Graph *graph){
    int totalNoChunks = graph->getNoVertexes()/CHUNKSIZE + 1;

    filter_tuple * toFilter[totalNoChunks];
    NODETYPE toFilterSizes[totalNoChunks];
    NODETYPE *extensions[totalNoChunks];
    NODETYPE extSizes[totalNoChunks];
    const int MAX_FILTER_SIZES = 1000000;
    const int MAX_EXT_SIZES = 10000000;
//  Print chunk statistics
    NODETYPE chunkCapacity[totalNoChunks];
    NODE * ndArray = graph->getNodeArray();
//    for(int i=0;i<totalNoChunks;i++){
//        chunkCapacity[i] = 0;
//    }
//    for(int i=0;i<graph->getNoVertexes();i++){
//        int chunkId = ndArray[i].id/CHUNKSIZE;
//        chunkCapacity[chunkId] = chunkCapacity[chunkId] + ndArray[i].size_plus;
//    }
//    cout << "chunk capacity\n";
//    for(int i=0;i<totalNoChunks;i++){
//        cout << chunkCapacity[i] <<" ";
//    }
    for(int i=0;i<totalNoChunks;i++){
        toFilter[i] =  (filter_tuple *)malloc(sizeof(filter_tuple) * MAX_FILTER_SIZES);
        toFilterSizes[i] = 0;
        extensions[i] = (NODETYPE *)malloc(sizeof(NODETYPE) * MAX_EXT_SIZES);
        extSizes[i] = 0;
    }

    NODETYPE * edgeArray = graph->getEdgeArray();

    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);
    start_timer(EXTEND);
    int insert = 0;
//    int *arr = (int *)malloc(1000*1000*3);
    int s = 0;
    for(int i=0;i<graph->getNoVertexes();i++){
//    for(int i=0;i<1000000;i++){
        NODE nd1 = ndArray[i];
        // By pass for high degree vertexes

        for(int j=0;j < nd1.size_plus;j++){

            NODETYPE nd2ID = edgeArray[nd1.offset_plus + j];
            NODE nd2 = ndArray[nd2ID];
            /* The results where snapshotted when size > 50      */
            if((nd2.size_plus > 50) || (nd1.size_plus > 50)){
                s = s + naive_intersect(&edgeArray[nd1.offset_plus],nd1.size_plus,
                        &edgeArray[nd2.offset_plus],nd2.size_plus );
                continue;
            }
            struct filter_tuple f;
            f.extend = nd1.id;
            f.filter = nd2ID;
            int chunkID = f.filter/CHUNKSIZE;
            f.extension_offset = extSizes[chunkID];
            f.extension_size = nd1.size_plus - (j+1);
            for(int k=j+1;k<nd1.size_plus;k++){
                NODETYPE nd3ID = edgeArray[nd1.offset_plus + k];
//                arr[insert] = nd3.id;
                assert(extSizes[chunkID] < MAX_EXT_SIZES);
                extensions[chunkID][extSizes[chunkID]] = nd3ID;
                extSizes[chunkID] ++ ;
            }
            if(f.extension_size !=0){
                assert(toFilterSizes[chunkID] < MAX_FILTER_SIZES);
                toFilter[chunkID][toFilterSizes[chunkID]] = f;
                toFilterSizes[chunkID] ++;
            }
        }
    }
    stop_timer(EXTEND);
    cout << "Amount of DATA written" << insert <<"\n";
    cout << "Extend part" << get_timer(EXTEND) <<"\n";


    start_timer(INTERSECT);
    for(int i=0;i<totalNoChunks;i++){
        filter_tuple * filter = toFilter[i];
        NODETYPE * extensionlist = extensions[i];
//        sort(filter->begin(),filter->end(),[&](filter_tuple a, filter_tuple b){
//           if(a.filter == b.filter)return a.extension < b.extension;
//            return a.filter <b.filter;
//        });

        for(int l=0; l < toFilterSizes[i];l++){
            filter_tuple f = filter[l];
            NODE nd = ndArray[f.filter];
            int j=0; int k=0;
            NODETYPE* ndEdgeArray = &edgeArray[nd.offset_plus];
            NODETYPE* extenArray = &extensionlist[f.extension_offset];
            while((j<nd.size_plus) && (k < f.extension_size)){
                if(ndEdgeArray[j] == extenArray[k]){
                    s++;
                    j++;
                    k++;
                    continue;
                }
                if(ndEdgeArray[j] < extenArray[k]){
                    j++;
                    continue;
                }
                if(ndEdgeArray[j] > extenArray[k]){
                    k++;
                    continue;
                }

            }

        }
    }
    stop_timer(INTERSECT);
    cout << "intersection time" <<get_timer(INTERSECT)<<"\n";
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total processing time" << get_timer(TOTALNODEPROCESSTIME) << "\n";
    cout << "total triangles " << s <<"\n";

}
#endif //V3_NAIVE_TRIANGLE_COUNTING_H
