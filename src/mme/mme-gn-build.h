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

#ifndef MME_GN_BUILD_H
#define MME_GN_BUILD_H

#include "ogs-nas-eps.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

ogs_pkbuf_t *mme_gn_build_sgsn_context_request(
                mme_ue_t *mme_ue, const ogs_nas_p_tmsi_signature_t *ptmsi_sig);

ogs_pkbuf_t *mme_gn_build_sgsn_context_response(
                mme_ue_t *mme_ue, uint8_t cause);

ogs_pkbuf_t *mme_gn_build_sgsn_context_ack(
                mme_ue_t *mme_ue, uint8_t cause);

ogs_pkbuf_t *mme_gn_build_ran_information_relay(
                uint8_t type, const uint8_t *buf, size_t len,
                const ogs_nas_rai_t *rai, uint16_t cell_id);

#endif /* MME_S11_BUILD_H */
