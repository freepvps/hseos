#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <memory.h>
#include <stdint.h>


typedef struct Node {
    ssize_t left;
    ssize_t right;
    const char* s;
} node_t;


node_t* get_left(node_t* p) {
    if (p->left == 0) {
        return NULL;
    } else {
        return &p[p->left];
    }
}

node_t* concat_left(
    node_t* root_a,
    size_t size_a,
    node_t* root_b,
    size_t size_b,
    node_t* node_a
) {
    node_t* res = malloc((size_a + size_b) * sizeof(*res));
    memcpy(res, root_a, size_a * sizeof(*res));
    memcpy(res + size_a, root_b, size_b * sizeof(*res));

    size_t offset = node_a - root_a;

    res[offset].left = size_a - offset;
    return res;
}

int main() {
    node_t* root = calloc(1, sizeof(*root));
    root->left = 0;
    root->right = 0;
    root->s = "abc";
    root = concat_left(root, 1, root, 1, root);

    printf("%s\n", get_left(root)->s);
    
    return 0;
}
