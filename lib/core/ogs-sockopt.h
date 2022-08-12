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

#ifndef OGS_SOCKOPT_H
#define OGS_SOCKOPT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sockopt_s {
    struct {
        uint32_t spp_hbinterval;
        uint32_t spp_sackdelay;
        uint32_t srto_initial;
        uint32_t srto_min;
        uint32_t srto_max;
#define OGS_DEFAULT_SCTP_MAX_NUM_OF_OSTREAMS 30
        uint16_t sinit_num_ostreams;
        uint16_t sinit_max_instreams;
        uint16_t sinit_max_attempts;
        uint16_t sinit_max_init_timeo;
    } sctp;

    bool sctp_nodelay;
    bool tcp_nodelay;

    struct {
        bool l_onoff;
        int l_linger;
    } so_linger;

    const char *so_bindtodevice;
} ogs_sockopt_t;

void ogs_sockopt_init(ogs_sockopt_t *option);

int ogs_nonblocking(ogs_socket_t fd);
int ogs_closeonexec(ogs_socket_t fd);
int ogs_listen_reusable(ogs_socket_t fd, int on);
int ogs_tcp_nodelay(ogs_socket_t fd, int on);
int ogs_so_linger(ogs_socket_t fd, int l_linger);
int ogs_bind_to_device(ogs_socket_t fd, const char *device);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SOCKOPT_H */
