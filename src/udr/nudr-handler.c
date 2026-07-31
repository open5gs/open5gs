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

#include "sbi-path.h"
#include "nudr-handler.h"

static bool nudr_validate_imsi_supi(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg, char *supi)
{
    char *type = NULL;
    char *value = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    if (!supi) {
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No SUPI", NULL, NULL));
        return false;
    }

    if (ogs_id_get_type_value(supi, &type, &value) == false) {
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Invalid SUPI", supi, NULL));
        return false;
    }

    if (strcmp(type, OGS_ID_SUPI_TYPE_IMSI) != 0) {
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                recvmsg, "Unknwon SUPI Type", supi, NULL));
        goto invalid;
    }

    if (ogs_imsi_bcd_is_valid(value) == false) {
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Invalid SUPI", supi, NULL));
        goto invalid;
    }

    ogs_free(type);
    ogs_free(value);

    return true;

invalid:
    if (type)
        ogs_free(type);
    if (value)
        ogs_free(value);

    return false;
}

bool udr_nudr_dr_handle_subscription_authentication(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;
    ogs_dbi_auth_info_t auth_info;

    char k_string[OGS_KEYSTRLEN(OGS_KEY_LEN)];
    char opc_string[OGS_KEYSTRLEN(OGS_KEY_LEN)];
    char amf_string[OGS_KEYSTRLEN(OGS_AMF_LEN)];
    char sqn_string[OGS_KEYSTRLEN(OGS_SQN_LEN)];

    char sqn[OGS_SQN_LEN];
    char *supi = NULL;

    OpenAPI_authentication_subscription_t AuthenticationSubscription;
    OpenAPI_sequence_number_t SequenceNumber;
    OpenAPI_list_t *PatchItemList = NULL;
    OpenAPI_lnode_t *node = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    supi = recvmsg->h.resource.component[1];
    if (nudr_validate_imsi_supi(stream, recvmsg, supi) == false) {
        ogs_error("Invalid SUPI [%s]", supi ? supi : "NULL");
        return false;
    }

    rv = ogs_dbi_auth_info(supi, &auth_info);
    if (rv != OGS_OK) {
        ogs_warn("[%s] Cannot find SUPI in DB", supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                recvmsg, "Cannot find SUPI Type", supi, NULL));
        return false;
    }

    SWITCH(recvmsg->h.resource.component[3])
    CASE(OGS_SBI_RESOURCE_NAME_AUTHENTICATION_SUBSCRIPTION)
        SWITCH(recvmsg->h.method)
        CASE(OGS_SBI_HTTP_METHOD_GET)
            memset(&AuthenticationSubscription, 0,
                    sizeof(AuthenticationSubscription));

            AuthenticationSubscription.authentication_method =
                OpenAPI_auth_method_5G_AKA;

            ogs_hex_to_ascii(auth_info.k, sizeof(auth_info.k),
                    k_string, sizeof(k_string));
            AuthenticationSubscription.enc_permanent_key = k_string;

            ogs_hex_to_ascii(auth_info.amf, sizeof(auth_info.amf),
                    amf_string, sizeof(amf_string));
            AuthenticationSubscription.authentication_management_field =
                    amf_string;

            if (!auth_info.use_opc)
                milenage_opc(auth_info.k, auth_info.op, auth_info.opc);

            ogs_hex_to_ascii(auth_info.opc, sizeof(auth_info.opc),
                    opc_string, sizeof(opc_string));
            AuthenticationSubscription.enc_opc_key = opc_string;

            ogs_uint64_to_buffer(auth_info.sqn, OGS_SQN_LEN, sqn);
            ogs_hex_to_ascii(sqn, sizeof(sqn), sqn_string, sizeof(sqn_string));

            memset(&SequenceNumber, 0, sizeof(SequenceNumber));
            SequenceNumber.sqn = sqn_string;
            AuthenticationSubscription.sequence_number = &SequenceNumber;

            memset(&sendmsg, 0, sizeof(sendmsg));

            ogs_assert(AuthenticationSubscription.authentication_method);
            sendmsg.AuthenticationSubscription =
                &AuthenticationSubscription;

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_OK);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));

            return true;

        CASE(OGS_SBI_HTTP_METHOD_PATCH)
            char *sqn_string = NULL;
            uint8_t sqn_ms[OGS_SQN_LEN];
            uint64_t sqn = 0;

            PatchItemList = recvmsg->PatchItemList;
            if (!PatchItemList) {
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        recvmsg, "No PatchItemList Array", NULL, NULL));
                return false;
            }

            OpenAPI_list_for_each(PatchItemList, node) {
                if (node->data) {
                    OpenAPI_patch_item_t *patch_item = node->data;
                    if (OpenAPI_IsString(patch_item->value))
                        sqn_string = cJSON_GetStringValue(patch_item->value->json);
                    else
                        ogs_error("Non-string value in patch not implemented");
                }
            }

            if (!sqn_string) {
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        recvmsg, "No PatchItemList", NULL, NULL));
                return false;
            }

            ogs_ascii_to_hex(sqn_string, strlen(sqn_string),
                    sqn_ms, sizeof(sqn_ms));
            sqn = ogs_buffer_to_uint64(sqn_ms, OGS_SQN_LEN);

            rv = ogs_dbi_update_sqn(supi, sqn);
            if (rv != OGS_OK) {
                ogs_fatal("[%s] Cannot update SQN", supi);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                        recvmsg, "Cannot update SQN", supi, NULL));
                return false;
            }

            rv = ogs_dbi_increment_sqn(supi);
            if (rv != OGS_OK) {
                ogs_fatal("[%s] Cannot increment SQN", supi);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                        recvmsg, "Cannot increment SQN", supi, NULL));
                return false;
            }

            memset(&sendmsg, 0, sizeof(sendmsg));

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));

            return true;

        DEFAULT
            ogs_error("Invalid HTTP method [%s]", recvmsg->h.method);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED,
                    recvmsg, "Invalid HTTP method", recvmsg->h.method,
                    NULL));
        END
        break;

    CASE(OGS_SBI_RESOURCE_NAME_AUTHENTICATION_STATUS)
        SWITCH(recvmsg->h.method)
        CASE(OGS_SBI_HTTP_METHOD_PUT)
        CASE(OGS_SBI_HTTP_METHOD_DELETE)
            OpenAPI_auth_event_t *AuthEvent = NULL;

            AuthEvent = recvmsg->AuthEvent;
            if (!AuthEvent &&
                !strcmp(recvmsg->h.method, OGS_SBI_HTTP_METHOD_PUT)) {
                ogs_error("[%s] No AuthEvent", supi);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(
                        stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        recvmsg, "No AuthEvent", supi, NULL));
                return false;
            }

            memset(&sendmsg, 0, sizeof(sendmsg));
            rv = ogs_dbi_increment_sqn(supi);
            if (rv != OGS_OK) {
                ogs_fatal("[%s] Cannot increment SQN", supi);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                        recvmsg, "Cannot increment SQN", supi, NULL));
                return false;
            }

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));

            return true;

        DEFAULT
            ogs_error("Invalid HTTP method [%s]", recvmsg->h.method);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED,
                    recvmsg, "Invalid HTTP method", recvmsg->h.method,
                    NULL));
        END
        break;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED,
                recvmsg, "Unknown resource name",
                recvmsg->h.resource.component[3], NULL));
    END

    return false;
}

