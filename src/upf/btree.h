//
// Created by Fatemeh Shafiei Ardestani on 2024-12-02.
//

#ifndef OPEN5GS_BTREE_H
#define OPEN5GS_BTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"
#include <stdint.h>
#include "context.h"
// Rust's 'str' type (immutable string slice)
typedef const char *str;

// Define the structure for a node in the B-tree
typedef struct _MapNode *MapNode;
struct _MapNode {
    void *key;
    void *value;
    MapNode left;
    MapNode right;
};

// Define the BTreeMap structure
typedef struct _BTreeMap *BTreeMap;

struct _BTreeMap {
    MapNode root;
    size_t size;

    int (*compare)(const void *, const void *); // Function pointer for comparison function
};

void btreemap_insert_helper(MapNode *root, void *key, void *value, int (*compare)(const void *, const void *));

void btreemap_insert(BTreeMap map, void *key, void *value);

MapNode btreemap_remove_helper(MapNode root, void *key, int (*compare)(const void *, const void *));

void btreemap_remove(BTreeMap map, void *key);

size_t btreemap_len(BTreeMap map);

void *btreemap_search_helper(MapNode root, void *key, int (*compare)(const void *, const void *));

void *btreemap_get(BTreeMap map, void *key);

void btreemap_clear_helper(MapNode root);

void btreemap_clear(BTreeMap map);

void btreemap_foreach_helper(MapNode root, void (*func)(void *key, void *value));

void btreemap_foreach(BTreeMap map, void (*func)(void *key, void *value));

void print_node(void *key, void *value);

BTreeMap new_str_btree(void);

MapNode btreemap_node(void *key, void *value);
BTreeMap new_u64_btree(void);
void btreemap_insert_u64(BTreeMap map, uint64_t key, void *value);
void btreemap_insert_str(BTreeMap map, const char *key, void *value);
// Comparison function for str keys
int str_compare(const void *a, const void *b);

int type_u64_compare(const void *a, const void *b);
void btreemap_free_helper(MapNode root, bool free_keys, bool free_values, void (*free_value_fn)(void *));
void btreemap_free(BTreeMap map, bool free_key, bool free_values, void (*free_value_fn)(void *));
#endif