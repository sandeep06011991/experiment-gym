//
// Created by sandeep on 15/11/19.
//

#include "intersection.h"
#include <immintrin.h>
#include "iostream"

/* Intersects set of 4 sorted integers pointed to by base and A.
 * */
__m128i intersect2setsOf4Integers(NODETYPE *base, NODETYPE *A){
    __m128i baseV = _mm_lddqu_si128(reinterpret_cast<const __m128i *>((__m128i *)base));
    __m128i AV = _mm_lddqu_si128(reinterpret_cast<const __m128i *>((__m128i *)A));

    __m128i shuffle1 =  _mm_shuffle_epi32 (AV, _MM_SHUFFLE(0,1,2,3));
    __m128i shuffle2 =  _mm_shuffle_epi32 (AV, _MM_SHUFFLE(1,2,3,0));
    __m128i shuffle3 =  _mm_shuffle_epi32 (AV, _MM_SHUFFLE(2,3,0,1));
    __m128i shuffle4 =  _mm_shuffle_epi32 (AV, _MM_SHUFFLE(3,0,1,2));

    __m128i i1= _mm_cmpeq_epi32 (baseV, shuffle1);
    __m128i i2= _mm_cmpeq_epi32 (baseV, shuffle2);
    __m128i i3= _mm_cmpeq_epi32 (baseV, shuffle3);
    __m128i i4= _mm_cmpeq_epi32 (baseV, shuffle4);
    __m128i ii = i1 | i2 | i3| i4;
    const __m128i K32 = _mm_setr_epi32(1, 1, 1, 1);
    __m128i res = ii & K32;
    return res;
}

int intersectSets8IntegerAttime(NODETYPE *base, int baseSize, NODETYPE *A, int ASize){
    int size = (baseSize-1)/8+1;
    int cbase= 0;
    int cAsize = 0;

    int r = 0;
//    int c_array[4] = {0,0,0,0};
    __m256i counter =_mm256_setr_epi32  (0,0,0,0,0,0,0,0);
//  Checks if there is a possible 4 tuple to intersect

    const __m256i K32 = _mm256_setr_epi32 (1,1,1,1,1, 1, 1, 1);
    while(cbase+7 < baseSize && cAsize+7 < ASize){
//        assert(i>>2 < size);
        __m256i baseV = _mm256_lddqu_si256(reinterpret_cast<const __m256i *>((__m256i *)&base[cbase]));
        __m128i AV1 = _mm_lddqu_si128(reinterpret_cast<const __m128i *>((__m128i *)&A[cAsize]));
        __m128i AV2 = _mm_lddqu_si128(reinterpret_cast<const __m128i *>((__m128i *)&A[cAsize+4]));

//        __m256i AV = _mm256_lddqu_si256(reinterpret_cast<const __m256i *>((__m256i *)&A[cAsize]));
        __m256i AV256_1 = _mm256_broadcastsi128_si256  (AV1);
        __m256i AV256_2 = _mm256_broadcastsi128_si256  (AV2);

        __m256i shuffle1 =  _mm256_shuffle_epi32 (AV256_1, _MM_SHUFFLE(0,1,2,3));
        __m256i shuffle2 =  _mm256_shuffle_epi32 (AV256_1, _MM_SHUFFLE(1,2,3,0));
        __m256i shuffle3 =  _mm256_shuffle_epi32 (AV256_1, _MM_SHUFFLE(2,3,0,1));
        __m256i shuffle4 =  _mm256_shuffle_epi32 (AV256_1, _MM_SHUFFLE(3,0,1,2));

        __m256i shuffle5 =  _mm256_shuffle_epi32 (AV256_2, _MM_SHUFFLE(0,1,2,3));
        __m256i shuffle6 =  _mm256_shuffle_epi32 (AV256_2, _MM_SHUFFLE(1,2,3,0));
        __m256i shuffle7 =  _mm256_shuffle_epi32 (AV256_2, _MM_SHUFFLE(2,3,0,1));
        __m256i shuffle8 =  _mm256_shuffle_epi32 (AV256_2, _MM_SHUFFLE(3,0,1,2));

        __m256i i1= _mm256_cmpeq_epi32  (baseV, shuffle1);
        __m256i i2= _mm256_cmpeq_epi32  (baseV, shuffle2);
        __m256i i3= _mm256_cmpeq_epi32  (baseV, shuffle3);
        __m256i i4= _mm256_cmpeq_epi32  (baseV, shuffle4);
        __m256i i5= _mm256_cmpeq_epi32  (baseV, shuffle5);
        __m256i i6= _mm256_cmpeq_epi32  (baseV, shuffle6);
        __m256i i7= _mm256_cmpeq_epi32  (baseV, shuffle7);
        __m256i i8= _mm256_cmpeq_epi32  (baseV, shuffle8);

        __m256i ii = i1 | i2 | i3| i4 |i5| i6 |i7 |i8;
        __m256i res = ii & K32;
        counter = _mm256_add_epi32 (counter , res);
        if(base[cbase+7] < A[cAsize+7]){
                cbase = cbase+8;
        }else {
                cAsize = cAsize + 8;
        }
    }
    r = _mm256_extract_epi32 (counter,0) +  _mm256_extract_epi32 (counter,1) +
            _mm256_extract_epi32(counter,2)  + _mm256_extract_epi32(counter,3) +
            _mm256_extract_epi32 (counter,4) +  _mm256_extract_epi32 (counter,5) +
            _mm256_extract_epi32(counter,6)  + _mm256_extract_epi32(counter,7) ;
//    r = c_array[0] + c_array[1] + c_array[2] + c_array[3];
    while(cbase<baseSize && cAsize < ASize){
        if(base[cbase]==A[cAsize]){
            r ++ ;
            cbase ++;
            cAsize ++;
            continue;
        }
        if(base[cbase]<A[cAsize]){
            cbase ++ ;
        }else{
            cAsize ++;
        }
    }
    return r;
}

