//
// Created by sandeep on 1/11/19.
//

#include <graph.h>
#include <iostream>
#include <intersection.h>
#include <MinHeap.h>
#include <ArrayUtilities.h>
#include <cstring>
#include <ctime>
#include <vector>
#include "set"
#include <algorithm>
#include "timer.h"

#ifndef V3_SQUARE_COUNTING_H
#define V3_SQUARE_COUNTING_H

#endif //V3_SQUARE_COUNTING_H

NODETYPE naive_vertex_explore(int i, Graph *graph){
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    NODE nd1 = ndArray[i];
    NODETYPE* aNeighourArray = &edgeArray[nd1.offset_plus];
    NODETYPE aSize = nd1.size_plus;
    NODETYPE offset_plus[nd1.size_plus];
    for(int ii=0;ii<nd1.size_plus;ii++){
        NODETYPE bNode = aNeighourArray[ii];
        NODE nd2 =  ndArray[bNode];
        NODETYPE* bNeighbourArray = &edgeArray[nd2.offset];
//          Neighbours of nd2 && greater than nd1
        unsigned int nd2_plus_offset = binarySearchFirstElementGreaterTarget
                (bNeighbourArray,0, nd2.size, nd1.id);
        offset_plus[ii] = nd2_plus_offset;
    }
    for(int j=0; j<nd1.size_plus ;j++){
        NODETYPE bNode = aNeighourArray[j];
        NODE nd2 =  ndArray[bNode];
        NODETYPE* bNeighbourArray = &edgeArray[nd2.offset];
//          Neighbours of nd2 && greater than nd1
        unsigned int nd2_plus_offset = offset_plus[j];
//            bNeighbourArray = &edgeArray[nd2.offset + nd2_plus_offset];
        NODETYPE bSize = nd2.size - nd2_plus_offset;
        for(int k=j+1; k<nd1.size_plus; k++){
            NODETYPE cNode = aNeighourArray[k];
            NODE nd3 =  ndArray[cNode];
            NODETYPE* cNeighbourArray = &edgeArray[nd3.offset];
            //          Neighbours of nd3 && greater than nd1
            unsigned int nd3_plus_offset = offset_plus[k];
//                cNeighbourArray = &edgeArray[nd3.offset + nd3_plus_offset];
            assert(nd3_plus_offset <= nd3.size);
            NODETYPE cSize = nd3.size - nd3_plus_offset;

//            s = s + naive_intersect
//                    ( &cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);
            s = s + hybrid_intersect(&cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);
        }
//            s = s + hybrid_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
    }
    return s;
}


/*  Find all squares in a pattern graph
 *  Youtube Graph
 *  Experiment:         Time(s) | cch-miss(Cr.) | cache-ref | branch | br-miss | Instruc
 *  Naive square pattern:   45s | 8  (45%)      | 18        | 4282   | 167     | 29955
 *  Binary square pattern:  73s | **            | **        | 6402   | 325     | 49019
 *  Hybrid (tol-5)       :  51s | 10 (40%)      | 23        | 3547   | 202     |
 *  Hybrid (tol-10)     :   37s | 9 (42%)       | 21        | 3253   | 187     | 23078
 *  Hybrid (tol-20)     :   41  | 8 (40%)       | 19        | 3307   | 178 (5%)| 23171
 *  pure-simd4          :   35  | 6 (40%)       | 17        | 1139   | 43 (3.8)| 23763
 *  pure-simd8          :   29  | 6 (37%)       | 21        |  869   | 27 (3.2)| 20344
 *  hybrid-simd4 (tol10):   33  | 5 (35%)       | 15        | 1458   | 70      | 20098
 *  hybrid-simd8 (tol10):   27  | 5 (34%)       | 14        | 1266   | 55 (4%) | 17988
 *  LIGHT               :   28  | 5 (31%)       | 18        |  835   | 83 (9%) | 14205
 *  Observations:
 *  1. Binary vs Naive -> More absolute branches and branch misses and instructions,
 *  only impactful when there is a substantial skew. Hybrid captures the best of both.
 *  As expected binary has a speed up only when used with the naive approach
 *
 *  2. Naive vs pure-simd4 vs pure-simd8 . simd should always be faster than binary.
 *  Expect fewer branches, fewer cache-misses, branches and branch-misses.
 *
 *  3. Hybrid vs Hybrid-SIMD4 -> Expect better performance in scenario of high skew.
 *  Minimizing some comparisions.Similar to 2.
 *
 *  4. Light vs SIMD8 -> Matching performance, However increased instructions from index
 *  construction prequery.
 *
 *  Final Notes:
 *  1. Although SIMD promises 4x performance improvement. We are barely coming near that.
 *  Driving home the importance of an efficient encoding.
 *
 *  2. Overall cache-miss rate is 30% and branch miss prediction is 4%.
 *
 *  Note:
 *  It can be observed that simd alone cannot give a speed up. Then how did LIGHT beat emptyheaded,
 *  which uses some encoding. On rechecking the paper I realized they skipped some experiments
 *  and only considered the cases where their idea is applicable. Later they performed all queries
 *  with map-reduce based solution, which would be definately slower due to data shuffling.
 *
 *
 *
 * */
