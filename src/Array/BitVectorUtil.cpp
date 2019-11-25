//
// Created by sandeep on 21/11/19.
//

#include <definitions.h>
#include <iostream>
#include "BitVectorUtil.h"
#include "ArrayUtilities.h"
#include "MinHeap.h"

NODETYPE a[maxArraySize];
NODETYPE b[maxArraySize];

/* Current run time : 12s*/
int computeNaiveBaseArrayAndReturnSize(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE * baseArray){
//  Union all elements.
//  copy one neighbourhood into the base array
    int t = 0;
    for(int k=0; k < arrsizes[0]; k++){
        a[t] = arrays[0][k];
        t ++ ;
    }

//      Union over all other elements.
    NODETYPE *base = a;
    int baseSize = t;
    NODETYPE *temp = b;
    for(int j=1;j<noArrays;j++){
//        NODE cNode = ndArray[edgeArray[nd0.offset_plus+j]];
        int bP = 0;
        t = 0;
        NODETYPE *currArray = arrays[j];
        int nodeP = 0;
        int nodePmax = arrsizes[j];
        while((bP < baseSize) && (nodeP < nodePmax)){
            if(base[bP] == currArray[nodeP]){
                temp[t] = base[bP];
                bP ++;
                nodeP ++;
                t++;
                continue;
            }
            if(base[bP] < currArray[nodeP]){
                temp[t] = base[bP];
                t++;
                bP ++;
            }else{
                temp[t] = currArray[nodeP];
                t++;
                nodeP ++;
            }
        }
        while(bP < baseSize){
            temp[t] = base[bP];
            t++;
            bP ++;
        }
        while(nodeP<nodePmax){
            temp[t] = currArray[nodeP];
            t++;
            nodeP ++;
        }
        NODETYPE *bck = base;
        base = temp;
        temp = bck;
        baseSize = t;
//          swap temp and base in the end
    }
    for(int i=0;i<baseSize;i++){
        baseArray[i] = base[i];
    }
//        cout << "###### BASE ARRAY ##############";
//        for(int j=0;j<baseSize;j++){
//            cout << base[j] << " " ;
//        }
//        cout <<"\n";

    return baseSize;
}


/* Current run time : 12s*/
int computeNaiveBaseArrayWithoutDuplicates(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE * baseArray){
//  Union all elements.
//  copy one neighbourhood into the base array
    int t = 0;
    for(int k=0; k < arrsizes[0]; k++){
        a[t] = arrays[0][k];
        t ++ ;
    }

//      Union over all other elements.
    NODETYPE *base = a;
    int baseSize = t;
    NODETYPE *temp = b;
    for(int j=1;j<noArrays;j++){
//        NODE cNode = ndArray[edgeArray[nd0.offset_plus+j]];
        int bP = 0;
        t = 0;
        NODETYPE *currArray = arrays[j];
        int nodeP = 0;
        int nodePmax = arrsizes[j];
        while((bP < baseSize) && (nodeP < nodePmax)){
            if(base[bP] == currArray[nodeP]){
                temp[t] = base[bP];
                t++;
                temp[t] = base[bP];
                bP ++;
                nodeP ++;
                t++;
                continue;
            }
            if(base[bP] < currArray[nodeP]){
                temp[t] = base[bP];
                t++;
                bP ++;
            }else{
                temp[t] = currArray[nodeP];
                t++;
                nodeP ++;
            }
        }
        while(bP < baseSize){
            temp[t] = base[bP];
            t++;
            bP ++;
        }
        while(nodeP<nodePmax){
            temp[t] = currArray[nodeP];
            t++;
            nodeP ++;
        }
        NODETYPE *bck = base;
        base = temp;
        temp = bck;
        baseSize = t;
//          swap temp and base in the end
    }
    NODETYPE prev = base[0];
    int count = 1;
    int bp=0;
    for(int i=1;i<baseSize;i++){
        NODETYPE curr = base[i];
        if(prev==curr){
            count++;
        }else{
            if(count >=2){
                baseArray[bp] = prev;
                bp++;
            }
            count = 1;
            prev = curr;
        }
    }
//        cout << "###### BASE ARRAY ##############";
//        for(int j=0;j<baseSize;j++){
//            cout << base[j] << " " ;
//        }
//        cout <<"\n";

    return bp;
}


