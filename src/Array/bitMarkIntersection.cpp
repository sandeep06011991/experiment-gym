//
// Created by sandeep on 21/11/19.
//

#include "intersection.h"
#include "ArrayUtilities.h"
#include "immintrin.h"

int simd4bitMark_intersect(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB,
                           NODETYPE sizeB ,unsigned char *bitV){
    int size = (baseSize-1)/8+1;
    int cbase= 0;
    int cAsize = 0;

    int r = 0;
//    int c_array[4] = {0,0,0,0};
    __m128i counter = _mm_setr_epi32 (0,0,0,0);
//  Checks if there is a possible 4 tuple to intersect

    const __m128i K32 = _mm_setr_epi32(1, 1, 1, 1);
    while(cbase+3 < baseSize && cAsize+3 < sizeB){
//        assert(i>>2 < size);
//        __m128i baseV = _mm_lddqu_si128((__m128i *)&base[cbase]);
//        __m128i AV = _mm_lddqu_si128((__m128i *)&A[cAsize]);
        __m128i baseV = _mm_lddqu_si128(reinterpret_cast<const __m128i *>((__m128i *)&baseA[cbase]));
        __m128i AV = _mm_lddqu_si128(reinterpret_cast<const __m128i *>((__m128i *)&arrB[cAsize]));
        __m128i shuffle1 =  _mm_shuffle_epi32 (AV, _MM_SHUFFLE(0,1,2,3));
        __m128i shuffle2 =  _mm_shuffle_epi32 (AV, _MM_SHUFFLE(1,2,3,0));
        __m128i shuffle3 =  _mm_shuffle_epi32 (AV, _MM_SHUFFLE(2,3,0,1));
        __m128i shuffle4 =  _mm_shuffle_epi32 (AV, _MM_SHUFFLE(3,0,1,2));

        __m128i i1= _mm_cmpeq_epi32 (baseV, shuffle1);
        __m128i i2= _mm_cmpeq_epi32 (baseV, shuffle2);
        __m128i i3= _mm_cmpeq_epi32 (baseV, shuffle3);
        __m128i i4= _mm_cmpeq_epi32 (baseV, shuffle4);
        __m128i ii = i1 | i2 | i3| i4;
        __m128i res = ii & K32;
//        bitV[cbase/8] = bitV[bp/8] | 1U << (7-(bp%8));
        counter = _mm_add_epi32 (counter , res);
        if(baseA[cbase+3] < arrB[sizeB+3]){
            cbase = cbase+4;
        }else {
            cAsize = cAsize + 4;
        }
    }

//    r = c_array[0] + c_array[1] + c_array[2] + c_array[3];
    while(cbase<baseSize && cAsize < sizeB){
        if(baseA[cbase]==arrB[cAsize]){
            bitV[cbase/8] = bitV[cbase/8] | 1U << (7-(cbase%8));
            r ++ ;
            cbase ++;
            cAsize ++;
            continue;
        }
        if(baseA[cbase]<arrB[cAsize]){
            cbase ++ ;
        }else{
            cAsize ++;
        }
    }
    return r;
}

int naivebitMark_intersect(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB,
        NODETYPE sizeB ,unsigned char *bitV){
    int bp = 0;
    int ap = 0;
    int t = 0;
    while((bp < baseSize) && (ap < sizeB)){
        if (baseA[bp] == arrB[ap]) {
            bitV[bp/8] = bitV[bp/8] | 1U << (7-(bp%8));
            t++;
            bp ++ ;
            ap ++ ;
            continue;
        }
        if (baseA[bp] < arrB[ap]) {
            bp ++;
        } else {
            ap ++;
        }
    }
    return t;
}


int binaryBitMark_intersectLargeBase(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB,
        NODETYPE sizeB ,unsigned char *bitV){
    // sizea >= sizeB
    int s = 0;
    int curr_a = 0;
    int curr_b = 0;
    while (curr_a < baseSize && curr_b < sizeB) {
        if (baseA[curr_a] == arrB[curr_b]) {
            bitV[curr_a/8] = bitV[curr_a/8] | 1U << (7-(curr_a%8));
            s ++ ;
            curr_a ++ ;
            curr_b ++ ;
            continue;
        }
        if (baseA[curr_a] < arrB[curr_b]) {
            curr_a = searchFirstElementGreaterOrEqualTarget(baseA,curr_a,baseSize,arrB[curr_b]);
//            curr_a = gallopingSearchFirstElementGreaterOrEqualTarget(arrA, curr_a, sizeA, arrB[curr_b]);
//            curr_a =  binarySearchFirstElementGreaterOrEqualTarget(arrA, curr_a, sizeA, arrB[curr_b]);
        } else {
            curr_b ++;
        }
    }
    return s;
}

int binaryBitMark_intersectLargeArray(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB,
                                     NODETYPE sizeB ,unsigned char *bitV){
    // sizea >= sizeB
    int s = 0;
    int curr_a = 0;
    int curr_b = 0;
    while (curr_a < baseSize && curr_b < sizeB) {
        if (baseA[curr_a] == arrB[curr_b]) {
            bitV[curr_a/8] = bitV[curr_a/8] | 1U << (7-(curr_a%8));
            s ++ ;
            curr_a ++ ;
            curr_b ++ ;
            continue;
        }
        if (baseA[curr_a] < arrB[curr_b]) {
            curr_a ++;
//            curr_a = gallopingSearchFirstElementGreaterOrEqualTarget(arrA, curr_a, sizeA, arrB[curr_b]);
//            curr_a =  binarySearchFirstElementGreaterOrEqualTarget(arrA, curr_a, sizeA, arrB[curr_b]);
        } else {
            curr_b = searchFirstElementGreaterOrEqualTarget(arrB,curr_b,sizeB,baseA[curr_a]);
        }
    }
    return s;
}


/* Hybrid-20 : 56s
 * Naive :   65s
 * Shaved around: 10 seconds.  */
int hybridBitMark_intersect(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB, NODETYPE sizeB ,unsigned char *pos){
    int tolerance =20;
    if(baseSize > 20*sizeB){
        return binaryBitMark_intersectLargeBase(baseA,baseSize,arrB,sizeB,pos);
    }
    if(sizeB > 20*sizeB){
        return binaryBitMark_intersectLargeArray(baseA,baseSize,arrB,sizeB,pos);
    }
//    return simd4bitMark_intersect(baseA,baseSize,arrB,sizeB,pos);
    return naivebitMark_intersect(baseA,baseSize,arrB,sizeB,pos);
}
