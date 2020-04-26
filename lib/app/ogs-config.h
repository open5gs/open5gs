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

#if !defined(OGS_APP_INSIDE) && !defined(OGS_APP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_APP_CONFIG_H
#define OGS_APP_CONFIG_H

#include "ogs-app.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_config_s {
    const char *file;
    void *document;

    const char *db_uri;
    struct {
        const char *file;
        const char *level;
        const char *domain;
    } logger;

    struct {
        /* Element */
        int no_hss;
        int no_sgw;
        int no_pgw;
        int no_pcrf;

        /* Network */
        int no_ipv4;
        int no_ipv6;
        int prefer_ipv4;
        int multicast;
        int no_slaac;
    } parameter;

    ogs_sockopt_t sockopt;
    struct {
        int udp_port;
    } usrsctp;

    struct {
        int sgw;
        int pgw;
        int vlr;
        int csmap;
        int enb;
        int ue;
        int smf;
        int upf;
    } max;

    struct {
        ogs_pkbuf_config_t defconfig;
        int packet;

        int ue;
        int pfcp;
        int sess;
        int bearer;
        int tunnel;
        int pf;
    } pool;
} ogs_config_t;

int ogs_config_init(void);
int ogs_config_final(void);
ogs_config_t *ogs_config(void);

int ogs_config_read(void);
int ogs_config_parse(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_APP_CONFIG_H */
