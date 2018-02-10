#include <stdlib.h>
#include <stdio.h>

/*** TYPEDEFS AND GLOBALS ***/

typedef struct {
    int u;
    int v;
    int wgt;
} edge;

typedef struct {
    int deg;
    int parent;
} ds_el;

typedef struct {
    ds_el *ds;
    int *remap;
} disjoint_set;

static int UB;
static int *mins;
static int **dist;
static int n;
static int iters = 0;
static edge *sorted_edges;

/*** DISJOINT SET (FOR KRUSKAL'S) ***/

disjoint_set *new_ds(long unvisited, int cnt_unv) {
    ds_el *ds = (ds_el *)malloc(cnt_unv*sizeof(ds_el));
    int *remap = (int *)malloc(n*sizeof(int));
    
    int i, j = 0;
    for (i = 0; i < n; i++) {
        if (unvisited & (1L << i)) {
            ds[j].deg = 0;
            ds[j].parent = j;
            remap[i] = j;
            j++;
        }
    }

    disjoint_set *set = (disjoint_set *)malloc(sizeof(disjoint_set));
    set->ds = ds;
    set->remap = remap;
    return set;
}

int ds_find(disjoint_set *set, int el) {
    if (set->ds[el].parent != el) {
        set->ds[el].parent = ds_find(set, set->ds[el].parent);
    }
    return set->ds[el].parent;
}

void ds_union(disjoint_set *set, int par1, int par2) {
    /* Unnecessary - par1 and par2 are given to be unequal root elements
    int par1 = ds_find(set, el1);
    int par2 = ds_find(set, el2);
    if (par1 == par2) return;
    */

    if (set->ds[par1].deg > set->ds[par2].deg) {
        set->ds[par2].parent = par1;
    }
    else if (set->ds[par1].deg < set->ds[par2].deg) {
        set->ds[par1].parent = par2;
    }
    else {
        set->ds[par2].parent = par1;
        set->ds[par1].deg++; 
    }
}

/*** LOWER BOUNDING (MINIMUM-1-TREE; USES KRUSKAL'S) ***/

int kruskal(long unvisited, int cnt_unv) {
    int wgt = 0;
    disjoint_set *set = new_ds(unvisited, cnt_unv);

    int i;
    for (i = 0; i < n*(n-1); i++) {
        edge e = sorted_edges[i];
        // Skip visited
        if (!(unvisited & (1L << e.u)) || !(unvisited & (1L << e.v))) {
            continue;
        }
        int el1 = set->remap[e.u], el2 = set->remap[e.v];
        int ds1 = ds_find(set, el1), ds2 = ds_find(set, el2);
        if (ds1 != ds2) {
            wgt += e.wgt;
            ds_union(set, ds1, ds2);
        }
    }

    free(set->ds);
    free(set->remap);
    free(set);
    return wgt;
}

int lower_bound(int start, int end, int cnt_unv, long unvisited) {
    int LB = (cnt_unv > 1) ? kruskal(unvisited, cnt_unv) : 0;
    printf("LBi: %d, cnt_unv: %d, unv: %lx\n", LB, cnt_unv, unvisited);
    
    // Link end to MST through min edge
    int i, min = 0x7fffffff;
    for (i = 0; i < n; i++) {
        if (dist[end][i] < min && (unvisited & (1L << i))) {
            min = dist[end][i];
        }
    }
    LB += min;

    // Link MST to start through min edge
    min = 0x7fffffff;
    for (i = 0; i < n; i++) {
        if (dist[i][start] < min && (unvisited & (1L << i))) {
            min = dist[i][start];
        }
    }
    LB += min;

    return LB;
}

/*** RADIX SORT ***/

