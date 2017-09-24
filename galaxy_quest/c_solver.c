#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long x;
    long y;
    int cnt;
} galaxy;

int nextTo(galaxy l, galaxy r, long d2) {
    return (l.x-r.x)*(l.x-r.x) + (l.y-r.y)*(l.y-r.y) <= d2;
}

galaxy find_majority(galaxy *stars, int lo, int hi, long d2) {
    if (hi-lo == 1) {
        //printf("  basecase: (%ld,%ld)\n", stars[lo].x, stars[lo].y);
        return stars[lo];
    }

    galaxy l = find_majority(stars, lo, (hi+lo)>>1, d2), r = find_majority(stars, (hi+lo)>>1, hi, d2);

    if (!l.cnt && !r.cnt) {
        //printf("nn\n");
        return l;
    }
    else if (l.cnt == r.cnt) {
        //printf("==\n");
        l.cnt = (nextTo(l, r, d2)) ? l.cnt + r.cnt : 0;
        return l;
    }
    else if (l.cnt > r.cnt) {
        //printf("l>r\n");
        int i;
        for (i = ((hi+lo)>>1); i < hi; i++) {
            if (nextTo(l, stars[i], d2)) {
                l.cnt++;
            }
        }
        if (l.cnt <= (hi-lo)>>1) {
            l.cnt = 0;
        }
        return l;
    }
    else {
        //printf("r>l\n");
        int i;
        for (i = lo; i < ((hi+lo)>>1); i++) {
            if (nextTo(r, stars[i], d2)) {
                r.cnt++;
            }
        }
        if (r.cnt <= (hi-lo)>>1) {
            r.cnt = 0;
        }
        return r;
    }
}

int main() {
    int ignore_fscanf __attribute__((unused));

    // Read in d and k
    long d2; int k, i;
    ignore_fscanf = fscanf(stdin, "%ld %d\n", &d2, &k);
    d2 *= d2;

    galaxy *stars = malloc(k*sizeof(galaxy));
    for (i = 0; i < k; i++) {
        ignore_fscanf = fscanf(stdin, "%ld %ld\n", &(stars[i].x), &(stars[i].y));
        stars[i].cnt = 1;
    }

    galaxy maj = find_majority(stars, 0, k, d2);
    if (maj.cnt) {
        printf("%d\n", maj.cnt);
    }
    else {
        printf("NO\n");
    }

    return 0;
}
