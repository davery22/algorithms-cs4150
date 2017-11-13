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

typedef struct {
    int start;
    int end;
    int *arr;
} circ;

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

void tree_insert2(tree **tr, int index) {
    if (!*tr) {
        *tr = (tree*)calloc(1, sizeof(tree));
        (*tr)->key.index = index;
    }
    else if (index < (*tr)->key.index) {
        tree_insert2(&(*tr)->left, index);
    }
    else {
        tree_insert2(&(*tr)->right, index);
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

void tree_traverse2(tree **trp, tree **trc, int *visited, int mark) {
    if (!*trp) {
        return;
    }

    int index = (*trp)->key.index;
    if (visited[index] != mark) {
        tree_insert2(trc, index);
        visited[index] = mark;
    }

    tree_traverse2(&(*trp)->left, trc, visited, mark);
    tree_traverse2(&(*trp)->right, trc, visited, mark);
}

void tree_traverse3(tree **trp, tree **trc, tb_el *graph, int *visited, int mark) {
    if (!*trp) {
        return;
    }

    tree_traverse3(&(*trp)->left, trc, graph, visited, mark);

    int index = (*trp)->key.index;
    printf("%s ", graph[index].name);
    tree_traverse2(&(graph[index].val.tr), trc, visited, mark);

    tree_traverse3(&(*trp)->right, trc, graph, visited, mark);
}

void solve(int index, int *visited, tb_el *graph, int mark, int len) {
    printf("%s ", graph[index].name);
    visited[index] = mark;

    // Put each unvisited child in a tree + mark each as visited
    // Print tree + put each unvisited child in a tree + mark each as visited
    // Stop when tree is empty
    tree *trc = NULL;
    tree_traverse2(&(graph[index].val.tr), &trc, visited, mark);

    while (trc) {
        tree *trp = malloc(sizeof(tree));
        *trp = *trc;
        trc = NULL;
        tree_traverse3(&trp, &trc, graph, visited, mark);
    }

    // Add in the remainders
    int i;
    for (i = 0; i < len; i++) {
        if (visited[i] != mark) {
            printf("%s ", graph[i].name);
        }
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

    int *visited = calloc(n, sizeof(int));

    // Read in student names, construct alphabetic list, store name->index in dictionary
    for (i = n; i > 0; i--) {
        char *name = malloc(21);
        ignore_fscanf = fscanf(stdin, "%s\n", name);

        tree_insert1(&tr, name);
    }

    tree_to_table_and_graph(&tr, table, mask, graph, &i);
    
    // Read in f (number of friend pairs)
    int f;
    ignore_fscanf = fscanf(stdin, "%d\n", &f);

    char *left = malloc(21), *right = malloc(21);

    // Read in friend pairs, construct alphabetic sub-structure
    for (i = 0; i < f; i++) {
        ignore_fscanf = fscanf(stdin, "%s %s\n", left, right);

        int li = table_get(table, mask, left), ri = table_get(table, mask, right);
        tree_insert2(&(graph[li].val.tr), ri);
        tree_insert2(&(graph[ri].val.tr), li);
    }
    
    // Read in r (number of reports)
    int r;
    ignore_fscanf = fscanf(stdin, "%d\n", &r);

    // Read in rumor-starters, print rumor reports
    for (i = r; i > 0; i--) {
        ignore_fscanf = fscanf(stdin, "%s\n", left);

        int index = table_get(table, mask, left);
        solve(index, visited, graph, i, n);
    }

    return 0;
}

