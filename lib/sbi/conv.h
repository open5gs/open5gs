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

#if !defined(OGS_SBI_INSIDE) && !defined(OGS_SBI_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SBI_CONV_H
#define OGS_SBI_CONV_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sbi_server_s ogs_sbi_server_t;
typedef struct ogs_sbi_client_s ogs_sbi_client_t;
typedef struct ogs_sbi_header_s ogs_sbi_header_t;

char *ogs_uridup(bool https, ogs_sockaddr_t *addr, ogs_sbi_header_t *h);
char *ogs_sbi_server_uri(ogs_sbi_server_t *server, ogs_sbi_header_t *h);
char *ogs_sbi_client_uri(ogs_sbi_client_t *client, ogs_sbi_header_t *h);

char *ogs_sbi_parse_uri(char *uri, const char *delim, char **saveptr);
ogs_sockaddr_t *ogs_sbi_getaddr_from_uri(char *uri);

#define OGS_SBI_BITRATE_BPS     0
#define OGS_SBI_BITRATE_KBPS    1
#define OGS_SBI_BITRATE_MBPS    2
#define OGS_SBI_BITRATE_GBPS    3
#define OGS_SBI_BITRATE_TBPS    4
char *ogs_sbi_bitrate_to_string(uint64_t bitrate, int unit);
uint64_t ogs_sbi_bitrate_from_string(char *str);

int ogs_strftimezone(char *str, size_t size, int tm_gmtoff);
char *ogs_sbi_localtime_string(ogs_time_t time);
char *ogs_sbi_gmtime_string(ogs_time_t time);
char *ogs_sbi_timezone_string(int tm_offset);
bool ogs_sbi_time_from_string(ogs_time_t *time, char *str);

char *ogs_sbi_s_nssai_to_string(ogs_s_nssai_t *s_nssai);
bool ogs_sbi_s_nssai_from_string(ogs_s_nssai_t *s_nssai, char *str);

OpenAPI_plmn_id_t *ogs_sbi_build_plmn_id(ogs_plmn_id_t *plmn_id);
bool ogs_sbi_parse_plmn_id(
        ogs_plmn_id_t *plmn_id, OpenAPI_plmn_id_t *PlmnId);
void ogs_sbi_free_plmn_id(OpenAPI_plmn_id_t *PlmnId);

OpenAPI_plmn_id_nid_t *ogs_sbi_build_plmn_id_nid(ogs_plmn_id_t *plmn_id);
bool ogs_sbi_parse_plmn_id_nid(
        ogs_plmn_id_t *plmn_id, OpenAPI_plmn_id_nid_t *PlmnIdNid);
void ogs_sbi_free_plmn_id_nid(OpenAPI_plmn_id_nid_t *PlmnIdNid);

OpenAPI_guami_t *ogs_sbi_build_guami(ogs_guami_t *guami);
bool ogs_sbi_parse_guami(ogs_guami_t *guami, OpenAPI_guami_t *Guami);
void ogs_sbi_free_guami(OpenAPI_guami_t *Guami);

OpenAPI_nr_location_t *ogs_sbi_build_nr_location(
    ogs_5gs_tai_t *tai, ogs_nr_cgi_t *nr_cgi);
bool ogs_sbi_parse_nr_location(ogs_5gs_tai_t *tai, ogs_nr_cgi_t *nr_cgi,
        OpenAPI_nr_location_t *NrLocation);
void ogs_sbi_free_nr_location(OpenAPI_nr_location_t *NrLocation);

OpenAPI_pcc_rule_t *ogs_sbi_build_pcc_rule(
        ogs_pcc_rule_t *pcc_rule, int flow_presence);
void ogs_sbi_free_pcc_rule(OpenAPI_pcc_rule_t *PccRule);
OpenAPI_qos_data_t *ogs_sbi_build_qos_data(ogs_pcc_rule_t *pcc_rule);
void ogs_sbi_free_qos_data(OpenAPI_qos_data_t *QosData);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_CONV_H */
