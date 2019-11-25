//
// Created by sandeep on 26/8/19.
//

#ifndef V2_MINHEAP_H
#define V2_MINHEAP_H

#include <definitions.h>
#include <assert.h>

struct HeapElement {
    NODETYPE value;
    int arrayId;
};

class MinHeap {

    HeapElement * array;
    int maxSize;

public:
    int currentSize = 0;

    MinHeap(int size);

    MinHeap(HeapElement * array,int size);

    void addElement(NODETYPE nd, int arrayId);

    void popElement(HeapElement * target);

    HeapElement peekTopElement();

    void popTopAndInsertNewElement(NODETYPE nd, int arrayId);

    void debugHeap();

    ~MinHeap(){}

    NODETYPE getSecondMinimum(){
        assert(currentSize>=2);
        return array[1].value;
    }

    void clear(){
        currentSize = 0;
    }
};

class MinArray {

    HeapElement * array;
    int maxSize;

public:
    int currentSize = 0;

    MinArray(int size);

    MinArray(HeapElement * array,int size);

    void addElement(NODETYPE nd, int arrayId);

    void popElement(HeapElement * target);

    HeapElement peekTopElement();

//    void popTopAndInsertNewElement(NODETYPE nd, int arrayId);

//    void debugHeap();

    ~MinArray(){}

};
#endif //V2_MINHEAP_H
