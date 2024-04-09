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
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No SUPI", NULL, NULL));
        return false;
    }

    if (strncmp(supi,
            OGS_ID_SUPI_TYPE_IMSI, strlen(OGS_ID_SUPI_TYPE_IMSI)) != 0) {
        ogs_error("[%s] Unknown SUPI Type", supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                recvmsg, "Unknwon SUPI Type", supi, NULL));
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
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    char *supi = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    supi = recvmsg->h.resource.component[1];
    if (!supi) {
        ogs_error("No SUPI");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No SUPI", NULL, NULL));
        return false;
    }

    if (strncmp(supi,
            OGS_ID_SUPI_TYPE_IMSI, strlen(OGS_ID_SUPI_TYPE_IMSI)) != 0) {
        ogs_error("[%s] Unknown SUPI Type", supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                recvmsg, "Unknwon SUPI Type", supi, NULL));
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
                char *pei = NULL;

                pei = ogs_strdup(Amf3GppAccessRegistration->pei);
                ogs_assert(pei);

                type = ogs_id_get_type(pei);
                ogs_assert(type);
                value = ogs_id_get_value(pei);
                ogs_assert(value);

                if (strcmp(type, "imeisv") == 0) {
                    ogs_assert(OGS_OK == ogs_dbi_update_imeisv(supi, value));
                } else {
                    ogs_fatal("Unknown Type = %s", type);
                    ogs_assert_if_reached();
                }

                ogs_free(pei);
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

