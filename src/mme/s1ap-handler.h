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

#ifndef S1AP_HANDLER_H
#define S1AP_HANDLER_H

#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

void s1ap_handle_s1_setup_request(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_initial_ue_message(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_uplink_nas_transport(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_ue_capability_info_indication(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_initial_context_setup_response(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_initial_context_setup_failure(
        mme_enb_t *enb, ogs_s1ap_message_t *message);

void s1ap_handle_ue_context_modification_response(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_ue_context_modification_failure(
        mme_enb_t *enb, ogs_s1ap_message_t *message);

void s1ap_handle_ue_context_release_request(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_ue_context_release_complete(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_ue_context_release_action(enb_ue_t *enb_ue);

void s1ap_handle_e_rab_setup_response(
        mme_enb_t *enb, ogs_s1ap_message_t *message);

void s1ap_handle_e_rab_modification_indication(
        mme_enb_t *enb, ogs_s1ap_message_t *message);

void s1ap_handle_path_switch_request(
        mme_enb_t *enb, ogs_s1ap_message_t *message);

void s1ap_handle_handover_required(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_handover_request_ack(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_handover_failure(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_handover_cancel(
        mme_enb_t *enb, ogs_s1ap_message_t *message);

void s1ap_handle_enb_status_transfer(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_enb_configuration_transfer(
        mme_enb_t *enb, ogs_s1ap_message_t *message, ogs_pkbuf_t *pkbuf);
void s1ap_handle_handover_notification(
        mme_enb_t *enb, ogs_s1ap_message_t *message);

void s1ap_handle_s1_reset(
        mme_enb_t *enb, ogs_s1ap_message_t *message);

void s1ap_handle_write_replace_warning_response(
        mme_enb_t *enb, ogs_s1ap_message_t *message);
void s1ap_handle_kill_response(
        mme_enb_t *enb, ogs_s1ap_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* S1AP_HANDLER_H */
