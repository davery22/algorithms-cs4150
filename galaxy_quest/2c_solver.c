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

galaxy find_majority(galaxy *stars, int count, long d2, int depth) {
    if (count == 1) {
        return stars[0];
    }

    galaxy *mnm = malloc(count*sizeof(galaxy));

    int i, mi = 0, ni = count-1;
    for (i = 0; i+1 < count; i+= 2) {
        if (nextTo(stars[i], stars[i+1], d2)) {
            stars[i].cnt += stars[i+1].cnt;
            mnm[mi++] = stars[i];
        }
        else {
            mnm[ni--] = stars[i];
            mnm[ni--] = stars[i+1];
        }
    }
    if (i+1 == count) {
        if (!mi) {
            mnm[mi++] = stars[i];
        }
        else {
            mnm[ni--] = stars[i];
        }
    }

    if (!mi) {
        stars[0].cnt = 0;
        return stars[0];
    }
    
    galaxy maj = find_majority(mnm, mi, d2, depth+1);

    if (!maj.cnt) {
        return maj;
    }
    
    for (ni++; ni < count; ni++) {
        if (nextTo(maj, mnm[ni], d2)) {
            maj.cnt += mnm[ni].cnt;
        }
    }
    if (maj.cnt <= (count<<depth)/2) {
        maj.cnt = 0;
    }
    return maj;
}

int main() {
    int ignore_fscanf __attribute__((unused));

    // Read in d and k
    long d2; int k, i;
    ignore_fscanf = fscanf(stdin, "%ld %d\n", &d2, &k);
    d2 *= d2;

    char r;

    galaxy *stars = malloc(k*sizeof(galaxy));
    for (i = 0; i < k; i++) {
        long x = 0, y = 0;
        while ((r = getc_unlocked(stdin)) != ' ') {
            x = x*10 + r-48;
        }
        while ((r = getc_unlocked(stdin)) != '\n') {
            y = y*10 + r-48;
        }

        stars[i].x = x;
        stars[i].y = y;
        stars[i].cnt = 1;
    }

    galaxy maj = find_majority(stars, k, d2, 0);
    if (maj.cnt) {
        printf("%d\n", maj.cnt);
    }
    else {
        printf("NO\n");
    }

    return 0;
}
