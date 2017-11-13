#include <stdlib.h>
#include <stdio.h>

typedef struct node {
    int val;
    struct node *next;
} node;

void ll_push(node **ll, int val) {
    node *new_node = malloc(sizeof(node));
    new_node->val = val;
    new_node->next = *ll;
    *ll = new_node;
}

int parse_num() {
    int n = 0;
    char r;
    while ((r = getc_unlocked(stdin)) >= '0') {
        n = n*10 + r-48;
    }
    return n;
}

static inline void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void percolate_up(int *queue, int start) {
    int current = start;
    int parent = current >> 1;
    while (parent && queue[parent] < queue[current]) {
        swap(&queue[parent], &queue[current]);
        current = parent;
        parent >>= 1;
    }
}

void pq_push(int *queue, int *queue_len, int val) {
    // Place at the end of the heap, then percolate up
    queue[*queue_len] = val;
    percolate_up(queue, *queue_len);
    *queue_len = *queue_len + 1;
}

void add_all_to_queue(node **ll, int *queue, int *queue_len) {
    node *current = *ll;
    while (current) {
        pq_push(queue, queue_len, current->val);
        current = current->next;
    }
}

static inline int maximum2(int a, int b, int c) {
    return a < b ? b < c ? 1 : 0 : a < c ? 1 : -1;
}

static inline int maximum(int current, int *queue, int queue_len) {
    if ((current<<1)+1 < queue_len) {
        return maximum2(queue[current], queue[current<<1], queue[(current<<1)+1]);
    }
    else {
        return (current<<1) < queue_len && queue[current<<1] > queue[current] ? 0 : -1;
    }
}

void percolate_down(int *queue, int queue_len) {
    int current = 1;
    int max = maximum(current, queue, queue_len);
    while (max >= 0) {
        int child = (current<<1)+max;
        swap(&queue[current], &queue[child]);
        current = child;
        max = maximum(current, queue, queue_len);
    }
}

int remove_max(int *queue, int *queue_len) {
    if (*queue_len == 1) {
        return 0;
    }

    int ret = queue[1];

    // Replace first with last in heap, then percolate down
    *queue_len = *queue_len - 1;
    queue[1] = queue[*queue_len];
    percolate_down(queue, *queue_len);

    return ret;
}

int main() {
    // Get set
    int N = parse_num(), T = parse_num();
    node **wait_list = calloc(T, sizeof(node*));

    // Collect people by wait time
    int i;
    for (i = 0; i < N; i++) {
        int c = parse_num(), t = parse_num();
        ll_push(wait_list + t, c);
    }

    // Set up sum and priority queue
    int sum = 0;
    int *queue = malloc((N+1)*sizeof(int));
    int queue_len = 1;

    for (i = T-1; i >= 0; i--) {
        // Add people at this wait time to the priority queue
        add_all_to_queue(wait_list + i, queue, &queue_len);
        
        // Remove and add the maximum
        sum += remove_max(queue, &queue_len);
    }

    printf("%d\n", sum);

    return 0;
}
