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

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_event_domain

typedef struct ogs_timer_mgr_s {
    OGS_POOL(pool, ogs_timer_t);
    ogs_rbtree_t tree;
} ogs_timer_mgr_t;

static void add_timer_node(
        ogs_rbtree_t *tree, ogs_timer_t *timer, ogs_time_t duration)
{
    ogs_rbnode_t **new = NULL;
    ogs_rbnode_t *parent = NULL;
    ogs_assert(tree);
    ogs_assert(timer);

    timer->timeout = ogs_get_monotonic_time() + duration;

    new = &tree->root;
    while (*new) {
        ogs_timer_t *this = ogs_rb_entry(*new, ogs_timer_t, rbnode);

        parent = *new;
        if (timer->timeout < this->timeout)
            new = &(*new)->left;
        else
            new = &(*new)->right;
    }

    ogs_rbtree_link_node(timer, parent, new);
    ogs_rbtree_insert_color(tree, timer);
}

ogs_timer_mgr_t *ogs_timer_mgr_create(unsigned int capacity)
{
    ogs_timer_mgr_t *manager = ogs_calloc(1, sizeof *manager);
    if (!manager) {
        ogs_error("ogs_calloc() failed");
        return NULL;
    }

    ogs_pool_init(&manager->pool, capacity);

    return manager;
}

void ogs_timer_mgr_destroy(ogs_timer_mgr_t *manager)
{
    ogs_assert(manager);

    ogs_pool_final(&manager->pool);
    ogs_free(manager);
}

ogs_timer_t *ogs_timer_add(
        ogs_timer_mgr_t *manager, void (*cb)(void *data), void *data)
{
    ogs_timer_t *timer = NULL;
    ogs_assert(manager);

    ogs_pool_alloc(&manager->pool, &timer);
    if (!timer) {
        ogs_fatal("ogs_pool_alloc() failed");
        return NULL;
    }

    memset(timer, 0, sizeof *timer);
    timer->cb = cb;
    timer->data = data;

    timer->manager = manager;

    return timer;
}

void ogs_timer_delete_debug(ogs_timer_t *timer, const char *file_line)
{
    ogs_timer_mgr_t *manager;
    ogs_assert(timer);
    manager = timer->manager;
    ogs_assert(manager);

    ogs_timer_stop(timer);

    ogs_pool_free(&manager->pool, timer);
}

void ogs_timer_start_debug(
        ogs_timer_t *timer, ogs_time_t duration, const char *file_line)
{
    ogs_timer_mgr_t *manager = NULL;
    ogs_assert(timer);
    ogs_assert(duration);

    manager = timer->manager;
    ogs_assert(manager);

    if (timer->running == true)
        ogs_rbtree_delete(&manager->tree, timer);

    timer->running = true;
    add_timer_node(&manager->tree, timer, duration);
}

void ogs_timer_stop_debug(ogs_timer_t *timer, const char *file_line)
{
    ogs_timer_mgr_t *manager = NULL;
    ogs_assert(timer);
    manager = timer->manager;
    ogs_assert(manager);

    if (timer->running == false)
        return;

    timer->running = false;
    ogs_rbtree_delete(&manager->tree, timer);
}

ogs_time_t ogs_timer_mgr_next(ogs_timer_mgr_t *manager)
{
    ogs_time_t current;
    ogs_rbnode_t *rbnode = NULL;
    ogs_assert(manager);

    current = ogs_get_monotonic_time();
    rbnode = ogs_rbtree_first(&manager->tree);
    if (rbnode) {
        ogs_timer_t *this = ogs_rb_entry(rbnode, ogs_timer_t, rbnode);
        if (this->timeout > current) {
            return (this->timeout - current);
        } else {
            return OGS_NO_WAIT_TIME;
        }
    }

    return OGS_INFINITE_TIME;
}

void ogs_timer_mgr_expire(ogs_timer_mgr_t *manager)
{
    OGS_LIST(list);
    ogs_lnode_t *lnode;

    ogs_time_t current;
    ogs_rbnode_t *rbnode;
    ogs_timer_t *this;
    ogs_assert(manager);

    current = ogs_get_monotonic_time();

    ogs_rbtree_for_each(&manager->tree, rbnode) {
        this = ogs_rb_entry(rbnode, ogs_timer_t, rbnode);

        if (this->timeout > current)
            break;

        ogs_list_add(&list, &this->lnode);
    }

    /* You should not perform a delete on a timer using ogs_timer_delete()
     * in a callback function this->cb(). */
    ogs_list_for_each(&list, lnode) {
        this = ogs_rb_entry(lnode, ogs_timer_t, lnode);
        ogs_timer_stop(this);
        if (this->cb)
            this->cb(this->data);
    }
}
