#include <stdio.h>
#include <stdlib.h>

int parse_num() {
    int n = 0;
    char r;
    while ((r = getc_unlocked(stdin)) >= '0') {
        n = n*10 + r-48;
    }
    return n;
}

int main() {
    // Get the number of cities
    int n = parse_num() + 1;
    int *distances = malloc(n*sizeof(int));
    int *penalties = malloc(n*sizeof(long));

    // Parse distances to each city
    int i;
    for (i = 0; i < n; i++) {
        distances[i] = parse_num();
    }

    // Calculate penalties for each city
    penalties[n-1] = 0;
    int j;
    for (i = n-2; i >= 0; i--) {
        long minsofar = 0x7fffffffffffffff;
        for (j = i+1; j < n; j++) {
            long inner = (400 - (distances[j]-distances[i]));
            long penalty = inner*inner + penalties[j];
            if (penalty < minsofar) {
                minsofar = penalty;
            }
        }
        penalties[i] = minsofar;
    }

    // Print the answer
    printf("%d\n", penalties[0]);

    return 0;
}
