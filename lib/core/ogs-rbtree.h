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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_RBTREE_H
#define OGS_RBTREE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OGS_RBTREE_BLACK = 0,
    OGS_RBTREE_RED = 1,
} ogs_rbtree_color_e;

typedef struct ogs_rbnode_s {
    struct ogs_rbnode_s *parent;
    struct ogs_rbnode_s *left;
    struct ogs_rbnode_s *right;

    ogs_rbtree_color_e color;
} ogs_rbnode_t;

typedef struct ogs_rbtree_s {
    ogs_rbnode_t *root;
} ogs_rbtree_t;

#define OGS_RBTREE(name) ogs_rbtree_t name = { NULL }

#define ogs_rb_entry(ptr, type, member) ogs_container_of(ptr, type, member)

static ogs_inline void ogs_rbtree_link_node(
        void *rb_node, ogs_rbnode_t *parent, ogs_rbnode_t **rb_link)
{
    ogs_rbnode_t *node = rb_node;
    node->parent = parent;
    node->left = node->right = NULL;
    node->color = OGS_RBTREE_RED;

    *rb_link = node;
}

void ogs_rbtree_insert_color(ogs_rbtree_t *tree, void *rb_node);
void ogs_rbtree_delete(ogs_rbtree_t *tree, void *rb_node);

static ogs_inline void *ogs_rbtree_min(const ogs_rbnode_t *rb_node)
{
    const ogs_rbnode_t *node = rb_node;
    ogs_assert(node);

    while (node->left)
        node = node->left;

    return (void *)node;
}

static ogs_inline void *ogs_rbtree_max(const void *rb_node)
{
    const ogs_rbnode_t *node = rb_node;
    ogs_assert(node);

    while (node->right)
        node = node->right;

    return (void *)node;
}

void *ogs_rbtree_first(const ogs_rbtree_t *tree);
void *ogs_rbtree_next(const void *node);
void *ogs_rbtree_last(const ogs_rbtree_t *tree);
void *ogs_rbtree_prev(const void *node);

#define ogs_rbtree_for_each(tree, node) \
    for (node = ogs_rbtree_first(tree); \
        (node); node = ogs_rbtree_next(node))

#define ogs_rbtree_reverse_for_each(tree, node) \
    for (node = ogs_rbtree_last(tree); \
        (node); node = ogs_rbtree_prev(node))

static ogs_inline bool ogs_rbtree_empty(const ogs_rbtree_t *tree)
{
    return tree->root == NULL;
}

static ogs_inline int ogs_rbtree_count(const ogs_rbtree_t *tree)
{
    ogs_rbnode_t *node;
    int i = 0;
    ogs_rbtree_for_each(tree, node)
        i++;
    return i;
}

#ifdef __cplusplus
}
#endif

#endif /* OGS_RBTREE_H */
