//
// Created by Miro Gohacki on 10/4/23.
//

#include <stdio.h>
#define NUMVALS (1024*1024)
#define NUMTHREADS 4

typedef struct {
    int startIndex; // start index at which to start looking
    int endIndex; // end index at which to stop looking
    float threshold; // threshold value
    int count; // result: #values that are greater than threshold
} CountInfo;

float gvals[NUMVALS];

void *doCount(void *){

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
