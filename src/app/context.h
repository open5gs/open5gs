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

#ifndef APP_CONTEXT_H
#define APP_CONTEXT_H

#include "ogs-core.h"
#include "ogs-yaml.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _config_t {
    const char *path;
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
        int enb;
        int ue;
        struct {
            int pool;
        } packet;
    } max;

} config_t;

typedef struct _context_t {
    config_t config;

    struct {
        bool initialized;
        const char *name;
        void *uri;
        void *client;
        void *database;
    } db;

    struct {
        const char *path;
        ogs_log_level_e level;
        const char *domain;
    } log;

    struct {
        int ue;
        int sess;
        int bearer;
        int tunnel;
        int pf;
    } pool;

} context_t;

int context_init(void);
int context_final(void);
context_t *context_self(void);

int context_read_file(void);
int context_parse_config(void);
int context_setup_log_module(void);

int context_db_init(const char *db_uri);
int context_db_final(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* APP_CONTEXT_H */