int naive_square_counting(Graph *graph){
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd1 = ndArray[i];
        NODETYPE* aNeighourArray = &edgeArray[nd1.offset_plus];
        NODETYPE aSize = nd1.size_plus;
        NODETYPE offset_plus[nd1.size_plus];
        for(int ii=0;ii<nd1.size_plus;ii++){
            NODETYPE bNode = aNeighourArray[ii];
            NODE nd2 =  ndArray[bNode];
            NODETYPE* bNeighbourArray = &edgeArray[nd2.offset];
//          Neighbours of nd2 && greater than nd1
            unsigned int nd2_plus_offset = binarySearchFirstElementGreaterTarget
                    (bNeighbourArray,0, nd2.size, nd1.id);
            offset_plus[ii] = nd2_plus_offset;
        }
        for(int j=0; j<nd1.size_plus ;j++){
            NODETYPE bNode = aNeighourArray[j];
            NODE nd2 =  ndArray[bNode];
            NODETYPE* bNeighbourArray = &edgeArray[nd2.offset];
//          Neighbours of nd2 && greater than nd1
            unsigned int nd2_plus_offset = offset_plus[j];
//            bNeighbourArray = &edgeArray[nd2.offset + nd2_plus_offset];
            NODETYPE bSize = nd2.size - nd2_plus_offset;
            for(int k=j+1; k<nd1.size_plus; k++){
                NODETYPE cNode = aNeighourArray[k];
                NODE nd3 =  ndArray[cNode];
                NODETYPE* cNeighbourArray = &edgeArray[nd3.offset];
                //          Neighbours of nd3 && greater than nd1
                unsigned int nd3_plus_offset = offset_plus[k];
//                cNeighbourArray = &edgeArray[nd3.offset + nd3_plus_offset];
                assert(nd3_plus_offset <= nd3.size);
                NODETYPE cSize = nd3.size - nd3_plus_offset;

//            s = s + naive_intersect
//                    ( &cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);
//                s = s + binary_intersect(&cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);

//                s = s + hybrid_intersect(&cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);
                s = s + intersectSets8IntegerAttime(&cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);
            }
//            s = s + hybrid_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
        }
    }
    stop_timer(TOTALNODEPROCESSTIME);
//    print_statistics();
    cout << "Naive timer " << get_timer(TOTALNODEPROCESSTIME) <<"\n";
    cout << "Naive exploration square found:" <<  s << "\n";
    return s;
}

/* Square counting using heap unify , performing the union of multiple
 * adjacency lists */
NODETYPE not_naive_vertex_explore(int i, Graph *graph){
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
        NODE nd1 = ndArray[i];
        NODETYPE* nd1NeighourArray = &edgeArray[nd1.offset_plus];
        NODE nd1NeighbourNodes[nd1.size_plus];

//      Heap support structures
        HeapElement array[nd1.size_plus];
        MinHeap heap(array,nd1.size_plus);
        int curr_p[nd1.size_plus];

//        Heap creation
        for(int ii=0;ii<nd1.size_plus;ii++){
            nd1NeighbourNodes[ii] = ndArray[nd1NeighourArray[ii]];
//          offset of neighbours of neiigbours of nd1 greater than nd1
            unsigned int offset = binarySearchFirstElementGreaterTarget
                    (&edgeArray[nd1NeighbourNodes[ii].offset],0, nd1NeighbourNodes[ii].size, nd1.id);
            curr_p[ii] = offset;
            if(offset!=nd1NeighbourNodes[ii].size){
                NODETYPE addNode = edgeArray[nd1NeighbourNodes[ii].offset + offset];
                heap.addElement(addNode, ii);
                curr_p[ii] ++;
            }
        }

//        Heap Unionify
        NODETYPE  lastSeen;
        if(heap.currentSize>0){
            HeapElement prev;
            heap.popElement(&prev);
            lastSeen = prev.value;
            if(curr_p[prev.arrayId] != nd1NeighbourNodes[prev.arrayId].size){
                HeapElement currMin = heap.peekTopElement();
                unsigned int offset = binarySearchFirstElementGreaterOrEqualTarget
                        (&edgeArray[nd1NeighbourNodes[prev.arrayId].offset],curr_p[prev.arrayId],
                         nd1NeighbourNodes[prev.arrayId].size, currMin.value);
                curr_p[prev.arrayId] = offset;
                if(offset != nd1NeighbourNodes[prev.arrayId].size){
                    heap.addElement(edgeArray[nd1NeighbourNodes[prev.arrayId].offset + curr_p[prev.arrayId]], prev.arrayId);
                    curr_p[prev.arrayId] ++;
                }
            }
        }
        int g_count = 0;
        int l_count = 1;
        while(heap.currentSize > 0){
            HeapElement prev;
            heap.popElement(&prev);
            if(curr_p[prev.arrayId] != nd1NeighbourNodes[prev.arrayId].size){
                heap.addElement(edgeArray[nd1NeighbourNodes[prev.arrayId].offset + curr_p[prev.arrayId]], prev.arrayId);
                curr_p[prev.arrayId] ++;
            }
            if(prev.value == lastSeen)l_count++;

            if(prev.value != lastSeen){
                if(l_count >= 2){
                    g_count = g_count + ((l_count*(l_count-1))>>1);
                }
                l_count = 1;
                lastSeen = prev.value;
            }
        }
        if(l_count >= 2){
            g_count = g_count + ((l_count*(l_count-1))>>1);
        }
        s = s + g_count;

    return s;
}

