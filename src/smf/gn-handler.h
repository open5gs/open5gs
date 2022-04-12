/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#ifndef SMF_GN_HANDLER_H
#define SMF_GN_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

void smf_gn_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp1_echo_request_t *req);
void smf_gn_handle_echo_response(
        ogs_gtp_xact_t *xact, ogs_gtp1_echo_response_t *req);

uint8_t smf_gn_handle_create_pdp_context_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp1_create_pdp_context_request_t *req);
void smf_gn_handle_delete_pdp_context_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp1_delete_pdp_context_request_t *req);
void smf_gn_handle_update_pdp_context_request(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp1_update_pdp_context_request_t *req);

#ifdef __cplusplus
}
#endif

#endif /* SMF_GN_HANDLER_H */
