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

#ifndef SMF_GY_HANDLER_H
#define SMF_GY_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t smf_gy_handle_cca_initial_request(
        smf_sess_t *sess, ogs_diam_gy_message_t *gy_message,
        ogs_gtp_xact_t *gtp_xact);
void smf_gy_handle_cca_update_request(
        smf_sess_t *sess, ogs_diam_gy_message_t *gy_message,
        ogs_pfcp_xact_t *pfcp_xact);
uint32_t smf_gy_handle_cca_termination_request(
        smf_sess_t *sess, ogs_diam_gy_message_t *gy_message,
        ogs_gtp_xact_t *gtp_xact);
void smf_gy_handle_re_auth_request(
        smf_sess_t *sess, ogs_diam_gy_message_t *gy_message);

#ifdef __cplusplus
}
#endif

#endif /* SMF_GY_HANDLER_H */
