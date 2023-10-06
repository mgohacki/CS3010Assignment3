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
    data->count = count;
    pthread_exit(NULL);
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
    // Initialize the global buffer with pseudo-random numbers
    int i1, i2;
    float f1, f2;
    for (int i = 0; i < NUMVALS; i++) {
        i1 = prand(100);
        i2 = prand(100);
        f1 = (float)i1 / 100.0;
        f2 = (float)i2 / 100.0;
        gvals[i] = f1 / (1.0 + f2);
    }

    // Set the threshold value
    float threshold = 0.5;

    // Create an array of CountInfo structures
    CountInfo info[NUMTHREADS];

    // Create an array of pthread_t variables
    pthread_t threads[NUMTHREADS];

    // Calculate the range for each thread
    int chunkSize = NUMVALS / NUMTHREADS;
    for (int i = 0; i < NUMTHREADS; i++) {
        info[i].startIndex = i * chunkSize;
        info[i].endIndex = (i == NUMTHREADS - 1) ? NUMVALS - 1 : (i + 1) * chunkSize - 1;
        info[i].threshold = threshold;
        info[i].count = 0;
    }

    // Create threads
    for (int i = 0; i < NUMTHREADS; i++) {
        pthread_create(&threads[i], NULL, doCount, &info[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUMTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calculate the total count
    int totalCount = 0;
    for (int i = 0; i < NUMTHREADS; i++) {
        totalCount += info[i].count;
    }

    // Print the total count
    printf("Total count of values greater than %.2f: %d\n", threshold, totalCount);

    return 0;
}
