//
// Created by sandeep on 15/11/19.
//
#include <bitset>
#include <cstring>
#include <assert.h>
#include "iostream"
#include "stdio.h"

#ifndef V3_BITDICTIONARY_H
#define V3_BITDICTIONARY_H

// Allows to read properties of 0-256 bit of information
class BitDictionary {


public:
    int dic[256];

    int offsets[8*256];

    void populateOffsets(){
        memset(offsets,0,sizeof(int) * 8*256);
        for(int i=0;i<256;i++){
            char t = (unsigned char)i;
            int c = 0;
            for(int j=7;j>=0;j--){
                if(1U<<j & t){
                    offsets[i*8+c] = j;
                    c++;
                }
            }
            assert(c == dic[i]);
        }
    }

    BitDictionary();

    int countSetBits(unsigned char t);

    void visualize();

};

inline int countSetBitsInline(int *dic, unsigned char t);

#endif //V3_BITDICTIONARY_H