bool udr_nudr_dr_handle_subscription_provisioned(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv, status = 0;
    char *strerror = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;
    ogs_subscription_data_t subscription_data;
    ogs_slice_data_t *slice_data = NULL;

    char *supi = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    memset(&subscription_data, 0, sizeof(ogs_subscription_data_t));

    supi = recvmsg->h.resource.component[1];
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

    rv = ogs_dbi_subscription_data(supi, &subscription_data);
    if (rv != OGS_OK) {
        strerror = ogs_msprintf("[%s] Cannot find SUPI in DB", supi);
        status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
        goto cleanup;
    }

    if (!subscription_data.ambr.uplink && !subscription_data.ambr.downlink) {
        strerror = ogs_msprintf("[%s] No UE-AMBR", supi);
        status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
        goto cleanup;
    }

    SWITCH(recvmsg->h.resource.component[4])
    CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
        int i;

        OpenAPI_access_and_mobility_subscription_data_t
            AccessAndMobilitySubscriptionData;
        OpenAPI_list_t *GpsiList = NULL;
        OpenAPI_ambr_rm_t SubscribedUeAmbr;

        OpenAPI_nssai_t NSSAI;
        OpenAPI_list_t *DefaultSingleNssaiList = NULL;
        OpenAPI_list_t *SingleNssaiList = NULL;
        OpenAPI_snssai_t *Snssai = NULL;

        OpenAPI_lnode_t *node = NULL;

        GpsiList = OpenAPI_list_create();
        for (i = 0; i < subscription_data.num_of_msisdn; i++) {
            char *gpsi = ogs_msprintf("%s-%s",
                    OGS_ID_GPSI_TYPE_MSISDN, subscription_data.msisdn[i].bcd);
            ogs_assert(gpsi);
            OpenAPI_list_add(GpsiList, gpsi);
        }

        SubscribedUeAmbr.uplink = ogs_sbi_bitrate_to_string(
                subscription_data.ambr.uplink, OGS_SBI_BITRATE_KBPS);
        SubscribedUeAmbr.downlink = ogs_sbi_bitrate_to_string(
                subscription_data.ambr.downlink, OGS_SBI_BITRATE_KBPS);

        memset(&NSSAI, 0, sizeof(NSSAI));
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

        memset(&AccessAndMobilitySubscriptionData, 0,
                sizeof(AccessAndMobilitySubscriptionData));

        if (GpsiList->count)
            AccessAndMobilitySubscriptionData.gpsis = GpsiList;

        AccessAndMobilitySubscriptionData.subscribed_ue_ambr =
            &SubscribedUeAmbr;

        if (DefaultSingleNssaiList->count)
            AccessAndMobilitySubscriptionData.nssai = &NSSAI;

        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.AccessAndMobilitySubscriptionData =
            &AccessAndMobilitySubscriptionData;

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));

        OpenAPI_list_for_each(GpsiList, node) {
            if (node->data) ogs_free(node->data);
        }
        OpenAPI_list_free(GpsiList);

        ogs_free(SubscribedUeAmbr.uplink);
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
        break;

    CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECTION_SUBSCRIPTION_DATA)
        int i, j;

        OpenAPI_smf_selection_subscription_data_t SmfSelectionSubscriptionData;

        OpenAPI_list_t *SubscribedSnssaiInfoList = NULL;
        OpenAPI_map_t *SubscribedSnssaiInfoMap = NULL;
        OpenAPI_snssai_info_t *SubscribedSnssaiInfo = NULL;

        OpenAPI_list_t *DnnInfoList = NULL;
        OpenAPI_dnn_info_t *DnnInfo = NULL;

        OpenAPI_lnode_t *node = NULL, *node2 = NULL;

        memset(&SmfSelectionSubscriptionData, 0,
                sizeof(SmfSelectionSubscriptionData));

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

        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.SmfSelectionSubscriptionData = &SmfSelectionSubscriptionData;

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));

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
        break;

    CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
        int i;

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
        OpenAPI_list_t *FrameRouteList = NULL;
        OpenAPI_lnode_t *node = NULL, *node2 = NULL;

        if (!recvmsg->param.single_nssai_presence) {
            strerror = ogs_msprintf("[%s] No S_NSSAI", supi);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        };

        ogs_assert(subscription_data.num_of_slice);
        slice_data = ogs_slice_find_by_s_nssai(
                subscription_data.slice, subscription_data.num_of_slice,
                &recvmsg->param.s_nssai);

        if (!slice_data) {
            strerror = ogs_msprintf("[%s] Cannot find S_NSSAI[SST:%d SD:0x%x]",
                    supi,
                    recvmsg->param.s_nssai.sst,
                    recvmsg->param.s_nssai.sd.v);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        }

        singleNSSAI.sst = slice_data->s_nssai.sst;
        singleNSSAI.sd = ogs_s_nssai_sd_to_string(slice_data->s_nssai.sd);

        dnnConfigurationList = OpenAPI_list_create();

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
            pduSessionTypeList->default_session_type = session->session_type;

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

        memset(&SessionManagementSubscriptionData, 0,
                sizeof(SessionManagementSubscriptionData));
        SessionManagementSubscriptionData.single_nssai = &singleNSSAI;
        if (dnnConfigurationList->count)
            SessionManagementSubscriptionData.dnn_configurations =
                dnnConfigurationList;

        memset(&sendmsg, 0, sizeof(sendmsg));
        
        sendmsg.SessionManagementSubscriptionDataList = OpenAPI_list_create();
        ogs_assert(sendmsg.SessionManagementSubscriptionDataList);

        OpenAPI_list_add(sendmsg.SessionManagementSubscriptionDataList,
            &SessionManagementSubscriptionData);

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));

        OpenAPI_list_free(sendmsg.SessionManagementSubscriptionDataList);

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

                    FrameRouteList = dnnConfiguration->ipv4_frame_route_list;
                    OpenAPI_list_for_each(FrameRouteList, node2) {
                        OpenAPI_frame_route_info_t *frame = node2->data;
                        if (frame)
                            ogs_free(frame);
                    }
                    OpenAPI_list_free(FrameRouteList);

                    FrameRouteList = dnnConfiguration->ipv6_frame_route_list;
                    OpenAPI_list_for_each(FrameRouteList, node2) {
                        OpenAPI_frame_route_info_t *frame = node2->data;
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

        break;

    DEFAULT
        strerror = ogs_msprintf("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
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
