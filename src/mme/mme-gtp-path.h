/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef MME_S11_PATH_H
#define MME_S11_PATH_H

#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

int mme_gtp_open(void);
void mme_gtp_close(void);

int mme_gtp_send_create_session_request(
        enb_ue_t *enb_ue, mme_sess_t *sess, int create_action);
int mme_gtp_send_modify_bearer_request(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue,
        int uli_presence, int modify_action);
int mme_gtp_send_delete_session_request(
        enb_ue_t *enb_ue, sgw_ue_t *sgw_ue, mme_sess_t *sess, int action);
void mme_gtp_send_delete_all_sessions(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue, int action);
int mme_gtp_send_create_bearer_response(
        mme_bearer_t *bearer, uint8_t cause_value);
int mme_gtp_send_update_bearer_response(
        mme_bearer_t *bearer, uint8_t cause_value);
int mme_gtp_send_delete_bearer_response(
        mme_bearer_t *bearer, uint8_t cause_value);
int mme_gtp_send_release_access_bearers_request(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue, int action);
void mme_gtp_send_release_all_ue_in_enb(mme_enb_t *enb, int action);

int mme_gtp_send_downlink_data_notification_ack(
        mme_bearer_t *bearer, uint8_t cause_value);

int mme_gtp_send_create_indirect_data_forwarding_tunnel_request(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue);
int mme_gtp_send_delete_indirect_data_forwarding_tunnel_request(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue, int action);

int mme_gtp_send_bearer_resource_command(
        mme_bearer_t *bearer, ogs_nas_eps_message_t *nas_message);

int mme_gtp1_send_sgsn_context_request(
        mme_sgsn_t *sgsn, mme_ue_t *mme_ue, const ogs_nas_p_tmsi_signature_t *ptmsi_sig);

int mme_gtp1_send_sgsn_context_response(
        mme_ue_t *mme_ue, uint8_t cause, ogs_gtp_xact_t *xact);

int mme_gtp1_send_sgsn_context_ack(
        mme_ue_t *mme_ue, uint8_t cause, ogs_gtp_xact_t *xact);

int mme_gtp1_send_ran_information_relay(
        mme_sgsn_t *sgsn, const uint8_t *buf, size_t len,
        const ogs_nas_rai_t *rai, uint16_t cell_id);

#ifdef __cplusplus
}
#endif

#endif /* MME_S11_PATH_H */
