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

#ifndef MME_S11_PATH_H
#define MME_S11_PATH_H

#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

int mme_gtp_open(void);
void mme_gtp_close(void);

int mme_gtp_send_create_session_request(mme_sess_t *sess);
int mme_gtp_send_modify_bearer_request(
        mme_bearer_t *bearer, int uli_presence);
int mme_gtp_send_delete_session_request(mme_sess_t *sess);
int mme_gtp_send_delete_all_sessions(mme_ue_t *mme_ue);
int mme_gtp_send_create_bearer_response(mme_bearer_t *bearer);
int mme_gtp_send_update_bearer_response(mme_bearer_t *bearer);
int mme_gtp_send_delete_bearer_response(mme_bearer_t *bearer);
int mme_gtp_send_release_access_bearers_request(mme_ue_t *mme_ue);

int mme_gtp_send_create_indirect_data_forwarding_tunnel_request(
        mme_ue_t *mme_ue);
int mme_gtp_send_delete_indirect_data_forwarding_tunnel_request(
        mme_ue_t *mme_ue);

#ifdef __cplusplus
}
#endif

#endif /* MME_S11_PATH_H */
