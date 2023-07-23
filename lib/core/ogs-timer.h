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

#ifndef OGS_TIMER_H
#define OGS_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_timer_mgr_s ogs_timer_mgr_t;
typedef struct ogs_timer_s {
    ogs_rbnode_t rbnode;
    ogs_lnode_t lnode;

    void (*cb)(void*);
    void *data;

    ogs_timer_mgr_t *manager;
    bool running;
    ogs_time_t timeout;
} ogs_timer_t;

ogs_timer_mgr_t *ogs_timer_mgr_create(unsigned int capacity);
void ogs_timer_mgr_destroy(ogs_timer_mgr_t *manager);

ogs_timer_t *ogs_timer_add(
        ogs_timer_mgr_t *manager, void (*cb)(void *data), void *data);
#define ogs_timer_delete(timer) \
    ogs_timer_delete_debug(timer, OGS_FILE_LINE)
void ogs_timer_delete_debug(ogs_timer_t *timer, const char *file_line);

#define ogs_timer_start(timer, duration) \
    ogs_timer_start_debug(timer, duration, OGS_FILE_LINE)
void ogs_timer_start_debug(
        ogs_timer_t *timer, ogs_time_t duration, const char *file_line);
#define ogs_timer_stop(timer) \
    ogs_timer_stop_debug(timer, OGS_FILE_LINE)
void ogs_timer_stop_debug(ogs_timer_t *timer, const char *file_line);

ogs_time_t ogs_timer_mgr_next(ogs_timer_mgr_t *manager);
void ogs_timer_mgr_expire(ogs_timer_mgr_t *manager);

#ifdef __cplusplus
}
#endif

#endif /* OGS_TIMER_H */
