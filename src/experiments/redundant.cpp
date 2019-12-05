//
// Created by sandeep on 4/12/19.
//


#include <graph.h>
#include <iostream>
#include <timer.h>
#include <ArrayUtilities.h>
#include <assert.h>
#include <intersection.h>
#include <algorithm>

/* Current objective: FIXME:
 * Naive Runtime:28s
 * Skip everything greater than 500--> 11.2s.
 * Impact of sorting --> 13.37
 *
 * */
struct triple{
    NODETYPE nd2;
    NODETYPE nd3;
    NODETYPE nd1;
};

struct trie{
    NODETYPE nd2;
    NODETYPE nd3;
    NODETYPE start;
    NODETYPE size;
};

bool compare(triple a, triple b){
    if(a.nd2 != b.nd2) return a.nd2 < b.nd2;
    if(a.nd3 != b.nd3) return a.nd3 < b.nd3;
    return a.nd1 < b.nd1;
}
// For live journal
//triple cache[20000000];
//trie Trie[2000000];
//NODETYPE nd1Array[20000000];
// For youtube journal
triple cache[11000000];
trie Trie[1000000];
NODETYPE nd1Array[11000000];
/* Experiment:      Filter                  | naive intersection  | Current Baseline (LIGHT)
 * 1. Youtube :      (nd1 | nd2) > 500      |         18s         |  27s
 * 1. Youtube :      (nd1 | nd2) > 100      |         16.4s       |  27s
 * 2. Live Journal:   (nd1 & nd2) > 500     |         595s        |  867s
 * Questions to Ask/Investigate.
 * 1. Recheck EmptyHeaded time : 192s
 *  ( EmptyHeaded only considers directed graphs, numbers for directed  square graph)
 * 2. Performance BreakDown for youtube graph.
 *  ### Low frequency intersection adj -10.88s
 *  ### Sorting time - .9s
 *  ### Process Trie - 8.6s
 *  Total Triples generated 2570050.
 * 3. Intermediate Compression.
 * FixME:
 * a. Add a gallopping based trie expansion.
 * b. Add a simd based trie expansion.
 *
 * */
void redundant_Intersection(Graph *graph){
    NODE * ndArray = graph->getNodeArray();
    NODETYPE * edgeArray = graph->getEdgeArray();
    long s = 0;
    int t=0;
    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);

    int tc = 0;
    double c_s = clock();
    for(int i=0;i<graph->getNoVertexes();i++){
//    for(int i=450000;i<graph->getNoVertexes();i++){// youtube has 470165
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
                if((cSize>500) || (bSize>500)){
//                 if(true){
                    cache[tc].nd2 = nd2.id;
                    cache[tc].nd1 = nd1.id;
                    cache[tc].nd3 = nd3.id;
                    tc++;
                    assert(tc<20000000);
                    continue;
                }
                s = s + hybrid_intersect(&cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);
//                s = s + intersectSets8IntegerAttime(&cNeighbourArray[nd3_plus_offset], cSize,&bNeighbourArray[nd2_plus_offset], bSize);
            }
//            s = s + hybrid_intersect(aNeighourArray, aSize, bNeighbourArray, bSize);
        }
    }
    double c_e = clock();
    cout << "Total Triples generated: " << tc << "\n";

    cout << "Time to filter low frequency vertex" << (c_e - c_s)/1e6 <<"\n";
    c_s = clock();
    sort(cache,cache + tc,compare);
    c_e = clock();
    cout << "Total Sorting Time " << (c_e - c_s)/1e6 << "\n";

    /* COST of Compression to tries */
    c_s = clock();
    triple prev = cache[0];
    Trie[0].nd2 = prev.nd2;
    Trie[0].nd3 = prev.nd3;
    Trie[0].start = 0;
    Trie[0].size = 1;
    int cTrie = 0;
    nd1Array[0] = cache[0].nd1;
    for(int i=1;i<tc;i++){
        triple curr = cache[i];
        nd1Array[i] = cache[i].nd1;
        if((prev.nd2 == curr.nd2) && (prev.nd3 == curr.nd3)){
            Trie[cTrie].size++;
        }else{
            prev = curr;
            cTrie++;
            Trie[cTrie].nd2 = prev.nd2;
            Trie[cTrie].nd3 = prev.nd3;
            Trie[cTrie].start = i;
            Trie[cTrie].size = 1;
            assert(cTrie < 2000000);
        }
    }
    cout << "unique triples " << cTrie <<"\n";
    c_e = clock();
    cout << "Total time :Trie Creation" << (c_e - c_s)/1e6 << "\n";
//  process Trie
//  intersectTrie4Square(NODETYPE *nd2,NODETYPE nd2Size, NODETYPE *nd3, int nd3Size,
//                         NODETYPE *nd1, NODETYPE nd1Size){
    c_e = clock();
    cout << "Start trie processing \n";
    for(int i=0;i<=cTrie;i++){
        NODE nd2 = ndArray[Trie[i].nd2];
        NODE nd3 = ndArray[Trie[i].nd3];
        s = s+intersectTrie4Square(&edgeArray[nd2.offset],nd2.size,&edgeArray[nd3.offset],nd3.size,
                        &nd1Array[Trie[i].start], Trie[i].size);
    }
    c_s = clock();
    cout << "Time trie processing " << (c_s - c_e)/1e6 << "\n";
    stop_timer(TOTALNODEPROCESSTIME);
//    print_statistics();
    cout << "Sort based " << get_timer(TOTALNODEPROCESSTIME) <<"\n";
    cout << "Naive exploration square found:" <<  s << "\n";
}
