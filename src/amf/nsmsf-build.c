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

#include "nsmsf-build.h"

ogs_sbi_request_t *amf_nsmsf_sm_service_build_activate(
        amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;

    OpenAPI_ue_sms_context_data_t UeSmsContextData;
    OpenAPI_user_location_t ueLocation;

    ogs_assert(amf_ue);
    ogs_assert(amf_ue->supi);
    ogs_assert(ran_ue_cycle(amf_ue->ran_ue));

    ogs_debug("[%s] Activate SMService", amf_ue->supi);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PUT;
    message.h.service.name =
        (char *)OGS_SBI_SERVICE_NAME_NSMSF_SMS;
    message.h.api.version = (char *)OGS_SBI_API_V2;
    message.h.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_UE_CONTEXTS;
    message.h.resource.component[1] = amf_ue->supi;

    memset(&ueLocation, 0, sizeof(ueLocation));
    memset(&UeSmsContextData, 0, sizeof(UeSmsContextData));

    UeSmsContextData.amf_id = NF_INSTANCE_ID(ogs_sbi_self()->nf_instance);
    if (!UeSmsContextData.amf_id) {
        ogs_error("No amf_id");
        goto end;
    }

    UeSmsContextData.supi = amf_ue->supi;
    if (!UeSmsContextData.supi) {
        ogs_error("No supi");
        goto end;
    }

    UeSmsContextData.access_type = amf_ue->nas.access_type;
    if (!UeSmsContextData.access_type) {
        ogs_error("No access_type");
        goto end;
    }

    if (amf_ue->num_of_msisdn) {
        if (amf_ue->msisdn[0]) {
            UeSmsContextData.gpsi = ogs_msprintf("%s-%s",
                        OGS_ID_GPSI_TYPE_MSISDN, amf_ue->msisdn[0]);
            if (!UeSmsContextData.gpsi) {
                ogs_error("No gpsi");
                goto end;
            }
        }
    }

    memset(&ueLocation, 0, sizeof(ueLocation));
    ueLocation.nr_location = ogs_sbi_build_nr_location(
            &amf_ue->nr_tai, &amf_ue->nr_cgi);
    if (!ueLocation.nr_location) {
        ogs_error("No ueLocation.nr_location");
        goto end;
    }
    ueLocation.nr_location->ue_location_timestamp =
        ogs_sbi_gmtime_string(amf_ue->ue_location_timestamp);
    if (!ueLocation.nr_location->ue_location_timestamp) {
        ogs_error("No ueLocation.nr_location->ue_location_timestamp");
        goto end;
    }
    UeSmsContextData.ue_location = &ueLocation;

    UeSmsContextData.ue_time_zone =
        ogs_sbi_timezone_string(ogs_timezone());
    if (!UeSmsContextData.ue_time_zone) {
        ogs_error("No time_zone");
        goto end;
    }

    server = ogs_sbi_server_first();
    if (!server) {
        ogs_error("No server");
        goto end;
    }

    message.UeSmsContextData = &UeSmsContextData;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (UeSmsContextData.gpsi)
        ogs_free(UeSmsContextData.gpsi);

    if (ueLocation.nr_location) {
        if (ueLocation.nr_location->ue_location_timestamp)
            ogs_free(ueLocation.nr_location->ue_location_timestamp);
        ogs_sbi_free_nr_location(ueLocation.nr_location);
    }
    if (UeSmsContextData.ue_time_zone)
        ogs_free(UeSmsContextData.ue_time_zone);

    return request;
}

ogs_sbi_request_t *amf_nsmsf_sm_service_build_deactivate(
        amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_assert(amf_ue);
    ogs_assert(amf_ue->supi);

    ogs_debug("[%s] Deactivate SMService", amf_ue->supi);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.service.name =
        (char *)OGS_SBI_SERVICE_NAME_NSMSF_SMS;
    message.h.api.version = (char *)OGS_SBI_API_V2;
    message.h.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_UE_CONTEXTS;
    message.h.resource.component[1] = amf_ue->supi;

    server = ogs_sbi_server_first();
    if (!server) {
        ogs_error("No server");
        goto end;
    }

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:

    return request;
}

ogs_sbi_request_t *amf_nsmsf_sm_service_build_uplink_sms(
        amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_sms_record_data_t smsRecordData;
    OpenAPI_ref_to_binary_data_t smsPayload;

    OpenAPI_user_location_t ueLocation;

    ogs_assert(amf_ue);
    ogs_assert(amf_ue->supi);

    ogs_debug("[%s] Sending UplinkSMS", amf_ue->supi);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name =
        (char *)OGS_SBI_SERVICE_NAME_NSMSF_SMS;
    message.h.api.version = (char *)OGS_SBI_API_V2;
    message.h.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_UE_CONTEXTS;
    message.h.resource.component[1] = amf_ue->supi;
    message.h.resource.component[2] = (char *)OGS_SBI_RESOURCE_NAME_SEND_SMS;

    memset(&smsRecordData, 0, sizeof(smsRecordData));

    memset(&ueLocation, 0, sizeof(ueLocation));
    ueLocation.nr_location = ogs_sbi_build_nr_location(
            &amf_ue->nr_tai, &amf_ue->nr_cgi);
    if (!ueLocation.nr_location) {
        ogs_error("No ueLocation.nr_location");
        goto end;
    }
    ueLocation.nr_location->ue_location_timestamp =
        ogs_sbi_gmtime_string(amf_ue->ue_location_timestamp);
    if (!ueLocation.nr_location->ue_location_timestamp) {
        ogs_error("No ueLocation.nr_location->ue_location_timestamp");
        goto end;
    }
    smsRecordData.ue_location = &ueLocation;

    smsRecordData.ue_time_zone = ogs_sbi_timezone_string(ogs_timezone());
    if (!smsRecordData.ue_time_zone) {
        ogs_error("No time_zone");
        goto end;
    }

    smsRecordData.sms_record_id = (char *)"1";
    if (!smsRecordData.sms_record_id) {
        ogs_error("No recordId");
        goto end;
    }

    ogs_pkbuf_t *smsbuf = NULL;
    smsbuf = data;

    memset(&smsPayload, 0, sizeof(smsPayload));
    smsPayload.content_id = (char *)OGS_SBI_CONTENT_SMS_ID;
    smsRecordData.sms_payload = &smsPayload;

    message.part[message.num_of_part].pkbuf = smsbuf;
    message.part[message.num_of_part].content_id =
        (char *)OGS_SBI_CONTENT_SMS_ID;
    message.part[message.num_of_part].content_type =
        (char *)OGS_SBI_CONTENT_SMS_TYPE;
    message.num_of_part++;

    message.SmsRecordData = &smsRecordData;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (ueLocation.nr_location) {
        if (ueLocation.nr_location->ue_location_timestamp)
            ogs_free(ueLocation.nr_location->ue_location_timestamp);
        ogs_sbi_free_nr_location(ueLocation.nr_location);
    }
    if (smsRecordData.ue_time_zone)
        ogs_free(smsRecordData.ue_time_zone);

    return request;
}
