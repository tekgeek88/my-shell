#include <stdio.h>
#include <time.h>

int main() {

    int msec = 0, trigger = 10000; /* 10ms */
    clock_t before = clock();
    int iterations = 0;

    fprintf(stdout, "Running for 10 seconds\n");
    fflush(stdout);

    do {
        clock_t difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
        iterations++;
    } while ( msec < trigger );

    printf("\n");
    printf("Time taken %d seconds %d milliseconds (%d iterations)\n",
           msec/1000, msec%1000, iterations);

    printf("\n\nDone Counting\n");
    return 0;
}