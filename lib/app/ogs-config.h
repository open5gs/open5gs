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

        int no_amf;
        int no_smf;
        int no_upf;
        int no_ausf;
        int no_udm;
        int no_udr;
        int no_nrf;

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

        int ue;
        int smf;
        int upf;
        int gnb;
        int nf;
    } max;

    struct {
        ogs_pkbuf_config_t defconfig;
        int packet;

        int ue;
        int auth;
        int pfcp;
        int sess;
        int bearer;
        int tunnel;
        int pf;
        int nf_service;
        int nf_subscription;
        int sbi_message;
    } pool;

    struct {
        struct {
            int heartbeat_interval;
            int no_heartbeat_margin;
            int validity_duration;
        } nf_instance;
        struct {
            int validity_duration;
        } subscription;

        struct {
            ogs_time_t duration;
            struct {
                ogs_time_t client_wait_duration;
                ogs_time_t connection_deadline;
                ogs_time_t nf_register_interval;
                ogs_time_t nf_register_interval_in_exception;
            } sbi;

            struct {
                ogs_time_t t3_response_duration;
                int n3_response_rcount;
                ogs_time_t t3_holding_duration;
                int n3_holding_rcount;
            } gtp;

            struct {
                ogs_time_t t1_response_duration;
                int n1_response_rcount;
                ogs_time_t t1_holding_duration;
                int n1_holding_rcount;
                ogs_time_t association_interval;
                ogs_time_t no_heartbeat_duration;
            } pfcp;
        } message;
    } time;
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
