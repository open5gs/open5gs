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

#ifndef MME_S11_BUILD_H
#define MME_S11_BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *mme_s11_build_create_session_request(
        uint8_t type, mme_sess_t *sess, int create_action);
ogs_pkbuf_t *mme_s11_build_modify_bearer_request(
        uint8_t type, mme_ue_t *mme_ue, int uli_presence);
ogs_pkbuf_t *mme_s11_build_delete_session_request(
        uint8_t type, mme_sess_t *sess, int action);
ogs_pkbuf_t *mme_s11_build_create_bearer_response(
        uint8_t type, mme_bearer_t *bearer, uint8_t cause_value);
ogs_pkbuf_t *mme_s11_build_update_bearer_response(
        uint8_t type, mme_bearer_t *bearer, uint8_t cause_value);
ogs_pkbuf_t *mme_s11_build_delete_bearer_response(
        uint8_t type, mme_bearer_t *bearer, uint8_t cause_value);
ogs_pkbuf_t *mme_s11_build_release_access_bearers_request(uint8_t type);
ogs_pkbuf_t *mme_s11_build_downlink_data_notification_ack(
        uint8_t type, uint8_t cause_value);
ogs_pkbuf_t *mme_s11_build_create_indirect_data_forwarding_tunnel_request(
        uint8_t type, mme_ue_t *mme_ue);

ogs_pkbuf_t *mme_s11_build_bearer_resource_command(
        uint8_t type, mme_bearer_t *bearer, ogs_nas_eps_message_t *nas_message);

#ifdef __cplusplus
}
#endif

#endif /* MME_S11_BUILD_H */
