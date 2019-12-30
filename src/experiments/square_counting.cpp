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
#include <BitDictionary.h>
#include <bitset>
#include <BitVectorUtil.h>
#include "immintrin.h"
#include <cmath>
#include <x86intrin.h>
#include "timer.h"

#ifndef V3_SQUARE_COUNTING_H
#define V3_SQUARE_COUNTING_H

#endif //V3_SQUARE_COUNTING_H

/* Explore seed vertex: i using the naive tecnhique.
 * This section is used to identify if for any vertexes different approaches are more effective.
 * However results where not anyway conclusive.
 * */
NODETYPE naive_vertex_explore(int i, Graph *graph){
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    long long s = 0;
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
 *  3. Skipping skewed instructions only saves only 7seconds.
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
    long s = 0;
    int t=0;
    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
//    for(int i=460000;i<460050;i++){// youtube has 470165
        if(i%100000==0){
            cout << i << ":" << graph->getNoVertexes() <<"\n";
        }

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

                s = s + hybrid_intersect(&cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);
//                s = s + intersectSets8IntegerAttime(&cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);
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

/* Infrastructure to explore different vertex exploration schemes.
 * Again Square counting using heap unify , performing the union of multiple
 * adjacency lists. I tried heap unify which was mostly bad.  */
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
/* Performance much worse : 60s > */
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

/* Measure different ways of exploring seed vertex.
 * Naive was always the most effective. */
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
        double s_n2 = std::clock();
        not_naive_vertex_explore(i,graph);
        double e_n2 = std::clock();
        double naive = e_n1 - s_n1;
        double not_naive = e_n2 - s_n2;
        if(not_naive <  .1 * naive){
//            cout << i << "\n";
//            cout << "Time:" << naive << " Not naive: " << not_naive << "\n";
            sd_eff = sd_eff + ndArray[i].size_plus;
            eff ++ ;
        }else{
            sd_ineff = sd_ineff + ndArray[i].size_plus;
            ineff ++ ;
        }

    }
    cout << "Not naive better:" << eff << " Naive better:" << ineff <<" \n";
//    cout << "average degree not naive" << (sd_eff*1.0)/eff <<" \n";
//    cout << "average degree naive" << (sd_ineff*1.0)/ineff <<" \n";
}

// Min heap is a bad idea. Overhead of insertion is way too much.
// For no vertexes was the not naive approach working.
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
/* Bit Vector square exploration
 * Function to explore different sub components such as
 * Base array construction, bitVector construction and intersection cost.
 * IMPORTANT: DONOT change this structure.
 * Experiment with different kinds of base constructions, bitconstructions and intersection const.
 * DONOT Evaluate hybrid methods
 * Current best simd vs non-simd range : 25-35s
 * Profile
 * Experiment:[ref     Time(s) | cch-miss(Cr.) | cache-ref | branch | br-miss | Instruc
 * Hybrid (tol-10)     :   37s | 9 (42%)       | 21        | 3253   | 187     | 23078
 * Naive square pattern:   45s | 8  (45%)      | 18        | 4282   | 167     | 29955
 * Bit Vector          :   65s | 6 (21%)       | 28        | 6407   | 104     | 54177
   OBSERVATIONS.
   Experiment:         BaseArray | BitVector | BitProcess | TotalTime
    Naive base unique     14.4   | 23        | 3.96       | 40.5
    Naive base duplicate  19.7   | 13        | 1.60       | 33.55
    Min Heap              50.56  | 15        | 1.81       | 66.21
    Infrafor bitmat       22.9   | 15        | 1.74       | 38
    RecursiveUnion        18     | 21        | 1.95       | 43s

   1. BitVector processing time is a small fraction of total time less than 5seconds, total time of 2/44s.
  Cost of construction is too high. using the naive base array construction, time taken for base array construction
  is 14.4, BitVector - 21.55 and BitMat process - 3.96
  STATUS -> Matching .. NON-SIMD.

   2. Naive approach works for the better invariably in almost all cases.

 * */
void bit_vector_based(Graph *graph){
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    //  Measure average degree
    init_metrics();
    BitDictionary *bitDictionary = new BitDictionary();
//    bitDictionary->visualize();
    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);
    NODETYPE a[1000000];
    NODETYPE b[1000000];
    NODETYPE baseArray[maxArraySize];
    long res = 0;

