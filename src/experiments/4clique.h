//
// Created by sandeep on 30/12/19.
//


#include <definitions.h>
#include <intersection.h>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "graph.h"
#include "ArrayUtilities.h"
#include <immintrin.h>
#include <BitDictionary.h>
#include <bitset>

#ifndef V3_4CLIQUE_H
#define V3_4CLIQUE_H


/* datastructures and headers. Intersection buffer with NODETYPE lists.  */
NODETYPE buffer[1000000];
NODETYPE bufferIds[1000000];

// Phase 1. accumulate.
struct triple triplelist[10000000];

/* Phase 2. Create a list of sets, break down triples into entitities*/
struct twos {
    NODETYPE nodeid;
//  # 1 for nd1 and #2 for nd2
    NODETYPE position;
    NODETYPE triple_id;
};


struct twos twobuffer[10000000];

struct freq {
//  FixMe: Dont need a. Delete this.
    NODETYPE a;
    NODETYPE f;
    NODETYPE start;
};

struct freq freqlist[10000000];

/* Baseline for 4 clique counting.
 * LJ total run time 80s */
NODETYPE naive4cliqueCounting(Graph* graph){
    NODE *ndArray = graph->getNodeArray();
    NODETYPE *edgeArray = graph->getEdgeArray();
    int res = 0;
    start_timer(TOTALNODEPROCESSTIME);
//    for(int i=0;i<0;i++){
    for(int i=0;i<graph->getNoVertexes();i++){
//        cout <<i <<"\n";
        NODE nd1 = ndArray[i];
        for(int j=0;j<nd1.size_plus;j++){
            NODE nd2 = ndArray[edgeArray[nd1.offset_plus + j]];
            int cs3 =intersectAndReturnCandidateSets(&edgeArray[nd1.offset_plus],nd1.size_plus,
                            &edgeArray[nd2.offset_plus],nd2.size_plus,
                                                buffer);
            for(int k=0;k<cs3;k++){
                NODE nd3 = ndArray[buffer[k]];
                res = res + hybrid_intersect(buffer,cs3,&edgeArray[nd3.offset_plus],nd3.size_plus);

            }
        }
    }
    stop_timer(TOTALNODEPROCESSTIME);

    cout << "total node processing time " << get_timer(TOTALNODEPROCESSTIME) << "\n";
    std::cout << "Total 4 cliques found: " << res <<"\n";
}

/* V1
 * Keep expensive triples. Sort based with triples. */
