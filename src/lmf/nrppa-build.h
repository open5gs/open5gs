/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef LMF_NRPPA_BUILD_H
#define LMF_NRPPA_BUILD_H

#include "ogs-core.h"
#include "ogs-sbi.h"
#include "nrppa-types.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Build NRPPa ECID Measurement Initiation Request PDU */
ogs_pkbuf_t *lmf_nrppa_build_ecid_measurement_request(
        uint32_t measurement_id,
        uint64_t ue_ngap_id,
        uint8_t requested_measurements);

/* Free NRPPa PDU structure */
void lmf_nrppa_pdu_free(lmf_nrppa_pdu_t *pdu);

#ifdef __cplusplus
}
#endif

#endif /* LMF_NRPPA_BUILD_H */



