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

#ifndef OGS_POLL_H
#define OGS_POLL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ogs_poll_handler_f)(short when, ogs_socket_t fd, void *data);

ogs_pollset_t *ogs_pollset_create(unsigned int capacity);
void ogs_pollset_destroy(ogs_pollset_t *pollset);

#define OGS_POLLIN      0x01
#define OGS_POLLOUT     0x02

ogs_poll_t *ogs_pollset_add(ogs_pollset_t *pollset, short when,
        ogs_socket_t fd, ogs_poll_handler_f handler, void *data);
void ogs_pollset_remove(ogs_poll_t *poll);

ogs_poll_t *ogs_pollset_cycle(ogs_pollset_t *pollset, ogs_poll_t *poll);
void *ogs_pollset_self_handler_data(void);

typedef struct ogs_pollset_actions_s {
    void (*init)(ogs_pollset_t *pollset);
    void (*cleanup)(ogs_pollset_t *pollset);

    int (*add)(ogs_poll_t *poll);
    int (*remove)(ogs_poll_t *poll);

    int (*poll)(ogs_pollset_t *pollset, ogs_time_t timeout);
    int (*notify)(ogs_pollset_t *pollset);
} ogs_pollset_actions_t;

extern ogs_pollset_actions_t ogs_pollset_actions;

#define ogs_pollset_poll ogs_pollset_actions.poll
#define ogs_pollset_notify ogs_pollset_actions.notify

#ifdef __cplusplus
}
#endif

#endif /* OGS_POLL_H */
