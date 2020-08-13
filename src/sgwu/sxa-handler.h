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

#ifndef SGWU_SXA_HANDLER_H
#define SGWU_SXA_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

void sgwu_sxa_handle_session_establishment_request(
        sgwu_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_establishment_request_t *req);
void sgwu_sxa_handle_session_modification_request(
        sgwu_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_modification_request_t *req);
void sgwu_sxa_handle_session_deletion_request(
        sgwu_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_deletion_request_t *req);

void sgwu_sxa_handle_session_report_response(
        sgwu_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_report_response_t *rsp);

#ifdef __cplusplus
}
#endif

#endif /* SGWU_SXA_HANDLER_H */
