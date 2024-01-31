/*
 * Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>
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

#include "nsmsf-handler.h"

#include "sbi-path.h"
#include "nas-path.h"

int amf_nsmsf_sm_service_handle_activate(
        amf_ue_t *amf_ue, int state, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_ue_sms_context_data_t *UeSmsContextData = NULL;

    ogs_assert(amf_ue);

    int r;

    UeSmsContextData = recvmsg->UeSmsContextData;
    if (!UeSmsContextData) {
        ogs_error("[%s] No UeSmsContextData", amf_ue->supi);
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    if (!UeSmsContextData->supi) {
        ogs_error("[%s] No supi", amf_ue->supi);
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    if (!UeSmsContextData->amf_id) {
        ogs_error("[%s] No amfId", amf_ue->supi);
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    if (!UeSmsContextData->access_type) {
        ogs_error("[%s] No accessType", amf_ue->supi);
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    amf_ue->sm_service_activated = true;

    return OGS_OK;
}

int amf_nsmsf_sm_service_handle_uplink_sms(
        amf_ue_t *amf_ue, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_sms_record_delivery_data_t *SmsRecordDeliveryData = NULL;

    ogs_assert(amf_ue);

    SmsRecordDeliveryData = recvmsg->SmsRecordDeliveryData;
    if (!SmsRecordDeliveryData) {
        ogs_error("[%s] No SmsRecordDeliveryData", amf_ue->supi);
        return OGS_ERROR;
    }

    if (!SmsRecordDeliveryData->sms_record_id) {
        ogs_error("[%s] No smsRecordId", amf_ue->supi);
        return OGS_ERROR;
    }

    if (!SmsRecordDeliveryData->delivery_status) {
        ogs_error("[%s] No deliveryStatus", amf_ue->supi);
        return OGS_ERROR;
    }
    
    ogs_debug("[%s] Uplink SMS was submitted", amf_ue->supi);

    return OGS_OK;
}
