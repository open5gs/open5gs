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
#include "nnrf-handler.h"
#include "nudr-handler.h"

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
    if (!supi) {
        ogs_error("No SUPI");
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No SUPI", NULL);
        return false;
    }

    if (strncmp(supi,
            OGS_ID_SUPI_TYPE_IMSI, strlen(OGS_ID_SUPI_TYPE_IMSI)) != 0) {
        ogs_error("[%s] Unknown SUPI Type", supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                recvmsg, "Unknwon SUPI Type", supi);
        return false;
    }

    rv = ogs_dbi_auth_info(supi, &auth_info);
    if (rv != OGS_OK) {
        ogs_warn("[%s] Cannot find SUPI in DB", supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                recvmsg, "Cannot find SUPI Type", supi);
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
            ogs_sbi_server_send_response(stream, response);

            return true;

        CASE(OGS_SBI_HTTP_METHOD_PATCH)
            char *sqn_string = NULL;
            uint8_t sqn_ms[OGS_SQN_LEN];
            uint64_t sqn = 0;

            PatchItemList = recvmsg->PatchItemList;
            if (!PatchItemList) {
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        recvmsg, "No PatchItemList Array", NULL);
                return false;
            }

            OpenAPI_list_for_each(PatchItemList, node) {
                if (node->data) {
                    OpenAPI_patch_item_t *patch_item = node->data;
                    sqn_string = patch_item->value;
                }
            }

            if (!sqn_string) {
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        recvmsg, "No PatchItemList", NULL);
                return false;
            }

            ogs_ascii_to_hex(sqn_string, strlen(sqn_string),
                    sqn_ms, sizeof(sqn_ms));
            sqn = ogs_buffer_to_uint64(sqn_ms, OGS_SQN_LEN);

            rv = ogs_dbi_update_sqn(supi, sqn);
            if (rv != OGS_OK) {
                ogs_fatal("[%s] Cannot update SQN", supi);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                        recvmsg, "Cannot update SQN", supi);
                return false;
            }

            rv = ogs_dbi_increment_sqn(supi);
            if (rv != OGS_OK) {
                ogs_fatal("[%s] Cannot increment SQN", supi);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                        recvmsg, "Cannot increment SQN", supi);
                return false;
            }

            memset(&sendmsg, 0, sizeof(sendmsg));

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_sbi_server_send_response(stream, response);

            return true;

        DEFAULT
            ogs_error("Invalid HTTP method [%s]", recvmsg->h.method);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED,
                    recvmsg, "Invalid HTTP method", recvmsg->h.method);
        END
        break;

    CASE(OGS_SBI_RESOURCE_NAME_AUTHENTICATION_STATUS)
        SWITCH(recvmsg->h.method)
        CASE(OGS_SBI_HTTP_METHOD_PUT)
            OpenAPI_auth_event_t *AuthEvent = NULL;

            AuthEvent = recvmsg->AuthEvent;
            if (!AuthEvent) {
                ogs_error("[%s] No AuthEvent", supi);
                ogs_sbi_server_send_error(
                        stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        recvmsg, "No AuthEvent", supi);
                return false;
            }

            memset(&sendmsg, 0, sizeof(sendmsg));
            rv = ogs_dbi_increment_sqn(supi);
            if (rv != OGS_OK) {
                ogs_fatal("[%s] Cannot increment SQN", supi);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                        recvmsg, "Cannot increment SQN", supi);
                return false;
            }

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_sbi_server_send_response(stream, response);

            return true;

        DEFAULT
            ogs_error("Invalid HTTP method [%s]", recvmsg->h.method);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED,
                    recvmsg, "Invalid HTTP method", recvmsg->h.method);
        END
        break;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
        ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED,
                recvmsg, "Unknown resource name",
                recvmsg->h.resource.component[3]);
    END

    return false;
}

