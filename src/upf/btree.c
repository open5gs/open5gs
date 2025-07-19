/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Modified by Fatemeh Shafiei Ardestani on 2024-12-07.
//
#include "btree.h"
int str_compare(const void *a, const void *b) {
  return strcmp((const char *) a, (const char *) b);
}

MapNode btreemap_node(void *key, void *value) {
  MapNode node = (MapNode) malloc(sizeof(struct _MapNode));
  if (node != NULL) {
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
  }
  return node;
}

int type_u64_compare(const void *a, const void *b) {
  uint64_t fa = *(const uint64_t *) a;
  uint64_t fb = *(const uint64_t *) b;
  return (fa > fb) - (fa < fb);
}

BTreeMap new_str_btree() {
  BTreeMap map = (BTreeMap) malloc(sizeof(struct _BTreeMap));
  if (map != NULL) {
    map->root = NULL;
    map->size = 0;
    map->compare = str_compare;
  }
  return map;
}

BTreeMap new_u64_btree() {
  BTreeMap map = (BTreeMap) malloc(sizeof(struct _BTreeMap));
  if (map != NULL) {
    map->root = NULL;
    map->size = 0;
    map->compare = type_u64_compare;
  }
  return map;
}

void btreemap_insert_helper(MapNode *root, void *key, void *value, int (*compare)(const void *, const void *)) {
  if (*root == NULL) {
    *root = btreemap_node(key, value);
  } else {
    if (compare(key, (*root)->key) < 0) {
      btreemap_insert_helper(&((*root)->left), key, value, compare);
    } else if (compare(key, (*root)->key) > 0) {
      btreemap_insert_helper(&((*root)->right), key, value, compare);
    } else {
      // Key already exists, update the value
      (*root)->value = value;
    }
  }
}

void btreemap_insert(BTreeMap map, void *key, void *value) {
  btreemap_insert_helper(&(map->root), key, value, map->compare);
  map->size++;
}

void btreemap_insert_str(BTreeMap map, const char *key, void *value) {
  char* dst = malloc(strlen(key) + 1);
  strcpy(dst, key);
  btreemap_insert_helper(&(map->root), dst, value, map->compare);
  map->size++;
}

void btreemap_insert_u64(BTreeMap map, uint64_t key, void *value) {
  uint64_t* dst = malloc(sizeof (uint64_t));
  *dst = key;
  btreemap_insert_helper(&(map->root), dst, value, map->compare);
  map->size++;
}

// Function to remove a key-value pair from the BTreeMap
MapNode btreemap_remove_helper(MapNode root, void *key, int (*compare)(const void *, const void *)) {
  if (root == NULL) {
    return NULL;
  }

  if (compare(key, root->key) < 0) {
    root->left = btreemap_remove_helper(root->left, key, compare);
  } else if (compare(key, root->key) > 0) {
    root->right = btreemap_remove_helper(root->right, key, compare);
  } else {
    // Found the node to remove
    if (root->left == NULL) {
      MapNode temp = root->right;
      free(root);
      return temp;
    } else if (root->right == NULL) {
      MapNode temp = root->left;
      free(root);
      return temp;
    }

    // Node with two children: Get the inorder successor (smallest in the right subtree)
    MapNode temp = root->right;
    while (temp->left != NULL) {
      temp = temp->left;
    }

    // Copy the inorder successor's content to this node
    root->key = temp->key;
    root->value = temp->value;

    // Delete the inorder successor
    root->right = btreemap_remove_helper(root->right, temp->key, compare);
  }
  return root;
}

void btreemap_remove(BTreeMap map, void *key) {
  if (map->root == NULL) {
    return;
  }
  map->root = btreemap_remove_helper(map->root, key, map->compare);
  map->size--;
}

// Function to get the number of elements in the BTreeMap
size_t btreemap_len(BTreeMap map) {
  return map->size;
}

// Function to search for a key in the BTreeMap
void *btreemap_search_helper(MapNode root, void *key, int (*compare)(const void *, const void *)) {
  if (root == NULL) {
    return NULL;
  }
  int cmp = compare(key, root->key);
  if (cmp == 0) {
    return root->value;
  } else if (cmp < 0) {
    return btreemap_search_helper(root->left, key, compare);
  } else {
    return btreemap_search_helper(root->right, key, compare);
  }
}

void *btreemap_get(BTreeMap map, void *key) {
  return btreemap_search_helper(map->root, key, map->compare);
}

// Function to clear the BTreeMap
void btreemap_clear_helper(MapNode root) {
  if (root == NULL) {
    return;
  }
  btreemap_clear_helper(root->left);
  btreemap_clear_helper(root->right);
  free(root);
}

void btreemap_clear(BTreeMap map) {
  btreemap_clear_helper(map->root);
  map->root = NULL;
  map->size = 0;
}

// Function to perform inorder traversal and apply a function to each node
void btreemap_foreach_helper(MapNode root, void (*func)(void *key, void *value)) {
  if (root == NULL) {
    return;
  }
  btreemap_foreach_helper(root->left, func);
  func(root->key, root->value);
  btreemap_foreach_helper(root->right, func);
}

// Function to iterate over all elements in the BTreeMap and apply a function to each node
void btreemap_foreach(BTreeMap map, void (*func)(void *key, void *value)) {
  btreemap_foreach_helper(map->root, func);
}