void radix_sort() {
    int i, j;
    sorted_edges = (edge *)malloc(n*(n-1)*sizeof(edge));
    int *radix = (int *)calloc(501, sizeof(int));

    // Count occurrences
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j) {
                continue;
            }
            radix[dist[i][j]]++;
        }
    }

    // Convert radix counts to indices
    for (i = 1; i < 501; i++) {
        radix[i] += radix[i-1];
    }

    // Place edges in sorted list
    for (i = n-1; i >= 0; i--) {
        for (j = n-1; j >= 0; j--) {
            if (i == j) {
                continue;
            }
            int place = --radix[dist[i][j]];
            sorted_edges[place].u = i;
            sorted_edges[place].v = j;
            sorted_edges[place].wgt = dist[i][j];
        }
    }
}

/*** SECOND LAYER HELPERS ***/

static inline int min(int a, int b) {
    return a < b ? a : b;
}

static inline int max(int a, int b) {
    return a > b ? a : b;
}

void solve(int u, int cnt_unv, long unvisited, int cur, int curLB) {
    iters++;
    if (!unvisited) {
        cur += dist[u][0];
        if (cur < UB) {
            UB = cur;
        }
        ////printf("returning... %d\n", UB);
        return;
    }

    if (cur + curLB >= UB) {
        ////printf("returning... %d > %d\n", cur+curLB, UB);
        return;
    }

    int v;
    for (v = 0; v < n; v++) {
        if (unvisited & (1L << v)) {
            solve(v, cnt_unv-1, unvisited & ~(1L << v), cur + dist[u][v], curLB - mins[v]);
        }
    }
}

int parse_num(int *ret) {
    int n = 0;
    char r;
    while ((r = getc_unlocked(stdin)) >= '0') {
        n = n*10 + r-'0';
    }
    *ret = r;
    return n;
}

/*** PQ SOLUTION ***/

typedef struct {
    long unvisited;
    int u;
    int cur;
    int curLB;
    int priority;
} partial;

typedef struct {
    partial *heap;
    int len;
} PQ;

static inline void swap(partial *a, partial *b) {
    partial tmp = *a;
    *a = *b;
    *b = tmp;
}

void percolate_up(PQ *queue, int start) {
    int current = start;
    int parent = current >> 1;
    while (parent && queue->heap[parent].priority > queue->heap[current].priority) {
        swap(&(queue->heap[parent]), &(queue->heap[current]));
        current = parent;
        parent >>= 1;
    }
}

void pq_push(PQ *queue, long unvisited, int u, int cur, int curLB) {
    // Place at the end of the heap, then percolate up
    queue->heap[queue->len].unvisited = unvisited;
    queue->heap[queue->len].u = u;
    queue->heap[queue->len].cur = cur;
    queue->heap[queue->len].curLB = curLB;
    queue->heap[queue->len].priority = cur + curLB;
    percolate_up(queue, queue->len);
    queue->len++;
}

static inline int minimum2(int a, int b, int c) {
    return a < b ? a < c ? -1 : 1 : b < c ? 0 : 1;
}

static inline int minimum(int current, PQ *queue) {
    if ((current<<1)+1 < queue->len) {
        return minimum2(queue->heap[current].priority, queue->heap[current<<1].priority, queue->heap[(current<<1)+1].priority);
    }
    else {
        return (current<<1) < queue->len && queue->heap[current<<1].priority < queue->heap[current].priority ? 0 : -1;
    }
}

void percolate_down(PQ *queue) {
    int current = 1;
    int mini = minimum(current, queue);
    while (mini >= 0) {
        int child = (current<<1)+mini;
        swap(&(queue->heap[current]), &(queue->heap[child]));
        current = child;
        mini = minimum(current, queue);
    }
}

partial remove_min(PQ *queue) {
    partial ret = queue->heap[1];

    // Replace first with last in heap, then percolate down
    queue->len--;
    queue->heap[1] = queue->heap[queue->len];
    percolate_down(queue);

    return ret;
}