//    for(int i=0;i<10;i++){
    for(int i=0;i<graph->getNoVertexes();i++){
        if(i%10000==0){
            cout << i << ":" << graph->getNoVertexes() <<"\n";
        }
        NODE nd0= ndArray[i];
        if(nd0.size_plus < 2) continue;
//      calculate required offsets
        NODETYPE* arrays[nd0.size_plus];
        NODETYPE arrsize[nd0.size_plus];
        NODETYPE offsets[nd0.size_plus];
        NODETYPE avgSize=0;
        for(int j=0;j<nd0.size_plus;j++){
                NODE nd1= ndArray[edgeArray[nd0.offset_plus+j]];
                offsets[j] = binarySearchFirstElementGreaterTarget(&edgeArray[nd1.offset],0,nd1.size,nd0.id);
                arrays[j] = &edgeArray[nd1.offset + offsets[j]];
                arrsize[j] = nd1.size - offsets[j];
                avgSize += arrsize[j];
        }
        avgSize = avgSize / nd0.size_plus;


//      Time till here  < 1s
        NODETYPE baseSize;
        NODETYPE *base;
//      Union all elements.
//      copy one neighbourhood into the base array
//        if(nd0.size_plus < 10)continue;
//        start_timer(BITMATRIXCONSTRUCTION);
//        baseSize = computeNaiveBaseArrayWithDuplicates(arrays, arrsize, nd0.size_plus,baseArray);
//             baseSize = usingTrivialSort(arrays, arrsize, nd0.size_plus,baseArray);
            baseSize =     sortNeighbourhood(arrays, arrsize, nd0.size_plus,baseArray);
 //            baseSize = computeNaiveBaseArrayAndReturnSize(arrays, arrsize, nd0.size_plus,baseArray);
//            baseSize = bloomfilter_based_baseArray(arrays, arrsize, nd0.size_plus,baseArray);
//            baseSize = computeBaseArrayWithMinHeap(arrays, arrsize, nd0.size_plus,baseArray);
//            baseSize = computeBaseArrayAndBitMatrixReturnSize(arrays, arrsize, nd0.size_plus,baseArray);
//            baseSize = computeNaiveBaseArrayAndReturnSize(arraysLarge,arrsizeLarge,t,baseArray);
//            baseSize = computeBaseArrayUsingBinaryRecursion(arrays, arrsize, nd0.size_plus,baseArray);
//              baseSize = computeNaiveBaseArraySkew(arrays, arrsize, nd0.size_plus,baseArray);
        base = baseArray;
        continue;
//      Compute of n Arrays, compute intersection with bit array.
//      size in char but rounded to int to allow next loop
//      Time till here is 7s
        int charbitVectorsize = ((baseSize + 32 )/32 * 4);
        unsigned char bitArray[charbitVectorsize * nd0.size_plus];
        memset(bitArray,0,sizeof(unsigned char) * charbitVectorsize * nd0.size_plus );
//        start_timer(BITSIMDINTERSECTIONTIME);
        for(int j=0;j<nd0.size_plus;j++){
            NODE cNode = ndArray[edgeArray[nd0.offset_plus+j]];
            hybridBitMark_intersect(base, baseSize, &edgeArray[cNode.offset+offsets[j]],
                    cNode.size - offsets[j], &bitArray[charbitVectorsize * j]);
//          if(cNode.size - offsets[j]<10)continue;
//          hybrid_intersect(base,baseSize,&edgeArray[cNode.offset+offsets[j]],cNode.size - offsets[j]);
        }
//        stop_timer(BITSIMDINTERSECTIONTIME);
//        continue;
//        #####################################################
//        cout << " #### Bit Matrix debug ### \n";
//        for(int ii=0;ii < nd0.size_plus;ii++){
//            NODE t = ndArray[edgeArray[nd0.offset_plus + ii]];
//            cout << t.id << ": ";
//            for(int j=offsets[ii]; j <t.size;j++){
//                cout << edgeArray[t.offset + j] <<" ";
//            }
//            cout <<"\n";
//
//            for(int j=0;j<charbitVectorsize;j++){
//                cout <<bitset<8>(bitArray[charbitVectorsize *ii +j]);
//            }
//            cout <<"\n";
//        }
//      #############################################################
        int s = 0;
//
/*      Experiment : For a youtube graph
 *      Using char and dictionary : 22.8
 *      Using int : .9*/

//        start_timer(BITCHARINTERSECTIONTIME);
//        for(int j=0;j<nd0.size_plus;j++){
//            int aoffset = charbitVectorsize * j;
//            for(int k=j+1;k<nd0.size_plus;k++){
//                int boffset = charbitVectorsize * k;
//                for(int t=0;t<charbitVectorsize;t++){
//                   unsigned char x = bitArray[aoffset + t] & bitArray[boffset + t];
//                   res = res + bitDictionary->countSetBits(x);
//                }
//            }
//        }
//      USing integer.
        for(int j=0;j<nd0.size_plus;j++){
            int aoffset = charbitVectorsize * j;
            unsigned int *ap = (unsigned int *)&bitArray[aoffset];
            for(int k=j+1;k<nd0.size_plus;k++){
                int boffset = charbitVectorsize * k;
                unsigned int *bp = (unsigned int *)&bitArray[boffset];
                for(int t=0;t<charbitVectorsize/4;t=t+1){
                      res = res + _popcnt32(bp[t] & ap[t]);
//                    unsigned char x = bitArray[aoffset + t] & bitArray[boffset + t];
//                    res = res + bitDictionary->countSetBits(x);
                }
            }
        }
//      Using SIMD Integer.
//        stop_timer(BITCHARINTERSECTIONTIME);
    }
    cout << "total squares found:" << res << "\n";
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total base array time " << get_timer(TOTALNODEPROCESSTIME) <<"\n";
    cout << "BitMatrix timer" << get_timer(BITMATRIXCONSTRUCTION) <<"\n";
    cout << "Hybrid timer" << get_timer(BITSIMDINTERSECTIONTIME) <<"\n";
    cout << "Bit char timer "<< get_timer(BITCHARINTERSECTIONTIME) <<"\n";
}

