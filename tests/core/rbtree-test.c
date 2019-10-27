/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-core.h"
#include "core/abts.h"

#define TEST_RB_NUM 3000
static void verify_properties(ogs_rbtree_t *tree);

typedef struct t1_node_s {
    ogs_rbnode_t node;
    int key;
} t1_node_t;

static void add_t1_node(ogs_rbtree_t *tree, t1_node_t *node)
{
    ogs_rbnode_t **new = NULL;
    ogs_rbnode_t *parent = NULL;
    ogs_assert(tree);
    ogs_assert(node);

    new = &tree->root;
    while (*new) {
        t1_node_t *this = ogs_rb_entry(*new, t1_node_t, node);

        parent = *new;
        if (node->key < this->key)
            new = &(*new)->left;
        else
            new = &(*new)->right;
    }

    ogs_rbtree_link_node(node, parent, new);
    ogs_rbtree_insert_color(tree, node);
}

static void test1_func(abts_case *tc, void *data)
{
    OGS_RBTREE(tree);
    t1_node_t test_node[TEST_RB_NUM];
    t1_node_t *node, *next_node;
    int i;

    ABTS_INT_EQUAL(tc, 0, ogs_rbtree_count(&tree));
    ABTS_TRUE(tc, ogs_rbtree_empty(&tree) == true);

    for (i = 0; i < TEST_RB_NUM; i++) {
        test_node[i].key = ogs_random32() % TEST_RB_NUM;
        add_t1_node(&tree, &test_node[i]);
        verify_properties(&tree);
    }

    ABTS_INT_EQUAL(tc, TEST_RB_NUM, ogs_rbtree_count(&tree));
    ABTS_TRUE(tc, ogs_rbtree_empty(&tree) == false);

    for (node = ogs_rbtree_first(&tree);
        (node) && (next_node = ogs_rbtree_next(node), 1); node = next_node) {
        if (next_node)
            ogs_assert(node->key <= next_node->key);
    }

    for (i = 0; i < TEST_RB_NUM; i++) {
        ogs_rbtree_delete(&tree, &test_node[i]);
        verify_properties(&tree);
    }

    ABTS_INT_EQUAL(tc, 0, ogs_rbtree_count(&tree));
    ABTS_TRUE(tc, ogs_rbtree_empty(&tree) == true);
}

typedef struct t2_node_s {
    int key;
    ogs_rbnode_t node;
} t2_node_t;

static void add_t2_node(ogs_rbtree_t *tree, t2_node_t *node)
{
    ogs_rbnode_t **new = NULL;
    ogs_rbnode_t *parent = NULL;
    ogs_assert(tree);
    ogs_assert(node);

    new = &tree->root;
    while (*new) {
        t2_node_t *this = ogs_rb_entry(*new, t2_node_t, node);

        parent = *new;
        if (node->key < this->key)
            new = &(*new)->left;
        else
            new = &(*new)->right;
    }

    ogs_rbtree_link_node(&node->node, parent, new);
    ogs_rbtree_insert_color(tree, &node->node);
}

static void test2_func(abts_case *tc, void *data)
{
    OGS_RBTREE(tree);
    t2_node_t test_node[TEST_RB_NUM];
    ogs_rbnode_t *node, *prev_node;
    int i;

    ABTS_INT_EQUAL(tc, 0, ogs_rbtree_count(&tree));
    ABTS_TRUE(tc, ogs_rbtree_empty(&tree) == true);

    for (i = 0; i < TEST_RB_NUM; i++) {
        test_node[i].key = ogs_random32() % TEST_RB_NUM;
        add_t2_node(&tree, &test_node[i]);
        verify_properties(&tree);
    }

    ABTS_INT_EQUAL(tc, TEST_RB_NUM, ogs_rbtree_count(&tree));
    ABTS_TRUE(tc, ogs_rbtree_empty(&tree) == false);

    for (node = ogs_rbtree_last(&tree);
        node && (prev_node = ogs_rbtree_prev(node), 1); node = prev_node) {
        t2_node_t *this = ogs_rb_entry(node, t2_node_t, node);
        if (prev_node) {
            t2_node_t *prev = ogs_rb_entry(prev_node, t2_node_t, node);
            ogs_assert(this->key >= prev->key);
        }
    }

    for (i = 0; i < TEST_RB_NUM; i++) {
        ogs_rbtree_delete(&tree, &test_node[i].node);
        verify_properties(&tree);
    }

    ABTS_INT_EQUAL(tc, 0, ogs_rbtree_count(&tree));
    ABTS_TRUE(tc, ogs_rbtree_empty(&tree) == true);
}

