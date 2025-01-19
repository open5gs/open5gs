/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_global_conf_s {
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
        int no_sepp;
        int no_scp;
        int no_nrf;

        int amf_count;
        int smf_count;
        int upf_count;
        int ausf_count;
        int udm_count;
        int pcf_count;
        int nssf_count;
        int bsf_count;
        int udr_count;

        /* Network */
        int no_ipv4;
        int no_ipv6;
        int prefer_ipv4;
        int multicast;

        int use_openair;
        int fake_csfb;
        int use_upg_vpp;
        int no_ipv4v6_local_addr_in_packet_filter;

        int no_pfcp_rr_select;
        int no_time_zone_information;
    } parameter;

    struct {
        uint64_t ue;
        uint64_t peer;
        uint64_t gtp_peer;
    } max;

    struct {
        int no_delay;
        int l_onoff;
        int l_linger;
    } sockopt;

    ogs_pkbuf_config_t pkbuf_config;

} ogs_app_global_conf_t;

typedef struct ogs_local_conf_s {
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
                ogs_time_t reconnect_interval;
                ogs_time_t reconnect_interval_in_exception;
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

    ogs_plmn_id_t serving_plmn_id[OGS_MAX_NUM_OF_PLMN];
    int num_of_serving_plmn_id;

    ogs_list_t policy_list;

} ogs_app_local_conf_t;

typedef struct ogs_app_policy_conf_s {
    ogs_lnode_t lnode;

    ogs_plmn_id_t plmn_id;

    ogs_list_t slice_list;
} ogs_app_policy_conf_t;

typedef struct ogs_app_slice_conf_s {
    ogs_lnode_t lnode;

    ogs_slice_data_t data;

    ogs_list_t sess_list;
    ogs_app_policy_conf_t *policy_conf;
} ogs_app_slice_conf_t;

typedef struct ogs_app_session_conf_s {
    ogs_lnode_t lnode;

    ogs_session_data_t data;

    ogs_app_slice_conf_t *slice_conf;
} ogs_app_session_conf_t;

int ogs_app_config_init(void);
void ogs_app_config_final(void);

ogs_app_global_conf_t *ogs_global_conf(void);
ogs_app_local_conf_t *ogs_local_conf(void);

int ogs_app_count_nf_conf_sections(const char *conf_section);
int ogs_app_global_conf_prepare(void);
int ogs_app_parse_global_conf(ogs_yaml_iter_t *parent);
int ogs_app_parse_local_conf(const char *local);

int ogs_app_parse_sockopt_config(
        ogs_yaml_iter_t *parent, ogs_sockopt_t *option);

int ogs_app_check_policy_conf(void);
int ogs_app_parse_session_conf(
        ogs_yaml_iter_t *parent, ogs_app_slice_conf_t *slice_conf);

ogs_app_policy_conf_t *ogs_app_policy_conf_add(ogs_plmn_id_t *plmn_id);
ogs_app_policy_conf_t *ogs_app_policy_conf_find_by_plmn_id(
        ogs_plmn_id_t *plmn_id);
void ogs_app_policy_conf_remove(ogs_app_policy_conf_t *policy_conf);
void ogs_app_policy_conf_remove_all(void);

ogs_app_slice_conf_t *ogs_app_slice_conf_add(
        ogs_app_policy_conf_t *policy_conf, ogs_s_nssai_t *s_nssai);
ogs_app_slice_conf_t *ogs_app_slice_conf_find_by_s_nssai(
        ogs_app_policy_conf_t *policy_conf, ogs_s_nssai_t *s_nssai);
void ogs_app_slice_conf_remove(ogs_app_slice_conf_t *slice_conf);
void ogs_app_slice_conf_remove_all(ogs_app_policy_conf_t *policy_conf);

ogs_app_session_conf_t *ogs_app_session_conf_add(
        ogs_app_slice_conf_t *slice_conf, char *name);
ogs_app_session_conf_t *ogs_app_session_conf_find_by_dnn(
        ogs_app_slice_conf_t *slice_conf, char *name);
void ogs_app_session_conf_remove(ogs_app_session_conf_t *session_conf);
void ogs_app_session_conf_remove_all(
        ogs_app_slice_conf_t *slice_conf);

int ogs_app_config_session_data(
        ogs_plmn_id_t *plmn_id, ogs_s_nssai_t *s_nssai, char *dnn,
        ogs_session_data_t *session_data);

#ifdef __cplusplus
}
#endif

#endif /* OGS_APP_CONFIG_H */