/* Current run time : Naive run time : 12s*/
int computeBaseArrayWithBinaryAndReturnSize(NODETYPE **arrays, NODETYPE *arrsizes,
        NODETYPE noArrays, NODETYPE * baseArray){
//  Union all elements.
//  copy one neighbourhood into the base array
    int t = 0;
    for(int k=0; k < arrsizes[0]; k++){
        a[t] = arrays[0][k];
        t ++ ;
    }

//      Union over all other elements.
    NODETYPE *base = a;
    int baseSize = t;
    NODETYPE *temp = b;
    for(int j=1;j<noArrays;j++){
//        NODE cNode = ndArray[edgeArray[nd0.offset_plus+j]];
        int bP = 0;
        t = 0;
        NODETYPE *currArray = arrays[j];
        int nodeP = 0;
        int nodePmax = arrsizes[j];
        while((bP < baseSize) && (nodeP < nodePmax)){
            if(base[bP] == currArray[nodeP]){
                temp[t] = base[bP];
                bP ++;
                nodeP ++;
                t++;
                continue;
            }
            if(base[bP] < currArray[nodeP]){
                temp[t] = base[bP];
                t++;
                bP ++;
                int off = binarySearchFirstElementGreaterOrEqualTarget
                        (base,bP, baseSize, currArray[nodeP] );
                while(bP < off){
                    temp[t] = base[bP];
                    t++;
                    bP ++;
                }

            }else{
                temp[t] = currArray[nodeP];
                t++;
                nodeP ++;
            }
        }
        while(bP < baseSize){
            temp[t] = base[bP];
            t++;
            bP ++;
        }
        while(nodeP<nodePmax){
            temp[t] = currArray[nodeP];
            t++;
            nodeP ++;
        }
        NODETYPE *bck = base;
        base = temp;
        temp = bck;
        baseSize = t;
//          swap temp and base in the end
    }
//    std::cout << baseSize <<" ";
//    for(int i=0; i< noArrays;i++){
//        std::cout << arrsizes[i] <<" ";
//    }
//    std::cout <<"\n";
//    for(int i=0;i<baseSize;i++){
//        baseArray[i] = base[i];
//    }
//        cout << "###### BASE ARRAY ##############";
//        for(int j=0;j<baseSize;j++){
//            cout << base[j] << " " ;
//        }
//        cout <<"\n";

    return baseSize;
}

int computeBaseArrayWithMinHeap(NODETYPE **arrays, NODETYPE *arrsizes,
                                            NODETYPE noArrays, NODETYPE * baseArray){
    HeapElement harr[noArrays];
    MinHeap heap(harr, noArrays);
    NODETYPE fill[noArrays];
    for(int i=0;i<noArrays;i++){
        heap.addElement(arrays[i][0],i);
        fill[i] = 1;
    }
    NODETYPE t=0;
    NODETYPE c=1;
    HeapElement e;
    heap.popElement(&e);
    NODETYPE prev=e.value;
    if(fill[e.arrayId] < arrsizes[e.arrayId]){
        heap.addElement(arrays[e.arrayId][fill[e.arrayId]], e.arrayId);
        fill[e.arrayId]++;
    }

    while(heap.currentSize > 1){
        heap.popElement(&e);
        if(prev == e.value){
            c++;
        }else{
           if(c>=2){
           baseArray[t] = prev;
           t++;
           }
           prev = e.value;
           c=1;
        }
        if(fill[e.arrayId] < arrsizes[e.arrayId]){
            if(arrsizes[e.arrayId] - fill[e.arrayId] >1000){
                HeapElement min = heap.peekTopElement();
                NODETYPE b = binarySearchFirstElementGreaterOrEqualTarget(arrays[e.arrayId],fill[e.arrayId],arrsizes[e.arrayId],min.value);
                heap.addElement(arrays[e.arrayId][b],e.arrayId);
                fill[e.arrayId] = b+1;
            }else{
                heap.addElement(arrays[e.arrayId][fill[e.arrayId]], e.arrayId);
                fill[e.arrayId]++;
            }
        }
    }
    if(c>=2){
        baseArray[t] = prev;
        t++;
    }
    return t;

}