bool udr_nudr_dr_handle_subscription_context(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    char *supi = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    supi = recvmsg->h.resource.component[1];
    if (nudr_validate_imsi_supi(stream, recvmsg, supi) == false) {
        ogs_error("Invalid SUPI [%s]", supi ? supi : "NULL");
        return false;
    }

    SWITCH(recvmsg->h.resource.component[3])
    CASE(OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS)
        SWITCH(recvmsg->h.method)
        CASE(OGS_SBI_HTTP_METHOD_PUT)
            OpenAPI_amf3_gpp_access_registration_t *Amf3GppAccessRegistration;

            Amf3GppAccessRegistration = recvmsg->Amf3GppAccessRegistration;
            if (!Amf3GppAccessRegistration) {
                ogs_error("[%s] No Amf3GppAccessRegistration", supi);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(
                        stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        recvmsg, "No Amf3GppAccessRegistration", supi, NULL));
                return false;
            }

            if (Amf3GppAccessRegistration->pei) {
                char *type = NULL, *value = NULL;
                char *pei = Amf3GppAccessRegistration->pei;

                if (ogs_id_get_type_value(pei, &type, &value) == false) {
                    ogs_error("Invalid PEI [%s]", pei);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                            recvmsg, "Invalid PEI", pei, NULL));
                    return false;
                }

                if (strcmp(type, OGS_ID_SUPI_TYPE_IMEISV) == 0) {
                    if (ogs_imeisv_bcd_is_valid(value) == false) {
                        ogs_error("Invalid IMEISV [%s]", pei);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                                recvmsg, "Invalid PEI", pei, NULL));
                        ogs_free(type);
                        ogs_free(value);
                        return false;
                    }

                    rv = ogs_dbi_update_imeisv(supi, value);
                    if (rv != OGS_OK) {
                        ogs_error("Cannot update IMEISV for SUPI [%s]", supi);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                                recvmsg, "Cannot update IMEISV",
                                supi, NULL));
                        ogs_free(type);
                        ogs_free(value);
                        return false;
                    }
                } else {
                    ogs_error("Unsupported PEI Type [%s]", type);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                            recvmsg, "Unsupported PEI Type", type, NULL));
                    ogs_free(type);
                    ogs_free(value);
                    return false;
                }

                ogs_free(type);
                ogs_free(value);
            }

            memset(&sendmsg, 0, sizeof(sendmsg));

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));

            return true;

        CASE(OGS_SBI_HTTP_METHOD_PATCH)
            OpenAPI_list_t *PatchItemList;

            PatchItemList = recvmsg->PatchItemList;
            if (!PatchItemList) {
                ogs_error("[%s] No PatchItemList", supi);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(
                        stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        recvmsg, "No PatchItemList", supi, NULL));
                return false;
            }

            /* TODO: parse PatchItemList */

            memset(&sendmsg, 0, sizeof(sendmsg));

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));

            return true;

        DEFAULT
            ogs_error("Invalid HTTP method [%s]", recvmsg->h.method);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED,
                    recvmsg, "Invalid HTTP method", recvmsg->h.method,
                    NULL));
        END
        break;
    CASE(OGS_SBI_RESOURCE_NAME_SMF_REGISTRATIONS)
        SWITCH(recvmsg->h.method)
        CASE(OGS_SBI_HTTP_METHOD_PUT)
            OpenAPI_smf_registration_t *SmfRegistration;

            SmfRegistration = recvmsg->SmfRegistration;
            if (!SmfRegistration) {
                ogs_error("[%s] No SmfRegistration", supi);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(
                        stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        recvmsg, "No SmfRegistration", supi, NULL));
                return false;
            }

            memset(&sendmsg, 0, sizeof(sendmsg));

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));

            return true;

        CASE(OGS_SBI_HTTP_METHOD_DELETE)
            memset(&sendmsg, 0, sizeof(sendmsg));

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));

            return true;

        DEFAULT
            ogs_error("Invalid HTTP method [%s]", recvmsg->h.method);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED,
                    recvmsg, "Invalid HTTP method", recvmsg->h.method,
                    NULL));
        END
        break;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED,
                recvmsg, "Unknown resource name",
                recvmsg->h.resource.component[3], NULL));
    END

    return false;
}

/*
 * Build the SessionManagementSubscriptionDataList (i.e. the sm-data
 * resource body) for a subscriber, optionally filtered by S-NSSAI and/or
 * DNN. Pass filter_by_nssai=false and filter_dnn=NULL for the complete,
 * unfiltered resource (used both by the plain GET and by change
 * notifications).
 */