/* Idea experiment:
 * Have multiple bit matrixes,
 * one small and one big.
 * This experiment also failed with total performance much worse than naive.*/
void dual_bit_matrix_based(Graph *graph){
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    //  Measure average degree
    init_metrics();
    BitDictionary *bitDictionary = new BitDictionary();
//    bitDictionary->visualize();
    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);
//    NODETYPE a[1000000];
//    NODETYPE b[1000000];
    NODETYPE baseArraySmall[maxArraySize];
    NODETYPE baseArrayLarge[maxArraySize];

    long res = 0;

    for(int i=460000;i<460050;i++){
//    for(int i=450000;i<graph->getNoVertexes();i++){
        if(i%1000==0){
            cout << i << ":" << graph->getNoVertexes() <<"\n";
        }

        NODE nd0= ndArray[i];
        if(nd0.size_plus == 0) continue;
//      calculate required offsets
        NODETYPE* arrays[nd0.size_plus];
        NODETYPE arrsize[nd0.size_plus];
        NODETYPE offsets[nd0.size_plus];
        NODETYPE avgSize=0;
        for(int j=0;j<nd0.size_plus;j++){
            NODE nd1= ndArray[edgeArray[nd0.offset_plus+j]];
            offsets[j] = binarySearchFirstElementGreaterTarget(&edgeArray[nd1.offset],0,nd1.size,nd0.id);
            arrays[j] = &edgeArray[nd1.offset + offsets[j]];
            arrsize[j] = nd1.size - offsets[j];
            avgSize += arrsize[j];
        }

        avgSize = avgSize / nd0.size_plus;
        bool isLarge[nd0.size_plus];
        bool alt_index[nd0.size_plus];

        NODETYPE* arraysLarge[nd0.size_plus];
        NODETYPE arrsizeLarge[nd0.size_plus];

        NODETYPE* arraysSmall[nd0.size_plus];
        NODETYPE arrsizeSmall[nd0.size_plus];
        int l_nodes = 0;
        int s_nodes= 0;
//        cout << "sizes:";
        for(int j=0;j<nd0.size_plus;j++){
//            cout << arrsize[j] <<" ";
            if((arrsize[j] < 800) && (arrsize[j] > 100)){
                arraysLarge[l_nodes] = arrays[j];
                arrsizeLarge[l_nodes] = arrsize[j];
                isLarge[j] = true;
                alt_index[j] = l_nodes;
                l_nodes++;
            }else{
                arraysSmall[s_nodes] = arrays[j];
                arrsizeSmall[s_nodes] = arrsize[j];
                isLarge[j] = false;
                alt_index[j] = false;
                s_nodes++;
            }
        }
//        cout << "\n";
//      Time till here  < 1s
//        NODETYPE baseSize;
//        NODETYPE *base;
//        continue;
//      Union all elements.
//        start_timer(BITMATRIXCONSTRUCTION);
        /* Naive can compute these 2 vertex in < e-06, best case is 1.5e-5 by the it comes here.
         * */
//        if((nd0.size_plus < 100) ){
//            baseSize = computeBaseArrayWithMinHeap(arrays, arrsize, nd0.size_plus,baseArray);
//      .09
//        NODETYPE baseSizeLarge = computeBaseArrayWithMinHeap(arraysLarge, arrsizeLarge,l_nodes, baseArrayLarge);
//            baseSize = computeBaseArrayAndBitMatrixReturnSize(arrays, arrsize, nd0.size_plus,baseArray);

        NODETYPE baseSizeLarge = computeNaiveBaseArrayAndReturnSize(arraysLarge,arrsizeLarge,l_nodes,baseArrayLarge);

        //        NODETYPE baseSizeSmall = computeNaiveBaseArrayAndReturnSize(arraysSmall,arrsizeSmall,s_nodes,baseArraySmall);

//        NODETYPE baseSizeLarge = computeBaseArrayAndBitMatrixReturnSize(arraysLarge,arrsizeLarge,l_nodes,baseArrayLarge);
//        NODETYPE baseSizeSmall = computeBaseArrayAndBitMatrixReturnSize(arraysSmall,arrsizeSmall,s_nodes,baseArraySmall);
//        if(baseSizeLarge>1000){
//            cout << baseSizeLarge << " " << baseSizeSmall <<"\n";
//        }
        //        }

        NODETYPE  baseSizeSmall = 0;
//        BitMatrix Small Timer: 12.6s
//        continue;
//        stop_timer(BITMATRIXCONSTRUCTION);

//        base = baseArray;
//        if(baseSize > 1000){
//            cout <<"baseSize" <<  i << ":" << baseSize <<"\n";
//        }
        //      Compute of n Arrays, compute intersection with bit array.
//      size in char but rounded to int to allow next loop
//      Time till here is 7s
        int smallcharbitVectorsize = ((baseSizeSmall + 32 )/32 * 4);
        unsigned char bitArraysmall[smallcharbitVectorsize * s_nodes];
//        memset(bitArraysmall,0,sizeof(unsigned char) * smallcharbitVectorsize * s_nodes );
        int largecharbitVectorsize = ((baseSizeLarge + 32 )/32 * 4);
        unsigned char bitArrayLarge[largecharbitVectorsize * l_nodes];
        memset(bitArrayLarge,0,sizeof(unsigned char) * largecharbitVectorsize * l_nodes );

//        start_timer(BITSIMDINTERSECTIONTIME);
        for(int j=0;j<nd0.size_plus;j++){
            NODE cNode = ndArray[edgeArray[nd0.offset_plus+j]];
            if(isLarge[j]){
                hybridBitMark_intersect(baseArrayLarge, baseSizeLarge, &edgeArray[cNode.offset+offsets[j]],
                                        cNode.size - offsets[j],
                                        &bitArrayLarge[largecharbitVectorsize * alt_index[j]]);
            }else{
//                hybridBitMark_intersect(baseArraySmall, baseSizeSmall, &edgeArray[cNode.offset+offsets[j]],
//                                        cNode.size - offsets[j],
//                                        &bitArraysmall[smallcharbitVectorsize * alt_index[j]]);

            }
//          if(cNode.size - offsets[j]<10)continue;
//          hybrid_intersect(base,baseSize,&edgeArray[cNode.offset+offsets[j]],cNode.size - offsets[j]);
        }
//        stop_timer(BITSIMDINTERSECTIONTIME);
        continue;
//        #####################################################
//        cout << " #### Bit Matrix debug ### \n";
//        for(int ii=0;ii < nd0.size_plus;ii++){
//            NODE t = ndArray[edgeArray[nd0.offset_plus + ii]];
//            cout << t.id << ": ";
//            for(int j=offsets[ii]; j <t.size;j++){
//                cout << edgeArray[t.offset + j] <<" ";
//            }
//            cout <<"\n";
//
//            for(int j=0;j<charbitVectorsize;j++){
//                cout <<bitset<8>(bitArray[charbitVectorsize *ii +j]);
//            }
//            cout <<"\n";
//        }
//      #############################################################
         int s = 0;
//
/*      Experiment : For a youtube graph
 *      Using char and dictionary : 22.8
 *      Using int : .9*/

//        start_timer(BITCHARINTERSECTIONTIME);
//        for(int j=0;j<nd0.size_plus;j++){
//            int aoffset = charbitVectorsize * j;
//            for(int k=j+1;k<nd0.size_plus;k++){
//                int boffset = charbitVectorsize * k;
//                for(int t=0;t<charbitVectorsize;t++){
//                   unsigned char x = bitArray[aoffset + t] & bitArray[boffset + t];
//                   res = res + bitDictionary->countSetBits(x);
//                }
//            }
//        }
//      USing integer.
        int aoffset; int boffset;
        unsigned int *ap;
        unsigned int *bp;
        for(int j=0;j<nd0.size_plus;j++){
            if(isLarge[j]){
                aoffset = largecharbitVectorsize * alt_index[j];
                ap = (unsigned int *)&bitArrayLarge[aoffset];
            }else{
                aoffset = smallcharbitVectorsize * alt_index[j];
                ap = (unsigned int *)& bitArraysmall[aoffset];
            }
            for(int k=j+1;k<nd0.size_plus;k++){
                if(isLarge[k]){
                    boffset = largecharbitVectorsize * alt_index[k];
                    bp = (unsigned int *)&bitArrayLarge[boffset];
                }else{
                    boffset = smallcharbitVectorsize * alt_index[k];
                    bp = (unsigned int *)& bitArraysmall[boffset];
                }
                if((isLarge[j])&&(isLarge[k])){
//               Saves .005s
                    for(int t=0;t<largecharbitVectorsize/4;t=t+1){
                        res = res + _popcnt32(bp[t] & ap[t]);

//                    unsigned char x = bitArray[aoffset + t] & bitArray[boffset + t];
//                    res = res + bitDictionary->countSetBits(x);
                    }
                    continue;
                }
//                if((!isLarge[j])&&(!isLarge[k])){
//                    for(int t=0;t<smallcharbitVectorsize/4;t=t+1){
//                        res = res + _popcnt32(bp[t] & ap[t]);
//
////                    unsigned char x = bitArray[aoffset + t] & bitArray[boffset + t];
////                    res = res + bitDictionary->countSetBits(x);
//                    }
//                    continue;
//                }
                res = res + hybrid_intersect(arrays[j],arrsize[j],arrays[k],arrsize[k]);
            }
        }
//      Using SIMD Integer.
//        stop_timer(BITCHARINTERSECTIONTIME);
    }
    cout << "total squares found:" << res << "\n";
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total base array time " << get_timer(TOTALNODEPROCESSTIME) <<"\n";
    cout << "BitMatrix timer" << get_timer(BITMATRIXCONSTRUCTION) <<"\n";
    cout << "Hybrid timer" << get_timer(BITSIMDINTERSECTIONTIME) <<"\n";
    cout << "Bit char timer "<< get_timer(BITCHARINTERSECTIONTIME) <<"\n";
}

