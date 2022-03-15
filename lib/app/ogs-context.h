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

#ifndef OGS_APP_CONTEXT_H
#define OGS_APP_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_app_context_s {
    const char *version;

    const char *file;
    void *document;

    const char *db_uri;
    struct {
        const char *file;
        const char *level;
        const char *domain;
    } logger;

    ogs_queue_t *queue;
    ogs_timer_mgr_t *timer_mgr;
    ogs_pollset_t *pollset;

    struct {
        /* Element */
        int no_mme;
        int no_hss;
        int no_sgw;
        int no_sgwc;
        int no_sgwu;
        int no_pgw;
        int no_pcrf;

        int no_amf;
        int no_smf;
        int no_upf;
        int no_ausf;
        int no_udm;
        int no_pcf;
        int no_nssf;
        int no_bsf;
        int no_udr;
        int no_nrf;

        /* Network */
        int no_ipv4;
        int no_ipv6;
        int prefer_ipv4;
        int multicast;

        int use_openair;
        int no_ipv4v6_local_addr_in_packet_filter;

        int no_pfcp_rr_select;
    } parameter;

    struct {
        int no_delay;
        int l_onoff;
        int l_linger;
    } sockopt;

    struct {
        int heartbit_interval;
        int sack_delay;
        int rto_initial;
        int rto_min;
        int rto_max;
        int max_num_of_ostreams;
        int max_num_of_istreams;
        int max_attempts;
        int max_initial_timeout;
    } sctp;

    struct {
        int udp_port;
    } usrsctp;

    struct {
        uint64_t ue;
        uint64_t gnb;
    } max;

    struct {
        ogs_pkbuf_config_t defconfig;
        uint64_t packet;

        uint64_t nf;

        uint64_t sess;
        uint64_t bearer;
        uint64_t tunnel;
        uint64_t nf_service;
        uint64_t nf_subscription;

        uint64_t csmap;

        uint64_t message;
        uint64_t event;
        uint64_t timer;

        uint64_t socket;
        uint64_t gtp_xact;
        uint64_t gtp_node;
        uint64_t pfcp_xact;
        uint64_t pfcp_node;

        uint64_t impi;
        uint64_t impu;
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

        struct {
            ogs_time_t duration;
            ogs_time_t complete_delay;
        } handover;

    } time;
} ogs_app_context_t;

int ogs_app_context_init(void);
void ogs_app_context_final(void);
ogs_app_context_t *ogs_app(void);

int ogs_app_context_parse_config(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_APP_CONTEXT_H */