static OpenAPI_list_t *udr_build_sm_subs_data_list(
        ogs_subscription_data_t *subscription_data,
        bool filter_by_nssai, const ogs_s_nssai_t *filter_nssai,
        const char *filter_dnn)
{
    int i, j;
    ogs_slice_data_t *slice_data = NULL;

    OpenAPI_list_t *SessionManagementSubscriptionDataList = NULL;
    OpenAPI_session_management_subscription_data_t
        *SessionManagementSubscriptionData = NULL;
    OpenAPI_snssai_t *singleNSSAI = NULL;
    OpenAPI_list_t *dnnConfigurationList = NULL;
    OpenAPI_map_t *dnnConfigurationMap = NULL;
    OpenAPI_dnn_configuration_t *dnnConfiguration = NULL;
    OpenAPI_pdu_session_types_t *pduSessionTypeList = NULL;
    OpenAPI_ssc_modes_t *sscModeList = NULL;
    OpenAPI_subscribed_default_qos_t *_5gQoSProfile = NULL;
    OpenAPI_ambr_t *sessionAmbr = NULL;
    OpenAPI_list_t *staticIpAddress = NULL;
    OpenAPI_ip_address_t *ipAddress = NULL;
    OpenAPI_list_t *FrameRouteList = NULL;

    ogs_assert(subscription_data);

    SessionManagementSubscriptionDataList = OpenAPI_list_create();
    ogs_assert(SessionManagementSubscriptionDataList);

    ogs_assert(subscription_data->num_of_slice);

    for (j = 0; j < subscription_data->num_of_slice; j++) {
        slice_data = &subscription_data->slice[j];

        if (!slice_data) {
            continue;
        }

        if (filter_by_nssai) {
            if (slice_data->s_nssai.sst != filter_nssai->sst ||
                    slice_data->s_nssai.sd.v != filter_nssai->sd.v) {
                continue;
            }
        }

        singleNSSAI = ogs_calloc(1, sizeof(*singleNSSAI));
        ogs_assert(singleNSSAI);

        dnnConfigurationList = ogs_calloc(1, sizeof(*dnnConfigurationList));
        ogs_assert(dnnConfigurationList);

        singleNSSAI->sst = slice_data->s_nssai.sst;
        singleNSSAI->sd = ogs_s_nssai_sd_to_string(slice_data->s_nssai.sd);

        for (i = 0; i < slice_data->num_of_session; i++) {
            ogs_session_t *session = NULL;

            if (i >= OGS_MAX_NUM_OF_SESS) {
                ogs_warn("Ignore max session count overflow [%d>=%d]",
                    slice_data->num_of_session, OGS_MAX_NUM_OF_SESS);
                break;
            }

            session = &slice_data->session[i];
            ogs_assert(session);
            ogs_assert(session->name);

            if (filter_dnn &&
                ogs_strcasecmp(filter_dnn, session->name) != 0)
                continue;

            if (!session->qos.index) {
                ogs_error("No 5QI");
                continue;
            }
            if (!session->qos.arp.priority_level) {
                ogs_error("No Priority Level");
                continue;
            }

            if (!session->ambr.uplink && !session->ambr.downlink) {
                ogs_error("No Session-AMBR");
                continue;
            }

            dnnConfiguration = ogs_calloc(1, sizeof(*dnnConfiguration));
            ogs_assert(dnnConfiguration);

            pduSessionTypeList = ogs_calloc(1, sizeof(*pduSessionTypeList));
            ogs_assert(pduSessionTypeList);
            pduSessionTypeList->default_session_type =
                    session->session_type;

            pduSessionTypeList->allowed_session_types =
                    OpenAPI_list_create();
            ogs_assert(pduSessionTypeList->allowed_session_types);
            switch (pduSessionTypeList->default_session_type) {
            case OpenAPI_pdu_session_type_IPV4:
                OpenAPI_list_add(pduSessionTypeList->allowed_session_types,
                        (void *)OpenAPI_pdu_session_type_IPV4);
                break;
            case OpenAPI_pdu_session_type_IPV6:
                OpenAPI_list_add(pduSessionTypeList->allowed_session_types,
                        (void *)OpenAPI_pdu_session_type_IPV6);
                break;
            case OpenAPI_pdu_session_type_IPV4V6:
                OpenAPI_list_add(pduSessionTypeList->allowed_session_types,
                        (void *)OpenAPI_pdu_session_type_IPV4);
                OpenAPI_list_add(pduSessionTypeList->allowed_session_types,
                        (void *)OpenAPI_pdu_session_type_IPV6);
                OpenAPI_list_add(pduSessionTypeList->allowed_session_types,
                        (void *)OpenAPI_pdu_session_type_IPV4V6);
                break;
            default:
                ogs_fatal("Unsupported PDN_TYPE[%d]",
                        pduSessionTypeList->default_session_type);
                ogs_assert_if_reached();
            }

            dnnConfiguration->pdu_session_types = pduSessionTypeList;

            sscModeList = ogs_calloc(1, sizeof(*sscModeList));
            ogs_assert(sscModeList);
            sscModeList->default_ssc_mode = OpenAPI_ssc_mode_SSC_MODE_1;

            sscModeList->allowed_ssc_modes = OpenAPI_list_create();
            ogs_assert(sscModeList->allowed_ssc_modes);
            OpenAPI_list_add(sscModeList->allowed_ssc_modes,
                    (void *)OpenAPI_ssc_mode_SSC_MODE_1);
            OpenAPI_list_add(sscModeList->allowed_ssc_modes,
                    (void *)OpenAPI_ssc_mode_SSC_MODE_2);
            OpenAPI_list_add(sscModeList->allowed_ssc_modes,
                    (void *)OpenAPI_ssc_mode_SSC_MODE_3);

            dnnConfiguration->ssc_modes = sscModeList;

            _5gQoSProfile = ogs_calloc(1, sizeof(*_5gQoSProfile));
            ogs_assert(_5gQoSProfile);
            _5gQoSProfile->_5qi = session->qos.index;
            _5gQoSProfile->is_priority_level = true;
            _5gQoSProfile->priority_level = session->qos.arp.priority_level;
            _5gQoSProfile->arp = ogs_calloc(1, sizeof(OpenAPI_arp_t));
            ogs_assert(_5gQoSProfile->arp);
            _5gQoSProfile->arp->priority_level =
                session->qos.arp.priority_level;
            if (session->qos.arp.pre_emption_capability ==
                    OGS_5GC_PRE_EMPTION_ENABLED)
                _5gQoSProfile->arp->preempt_cap =
                        OpenAPI_preemption_capability_MAY_PREEMPT;
            else if (session->qos.arp.pre_emption_capability ==
                    OGS_5GC_PRE_EMPTION_DISABLED)
                _5gQoSProfile->arp->preempt_cap =
                        OpenAPI_preemption_capability_NOT_PREEMPT;
            ogs_assert(_5gQoSProfile->arp->preempt_cap);

            if (session->qos.arp.pre_emption_vulnerability ==
                OGS_5GC_PRE_EMPTION_ENABLED)
                _5gQoSProfile->arp->preempt_vuln =
                    OpenAPI_preemption_vulnerability_PREEMPTABLE;
            else if (session->qos.arp.pre_emption_vulnerability ==
                OGS_5GC_PRE_EMPTION_DISABLED)
                _5gQoSProfile->arp->preempt_vuln =
                    OpenAPI_preemption_vulnerability_NOT_PREEMPTABLE;
            ogs_assert(_5gQoSProfile->arp->preempt_vuln);

            dnnConfiguration->_5g_qos_profile = _5gQoSProfile;

            ogs_assert(session->ambr.uplink || session->ambr.downlink);
            sessionAmbr = ogs_calloc(1, sizeof(*sessionAmbr));
            ogs_assert(sessionAmbr);
            sessionAmbr->uplink = ogs_sbi_bitrate_to_string(
                    session->ambr.uplink, OGS_SBI_BITRATE_KBPS);
            sessionAmbr->downlink = ogs_sbi_bitrate_to_string(
                    session->ambr.downlink, OGS_SBI_BITRATE_KBPS);

            dnnConfiguration->session_ambr = sessionAmbr;

            staticIpAddress = OpenAPI_list_create();
            ogs_assert(staticIpAddress);

            if (session->ue_ip.ipv4 || session->ue_ip.ipv6) {
                ipAddress = ogs_calloc(1, sizeof(*ipAddress));
                ogs_assert(ipAddress);

                if (session->ue_ip.ipv4) {
                    ipAddress->ipv4_addr =
                        ogs_ipv4_to_string(session->ue_ip.addr);
                    ogs_assert(ipAddress->ipv4_addr);
                }
                if (session->ue_ip.ipv6) {
                    ipAddress->ipv6_addr =
                        ogs_ipv6addr_to_string(session->ue_ip.addr6);
                    ogs_assert(ipAddress->ipv6_addr);
                }

                if (ipAddress->ipv4_addr || ipAddress->ipv6_addr)
                    OpenAPI_list_add(staticIpAddress, ipAddress);
                else
                    ogs_free(ipAddress);
            }

            if (staticIpAddress->count)
                dnnConfiguration->static_ip_address = staticIpAddress;
            else
                OpenAPI_list_free(staticIpAddress);

            dnnConfigurationMap = OpenAPI_map_create(
                    session->name, dnnConfiguration);
            ogs_assert(dnnConfigurationMap);
            OpenAPI_list_add(dnnConfigurationList, dnnConfigurationMap);

            if (session->ipv4_framed_routes) {
                int i;
                FrameRouteList = OpenAPI_list_create();

                for (i = 0; i < OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI; i++) {
                    const char *route = session->ipv4_framed_routes[i];
                    if (!route) break;
                    OpenAPI_list_add(FrameRouteList,
                                    OpenAPI_frame_route_info_create(
                                            ogs_strdup(route), NULL));
                }
                dnnConfiguration->ipv4_frame_route_list = FrameRouteList;
            }

            if (session->ipv6_framed_routes) {
                int i;
                FrameRouteList = OpenAPI_list_create();

                for (i = 0; i < OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI; i++) {
                    const char *route = session->ipv6_framed_routes[i];
                    if (!route) break;
                    OpenAPI_list_add(FrameRouteList,
                                    OpenAPI_frame_route_info_create(
                                            NULL, ogs_strdup(route)));
                }
                dnnConfiguration->ipv6_frame_route_list = FrameRouteList;
            }
        }

        if (dnnConfigurationList->count) {
            SessionManagementSubscriptionData = ogs_calloc(1,
                    sizeof(*SessionManagementSubscriptionData));
            ogs_assert(SessionManagementSubscriptionData);
            SessionManagementSubscriptionData->single_nssai = singleNSSAI;
            SessionManagementSubscriptionData->dnn_configurations =
                dnnConfigurationList;
            OpenAPI_list_add(SessionManagementSubscriptionDataList,
                SessionManagementSubscriptionData);
        } else {
            if (singleNSSAI->sd)
                ogs_free(singleNSSAI->sd);
            ogs_free(singleNSSAI);
            OpenAPI_list_free(dnnConfigurationList);
        }
    }

    return SessionManagementSubscriptionDataList;
}

/* Frees a list built by udr_build_sm_subs_data_list(). */
static void udr_free_sm_subs_data_list(OpenAPI_list_t *list)
{
    OpenAPI_lnode_t *node = NULL, *node2 = NULL, *node3 = NULL;

    if (!list)
        return;

    OpenAPI_list_for_each(list, node) {
        OpenAPI_session_management_subscription_data_t
            *SessionManagementSubscriptionData = node->data;
        OpenAPI_snssai_t *singleNSSAI = NULL;
        OpenAPI_list_t *dnnConfigurationList = NULL;

        if (SessionManagementSubscriptionData->single_nssai) {
            singleNSSAI = SessionManagementSubscriptionData->single_nssai;
            if (singleNSSAI->sd)
                ogs_free(singleNSSAI->sd);
            ogs_free(singleNSSAI);
        }

        if (SessionManagementSubscriptionData->dnn_configurations) {
            dnnConfigurationList =
                    SessionManagementSubscriptionData->dnn_configurations;
            OpenAPI_list_for_each(dnnConfigurationList, node2) {
                OpenAPI_map_t *dnnConfigurationMap = node2->data;
                if (dnnConfigurationMap) {
                    OpenAPI_dnn_configuration_t *dnnConfiguration =
                        dnnConfigurationMap->value;
                    if (dnnConfiguration) {
                        OpenAPI_pdu_session_types_t *pduSessionTypeList =
                                dnnConfiguration->pdu_session_types;
                        OpenAPI_ssc_modes_t *sscModeList =
                                dnnConfiguration->ssc_modes;
                        OpenAPI_subscribed_default_qos_t *_5gQoSProfile =
                                dnnConfiguration->_5g_qos_profile;
                        OpenAPI_ambr_t *sessionAmbr =
                                dnnConfiguration->session_ambr;
                        OpenAPI_list_t *staticIpAddress =
                                dnnConfiguration->static_ip_address;
                        OpenAPI_list_t *FrameRouteList = NULL;

                        if (pduSessionTypeList) {
                            if (pduSessionTypeList->allowed_session_types)
                                OpenAPI_list_free(
                                        pduSessionTypeList->allowed_session_types);
                            ogs_free(pduSessionTypeList);
                        }
                        if (sscModeList) {
                            if (sscModeList->allowed_ssc_modes)
                                OpenAPI_list_free(
                                        sscModeList->allowed_ssc_modes);
                            ogs_free(sscModeList);
                        }
                        if (_5gQoSProfile) {
                            if (_5gQoSProfile->arp)
                                ogs_free(_5gQoSProfile->arp);
                            ogs_free(_5gQoSProfile);
                        }
                        if (sessionAmbr) {
                            if (sessionAmbr->uplink)
                                ogs_free(sessionAmbr->uplink);
                            if (sessionAmbr->downlink)
                                ogs_free(sessionAmbr->downlink);
                            ogs_free(sessionAmbr);
                        }
                        if (staticIpAddress) {
                            OpenAPI_list_for_each(staticIpAddress, node3) {
                                OpenAPI_ip_address_t *ipAddress = node3->data;
                                if (ipAddress) {
                                    if (ipAddress->ipv4_addr)
                                        ogs_free(ipAddress->ipv4_addr);
                                    if (ipAddress->ipv6_addr)
                                        ogs_free(ipAddress->ipv6_addr);
                                    ogs_free(ipAddress);
                                }
                            }
                            OpenAPI_list_free(staticIpAddress);
                        }

                        FrameRouteList = dnnConfiguration->ipv4_frame_route_list;
                        OpenAPI_list_for_each(FrameRouteList, node3) {
                            OpenAPI_frame_route_info_t *frame = node3->data;
                            if (frame)
                                ogs_free(frame);
                        }
                        OpenAPI_list_free(FrameRouteList);

                        FrameRouteList = dnnConfiguration->ipv6_frame_route_list;
                        OpenAPI_list_for_each(FrameRouteList, node3) {
                            OpenAPI_frame_route_info_t *frame = node3->data;
                            if (frame)
                                ogs_free(frame);
                        }
                        OpenAPI_list_free(FrameRouteList);

                        ogs_free(dnnConfiguration);
                    }
                    ogs_free(dnnConfigurationMap);
                }
            }
            OpenAPI_list_free(dnnConfigurationList);
        }
        ogs_free(SessionManagementSubscriptionData);
    }
    OpenAPI_list_free(list);
}

