#include <stdlib.h>
#include <stdio.h>

#define N_BOUND 10000
#define M_BOUND 15000

typedef unsigned int u_int;
typedef unsigned long u_long;

typedef struct {
    int index;
    float weight;
} node;

static inline int fixed_mult(u_int f1, u_int f2) {
    if (~f1 && ~f2) {  // Only if they're both not 1.0 (encoded as all 1's)
        return (u_int)(((u_long)f1 * (u_long)f2) >> 32);
    }
    return f1 & f2;
}

void print_fixed(u_int f) {
    if (~f) {
        u_long fl = (u_long)f;
        u_int acc = 0;
        int i;
        for (i = 0; i < 4; i++) {
            fl *= 10;
            acc = acc*10 + (fl >> 32);
            fl &= 0xffffffff;
        }
        fl *= 10;
        if ((fl >> 32) >= 5) {
            acc++;
        }
        printf("0.%04d\n", acc);
        /*u_int i, ft = 0, num = 5000, bit = 0x80000000; // TODO: Need better precision
        for (i = 0; i < 13; i++) {
            if (f & bit) {
                ft += num;   
            }
            num >>= 1;
            bit >>= 1;
        }
        printf("0.%04d\n", ft);*/
    }
    else {
        printf("1.0000\n");
    }
}

void parse_three(int *x, int *y, u_int *f) {
    int xt = 0, yt = 0;
    u_int fpf = 0;
    char r;
    while ((r = getc_unlocked(stdin)) != ' ') {
        xt = xt*10 + r - '0';
    }
    while ((r = getc_unlocked(stdin)) != ' ') {
        yt = yt*10 + r - '0';
    }
    if (getc_unlocked(stdin) == '1') {
        fpf = ~0;
        while (getc_unlocked(stdin) != '\n') {} // Advance to next line
    }
    else {
        getc_unlocked(stdin); // Skip '.'
        int i = 4, ft = 0;
        while ((r = getc_unlocked(stdin)) != '\n') {
            ft = ft*10 + r - '0';
            i--;
        }
        while (i--) {
            ft *= 10;
        }

        // Convert to fixed-point representation
        u_int num = 5000, agg = 2500, bit = 0x80000000;
        for (i = 0; i < 32; i++) {
            //printf("ft: %d, num: %d, bit: %x\n", ft, num, bit);
            if (ft >= num) {
                fpf += bit;
                num += agg;
            }
            else {
                num -= agg;
            }
            agg >>= 1;
            bit >>= 1;
        }
        //printf("parsed: %d  ->  %x\n", ft, fpf);
    }
    
    *x = xt;
    *y = yt;
    *f = fpf;
}

void percolate_up(node *heap, int *loc, int start) {
    int current = start;
    while (heap[current].weight > heap[current>>1].weight) {
        // Swap nodes (indices, weights) and loc's
        int tmp_idx = heap[current].index;
        heap[current].index = heap[current>>1].index;
        heap[current>>1].index = tmp_idx;

        float tmp_wgt = heap[current].weight;
        heap[current].weight = heap[current>>1].weight;
        heap[current>>1].weight = tmp_wgt;

        loc[heap[current].index] = current;
        loc[heap[current>>1].index] = current>>1;

        current >>= 1;
    }
}

static inline int maximum(float a, float b, float c) {
    return a < b ? b < c ? 1 : 0 : a < c ? 1 : -1;
}

void percolate_down(node *heap, int *heap_len, int *loc, int start) {
    int current = start;
    int max;
    if ((current<<1)+1 < *heap_len) {
        max = maximum(heap[current].weight, heap[current<<1].weight, heap[(current<<1)+1].weight);
    }
    else {
        max = ((current<<1) < *heap_len) && (heap[current<<1].weight > heap[current].weight) ? 0 : -1;
    }
    while (max >= 0) {
        int tmp_idx = heap[current].index;
        heap[current].index = heap[(current<<1)+max].index;
        heap[(current<<1)+max].index = tmp_idx;

        float tmp_wgt = heap[current].weight;
        heap[current].weight = heap[(current<<1)+max].weight;
        heap[(current<<1)+max].weight = tmp_wgt;

        loc[heap[current].index] = current;
        loc[heap[(current<<1)+max].index] = (current<<1)+max;

        current = (current<<1)+max;

        if ((current<<1)+1 < *heap_len) {
            max = maximum(heap[current].weight, heap[current<<1].weight, heap[(current<<1)+1].weight);
        }
        else {
            max = ((current<<1) < *heap_len) && (heap[current<<1].weight > heap[current].weight) ? 0 : -1;
        }
    }
}

