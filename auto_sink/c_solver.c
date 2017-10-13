#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INT_MAX 0x7fffffff

typedef struct {
    char *city;
    int num;
} tb_el;

typedef struct node_el {
    int num;
    struct node_el *next;
} node;

void table_put(tb_el *table, int mask, char *city, int num) {
    int i, hash = 5381;
    for (i = 0; city[i]; i++) {
        hash = ((hash << 5) + hash) + city[i];
    }
    hash &= mask;

    while (table[hash].city) {
        hash = mask & (hash + 91);
    }

    table[hash].city = city;
    table[hash].num = num;
}

int table_get(tb_el *table, int mask, char *city) {
    int i, hash = 5381;
    for (i = 0; city[i]; i++) {
        hash = ((hash << 5) + hash) + city[i];
    }
    hash &= mask;

    while (table[hash].city) {
        if (!strcmp(table[hash].city, city)) {
            return table[hash].num;
        }
        hash = mask & (hash + 91);
    }

    return -1;
}

void list_add(node **list, int li, int ri) {
    node *el = calloc(1, sizeof(node));
    el->num = ri;

    if (list[li]) {
        el->next = list[li];
    }

    list[li] = el;
}

void topo_process(int cur, int *visited, int *sorted, node **adj_list, int *post) {
    if (visited[cur]) {
        return;
    }
    visited[cur] = 1;

    node *el = adj_list[cur];
    while (el) {
        topo_process(el->num, visited, sorted, adj_list, post);
        el = el->next;
    }

    sorted[*post] = cur;
    *post = *post + 1;
}

void topo_sort(int *visited, int *sorted, node **adj_list, int len) {
    int i, post = 0;
    for (i = 0; i < len; i++) {
        topo_process(i, visited, sorted, adj_list, &post);
    }
}

void resolve(int *visited, int *sorted, node **adj_list, int *adj_matrix, int *tolls, int len) {
    topo_sort(visited, sorted, adj_list, len);

    int i;
    for (i = 0; i < len; i++) {
        int cur = sorted[i];
        int p_row = cur*len;

        node *c_el = adj_list[cur];

        // Loop over children
        while (c_el) {
            int c_toll = tolls[c_el->num];
            int c_row = c_el->num * len;
            node *gc_el = adj_list[c_el->num];

            // Loop over grandchildren
            while (gc_el) {
                if ((c_toll + adj_matrix[c_row + gc_el->num]) < adj_matrix[p_row + gc_el->num]) {
                    if (adj_matrix[p_row + gc_el->num] == INT_MAX) {
                        list_add(adj_list, cur, gc_el->num);
                    }
                    adj_matrix[p_row + gc_el->num] = (c_toll + adj_matrix[c_row + gc_el->num]);
                }
                
                gc_el = gc_el->next;
            }

            c_el = c_el->next;
        }
    }
}

int main() {
    int ignore_fscanf __attribute__((unused));
    int i; // reused iterator

    // Read in n (number of cities)
    int n;
    ignore_fscanf = fscanf(stdin, "%d\n", &n);

    // Initialize hash table, adj-list, adj-matrix, and tolls
    int mask = 1;
    while (mask < n) {
        mask <<= 1;
    }
    mask <<= 2; // Ensure table capacity > 4*n, for better hashing

    tb_el *table = (tb_el*)calloc(mask, sizeof(tb_el));
    mask -= 1;

    node **adj_list = calloc(n, sizeof(node*));

    int *adj_matrix = malloc(n*n*sizeof(int));
    for (i = 0; i < n*n; i++) {
        adj_matrix[i] = INT_MAX;
    }

    int *tolls = malloc(n*sizeof(int));

    // Read in the cities and tolls
    for (i = 0; i < n; i++) {
        adj_matrix[n*i + i] = 0;

        char *city = malloc(21); // Max length 20
        int toll;
        
        ignore_fscanf = fscanf(stdin, "%s %d\n", city, &toll);

        table_put(table, mask, city, i);
        tolls[i] = toll;
    }

    // Read in h (number of highways)
    int h;
    ignore_fscanf = fscanf(stdin, "%d\n", &h);

    // Read in the highways/graph edges
    char *left = malloc(21), *right = malloc(21);
    for (i = 0; i < h; i++) {
        ignore_fscanf = fscanf(stdin, "%s %s\n", left, right);

        int li = table_get(table, mask, left);
        int ri = table_get(table, mask, right);

        list_add(adj_list, li, ri);
        adj_matrix[li*n + ri] = tolls[ri];
    }

    int *visited = calloc(n, sizeof(int));
    int *sorted = malloc(n*sizeof(int));

    // Do the hard work
    resolve(visited, sorted, adj_list, adj_matrix, tolls, n);
    
    // Read in t (number of trips)
    int t;
    ignore_fscanf = fscanf(stdin, "%d\n", &t);

    // Read in the trips, print answers
    for (i = 0; i < t; i++) {
        ignore_fscanf = fscanf(stdin, "%s %s\n", left, right);
        
        int li = table_get(table, mask, left);
        int ri = table_get(table, mask, right);

        int cost = adj_matrix[n*li + ri];

        if (cost < INT_MAX) {
            printf("%d\n", cost);
        }
        else {
            printf("NO\n");
        }
    }

    return 0;
}