bool udr_nudr_dr_handle_subscription_provisioned(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv, status = 0;
    char *strerror = NULL;

    bool processAmData = false;
    bool processSmfSel = false;
    bool processSmData = false;
    bool returnProvisionedData = false;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;
    ogs_subscription_data_t subscription_data;
    ogs_slice_data_t *slice_data = NULL;

    OpenAPI_access_and_mobility_subscription_data_t
        AccessAndMobilitySubscriptionData;
    OpenAPI_list_t *SessionManagementSubscriptionDataList = NULL;
    OpenAPI_smf_selection_subscription_data_t SmfSelectionSubscriptionData;

    OpenAPI_list_t *GpsiList = NULL;
    OpenAPI_ambr_rm_t SubscribedUeAmbr;
    OpenAPI_list_t *DefaultSingleNssaiList = NULL;
    OpenAPI_list_t *SingleNssaiList = NULL;
    OpenAPI_nssai_t NSSAI;
    OpenAPI_snssai_t *Snssai = NULL;

    OpenAPI_list_t *SubscribedSnssaiInfoList = NULL;
    OpenAPI_map_t *SubscribedSnssaiInfoMap = NULL;
    OpenAPI_snssai_info_t *SubscribedSnssaiInfo = NULL;
    OpenAPI_list_t *DnnInfoList = NULL;
    OpenAPI_dnn_info_t *DnnInfo = NULL;

    OpenAPI_sm_subs_data_t smSubsData;

    char *supi = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    memset(&AccessAndMobilitySubscriptionData, 0,
            sizeof(AccessAndMobilitySubscriptionData));
    memset(&SubscribedUeAmbr, 0, sizeof(SubscribedUeAmbr));
    memset(&NSSAI, 0, sizeof(NSSAI));
    memset(&subscription_data, 0, sizeof(ogs_subscription_data_t));
    memset(&SmfSelectionSubscriptionData, 0,
            sizeof(SmfSelectionSubscriptionData));

    supi = recvmsg->h.resource.component[1];
    if (!supi) {
        ogs_error("No SUPI");
        strerror = ogs_msprintf("No SUPI");
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        goto cleanup;
    }

    {
        char *type = NULL, *value = NULL;
        if (ogs_id_get_type_value(supi, &type, &value) == false) {
            ogs_error("Invalid SUPI [%s]", supi);
            strerror = ogs_msprintf("[%s] Invalid SUPI", supi);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        }

        if (strcmp(type, OGS_ID_SUPI_TYPE_IMSI) != 0) {
            ogs_error("Unknown SUPI Type [%s]", supi);
            ogs_free(type);
            ogs_free(value);
            strerror = ogs_msprintf("[%s] Unknown SUPI Type", supi);
            status = OGS_SBI_HTTP_STATUS_FORBIDDEN;
            goto cleanup;
        }

        if (ogs_imsi_bcd_is_valid(value) == false) {
            ogs_error("Invalid IMSI SUPI [%s]", supi);
            ogs_free(type);
            ogs_free(value);
            strerror = ogs_msprintf("[%s] Invalid SUPI", supi);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        }

        ogs_free(type);
        ogs_free(value);
    }

    rv = ogs_dbi_subscription_data(supi, &subscription_data);
    if (rv != OGS_OK) {
        strerror = ogs_msprintf("[%s] Cannot find SUPI in DB", supi);
        status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
        goto cleanup;
    }

    if (recvmsg->h.resource.component[4]) {
        SWITCH(recvmsg->h.resource.component[4])
        CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
            processAmData = true;
            break;
        CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECTION_SUBSCRIPTION_DATA)
            processSmfSel = true;
            break;
        CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
            processSmData = true;
            break;
        DEFAULT
            strerror = ogs_msprintf("Invalid resource name [%s]",
                    recvmsg->h.resource.component[4]);
            status = OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED;
            goto cleanup;
        END
    } else {
        returnProvisionedData = true;
        if (recvmsg->param.num_of_dataset_names) {
            int i, validParams = false;
            for (i = 0; i < recvmsg->param.num_of_dataset_names; i++) {
                SWITCH(recvmsg->param.dataset_names[i])
                CASE(OGS_SBI_PARAM_DATASET_NAME_AM)
                    processAmData = true;
                    validParams = true;
                    break;
                CASE(OGS_SBI_PARAM_DATASET_NAME_SMF_SEL)
                    processSmfSel = true;
                    validParams = true;
                    break;
                CASE(OGS_SBI_PARAM_DATASET_NAME_SM)
                    processSmData = true;
                    validParams = true;
                    break;
                DEFAULT
                    ogs_error("Unexpected dataset-name! [%s]",
                            recvmsg->param.dataset_names[i]);
                END
            }
            if (!validParams) {
                strerror = ogs_msprintf("No valid dataset-names");
                status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
                goto cleanup;
            }
        } else {
            processAmData = true;
            processSmfSel = true;
            processSmData = true;
        }
    }

    if (processAmData) {
        int i;
        bool processGpsi = false;
        bool processUeAmbr = false;
        bool processNssai = false;

        /* Apply filtering based on fields query parameter */
        if (recvmsg->param.num_of_fields) {
            for (i = 0; i < recvmsg->param.num_of_fields; i++) {
                SWITCH(recvmsg->param.fields[i])
                CASE(OGS_SBI_PARAM_FIELDS_GPSIS)
                    processGpsi = true;
                    break;
                CASE(OGS_SBI_PARAM_FIELDS_SUBSCRIBED_UE_AMBR)
                    processUeAmbr = true;
                    break;
                CASE(OGS_SBI_PARAM_FIELDS_NSSAI)
                    processNssai = true;
                    break;
                DEFAULT
                    ogs_error("Unexpected field! [%s]",
                            recvmsg->param.fields[i]);
                END
            }
        } else {
            processGpsi = true;
            processUeAmbr = true;
            processNssai = true;
        }

        /*
         * UE-AMBR is only meaningful for Access and Mobility
         * subscription data. Checking it before the dataset dispatch
         * rejected SMF Selection and Session Management queries of
         * subscribers without UE-AMBR with 404 as well.
         */
        if (processUeAmbr &&
            !subscription_data.ambr.uplink &&
            !subscription_data.ambr.downlink) {
            strerror = ogs_msprintf("[%s] No UE-AMBR", supi);
            status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
            goto cleanup;
        }

        if (processGpsi) {
            GpsiList = OpenAPI_list_create();
            for (i = 0; i < subscription_data.num_of_msisdn; i++) {
                char *gpsi = ogs_msprintf("%s-%s",
                        OGS_ID_GPSI_TYPE_MSISDN,
                        subscription_data.msisdn[i].bcd);
                ogs_assert(gpsi);
                OpenAPI_list_add(GpsiList, gpsi);
            }

            if (GpsiList->count)
                AccessAndMobilitySubscriptionData.gpsis = GpsiList;
        }

        if (processUeAmbr) {
            SubscribedUeAmbr.uplink = ogs_sbi_bitrate_to_string(
                    subscription_data.ambr.uplink, OGS_SBI_BITRATE_KBPS);
            SubscribedUeAmbr.downlink = ogs_sbi_bitrate_to_string(
                    subscription_data.ambr.downlink, OGS_SBI_BITRATE_KBPS);

            AccessAndMobilitySubscriptionData.subscribed_ue_ambr =
                &SubscribedUeAmbr;
        }

        if (processNssai) {
            DefaultSingleNssaiList = OpenAPI_list_create();
            for (i = 0; i < subscription_data.num_of_slice; i++) {
                slice_data = &subscription_data.slice[i];

                if (slice_data->default_indicator == false)
                    continue;

                Snssai = ogs_calloc(1, sizeof(*Snssai));
                ogs_assert(Snssai);

                Snssai->sst = slice_data->s_nssai.sst;
                Snssai->sd = ogs_s_nssai_sd_to_string(slice_data->s_nssai.sd);

                OpenAPI_list_add(DefaultSingleNssaiList, Snssai);
            }
            if (DefaultSingleNssaiList->count) {
                NSSAI.default_single_nssais = DefaultSingleNssaiList;
            }

            SingleNssaiList = OpenAPI_list_create();
            for (i = 0; i < subscription_data.num_of_slice; i++) {
                slice_data = &subscription_data.slice[i];

                if (slice_data->default_indicator == true)
                    continue;

                Snssai = ogs_calloc(1, sizeof(*Snssai));
                ogs_assert(Snssai);

                Snssai->sst = slice_data->s_nssai.sst;
                Snssai->sd = ogs_s_nssai_sd_to_string(slice_data->s_nssai.sd);

                OpenAPI_list_add(SingleNssaiList, Snssai);
            }

            if (DefaultSingleNssaiList->count) {
                if (SingleNssaiList->count) {
                    NSSAI.single_nssais = SingleNssaiList;
                }
            } else {
                if (SingleNssaiList->count) {
                    ogs_fatal("No Default S-NSSAI");
                    ogs_assert_if_reached();
                }
            }

            if (DefaultSingleNssaiList->count)
                AccessAndMobilitySubscriptionData.nssai = &NSSAI;
        }

        if (!returnProvisionedData) {
            memset(&sendmsg, 0, sizeof(sendmsg));
            sendmsg.AccessAndMobilitySubscriptionData =
                &AccessAndMobilitySubscriptionData;

            response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        }
    }
    if (processSmfSel) {
        int i, j;

        SubscribedSnssaiInfoList = OpenAPI_list_create();
        ogs_assert(SubscribedSnssaiInfoList);

        for (i = 0; i < subscription_data.num_of_slice; i++) {
            if (i >= OGS_MAX_NUM_OF_SLICE) {
                ogs_warn("Ignore max slice count overflow [%d>=%d]",
                    subscription_data.num_of_slice, OGS_MAX_NUM_OF_SLICE);
                break;
            }
            slice_data = &subscription_data.slice[i];

            DnnInfoList = OpenAPI_list_create();
            ogs_assert(DnnInfoList);

            for (j = 0; j < slice_data->num_of_session; j++) {
                ogs_session_t *session = NULL;

                if (j >= OGS_MAX_NUM_OF_SESS) {
                    ogs_warn("Ignore max session count overflow [%d>=%d]",
                        slice_data->num_of_session, OGS_MAX_NUM_OF_SESS);
                    break;
                }

                session = &slice_data->session[j];
                ogs_assert(session);
                ogs_assert(session->name);

                DnnInfo = ogs_calloc(1, sizeof(*DnnInfo));
                ogs_assert(DnnInfo);

                DnnInfo->dnn = session->name;

                /* 0-index DNN becomes the default DNN */
                if (j == 0) {
                    DnnInfo->is_default_dnn_indicator = true;
                    DnnInfo->default_dnn_indicator = true;
                }

                if (session->lbo_roaming_allowed == true) {
                    DnnInfo->is_lbo_roaming_allowed = true;
                    DnnInfo->lbo_roaming_allowed = true;
                }

                OpenAPI_list_add(DnnInfoList, DnnInfo);
            }

            SubscribedSnssaiInfo = ogs_calloc(1, sizeof(*SubscribedSnssaiInfo));
            ogs_assert(SubscribedSnssaiInfo);

            if (DnnInfoList->count)
                SubscribedSnssaiInfo->dnn_infos = DnnInfoList;
            else
                OpenAPI_list_free(DnnInfoList);

            SubscribedSnssaiInfoMap = OpenAPI_map_create(
                    ogs_sbi_s_nssai_to_string(&slice_data->s_nssai),
                    SubscribedSnssaiInfo);
            ogs_assert(SubscribedSnssaiInfoMap);
            ogs_assert(SubscribedSnssaiInfoMap->key);

            OpenAPI_list_add(SubscribedSnssaiInfoList, SubscribedSnssaiInfoMap);
        }

        if (SubscribedSnssaiInfoList->count)
            SmfSelectionSubscriptionData.subscribed_snssai_infos =
                SubscribedSnssaiInfoList;
        else
            OpenAPI_list_free(SubscribedSnssaiInfoList);

        if (!returnProvisionedData) {
            memset(&sendmsg, 0, sizeof(sendmsg));
            sendmsg.SmfSelectionSubscriptionData =
                    &SmfSelectionSubscriptionData;

            response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        }
    }
    if (processSmData) {
        if (recvmsg->param.single_nssai_presence && !returnProvisionedData) {
            slice_data = ogs_slice_find_by_s_nssai(
                    subscription_data.slice, subscription_data.num_of_slice,
                    &recvmsg->param.s_nssai);

            if (!slice_data) {
                strerror = ogs_msprintf("[%s] Cannot find S_NSSAI"
                        "[SST:%d SD:0x%x]",
                        supi,
                        recvmsg->param.s_nssai.sst,
                        recvmsg->param.s_nssai.sd.v);
                status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
                goto cleanup;
            }
        }

        /* According to 29.503 6.1.3.8.3.1
        * sm-data can be filtered by singleNssai or dnn query params.
        * If there is no filtering, then all NSSAIs and all DNNs are returned.
        */

        SessionManagementSubscriptionDataList = udr_build_sm_subs_data_list(
                &subscription_data,
                recvmsg->param.single_nssai_presence && !returnProvisionedData,
                &recvmsg->param.s_nssai,
                (recvmsg->param.dnn && !returnProvisionedData) ?
                    recvmsg->param.dnn : NULL);

        memset(&smSubsData, 0, sizeof(smSubsData));
        smSubsData.session_management_subscription_data_list =
            SessionManagementSubscriptionDataList;

        if (!returnProvisionedData) {
            if (!SessionManagementSubscriptionDataList->count) {
                OpenAPI_list_free(SessionManagementSubscriptionDataList);

                strerror = ogs_msprintf("[%s] Cannot find S_NSSAI with DNN"
                        "[SST:%d SD:0x%x, DNN:%s]",
                        supi,
                        recvmsg->param.s_nssai.sst,
                        recvmsg->param.s_nssai.sd.v,
                        recvmsg->param.dnn);
                status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
                goto cleanup;
            }
            memset(&sendmsg, 0, sizeof(sendmsg));

            sendmsg.SessionManagementSubscriptionDataList =
                    SessionManagementSubscriptionDataList;
            ogs_assert(sendmsg.SessionManagementSubscriptionDataList);

            response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        }
    }

    /* Build Provisioned Data Sets */
    if (returnProvisionedData) {
        OpenAPI_provisioned_data_sets_t ProvisionedDataSets;

        memset(&ProvisionedDataSets, 0,
                sizeof(ProvisionedDataSets));

        if (processAmData) {
            ProvisionedDataSets.am_data = &AccessAndMobilitySubscriptionData;
        }
        if (processSmfSel) {
            ProvisionedDataSets.smf_sel_data = &SmfSelectionSubscriptionData;
        }
        if (processSmData) {
            ProvisionedDataSets.sm_data = &smSubsData;
        }
        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.ProvisionedDataSets = &ProvisionedDataSets;

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
    }

    /* Free resources used above */
    if (processAmData) {
        OpenAPI_lnode_t *node = NULL;

        OpenAPI_list_for_each(GpsiList, node) {
            if (node->data) ogs_free(node->data);
        }
        OpenAPI_list_free(GpsiList);

        if (SubscribedUeAmbr.uplink)
            ogs_free(SubscribedUeAmbr.uplink);
        if (SubscribedUeAmbr.downlink)
            ogs_free(SubscribedUeAmbr.downlink);

        OpenAPI_list_for_each(DefaultSingleNssaiList, node) {
            OpenAPI_snssai_t *Snssai = node->data;
            if (Snssai) {
                if (Snssai->sd)
                    ogs_free(Snssai->sd);
                ogs_free(Snssai);
            }
        }
        OpenAPI_list_free(DefaultSingleNssaiList);
        OpenAPI_list_for_each(SingleNssaiList, node) {
            OpenAPI_snssai_t *Snssai = node->data;
            if (Snssai) {
                if (Snssai->sd)
                    ogs_free(Snssai->sd);
                ogs_free(Snssai);
            }
        }
        OpenAPI_list_free(SingleNssaiList);
    }
    if (processSmfSel) {
        OpenAPI_lnode_t *node = NULL, *node2 = NULL;

        SubscribedSnssaiInfoList =
            SmfSelectionSubscriptionData.subscribed_snssai_infos;
        OpenAPI_list_for_each(SubscribedSnssaiInfoList, node) {
            SubscribedSnssaiInfoMap = node->data;
            if (SubscribedSnssaiInfoMap) {
                SubscribedSnssaiInfo = SubscribedSnssaiInfoMap->value;
                if (SubscribedSnssaiInfo) {
                    DnnInfoList = SubscribedSnssaiInfo->dnn_infos;
                    if (DnnInfoList) {
                        OpenAPI_list_for_each(DnnInfoList, node2) {
                            DnnInfo = node2->data;
                            if (DnnInfo) {
                                ogs_free(DnnInfo);
                            }
                        }
                        OpenAPI_list_free(DnnInfoList);
                    }
                    ogs_free(SubscribedSnssaiInfo);
                }
                if (SubscribedSnssaiInfoMap->key)
                    ogs_free(SubscribedSnssaiInfoMap->key);
                ogs_free(SubscribedSnssaiInfoMap);
            }
        }
        OpenAPI_list_free(SubscribedSnssaiInfoList);
    }
    if (processSmData)
        udr_free_sm_subs_data_list(SessionManagementSubscriptionDataList);

    ogs_subscription_data_free(&subscription_data);

    return true;

