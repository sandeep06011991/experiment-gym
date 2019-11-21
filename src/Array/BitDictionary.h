//
// Created by sandeep on 15/11/19.
//

#ifndef V3_BITDICTIONARY_H
#define V3_BITDICTIONARY_H

// Allows to read properties of 0-256 bit of information
class BitDictionary {

    int dic[256];
public:
    BitDictionary();

    int countSetBits(unsigned char t);

    void visualize();

};


#endif //V3_BITDICTIONARY_H
