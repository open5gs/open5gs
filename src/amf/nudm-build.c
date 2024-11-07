/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "nudm-build.h"

ogs_sbi_request_t *amf_nudm_uecm_build_registration(
        amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_header_t header;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;

    OpenAPI_amf3_gpp_access_registration_t Amf3GppAccessRegistration;

    ogs_assert(amf_ue);
    ogs_assert(amf_ue->supi);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PUT;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_UECM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] = amf_ue->supi;
    message.h.resource.component[1] =
            (char *)OGS_SBI_RESOURCE_NAME_REGISTRATIONS;
    message.h.resource.component[2] =
            (char *)OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS;

    memset(&Amf3GppAccessRegistration, 0, sizeof(Amf3GppAccessRegistration));

    Amf3GppAccessRegistration.amf_instance_id =
        NF_INSTANCE_ID(ogs_sbi_self()->nf_instance);
    if (!Amf3GppAccessRegistration.amf_instance_id) {
        ogs_error("No amf_instance_id");
        goto end;
    }

    server = ogs_sbi_server_first();
    if (!server) {
        ogs_error("No server");
        goto end;
    }

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_CALLBACK;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = amf_ue->supi;
    header.resource.component[1] = (char *)OGS_SBI_RESOURCE_NAME_DEREG_NOTIFY;
    Amf3GppAccessRegistration.dereg_callback_uri =
                        ogs_sbi_server_uri(server, &header);
    if (!Amf3GppAccessRegistration.dereg_callback_uri) {
        ogs_error("No dereg_callback_uri");
        goto end;
    }

    Amf3GppAccessRegistration.guami = ogs_sbi_build_guami(amf_ue->guami);
    Amf3GppAccessRegistration.rat_type = amf_ue_rat_type(amf_ue);
    if (Amf3GppAccessRegistration.rat_type == OpenAPI_rat_type_NULL) {
        ogs_error("No rat_type");
        goto end;
    }
    Amf3GppAccessRegistration.pei = amf_ue->pei;

    if (amf_ue->nas.registration.value ==
                OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL) {
        Amf3GppAccessRegistration.is_initial_registration_ind = true;
        Amf3GppAccessRegistration.initial_registration_ind = 1;
    }

    message.Amf3GppAccessRegistration = &Amf3GppAccessRegistration;

    message.http.custom.callback =
        (char *)OGS_SBI_CALLBACK_NUDM_UECM_DEREGISTRATION_NOTIFICATION;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:

    if (Amf3GppAccessRegistration.guami)
        ogs_sbi_free_guami(Amf3GppAccessRegistration.guami);
    if (Amf3GppAccessRegistration.dereg_callback_uri)
        ogs_free(Amf3GppAccessRegistration.dereg_callback_uri);

    return request;
}

ogs_sbi_request_t *amf_nudm_uecm_build_registration_delete(
        amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_amf3_gpp_access_registration_modification_t
        Amf3GppAccessRegistrationModification;

    ogs_assert(amf_ue);
    ogs_assert(amf_ue->supi);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PATCH;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_UECM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] = amf_ue->supi;
    message.h.resource.component[1] =
            (char *)OGS_SBI_RESOURCE_NAME_REGISTRATIONS;
    message.h.resource.component[2] =
            (char *)OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS;

    memset(&Amf3GppAccessRegistrationModification, 0,
            sizeof(Amf3GppAccessRegistrationModification));

    Amf3GppAccessRegistrationModification.guami =
            ogs_sbi_build_guami(amf_ue->guami);
    if (!Amf3GppAccessRegistrationModification.guami) {
        ogs_error("No guami");
        goto end;
    }
    Amf3GppAccessRegistrationModification.is_purge_flag = true;
    Amf3GppAccessRegistrationModification.purge_flag = 1;

    message.Amf3GppAccessRegistrationModification =
            &Amf3GppAccessRegistrationModification;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:

    if (Amf3GppAccessRegistrationModification.guami)
        ogs_sbi_free_guami(Amf3GppAccessRegistrationModification.guami);

    return request;
}

ogs_sbi_request_t *amf_nudm_sdm_build_get(amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(amf_ue);
    ogs_assert(amf_ue->supi);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_SDM;
    message.h.api.version = (char *)OGS_SBI_API_V2;
    message.h.resource.component[0] = amf_ue->supi;
    message.h.resource.component[1] = data;

    message.param.plmn_id_presence = true;
    memcpy(&message.param.plmn_id, &amf_ue->home_plmn_id,
            sizeof(message.param.plmn_id));

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}

ogs_sbi_request_t *amf_nudm_sdm_build_subscription(amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_header_t header;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;

    OpenAPI_sdm_subscription_t SDMSubscription;
    uint64_t supported_features = 0;

    char *monres = NULL;

    ogs_assert(amf_ue);
    ogs_assert(amf_ue->supi);
    ogs_assert(data);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_SDM;
    message.h.api.version = (char *)OGS_SBI_API_V2;
    message.h.resource.component[0] = amf_ue->supi;
    message.h.resource.component[1] =
            (char *)OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS;

    memset(&SDMSubscription, 0, sizeof(SDMSubscription));

    SDMSubscription.nf_instance_id =
        NF_INSTANCE_ID(ogs_sbi_self()->nf_instance);

    server = ogs_sbi_server_first();
    if (!server) {
        ogs_error("No server");
        goto end;
    }

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_CALLBACK;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = amf_ue->supi;
    header.resource.component[1] =
            (char *)OGS_SBI_RESOURCE_NAME_SDMSUBSCRIPTION_NOTIFY;
    SDMSubscription.callback_reference = ogs_sbi_server_uri(server, &header);
    if (!SDMSubscription.callback_reference) {
        ogs_error("No callback_reference");
        goto end;
    }

    SDMSubscription.monitored_resource_uris = OpenAPI_list_create();

    monres = ogs_msprintf("%s/%s", amf_ue->supi, (char *)data);
    if (!monres) {
        ogs_error("No monres");
        goto end;
    }

    OpenAPI_list_add(SDMSubscription.monitored_resource_uris, monres);
    SDMSubscription.is_implicit_unsubscribe = true;
    SDMSubscription.implicit_unsubscribe = 1;

    OGS_SBI_FEATURES_SET(supported_features,
            OGS_SBI_NUDM_SDM_LIMITED_SUBSCRIPTIONS);
    SDMSubscription.supported_features =
            ogs_uint64_to_string(supported_features);

    SDMSubscription.is_unique_subscription = true;
    SDMSubscription.unique_subscription = 1;

    message.SDMSubscription = &SDMSubscription;

    message.http.custom.callback =
        (char *)OGS_SBI_CALLBACK_NUDM_SDM_NOTIFICATION;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (SDMSubscription.supported_features)
        ogs_free(SDMSubscription.supported_features);
    if (monres)
        ogs_free(monres);
    OpenAPI_list_free(SDMSubscription.monitored_resource_uris);
    if (SDMSubscription.callback_reference)
        ogs_free(SDMSubscription.callback_reference);

    return request;
}

ogs_sbi_request_t *amf_nudm_sdm_build_subscription_delete(
        amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(amf_ue);
    ogs_assert(amf_ue->supi);
    ogs_assert(amf_ue->data_change_subscription.resource_uri);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.uri = amf_ue->data_change_subscription.resource_uri;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}