cleanup:
    ogs_assert(strerror);
    ogs_assert(status);
    ogs_error("%s", strerror);
    ogs_assert(true ==
        ogs_sbi_server_send_error(stream, status, recvmsg, strerror, NULL,
                NULL));
    ogs_free(strerror);

    ogs_subscription_data_free(&subscription_data);

    return false;
}

bool udr_nudr_dr_handle_policy_data(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv, i, status = 0;
    char *strerror = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_subscription_data_t subscription_data;
    ogs_slice_data_t *slice_data = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    memset(&subscription_data, 0, sizeof(ogs_subscription_data_t));

    SWITCH(recvmsg->h.resource.component[1])
    CASE(OGS_SBI_RESOURCE_NAME_UES)
        char *supi = recvmsg->h.resource.component[2];

        if (!supi) {
            strerror = ogs_msprintf("No SUPI");
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        }

        if (strncmp(supi,
                OGS_ID_SUPI_TYPE_IMSI, strlen(OGS_ID_SUPI_TYPE_IMSI)) != 0) {
            strerror = ogs_msprintf("[%s] Unknown SUPI Type", supi);
            status = OGS_SBI_HTTP_STATUS_FORBIDDEN;
            goto cleanup;
        }

        SWITCH(recvmsg->h.method)
        CASE(OGS_SBI_HTTP_METHOD_GET)
            OpenAPI_lnode_t *node = NULL, *node2 = NULL;

            rv = ogs_dbi_subscription_data(supi, &subscription_data);
            if (rv != OGS_OK) {
                strerror = ogs_msprintf("[%s] Cannot find SUPI in DB", supi);
                status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
                goto cleanup;
            }

            SWITCH(recvmsg->h.resource.component[3])
            CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
                OpenAPI_am_policy_data_t AmPolicyData;

                memset(&AmPolicyData, 0, sizeof(AmPolicyData));

                memset(&sendmsg, 0, sizeof(sendmsg));
                sendmsg.AmPolicyData = &AmPolicyData;

                response = ogs_sbi_build_response(
                        &sendmsg, OGS_SBI_HTTP_STATUS_OK);
                ogs_assert(response);
                ogs_assert(true ==
                        ogs_sbi_server_send_response(stream, response));

                break;

            CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
                OpenAPI_sm_policy_data_t SmPolicyData;

                OpenAPI_list_t *SmPolicySnssaiDataList = NULL;
                OpenAPI_map_t *SmPolicySnssaiDataMap = NULL;
                OpenAPI_sm_policy_snssai_data_t *SmPolicySnssaiData = NULL;

                OpenAPI_snssai_t *sNSSAI = NULL;

                OpenAPI_list_t *SmPolicyDnnDataList = NULL;
                OpenAPI_map_t *SmPolicyDnnDataMap = NULL;
                OpenAPI_sm_policy_dnn_data_t *SmPolicyDnnData = NULL;

                if (!recvmsg->param.snssai_presence) {
                    strerror = ogs_msprintf("[%s] No S_NSSAI", supi);
                    status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
                    goto cleanup;
                }

                ogs_assert(subscription_data.num_of_slice);
                slice_data = ogs_slice_find_by_s_nssai(
                        subscription_data.slice, subscription_data.num_of_slice,
                        &recvmsg->param.s_nssai);

                if (!slice_data) {
                    strerror = ogs_msprintf(
                            "[%s] Cannot find S_NSSAI[SST:%d SD:0x%x]",
                            supi,
                            recvmsg->param.s_nssai.sst,
                            recvmsg->param.s_nssai.sd.v);
                    status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
                    goto cleanup;
                }

                sNSSAI = ogs_calloc(1, sizeof(*sNSSAI));
                ogs_assert(sNSSAI);
                sNSSAI->sst = slice_data->s_nssai.sst;
                sNSSAI->sd = ogs_s_nssai_sd_to_string(slice_data->s_nssai.sd);

                SmPolicyDnnDataList = OpenAPI_list_create();
                ogs_assert(SmPolicyDnnDataList);

                slice_data = &subscription_data.slice[0];

                for (i = 0; i < slice_data->num_of_session; i++) {
                    ogs_session_t *session = NULL;

                    if (i >= OGS_MAX_NUM_OF_SESS) {
                        ogs_warn("Ignore max session count overflow [%d>=%d]",
                            slice_data->num_of_session, OGS_MAX_NUM_OF_SESS);
                        break;
                    }

                    session = &slice_data->session[i];
                    ogs_assert(session);
                    ogs_assert(session->name);

                    if (recvmsg->param.dnn &&
                        ogs_strcasecmp(recvmsg->param.dnn, session->name) != 0)
                        continue;

                    SmPolicyDnnData = ogs_calloc(1, sizeof(*SmPolicyDnnData));
                    ogs_assert(SmPolicyDnnData);

                    SmPolicyDnnData->dnn = session->name;

                    SmPolicyDnnDataMap = OpenAPI_map_create(
                            session->name, SmPolicyDnnData);
                    ogs_assert(SmPolicyDnnDataMap);

                    OpenAPI_list_add(SmPolicyDnnDataList, SmPolicyDnnDataMap);
                }

                SmPolicySnssaiData = ogs_calloc(1, sizeof(*SmPolicySnssaiData));
                ogs_assert(SmPolicySnssaiData);

                SmPolicySnssaiData->snssai = sNSSAI;
                if (SmPolicyDnnDataList->count)
                    SmPolicySnssaiData->sm_policy_dnn_data =
                        SmPolicyDnnDataList;
                else
                    OpenAPI_list_free(SmPolicyDnnDataList);

                SmPolicySnssaiDataMap = OpenAPI_map_create(
                        ogs_sbi_s_nssai_to_string(&recvmsg->param.s_nssai),
                        SmPolicySnssaiData);
                ogs_assert(SmPolicySnssaiDataMap);
                ogs_assert(SmPolicySnssaiDataMap->key);

                SmPolicySnssaiDataList = OpenAPI_list_create();
                ogs_assert(SmPolicySnssaiDataList);

                OpenAPI_list_add(SmPolicySnssaiDataList, SmPolicySnssaiDataMap);

                memset(&SmPolicyData, 0, sizeof(SmPolicyData));

                if (SmPolicySnssaiDataList->count)
                    SmPolicyData.sm_policy_snssai_data = SmPolicySnssaiDataList;
                else
                    OpenAPI_list_free(SmPolicySnssaiDataList);

                memset(&sendmsg, 0, sizeof(sendmsg));
                sendmsg.SmPolicyData = &SmPolicyData;

                response = ogs_sbi_build_response(
                        &sendmsg, OGS_SBI_HTTP_STATUS_OK);
                ogs_assert(response);
                ogs_assert(true ==
                        ogs_sbi_server_send_response(stream, response));

                SmPolicySnssaiDataList = SmPolicyData.sm_policy_snssai_data;
                OpenAPI_list_for_each(SmPolicySnssaiDataList, node) {
                    SmPolicySnssaiDataMap = node->data;
                    if (SmPolicySnssaiDataMap) {
                        SmPolicySnssaiData = SmPolicySnssaiDataMap->value;
                        if (SmPolicySnssaiData) {
                            sNSSAI = SmPolicySnssaiData->snssai;
                            if (sNSSAI) {
                                if (sNSSAI->sd) ogs_free(sNSSAI->sd);
                                ogs_free(sNSSAI);
                            }
                            SmPolicyDnnDataList =
                                SmPolicySnssaiData->sm_policy_dnn_data;
                            if (SmPolicyDnnDataList) {
                                OpenAPI_list_for_each(
                                        SmPolicyDnnDataList, node2) {
                                    SmPolicyDnnDataMap = node2->data;
                                    if (SmPolicyDnnDataMap) {
                                        SmPolicyDnnData =
                                            SmPolicyDnnDataMap->value;
                                        if (SmPolicyDnnData) {
                                            ogs_free(SmPolicyDnnData);
                                        }
                                        ogs_free(SmPolicyDnnDataMap);
                                    }
                                }
                                OpenAPI_list_free(SmPolicyDnnDataList);
                            }
                            ogs_free(SmPolicySnssaiData);
                        }
                        if (SmPolicySnssaiDataMap->key)
                            ogs_free(SmPolicySnssaiDataMap->key);
                        ogs_free(SmPolicySnssaiDataMap);
                    }
                }
                OpenAPI_list_free(SmPolicySnssaiDataList);

                break;

            DEFAULT
                strerror = ogs_msprintf("Invalid resource name [%s]",
                        recvmsg->h.resource.component[3]);
                status = OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED;
                goto cleanup;
            END

            break;

        DEFAULT
            strerror = ogs_msprintf("Invalid HTTP method [%s]",
                    recvmsg->h.method);
            status = OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED;
            goto cleanup;
        END

        break;

    DEFAULT
        strerror = ogs_msprintf("Invalid resource name [%s]",
                recvmsg->h.resource.component[1]);
        status = OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED;
        goto cleanup;
    END

    ogs_subscription_data_free(&subscription_data);

    return true;

cleanup:
    ogs_assert(strerror);
    ogs_assert(status);
    ogs_error("%s", strerror);
    ogs_assert(true ==
        ogs_sbi_server_send_error(stream, status, recvmsg, strerror,
                NULL, NULL));
    ogs_free(strerror);

    ogs_subscription_data_free(&subscription_data);

    return false;
}

