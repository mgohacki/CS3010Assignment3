//
// Created by Miro Gohacki on 10/4/23.
//

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMVALS (1024*1024)
#define NUMTHREADS 1

typedef struct {
    int startIndex; // start index at which to start looking
    int endIndex; // end index at which to stop looking
    float threshold; // threshold value
    int count; // result: #values that are greater than threshold
} CountInfo;

float gvals[NUMVALS];

void *doCount(void * param){
    CountInfo *data;
    int count = 0;
    data = (CountInfo *) param;
    for(int i = data->startIndex; i < data->endIndex;i++){
        if(gvals[i]>data->threshold){
            count++;
        }
    }
    printf("%d",count);
}

int prand() {
    static int p = 1;
    const int a = 105491;
    const int b = 733;
    const int c = 6619;
    const int base = 100;
    int val;
    p = p*a + b;
    p = p % c;
    val = p % base;
    return val;
}


typedef struct {
    int lowVal;
    int highVal;
    int maxVal;
} ThreadInfo;


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

int main() {
    CountInfo tdata[NUMTHREADS]; // holds data we want to give to each thread
    pthread_t tids[NUMTHREADS];    // thread identifier for child thread #1
    int maxVal;
    int i, pos, chunkSize;

    // initialize the array with random integers in the range 0..NUMVALS
    int i1, i2;
    float f1, f2;
    for (i=0; i<NUMVALS; ++i) {
        i1 = prand(100);
        i2 = prand(100);
        f1 = i1 / 100.0;
        f2 = i2 / 100.0;
        gvals[i] = f1 / (1.0 + f2);
    }

    // set up bounds for the threads
    chunkSize = NUMVALS / NUMTHREADS;
    pos = 0;
    for (i=0; i<NUMTHREADS; ++i) {
        tdata[i].lowVal = pos;
        tdata[i].highVal = tdata[i].lowVal + chunkSize;
        pos = pos + chunkSize + 1;
    }

    // adjust last region if necessary
    if (tdata[NUMTHREADS-1].highVal < NUMVALS-1)
        tdata[NUMTHREADS-1].highVal = NUMVALS - 1;
    else if (tdata[NUMTHREADS-1].highVal > NUMVALS-1)
        tdata[NUMTHREADS-1].highVal = NUMVALS - 1;

    // create child threads
    for (i=0; i<NUMTHREADS; ++i)
        pthread_create(&tids[i], NULL, doCount, &tdata[i]);

    // wait for the child threads to terminate
    for (i=0; i<NUMTHREADS; ++i)
        pthread_join(tids[i], NULL);

    // gather data from the individual results
    maxVal = tdata[0].maxVal;
    for (i=1; i<NUMTHREADS; ++i) {
        if (tdata[i].maxVal > maxVal)
            maxVal = tdata[i].maxVal;
    }

    printf("overall max is %d\n", maxVal);
    return 0;
} // main()