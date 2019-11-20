//
// Created by sandeep on 15/9/19.
//
#include "ArrayUtilities.h"
#include <iostream>
#include <assert.h>

using namespace std;

const int linearSearchThresold = 20;
/* NON UNIFORM API: gives out offset in some cases and
 * position in some cases. */
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

/*
 * Searches array from start, start + size for element greater or equal to
 * than target returns start + offset*/
//FixMe: Clean this code
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
//    if(!((arr[mid]==target) ||((arr[mid]>target) &&((mid==0)||(arr[mid-1]<target))))){
//        cout <<mid << " " <<size << " " << arr[mid] << " " << target <<"\n";
//    }
//    assert((arr[mid]==target) ||((arr[mid]>target) &&((mid==0)||(arr[mid-1]<target))));
    return mid;
}

/*
 * Searches array from start, start + size for element greater or equal to
 * than target returns start + offset*/
int binarySearchFirstElementGreaterTarget(NODETYPE *arr,int start, NODETYPE size, int target){

    if(arr[start]>target)return start;
    if(size<linearSearchThresold){
        for(int i=start;i<size;i++){
            if(arr[i]>target)return i;
        }
        return size;
    }
    int end = size-1;
    int mid;
    assert(size>0);
    int ans = -1;
    while(start <= end) {
        mid = (start + end)/2;

        if(arr[mid]<= target){
            start = mid +1;
        }else{
            ans = mid;
            end = mid - 1;
        }
    }
    if(ans==-1)ans = size;
    mid = ans;
    return mid;
}


int gallopingSearchFirstElementGreaterOrEqualTarget(NODETYPE *arr, int start, NODETYPE size, int target) {
    int end = size-1;
    if(start>end){
        return end+1;
    }

    if(arr[start] >= target)return start;
    if(start == end) return start+1;

    if(arr[start+1] >=target)return start+1;
    if(start+1 == end)return start+2;

    int t = 1;
    int max = size - start - 1;


    while(t <= max){
        if(arr[start+t]==target)return start+t;
        if(arr[start+t]<target)t = t<<1;
        else{
//            cout << "space reduction " << t << " vs " << size-start <<"\n";
            return binarySearchFirstElementGreaterOrEqualTarget(arr,start+t>>1,start+t+1,target);

//            return binarySearchFirstElementGreaterOrEqualTarget(arr,start+t>>1,start+t +1,target);
        }
    }
    return binarySearchFirstElementGreaterOrEqualTarget(arr,start + t>>1,size,target);
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


/* Experiment:
 * Evaluate performance of binary search vs gallopping search for youtube graph.
 * Square query:
 * a. binary search + naive intersection : 38.7, 35.47, 35.99
 * b. gallopping search + naive intesection : 38.678 , 39.211, 39.6
 * Conclusion: Not a huge and actually slightly worse.
 * Will be using binary evaluation
 * */
int searchFirstElementGreaterOrEqualTarget(NODETYPE *arr,int start, NODETYPE size, int target){
    return binarySearchFirstElementGreaterOrEqualTarget(arr,start,size,target);
//    return gallopingSearchFirstElementGreaterOrEqualTarget(arr,start,size,target);
}