/*
 * A monitored resource URI is only supported if it resolves to the
 * provisioned sm-data resource. On success, *ue_id is a newly allocated
 * copy of the URI's ueId component; the caller must free it.
 */
static bool parse_sm_data_monitored_uri(const char *uri, char **ue_id)
{
    ogs_sbi_message_t tmp_message;
    ogs_sbi_header_t header;
    bool ok = false;

    ogs_assert(uri);
    ogs_assert(ue_id);

    memset(&header, 0, sizeof(header));
    header.uri = (char *)uri;

    if (ogs_sbi_parse_header(&tmp_message, &header) != OGS_OK)
        return false;

    if (header.service.name &&
        ogs_sbi_service_name_id_from_string(header.service.name) ==
            OpenAPI_service_name_nudr_dr &&
        header.api.version &&
        !strcmp(header.api.version, OGS_SBI_API_V1) &&
        header.resource.component[0] &&
        !strcmp(header.resource.component[0],
                OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA) &&
        header.resource.component[1] &&                       /* ueId */
        header.resource.component[2] &&                       /* servingPlmnId */
        header.resource.component[3] &&
        !strcmp(header.resource.component[3],
                OGS_SBI_RESOURCE_NAME_PROVISIONED_DATA) &&
        header.resource.component[4] &&
        !strcmp(header.resource.component[4], OGS_SBI_RESOURCE_NAME_SM_DATA) &&
        !header.resource.component[5]) {
        *ue_id = ogs_strdup(header.resource.component[1]);
        ogs_assert(*ue_id);
        ok = true;
    }

    ogs_sbi_header_free(&header);

    return ok;
}

