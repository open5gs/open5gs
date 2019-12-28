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

#ifndef SGW_S11_HANDLER_H
#define SGW_S11_HANDLER_H

#include "sgw-context.h"

#ifdef __cplusplus
extern "C" {
#endif

void sgw_s11_handle_create_session_request(ogs_gtp_xact_t *s11_xact,
        sgw_ue_t *sgw_ue, ogs_gtp_message_t *message);
void sgw_s11_handle_modify_bearer_request(ogs_gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, ogs_gtp_modify_bearer_request_t *req);
void sgw_s11_handle_delete_session_request(ogs_gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, ogs_gtp_message_t *message);
void sgw_s11_handle_create_bearer_response(ogs_gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, ogs_gtp_message_t *message);
void sgw_s11_handle_update_bearer_response(ogs_gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, ogs_gtp_message_t *message);
void sgw_s11_handle_delete_bearer_response(ogs_gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, ogs_gtp_message_t *message);

void sgw_s11_handle_release_access_bearers_request(
        ogs_gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue,
        ogs_gtp_release_access_bearers_request_t *req);
void sgw_s11_handle_lo_dldata_notification(sgw_bearer_t *bearer);
void sgw_s11_handle_downlink_data_notification_ack(
        ogs_gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue,
        ogs_gtp_downlink_data_notification_acknowledge_t *ack);

void sgw_s11_handle_create_indirect_data_forwarding_tunnel_request(
        ogs_gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue,
        ogs_gtp_create_indirect_data_forwarding_tunnel_request_t *req);
void sgw_s11_handle_delete_indirect_data_forwarding_tunnel_request(
        ogs_gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue);

void sgw_s11_handle_bearer_resource_command(ogs_gtp_xact_t *s11_xact,
        sgw_ue_t *sgw_ue, ogs_gtp_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* SGW_S11_HANDLER_H */
