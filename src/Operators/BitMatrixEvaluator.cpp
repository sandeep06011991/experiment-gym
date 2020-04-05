//
// Created by sandeep on 27/3/20.
//

#include <iostream>
#include "BitMatrixEvaluator.h"
#include "timer.h"
#include <cstring>

inline int naivebitMark_intersect(NODETYPE* baseA, NODETYPE baseSize, NODETYPE* arrB,
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


BitMatrixEvaluator::BitMatrixEvaluator(Graph *graph,GHDNode *ghdNode, Trie * trie){
    b = new BitMatrix(graph,ghdNode);
    this->graph = graph;
    this->ghdNode = ghdNode;
    this->trie = trie;
    VEBuckets = (struct V_E_tuple **)malloc(buckets * sizeof(struct V_E_tuple));
    veBucketFill = (int *)malloc(sizeof(int) * buckets);
    for(int i=0;i<buckets;i++){
        VEBuckets[i] = (struct V_E_tuple *)malloc(MAX_BUCKET_FILL * sizeof(struct V_E_tuple));
        veBucketFill[i] = 0;
    }

    partials = (struct PEmbedding *)malloc(MAX_PEMBEDDINGS * sizeof(struct PEmbedding));
    VFTuples = (struct V_F_tuple *)malloc(MAX_V_F_TUPLE * sizeof(struct V_F_tuple));
    nbNodes = (NODETYPE *)malloc(sizeof(NODETYPE) * MAX_NB_NODES);
    candidateSets = (NODETYPE *)malloc(sizeof(NODETYPE)*10000);
}

void BitMatrixEvaluator::checkIfFitsInDatastructures( int levelNo, int startMetaBlock, int noBlocks) {
    TrieLevel * level = trie->levels[levelNo];
    int countEmbeddings = 0;
    for(int bid = 0;bid < noBlocks;bid ++ ){
        countEmbeddings = countEmbeddings + level->meta_blocks[startMetaBlock + bid].block_size;
    }
    int vertexCount = countEmbeddings * ghdNode->getNoIncidentAttributes(levelNo+1);
    assert(vertexCount < MAX_V_F_TUPLE);
    assert(vertexCount < MAX_V_E_TUPLE);
    assert(countEmbeddings < MAX_PEMBEDDINGS);
    assert( ghdNode->getNoIncidentAttributes(levelNo+1) < MAX_NEIGHBOURS );
}

void inline BitMatrix::insertIntoBitMatrix(NODETYPE id){
//    assert(remapping[id] <= prevOffset);
//    if(remapping[id]<=prevOffset){
        noElementsInBitMatrix ++;
        assert(noElementsInBitMatrix * bitArraySizeInChar < MAX_BITMATRIX_SIZE);
        NODE nd = ndArray[id];
        remapping[id] = prevOffset + noElementsInBitMatrix;
        int pos = remapping[id] - prevOffset;
        unsigned  char * bitMatrixArray = &bitMatrix[bitArraySizeInChar * pos];
        memset(bitMatrixArray, 0, sizeof(unsigned char) * bitArraySizeInChar);
        naivebitMark_intersect(&edgeArray[anchorNode.offset_plus],
                           anchorNode.size_plus, &edgeArray[nd.offset_plus], nd.size_plus,
                           bitMatrixArray);

//    }
}

inline int BitMatrix::expandBitArrayIntoResultVector(NODETYPE *resultArray){
    int size = 0;
    assert(anchorNode.size_plus<10000);
    int r = 0; bool atleastOnce = false;
    for(int i=0; i< (anchorNode.size_plus/8)+1; i++){
        if(defaultBitArray[i] == 0){
//            r++;
            continue;
        }
        atleastOnce = true;
        for(int j=7;j>=0;j--){
            if(i*8+7-j > anchorNode.size_plus)continue;
            if(defaultBitArray[i] & 1U << (j)){
                resultArray[size] = edgeArray[anchorNode.offset_plus + i*8+7-j];
                size ++;
            }
        }
    }

//    cout << r * 100.0 / ((anchorNode.size_plus/8)+1) <<" ";
    return size;
}

inline int BitMatrix::sparseBitIntersect(NODETYPE *nds, int noNodes, NODETYPE *resultArray){
    NODE nd1 = ndArray[nds[0]];
    int size = nd1.size_plus;
    if(size == 0) return 0;
    assert ( noNodes>1 );
    memset(defaultBitArray, 0xFF, bitArraySizeInChar);

//    NODETYPE bitNodes[noNodes];
    NODETYPE offsets[noNodes];
    unsigned int * intPointers[noNodes];
    int n = 0;

    for(int i=0; i<noNodes; i++){
        if(nds[i] == anchor)continue;
        if(remapping[nds[i]] <= prevOffset){
            insertIntoBitMatrix(nds[i]);
        }
//        bitNodes[n] = nds[i];
        offsets[n] = remapping[nds[i]]-prevOffset;
        intPointers[n] = (unsigned int *) &bitMatrix[offsets[n]*bitArraySizeInChar];
        n++;
    }
    NODETYPE noZeros[bitArraySizeInChar];
    int noZeroCount =0;
    unsigned int * intBitArray = (unsigned int *)defaultBitArray;
    for(int i=0;i< bitArraySizeInChar/4;i=i+1){
        for(int j=0;j<n;j++){
            intBitArray[i]= intBitArray[i] & intPointers[j][i];
        }
        if( intBitArray[i]!=0){
            noZeros[noZeroCount] = i*4;
            noZeros[noZeroCount+1] = 4*i+1;
            noZeros[noZeroCount+2] = 4*i+2;
            noZeros[noZeroCount+3] = 4*i+3;
            noZeroCount = noZeroCount + 4;
        }
    }
    for(int i=(bitArraySizeInChar/4)*4; i < bitArraySizeInChar;i++){
        for(int j=0;j<n;j++){
            defaultBitArray[i] = defaultBitArray[i] & bitMatrix[offsets[j]*bitArraySizeInChar + i];
        }
        if(defaultBitArray[i]!=0){
            noZeros[noZeroCount] = i;
            noZeroCount ++;
        }
    };
//    Default intersection method
//    for(int i=0;i < bitArraySizeInChar;i++){
//        for(int j=0;j<n;j++){
//            defaultBitArray[i] = defaultBitArray[i] & bitMatrix[offsets[j]*bitArraySizeInChar + i];
//        }
//    }
    if(noZeroCount==0)return 0;
    size = 0;
    for(int ii=0; ii< noZeroCount; ii++){
        int i = noZeros[ii];
        int limit = bitDictionary->dic[defaultBitArray[i]];
        for(int k=0;k<limit;k++){
            int j = bitDictionary->offsets[defaultBitArray[i] * 8 + k];
            if(i*8+7-j > anchorNode.size_plus)continue;
            resultArray[size] = edgeArray[anchorNode.offset_plus + i*8+7-j];
            size ++;
        }
    }

    return size;
}

inline int BitMatrix::bitIntersect(NODETYPE *nds, int noNodes, NODETYPE *resultArray) {
    NODE nd1 = ndArray[nds[0]];
    int size = nd1.size_plus;
    if(size == 0) return 0;
    assert(noNodes>1);
    memset(defaultBitArray, 0xFF, bitArraySizeInChar);
    char aczero = 0x00;
    int aczeroI = 0;
    for(int j=0;j<noNodes;j++){
//      perform intersection and get final list of candidates.
        NODE nd= ndArray[nds[j]];
        if(nd.id == anchor)continue;

        if(remapping[nd.id] <= prevOffset){
//            start_timer(SIMDINTERSECTION);
            insertIntoBitMatrix(nd.id);
//            stop_timer(SIMDINTERSECTION);
        }
        int offset = remapping[nd.id]-prevOffset;
//      offload this line to another entity.
        unsigned char * bitArray = &bitMatrix[offset*bitArraySizeInChar];
        unsigned int * a = (unsigned int *)defaultBitArray;
        unsigned int * b = (unsigned int *)bitArray;
        char zero = 0x00;
        int zeroI = 0;
//        if(bitArraySizeInChar > 64){
//            cout << bitArraySizeInChar << " ";
//        }
//        for(int i=0;i < 4 && i < bitArraySizeInChar;i++){
//            defaultBitArray[i] = defaultBitArray[i] & bitArray[i];
//            zero = zero | bitArray[i];
//        }
//        for(int i=1;i<bitArraySizeInChar/4;i++){
//            a[i] = a[i] & b[i];
//            zeroI = zeroI | b[i];
//        }
//        for(int i=(bitArraySizeInChar/4)*4; i < bitArraySizeInChar;i++){
//            defaultBitArray[i] = defaultBitArray[i] & bitArray[i];
//            zero = zero | bitArray[i];
//        }

//        for(int i=0;i < 4 && i < bitArraySizeInChar;i++){
//            defaultBitArray[i] = defaultBitArray[i] & bitArray[i];
//            zero = zero | bitArray[i];
//        }
//        for(int i=1;i<bitArraySizeInChar/4;i++){
//            a[i] = a[i] & b[i];
//            zeroI = zeroI | b[i];
//        }
//        for(int i=(bitArraySizeInChar/4)*4; i < bitArraySizeInChar;i++){
//            defaultBitArray[i] = defaultBitArray[i] & bitArray[i];
//            zero = zero | bitArray[i];
//        }
        for(int i=0;i < bitArraySizeInChar;i++){
            defaultBitArray[i] = defaultBitArray[i] & bitArray[i];
            zero = zero | defaultBitArray[i];
        }
        if(zero == 0x00 && zeroI == 0){
            return  0;
        }
    }
//    start_timer(ADGLISTINTERSECTION);
    int s = 0;
//    if(count){
//        for(int i=0;i<bitArraySizeInChar;i++){
//           s = s + bitDictionary->dic[(int)defaultBitArray[i]];
//        }
//        return s;
//    }

    s = expandBitArrayIntoResultVector(resultArray);

    return s;
}

int BitMatrixEvaluator::process(int level, int startMetaBlock, int noBlocks){
    checkIfFitsInDatastructures(level, startMetaBlock, noBlocks);
    // populate key datastructures.
    NODETYPE nbs[MAX_NEIGHBOURS];
    totalNoPartials = 0;
    noVFTuples = 0;
    noNbNodes = 0;
    int s = 0;
    int nbSize = ghdNode->getNoIncidentAttributes(level+1);
    assert(nbSize < MAX_NEIGHBOURS);
    NODETYPE prev[nbSize];
    int prevBid[nbSize];
    for(int i=0;i<nbSize;i++){
        prev[i] = -1;
        prevBid[nbSize] = -1;
    }
    memset(veBucketFill,0,sizeof(int) * buckets);
//    return 0;

    for(int bid=startMetaBlock;bid < startMetaBlock + noBlocks; bid ++){
        Level_Meta *lm =  &trie->levels[level]->meta_blocks[bid];
        if(lm->block_size==0)continue;

//        trie->getIncidentNbs(nbs, ghdNode->getNPlusNeighbourhood(level+1), level, *lm, 0);
        trie->getIncidentNbs(nbs, ghdNode->getNPlusNeighbourhood(level+1), level, *lm, 0);
        for(int offset = 0;offset < lm->block_size; offset ++){
//            FIXME: ONLY FOR CLIQUES BUT CAN BE EASILY EXTENDED
            nbs[level] = trie->levels[level]->data[lm->block_start + offset];
//            trie->debugEmbedding(level,*lm, offset);
            assert(lm!= nullptr);
            partials[totalNoPartials].meta = lm;
            partials[totalNoPartials].offset = offset;
            partials[totalNoPartials].isDone = false;
            partials[totalNoPartials].nbStart = noNbNodes;
//            trie->getIncidentNbs(nbs, ghdNode->getNPlusNeighbourhood(level+1), level, *lm, offset);

            for(int i=0;i<nbSize;i++){
                nbNodes[noNbNodes] = nbs[i];
                noNbNodes ++;
                assert(noNbNodes < MAX_NB_NODES);
                int bucketId = nbs[i] % buckets;
                struct V_E_tuple * v =  &VEBuckets[bucketId][veBucketFill[bucketId]];
                if(prev[i]==-1){
                    v->vid = nbs[i];
                    v->start = totalNoPartials;
                    v->size = 1;
                    prev[i] = veBucketFill[bucketId];
                    prevBid[i] = bucketId;
                    veBucketFill[bucketId] ++;
                    continue;
                }else{
                    if((prevBid[i] == bucketId) &&
                        (veBucketFill[bucketId]== prev[i]) &&
                            (VEBuckets[bucketId][prev[i]].vid == nbs[i])){
                        VEBuckets[prevBid[i]][prev[i]].size ++;
                    }else{
                        v->vid = nbs[i];
                        v->start = totalNoPartials;
                        v->size =1;
                        prev[i] = veBucketFill[bucketId];
                        prevBid[i] = bucketId;
                        veBucketFill[bucketId] ++;
                    }
                }
                assert(veBucketFill[bucketId] < MAX_BUCKET_FILL);
            }
            totalNoPartials ++;
        }
    }
    // Check offsets.

//    start_timer(BITMATRIXCONSTRUCTION);
    for(int i=0;i < buckets;i++){
        sort(VEBuckets[i],VEBuckets[i] + veBucketFill[i],[&](struct V_E_tuple a, struct V_E_tuple b){
            return a.vid < b.vid;
        });
    }

    for(int bid=0;bid < buckets; bid ++){
        if(veBucketFill[bid]==0)continue;
        VFTuples[noVFTuples].vid = VEBuckets[bid][0].vid;
        VFTuples[noVFTuples].start = 0;
        VFTuples[noVFTuples].freq = VEBuckets[bid][0].size;
        noVFTuples ++;
        for(int i=1;i < veBucketFill[bid];i++){
            if(VFTuples[noVFTuples-1].vid == VEBuckets[bid][i].vid){
                VFTuples[noVFTuples-1].freq = VFTuples[noVFTuples-1].freq + VEBuckets[bid][i].size;
            }else{
                VFTuples[noVFTuples].vid = VEBuckets[bid][i].vid;
                VFTuples[noVFTuples].start = i;
                VFTuples[noVFTuples].freq = VEBuckets[bid][i].size;
                noVFTuples ++;
            }
        }
    }
    //

    sort(VFTuples,VFTuples + noVFTuples, [&](struct V_F_tuple a, struct V_F_tuple b){
        return a.freq > b.freq;
    });

    int size;
    for(int i=0; i < noVFTuples;i++){
        b->setAnchor(VFTuples[i].vid,level);
        int j = 0;
        int f_tuple_offset = 0;
        int bid = VFTuples[i].vid % buckets;
        while(j < VFTuples[i].freq){
            V_E_tuple *ve = &VEBuckets[bid][VFTuples[i].start + f_tuple_offset];
            for(int poffset = 0; poffset < ve->size; poffset ++){
                PEmbedding* pe = &partials[ve->start + poffset];
                if(pe->isDone)continue;
                pe->isDone = true;
                size = 0;
                size = b->sparseBitIntersect(&nbNodes[pe->nbStart], ghdNode->getNoIncidentAttributes(level+1), candidateSets);
                if(size == 0)continue;
                if(level == ghdNode->getNoAttributes()-2){
                    s = s + size;
                }else{
                    s = s + size;
                    struct Level_Meta lm;
                    lm.parent = pe->meta;
                    assert(lm.parent != nullptr);
                    lm.parent_offset = pe->offset;
    //                cout <<"inserting " << lm.parent <<"\n";
                    trie->appendTrieBlockToLevel(level+1, &lm, candidateSets, size);
                }
            }
            f_tuple_offset ++ ;
            j = j + ve->size;
        }

    }
    return  s;
}
