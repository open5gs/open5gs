/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef NGAP_HANDLER_H
#define NGAP_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

void ngap_handle_ng_setup_request(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
#if 0
void ngap_handle_initial_ue_message(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
void ngap_handle_uplink_nas_transport(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
void ngap_handle_ue_capability_info_indication(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
void ngap_handle_initial_context_setup_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
void ngap_handle_initial_context_setup_failure(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);

void ngap_handle_ue_context_modification_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
void ngap_handle_ue_context_modification_failure(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);

void ngap_handle_ue_context_release_request(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
void ngap_handle_ue_context_release_complete(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);

void ngap_handle_e_rab_setup_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);

void ngap_handle_path_switch_request(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);

void ngap_handle_handover_required(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
void ngap_handle_handover_request_ack(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
void ngap_handle_handover_failure(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
void ngap_handle_handover_cancel(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);

void ngap_handle_gnb_status_transfer(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
void ngap_handle_gnb_configuration_transfer(
        amf_gnb_t *gnb, ogs_ngap_message_t *message, ogs_pkbuf_t *pkbuf);
void ngap_handle_handover_notification(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);

void ngap_handle_s1_reset(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);

void ngap_handle_write_replace_warning_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
void ngap_handle_kill_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message);
#endif

#ifdef __cplusplus
}
#endif

#endif /* NGAP_HANDLER_H */
