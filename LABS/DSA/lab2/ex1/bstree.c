#include <stdlib.h>
#include <string.h>
#include "bstree.h"

struct bstree *bstree_create(char *key, int value) {
    struct bstree *node = malloc(sizeof(*node));
    if (node != NULL) {
        node->key = strdup(key);
        node->value = value;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

void bstree_add(struct bstree *tree, char *key, int value) {
    if (tree == NULL) {
        return;
    }

    struct bstree *parent = NULL;
    struct bstree *current = tree;
    while (current != NULL) {
        parent = current;
        if (strcmp(key, current->key) < 0) {
            current = current->left;
        } else if (strcmp(key, current->key) > 0) {
            current = current->right;
        } else {
            current->value = value;
            return;
        }
    }

    struct bstree *newNode = bstree_create(key, value);
    if (strcmp(key, parent->key) < 0) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
}

struct bstree *bstree_lookup(struct bstree *tree, char *key) {
    while (tree != NULL) {
        int cmp = strcmp(key, tree->key);
        if (cmp < 0) {
            tree = tree->left;
        } else if (cmp > 0) {
            tree = tree->right;
        } else {
            return tree;
        }
    }
    return NULL;
}

struct bstree *bstree_min(struct bstree *tree) {
    if (tree == NULL) return NULL;
    while (tree->left != NULL) {
        tree = tree->left;
    }
    return tree;
}

struct bstree *bstree_max(struct bstree *tree) {
    if (tree == NULL) return NULL;
    while (tree->right != NULL) {
        tree = tree->right;
    }
    return tree;
}
