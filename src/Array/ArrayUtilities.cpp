//
// Created by sandeep on 15/9/19.
//
#include "ArrayUtilities.h"
#include <iostream>
#include <assert.h>

using namespace std;

const int linearSearchThresold = 20;

int binary_search(NODETYPE* nd, int start,int end, NODETYPE target){
    if((end-start+1) < linearSearchThresold){
        for(int i=start;i<=end;i++){
            if(nd[i]==target)return i;
        }
        assert(false);
    }
    int base = start;
    while(start<=end){
        int mid = (start+end)/2;
        if(nd[mid]==target)return mid-base;
        if(nd[mid]<target)start = mid+1;
        if(nd[mid]>target)end = mid-1;
    }
//  Should never be reached
    cout << "Could not find " << target << "\n";
    for(int i=start;i<=end;i++){
        cout << nd[i] << " ";
    }
    cout << "Could not find " << target << "\n";
    assert(false);
    return -1;
}


int binarySearchFirstElementGreaterOrEqualTarget(NODETYPE *arr,int start, NODETYPE size, int target){
//    return start;
    if(arr[start]>=target)return start;
    if(size<linearSearchThresold){
        for(int i=start;i<size;i++){
            if(arr[i]>=target)return i;
        }
        return size;
    }
    int end = size-1;
    int mid;
    assert(size>0);
    while(start < end){
        mid = (start+end)/2;
//        assert(mid>=1);
        if((arr[mid]==target) ||((arr[mid]>target) && ((mid==0)||(arr[mid-1]<target)))) return mid;
        if(arr[mid]<target) start = mid+1;
        if(arr[mid]>target) end = mid-1;
    }
    mid = (start+end)/2;
//    cout << "new "<< arr[mid]<<"\n";
    if(mid==size-1 && arr[mid]<target)return mid+1;
//    std::cout << arr[mid] << ":" << target << ":"<<  arr[mid-1] <<"\n";
    assert((arr[mid]==target) ||((arr[mid]>target) &&((mid==0)||(arr[mid-1]<target))));
    return mid;
}

//  This has not been debugged.
int binarySearchFirstElementLessOrEqualTarget(NODETYPE *arr,int start, NODETYPE size, int target){
    assert(false);
    if(size<linearSearchThresold){
        for(int i=start;i<size;i++){
            if(arr[i]<=target)return i;
        }
        return size;
    }
    int end = size-1;
    int safety = end;
    int mid;
//    cout << "start "<< target <<"\n";
    while(start < end){
        mid = (start+end)/2;
//        std::cout <<arr[start] <<":" << arr[end] << ":"<<  arr[mid] <<"\n";
        assert(mid>=1);
        if((arr[mid]==target) ||((arr[mid]<target) &&((mid==safety)||(arr[mid+1]>target)))) return mid;
        if(arr[mid]<target) start = mid+1;
        if(arr[mid]>target) end = mid-1;
    }
    mid = (start+end)/2;
//    cout << "new "<< arr[mid]<<"\n";
    if(mid==0 &&arr[mid]>target)return -1;
//    std::cout <<arr[mid] <<":" << target << ":"<<  arr[mid-1] <<"\n";
    assert((arr[mid]==target) ||((arr[mid]<target) && (arr[mid+1]<target)));
    return mid;
}