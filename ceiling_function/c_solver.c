#include <stdlib.h>
#include <stdio.h>

typedef struct node {
    int val;
    struct node *left;
    struct node *right;
} tree;

void insert_tree(int j, tree **tr) {
    if (!*tr) {
        *tr = (tree*)calloc(1, sizeof(tree));
        (*tr)->val = j;
    }
    else if (j < (*tr)->val) {
        insert_tree(j, &(*tr)->left);
    }
    else { // (j >= tr->val)
        insert_tree(j, &(*tr)->right);
    }
}

char tree_shape_cmp(tree *a, tree *b) {
    if (!a || !b) {
        return !(!a ^ !b);
    }
        
    return tree_shape_cmp(a->left, b->left) && tree_shape_cmp(a->right, b->right);
}

char table_contains(tree **table, tree *tr, int table_cap_mask, int *out_idx) {
    while (table[*out_idx]) {
        if (tree_shape_cmp(table[*out_idx], tr)) {
            return 1;
        }

        *out_idx = table_cap_mask & (*out_idx + 1);
    }

    return 0;
}

// Compute a hash for the tree (based on tree shape)
void hash_tree(tree *tr, int *hash, int pos) {
    if (!tr) {
        return;
    }

    hash_tree(tr->left, hash, (pos << 1));
    *hash = (*hash << 5) + *hash + pos;
    hash_tree(tr->right, hash, (pos << 1) + 1);
}

int main() {
    int ignore_fscanf __attribute__((unused));

    // Read in n and k
    int n, k, table_cap_mask = 1, count = 0;
    ignore_fscanf = fscanf(stdin, "%d %d\n", &n, &k);
    while (table_cap_mask < n) {
        table_cap_mask <<= 1;
    }
    table_cap_mask <<= 2; // Ensure table capacity > 4*n, for better hashing

    tree **table = (tree**)calloc(table_cap_mask, sizeof(tree*));
    table_cap_mask -= 1;

    do {
        tree *tr = NULL;

        // Read in the next sequence and build a tree
        int i, len = 0, hash = 5381;
        for (i = 0; i < k; i++) {
            int j;
            ignore_fscanf = fscanf(stdin, "%d", &j);

            insert_tree(j, &tr);
        }

        // Compute a hash for the tree
        hash_tree(tr, &hash, 0);
        hash &= table_cap_mask;

        // Place the tree in the table
        if (!table_contains(table, tr, table_cap_mask, &hash)) {
            table[hash] = tr;
            count++;
        }
    } while (--n);

    printf("%d\n", count);

    return 0;
}