void update_insert(node *heap, int *heap_len, int *loc, int node_idx, float node_wgt) {
    if (loc[node_idx]) {
        // Update - find in the heap; update; percolate up or down
        int pos = loc[node_idx];
        heap[pos].weight = node_wgt;
        
        // Only one of the following will potentially do something
        percolate_up(heap, loc, pos);
        percolate_down(heap, heap_len, loc, pos);
    }
    else {
        // Insert - place at the end of the heap; percolate up
        heap[*heap_len].index = node_idx;
        heap[*heap_len].weight = node_wgt;
        loc[node_idx] = *heap_len;

        int pos = *heap_len;
        percolate_up(heap, loc, pos);

        *heap_len = *heap_len + 1;
    }
}

int delete_min(node *heap, int *heap_len, int *loc) {
    int ret = heap[1].index;
    //loc[ret] = 0; Shouldn't need this - done between rounds

    *heap_len = *heap_len - 1;
    heap[1].index = heap[*heap_len].index;
    heap[1].weight = heap[*heap_len].weight;
    loc[heap[1].index] = 1;

    percolate_down(heap, heap_len, loc, 1);
    
    return ret;
} 

void solve(node *graph, int V, node *heap, int *loc) {
    int heap_len = 1;

    graph[0].weight = 1.0;
    update_insert(heap, &heap_len, loc, 0, 1.0);
    
    int i;
    while (heap_len > 1) {
        int u = delete_min(heap, &heap_len, loc);
        float in_frac = graph[N_BOUND*u].weight;

        if (u == V-1) {
            printf("%.4f\n", graph[N_BOUND*u].weight);
            return;
        }

        // Loop over (count of) reachable nodes
        for (i = 1; i <= graph[N_BOUND*u].index; i++) {
            int v = graph[N_BOUND*u + i].index;

            float next_frac = graph[N_BOUND*u + i].weight;
            float out_frac = in_frac * next_frac;

            //printf("u: %d, u.w: %f, v: %d, v.w: %f\n", u, in_frac, v, next_frac);

            if (out_frac > graph[N_BOUND*v].weight) {
                graph[N_BOUND*v].weight = out_frac;
                update_insert(heap, &heap_len, loc, v, out_frac);
            }
        }
    }
}

int main() {
    int ignore_fscanf __attribute__((unused));
    int i; // reused iterator

    node *graph = malloc(N_BOUND*N_BOUND*sizeof(node)); // 800MB - Not a small amount of memory ;)

    node *heap = malloc(N_BOUND*sizeof(node)); // indexed from 1
    int *loc = calloc(N_BOUND, sizeof(int)); // ^ So, 0 means missing
    heap[0].weight = 0; // Useful for bounding percolation later

    int n, m;
    ignore_fscanf = fscanf(stdin, "%d %d\n", &n, &m);

    while (n | m) {
        // Reset relevant counts, weights, and locs
        for (i = 0; i < n; i++) {
            graph[N_BOUND*i].index = 0;
            graph[N_BOUND*i].weight = 0;
            loc[i] = 0;
        }

        // Extract the graph
        for (i = 0; i < m; i++) {
            float f;
            int x, y;
            ignore_fscanf = fscanf(stdin, "%d %d %f\n", &x, &y, &f);
            //printf("(%d,%d) -> %f\n", x, y, f);

            node *xx = &graph[N_BOUND*x + (++graph[N_BOUND*x].index)];
            node *yy = &graph[N_BOUND*y + (++graph[N_BOUND*y].index)];
            xx->index = y;
            yy->index = x;
            xx->weight = yy->weight = f;
        }

        // Solve - Dijkstra's
        solve(graph, n, heap, loc);

        // Repeat
        ignore_fscanf = fscanf(stdin, "%d %d\n", &n, &m);
    }

    return 0;
}
