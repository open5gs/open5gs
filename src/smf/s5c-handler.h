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

#ifndef SMF_S5C_HANDLER_H
#define SMF_S5C_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

void smf_s5c_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp2_echo_request_t *req);
void smf_s5c_handle_echo_response(
        ogs_gtp_xact_t *xact, ogs_gtp2_echo_response_t *req);

uint8_t smf_s5c_handle_create_session_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_create_session_request_t *req);
uint8_t smf_s5c_handle_delete_session_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_delete_session_request_t *req);
void smf_s5c_handle_modify_bearer_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_modify_bearer_request_t *req);
void smf_s5c_handle_create_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_create_bearer_response_t *rsp);
void smf_s5c_handle_update_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_update_bearer_response_t *rsp);
bool smf_s5c_handle_delete_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_delete_bearer_response_t *rsp);
void smf_s5c_handle_bearer_resource_command(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_bearer_resource_command_t *cmd);

#ifdef __cplusplus
}
#endif

#endif /* SMF_S5C_HANDLER_H */