bool udr_nudr_dr_handle_subs_to_notify_create(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int i;
    char *common_ue_id = NULL;
    udr_subscription_t *subscription = NULL;
    OpenAPI_subscription_data_subscriptions_t *SubscriptionDataSubscriptions;
    OpenAPI_list_t *monitored_resource_uris = NULL;
    OpenAPI_lnode_t *node = NULL;
    ogs_uuid_t uuid;
    char id[OGS_UUID_FORMATTED_LENGTH + 1];
    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    SubscriptionDataSubscriptions = recvmsg->SubscriptionDataSubscriptions;
    if (!SubscriptionDataSubscriptions) {
        ogs_error("No SubscriptionDataSubscriptions");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No SubscriptionDataSubscriptions", NULL, NULL));
        return false;
    }

    if (!SubscriptionDataSubscriptions->callback_reference) {
        ogs_error("No callbackReference");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No callbackReference", NULL, NULL));
        return false;
    }

    {
        OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
        char *fqdn = NULL;
        uint16_t fqdn_port = 0;
        ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

        if (ogs_sbi_getaddr_from_uri(&scheme, &fqdn, &fqdn_port,
                &addr, &addr6,
                SubscriptionDataSubscriptions->callback_reference) == false ||
                scheme == OpenAPI_uri_scheme_NULL) {
            ogs_error("Invalid callbackReference [%s]",
                    SubscriptionDataSubscriptions->callback_reference);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, recvmsg,
                    "Invalid callbackReference",
                    SubscriptionDataSubscriptions->callback_reference, NULL));
            return false;
        }

        client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client)
            client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);

        ogs_free(fqdn);
        ogs_freeaddrinfo(addr);
        ogs_freeaddrinfo(addr6);

        if (!client) {
            ogs_error("ogs_sbi_client_add() failed");
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    recvmsg, "No SBI client available", NULL, NULL));
            return false;
        }
    }

    monitored_resource_uris =
        SubscriptionDataSubscriptions->monitored_resource_uris;
    if (!monitored_resource_uris || monitored_resource_uris->count == 0) {
        ogs_error("No monitoredResourceUris");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No monitoredResourceUris", NULL, NULL));
        return false;
    }

    if (monitored_resource_uris->count >
            UDR_MAX_NUM_OF_MONITORED_RESOURCE_URI) {
        ogs_error("Too many monitoredResourceUris");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Too many monitoredResourceUris", NULL, NULL));
        return false;
    }

    OpenAPI_list_for_each(monitored_resource_uris, node) {
        char *uri_ue_id = NULL;
        char *uri = (char *)node->data;

        if (!uri || !parse_sm_data_monitored_uri(uri, &uri_ue_id)) {
            ogs_error("Unsupported monitoredResourceUri [%s]", uri);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, recvmsg,
                    "Unsupported monitoredResourceUri", uri, NULL));
            if (common_ue_id)
                ogs_free(common_ue_id);
            return false;
        }

        if (!common_ue_id) {
            common_ue_id = uri_ue_id;
        } else if (strcmp(common_ue_id, uri_ue_id) != 0) {
            ogs_error("Inconsistent ueId across monitoredResourceUris");
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, recvmsg,
                    "Inconsistent ueId across monitoredResourceUris",
                    NULL, NULL));
            ogs_free(uri_ue_id);
            ogs_free(common_ue_id);
            return false;
        } else {
            ogs_free(uri_ue_id);
        }
    }

    if (SubscriptionDataSubscriptions->ue_id && common_ue_id &&
            strcmp(SubscriptionDataSubscriptions->ue_id,
                common_ue_id) != 0) {
        ogs_error("ueId does not match monitoredResourceUris");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "ueId does not match monitoredResourceUris",
                NULL, NULL));
        ogs_free(common_ue_id);
        return false;
    }

    subscription = udr_subscription_add();
    if (!subscription) {
        ogs_free(common_ue_id);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                recvmsg, "No memory", NULL, NULL));
        return false;
    }

    ogs_uuid_get(&uuid);
    ogs_uuid_format(id, &uuid);
    subscription->id = ogs_strdup(id);
    ogs_assert(subscription->id);

    subscription->callback_reference =
        ogs_strdup(SubscriptionDataSubscriptions->callback_reference);
    ogs_assert(subscription->callback_reference);

    if (SubscriptionDataSubscriptions->original_callback_reference) {
        subscription->original_callback_reference = ogs_strdup(
                SubscriptionDataSubscriptions->original_callback_reference);
        ogs_assert(subscription->original_callback_reference);
    }

    subscription->ue_id = common_ue_id;
    OGS_SBI_SETUP_CLIENT(subscription, client);

    i = 0;
    OpenAPI_list_for_each(monitored_resource_uris, node) {
        subscription->monitored_resource_uri[i] =
            ogs_strdup((char *)node->data);
        ogs_assert(subscription->monitored_resource_uri[i]);
        i++;
    }
    subscription->num_of_monitored_resource_uri = i;

    /* Individual resource URI, per TS29505: {apiRoot}/nudr-dr/{apiVersion}/
     * subscription-data/subs-to-notify/{subsId} */
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    memset(&header, 0, sizeof(header));
    header.service.name =
        (char *)OpenAPI_service_name_ToString(OpenAPI_service_name_nudr_dr);
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA;
    header.resource.component[1] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBS_TO_NOTIFY;
    header.resource.component[2] = subscription->id;

    recvmsg->http.location = ogs_sbi_server_uri(server, &header);

    /* Echo the accepted subscription back, adding the UDR-allocated ID. */
    SubscriptionDataSubscriptions->subscription_id =
        ogs_strdup(subscription->id);
    ogs_assert(SubscriptionDataSubscriptions->subscription_id);

    response = ogs_sbi_build_response(recvmsg, OGS_SBI_HTTP_STATUS_CREATED);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    if (recvmsg->http.location)
        ogs_free(recvmsg->http.location);

    ogs_info("[%s] sm-data change subscription created for ueId[%s]",
            subscription->id,
            subscription->ue_id ? subscription->ue_id : "(none)");

    return true;
}