#define TEST_TIMER_NUM 5

static void test3_func(abts_case *tc, void *data)
{
    OGS_RBTREE(tree);
    t1_node_t test_node[TEST_TIMER_NUM];
    int key[5] = { 500, 50, 200, 90, 800 };
    int i;

    ABTS_INT_EQUAL(tc, 0, ogs_rbtree_count(&tree));
    ABTS_TRUE(tc, ogs_rbtree_empty(&tree) == true);

    for (i = 0; i < TEST_TIMER_NUM; i++) {
        test_node[i].key = key[i];
        add_t1_node(&tree, &test_node[i]);
        verify_properties(&tree);
    }

    ABTS_INT_EQUAL(tc, TEST_TIMER_NUM, ogs_rbtree_count(&tree));
    ABTS_TRUE(tc, ogs_rbtree_empty(&tree) == false);

    for (i = 0; i < TEST_TIMER_NUM; i++) {
        ogs_rbtree_delete(&tree, &test_node[i]);
        verify_properties(&tree);
    }

    ABTS_INT_EQUAL(tc, 0, ogs_rbtree_count(&tree));
    ABTS_TRUE(tc, ogs_rbtree_empty(&tree) == true);
}

abts_suite *test_rbtree(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);

    return suite;
}

static void verify_property_1(ogs_rbnode_t *node);
static void verify_property_2(ogs_rbnode_t *root);
static void verify_property_4(ogs_rbnode_t *node);
static void verify_property_5(ogs_rbnode_t *root);
static void verify_property_5_helper(
        ogs_rbnode_t *node, int black_count, int* path_black_count);

#define rb_is_red(r)   ((r) && (r)->color == OGS_RBTREE_RED)
#define rb_is_black(r) ((!r) || (r)->color == OGS_RBTREE_BLACK)

static void verify_properties(ogs_rbtree_t *tree)
{
    verify_property_1(tree->root);
    verify_property_2(tree->root);
    /* Property 3 is implicit */
    verify_property_4(tree->root);
    verify_property_5(tree->root);
}
static void verify_property_1(ogs_rbnode_t *node)
{
    ogs_assert(rb_is_red(node) || rb_is_black(node));
    if (node == NULL) return;
    verify_property_1(node->left);
    verify_property_1(node->right);
}
static void verify_property_2(ogs_rbnode_t *root)
{
    ogs_assert(rb_is_black(root));
}
static void verify_property_4(ogs_rbnode_t *node)
{
    if (rb_is_red(node)) {
        ogs_assert(rb_is_black(node->left));
        ogs_assert(rb_is_black(node->right));
        ogs_assert(rb_is_black(node->parent));
    }
    if (node == NULL) return;
    verify_property_4(node->left);
    verify_property_4(node->right);
}
static void verify_property_5(ogs_rbnode_t *root)
{
    int black_count_path = -1;
    verify_property_5_helper(root, 0, &black_count_path);
}
static void verify_property_5_helper(
        ogs_rbnode_t *node, int black_count, int* path_black_count)
{
    if (rb_is_black(node)) {
        black_count++;
    }
    if (node == NULL) {
        if (*path_black_count == -1) {
            *path_black_count = black_count;
        } else {
            ogs_assert(black_count == *path_black_count);
        }
        return;
    }
    verify_property_5_helper(node->left,  black_count, path_black_count);
    verify_property_5_helper(node->right, black_count, path_black_count);
}
