//
// Created by sandeep on 26/8/19.
//

#include <cstdlib>
#include <iostream>
#include <assert.h>
#include "MinHeap.h"


void swap(HeapElement *a, HeapElement*b){
    NODETYPE t = a->value;
    int tt = a->arrayId;

    a->value = b->value;
    a->arrayId = b->arrayId;
    b->value = t;
    b->arrayId = tt;
}

MinHeap::MinHeap(int size){
    this->array = (HeapElement *)malloc(sizeof(HeapElement));
    maxSize = size;
}

MinHeap::MinHeap(HeapElement * array,int size){
    this->array = array;
    maxSize = size;
}

HeapElement MinHeap::peekTopElement() {
    HeapElement top;
    top.value= array[0].value;
    top.arrayId = array[0].arrayId;
    return top;
}

void MinHeap::popTopAndInsertNewElement(NODETYPE nd, int arrayId) {
    array[0].value = nd;
    array[0].arrayId = arrayId;

    if(currentSize==1){
        return;
    }

    int t= 0;
    while(true){
       assert(t>=0);
       int l = 2*t +1;
       int r = 2*t +2;
       int copy = t;
       if (l < currentSize && array[l].value < array[t].value) {
           t = l;
       }
        // If right child is larger than largest so far
        if (r < currentSize && array[r].value < array[t].value){
            t = r;
        }
        // If largest is not root
        if (t != copy) {
            swap(&array[copy], &array[t]);
        }else{
            break;
        }
    }
}

/*FIX ME*/
void MinHeap::addElement(NODETYPE nd, int arrayId){
    array[currentSize].arrayId = arrayId;
    array[currentSize].value = nd;
    int i = currentSize;
//    check for space
//    std::cout << currentSize << " "  << maxSize <<"\n";
    assert(currentSize < maxSize  );
    currentSize ++;
    while(i > 0) {
        int ii = (i-1)/2;
        if(array[i].value <array[ii].value){
            swap(&array[i],&array[ii]);
            i = ii;
        } else {

            return;
        }
    }
}

// FixME: Worst code of my life.
void MinHeap::popElement(HeapElement * target){
    target->value = array[0].value;
    target->arrayId = array[0].arrayId;
    if(currentSize==1){
        currentSize--;
        return;
    }

    array[0].value = array[currentSize-1].value;
    array[0].arrayId = array[currentSize-1].arrayId;
    int t= 0;
    currentSize --;
    while(true){
        assert(t>=0);
        int l = 2*t +1;
        int r = 2*t +2;
        int copy = t;
        if (r < currentSize && array[r].value < array[t].value) {
            t = r;
        }
        if (l < currentSize && array[l].value < array[t].value) {
            t = l;
        }

        // If largest is not root
        if (t != copy) {
//            std::cout << (array[copy].value) <<" "<< (array[t].value) << " \n";
            swap(&array[copy], &array[t]);
        }else{
            break;
        }
    }
    return;
//  While t is not a leaf node . If there are n nodes, n/2 are non-leaf.
    while(t < currentSize){
        int tt1 = (t)*2;
        int tt2 = (t)*2+1;
        if((tt1 >currentSize ) && (tt2>currentSize)){
            return;
        }
        if((tt1 >currentSize ) || (tt2>currentSize)){
            int curr;
            if(tt1 > currentSize){
                curr = tt2;
            }else{
                curr = tt1;
            }
            if(array[t].value > array[curr].value){
                swap(&array[curr],&array[t]);
            }
            return;
        }
        if((array[t].value < array[tt1].value) && (array[t].value < array[tt2].value)){
//            std::cout << array[t].value << " is lesser than "
//            << array[tt1].value << ":" << array[tt2].value <<"\n";
            return;
        }
        if(array[tt1].value < array[tt2].value){
            swap(&array[tt1],&array[t]);
            t = tt1;
        }else{
            swap(&array[tt2],&array[t]);
            t = tt2;
        }
    }
}

void MinHeap::debugHeap(){
    std :: cout << "######## \n";
    std::cout << currentSize;
    for(int i=0;i<currentSize;i++){
        std::cout << i<<":"<<array[i].value << " " ;
    }
    std::cout << "\n";
}
void heapTest(){
        int x = 22;
        HeapElement  el;
        MinHeap * m = new MinHeap(x);
        for(int i=x-1;i>=0;i--){
        m->addElement(i,1);
        m->debugHeap();
        }

        for(int i=0;i<x;i++){
        m->debugHeap();
        m->popElement(&el);
        assert(i==el.value);
//        std::cout << i << ":" << el.value << "\n";
        }
}