NODETYPE v1_4cliqueCounting(Graph* graph){
    NODE *ndArray = graph->getNodeArray();
    NODETYPE *edgeArray = graph->getEdgeArray();

    int res = 0;
    int triple_c = 0;
    reset_timer(TOTALNODEPROCESSTIME);
    start_timer(TOTALNODEPROCESSTIME);
//    for(int i=0;i<0;i++){
    for(int i=0;i<graph->getNoVertexes();i++){
//        cout <<i <<"\n";
        NODE nd1 = ndArray[i];
        for(int j=0;j<nd1.size_plus;j++){
            NODE nd2 = ndArray[edgeArray[nd1.offset_plus + j]];
            int cs3 =intersectAndReturnCandidateSets(&edgeArray[nd1.offset_plus],nd1.size_plus,
                                                     &edgeArray[nd2.offset_plus],nd2.size_plus,
                                                     buffer);
            for(int k=0;k<cs3;k++){
                NODE nd3 = ndArray[buffer[k]];
                if(nd3.size_plus>500){
//                    cout << "Adding triples " << nd1.id << " " << nd2.id <<" " << nd3.id <<"\n";
//                    triplelist[triple_c].nd1 = nd1.id;
//                    triplelist[triple_c].nd2 = nd2.id;
//                    triplelist[triple_c].nd3 = nd3.id;
//                    triple_c ++;
//                    assert(triple_c < 10000000);
                     continue;
                }
                res = res + hybrid_intersect(buffer,cs3,&edgeArray[nd3.offset_plus],nd3.size_plus);

            }
        }
    }
    cout << "Begin processing triples\n";
//    if(triple_c ==0 ){
    if(false){
        stop_timer(TOTALNODEPROCESSTIME);
        cout << "NO triples no use of opitmizations !!!\n";
        cout << "total node processing time " << get_timer(TOTALNODEPROCESSTIME) << "\n";
        std::cout << "Total 4 cliques found: " << res <<"\n";
        return res;
    }else{
        cout << "Triples found " << triple_c <<"\n";
    }
    sort(triplelist, triplelist+triple_c, [&](triple a, triple b){
        return a.nd3 < b.nd3;
    });
//  Triple list
/*  Ls <a,b,c> , <a,b,c>*/
    bool areTriplesProessed[triple_c];
    memset(areTriplesProessed, 0, sizeof(bool)* triple_c);

    /* offset_start refers to start of contiguous block of triples
     * and offset size refers to size of contiguos block all having the same node3
     * * */
//    int offset_start = 0;
    int twos_size = 2;

    /* break down triples in induvidual components. */
//    assert(triple_c>0);
//    int twos_c = 2;
    twobuffer[0].nodeid = triplelist[0].nd1 ;
    twobuffer[0].triple_id = 0;
    twobuffer[0].position = 1;
    twobuffer[1].nodeid = triplelist[0].nd2 ;
    twobuffer[1].triple_id = 0;
    twobuffer[1].position = 2;
    int curr_nd3 = triplelist[0].nd3;

    for(int i=1;i<triple_c+1;i++){
        if((triplelist[i].nd3 != curr_nd3) || (i==triple_c)){
            /*  Process this offset  */

            sort(twobuffer, twobuffer + twos_size, [&](twos a, twos b){
                return a.nodeid < b.nodeid;
            });
//          Get a list of frequencies of individual components.
            int freq_c = 0;
            int prev_id = 0;
            freqlist[freq_c].a = twobuffer[0].nodeid;
            freqlist[freq_c].f = 1;
            freqlist[freq_c].start   = 0;
            for(int j=1; j<twos_size; j++){
                if((twobuffer[prev_id].nodeid == twobuffer[j].nodeid)){
                    freqlist[freq_c].f ++;
                }else{
                    freq_c ++;
                    freqlist[freq_c].a = twobuffer[j].nodeid;
                    freqlist[freq_c].f = 1;
                    freqlist[freq_c].start   = j;
                }
            }
            sort(freqlist,freqlist+freq_c, [&](freq a, freq b){
                return a.f > b.f ;
            });
//          Process each frequency indiviudally.
            for(int k=0;k<freq_c;k++){
                NODETYPE nd3 =  curr_nd3;
                NODETYPE nd2 = freqlist[k].a;
                NODE node3 = ndArray[nd3];
                NODE node2 = ndArray[nd2];

                int intersect1 = intersectAndReturnCandidateSets(&edgeArray[node3.offset_plus],node3.size_plus,
                            &edgeArray[node2.offset_plus], node2.size_plus, buffer);
                if(intersect1 !=0) {
                    for (int l = 0; l < freqlist[k].f; l++) {
                        twos current_two = twobuffer[freqlist[k].start + l];
                        NODETYPE tripleId = current_two.triple_id;
                        if (!areTriplesProessed[tripleId]) {
                            if (current_two.position == 1) {
//                           This is nd1
                                NODE nd1 = ndArray[triplelist[tripleId].nd2];
//                            cout << "Inter" << nd1.id <<" " <<node2.id <<" " << node3.id << " intermediate "<< intersect1 << "\n ";
                                res = res +
                                      hybrid_intersect(&edgeArray[nd1.offset_plus], nd1.size_plus, buffer, intersect1);
                            } else {
//                           This is nd2
//                            cout << "intersection1\n";
                                NODE nd1 = ndArray[triplelist[tripleId].nd1];
                                res = res +
                                      hybrid_intersect(&edgeArray[nd1.offset_plus], nd1.size_plus, buffer, intersect1);
                            }
                            areTriplesProessed[tripleId] = true;
                        }
                    }
                }
            }

            twos_size = 0;
        }
        twobuffer[twos_size].nodeid = triplelist[i].nd1 ;
        twobuffer[twos_size].triple_id = i;
        twobuffer[twos_size].position = 1;
        twos_size ++;
        twobuffer[twos_size].nodeid = triplelist[i].nd2 ;
        twobuffer[twos_size].triple_id = i;
        twobuffer[twos_size].position = 2;
        curr_nd3 = triplelist[i].nd3;
        twos_size++;
    }
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total node processing time " << get_timer(TOTALNODEPROCESSTIME) << "\n";
    std::cout << "Total 4 cliques found: " << res <<"\n";
}
#endif //V3_4CLIQUE_H



