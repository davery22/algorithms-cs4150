#include <stdlib.h>
#include <stdio.h>

#define N_BOUND 10000
#define M_BOUND 15000

typedef unsigned int u_int;
typedef unsigned long u_long;

typedef struct {
    int index;
    u_int weight;
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
        u_int bit = 0x80000000;
        do {
            ft <<= 1;
            if (ft >= 10000) {
                fpf += bit;
                ft -= 10000;
            }
        } while ((bit >>= 1) && ft);
    }
    
    *x = xt;
    *y = yt;
    *f = fpf;
}

static inline void swap(node *x, node *y) {
    node tmp = *x;
    *x = *y;
    *y = tmp;
}

void percolate_up(node *heap, int *loc, int start) {
    int current = start;
    int par_cur = current>>1;
    while (heap[current].weight > heap[par_cur].weight) {
        // Swap nodes (indices, weights) and loc's
        swap(&heap[current], &heap[par_cur]);

        /*int tmp_idx = heap[current].index;
        heap[current].index = heap[current>>1].index;
        heap[current>>1].index = tmp_idx;

        u_int tmp_wgt = heap[current].weight;
        heap[current].weight = heap[current>>1].weight;
        heap[current>>1].weight = tmp_wgt;*/

        loc[heap[current].index] = current;
        loc[heap[par_cur].index] = par_cur;

        current = par_cur;
        par_cur >>= 1;
    }
}

static inline int maximum2(u_int a, u_int b, u_int c) {
    return a < b ? b < c ? 1 : 0 : a < c ? 1 : -1;
}

static inline int maximum(int current, node *heap, int *heap_len) {
    if ((current<<1)+1 < *heap_len) {
        return maximum2(heap[current].weight, heap[current<<1].weight, heap[(current<<1)+1].weight);
    }
    else {
        return ((current<<1) < *heap_len) && (heap[current<<1].weight > heap[current].weight) ? 0 : -1;
    }
}

void percolate_down(node *heap, int *heap_len, int *loc, int start) {
    int current = start;
    int max = maximum(current, heap, heap_len);
    while (max >= 0) {
        int chi_cur = (current<<1)+max;
        swap(&heap[current], &heap[chi_cur]);

        /*int tmp_idx = heap[current].index;
        heap[current].index = heap[(current<<1)+max].index;
        heap[(current<<1)+max].index = tmp_idx;

        u_int tmp_wgt = heap[current].weight;
        heap[current].weight = heap[(current<<1)+max].weight;
        heap[(current<<1)+max].weight = tmp_wgt;*/

        loc[heap[current].index] = current;
        loc[heap[chi_cur].index] = chi_cur;

        current = chi_cur;
        max = maximum(current, heap, heap_len);
    }
}

void update_insert(node *heap, int *heap_len, int *loc, int node_idx, int node_wgt) {
    if (loc[node_idx]) {
        // Update - find in the heap; update; percolate up or down
        int pos = loc[node_idx];
        heap[pos].weight = node_wgt;
        
        // Only one of the following will potentially do something
        percolate_up(heap, loc, pos);
        //percolate_down(heap, heap_len, loc, pos); Shouldn't be doing this - only update on greater weight
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

    *heap_len = *heap_len - 1;
    heap[1].index = heap[*heap_len].index;
    heap[1].weight = heap[*heap_len].weight;
    loc[heap[1].index] = 1;

    percolate_down(heap, heap_len, loc, 1);
    
    return ret;
} 

void solve(node *graph, int V, node *heap, int *loc) {
    int heap_len = 1;

    graph[0].weight = ~0;
    update_insert(heap, &heap_len, loc, 0, ~0);
    
    int i;
    while (heap_len > 1) {
        int u = delete_min(heap, &heap_len, loc);
        u_int in_frac = graph[N_BOUND*u].weight;

        if (u == V-1) {
            print_fixed(in_frac);
            return;
        }

        // Loop over (count of) reachable nodes
        for (i = 1; i <= graph[N_BOUND*u].index; i++) {
            int v = graph[N_BOUND*u + i].index;

            u_int next_frac = graph[N_BOUND*u + i].weight;
            u_int out_frac = fixed_mult(in_frac, next_frac);

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
    heap[0].weight = ~0; // Useful for bounding percolation later

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
            u_int f;
            int x, y;
            parse_three(&x, &y, &f);

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
