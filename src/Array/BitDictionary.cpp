//
// Created by sandeep on 15/11/19.
//

#include "BitDictionary.h"

#include "iostream"

BitDictionary::BitDictionary(){
    for(int i=0;i<256;i++){
        int t=0;
        for(int j=0;j<8;j++){
            if((i & (1U<<j))!=0)t++;
        }
        dic[i] = t;
    }
    populateOffsets();
}

int BitDictionary::countSetBits(unsigned char t){
    return dic[(int)t];
};

void BitDictionary::visualize(){
    for(int i=0;i<256;i++){
        std::cout << i << " " << dic[i] << "\n";
    }
}

inline int countSetBitsInline(int *dic, unsigned char t){
    return dic[(int)t];
}