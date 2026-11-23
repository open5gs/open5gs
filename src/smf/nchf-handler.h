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
 * SMF-side Nchf_ConvergedCharging response handlers.
 *
 * Each handler processes the CHF's HTTP response and updates smf_sess_t
 * state accordingly.  Return value is true on success (caller may proceed
 * to the next FSM step), false on error (caller should trigger teardown).
 *
 * 3GPP references:
 *   TS 32.291 §6.3.1.2 — CHF 201 Created  (Create response)
 *   TS 32.291 §6.3.2.2 — CHF 200 OK       (Update response)
 *   TS 32.291 §6.3.3.2 — CHF 204 No Content (Release response)
 */

#ifndef SMF_NCHF_HANDLER_H
#define SMF_NCHF_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The resource the SMF offers the CHF as notifyUri is
 * OGS_SBI_RESOURCE_NAME_CHARGING_NOTIFY, under the SMF's own nsmf-callback
 * service:
 *
 *   POST {nsmf-callback}/v1/charging-notify/{smContextRef}
 *
 * TS 32.291 defines the ChargingNotifyRequest body and leaves the URI to
 * the SMF, which hands it to the CHF in every ChargingDataRequest.
 */

/*
 * smf_nchf_convergedcharging_handle_charging_notify()
 *
 * Handles a ChargingNotifyRequest from the CHF (TS 32.291 §6.3.4): the
 * CHF has something this session should pick up now - a top-up, a bar, a
 * changed plan - rather than at its next report.  Answers the CHF and
 * reports the session's usage straight away with triggerType
 * FORCED_REAUTHORISATION, which is what fetches the new grant.
 *
 * Always answers `stream`; the CHF is never left waiting.
 *
 * The body arrives already parsed into recvmsg->ChargingNotifyRequest; a
 * notification with no usable notificationType is refused, since that
 * field is mandatory.
 */
void smf_nchf_convergedcharging_handle_charging_notify(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        ogs_sbi_message_t *recvmsg);

/*
 * smf_nchf_convergedcharging_handle_create()
 *
 * Processes CHF 201 Created response to POST /chargingdata.
 * On success:
 *   - Extracts Location header → stores ChargingDataRef via CHF_STORE()
 *   - Parses grantedUnit (totalVolume, validityTime) from response body
 *   - Configures bearer URR quota/threshold/validity for PFCP establishment
 * TS 32.291 §6.3.1.2 — ChargingDataResponse for initial request.
 *
 * The body arrives already parsed into recvmsg->ChargingDataResponse.
 */
bool smf_nchf_convergedcharging_handle_create(
        smf_sess_t *sess, ogs_sbi_message_t *recvmsg);

/*
 * smf_nchf_convergedcharging_handle_update()
 *
 * Processes CHF 200 OK response to POST /chargingdata/{ref}/update.
 * On success:
 *   - Parses refreshed grantedUnit from multipleUnitInformation
 *   - Updates bearer URR quota/threshold/validity
 *   - Sends PFCP Session Modification to push new URR params to UPF
 * TS 32.291 §6.3.2.2 — ChargingDataResponse for update request.
 *
 * The body arrives already parsed into recvmsg->ChargingDataResponse.
 */
bool smf_nchf_convergedcharging_handle_update(
        smf_sess_t *sess, ogs_sbi_message_t *recvmsg);

/*
 * smf_nchf_convergedcharging_handle_release()
 *
 * Processes CHF 204 No Content response to POST /chargingdata/{ref}/release.
 * On success:
 *   - Clears CHF association (CHF_CLEAR)
 *   - Clears in-flight guard
 * TS 32.291 §6.3.3.2 — ChargingDataResponse for termination request.
 */
bool smf_nchf_convergedcharging_handle_release(
        smf_sess_t *sess, ogs_sbi_message_t *recvmsg);

/*
 * smf_nchf_send_release()
 *
 * Send the final Nchf_ConvergedCharging Release for a session, once.
 * No-op when the session has no CHF association or a Release is already in
 * flight.  Callers must invoke it only AFTER the session's final usage has
 * been accumulated (see the PFCP deletion paths), since the Release carries
 * the last usage delta and nothing is reported after it.
 *
 * The response needs no state machine and is handled centrally in
 * smf-sm.c, so it is safe to call from any state.
 */
void smf_nchf_send_release(smf_sess_t *sess);

#ifdef __cplusplus
}
#endif

#endif /* SMF_NCHF_HANDLER_H */