bool udr_nudr_dr_handle_subs_to_notify_delete(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg, char *subs_id)
{
    udr_subscription_t *subscription = NULL;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    if (!subs_id) {
        ogs_error("No subscription ID");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No subscription ID", NULL, NULL));
        return false;
    }

    subscription = udr_subscription_find_by_id(subs_id);
    if (!subscription) {
        ogs_error("[%s] Subscription not found", subs_id);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                recvmsg, "Subscription not found", subs_id, NULL));
        return false;
    }

    ogs_info("[%s] sm-data change subscription deleted", subscription->id);
    udr_subscription_remove(subscription);

    response = ogs_sbi_build_response(recvmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return true;
}

static int client_notify_cb(
        int status, ogs_sbi_response_t *response, void *data)
{
    int rv;
    ogs_sbi_message_t message;

    if (status != OGS_OK) {
        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "DataChangeNotify callback failed [%d]", status);
        return OGS_ERROR;
    }

    ogs_assert(response);

    rv = ogs_sbi_parse_response(&message, response);
    if (rv != OGS_OK) {
        ogs_error("cannot parse HTTP response");
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        return OGS_ERROR;
    }

    if (message.res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT)
        ogs_warn("DataChangeNotify callback returned [%d]",
                message.res_status);

    ogs_sbi_message_free(&message);
    ogs_sbi_response_free(response);
    return OGS_OK;
}

/*
 * Notify every subscription monitoring `supi`'s sm-data resource that it
 * has changed. One failed/slow callback (this call is non-blocking) does
 * not prevent notifying the others.
 */
void udr_nudr_dr_notify_sm_data_change(const char *supi)
{
    udr_subscription_t *subscription = NULL;
    ogs_subscription_data_t subscription_data;
    bool fetched = false;

    ogs_assert(supi);

    ogs_list_for_each(&udr_self()->subscription_list, subscription) {
        int i;
        OpenAPI_list_t *NotifyItems = NULL;
        OpenAPI_data_change_notify_t DataChangeNotify;
        ogs_sbi_message_t message;
        ogs_sbi_request_t *request = NULL;

        if (!subscription->client)
            continue;
        if (!subscription->ue_id || strcmp(subscription->ue_id, supi) != 0)
            continue;
        if (subscription->expiry && subscription->expiry < ogs_time_now())
            continue;

        if (!fetched) {
            memset(&subscription_data, 0, sizeof(subscription_data));
            if (ogs_dbi_subscription_data(
                    (char *)supi, &subscription_data) != OGS_OK) {
                ogs_error("[%s] Cannot find SUPI in DB", supi);
                return;
            }
            fetched = true;
        }

        NotifyItems = OpenAPI_list_create();

        for (i = 0; i < subscription->num_of_monitored_resource_uri; i++) {
            OpenAPI_list_t *SessionManagementSubscriptionDataList = NULL;
            OpenAPI_sm_subs_data_t smSubsData;
            cJSON *newValueJSON = NULL;
            OpenAPI_change_item_t *ChangeItem = NULL;
            OpenAPI_list_t *Changes = NULL;
            OpenAPI_notify_item_t *NotifyItem = NULL;

            SessionManagementSubscriptionDataList =
                udr_build_sm_subs_data_list(
                        &subscription_data, false, NULL, NULL);

            memset(&smSubsData, 0, sizeof(smSubsData));
            smSubsData.session_management_subscription_data_list =
                SessionManagementSubscriptionDataList;

            newValueJSON = OpenAPI_sm_subs_data_convertToJSON(&smSubsData);
            udr_free_sm_subs_data_list(SessionManagementSubscriptionDataList);

            if (!newValueJSON) {
                ogs_error("OpenAPI_sm_subs_data_convertToJSON() failed");
                continue;
            }

            ChangeItem = OpenAPI_change_item_create(
                    OpenAPI_change_type_REPLACE, ogs_strdup(""), NULL,
                    false, NULL,
                    false, OpenAPI_any_type_create(newValueJSON));
            ogs_assert(ChangeItem);

            Changes = OpenAPI_list_create();
            OpenAPI_list_add(Changes, ChangeItem);

            NotifyItem = OpenAPI_notify_item_create(
                    ogs_strdup(subscription->monitored_resource_uri[i]),
                    Changes);
            ogs_assert(NotifyItem);

            OpenAPI_list_add(NotifyItems, NotifyItem);
        }

        if (!NotifyItems->count) {
            OpenAPI_list_free(NotifyItems);
            continue;
        }

        memset(&DataChangeNotify, 0, sizeof(DataChangeNotify));
        DataChangeNotify.ue_id = ogs_strdup(subscription->ue_id);
        ogs_assert(DataChangeNotify.ue_id);
        DataChangeNotify.notify_items = NotifyItems;

        memset(&message, 0, sizeof(message));
        message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
        message.h.uri = subscription->callback_reference;
        message.DataChangeNotify = &DataChangeNotify;

        request = ogs_sbi_build_request(&message);
        if (!request) {
            ogs_error("[%s] ogs_sbi_build_request() failed", subscription->id);
        } else {
            if (!ogs_sbi_send_request_to_client(
                    (ogs_sbi_client_t *)subscription->client,
                    client_notify_cb, request, NULL))
                ogs_error("[%s] Failed to send DataChangeNotify",
                        subscription->id);
            ogs_sbi_request_free(request);
        }

        if (DataChangeNotify.ue_id)
            ogs_free(DataChangeNotify.ue_id);
        if (DataChangeNotify.notify_items) {
            OpenAPI_lnode_t *node = NULL;
            OpenAPI_list_for_each(DataChangeNotify.notify_items, node)
                OpenAPI_notify_item_free(node->data);
            OpenAPI_list_free(DataChangeNotify.notify_items);
        }

        ogs_info("[%s] sm-data DataChangeNotify sent to [%s]",
                subscription->id, subscription->callback_reference);
    }

    if (fetched)
        ogs_subscription_data_free(&subscription_data);
}
