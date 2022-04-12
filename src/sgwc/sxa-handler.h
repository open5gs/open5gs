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

#ifndef SGWC_SXA_HANDLER_H
#define SGWC_SXA_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

void sgwc_sxa_handle_session_establishment_response(
        sgwc_sess_t *sess, ogs_pfcp_xact_t *pfcp_xact,
        ogs_gtp2_message_t *gtp_message,
        ogs_pfcp_session_establishment_response_t *pfcp_rsp);
void sgwc_sxa_handle_session_modification_response(
        sgwc_sess_t *sess, ogs_pfcp_xact_t *pfcp_xact,
        ogs_gtp2_message_t *gtp_message,
        ogs_pfcp_session_modification_response_t *pfcp_rsp);
void sgwc_sxa_handle_session_deletion_response(
        sgwc_sess_t *sess, ogs_pfcp_xact_t *pfcp_xact,
        ogs_gtp2_message_t *gtp_message,
        ogs_pfcp_session_deletion_response_t *pfcp_rsp);
void sgwc_sxa_handle_session_report_request(
        sgwc_sess_t *sess, ogs_pfcp_xact_t *pfcp_xact,
        ogs_pfcp_session_report_request_t *pfcp_req);

#ifdef __cplusplus
}
#endif

#endif /* SGWC_SXA_HANDLER_H */
