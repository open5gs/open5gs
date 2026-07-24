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

static amf_eir_result_e failure_result(void)
{
    return amf_self()->eir.failure_action == AMF_EIR_ACTION_REJECT ?
        AMF_EIR_RESULT_REJECT : AMF_EIR_RESULT_ALLOW;
}

void amf_n5geir_eic_handle_equipment_status(
        amf_ue_t *amf_ue, ogs_sbi_message_t *recvmsg)
{
    ogs_assert(amf_ue);
    ogs_assert(recvmsg);

    switch (recvmsg->res_status) {
    case OGS_SBI_HTTP_STATUS_OK:
        if (!recvmsg->EirResponseData) {
            ogs_error("[%s] No EirResponseData", amf_ue->supi);
            gmm_ue_equipment_check_complete(amf_ue, failure_result());
            return;
        }

        switch (recvmsg->EirResponseData->status) {
        case OpenAPI_equipment_status_WHITELISTED:
            gmm_ue_equipment_check_complete(amf_ue, AMF_EIR_RESULT_ALLOW);
            break;

        case OpenAPI_equipment_status_GREYLISTED:
            ogs_warn("[%s] Equipment greylisted by 5G-EIR", amf_ue->supi);
            gmm_ue_equipment_check_complete(amf_ue, AMF_EIR_RESULT_ALLOW);
            break;

        case OpenAPI_equipment_status_BLACKLISTED:
            gmm_ue_equipment_check_complete(
                    amf_ue, AMF_EIR_RESULT_BLACKLISTED);
            break;

        default:
            ogs_error("[%s] Malformed EirResponseData", amf_ue->supi);
            gmm_ue_equipment_check_complete(amf_ue, failure_result());
        }
        break;

    case OGS_SBI_HTTP_STATUS_NOT_FOUND:
        ogs_info("[%s] Unknown equipment (5G-EIR)", amf_ue->supi);
        gmm_ue_equipment_check_complete(amf_ue,
                amf_self()->eir.unknown_action == AMF_EIR_ACTION_REJECT ?
                    AMF_EIR_RESULT_REJECT : AMF_EIR_RESULT_ALLOW);
        break;

    default:
        ogs_error("[%s] 5G-EIR HTTP response error [%d]",
                amf_ue->supi, recvmsg->res_status);
        gmm_ue_equipment_check_complete(amf_ue, failure_result());
    }
}

void amf_n5geir_eic_handle_discovery_failure(amf_ue_t *amf_ue)
{
    ogs_assert(amf_ue);

    ogs_error("[%s] 5G-EIR discovery/communication failure", amf_ue->supi);
    gmm_ue_equipment_check_complete(amf_ue, failure_result());
}
