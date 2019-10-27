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

static ogs_inline void rb_change_child(ogs_rbtree_t *tree,
        ogs_rbnode_t *old, ogs_rbnode_t *new, ogs_rbnode_t *parent)
{
    if (parent) {
        if (old == parent->left)
            parent->left = new;
        else
            parent->right = new;
    } else {
        tree->root = new;
    }
}

static ogs_inline void rb_replace_node(ogs_rbtree_t *tree,
        ogs_rbnode_t *old, ogs_rbnode_t *new, ogs_rbnode_t *parent)
{
    rb_change_child(tree, old, new, parent);

    if (new)
        new->parent = parent;
}

/*
 * Example - Left rotate at A
 *
 *       A            B 
 *      / \          / \
 *     B   C  <--   D   A
 *    / \ / \      / \ / \
 *   D  3 4  5    1  2 3  C
 *  / \                  / \
 * 1   2                1   2
 */
static void rb_rotate_left(ogs_rbtree_t *tree, ogs_rbnode_t *node)
{
    ogs_rbnode_t *right = node->right;
    node->right = right->left;
    if (right->left)
        right->left->parent = node;

    rb_replace_node(tree, node, right, node->parent);

    right->left = node;
    node->parent = right;
}

/*
 * Example - right rotate at A
 *
 *       A            B 
 *      / \          / \
 *     B   C  -->   D   A
 *    / \ / \      / \ / \
 *   D  3 4  5    1  2 3  C
 *  / \                  / \
 * 1   2                1   2
 */
static void rb_rotate_right(ogs_rbtree_t *tree, ogs_rbnode_t *node)
{
    ogs_rbnode_t *left = node->left;
    node->left = left->right;
    if (left->right)
        left->right->parent = node;

    rb_replace_node(tree, node, left, node->parent);

    left->right = node;
    node->parent = left;
}

void ogs_rbtree_insert_color(ogs_rbtree_t *tree, void *rb_node)
{
    ogs_rbnode_t *node = rb_node;
    ogs_rbnode_t *parent;
    ogs_assert(tree);
    ogs_assert(node);

    while ((parent = node->parent) && parent->color == OGS_RBTREE_RED) {
        ogs_rbnode_t *gparent = parent->parent;
        ogs_assert(gparent);

        /* parent == grandparent's left child */
        if (parent == gparent->left) {
            ogs_rbnode_t *uncle = gparent->right;

            if (uncle && uncle->color == OGS_RBTREE_RED) {
                /*
                 * node's uncle == red (color flips)
                 *
                 *       G            g
                 *      / \          / \
                 *     p   u  -->   P   U
                 *    /            /
                 *   n            n
                 */
                uncle->color = OGS_RBTREE_BLACK;
                parent->color = OGS_RBTREE_BLACK;

                gparent->color = OGS_RBTREE_RED;

                node = gparent;
            } else {
                /* node's uncle == black */
                if (node == parent->right) {
                    /*
                     * node == the parent's right child,
                     * (left rotate at parent)
                     *
                     *      G             G
                     *     / \           / \
                     *    p   U  -->    p   U
                     *     \           /
                     *      n         n
                     */
                    node = node->parent;
                    rb_rotate_left(tree, node);
                }

                /*
                 * Now we're the left child
                 * (right rotate at grand parent)
                 *
                 *      g           P
                 *     / \         / \
                 *    P   U  -->  n   g
                 *   /                 \
                 *  n                   U
                 */
                node->parent->color = OGS_RBTREE_BLACK;
                gparent->color = OGS_RBTREE_RED;
                rb_rotate_right(tree, gparent);
            }
        /* parent  == grandparent's right child */
        } else {
            ogs_rbnode_t *uncle = gparent->left;

            if (uncle && uncle->color == OGS_RBTREE_RED) {
                /*
                 * node's uncle == red (color flips)
                 *
                 *       G            g
                 *      / \          / \
                 *     u   p  -->   U   P
                 *          \            \
                 *           n            n
                 */
                uncle->color = OGS_RBTREE_BLACK;
                parent->color = OGS_RBTREE_BLACK;

                gparent->color = OGS_RBTREE_RED;

                node = gparent;
            } else {
                /* node's uncle == black */
                if (node == parent->left) {
                    /*
                     * node == the parent's left child,
                     * (right rotate at parent)
                     *
                     *      G             G
                     *     / \           / \
                     *    p   U  -->    p   U
                     *   /               \
                     *  n                 n
                     */
                    node = node->parent;
                    rb_rotate_right(tree, node);
                }

                /*
                 * Now we're the right child,
                 * (left rotate at grand parent)
                 *
                 *      g           P
                 *     / \         / \
                 *    P   U  -->  n   g
                 *     \               \
                 *      n               U
                 */
                node->parent->color = OGS_RBTREE_BLACK;
                gparent->color = OGS_RBTREE_RED;
                rb_rotate_left(tree, gparent);
            }
        }
    }

    ogs_assert(tree->root);
    tree->root->color = OGS_RBTREE_BLACK;
}