bool udr_nudr_dr_handle_subscription_context(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    char *supi = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    supi = recvmsg->h.resource.component[1];
    if (!supi) {
        ogs_error("No SUPI");
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No SUPI", NULL);
        return false;
    }

    if (strncmp(supi,
            OGS_ID_SUPI_TYPE_IMSI, strlen(OGS_ID_SUPI_TYPE_IMSI)) != 0) {
        ogs_error("[%s] Unknown SUPI Type", supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                recvmsg, "Unknwon SUPI Type", supi);
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
                ogs_sbi_server_send_error(
                        stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        recvmsg, "No Amf3GppAccessRegistration", supi);
                return false;
            }

            memset(&sendmsg, 0, sizeof(sendmsg));

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_sbi_server_send_response(stream, response);

            return true;

        DEFAULT
            ogs_error("Invalid HTTP method [%s]", recvmsg->h.method);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED,
                    recvmsg, "Invalid HTTP method", recvmsg->h.method);
        END
        break;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
        ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED,
                recvmsg, "Unknown resource name",
                recvmsg->h.resource.component[3]);
    END

    return false;
}

static OpenAPI_pdu_session_type_e pdu_session_type_from_dbi(uint8_t pdn_type)
{
    if (pdn_type == OGS_DIAM_PDN_TYPE_IPV4)
        return OpenAPI_pdu_session_type_IPV4;
    else if (pdn_type == OGS_DIAM_PDN_TYPE_IPV6)
        return OpenAPI_pdu_session_type_IPV6;
    else if (pdn_type == OGS_DIAM_PDN_TYPE_IPV4V6)
        return OpenAPI_pdu_session_type_IPV4V6;
    else {
        ogs_fatal("Unsupported PDN_TYPE[%d]", pdn_type);
        ogs_assert_if_reached();
    }
}

