//
// Created by Miro Gohacki on 10/4/23.
//

#ifndef CS3010ASSIGNMENT3_MTCOUNT_MGOHACKI_AMONKS_H
#define CS3010ASSIGNMENT3_MTCOUNT_MGOHACKI_AMONKS_H

#define NUMVALS (1024*1024)
#define NUMTHREADS 4

typedef struct {
    int startIndex; // start index at which to start looking
    int endIndex; // end index at which to stop looking
    float threshold; // threshold value
    int count; // result: #values that are greater than threshold
} CountInfo;

#endif //CS3010ASSIGNMENT3_MTCOUNT_MGOHACKI_AMONKS_H
