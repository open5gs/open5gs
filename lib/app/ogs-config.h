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
        uint64_t ue;
        uint64_t peer;
        uint64_t gtp_peer;
    } max;

    struct {
        int no_delay;
        int l_onoff;
        int l_linger;
    } sockopt;

} ogs_global_conf_t;

int ogs_global_conf_init(void);
void ogs_global_conf_final(void);
ogs_global_conf_t *ogs_global_conf(void);

int ogs_global_conf_parse_sockopt(
        ogs_yaml_iter_t *parent, ogs_sockopt_t *option);

#ifdef __cplusplus
}
#endif

#endif /* OGS_APP_CONFIG_H */