bool udr_nudr_dr_handle_subscription_provisioned(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv, i;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;
    ogs_subscription_data_t subscription_data;

    char *supi = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    supi = recvmsg->h.resource.component[1];
    if (!supi) {
        ogs_error("No SUPI");
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No SUPI", NULL);
        return false;
    }

    if (strncmp(supi,
            OGS_ID_SUPI_TYPE_IMSI, strlen(OGS_ID_SUPI_TYPE_IMSI)) != 0) {
        ogs_error("[%s] Unknown SUPI Type", supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                recvmsg, "Unknwon SUPI Type", supi);
        return false;
    }

    rv = ogs_dbi_subscription_data(supi, &subscription_data);
    if (rv != OGS_OK) {
        ogs_error("[%s] Cannot find SUPI in DB", supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                recvmsg, "Cannot find SUPI Type", supi);
        return false;
    }

    if (!subscription_data.ambr.uplink && !subscription_data.ambr.downlink) {
        ogs_error("[%s] No UE-AMBR", supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                recvmsg, "No UE-AMBR", supi);

        return false;
    }

    SWITCH(recvmsg->h.resource.component[4])
    CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
        OpenAPI_access_and_mobility_subscription_data_t
            AccessAndMobilitySubscriptionData;
        OpenAPI_ambr_rm_t subscribed_ue_ambr;
        OpenAPI_list_t *gpsiList = NULL;
        OpenAPI_lnode_t *node = NULL;

        subscribed_ue_ambr.uplink = ogs_sbi_bitrate_to_string(
                subscription_data.ambr.uplink, OGS_SBI_BITRATE_KBPS);
        subscribed_ue_ambr.downlink = ogs_sbi_bitrate_to_string(
                subscription_data.ambr.downlink, OGS_SBI_BITRATE_KBPS);

        gpsiList = OpenAPI_list_create();

        for (i = 0; i < subscription_data.num_of_msisdn; i++) {
            char *gpsi = ogs_msprintf("%s-%s",
                    OGS_ID_GPSI_TYPE_MSISDN, subscription_data.msisdn[i].bcd);
            ogs_assert(gpsi);
            OpenAPI_list_add(gpsiList, gpsi);
        }

        memset(&AccessAndMobilitySubscriptionData, 0,
                sizeof(AccessAndMobilitySubscriptionData));
        AccessAndMobilitySubscriptionData.subscribed_ue_ambr =
            &subscribed_ue_ambr;

        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.AccessAndMobilitySubscriptionData =
            &AccessAndMobilitySubscriptionData;

        if (gpsiList->count)
            AccessAndMobilitySubscriptionData.gpsis = gpsiList;

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        ogs_free(subscribed_ue_ambr.uplink);
        ogs_free(subscribed_ue_ambr.downlink);
        OpenAPI_list_for_each(gpsiList, node) {
            if (node->data) ogs_free(node->data);
        }
        OpenAPI_list_free(gpsiList);

        break;

    CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
        OpenAPI_smf_selection_subscription_data_t SmfSelectionSubscriptionData;

        memset(&SmfSelectionSubscriptionData, 0,
                sizeof(SmfSelectionSubscriptionData));

        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.SmfSelectionSubscriptionData = &SmfSelectionSubscriptionData;

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        break;

    CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
        OpenAPI_session_management_subscription_data_t
            SessionManagementSubscriptionData;
        OpenAPI_snssai_t singleNSSAI;
        OpenAPI_list_t *dnnConfigurationList = NULL;
        OpenAPI_map_t *dnnConfigurationMap = NULL;
        OpenAPI_dnn_configuration_t *dnnConfiguration = NULL;
        OpenAPI_pdu_session_types_t *pduSessionTypeList = NULL;
        OpenAPI_ssc_modes_t *sscModeList = NULL;
        OpenAPI_subscribed_default_qos_t *_5gQoSProfile = NULL;
        OpenAPI_ambr_t *sessionAmbr = NULL;
        OpenAPI_list_t *staticIpAddress = NULL;
        OpenAPI_ip_address_t *ipAddress = NULL;
        OpenAPI_lnode_t *node = NULL, *node2 = NULL;

        if (!recvmsg->param.single_nssai_presence) {
            ogs_error("[%s] Cannot find S_NSSAI", supi);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "Cannot find S_NSSAI", supi);
            return false;
        }

        singleNSSAI.sst = recvmsg->param.single_nssai.sst;
        singleNSSAI.sd = ogs_s_nssai_sd_to_string(
                            recvmsg->param.single_nssai.sd);

        dnnConfigurationList = OpenAPI_list_create();

        for (i = 0; i < subscription_data.num_of_pdn; i++) {
            ogs_pdn_t *pdn = &subscription_data.pdn[i];
            ogs_assert(pdn);

            if (recvmsg->param.dnn &&
                ogs_strcasecmp(recvmsg->param.dnn, pdn->apn) != 0) continue;

            if (!pdn->qos.qci) {
                ogs_error("No QCI");
                continue;
            }
            if (!pdn->qos.arp.priority_level) {
                ogs_error("No Priority Level");
                continue;
            }

            if (!pdn->ambr.uplink && !pdn->ambr.downlink) {
                if (!subscription_data.ambr.uplink &&
                        !subscription_data.ambr.downlink) {
                    ogs_error("No Session-AMBR");
                    ogs_error("No UE-AMBR");
                    continue;
                }

                pdn->ambr.uplink = subscription_data.ambr.uplink;
                pdn->ambr.downlink = subscription_data.ambr.downlink;
                ogs_warn("No Session-AMBR : Set UE-AMBR to Session-AMBR "
                        "[DL-%lld:UL-%lld]",
                        (long long)pdn->ambr.downlink,
                        (long long)pdn->ambr.uplink);
            }

            dnnConfiguration = ogs_calloc(1, sizeof(*dnnConfiguration));
            ogs_assert(dnnConfiguration);

            pduSessionTypeList = ogs_calloc(1, sizeof(*pduSessionTypeList));
            ogs_assert(pduSessionTypeList);
            pduSessionTypeList->default_session_type =
                pdu_session_type_from_dbi(pdn->pdn_type);

            pduSessionTypeList->allowed_session_types = OpenAPI_list_create();
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
            _5gQoSProfile->_5qi = pdn->qos.qci;
            _5gQoSProfile->priority_level = pdn->qos.arp.priority_level;
            _5gQoSProfile->arp = ogs_calloc(1, sizeof(OpenAPI_arp_t));
            ogs_assert(_5gQoSProfile->arp);
            _5gQoSProfile->arp->priority_level = pdn->qos.arp.priority_level;
            if (pdn->qos.arp.pre_emption_capability ==
                    OGS_PDN_PRE_EMPTION_CAPABILITY_DISABLED)
                _5gQoSProfile->arp->preempt_cap =
                        OpenAPI_preemption_capability_NOT_PREEMPT;
            else
                _5gQoSProfile->arp->preempt_cap =
                        OpenAPI_preemption_capability_MAY_PREEMPT;
            if (pdn->qos.arp.pre_emption_vulnerability ==
                OGS_PDN_PRE_EMPTION_VULNERABILITY_DISABLED)
                _5gQoSProfile->arp->preempt_vuln =
                    OpenAPI_preemption_vulnerability_NOT_PREEMPTABLE;
            else
                _5gQoSProfile->arp->preempt_vuln =
                    OpenAPI_preemption_vulnerability_PREEMPTABLE;

            dnnConfiguration->_5g_qos_profile = _5gQoSProfile;

            ogs_assert(pdn->ambr.uplink || pdn->ambr.downlink);
            sessionAmbr = ogs_calloc(1, sizeof(*sessionAmbr));
            ogs_assert(sessionAmbr);
            sessionAmbr->uplink = ogs_sbi_bitrate_to_string(
                    pdn->ambr.uplink, OGS_SBI_BITRATE_KBPS);
            sessionAmbr->downlink = ogs_sbi_bitrate_to_string(
                    pdn->ambr.downlink, OGS_SBI_BITRATE_KBPS);

            dnnConfiguration->session_ambr = sessionAmbr;

            staticIpAddress = OpenAPI_list_create();
            ogs_assert(staticIpAddress);

            if (pdn->ue_ip.ipv4 || pdn->ue_ip.ipv6) {
                ipAddress = ogs_calloc(1, sizeof(*ipAddress));
                ogs_assert(ipAddress);

                if (pdn->ue_ip.ipv4)
                    ipAddress->ipv4_addr = ogs_ipv4_to_string(pdn->ue_ip.addr);
                if (pdn->ue_ip.ipv6)
                    ipAddress->ipv6_addr = ogs_ipv6_to_string(pdn->ue_ip.addr6);

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
                    pdn->apn, dnnConfiguration);
            ogs_assert(dnnConfigurationMap);
            OpenAPI_list_add(dnnConfigurationList, dnnConfigurationMap);
        }

        memset(&SessionManagementSubscriptionData, 0,
                sizeof(SessionManagementSubscriptionData));
        SessionManagementSubscriptionData.single_nssai = &singleNSSAI;
        if (dnnConfigurationList->count)
            SessionManagementSubscriptionData.dnn_configurations =
                dnnConfigurationList;

        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.SessionManagementSubscriptionData =
            &SessionManagementSubscriptionData;

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        if (singleNSSAI.sd)
            ogs_free(singleNSSAI.sd);

        OpenAPI_list_for_each(dnnConfigurationList, node) {
            dnnConfigurationMap = node->data;
            if (dnnConfigurationMap) {
                dnnConfiguration = dnnConfigurationMap->value;
                if (dnnConfiguration) {
                    pduSessionTypeList = dnnConfiguration->pdu_session_types;
                    if (pduSessionTypeList) {
                        if (pduSessionTypeList->allowed_session_types)
                            OpenAPI_list_free(
                                    pduSessionTypeList->allowed_session_types);
                        ogs_free(pduSessionTypeList);
                    }
                    sscModeList = dnnConfiguration->ssc_modes;;
                    if (sscModeList) {
                        if (sscModeList->allowed_ssc_modes)
                            OpenAPI_list_free(sscModeList->allowed_ssc_modes);
                        ogs_free(sscModeList);
                    }
                    _5gQoSProfile = dnnConfiguration->_5g_qos_profile;
                    if (_5gQoSProfile) {
                        if (_5gQoSProfile->arp)
                            ogs_free(_5gQoSProfile->arp);
                        ogs_free(_5gQoSProfile);
                    }

                    sessionAmbr = dnnConfiguration->session_ambr;
                    if (sessionAmbr) {
                        if (sessionAmbr->uplink)
                            ogs_free(sessionAmbr->uplink);
                        if (sessionAmbr->downlink)
                            ogs_free(sessionAmbr->downlink);
                        ogs_free(sessionAmbr);
                    }

                    staticIpAddress = dnnConfiguration->static_ip_address;
                    if (staticIpAddress) {
                        OpenAPI_list_for_each(staticIpAddress, node2) {
                            if (node2->data) {
                                ipAddress = node2->data;
                                if (ipAddress) {
                                    if (ipAddress->ipv4_addr)
                                        ogs_free(ipAddress->ipv4_addr);
                                    if (ipAddress->ipv6_addr)
                                        ogs_free(ipAddress->ipv6_addr);
                                    ogs_free(ipAddress);
                                }
                            }
                        }
                        OpenAPI_list_free(staticIpAddress);
                    }

                    ogs_free(dnnConfiguration);
                }
                ogs_free(dnnConfigurationMap);
            }
        }

        OpenAPI_list_free(dnnConfigurationList);
        break;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
        ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED,
                recvmsg, "Unknown resource name",
                recvmsg->h.resource.component[3]);
        return false;
    END

    return true;
}

