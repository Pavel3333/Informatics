#pragma once

#include "error.h"

#define INDENT_SIZE 4

typedef int64_t Value;

typedef struct bstree {
    int key;
    Value value;
    struct bstree* left;
    struct bstree* right;
} BSTree;

// Declaring public functions
void print_node(BSTree* node);
void print_tree(BSTree* tree);

Error init_tree(int key, Value value, BSTree** dest);
Error add_node(BSTree* tree, BSTree* node);
Error min_node(BSTree* tree, BSTree** dest);
Error max_node(BSTree* tree, BSTree** dest);
Error find_node(BSTree* tree, int key, BSTree*** dest);
Error delete_node(BSTree* tree, int key);

void  fini_tree(BSTree* tree);