void solve2(PQ *queue, long unvisited, int u, int cur, int curLB) {
    iters++;
    if (!unvisited) {
        cur += dist[u][0];
        if (cur < UB) {
            UB = cur;
            int i, len = queue->len;
            for (i = 1; i < len && queue->heap[i].priority < UB; i++) {}
            queue->len = i;
            for (; i < len; i++) {
                if (queue->heap[i].priority < UB) {
                    partial x = queue->heap[i];
                    pq_push(queue, x.unvisited, x.u, x.cur, x.curLB);
                }
            }
            //printf("updated\n");
        }
        return;
    }

    int v;
    for (v = 0; v < n; v++) {
        if (unvisited & (1L << v)) {
            int next = cur + dist[u][v];
            int nextLB = curLB - mins[v]; 
            if (next + nextLB < UB) {
                pq_push(queue, unvisited & ~(1L << v), v, next, nextLB);
            }
        }
    }
}

void solver(int LB) {
    // Make a priority queue
    partial *space = (partial *)malloc(10000000*sizeof(partial));
    PQ *queue = (PQ *)malloc(sizeof(PQ));
    queue->heap = space;
    queue->len = 1;

    pq_push(queue, (1 << n)-2, 0, 0, LB);

    while (queue->len > 1 /*&& queue->heap[1].priority < UB*/) {
        //printf("queue->len: %d\n", queue->len);
        //if (queue->len > 1000) {
            int i;
            for (i = 1; i < queue->len && queue->heap[i].priority < UB; i++) {}
        //    printf("queue->len: %d, relevant-up-to: %d\n", queue->len, i);
        //    exit(1);
        //}
        partial x = remove_min(queue);
        solve2(queue, x.unvisited, x.u, x.cur, x.curLB);
    }
}

/*** MAIN ENTRY POINT ***/

int main() {
    int i, j;
    n = parse_num(&i);

    // Set up
    mins = (int *)malloc(n*sizeof(int));
    dist = (int **)malloc(n*sizeof(int*));
    int *space = (int *)malloc(n*n*sizeof(int));

    for (i = 0; i < n; i++) {
        dist[i] = space + n*i;
    }

    int LB = 0, UBi = 0;
    int current = 0, next = 0;

    // Read in values, compute lower bound, and highest value (next)
    for (i = 0; i < n; i++) {
        int ret;
        for (j = 0; j < n; j++) {
            int val = parse_num(&ret);
            dist[i][j] = (i == j) ? 250000 : val;
            
            if (dist[i][j] < 250000 && dist[i][j] > next) {
                next = dist[i][j];
                current = i;
            }
        }

        // Work-around for bad test formatting... :(
        while (ret != '\n') {
            parse_num(&ret);
        }

        UBi += (i < n-1) ? dist[i][i+1] : 0;
    }

    UBi += dist[n-1][0];

    // Compute initial lower bound -- min-ins
    for (i = 0; i < n; i++) {
        mins[i] = 0x7fffffff;

        for (j = 0; j < n; j++) {
            if (dist[j][i] < mins[i]) {
                mins[i] = dist[j][i];
            }
        }

        LB += mins[i];
    }

    // Compute initial upper bound -- nearest neighbor
    long unvisited = ((1L << n) - 1) & ~(1L << current);
    int first = current;
    UB = 0;

    while (unvisited) {
        int minsofar = 0x7fffffff;
        for (i = 0; i < n; i++) {
            if ((unvisited & (1L << i)) && (dist[current][i] < minsofar)) {
                minsofar = dist[current][i];
                next = i;
            }
        }
        
        UB += minsofar;
        current = next;
        unvisited &= ~(1L << next);
    }

    UB += dist[current][first];

    // Sort edges
    ////radix_sort();

    // Finalize bounds
    ////printf("UB: %d, UBi: %d\n", UB, UBi);
    UB = min(UB, UBi);

    ////printf("LB: %d, UB: %d\n", LB, UB);

    // Solve (if we haven't gotten super lucky)
    if (UB != LB) {
        //unvisited = (1L << n) - 2;
        //solve(0, n-1, unvisited, 0, LB);
        solver(LB);
    }

    ////printf("iterations: %d\n", iters);
    printf("%d\n", UB);

    return 0;
}
