//
// Created by Miro Gohacki and Adrien Monks on 10/4/23.
//

#include <stdio.h>
#include <pthread.h>

#define NUMVALS (1024*1024)
#define NUMTHREADS 8

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
        if(gvals[i] > data->threshold){
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

//---------------------------------------------------------------------------

int main() {
    // Initialize buffer with random numbers
    int i1, i2;
    float f1, f2;
    for (int i = 0; i < NUMVALS; i++) {
        i1 = prand(100);
        i2 = prand(100);
        f1 = (float)i1 / 100.0;
        f2 = (float)i2 / 100.0;
        gvals[i] = f1 / (1.0 + f2);
    }

    // Set threshold value
    float threshold = 0.5;

    // Create array of CountInfo structs
    CountInfo info[NUMTHREADS];

    // Calculate range of each thread
    int chunkSize = NUMVALS / NUMTHREADS;
    for (int i = 0; i < NUMTHREADS; i++) {
        info[i].startIndex = i * chunkSize;

        if (i == NUMTHREADS - 1){
            info[i].endIndex = NUMVALS;
        }
        else{
            info[i].endIndex = (i + 1) * chunkSize;
        }

        info[i].threshold = threshold;
        info[i].count = 0;
    }

    // Create array of pthread_t vars
    pthread_t threads[NUMTHREADS];

    // Create and run threads
    for (int i = 0; i < NUMTHREADS; i++) {
        pthread_create(&threads[i], NULL, doCount, &info[i]);
    }

    // Join all threads
    for (int i = 0; i < NUMTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calculate total
    int totalCount = 0;
    for (int i = 0; i < NUMTHREADS; i++) {
        totalCount += info[i].count;
    }

    // Print total
    printf("Total values greater than %.2f: %d\n", threshold, totalCount);

    return 0;
}
