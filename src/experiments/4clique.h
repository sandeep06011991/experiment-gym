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
#ifndef V3_4CLIQUE_H
#define V3_4CLIQUE_H


/* datastructures and headers. Intersection buffer with NODETYPE lists.  */
NODETYPE buffer[1000000];

// Phase 1. accumulate.
struct triple triplelist[1000000];

/* Phase 2. Create a list of sets, break down triples into entitities*/
struct twos {
    NODETYPE a;
    NODETYPE position;
    NODETYPE triple_id;
};


struct twos twobuffer[1000000];

struct freq {
    NODETYPE a;
    NODETYPE f;
    NODETYPE start;
};

struct freq freqlist[1000000];

/* Baseline for 4 clique counting. */
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

/* Finish v1 and get measureents by 12. and then go to gym.*/
NODETYPE v1_4cliqueCounting(Graph* graph){
    NODE *ndArray = graph->getNodeArray();
    NODETYPE *edgeArray = graph->getEdgeArray();

    int res = 0;
    int triple_c = 0;

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
                if(nd3.size_plus>100){
                    triplelist[triple_c].nd1 = nd1.id;
                    triplelist[triple_c].nd2 = nd2.id;
                    triplelist[triple_c].nd3 = nd3.id;
                    triple_c ++;
                    continue;
                }
//                res = res + hybrid_intersect(buffer,cs3,&edgeArray[nd3.offset_plus],nd3.size_plus);

            }

        }
    }

    sort(triplelist, triplelist+triple_c, [&](triple a, triple b){
        return a.nd3 < b.nd3;
    });

//  Triple list
/*  Ls <a,b,c> , <a,b,c>*/
    bool areTriplesProessed[triple_c];
    memset(areTriplesProessed, 0, sizeof(bool)* triple_c);

    int offset_start = 0;
    int offset_size = 0;

    /* break down triples in induvidual components. */
    int twos_c = 2;
    twobuffer[0].a = triplelist[0].nd1 ;
    twobuffer[0].triple_id = 0;
    twobuffer[0].position = 0;
    twobuffer[1].a = triplelist[0].nd2 ;
    twobuffer[1].triple_id = 0;
    twobuffer[1].position = 1;

    /* What do you want to do right now ? */


    for(int i=1;i<triple_c;i++){
        if(triplelist[i].nd3 != triplelist[offset_start].nd3){
            /*  Process this offset  */
            sort(twobuffer+offset_start*2, twobuffer + offset_size*2, [&](twos a, twos b){
                return a.a < b.a;
            });

//          Get a list of frequencies.
            int freq_c = 0;
            int prev_id = offset_start*2;
            freqlist[freq_c].a = twobuffer[offset_start*2].a;
            freqlist[freq_c].f = 1;
            freqlist[freq_c].start   = offset_start;
            for(int j=offset_start*2+1; j<offset_size*2; j++){
                if((twobuffer[prev_id].a == twobuffer[j].a)){
                    freqlist[freq_c].f ++;
                }else{
                    freq_c ++;
                    freqlist[freq_c].a = twobuffer[j].a;
                    freqlist[freq_c].f = 1;
                    freqlist[freq_c].start   = j;
                }
            }
            sort(freqlist,freqlist+freq_c, [&](freq a, freq b){
                return a.f > b.f ;
            });
//          Process each frequency indiviudally.
            for(int k=0;k<freq_c;k++){
                NODETYPE nd3 =  triplelist[offset_start].nd3;
                NODETYPE nd2 = freqlist[k].a;
                NODE node3 = ndArray[nd3];
                NODE node2 = ndArray[nd2];
                int intersect1 = intersectAndReturnCandidateSets(&edgeArray[node3.offset_plus],node3.size_plus,
                            &edgeArray[node2.offset_plus], node2.offset_plus, buffer);
                for(int l=0;l < freqlist[freq_c].f;l++){
                    twos current_two = twobuffer[freqlist[l].start + l];
                    NODETYPE tripleId = current_two.triple_id;
                    if(!areTriplesProessed[tripleId]){
                        if(current_two.position ==0){
//                           This is nd1
                            NODE nd1 = ndArray[triplelist[tripleId].nd2];
                            res = hybrid_intersect(&edgeArray[nd1.offset_plus],nd1.size_plus, buffer, intersect1);
                        }else{
//                           This is nd2
                            NODE nd1 = ndArray[triplelist[tripleId].nd1];
                            res = hybrid_intersect(&edgeArray[nd1.offset_plus],nd1.size_plus, buffer, intersect1);
                        }
                        areTriplesProessed[tripleId] = true;

                    }
                }
            }

            offset_start = i;
            offset_size = 0;
            twos_c = 0;
        }
        twobuffer[twos_c].a = triplelist[i].nd1 ;
        twobuffer[twos_c].triple_id = i;
        twobuffer[twos_c].position = 0;
        twos_c ++;
        twobuffer[twos_c].a = triplelist[i].nd2 ;
        twobuffer[twos_c].triple_id = i;
        twobuffer[twos_c].position = 1;
        twos_c ++;
        offset_size++;
    }
    stop_timer(TOTALNODEPROCESSTIME);
    cout << "total node processing time " << get_timer(TOTALNODEPROCESSTIME) << "\n";
    std::cout << "Total 4 cliques found: " << res <<"\n";
}
#endif //V3_4CLIQUE_H
