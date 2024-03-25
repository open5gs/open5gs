/*
 * Copyright (C) 2023 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#ifndef MME_GN_HANDLER_H
#define MME_GN_HANDLER_H

#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

void mme_gn_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp1_echo_request_t *req);

void mme_gn_handle_echo_response(
        ogs_gtp_xact_t *xact, ogs_gtp1_echo_response_t *req);

void mme_gn_handle_sgsn_context_request(
        ogs_gtp_xact_t *xact, ogs_gtp1_sgsn_context_request_t *req);

int mme_gn_handle_sgsn_context_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue, ogs_gtp1_sgsn_context_response_t *resp);

void mme_gn_handle_sgsn_context_acknowledge(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue, ogs_gtp1_sgsn_context_acknowledge_t *req);

void mme_gn_handle_ran_information_relay(
        ogs_gtp_xact_t *xact, ogs_gtp1_ran_information_relay_t *req);

void guti_to_rai_ptmsi(const ogs_nas_eps_guti_t *nas_guti, ogs_nas_rai_t *rai,
        mme_p_tmsi_t *ptmsi);

#ifdef __cplusplus
}
#endif

#endif /* MME_GN_HANDLER_H */