int not_naive_square(Graph *graph){
    init_metrics();
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
        s = s + not_naive_vertex_explore(i,graph);
    }
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "min heap total time " << get_timer(TOTALNODEPROCESSTIME) <<"\n";
    cout << "Min Heap based:" <<  s << "\n";
    return s;

}

/*Very important experiment log this. */
void compare_naive_vs_not_naive(Graph *graph){
    init_metrics();
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    int s = 0;
    int eff = 0;
    int ineff = 0;
    int sd_eff = 0;
    int sd_ineff = 0;
//  Measure average degree
    for(int i=0;i<graph->getNoVertexes();i++){
//    for(int i=6316;i<6317;i++){
        NODE nd = ndArray[i];
        double s_n1 = std::clock();
        naive_vertex_explore(i,graph);
        double e_n1 = std::clock();
//        double s_n2 = std::clock();
//        not_naive_vertex_explore(i,graph);
//        double e_n2 = std::clock();
//        double naive = e_n1 - s_n1;
//        double not_naive = e_n2 - s_n2;
//        if(not_naive <  .1 * naive){
////            cout << i << "\n";
////            cout << "Time:" << naive << " Not naive: " << not_naive << "\n";
//            sd_eff = sd_eff + ndArray[i].size_plus;
//            eff ++ ;
//        }else{
//            sd_ineff = sd_ineff + ndArray[i].size_plus;
//            ineff ++ ;
//        }

    }
    cout << "Not naive better:" << eff << " Naive better:" << ineff <<" \n";
//    cout << "average degree not naive" << (sd_eff*1.0)/eff <<" \n";
//    cout << "average degree naive" << (sd_ineff*1.0)/ineff <<" \n";
}

void hybrid_square(Graph *graph,int skew){
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    //  Measure average degree
    int s = 0;
    init_metrics();
    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd= ndArray[i];
        if(nd.size_plus > skew){
            s = s + not_naive_vertex_explore(i,graph);
        }else{
            s = s + naive_vertex_explore(i,graph);
        }
    }
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total hybrid time " << get_timer(TOTALNODEPROCESSTIME) <<"\n";
    cout << "skew:" << skew << " results" << s <<"\n";
}

struct intersect_pair{
    NODETYPE a;
    NODETYPE b;
    NODETYPE source;
};

bool comp_intersect_pair (intersect_pair a,intersect_pair b) {
    if(a.a != b.a) return a.a < b.a;
    if(a.b != b.b) return a.b < b.b;
    return a.source < b.source;
}

bool eq_intersect_pair (intersect_pair a,intersect_pair b) {
    return (a.a == b.a) && (a.b == b.b) ;
}

/* Do not use external library such as malloc unless absolutely required. */
/* Bit Vector square exploration */
void bit_vector_based(Graph *graph){\
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    //  Measure average degree
    init_metrics();
    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);
    NODETYPE a[1000000];
    NODETYPE b[1000000];

    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd0= ndArray[i];

//      calculate required offsets
        NODETYPE offsets[nd0.size_plus];
        for(int j=0;j<nd0.size_plus;j++){
                NODE nd1= ndArray[edgeArray[nd0.offset_plus+j]];
                offsets[j] = binarySearchFirstElementGreaterTarget(&edgeArray[nd1.offset],0,nd1.size,nd0.id);
        }

