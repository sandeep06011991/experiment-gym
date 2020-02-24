//
// Created by sandeep on 15/11/19.
//

#ifndef V3_BITDICTIONARY_H
#define V3_BITDICTIONARY_H

// Allows to read properties of 0-256 bit of information
class BitDictionary {


public:
    int dic[256];

    BitDictionary();

    int countSetBits(unsigned char t);

    void visualize();

};

inline int countSetBitsInline(int *dic, unsigned char t);

#endif //V3_BITDICTIONARY_H
