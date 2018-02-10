#include <stdlib.h>
#include <stdio.h>

#define INF 0x7fffffff
#define CACHE_MASK 0xfffffff

typedef struct {
    int tag;
    int val;
    int cur;
    int maxheight;
    int pos;
    long path;
} tb_el;

static tb_el *cache;
static int tag;
static int iters;

int cache_locate(int cur, int pos, int maxheight, int *ret) {
    int hash = 5381;
    hash = (hash << 5) + hash + cur;
    hash = (hash << 5) + hash + pos;
    hash = (hash << 5) + hash + maxheight;
                
    hash &= CACHE_MASK;

    while (cache[hash].tag == tag) {
        if (cache[hash].cur == cur && cache[hash].pos == pos && cache[hash].maxheight == maxheight) {
            *ret = hash;
            return 1;
        }
        hash = (hash + 91) & CACHE_MASK;
    }

    *ret = hash;
    return 0;
}

tb_el cache_put(int cur, int pos, int maxheight, int val, long path, int up) {
    int loc;
    if (!cache_locate(cur, pos, maxheight, &loc)) {
        cache[loc].tag = tag;
        cache[loc].cur = cur;
        cache[loc].pos = pos;
        cache[loc].maxheight = maxheight;
        cache[loc].val = val;
        cache[loc].path = up ? path | (1L << pos) : path & ~(1L << pos);
    }
    return cache[loc];
}

int cache_get(int cur, int pos, int maxheight, tb_el *ret) {
    int loc;
    if (cache_locate(cur, pos, maxheight, &loc)) {
        *ret = cache[loc];
        return 1;
    }
    return 0;
}

static inline int max(int a, int b) {
    return a > b ? a : b;
}

static inline tb_el min(tb_el a, tb_el b, int *dir) {
    *dir = (a.val < b.val);
    return (a.val < b.val) ? a : b;
}

tb_el solve(int cur, int maxheight, int pos, int M, int *dist) {
    iters++;
    //printf("iters: %d\n", iters);
    //printf("pos: %d, cur: %d, maxheight: %d\n", pos, cur, maxheight);
    tb_el val;
    if (cache_get(cur, pos, maxheight, &val)) {
        return val;
    }

    if (pos == M) {
        return cache_put(cur, pos, maxheight, (cur == 0) ? maxheight : INF, 0L, 0);
    }
    int d = dist[pos];
    if (cur < d) {
        val = solve(cur+d, max(maxheight, cur+d), pos+1, M, dist);
        return cache_put(cur, pos, maxheight, val.val, val.path, 1);
    }
    int dir;
    val = min(solve(cur+d, max(maxheight, cur+d), pos+1, M, dist), solve(cur-d, maxheight, pos+1, M, dist), &dir);
    return cache_put(cur, pos, maxheight, val.val, val.path, dir);
}

static inline int parse_num() {
    int n = 0;
    char r;
    while ((r = getc_unlocked(stdin)) >= '0') {
        n = n*10 + r-'0';
    }
    return n;
}

void print_solution(tb_el val, int M) {
    //printf("iters: %d\n", iters);
    if (val.val < INF) {
        int i;
        for (i = 0; i < M; i++) {
            char c = (val.path & (1 << i)) ? 'U' : 'D';
            printf("%c", c);
        }
        printf("\n");
        return;
    }
    printf("IMPOSSIBLE\n");
}

int main() {
    int N = parse_num();
    int *dist = malloc(40*sizeof(int));
    cache = (tb_el *)calloc(CACHE_MASK+1, sizeof(tb_el));
    tag = 1;

    while (N--) {
        iters = 0;
        int M = parse_num();
        int i;
        for (i = 0; i < M; i++) {
            dist[i] = parse_num();
        }
        tb_el val = solve(0, 0, 0, M, dist);
        print_solution(val, M);

        tag++;
    }

    return 0;
}
