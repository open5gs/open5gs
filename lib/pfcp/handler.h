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

#ifndef OGS_PFCP_HANDLER_H
#define OGS_PFCP_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

bool ogs_pfcp_handle_heartbeat_request(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_heartbeat_request_t *req);
bool ogs_pfcp_handle_heartbeat_response(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_heartbeat_response_t *req);

bool ogs_pfcp_cp_handle_association_setup_request(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_association_setup_request_t *req);
bool ogs_pfcp_cp_handle_association_setup_response(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_association_setup_response_t *req);

bool ogs_pfcp_up_handle_association_setup_request(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_association_setup_request_t *req);
bool ogs_pfcp_up_handle_association_setup_response(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_association_setup_response_t *req);

bool ogs_pfcp_up_handle_pdr(
        ogs_pfcp_pdr_t *pdr, uint8_t type,
        ogs_gtp2_header_desc_t *recvhdr, ogs_pkbuf_t *recvbuf,
        ogs_pfcp_user_plane_report_t *report);
bool ogs_pfcp_up_handle_error_indication(
        ogs_pfcp_far_t *far, ogs_pfcp_user_plane_report_t *report);

ogs_pfcp_pdr_t *ogs_pfcp_handle_create_pdr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_create_pdr_t *message,
        ogs_pfcp_sereq_flags_t *sereq_flags,
        uint8_t *cause_value, uint8_t *offending_ie_value);
ogs_pfcp_pdr_t *ogs_pfcp_handle_created_pdr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_created_pdr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);
ogs_pfcp_pdr_t *ogs_pfcp_handle_update_pdr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_update_pdr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);
bool ogs_pfcp_handle_remove_pdr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_remove_pdr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);

ogs_pfcp_far_t *ogs_pfcp_handle_create_far(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_create_far_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);
ogs_pfcp_far_t *ogs_pfcp_handle_update_far_flags(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_update_far_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);
ogs_pfcp_far_t *ogs_pfcp_handle_update_far(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_update_far_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);
bool ogs_pfcp_handle_remove_far(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_remove_far_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);

ogs_pfcp_qer_t *ogs_pfcp_handle_create_qer(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_create_qer_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);
ogs_pfcp_qer_t *ogs_pfcp_handle_update_qer(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_update_qer_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);
bool ogs_pfcp_handle_remove_qer(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_remove_qer_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);

ogs_pfcp_bar_t *ogs_pfcp_handle_create_bar(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_create_bar_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);
bool ogs_pfcp_handle_remove_bar(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_remove_bar_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);

ogs_pfcp_urr_t *ogs_pfcp_handle_create_urr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_create_urr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);
ogs_pfcp_urr_t *ogs_pfcp_handle_update_urr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_update_urr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);
bool ogs_pfcp_handle_remove_urr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_remove_urr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_HANDLER_H */