//      Union all elements.
//      copy one neighbourhood into the base array
        NODE tNode = ndArray[edgeArray[nd0.offset_plus]];
        int t = 0 ;
        for(int k=offsets[0]; k<tNode.size; k++){
            a[t] = edgeArray[tNode.offset + k];
            t ++ ;
        }

//      Union over all other elements.
        NODETYPE *base = a;
        int baseSize = t;
        NODETYPE *temp = b;
        for(int j=1;j<nd0.size_plus;j++){
            NODE cNode = ndArray[edgeArray[nd0.offset_plus+j]];
            int bP = 0;
            t = 0;
            int nodeP = offsets[j];
            while((bP < baseSize) && (nodeP < cNode.size)){
                if(base[bP] == edgeArray[nodeP]){
                    bP ++;
                    nodeP ++;
                    temp[t] = base[bP];
                    t++;
                    continue;
                }
                if(base[bP] < edgeArray[nodeP]){
                    temp[t] = base[bP];
                    t++;
                    bP ++;
                }else{
                    temp[t] = edgeArray[nodeP];
                    t++;
                    nodeP ++;
                }
            }
            while(bP < baseSize){
                temp[t] = base[bP];
                t++;
                bP ++;
            }
            while(nodeP<cNode.size){
                temp[t] = edgeArray[nodeP];
                t++;
                nodeP ++;
            }
            NODETYPE *bck = base;
            base = temp;
            temp = bck;
            baseSize = t;
//          swap temp and base in the end
        }
//        for(int j=0;j<baseSize;j++){
//            cout << base[j] << " " ;
//        }
//        cout <<"\n";
//      Compute of n Arrays, compute intersection as well.
        for(int j=0;j<nd0.size_plus;j++){
            NODE cNode = ndArray[edgeArray[nd0.offset_plus+j]];
            hybrid_intersect(base,baseSize,&edgeArray[cNode.offset+offsets[j]],cNode.size - offsets[j]);
        }
        int bitVsize = baseSize/32;
        int s = 0;

//      Use these bit vectors to compute
        for(int j=0;j<nd0.size_plus;j++){
            for(int k=j+1;k<nd0.size_plus;k++){
                for(int t=0;t<bitVsize;t++){
                    s = 12 & 1234;
                }
            }
        }


    }
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total base array time " << get_timer(TOTALNODEPROCESSTIME) <<"\n";

}


/* Idea2 : Query push down with sort*/
void sort_square(Graph *graph){
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    //  Measure average degree
    init_metrics();
    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);
    vector<intersect_pair> *v = new vector<intersect_pair>;
//    v->reserve(1000000);
//    v->push_back((intersect_pair) {1,2});
//    v->push_back((intersect_pair) {3,4});
//    v->push_back((intersect_pair) {1,2});

    int count=0;
    for(int i=0;i<graph->getNoVertexes();i++){
        NODE nd= ndArray[i];
        for(int j=0;j<nd.size_plus;j++){
            for(int k=j+1;k<nd.size_plus;k++){
                struct intersect_pair tt;
                tt.a = edgeArray[nd.offset_plus + j];
                tt.b = edgeArray[nd.offset_plus + k];
                tt.source = nd.id;
                v->push_back(tt);
                count++;
            }
        }
    }
    sort(v->begin(),v->end(),comp_intersect_pair);
    // using predicate comparison:

    cout << "vector size "<< v->size()<<"\n";
//    vector<intersect_pair>::iterator ip;
//    double o = v->size();
//    ip = std::unique (v->begin(), v->end(), eq_intersect_pair);   // (no changes)
//    v->resize(std::distance(v->begin(),ip));
//    cout << "after duplicates removed "<< v->size()<<"\n";
//    // print out content:
//    std::cout << "fraction remaining" << v->size()/o <<"\n";
    int s = 0;
    intersect_pair prev;
    for (auto it=v->begin(); it!=v->end(); ++it){
        intersect_pair t = *it;
        NODETYPE n1 = t.a;
        NODETYPE n2 = t.b;
        NODE nd1 = ndArray[n1];
        NODE nd2 = ndArray[n2];
        int ss = 0;
        if(!eq_intersect_pair(prev,t))int ss = hybrid_intersect(&edgeArray[nd1.offset],nd1.size,&edgeArray[nd2.offset],nd2.size);
        prev = t;
        s = s + ss ;
    }

//  std::cout << "sum found :" << s << '\n';
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total hybrid time " << get_timer(TOTALNODEPROCESSTIME) <<"\n";
}