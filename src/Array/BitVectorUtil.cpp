//
// Created by sandeep on 21/11/19.
//

#include <definitions.h>
#include <iostream>
#include <cstring>
#include "BitVectorUtil.h"
#include "ArrayUtilities.h"
#include "MinHeap.h"
#include "algorithm"

NODETYPE a[maxArraySize];
NODETYPE b[maxArraySize];

HeapElement ha[maxArraySize];
HeapElement hb[maxArraySize];

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
//        std::cout << "###### BASE ARRAY ##############";
//        for(int j=0;j<baseSize;j++){
//            std::cout << base[j] << " " ;
//        }
//        std::cout <<"\n";

    return baseSize;
}



/* Current run time : 12s*/
int computeNaiveBaseArrayWithDuplicates(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE * baseArray){
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
        if(nodePmax * 40< baseSize){
            continue;
        }
        if(baseSize * 40< nodePmax){
            continue;
        }
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
    if(count >=2){
        baseArray[bp] = prev;
        bp++;
    }
//        cout << "###### BASE ARRAY ##############";
//        for(int j=0;j<baseSize;j++){
//            cout << base[j] << " " ;
//        }
//        cout <<"\n";

    return bp;
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

/* Current run time : 12s*/
int computeBaseArrayAndBitMatrixReturnSize(NODETYPE **arrays, NODETYPE *arrsizes,
        NODETYPE noArrays, NODETYPE * baseArray){
//  Union all elements.
//  copy one neighbourhood into the base array
    if(noArrays <2)return 0;

    int t = 0;
    for(int k=0; k < arrsizes[0]; k++){
        ha[t].value = arrays[0][k];
        ha[t].arrayId = 0;
        t ++ ;
    }

//  Union over all other elements.
    HeapElement *base = ha;
    int baseSize = t;
    HeapElement *temp = hb;
    for(int j=1;j<noArrays;j++){
//        NODE cNode = ndArray[edgeArray[nd0.offset_plus+j]];
        int bP = 0;
        t = 0;
        NODETYPE *currArray = arrays[j];
        int nodeP = 0;
        int nodePmax = arrsizes[j];
        while((bP < baseSize) && (nodeP < nodePmax)){
            if(base[bP].value == currArray[nodeP]){
                temp[t] = base[bP];
                bP ++;
                t++;
                temp[t].value = currArray[nodeP];
                temp[t].arrayId = j;
                t++;
                nodeP ++;
                continue;
            }
            if(base[bP].value < currArray[nodeP]){
                temp[t] = base[bP];
                t++;
                bP ++;
            }else{
                temp[t].value = currArray[nodeP];
                temp[t].arrayId = j;
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
            temp[t].value = currArray[nodeP];
            temp[t].arrayId = j;
            t++;
            nodeP ++;
        }
        HeapElement *bck = base;
        base = temp;
        temp = bck;
        baseSize = t;
//          swap temp and base in the end
    }
    NODETYPE prev= base[0].value;
    int cnt = 1;
    int finalSize = 0;
    for(int i=1;i<baseSize;i++){
        if(prev == base[i].value)cnt++;
        if(prev != base[i].value){
            if(cnt>=2){
                baseArray[finalSize] = base[i].value;
                finalSize++;
            }
            cnt = 1;
            prev = base[i].value;
        }
    }
//        std::cout << "###### BASE ARRAY ##############";
//        for(int j=0;j<baseSize;j++){
//            std::cout << base[j] << " " ;
//        }
//        std::cout <<"\n";

    return finalSize;
}

int unionNaiveOf2Arrays(NODETYPE *baseA,NODETYPE *baseB,
        NODETYPE baseSizesA, NODETYPE baseSizesB, NODETYPE *baseArray){
    int i=0;int j=0;int t=0;
    while((i<baseSizesA) && (j<baseSizesB)){
        if(baseA[i] == baseB[j]){
            baseArray[t] =baseA[i];
            baseArray[t+1] =baseA[i];
            t=t+2;
//            t++;
            i++;
            j++;
            continue;
        }
        if(baseA[i] < baseB[j]){
            baseArray[t] = baseA[i];
            t++;
            i++;
        }else{
            baseArray[t] = baseB[j];
            t++;
            j++;
        }
    }
    while(i < baseSizesA){
        baseArray[t] = baseA[i];
        t++;
        i++;
    }
    while(j < baseSizesB){
        baseArray[t] = baseB[j];
        t++;
        j++;
    }

    return t;
}

int highlyskewedUnionOf2Arrays(NODETYPE *baseA,NODETYPE *baseB, NODETYPE baseSizesA,
        NODETYPE baseSizesB, NODETYPE *baseArray){
    int i=0;int j=0;int t=0;
//  baseA is the larger array.
    while((i<baseSizesA) && (j<baseSizesB)){
        if(baseA[i] == baseB[j]){
            baseArray[t] =baseA[i];
//            baseArray[t+1] =baseA[i];
//            t=t+2;
            t++;
            i++;
            j++;
            continue;
        }
        if(baseA[i] < baseB[j]){
            int off = binarySearchFirstElementGreaterOrEqualTarget(baseA,i+1,baseSizesA,baseB[j]);
            while(i < off){
                baseArray[t] = baseA[i];
                t++;
                i++;
            }
        }else{
            baseArray[t] = baseB[j];
            t++;
            j++;
        }
    }
    while(i < baseSizesA){
        baseArray[t] = baseA[i];
        t++;
        i++;
    }
    while(j < baseSizesB){
        baseArray[t] = baseB[j];
        t++;
        j++;
    }

    return t;
}

int  computeNaiveBaseArraySkew(NODETYPE **arrays, NODETYPE *arrsizes,
                               NODETYPE noArrays, NODETYPE* baseArray){
    //  Union all elements.
//  copy one neighbourhood into the base array
    if(noArrays < 2) return 0;
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
        if((baseSize *20 < arrsizes[j])){
            t = highlyskewedUnionOf2Arrays(arrays[j] , base,arrsizes[j] ,baseSize,temp);
        }else{
            if((arrsizes[j] * 20  < baseSize)){
                t = highlyskewedUnionOf2Arrays(base, arrays[j] ,baseSize ,arrsizes[j],temp);
            }else{
                t = unionNaiveOf2Arrays(base, arrays[j] ,baseSize ,arrsizes[j],temp);
            }
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
//        std::cout << "###### BASE ARRAY ##############";
//        for(int j=0;j<baseSize;j++){
//            std::cout << base[j] << " " ;
//        }
//        std::cout <<"\n";

    return baseSize;
}

int computeUnionWithRepetition(NODETYPE **arrays, NODETYPE *arrsizes, NODETYPE noArrays, NODETYPE * baseArray){
    if(noArrays == 1){
        for(int i=0;i<arrsizes[0];i++){
            baseArray[i] = arrays[0][i];
        }
        return arrsizes[0];
    }
    int split_a = noArrays/2;
    int split_b = noArrays-split_a;
    NODETYPE *arrsA[split_a]; NODETYPE sizesA[split_a]; NODETYPE baseSizesA = 0;
    NODETYPE *arrsB[split_b]; NODETYPE sizesB[split_b]; NODETYPE baseSizesB = 0;
    for(int i = 0;i<noArrays;i++){
        if(i<split_a){
            arrsA[i] = arrays[i];
            sizesA[i] = arrsizes[i];
            baseSizesA = baseSizesA + arrsizes[i];
        } else{
            arrsB[i-split_a] = arrays[i];
            sizesB[i - split_a] = arrsizes[i];
            baseSizesB = baseSizesB + arrsizes[i];
        }
    }
    NODETYPE baseA[baseSizesA];
    NODETYPE baseB[baseSizesB];
    baseSizesA = computeUnionWithRepetition(arrsA, sizesA,split_a,baseA);
    baseSizesB = computeUnionWithRepetition(arrsB, sizesB,split_b,baseB);
    int i=0;int j=0;int t=0;
//    if((baseSizesA *20 < baseSizesB)){
//        return highlyskewedUnionOf2Arrays(baseB,baseA,baseSizesB,baseSizesA,baseArray);       std::cout << baseSizesA <<":" << baseSizesB<<"\n";
//    }
//    if((baseSizesB *20 < baseSizesA)){
//        return highlyskewedUnionOf2Arrays(baseA,baseB,baseSizesA,baseSizesB,baseArray);
//    }
    return unionNaiveOf2Arrays(baseA,baseB,baseSizesA,baseSizesB,baseArray);

}

int computeBaseArrayUsingBinaryRecursion(NODETYPE **arrays, NODETYPE *arrsizes, NODETYPE noArrays, NODETYPE * baseArray){
    int s = 0;
    assert(noArrays > 1);
    for(int i=0;i<noArrays;i++){
        s = s + arrsizes[i];
    }
//    NODETYPE duplicates[s];
    int unionSize = computeUnionWithRepetition(arrays,arrsizes,noArrays, baseArray);
//    assert(unionSize == s);
//    NODETYPE prev= duplicates[0];
//    int cnt = 1;
//    int finalSize = 0;
//    for(int i=1;i<unionSize;i++){
//        if(prev == duplicates[i])cnt++;
//        if(prev != duplicates[i]){
//            if(cnt>=2){
//                baseArray[finalSize] = duplicates[i];
//                finalSize++;
//            }
//            cnt = 1;
//            prev = duplicates[i];
//        }
//    }
    return  unionSize;
//    return finalSize;
}

/* Current run time : 12s*/
int bloomfilter_based_baseArray(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE * baseArray){
//  Union all elements.
//  copy one neighbourhood into the base array
    int t = 0;
    int tsizes = 0;
    for(int i=0;i<noArrays;i++){
        tsizes = tsizes + arrsizes[i];
    }
    int power = 1;
    int p=0;
    while(power < tsizes){
        power*=2;
        p++;
    }
    power = power * 4;
    int bloom[power];
    memset(bloom,0,sizeof(int)*power);

    for(int k=0; k < arrsizes[0]; k++){
        bloom[arrays[0][k] & power]++;
//        a[t] = arrays[0][k];
//        t ++ ;
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
                if(bloom[currArray[nodeP]& power] >1){
                    temp[t] = currArray[nodeP];
                    t++;
                }else{
                    bloom[currArray[nodeP]& power] ++;
                }
                nodeP ++;
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
            if(bloom[currArray[nodeP]& power] >1){
                temp[t] = currArray[nodeP];
                t++;
            }else{
                bloom[currArray[nodeP]& power] ++;
            }
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
//        std::cout << "###### BASE ARRAY ##############";
//        for(int j=0;j<baseSize;j++){
//            std::cout << base[j] << " " ;
//        }
//        std::cout <<"\n";

    return baseSize;
}


int usingTrivialSort(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE * baseArray){
    int tsizes = 0;
    for(int i=0;i<noArrays;i++){
        tsizes = tsizes + arrsizes[i];
    }
    NODETYPE arr[tsizes];
    NODETYPE arrt[tsizes];
    NODETYPE *a,*b;
    a = &arr[0];
    b = &arrt[0];
    int t=0;
    for(int i=0;i<noArrays;i++){
        std::merge(a,a+t,arrays[i],arrays[i]+arrsizes[i],b);
        NODETYPE *temp = a;
        a = b;
        b = temp;
        t = t + arrsizes[i];
    }
//    std::cout << "debug\n";
//    for(int i=0;i<t;i++){
//        std::cout << a[i] << " ";
//    }
//    std::cout <<"\n";

    return 0;

}

bool compare(HeapElement h1, HeapElement h2){
    return h1.value  < h2.value ;
}

int sortNeighbourhood(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE * baseArray){
    int curr = 0;
    for(int i=0;i < noArrays; i++){
        HeapElement t;
        t.arrayId = 0;
        for(int j=0; j < arrsizes[i];j++){
            t.value = arrays[i][j];
            ha[curr] = t;
            curr ++;
        }
    }
//    std::sort(ha,ha+curr,compare);
}