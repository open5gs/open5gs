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

#if !defined(OGS_DIAMETER_INSIDE) && !defined(OGS_DIAMETER_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_DIAM_BASE_H
#define OGS_DIAM_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

/* This is default diameter configuration if there is no config file 
 * The Configuration : No TLS, Only TCP */
typedef struct ogs_diam_config_s {
    /* Diameter Identity of the local peer (FQDN -- ASCII) */
    const char *cnf_diamid; 
    /* Diameter realm of the local peer, default to realm part of cnf_diamid */
    const char *cnf_diamrlm; 
    /* IP address of the local peer */
    const char *cnf_addr;

    /* the local port for legacy Diameter (default: 3868) in host byte order */
    uint16_t cnf_port;
    /* the local port for Diameter/TLS (default: 5658) in host byte order */
    uint16_t cnf_port_tls;

    struct {
        /* the peer does not relay messages (0xffffff app id) */
        unsigned no_fwd: 1;
        /* disable the use of SCTP */
        unsigned no_sctp: 1;
    } cnf_flags;

#define MAX_NUM_OF_FD_EXTENSION 32
    struct {
        const char *module;
        const char *conf;
    } ext[MAX_NUM_OF_FD_EXTENSION];
    int num_of_ext;

#define MAX_NUM_OF_FD_CONN 16
    /* (supposedly) UTF-8, \0 terminated. 
     * The Diameter Identity of the remote peer. */
    struct {
        const char *identity; 
        const char *addr; /* IP address of the remote peer */
        uint16_t port; /* port to connect to. 0: default. */
    } conn[MAX_NUM_OF_FD_CONN];
    int num_of_conn;
} ogs_diam_config_t;

int ogs_diam_init(int mode, const char *conffile, ogs_diam_config_t *fd_config);
int ogs_diam_start(void);
void ogs_diam_final(void);

int ogs_diam_config_init(ogs_diam_config_t *fd_config);
bool ogs_diam_app_connected(uint32_t app_id);

int fd_avp_search_avp ( struct avp * groupedavp, 
        struct dict_object * what, struct avp ** avp );

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_BASE_H */