int intersectSets4IntegerAttime(NODETYPE *base, int baseSize, NODETYPE *A, int ASize){
    int size = (baseSize-1)/8+1;
    int cbase= 0;
    int cAsize = 0;

    int r = 0;
//    int c_array[4] = {0,0,0,0};
    __m128i counter = _mm_setr_epi32 (0,0,0,0);
//  Checks if there is a possible 4 tuple to intersect

    const __m128i K32 = _mm_setr_epi32(1, 1, 1, 1);
    while(cbase+3 < baseSize && cAsize+3 < ASize){
//        assert(i>>2 < size);
//        __m128i baseV = _mm_lddqu_si128((__m128i *)&base[cbase]);
//        __m128i AV = _mm_lddqu_si128((__m128i *)&A[cAsize]);
        __m128i baseV = _mm_lddqu_si128(reinterpret_cast<const __m128i *>((__m128i *)&base[cbase]));
        __m128i AV = _mm_lddqu_si128(reinterpret_cast<const __m128i *>((__m128i *)&A[cAsize]));
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
        counter = _mm_add_epi32 (counter , res);
        if(base[cbase+3] < A[cAsize+3]){
            cbase = cbase+4;
        }else {
            cAsize = cAsize + 4;
        }
    }
    r = _mm_extract_epi32 (counter,0) + _mm_extract_epi32 (counter,1) +
        _mm_extract_epi32 (counter,2)  + _mm_extract_epi32 (counter,3) ;
//    r = c_array[0] + c_array[1] + c_array[2] + c_array[3];
    while(cbase<baseSize && cAsize < ASize){
        if(base[cbase]==A[cAsize]){
            r ++ ;
            cbase ++;
            cAsize ++;
            continue;
        }
        if(base[cbase]<A[cAsize]){
            cbase ++ ;
        }else{
            cAsize ++;
        }
    }
    return r;
}