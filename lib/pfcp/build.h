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

#ifndef OGS_PFCP_BUILD_H
#define OGS_PFCP_BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *ogs_pfcp_build_heartbeat_request(uint8_t type);
ogs_pkbuf_t *ogs_pfcp_build_heartbeat_response(uint8_t type);

ogs_pkbuf_t *ogs_pfcp_cp_build_association_setup_request(uint8_t type);
ogs_pkbuf_t *ogs_pfcp_cp_build_association_setup_response(uint8_t type,
        uint8_t cause);

ogs_pkbuf_t *ogs_pfcp_up_build_association_setup_request(uint8_t type);
ogs_pkbuf_t *ogs_pfcp_up_build_association_setup_response(uint8_t type,
        uint8_t cause);

void ogs_pfcp_pdrbuf_init(void);
void ogs_pfcp_pdrbuf_clear(void);

void ogs_pfcp_build_create_pdr(
    ogs_pfcp_tlv_create_pdr_t *message, int i, ogs_pfcp_pdr_t *pdr);
bool ogs_pfcp_build_created_pdr(
    ogs_pfcp_tlv_created_pdr_t *message, int i, ogs_pfcp_pdr_t *pdr);
void ogs_pfcp_build_update_pdr(
    ogs_pfcp_tlv_update_pdr_t *message, int i,
    ogs_pfcp_pdr_t *pdr, uint64_t modify_flags);

void ogs_pfcp_build_create_far(
    ogs_pfcp_tlv_create_far_t *message, int i, ogs_pfcp_far_t *far);
void ogs_pfcp_build_update_far_deactivate(
    ogs_pfcp_tlv_update_far_t *message, int i, ogs_pfcp_far_t *far);
void ogs_pfcp_build_update_far_activate(
    ogs_pfcp_tlv_update_far_t *message, int i, ogs_pfcp_far_t *far);

void ogs_pfcp_build_create_qer(
    ogs_pfcp_tlv_create_qer_t *message, int i, ogs_pfcp_qer_t *qer);
void ogs_pfcp_build_update_qer(
    ogs_pfcp_tlv_update_qer_t *message, int i, ogs_pfcp_qer_t *qer);

void ogs_pfcp_build_create_urr(
    ogs_pfcp_tlv_create_urr_t *message, int i, ogs_pfcp_urr_t *urr);
void ogs_pfcp_build_update_urr(
    ogs_pfcp_tlv_update_urr_t *message, int i, ogs_pfcp_urr_t *urr, uint64_t modify_flags);

void ogs_pfcp_build_create_bar(
    ogs_pfcp_tlv_create_bar_t *message, ogs_pfcp_bar_t *bar);

ogs_pkbuf_t *ogs_pfcp_build_session_report_request(
        uint8_t type, ogs_pfcp_user_plane_report_t *report);
ogs_pkbuf_t *ogs_pfcp_build_session_report_response(
        uint8_t type, uint8_t cause);

ogs_pkbuf_t *ogs_pfcp_build_session_deletion_response( uint8_t type, uint8_t cause,
        ogs_pfcp_user_plane_report_t *report);


#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_BUILD_H */