static void rb_delete_color(
    ogs_rbtree_t *tree, ogs_rbnode_t *node, ogs_rbnode_t *parent)
{
    ogs_rbnode_t *sibling;
    ogs_assert(tree);

#define rb_is_black(r) ((!r) || (r)->color == OGS_RBTREE_BLACK)
    while (node != tree->root && rb_is_black(node)) {
        if (node == parent->left) {
            sibling = parent->right;
            if (sibling->color == OGS_RBTREE_RED) {
                /*
                 * Case 1 - left rotate at parent
                 *
                 *     P               S
                 *    / \             / \
                 *   N   s    -->    p   Sr
                 *      / \         / \
                 *     Sl  Sr      N   Sl
                 */
                sibling->color = OGS_RBTREE_BLACK;
                parent->color = OGS_RBTREE_RED;
                rb_rotate_left(tree, parent);
                sibling = parent->right;
            }
            if (rb_is_black(sibling->left) && rb_is_black(sibling->right)) {
                /*
                 * Case 2 - sibling color flip
                 * (p could be either color here)
                 *
                 *    (p)           (p)
                 *    / \           / \
                 *   N   S    -->  N   s
                 *      / \           / \
                 *     Sl  Sr        Sl  Sr
                 */
                sibling->color = OGS_RBTREE_RED;
                node = parent;
                parent = node->parent;
            } else {
                if (rb_is_black(sibling->right)) {
                    /*
                     * Case 3 - right rotate at sibling
                     * (p could be either color here)
                     *
                     *   (p)           (p)
                     *   / \           / \
                     *  N   S    -->  N   Sl
                     *     / \             \
                     *    sl  Sr            s
                     *                       \
                     *                        Sr
                     */
                    sibling->left->color = OGS_RBTREE_BLACK;
                    sibling->color = OGS_RBTREE_RED;
                    rb_rotate_right(tree, sibling);
                    sibling = parent->right;
                }
                /*
                 * Case 4 - left rotate at parent + color flips
                 * (p and sl could be either color here.
                 *  After rotation, p becomes black, s acquires
                 *  p's color, and sl keeps its color)
                 *
                 *      (p)             (s)
                 *      / \             / \
                 *     N   S     -->   P   Sr
                 *        / \         / \
                 *      (sl) sr      N  (sl)
                 */
                sibling->color = parent->color;
                parent->color = OGS_RBTREE_BLACK;
                sibling->right->color = OGS_RBTREE_BLACK;
                rb_rotate_left(tree, parent);
                node = tree->root;
            }
        } else {
            sibling = parent->left;
            if (sibling->color == OGS_RBTREE_RED) {
                sibling->color = OGS_RBTREE_BLACK;
                parent->color = OGS_RBTREE_RED;
                /* Case 1 - right rotate at parent */
                rb_rotate_right(tree, parent);
                sibling = parent->left;
            }
            if (rb_is_black(sibling->left) && rb_is_black(sibling->right)) {
                /* Case 2 - sibling color flip */
                sibling->color = OGS_RBTREE_RED;
                node = parent;
                parent = node->parent;
            } else {
                if (rb_is_black(sibling->left)) {
                    sibling->right->color = OGS_RBTREE_BLACK;
                    sibling->color = OGS_RBTREE_RED;
                    /* Case 3 - left rotate at sibling */
                    rb_rotate_left(tree, sibling);
                    sibling = parent->left;
                }
                /* Case 4 - right rotate at parent + color flips */
                sibling->color = parent->color;
                parent->color = OGS_RBTREE_BLACK;
                sibling->left->color = OGS_RBTREE_BLACK;
                rb_rotate_right(tree, parent);
                node = tree->root;
            }
        }
    }
    if (node)
        node->color = OGS_RBTREE_BLACK;
}

void ogs_rbtree_delete(ogs_rbtree_t *tree, void *rb_node)
{
    ogs_rbnode_t *node = rb_node;
    ogs_rbnode_t *child, *parent;
    ogs_rbtree_color_e color;
    ogs_assert(tree);
    ogs_assert(node);

    if (!node->left) {
        child = node->right;
        parent = node->parent;
        color = node->color;

        rb_replace_node(tree, node, child, parent);
    } else if (!node->right) {
        child = node->left;
        parent = node->parent;
        color = node->color;

        rb_replace_node(tree, node, child, parent);
    } else {
        ogs_rbnode_t *new = ogs_rbtree_min(node->right);

        child = new->right;
        parent = new->parent;
        color = new->color;

        new->left = node->left;
        node->left->parent = new;

        if (parent == node) {
            parent = new;
        } else {
            if (child) {
                child->parent = parent;
            }
            parent->left = child;

            new->right = node->right;
            node->right->parent = new;
        }

        new->color = node->color;

        rb_replace_node(tree, node, new, node->parent);
    }

    if (color == OGS_RBTREE_BLACK)
        rb_delete_color(tree, child, parent);
}

void *ogs_rbtree_first(const ogs_rbtree_t *tree)
{
    ogs_rbnode_t *node;
    ogs_assert(tree);

    node = tree->root;
    if (!node)
        return NULL;

    return ogs_rbtree_min(node);
}

void *ogs_rbtree_last(const ogs_rbtree_t *tree)
{
    ogs_rbnode_t *node;
    ogs_assert(tree);

    node = tree->root;
    if (!node)
        return NULL;

    return ogs_rbtree_max(node);
}

#define rb_empty_node(node) ((node)->parent == (node))

void *ogs_rbtree_next(const void *rb_node)
{
    const ogs_rbnode_t *node = rb_node;
    ogs_rbnode_t *parent;
    ogs_assert(node);

    if (rb_empty_node(node))
        return NULL;

    if (node->right)
        return ogs_rbtree_min(node->right);

    while ((parent = node->parent) && node == parent->right)
        node = parent;

    return parent;
}

void *ogs_rbtree_prev(const void *rb_node)
{
    const ogs_rbnode_t *node = rb_node;
    ogs_rbnode_t *parent;
    ogs_assert(node);

    if (rb_empty_node(node))
        return NULL;

    if (node->left)
        return ogs_rbtree_max(node->left);

    while ((parent = node->parent) && node == parent->left)
        node = parent;

    return parent;
}
