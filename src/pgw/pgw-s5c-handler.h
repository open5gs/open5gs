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

#ifndef PGW_S5C_HANDLER_H
#define PGW_S5C_HANDLER_H

#include "pgw-context.h"

#ifdef __cplusplus
extern "C" {
#endif

void pgw_s5c_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp_echo_request_t *req);
void pgw_s5c_handle_echo_response(
        ogs_gtp_xact_t *xact, ogs_gtp_echo_response_t *req);

void pgw_s5c_handle_create_session_request(
        pgw_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_create_session_request_t *req);
void pgw_s5c_handle_delete_session_request(
        pgw_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_delete_session_request_t *req);
void pgw_s5c_handle_create_bearer_response(
        pgw_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_create_bearer_response_t *req);
void pgw_s5c_handle_update_bearer_response(
        pgw_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_update_bearer_response_t *req);
void pgw_s5c_handle_delete_bearer_response(
        pgw_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_delete_bearer_response_t *req);
void pgw_s5c_handle_bearer_resource_command(
        pgw_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp_bearer_resource_command_t *cmd);

#ifdef __cplusplus
}
#endif

#endif /* PGW_S5C_HANDLER_H */
