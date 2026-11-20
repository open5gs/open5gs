/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
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

/*
 * SMF-side Nchf_ConvergedCharging request builders.
 *
 * Each function produces an ogs_sbi_request_t ready to send to the CHF via
 * smf_sbi_discover_and_send() / smf_sbi_send_request().  The caller is
 * responsible for freeing the returned request with ogs_sbi_request_free().
 *
 * JSON body construction uses cJSON directly (OpenAPI codegen for TS 32.291 is
 * not yet integrated; cJSON approach matches src/chf/nchf-handler.c).
 *
 * 3GPP references:
 *   TS 32.291 §6.3.1 — Nchf_ConvergedCharging_Create  (POST /chargingdata)
 *   TS 32.291 §6.3.2 — Nchf_ConvergedCharging_Update  (POST /{ref}/update)
 *   TS 32.291 §6.3.3 — Nchf_ConvergedCharging_Release (POST /{ref}/release)
 *   TS 32.291 §7     — ChargingDataRequest schema
 */

#ifndef SMF_NCHF_BUILD_H
#define SMF_NCHF_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * smf_nchf_convergedcharging_build_create()
 *
 * Builds: POST /nchf-convergedcharging/v3/chargingdata
 * Body:   ChargingDataRequest (initial — no usedUnitContainer)
 * TS 32.291 §6.3.1.2 — ChargingDataRequest for initial request.
 */
ogs_sbi_request_t *smf_nchf_convergedcharging_build_create(
        smf_sess_t *sess, void *data);

/*
 * smf_nchf_convergedcharging_build_update()
 *
 * Builds: POST /nchf-convergedcharging/v3/chargingdata/{ChargingDataRef}/update
 * Body:   ChargingDataRequest with multipleUnitUsage (delta since last report)
 * TS 32.291 §6.3.2.2 — ChargingDataRequest for update request.
 */
ogs_sbi_request_t *smf_nchf_convergedcharging_build_update(
        smf_sess_t *sess, void *data);

/*
 * smf_nchf_convergedcharging_build_release()
 *
 * Builds: POST /nchf-convergedcharging/v3/chargingdata/{ChargingDataRef}/release
 * Body:   ChargingDataRequest with final usedUnitContainer
 * TS 32.291 §6.3.3.2 — ChargingDataRequest for termination request.
 */
ogs_sbi_request_t *smf_nchf_convergedcharging_build_release(
        smf_sess_t *sess, void *data);

#ifdef __cplusplus
}
#endif

#endif /* SMF_NCHF_BUILD_H */
