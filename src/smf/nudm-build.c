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

ogs_sbi_request_t *smf_nudm_sdm_build_get(smf_sess_t *sess, void *data)
{
    smf_ue_t *smf_ue = NULL;
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);
    ogs_assert(smf_ue->supi);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_SDM;
    message.h.api.version = (char *)OGS_SBI_API_V2;
    message.h.resource.component[0] = smf_ue->supi;
    message.h.resource.component[1] = data;

    message.param.single_nssai_presence = true;
    memcpy(&message.param.s_nssai, &sess->s_nssai,
            sizeof(message.param.s_nssai));

    message.param.plmn_id_presence = true;
    memcpy(&message.param.plmn_id, &sess->home_plmn_id,
            sizeof(message.param.plmn_id));

    if (sess->session.name)
        message.param.dnn = sess->session.name;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}

ogs_sbi_request_t *smf_nudm_uecm_build_registration(
        smf_sess_t *sess, void *data)
{
    smf_ue_t *smf_ue = NULL;
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_smf_registration_t SmfRegistration;
    OpenAPI_snssai_t single_nssai;

    ogs_assert(sess);
    ogs_assert(sess->psi);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);
    ogs_assert(smf_ue->supi);

    memset(&message, 0, sizeof(message));

    memset(&SmfRegistration, 0, sizeof(SmfRegistration));
    message.SmfRegistration = &SmfRegistration;

    memset(&single_nssai, 0, sizeof(single_nssai));
    SmfRegistration.single_nssai = &single_nssai;

    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PUT;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_UECM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] = smf_ue->supi;
    message.h.resource.component[1] =
        (char *)OGS_SBI_RESOURCE_NAME_REGISTRATIONS;
    message.h.resource.component[2] =
        (char *)OGS_SBI_RESOURCE_NAME_SMF_REGISTRATIONS;
    message.h.resource.component[3] = ogs_msprintf("%d", sess->psi);
    if (!message.h.resource.component[3]) {
        ogs_error("No memory : message.h.resource.component[3]");
        goto end;
    }

    SmfRegistration.smf_instance_id =
        NF_INSTANCE_ID(ogs_sbi_self()->nf_instance);
    SmfRegistration.pdu_session_id = sess->psi;

    single_nssai.sst = sess->s_nssai.sst;
    single_nssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);

    SmfRegistration.dnn = sess->session.name;

    SmfRegistration.plmn_id = ogs_sbi_build_plmn_id(&sess->serving_plmn_id);
    if (!SmfRegistration.plmn_id) {
        ogs_error("No memory : SmfRegistration.plmn_id");
        goto end;
    }

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    ogs_free(message.h.resource.component[3]);
    ogs_free(single_nssai.sd);
    ogs_sbi_free_plmn_id(SmfRegistration.plmn_id);

    return request;
}

ogs_sbi_request_t *smf_nudm_uecm_build_deregistration(
        smf_sess_t *sess, void *data)
{
    smf_ue_t *smf_ue = NULL;
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sess);
    ogs_assert(sess->psi);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);
    ogs_assert(smf_ue->supi);

    memset(&message, 0, sizeof(message));

    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_UECM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] = smf_ue->supi;
    message.h.resource.component[1] =
        (char *)OGS_SBI_RESOURCE_NAME_REGISTRATIONS;
    message.h.resource.component[2] =
        (char *)OGS_SBI_RESOURCE_NAME_SMF_REGISTRATIONS;
    message.h.resource.component[3] = ogs_msprintf("%d", sess->psi);
    if (!message.h.resource.component[3]) {
        ogs_error("No memory : message.h.resource.component[3]");
        goto end;
    }

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    ogs_free(message.h.resource.component[3]);

    return request;
}

ogs_sbi_request_t *smf_nudm_sdm_build_subscription(
        smf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_header_t header;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;
    smf_ue_t *smf_ue;

    OpenAPI_sdm_subscription_t SDMSubscription;
    OpenAPI_snssai_t sNSSAI;
    uint64_t supported_features = 0;

    char *monres = NULL;

    ogs_assert(sess);
    ogs_assert(data);

    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);
    ogs_assert(smf_ue->supi);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_SDM;
    message.h.api.version = (char *)OGS_SBI_API_V2;
    message.h.resource.component[0] = smf_ue->supi;
    message.h.resource.component[1] =
            (char *)OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS;

    memset(&SDMSubscription, 0, sizeof(SDMSubscription));
    memset(&sNSSAI, 0, sizeof(sNSSAI));

    SDMSubscription.nf_instance_id =
        NF_INSTANCE_ID(ogs_sbi_self()->nf_instance);

    server = ogs_sbi_server_first();
    if (!server) {
        ogs_error("No server");
        goto end;
    }

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_CALLBACK;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SDMSUBSCRIPTION_NOTIFY;
    header.resource.component[1] = smf_ue->supi;
    SDMSubscription.callback_reference = ogs_sbi_server_uri(server, &header);
    if (!SDMSubscription.callback_reference) {
        ogs_error("No callback_reference");
        goto end;
    }

    SDMSubscription.monitored_resource_uris = OpenAPI_list_create();

    monres = ogs_msprintf("%s/%s", smf_ue->supi, (char *)data);
    ogs_assert(monres);

    OpenAPI_list_add(SDMSubscription.monitored_resource_uris, monres);
    SDMSubscription.is_implicit_unsubscribe = true;
    SDMSubscription.implicit_unsubscribe = 1;

    OGS_SBI_FEATURES_SET(supported_features,
            OGS_SBI_NUDM_SDM_LIMITED_SUBSCRIPTIONS);
    SDMSubscription.supported_features =
            ogs_uint64_to_string(supported_features);

    SDMSubscription.is_unique_subscription = true;
    SDMSubscription.unique_subscription = 1;

    SDMSubscription.dnn = sess->session.name;
    SDMSubscription.plmn_id = ogs_sbi_build_plmn_id(&sess->serving_plmn_id);

    sNSSAI.sst = sess->s_nssai.sst;
    sNSSAI.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
    SDMSubscription.single_nssai = &sNSSAI;

    message.SDMSubscription = &SDMSubscription;

    message.http.custom.callback =
        (char *)OGS_SBI_CALLBACK_NUDM_SDM_NOTIFICATION;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (sNSSAI.sd)
        ogs_free(sNSSAI.sd);
    if (SDMSubscription.plmn_id)
        ogs_sbi_free_plmn_id(SDMSubscription.plmn_id);
    if (SDMSubscription.supported_features)
        ogs_free(SDMSubscription.supported_features);
    if (monres)
        ogs_free(monres);
    OpenAPI_list_free(SDMSubscription.monitored_resource_uris);
    if (SDMSubscription.callback_reference)
        ogs_free(SDMSubscription.callback_reference);
    return request;
}

ogs_sbi_request_t *smf_nudm_sdm_build_subscription_delete(
        smf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sess);
    ogs_assert(sess->data_change_subscription.resource_uri);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.uri = sess->data_change_subscription.resource_uri;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}
