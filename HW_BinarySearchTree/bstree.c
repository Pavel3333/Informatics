#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "bstree.h"

/* internal functions, trading safety for speed */

static BSTree** _far_node(BSTree* tree, uint8_t is_left) {
    BSTree** node = &tree;

    while (*node) {
        BSTree** next = is_left ? &((*node)->left) : &((*node)->right);
        if (!*next)
            break;
        node = next;
    }

    return node;
}

#define _min_node(tree) _far_node(tree, 1)
#define _max_node(tree) _far_node(tree, 0)

static void _print_indent(uint16_t indent) {
    for (uint16_t i = 0; i < indent; i++)
        putc(' ', stdout);
}

static void _print_tree(BSTree* tree, uint16_t indent) {
    _print_indent(indent);
    print_node(tree);

    if (!tree)
        return;

    _print_tree(tree->left, indent + INDENT_SIZE);
    _print_tree(tree->right, indent + INDENT_SIZE);
}

/* public functions */

void print_node(BSTree* node) {
    if (!node) {
        puts("NULL");
        return;
    }

    printf("%d (%lld)\n", node->key, (int64_t)(node->value));
}

void print_tree(BSTree* tree) {
    printf("Tree:\n");
    _print_tree(tree, 0);
}

Error init_tree(int key, Value value, BSTree** dest) {
    if (!dest)
        return E_NullArg;

    // Memory allocation
    BSTree* new_tree = (BSTree*)malloc(sizeof(BSTree));
    if (!new_tree)
        return E_MemoryAlloc;

    new_tree->key = key;
    new_tree->value = value;

    new_tree->left = new_tree->right = NULL;

    *dest = new_tree;

    return E_Success;
}

Error add_node(BSTree* tree, BSTree* node) {
    if (!tree || !node)
        return E_NullArg;

    Error err;

    BSTree** field; // &(node->left) or &(node->right)
    err = find_node(tree, node->key, &field);
    if (is_error(err))
        return err;
    if (is_success(err))
        return W_KeyAlreadyExists;

    *field = node;

    return E_Success;
}

Error min_node(BSTree* tree, BSTree** dest) {
    if (!tree || !dest)
        return E_NullArg;

    *dest = *(_min_node(tree));

    return E_Success;
}

Error max_node(BSTree* tree, BSTree** dest) {
    if (!tree || !dest)
        return E_NullArg;

    *dest = *(_max_node(tree));

    return E_Success;
}

Error find_node(BSTree* tree, int key, BSTree*** dest) {
    if (!tree || !dest)
        return E_NullArg;

    BSTree*  temp;
    BSTree** next = &tree;

    while (temp = *next) {
        if (key < (*next)->key)
            next = &(temp->left);
        else if (key > (*next)->key)
            next = &(temp->right);
        else {
            *dest = next;
            return E_Success;
        }
    }

    *dest = next;
    return W_NotFound;
}

Error delete_node(BSTree* tree, int key) {
    if (!tree)
        return E_NullArg;

    Error err;

    BSTree** dst;
    BSTree** src;

    err = find_node(tree, key, &dst);
    if (!is_success(err))
        return err;

    src = _max_node((*dst)->left);

    if (*src)
        goto apply;

    src = _min_node((*dst)->right);

apply:
    if (*src) {
        (*src)->left  = (*dst)->left;
        (*src)->right = (*dst)->right;
    }

    free(*dst);
    *dst = *src;
    *src = NULL;

    return E_Success;
}

void fini_tree(BSTree* tree) {
    if (tree) {
        fini_tree(tree->left);
        fini_tree(tree->right);
    }

    free(tree);
}