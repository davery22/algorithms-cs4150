#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int tag;
    int r;
    int open;
    int k;
    int val;
} tb_el;

static int **values;
static tb_el *cache;
static int tag;
#define CACHE_MASK 0xffff

int cache_locate(int r, int open, int k, int *ret) {
    int hash = 5381;
    hash = (hash << 5) + hash + r;
    hash = (hash << 5) + hash + open;
    hash = (hash << 5) + hash + k;
    
    hash &= CACHE_MASK;

    while (cache[hash].tag == tag) {
        if (cache[hash].r == r && cache[hash].open == open && cache[hash].k == k) {
            *ret = hash;
            return 1;
        }
        hash = (hash + 91) & CACHE_MASK;
    }

    *ret = hash;
    return 0;
}

int cache_get(int r, int open, int k, int *ret) {
    if (cache_locate(r, open, k, ret)) {
        *ret = cache[*ret].val;
        return 1;
    }
    return 0;
}

void cache_put(int r, int open, int k, int value) {
    int loc;
    if (!cache_locate(r, open, k, &loc)) {
        cache[loc].tag = tag;
        cache[loc].r = r;
        cache[loc].open = open;
        cache[loc].k = k;
        cache[loc].val = value;
    }
}

int parse_num() {
    int n = 0;
    char r;
    while((r = getc_unlocked(stdin)) >= '0') {
        n = n*10 + r-'0';
    }
    return n;
}

static inline int max2(int a, int b) {
    return a > b ? a : b;
}

static inline int max3(int a, int b, int c) {
    return a > b ? a > c ? a : c : b > c ? b : c;
}

static inline int memorize(int r, int open, int k, int val) {
    cache_put(r, open, k, val);
    return val;
}

int maxValue(int r, int open, int k) {
    if (r < 0) {
        return 0;
    }

    int val;
    if (cache_get(r, open, k, &val)) {
        return val;
    }

    if (k < 0) {
        val = memorize(r, -1, k, values[r][0] + values[r][1] + maxValue(r-1, open, k));
        memorize(r, 0, k, val);
        memorize(r, 1, k, val);
        return val;
    }

    if (k == r) {
        if (open < 0) {
            return memorize(r, open, k, max2(values[r][0] + maxValue(r-1, 0, k-1), values[r][1] + maxValue(r-1, 1, k-1)));
        }
        return memorize(r, open, k, values[r][open] + maxValue(r-1, open, k-1));
    }

    else { // (k < r)
        if (open < 0) {
            return memorize(r, open, k, max3(values[r][0] + maxValue(r-1, 0, k-1), values[r][1] + maxValue(r-1, 1, k-1), values[r][0] + values[r][1] + maxValue(r-1, open, k)));
        }
        return memorize(r, open, k, max2(values[r][open] + maxValue(r-1, open, k-1), values[r][0] + values[r][1] + maxValue(r-1, -1, k)));
    }
}

int main() {
    // Set up data structures
    values = (int **)malloc(200*sizeof(int *));
    int *placeholder = (int *)malloc(400*sizeof(int));

    int i;
    for (i = 0; i < 200; i++) {
        values[i] = placeholder + (i<<1);
    }

    cache = (tb_el *)calloc((CACHE_MASK+1), sizeof(tb_el));
    tag = 0;

    // Get number of rows (N) and number of rooms to close off (k)
    int N = parse_num(), k = parse_num();

    while (N || k) {
        tag++;

        // Parse room values
        for (i = 0; i < N; i++) {
            values[i][0] = parse_num(); values[i][1] = parse_num();
        }

        // Print solution
        printf("%d\n", maxValue(N-1, -1, k-1));
        
        // Start next gallery
        N = parse_num(); k = parse_num();
    }

    return 0;
}
