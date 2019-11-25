//
// Created by sandeep on 24/11/19.
//

#include "graph.h"
#include<cstring>
#include <fstream>
#include <assert.h>

/* These functions are used to explore the cost of of using a bitvectors
 * Experiment.
 * (Node 1, neighbours [a1....a_n], each having a degree d_n
 * all connected to the same elements {d1....d_n}
 * Time to find out all squares starting from node 1.
 * Prove that there exists a cartesian product which can be avoided.
 * a_n | d_n | Bit      | naive + simd | Speedup of Bit
 * 10  | 10  | 9e-6     | 7e-6         | ~1
 * 100 | 10  | .000132  | .0005        | x5
 * 1000| 10  | .009     | .045         | x5
 * 10  | 100 | 1.7e-5   | 3.1e-5       | x2
 * 10  | 1000| .000151  | .000321      | x2
 * 100 | 100 | .000481  | .00339       | x10
 * 100 | 1000| .003     | .029         | x10
 * Conclusion there is definetely a speed up in the best case x10
 */
void generateGraphAllCommon(int a_n, int d_n, string outfile){
    int total_vertex = a_n + 1 + d_n;
    int total_edges = a_n + a_n*d_n;
    std::ofstream of(outfile);
    of << total_vertex << " " << total_edges << "\n";
    for(int i=1;i<=a_n;i++){
        of << 0 << " " << i <<"\n";
    }
    for(int i=1;i<=a_n;i++){
        for(int j=1;j<=d_n;j++){
            of << i << " " << a_n+j<< "\n";
        }
    }
    of.close();
}

/*  Experiment:
 *  Cost of finding out all square starting from vertex 0.
 *  Adj(0) -> {a_1,a_2.....a_n}
 *  Degree(a_i)= d_n
 *  However a_i /\ a_j = null set. Nothing in common
 *  a_n | d_n | Naive |BitVector * Time to find out all squares starting from node 1.
 * Prove that there exists a cartesian product which can be avoided.
 * Evaluate various costs of bit matrix construction.
 * a_n | d_n  | Naive+simd | Bit-Naive    | Bit-MinHeap
 * 10  | 1000 | .00029     | .00016 [2x]  | .000449
 * 100 | 10   | .000335    | .000263      | .000266 [.5x]
 * 100 | 100  | .0002      | .04          | .0016  [ .2x]
 * 100 | 1000 | .016       | .038  [BAD]  | .0043 [4x]
 * 1000| 10   | .034       | 2.47  [BAD]  | .0125 [2x]
 * Worst case:
 * Isolate cases where a_n > 100
 * Conclusion there is definetely a speed up in the best case x10 worst case 2x
 * 1. a_n is less<20 do not use a minheap for base construction
 * 2. When a_n is more than say 20 use a min heap */
void generateGraphNothingCommon(int a_n, int d_n, string outfile){
    int total_vertex = a_n + 1 + d_n;
    int total_edges = a_n + a_n*d_n;
    std::ofstream of(outfile);
    of << total_vertex << " " << total_edges << "\n";
    for(int i=1;i<=a_n;i++){
        of << 0 << " " << i <<"\n";
    }
    for(int i=1;i<=a_n;i++){
        for(int j=1;j<=d_n;j++){
            of << i << " " << a_n*i+j<< "\n";
        }
    }
    of.close();
}

/*
 * Lot of small degree vertexes and some large degree vertexes.
 * a_small | d_small | a_large | d_large | Naive | Bit | Performance
 * 100     |    100  |      5  |  1000   |  e-5  | e-6 | Bad
 * 100     |    100  |      5  |  10000  |  e-5  | e-6 | Bad
 *
 * Conclusion.
 * Highly skewed case has a 10 order detriment.
 *
 * */
void generateGraphSkewNothingCommon(int a_small, int d_small,
        int a_large, int d_large, string outfile){
    int total_vertex = a_small + a_large + 1 + a_small * d_small + a_large * d_large;
    int total_edges = a_small + a_large + a_small*d_small + a_large * d_large;
    std::ofstream of(outfile);
    of << total_vertex << " " << total_edges << "\n";
    for(int i=1;i<=a_small+a_large;i++){
        of << 0 << " " << i <<"\n";
    }
    for(int i=1;i<=a_small;i++){
        for(int j=1;j<=d_small;j++){
            of << i << " " << a_small*i+j<< "\n";
        }
    }

    for(int i=a_small+1;i<=a_small+a_large;i++){
        for(int j=1;j<=d_large;j++){
            of << i << " " << (a_small+a_large*i+j)<< "\n";
        }
    }
    of.close();
}

void experiment_code_to_compute_cost_model(){
//    int i = 100;
//    int j = 1000;
//    cout << "neighbours: " << i << " avg_degree:" << j <<"\n";
//    generateGraphNothingCommon(i,j,"data/data1.graph");
//    Graph * graph = new Graph("data/data1.graph");
//    naive_square_counting(graph);
//    bit_vector_based(graph);
}

/*
 * 3177751:133:100
3177758:140:106
3177762:142:109
3177763:108:114
3177779:108:115
3177784:111:101
3177793:109:115
3177799:106:92
3177806:106:100
3177807:156:101
3177819:119:117
3177823:117:101
3177825:126:93
3177844:133:111
3177854:107:107
3177869:184:105
3177879:148:108
3177880:144:102
3177911:154:101
3177918:113:116
3177946:111:109
3171247:307:326
3171253:305:384
3171254:302:382
3171389:351:380
3171390:302:377
3171488:303:371
3171629:304:368
3171851:304:318
3171886:307:317
3171887:304:316
3171981:366:379
3172063:307:314
3172064:305:313
3172072:302:313
3172104:367:378
3172106:305:348
3172107:302:312
3172142:305:310
3172149:301:310
3172153:306:347
3172154:307:343
3172188:305:344
3172272:370:377
3172303:370:376
3172304:366:375
3172305:306:341
3172346:368:374
3172387:370:373
3172388:369:372
3172389:368:371
3172443:366:370
3172444:367:369
3172445:365:368
3172446:365:367
3172478:361:366
3172479:365:365
3172480:365:364
3172481:363:363
3172514:358:362
3172515:361:361
3172516:362:360
3172517:361:359
3172541:355:358
3172542:356:357
3172543:358:356
3172544:356:355
3172545:355:355
3172580:353:354
3172581:354:352
3172582:352:351
3172583:350:351
3172584:349:350
3172585:349:348
3172586:349:347
3172587:348:346
3172588:347:345
3172590:306:333
3172640:345:344
3172641:319:343
3172642:344:342
3172643:342:341
3172644:342:340
3172645:342:339
3172646:341:338
3172647:338:337
3172648:337:336
3172649:338:335
3172650:336:335
3172651:335:334
3172652:333:333
3172653:333:332
3172654:332:331
3172655:331:330
3172656:330:329
3172657:329:328
3172658:328:327
3172659:327:326
3172660:326:325
3172661:325:324
3172662:323:323
3172663:323:322
3172664:322:321
3172665:321:320
3172701:317:319
3172702:316:318
3172703:319:317
3172704:318:316
3172705:317:315
3172706:315:314
3172707:315:313
3172708:314:312
3172709:313:311
3172710:312:310
3172711:311:309
3172712:309:308
3172713:309:307
3172714:307:306
3172715:307:305
3172716:305:304
3172717:304:303
3172718:303:302
3172719:302:301
3172720:301:300
3173057:301:320
3173125:302:317
3173154:302:316
3174158:421:144
3175068:337:176
3178143:521:69

 */