bool udr_nudr_dr_handle_policy_data(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv, i;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[1])
    CASE(OGS_SBI_RESOURCE_NAME_UES)
        ogs_subscription_data_t subscription_data;
        char *supi = recvmsg->h.resource.component[2];

        if (!supi) {
            ogs_error("No SUPI");
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No SUPI", NULL);
            return false;
        }

        if (strncmp(supi,
                OGS_ID_SUPI_TYPE_IMSI, strlen(OGS_ID_SUPI_TYPE_IMSI)) != 0) {
            ogs_error("[%s] Unknown SUPI Type", supi);
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                    recvmsg, "Unknwon SUPI Type", supi);
            return false;
        }

        SWITCH(recvmsg->h.method)
        CASE(OGS_SBI_HTTP_METHOD_GET)
            OpenAPI_lnode_t *node = NULL, *node2 = NULL;

            rv = ogs_dbi_subscription_data(supi, &subscription_data);
            if (rv != OGS_OK) {
                ogs_error("[%s] Cannot find SUPI in DB", supi);
                ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                        recvmsg, "Cannot find SUPI Type", supi);
                return false;
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
                ogs_sbi_server_send_response(stream, response);

                return true;

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
                    ogs_error("[%s] Cannot find S_NSSAI", supi);
                    ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                            recvmsg, "Cannot find S_NSSAI", supi);
                    return false;
                }

                sNSSAI = ogs_calloc(1, sizeof(*sNSSAI));
                ogs_assert(sNSSAI);
                sNSSAI->sst = recvmsg->param.snssai.sst;
                sNSSAI->sd = ogs_s_nssai_sd_to_string(recvmsg->param.snssai.sd);

                SmPolicyDnnDataList = OpenAPI_list_create();
                ogs_assert(SmPolicyDnnDataList);

                for (i = 0; i < subscription_data.num_of_pdn; i++) {
                    ogs_pdn_t *pdn = &subscription_data.pdn[i];
                    ogs_assert(pdn);

                    if (recvmsg->param.dnn &&
                        ogs_strcasecmp(recvmsg->param.dnn, pdn->apn) != 0)
                        continue;

                    SmPolicyDnnData = ogs_calloc(1, sizeof(*SmPolicyDnnData));
                    ogs_assert(SmPolicyDnnData);

                    SmPolicyDnnData->dnn = pdn->apn;

                    SmPolicyDnnDataMap = OpenAPI_map_create(
                            pdn->apn, SmPolicyDnnData);
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
                        ogs_sbi_s_nssai_to_string(&recvmsg->param.snssai),
                        SmPolicySnssaiData);
                ogs_assert(SmPolicySnssaiDataMap);

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
                ogs_sbi_server_send_response(stream, response);

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

                return true;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        recvmsg->h.resource.component[3]);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED,
                        recvmsg, "Unknown resource name",
                        recvmsg->h.resource.component[3]);
            END
            break;

        DEFAULT
            ogs_error("Invalid HTTP method [%s]", recvmsg->h.method);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED,
                    recvmsg, "Invalid HTTP method", recvmsg->h.method);
        END
        break;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[1]);
        ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED,
                recvmsg, "Unknown resource name",
                recvmsg->h.resource.component[1]);
    END

    return false;
}
