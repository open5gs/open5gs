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

#ifndef SGWC_S11_HANDLER_H
#define SGWC_S11_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

void sgwc_s11_handle_create_session_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp_message_t *message);
void sgwc_s11_handle_modify_bearer_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp_message_t *message);
void sgwc_s11_handle_delete_session_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp_message_t *message);
void sgwc_s11_handle_create_bearer_response(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp_message_t *message);
void sgwc_s11_handle_update_bearer_response(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp_message_t *message);
void sgwc_s11_handle_delete_bearer_response(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp_message_t *message);

void sgwc_s11_handle_release_access_bearers_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp_message_t *message);
void sgwc_s11_handle_downlink_data_notification_ack(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp_message_t *message);

void sgwc_s11_handle_create_indirect_data_forwarding_tunnel_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp_message_t *message);
void sgwc_s11_handle_delete_indirect_data_forwarding_tunnel_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp_message_t *message);

void sgwc_s11_handle_bearer_resource_command(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* SGWC_S11_HANDLER_H */
