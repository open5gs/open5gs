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

#ifndef OGS_LIST_H
#define OGS_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

struct ogs_list_s {
    struct ogs_list_s *prev, *next;
};
typedef struct ogs_list_s ogs_list_t;
typedef struct ogs_list_s ogs_lnode_t;;

#define OGS_LIST(name) \
    ogs_list_t name = { NULL, NULL }

#define ogs_list_init(list) do { \
    (list)->prev = (NULL); \
    (list)->next = (NULL); \
} while (0)

static ogs_inline void *ogs_list_first(const ogs_list_t *list)
{
    return list->next;
}

static ogs_inline void *ogs_list_last(const ogs_list_t *list)
{
    return list->prev;
}

static ogs_inline void *ogs_list_next(void *lnode)
{
    ogs_list_t *node = lnode;
    return node->next;
}

static ogs_inline void *ogs_list_prev(void *lnode)
{
    ogs_list_t *node = lnode;
    return node->prev;
}

#define ogs_list_entry(ptr, type, member) ogs_container_of(ptr, type, member)

#define ogs_list_for_each(list, node) \
    for (node = ogs_list_first(list); (node); \
        node = ogs_list_next(node))

#define ogs_list_for_each_entry(list, node, member) \
    for (node = ogs_list_entry(ogs_list_first(list), typeof(*node), member); \
            (&node->member); \
                node = ogs_list_entry( \
                        ogs_list_next(&node->member), typeof(*node), member))

#define ogs_list_for_each_safe(list, n, node) \
    for (node = ogs_list_first(list); \
        (node) && (n = ogs_list_next(node), 1); \
        node = n)

#define ogs_list_for_each_entry_safe(list, n, node, member) \
    for (node = ogs_list_entry(ogs_list_first(list), typeof(*node), member); \
            (&node->member) && \
                (n = ogs_list_entry( \
                    ogs_list_next(&node->member), typeof(*node), member), 1); \
            node = n)

static ogs_inline void ogs_list_prepend(ogs_list_t *list, void *lnode)
{
    ogs_list_t *node = lnode;

    node->prev = NULL;
    node->next = list->next;
    if (list->next)
        list->next->prev = node;
    else
        list->prev = node;
    list->next = node;
}

static ogs_inline void ogs_list_add(ogs_list_t *list, void *lnode)
{
    ogs_list_t *node = lnode;

    node->prev = list->prev;
    node->next = NULL;
    if (list->prev)
        list->prev->next = node;
    else
        list->next = node;
    list->prev = node;
}

static ogs_inline void ogs_list_remove(ogs_list_t *list, void *lnode)
{
    ogs_list_t *node = lnode;
    ogs_list_t *prev = node->prev;
    ogs_list_t *next = node->next;;

    if (prev)
        prev->next = next;
    else
        list->next = next;

    if (next)
        next->prev = prev;
    else
        list->prev = prev;
}

static ogs_inline void ogs_list_insert_prev(
        ogs_list_t *list, void *lnext, void *lnode)
{
    ogs_list_t *node = lnode;
    ogs_list_t *next = lnext;

    node->prev = next->prev;
    node->next = next;
    if (next->prev)
        next->prev->next = node;
    else
        list->next = node;
    next->prev = node;
}

static ogs_inline void ogs_list_insert_next(
        ogs_list_t *list, void *lprev, void *lnode)
{
    ogs_list_t *node = lnode;
    ogs_list_t *prev = lprev;

    node->prev = prev;
    node->next = prev->next;
    if (prev->next)
        prev->next->prev = node;
    else
        list->prev = node;
    prev->next = node;
}

typedef int (*ogs_list_compare_f)(ogs_lnode_t *n1, ogs_lnode_t *n2);
#define ogs_list_insert_sorted(__list, __lnode, __compare) \
    __ogs_list_insert_sorted(__list, __lnode, (ogs_list_compare_f)__compare);

static ogs_inline void __ogs_list_insert_sorted(
    ogs_list_t *list, void *lnode, ogs_list_compare_f compare)
{
    ogs_list_t *node = lnode;
    ogs_list_t *iter = NULL;

    ogs_list_for_each(list, iter) {
        if ((*compare)(node, iter) < 0) {
            ogs_list_insert_prev(list, iter, node);
            break;
        }
    }

    if (iter == NULL)
        ogs_list_add(list, node);
}

static ogs_inline bool ogs_list_empty(const ogs_list_t *list)
{
    return list->next == NULL;
}

static ogs_inline int ogs_list_count(const ogs_list_t *list)
{
    ogs_list_t *node;
    int i = 0;
    ogs_list_for_each(list, node)
        i++;
    return i;
}

#ifdef __cplusplus
}
#endif

#endif /* OGS_LIST_H */
