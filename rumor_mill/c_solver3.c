#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Notes:
//
// tree1: name
// tree2: index
// graph: index->(name,tree2)
// table: name->index
// queue: index
//
// Flow: name -> table@name:index -> graph@index:tree2 -> bfs(tree2,graph,visited,queue)

typedef struct node {
    union {
        int index;
        char *name;
    } key;
    struct node *left;
    struct node *right;
} tree;

typedef struct {
    char *name;
    union {
        int index;
        tree *tr;
    } val;
} tb_el;

void tree_insert1(tree **tr, char *name) {
    if (!*tr) {
        *tr = (tree*)calloc(1, sizeof(tree));
        (*tr)->key.name = name;
    }
    else if (strcmp(name, (*tr)->key.name) < 0) {
        tree_insert1(&(*tr)->left, name);
    }
    else {
        tree_insert1(&(*tr)->right, name);
    }
}

void table_put(tb_el *table, int mask, char *name, int index) {
    int i, hash = 5381;
    for (i = 0; name[i]; i++) {
        hash = ((hash << 5) + hash) + name[i];
    }
    hash &= mask;

    while (table[hash].name) {
        hash = mask & (hash + 91);
    }

    table[hash].name = name;
    table[hash].val.index = index;
}

int table_get(tb_el *table, int mask, char *name) {
    int i, hash = 5381;
    for (i = 0; name[i]; i++) {
        hash = ((hash << 5) + hash) + name[i];
    }
    hash &= mask;

    while (table[hash].name) {
        if (!strcmp(table[hash].name, name)) {
            return table[hash].val.index;
        }
        hash = mask & (hash + 91);
    }

    return -1;
}

void tree_to_table_and_graph(tree **tr, tb_el *table, int mask, tb_el *graph, int *index) {
    if (!*tr) {
        return;
    }
    
    tree_to_table_and_graph(&(*tr)->left, table, mask, graph, index);

    graph[*index].name = (*tr)->key.name;
    table_put(table, mask, (*tr)->key.name, *index);
    *index = *index + 1;

    tree_to_table_and_graph(&(*tr)->right, table, mask, graph, index);
}

void heap_children(int *heapp, int *heapp_len, int *edges, int row, int *visited) {
    int i;
    for (i = 1; i <= edges[row]; i++) {
        if (visited[edges[row+i]]) {
            continue; // Already got this one
        }

        visited[edges[row+i]] = 1;
        heapp[*heapp_len] = edges[row+i]; // Put child at end of the heap

        if (*heapp_len) {
            int current = *heapp_len;
            while (heapp[current] < heapp[(current-1)>>1]) { // Ensure min heap property - swap with parents
                int tmp = heapp[current];
                heapp[current] = heapp[(current-1)>>1];
                heapp[(current-1)>>1] = tmp;
                current = (current-1)>>1;
            }
        }

        *heapp_len = *heapp_len + 1;
    }
}

static inline int minimum(int a, int b, int c) {
    return a < b ? a < c ? 0 : 2 : b < c ? 1 : 2;
}

void pop_print_heaps(int *heapp, int *heapp_len, int *heapc, int *heapc_len, int *edges, int *visited, tb_el *graph, int len) {
    while (*heapc_len) {
        // Remove the min of heapc
        int min = heapc[0];
        *heapc_len = *heapc_len - 1;
        heapc[0] = heapc[*heapc_len];

        int current = 0;
        int minm; 
        if ((current<<1)+2 < *heapc_len) {
            minm = minimum(heapc[current], heapc[(current<<1)+1], heapc[(current<<1)+2]);
        }
        else {
            minm = ((current<<1)+1 < *heapc_len) && (heapc[(current<<1)+1] < heapc[current]);
        }
        while (minm) { // Ensure min heap property - swap with min child
            int tmp = heapc[current];
            heapc[current] = heapc[(current << 1) + minm];
            heapc[(current << 1) + minm] = tmp;
            current = (current << 1) + minm;
            if ((current<<1)+2 < *heapc_len) {
                minm = minimum(heapc[current], heapc[(current<<1)+1], heapc[(current<<1)+2]);
            }
            else {
                minm = ((current<<1)+1 < *heapc_len) && (heapc[(current<<1)+1] < heapc[current]);
            }
        }

        printf("%s ", graph[min].name);
        heap_children(heapp, heapp_len, edges, min*len, visited); // Put children on the next heap
    }
}

void solve(int index, int *visited, tb_el *graph, int *edges, int len, int *heapp, int *heapc) {
    printf("%s ", graph[index].name);
    visited[index] = 1;

    int heapp_len = 0, heapc_len = 0;

    heap_children(heapp, &heapp_len, edges, index*len, visited);

    while (heapp_len) {
        // Swap the heap pointers
        int *tmp1 = heapp; int tmp2 = heapp_len;
        heapp = heapc; heapp_len = heapc_len;
        heapc = tmp1; heapc_len = tmp2;

        // Get to work
        pop_print_heaps(heapp, &heapp_len, heapc, &heapc_len, edges, visited, graph, len);
    }

    // Add in the remainders
    int i;
    for (i = 0; i < len; i++) {
        if (!visited[i]) {
            printf("%s ", graph[i].name);
        }
        visited[i] = 0;
    }

    printf("\n");
}

int main() {
    int ignore_fscanf __attribute((unused));
    int i; // Reused iterator

    // Read in n (number of students)
    int n;
    ignore_fscanf = fscanf(stdin, "%d\n", &n);

    // Set up tree, table, graph, and visited
    tree *tr = NULL;

    int mask = 1;
    while (mask < n) {
        mask <<= 1;
    }
    mask <<= 2;
    tb_el *table = malloc(mask*sizeof(tb_el));
    mask -= 1;

    tb_el *graph = calloc(n, sizeof(tb_el));
    int *edges = malloc(n*n*sizeof(int));

    int *visited = calloc(n, sizeof(int));

    // Read in student names, construct alphabetic list, store name->index in dictionary
    for (i = n; i > 0; i--) {
        edges[n*(i-1)] = 0; // Initialize each node's edge count to 0

        char *name = malloc(21);
        ignore_fscanf = fscanf(stdin, "%s\n", name);

        tree_insert1(&tr, name);
    }

    tree_to_table_and_graph(&tr, table, mask, graph, &i);
    
    // Read in f (number of friend pairs)
    int f;
    ignore_fscanf = fscanf(stdin, "%d\n", &f);

    char *left = malloc(21), *right = malloc(21);

    // Read in friend pairs, construct sub-structure
    for (i = 0; i < f; i++) {
        ignore_fscanf = fscanf(stdin, "%s %s\n", left, right);

        int li = table_get(table, mask, left), ri = table_get(table, mask, right);

        edges[n*li + (++edges[n*li])] = ri;
        edges[n*ri + (++edges[n*ri])] = li;
    }

    // Read in r (number of reports)
    int r;
    ignore_fscanf = fscanf(stdin, "%d\n", &r);

    int *heapp = malloc(n*sizeof(int));
    int *heapc = malloc(n*sizeof(int));

    // Read in rumor-starters, print rumor reports
    for (i = r; i > 0; i--) {
        ignore_fscanf = fscanf(stdin, "%s\n", left);

        int index = table_get(table, mask, left);
        solve(index, visited, graph, edges, n, heapp, heapc);
    }

    return 0;
}

