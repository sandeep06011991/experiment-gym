//
// Created by sandeep on 21/11/19.
//

#include <definitions.h>
#include "BitVectorUtil.h"

NODETYPE a[maxArraySize];
NODETYPE b[maxArraySize];


int computeBaseArrayAndReturnSize(NODETYPE **arrays, NODETYPE *arrsizes,NODETYPE noArrays, NODETYPE * baseArray){
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
    return baseSize;
}