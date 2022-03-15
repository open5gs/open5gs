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

#ifndef OGS_SOCKNODE_H
#define OGS_SOCKNODE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_pollset_s ogs_pollset_t;
typedef struct ogs_poll_s ogs_poll_t;

typedef struct ogs_socknode_s {
    ogs_lnode_t node;

    ogs_sockaddr_t *addr;
    char *dev;

    ogs_sock_t *sock;
    void (*cleanup)(ogs_sock_t *sock);
    ogs_poll_t *poll;

    ogs_sockopt_t *option;
} ogs_socknode_t;

ogs_socknode_t *ogs_socknode_new(ogs_sockaddr_t *addr);
void ogs_socknode_free(ogs_socknode_t *node);

ogs_socknode_t *ogs_socknode_add(ogs_list_t *list,
        int family, ogs_sockaddr_t *addr, ogs_sockopt_t *option);
void ogs_socknode_remove(ogs_list_t *list, ogs_socknode_t *node);
void ogs_socknode_remove_all(ogs_list_t *list);

int ogs_socknode_probe(ogs_list_t *list, ogs_list_t *list6,
        const char *dev, uint16_t port, ogs_sockopt_t *option);
#if 0 /* deprecated */
int ogs_socknode_fill_scope_id_in_local(ogs_sockaddr_t *sa_list);
#endif

void ogs_socknode_set_cleanup(
        ogs_socknode_t *node, void (*cleanup)(ogs_sock_t *));

ogs_sock_t *ogs_socknode_sock_first(ogs_list_t *list);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SOCKNODE_H */
