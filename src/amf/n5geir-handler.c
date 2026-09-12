/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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

#include "n5geir-handler.h"
#include "amf-sm.h"

/* The 5G-EIR gave no usable answer: reject as for any unreachable NF */
ogs_nas_5gmm_cause_t amf_n5geir_eic_failure_cause(void)
{
    ogs_warn("Applying 5G-EIR failure_action[%s]",
            amf_self()->eir.failure_action ==
                AMF_EIR_ACTION_REJECT ? "reject" : "allow");
    return amf_self()->eir.failure_action == AMF_EIR_ACTION_REJECT ?
        OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED :
        OGS_5GMM_CAUSE_REQUEST_ACCEPTED;
}

ogs_nas_5gmm_cause_t amf_n5geir_eic_handle_equipment_status(
        amf_ue_t *amf_ue, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_eir_response_data_t *EirResponseData = NULL;

    ogs_assert(amf_ue);
    ogs_assert(recvmsg);

    if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_NOT_FOUND &&
        recvmsg->ProblemDetails && recvmsg->ProblemDetails->cause &&
        !strcmp(recvmsg->ProblemDetails->cause, "ERROR_EQUIPMENT_UNKNOWN")) {
        ogs_info("[%s] Unknown equipment [%s] [unknown_action:%s]",
                amf_ue->supi, amf_ue->pei,
                amf_self()->eir.unknown_action ==
                    AMF_EIR_ACTION_REJECT ? "reject" : "allow");
        return amf_self()->eir.unknown_action == AMF_EIR_ACTION_REJECT ?
            OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED :
            OGS_5GMM_CAUSE_REQUEST_ACCEPTED;
    }

    if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_OK) {
        ogs_warn("[%s] 5G-EIR HTTP response error [%d]",
                amf_ue->supi, recvmsg->res_status);
        return amf_n5geir_eic_failure_cause();
    }

    EirResponseData = recvmsg->EirResponseData;
    if (!EirResponseData) {
        ogs_error("[%s] No EirResponseData", amf_ue->supi);
        return amf_n5geir_eic_failure_cause();
    }

    switch (EirResponseData->status) {
    case OpenAPI_equipment_status_WHITELISTED:
        ogs_info("[%s] Whitelisted equipment [%s]",
                amf_ue->supi, amf_ue->pei);
        return OGS_5GMM_CAUSE_REQUEST_ACCEPTED;

    case OpenAPI_equipment_status_GREYLISTED:
        ogs_warn("[%s] Greylisted equipment [%s]", amf_ue->supi, amf_ue->pei);
        return OGS_5GMM_CAUSE_REQUEST_ACCEPTED;

    case OpenAPI_equipment_status_BLACKLISTED:
        ogs_warn("[%s] Blacklisted equipment [%s]",
                amf_ue->supi, amf_ue->pei);
        return OGS_5GMM_CAUSE_ILLEGAL_ME;

    default:
        ogs_error("[%s] Unknown EquipmentStatus [%d]",
                amf_ue->supi, EirResponseData->status);
        return amf_n5geir_eic_failure_cause();
    }
}

void amf_n5geir_eic_handle_discovery_failure(amf_ue_t *amf_ue)
{
    amf_event_t e;

    ogs_assert(amf_ue);

    if (!OGS_FSM_CHECK(&amf_ue->sm, gmm_state_security_mode) ||
            !amf_ue->eir_check_pending) {
        ogs_info("[%s] Ignore stale 5G-EIR discovery/communication failure",
                amf_ue->supi);
        return;
    }

    ogs_error("[%s] 5G-EIR discovery/communication failure", amf_ue->supi);

    /* Discovery failures and timeouts arrive outside UE FSM dispatch.
     * Dispatch locally so leaving security mode cancels its EIR transactions
     * and runs the next state's entry actions. */
    memset(&e, 0, sizeof(e));
    e.h.id = AMF_EVENT_5GMM_EIR_FAILURE;
    e.amf_ue_id = amf_ue->id;
    ogs_fsm_dispatch(&amf_ue->sm, &e);
}