/* Move this to newxt function: Query push down with sort*/
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


/*  Infrastructure to evaluate
 *  Question: For how many vertexes is the naive bitmatrix approach better.
 *  Experiment:
 *  Naive BitVertex is better for : 11269
 *  Min Heap is better for :         7478
 *  Naive is better for :          170976
 *  Conclusion
 *  This approach works better for less than 5% of the vertexes.
 *  What is the total saving in time by using all of these fancy methods
 *  This experiment results where not repeatable as the cost of measuring time for one vertex
 *  is more than the cost of computation.
 *  5% are heavy hitters and could be much more important .
 *  However isolating such vertexes was not successfull as time measurement had to much interference
 *  from system calls.
 *  */
void hybrid_bit_vector_based(Graph *graph){
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    init_metrics();
    BitDictionary *bitDictionary = new BitDictionary();
    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);
    NODETYPE a[1000000];
    NODETYPE b[1000000];
    NODETYPE baseArray[maxArraySize];
    double naive_timer;
    int naive_v=0;
    double min_heap_timer;
    int min_heap_v = 0;
    double naive_bit_timer;
    int naive_bit_v = 0;
    long res = 0;
    double naive_total = 0;
    double current_min = 0;
    double min_total = 0;
//    for(int i=502;i<503;i++) {
    for(int i=0;i<graph->getNoVertexes();i++){
        if (i % 100000 == 0) {
//            cout << i << ":" << graph->getNoVertexes() << "\n";
//            stop_timer(TOTALNODEPROCESSTIME);
//            cout << "total base array time " << get_timer(TOTALNODEPROCESSTIME) <<"\n";
//            reset_timer(TOTALNODEPROCESSTIME);
//            start_timer(TOTALNODEPROCESSTIME);
        }
        NODE nd0 = ndArray[i];
        if (nd0.size_plus == 0) continue;
//      calculate required offsets
        NODETYPE *arrays[nd0.size_plus];
        NODETYPE arrsize[nd0.size_plus];
        NODETYPE offsets[nd0.size_plus];
        NODETYPE avgSize = 0;
        for (int j = 0; j < nd0.size_plus; j++) {
            NODE nd1 = ndArray[edgeArray[nd0.offset_plus + j]];
            offsets[j] = binarySearchFirstElementGreaterTarget(&edgeArray[nd1.offset], 0, nd1.size, nd0.id);
            arrays[j] = &edgeArray[nd1.offset + offsets[j]];
            arrsize[j] = nd1.size - offsets[j];
            avgSize += arrsize[j];
        }
        avgSize = avgSize / nd0.size_plus;
//        cout << nd0.size_plus << ":" << avgSize <<"\n";

        double s1 = std::clock();
        NODETYPE *aNeighourArray = &edgeArray[nd0.offset_plus];
        for (int j = 0; j < nd0.size_plus; j++) {
            NODETYPE bNode = aNeighourArray[j];
            NODE nd2 = ndArray[bNode];
            NODETYPE *bNeighbourArray = &edgeArray[nd2.offset];
//          Neighbours of nd2 && greater than nd1
            unsigned int nd2_plus_offset = offsets[j];
//            bNeighbourArray = &edgeArray[nd2.offset + nd2_plus_offset];
            NODETYPE bSize = nd2.size - nd2_plus_offset;

            for (int k = j + 1; k < nd0.size_plus; k++) {
                NODETYPE cNode = aNeighourArray[k];
                NODE nd3 = ndArray[cNode];
                NODETYPE *cNeighbourArray = &edgeArray[nd3.offset];
                //          Neighbours of nd3 && greater than nd1
                unsigned int nd3_plus_offset = offsets[k];
//                cNeighbourArray = &edgeArray[nd3.offset + nd3_plus_offset];
                assert(nd3_plus_offset <= nd3.size);
                NODETYPE cSize = nd3.size - nd3_plus_offset;

//            s = s + naive_intersect
//                    ( &cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);

                res = res +
                      hybrid_intersect(&cNeighbourArray[nd3_plus_offset], cSize, &bNeighbourArray[nd2_plus_offset],
                                       bSize);
            }
//            s = s + hybrid_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
        double s2 = std::clock();
        naive_timer = s2-s1;
        naive_total  =  naive_total + naive_timer/1000000;
        }
//      Time till here  < 1s

        NODETYPE baseSize;
        NODETYPE *base;

//      Union all elements.
        {
            double s1 = std::clock();
            baseSize = computeBaseArrayWithMinHeap(arrays, arrsize, nd0.size_plus, baseArray);
            base = baseArray;
            int charbitVectorsize = ((baseSize + 32) / 32 * 4);
            unsigned char bitArray[charbitVectorsize * nd0.size_plus];
            memset(bitArray, 0, sizeof(unsigned char) * charbitVectorsize * nd0.size_plus);

            for (int j = 0; j < nd0.size_plus; j++) {
                NODE cNode = ndArray[edgeArray[nd0.offset_plus + j]];
                hybridBitMark_intersect(base, baseSize, &edgeArray[cNode.offset + offsets[j]],
                                        cNode.size - offsets[j], &bitArray[charbitVectorsize * j]);
            }
            for (int j = 0; j < nd0.size_plus; j++) {
                int aoffset = charbitVectorsize * j;
                for (int k = j + 1; k < nd0.size_plus; k++) {
                    int boffset = charbitVectorsize * k;
                    for (int t = 0; t < charbitVectorsize; t++) {
                        unsigned char x = bitArray[aoffset + t] & bitArray[boffset + t];
                        res = res + bitDictionary->countSetBits(x);
                    }
                }
            }

            double s2 = std::clock();
            min_heap_timer = s2 - s1;
            if(min_heap_timer < naive_timer)current_min = min_heap_timer;
        }
        {
            double s3 = std::clock();
            baseSize = computeNaiveBaseArrayAndReturnSize(arrays, arrsize, nd0.size_plus, baseArray);
            base = baseArray;
            int charbitVectorsize = ((baseSize + 32) / 32 * 4);
            unsigned char bitArray[charbitVectorsize * nd0.size_plus];
            memset(bitArray, 0, sizeof(unsigned char) * charbitVectorsize * nd0.size_plus);

            for (int j = 0; j < nd0.size_plus; j++) {
                NODE cNode = ndArray[edgeArray[nd0.offset_plus + j]];
                hybridBitMark_intersect(base, baseSize, &edgeArray[cNode.offset + offsets[j]],
                                        cNode.size - offsets[j], &bitArray[charbitVectorsize * j]);
//          if(cNode.size - offsets[j]<10)continue;
//          hybrid_intersect(base,baseSize,&edgeArray[cNode.offset+offsets[j]],cNode.size - offsets[j]);
            }
                  for(int j=0;j<nd0.size_plus;j++){
                      int aoffset = charbitVectorsize * j;
                      unsigned int *ap = (unsigned int *)&bitArray[aoffset];
                      for(int k=j+1;k<nd0.size_plus;k++){
                          int boffset = charbitVectorsize * k;
                          unsigned int *bp = (unsigned int *)&bitArray[boffset];
                          for(int t=0;t<charbitVectorsize/4;t=t+1){
                              res = res + _popcnt32(bp[t] & ap[t]);
//                    unsigned char x = bitArray[aoffset + t] & bitArray[boffset + t];
//                    res = res + bitDictionary->countSetBits(x);
                          }
                      }
                  }
            double s4 = std::clock();
            naive_bit_timer= s4 - s3;
            if(naive_bit_timer < current_min)current_min = naive_bit_timer;
        }
        if (( naive_bit_timer <  min_heap_timer) && (naive_bit_timer <  naive_timer)) {
//            cout <<i <<  "Naive BIT timer " << nd0.size_plus << ":" << avgSize << ":" << naive_bit_timer <<":"<<
//                        min_heap_timer <<":" << naive_timer << "\n";
            naive_bit_v ++ ;
        }
        if (( min_heap_timer < naive_bit_timer) && ( min_heap_timer <  naive_timer)) {
//            cout << i << "MIN HEAP timer " << nd0.size_plus << ":" << avgSize << "\n";
            min_heap_v++;
        }
        if ((naive_timer < naive_bit_timer) && (naive_timer <  min_heap_timer)) {
//            cout << i << "Naive timer " << nd0.size_plus << ":" << avgSize << ":" << naive_bit_timer <<":"<<
//                                                                           min_heap_timer <<":" << naive_timer << "\n";
            naive_v ++;
        }
//        cout << naive_total <<":"<< min_total<<"\n";
    }
    cout << "Naive bit v" << naive_bit_v << "\n";
    cout << "Min Heap v" << min_heap_v << "\n";
    cout << "Naive V" << naive_v << "\n";
    cout << "total squares found:" << res << "\n";
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total base array time " << get_timer(TOTALNODEPROCESSTIME) <<"\n";

}