/* Baseline for 4 clique counting.
 * LJ total running time 80s
 * Triangle finding time 16s.
 * V2 use previously computed shorter intersections
 * */
NODETYPE v2cliqueCounting(Graph* graph){
    NODE *ndArray = graph->getNodeArray();
    NODETYPE *edgeArray = graph->getEdgeArray();
    int res = 0;
    start_timer(TOTALNODEPROCESSTIME);
//    for(int i=0;i<0;i++){
    for(int i=0;i<graph->getNoVertexes();i++){
//        cout <<i <<"\n";
        NODE nd1 = ndArray[i];
        NODETYPE cs3[nd1.size_plus];
        NODETYPE bufferstart[nd1.size_plus];
        int offset=0;
        for(int j=0;j<nd1.size_plus;j++) {
            NODE nd2 = ndArray[edgeArray[nd1.offset_plus + j]];
//          FixMe: Add a buffer point.
            bufferstart[j] = offset;
            cs3[j] = intersectAndReturnCandidateIds(&edgeArray[nd1.offset_plus], nd1.size_plus,
                                                    &edgeArray[nd2.offset_plus], nd2.size_plus,
                                                    &buffer[offset],&bufferIds[offset]);
            offset =offset + cs3[j];
        }
        for(int j=0;j<nd1.size_plus;j++){
            for(int k=0;k<cs3[j];k++){
                NODETYPE id3 = bufferIds[bufferstart[j]+k];
                res = res + hybrid_intersect(&buffer[bufferstart[j]],cs3[j],&buffer[bufferstart[id3]],cs3[id3]);
//
            }
        }
    }
    stop_timer(TOTALNODEPROCESSTIME);

    cout << "total node processing time of version 2" << get_timer(TOTALNODEPROCESSTIME) << "\n";
    std::cout << "Total 4 cliques found: " << res <<"\n";
}

/* V3: Evaluating the potential of a bit-matrix
 * Current Performane.
 * Youtube -> .617s
 * LJ -> 25s
 * */
NODETYPE v3cliqueCounting(Graph* graph){
    NODE *ndArray = graph->getNodeArray();
    NODETYPE *edgeArray = graph->getEdgeArray();
    long res = 0;
    BitDictionary * dict = new BitDictionary();
    int * dicarray = dict->dic;
    start_timer(TOTALNODEPROCESSTIME);
    for(int i=0;i<graph->getNoVertexes();i++){
//    for(int i=0;i<graph->getNoVertexes();i++){
//        cout <<i <<"\n";
        NODE nd1 = ndArray[i];
        NODETYPE cs3[nd1.size_plus];
        NODETYPE bufferstart[nd1.size_plus];
        int offset=0;
        memset(buffer,0,nd1.size_plus * nd1.size_plus);
        memset(bufferIds,0,nd1.size_plus * nd1.size_plus);
        for(int j=0;j<nd1.size_plus;j++) {
            NODE nd2 = ndArray[edgeArray[nd1.offset_plus + j]];
//          FixMe: Add a buffer point.
            bufferstart[j] = offset;
            cs3[j] = intersectAndMarkBits(&edgeArray[nd1.offset_plus], nd1.size_plus,
                                                    &edgeArray[nd2.offset_plus], nd2.size_plus,
                                                    (unsigned char *)&buffer[offset], &bufferIds[offset]);
            offset =offset + nd1.size_plus;
        }
        int t_count = 0;
        for(int j=0;j<nd1.size_plus;j++){
            for(int k=0;k<cs3[j];k++){
                NODETYPE id3 = bufferIds[bufferstart[j]+k];
                assert(id3 < nd1.size_plus);
                unsigned char * a = (unsigned char *)&buffer[bufferstart[j]];
                unsigned char * b = (unsigned char *)&buffer[bufferstart[id3]];
                for(int l=0;l<nd1.size_plus/8 + 1;l++){
                    unsigned char c= a[l] & b[l];
                    t_count = t_count + dicarray[(int)c];
//                    t_count  = t_count + dict->countSetBits(c)    ;
                    //  res = res + _mm_countbits_32 (t);
                }
//                res = res + hybrid_intersect(&buffer[bufferstart[j]],cs3[j],&buffer[bufferstart[id3]],cs3[id3]);
//
            }
        }
        res = res+ t_count;
    }
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total node processing time of version 3 :  " << get_timer(TOTALNODEPROCESSTIME) << "\n";
    std::cout << "Total 3 cliques found: " << res <<"\n";
}