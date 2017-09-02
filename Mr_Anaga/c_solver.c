#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int *val;
    int cnt;
} table_element;

int int_cmp(int *a, int *b) {
    int i;
    for (i = 0; i < 26; i++) {
        if (*(a + i) ^ *(b + i)) {
            return 1;
        }
    }

    return 0;
}

// Determines whether a string is in the hash table, and helpfully indicates where it is or would go
char table_contains(table_element *hash_table, int table_cap_mask, int *str, int *out_idx) {
    while (hash_table[*out_idx].val) {
        if (!int_cmp(hash_table[*out_idx].val, str)) {
            return 1;
        }

        *out_idx = table_cap_mask & (*out_idx + 91);
    }

    return 0;
}

int main() {
    int ignore_fscanf __attribute__((unused));
    char *ignore_fgets __attribute__((unused));

    // Read in n and k
    int n, k, table_cap_mask = 1, count = 0;
    ignore_fscanf = fscanf(stdin, "%d %d\n", &n, &k);

    while (table_cap_mask < n) {
        table_cap_mask <<= 1;
    }
    table_cap_mask <<= 2; // Ensure table capacity > 4*n, for better hashing
    table_cap_mask -= 1;

    table_element *hash_table = (table_element *)calloc(table_cap_mask + 1, sizeof(table_element));
    int letter_cnts[26] = {0};
    int space = (k+1) > 104 ? (k+1) : 104;

    do {
        // Collect the next string
        char *str = malloc(space);
        ignore_fgets = fgets(str, k+2, stdin);
        *(str + k) = '\0';

        // Encode the string, calculate the hash
        int i, hash = 5381;
        for (i = 0; *(str + i); i++) {
            letter_cnts[*(str + i) - 97]++;
        }

        for (i = 0; i < 26; i++) {
            *(int *)(str + (i << 2)) = letter_cnts[i];
            hash = ((hash << 5) + hash) + letter_cnts[i];
            letter_cnts[i] = 0;
        }

        hash &= table_cap_mask;

        // Place the string
        if (table_contains(hash_table, table_cap_mask, (int *)str, &hash)) {
            if (!hash_table[hash].cnt++) {
                count--;
            }
        }
        else {
            hash_table[hash].val = (int *)str;
            count++;
        }
    } while (--n);

    printf("%d\n", count);
    
    return 0;
}
