/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "namf-handler.h"
#include "nsmf-handler.h"

#include "nas-path.h"
#include "ngap-path.h"
#include "sbi-path.h"

int amf_namf_comm_handle_n1_n2_message_transfer(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int status, r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    amf_sess_t *sess = NULL;

    ogs_pkbuf_t *n1buf = NULL;
    ogs_pkbuf_t *n2buf = NULL;

    ogs_pkbuf_t *gmmbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    char *supi = NULL;
    uint8_t pdu_session_id = OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_n1_n2_message_transfer_req_data_t *N1N2MessageTransferReqData;
    OpenAPI_n1_n2_message_transfer_rsp_data_t N1N2MessageTransferRspData;
    OpenAPI_n1_message_container_t *n1MessageContainer = NULL;
    OpenAPI_ref_to_binary_data_t *n1MessageContent = NULL;
    OpenAPI_n2_info_container_t *n2InfoContainer = NULL;
    OpenAPI_n2_sm_information_t *smInfo = NULL;
    OpenAPI_n2_info_content_t *n2InfoContent = NULL;
    OpenAPI_ref_to_binary_data_t *ngapData = NULL;

    OpenAPI_ngap_ie_type_e ngapIeType = OpenAPI_ngap_ie_type_NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    N1N2MessageTransferReqData = recvmsg->N1N2MessageTransferReqData;
    if (!N1N2MessageTransferReqData) {
        ogs_error("No N1N2MessageTransferReqData");
        return OGS_ERROR;
    }

    if (N1N2MessageTransferReqData->is_pdu_session_id == false) {
        ogs_error("No PDU Session Identity");
        return OGS_ERROR;
    }
    pdu_session_id = N1N2MessageTransferReqData->pdu_session_id;

    supi = recvmsg->h.resource.component[1];
    if (!supi) {
        ogs_error("No SUPI");
        return OGS_ERROR;
    }

    amf_ue = amf_ue_find_by_supi(supi);
    if (!amf_ue) {
        ogs_error("No UE context [%s]", supi);
        return OGS_ERROR;
    }

    sess = amf_sess_find_by_psi(amf_ue, pdu_session_id);
    if (!sess) {
        ogs_error("[%s] No PDU Session Context [%d]",
                amf_ue->supi, pdu_session_id);
        return OGS_ERROR;
    }

    n1MessageContainer = N1N2MessageTransferReqData->n1_message_container;
    if (n1MessageContainer) {
        n1MessageContent = n1MessageContainer->n1_message_content;
        if (!n1MessageContent || !n1MessageContent->content_id) {
            ogs_error("No n1MessageContent");
            return OGS_ERROR;
        }

        n1buf = ogs_sbi_find_part_by_content_id(
                recvmsg, n1MessageContent->content_id);
        if (!n1buf) {
            ogs_error("[%s] No N1 SM Content", amf_ue->supi);
            return OGS_ERROR;
        }

        /*
         * NOTE : The pkbuf created in the SBI message will be removed
         *        from ogs_sbi_message_free(), so it must be copied.
         */
        n1buf = ogs_pkbuf_copy(n1buf);
        ogs_assert(n1buf);
    }

    n2InfoContainer = N1N2MessageTransferReqData->n2_info_container;
    if (n2InfoContainer) {
        smInfo = n2InfoContainer->sm_info;
        if (!smInfo) {
            ogs_error("No smInfo");
            return OGS_ERROR;
        }

        n2InfoContent = smInfo->n2_info_content;
        if (!n2InfoContent) {
            ogs_error("No n2InfoContent");
            return OGS_ERROR;
        }

        ngapIeType = n2InfoContent->ngap_ie_type;

        ngapData = n2InfoContent->ngap_data;
        if (!ngapData || !ngapData->content_id) {
            ogs_error("No ngapData");
            return OGS_ERROR;
        }
        n2buf = ogs_sbi_find_part_by_content_id(
                recvmsg, ngapData->content_id);
        if (!n2buf) {
            ogs_error("[%s] No N2 SM Content", amf_ue->supi);
            return OGS_ERROR;
        }

        /*
         * NOTE : The pkbuf created in the SBI message will be removed
         *        from ogs_sbi_message_free(), so it must be copied.
         */
        n2buf = ogs_pkbuf_copy(n2buf);
        ogs_assert(n2buf);
    }

    memset(&sendmsg, 0, sizeof(sendmsg));

    status = OGS_SBI_HTTP_STATUS_OK;

    memset(&N1N2MessageTransferRspData, 0, sizeof(N1N2MessageTransferRspData));
    N1N2MessageTransferRspData.cause =
        OpenAPI_n1_n2_message_transfer_cause_N1_N2_TRANSFER_INITIATED;

    sendmsg.N1N2MessageTransferRspData = &N1N2MessageTransferRspData;

    switch (ngapIeType) {
    case OpenAPI_ngap_ie_type_PDU_RES_SETUP_REQ:
        if (!n2buf) {
            ogs_error("[%s] No N2 SM Content", amf_ue->supi);
            return OGS_ERROR;
        }

        if (n1buf) {
            gmmbuf = gmm_build_dl_nas_transport(sess,
                    OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, n1buf, 0, 0);
            ogs_assert(gmmbuf);
        }

        if (gmmbuf) {
            /***********************************
             * 4.3.2 PDU Session Establishment *
             ***********************************/

            ran_ue = ran_ue_cycle(sess->ran_ue);
            if (ran_ue) {
                if (sess->pdu_session_establishment_accept) {
                    ogs_pkbuf_free(sess->pdu_session_establishment_accept);
                    sess->pdu_session_establishment_accept = NULL;
                }

                if (ran_ue->initial_context_setup_request_sent == true) {
                    ngapbuf =
                        ngap_sess_build_pdu_session_resource_setup_request(
                                ran_ue, sess, gmmbuf, n2buf);
                    ogs_assert(ngapbuf);
                } else {
                    ngapbuf = ngap_sess_build_initial_context_setup_request(
                            ran_ue, sess, gmmbuf, n2buf);
                    ogs_assert(ngapbuf);

                    ran_ue->initial_context_setup_request_sent = true;
                }

                if (SESSION_CONTEXT_IN_SMF(sess)) {
                /*
                 * [1-CLIENT] /nsmf-pdusession/v1/sm-contexts
                 * [2-SERVER] /namf-comm/v1/ue-contexts/{supi}/n1-n2-messages
                 *
                 * If [2-SERVER] arrives after [1-CLIENT],
                 * sm-context-ref is created in [1-CLIENT].
                 * So, the PDU session establishment accpet can be transmitted.
                 */
                    r = ngap_send_to_ran_ue(ran_ue, ngapbuf);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else {
                    sess->pdu_session_establishment_accept = ngapbuf;
                }
            } else {
                ogs_warn("[%s] RAN-NG Context has already been removed",
                            amf_ue->supi);
            }

        } else {
            /*********************************************
             * 4.2.3.3 Network Triggered Service Request *
             *********************************************/

            if (CM_IDLE(amf_ue)) {
                bool rc;
                ogs_sbi_server_t *server = NULL;
                ogs_sbi_header_t header;
                ogs_sbi_client_t *client = NULL;
                OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
                char *fqdn = NULL;
                uint16_t fqdn_port = 0;
                ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

                if (!N1N2MessageTransferReqData->n1n2_failure_txf_notif_uri) {
                    ogs_error("[%s:%d] No n1-n2-failure-notification-uri",
                            amf_ue->supi, sess->psi);
                    return OGS_ERROR;
                }

                rc = ogs_sbi_getaddr_from_uri(
                        &scheme, &fqdn, &fqdn_port, &addr, &addr6,
                        N1N2MessageTransferReqData->n1n2_failure_txf_notif_uri);
                if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
                    ogs_error("[%s:%d] Invalid URI [%s]",
                            amf_ue->supi, sess->psi,
                            N1N2MessageTransferReqData->
                                n1n2_failure_txf_notif_uri);
                    return OGS_ERROR;
                }

                client = ogs_sbi_client_find(
                        scheme, fqdn, fqdn_port, addr, addr6);
                if (!client) {
                    ogs_debug("%s: ogs_sbi_client_add()", OGS_FUNC);
                    client = ogs_sbi_client_add(
                            scheme, fqdn, fqdn_port, addr, addr6);
                    if (!client) {
                        ogs_error("%s: ogs_sbi_client_add() failed", OGS_FUNC);

                        ogs_free(fqdn);
                        ogs_freeaddrinfo(addr);
                        ogs_freeaddrinfo(addr6);

                        return OGS_ERROR;
                    }
                }
                OGS_SBI_SETUP_CLIENT(&sess->paging, client);

                ogs_free(fqdn);
                ogs_freeaddrinfo(addr);
                ogs_freeaddrinfo(addr6);

                status = OGS_SBI_HTTP_STATUS_ACCEPTED;
                N1N2MessageTransferRspData.cause =
                    OpenAPI_n1_n2_message_transfer_cause_ATTEMPTING_TO_REACH_UE;

                /* Location */
                server = ogs_sbi_server_from_stream(stream);
                ogs_assert(server);

                memset(&header, 0, sizeof(header));
                header.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_COMM;
                header.api.version = (char *)OGS_SBI_API_V1;
                header.resource.component[0] =
                    (char *)OGS_SBI_RESOURCE_NAME_UE_CONTEXTS;
                header.resource.component[1] = amf_ue->supi;
                header.resource.component[2] =
                    (char *)OGS_SBI_RESOURCE_NAME_N1_N2_MESSAGES;
                header.resource.component[3] = sess->sm_context.ref;

                sendmsg.http.location = ogs_sbi_server_uri(server, &header);

                /* Store Paging Info */
                AMF_SESS_STORE_PAGING_INFO(
                    sess, sendmsg.http.location,
                    N1N2MessageTransferReqData->n1n2_failure_txf_notif_uri);

                /* Store N2 Transfer message */
                AMF_SESS_STORE_N2_TRANSFER(
                        sess, pdu_session_resource_setup_request, n2buf);

                r = ngap_send_paging(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

            } else if (CM_CONNECTED(amf_ue)) {
                r = nas_send_pdu_session_setup_request(sess, NULL, n2buf);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

            } else {

                ogs_fatal("[%s] Invalid AMF-UE state", amf_ue->supi);
                ogs_assert_if_reached();

            }

        }
        break;

    case OpenAPI_ngap_ie_type_PDU_RES_MOD_REQ:
        if (!n1buf) {
            ogs_error("[%s] No N1 SM Content", amf_ue->supi);
            return OGS_ERROR;
        }
        if (!n2buf) {
            ogs_error("[%s] No N2 SM Content", amf_ue->supi);
            return OGS_ERROR;
        }

        if (CM_IDLE(amf_ue)) {
            ogs_sbi_server_t *server = NULL;
            ogs_sbi_header_t header;

            status = OGS_SBI_HTTP_STATUS_ACCEPTED;
            N1N2MessageTransferRspData.cause =
                OpenAPI_n1_n2_message_transfer_cause_ATTEMPTING_TO_REACH_UE;

            /* Location */
            server = ogs_sbi_server_from_stream(stream);
            ogs_assert(server);

            memset(&header, 0, sizeof(header));
            header.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_COMM;
            header.api.version = (char *)OGS_SBI_API_V1;
            header.resource.component[0] =
                (char *)OGS_SBI_RESOURCE_NAME_UE_CONTEXTS;
            header.resource.component[1] = amf_ue->supi;
            header.resource.component[2] =
                (char *)OGS_SBI_RESOURCE_NAME_N1_N2_MESSAGES;
            header.resource.component[3] = sess->sm_context.ref;

            sendmsg.http.location = ogs_sbi_server_uri(server, &header);

            /* Store Paging Info */
            AMF_SESS_STORE_PAGING_INFO(sess, sendmsg.http.location, NULL);

            /* Store 5GSM Message */
            AMF_SESS_STORE_5GSM_MESSAGE(sess,
                    OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND,
                    n1buf, n2buf);

            r = ngap_send_paging(amf_ue);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

        } else if (CM_CONNECTED(amf_ue)) {
            if (CONTEXT_SETUP_ESTABLISHED(amf_ue)) {
                r = nas_send_pdu_session_modification_command(
                        sess, n1buf, n2buf);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            } else {
                /* Store 5GSM Message */
                ogs_warn("[Session MODIFY] Context setup is not established");
                AMF_SESS_STORE_5GSM_MESSAGE(sess,
                        OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND,
                        n1buf, n2buf);
            }
        } else {
            ogs_fatal("[%s] Invalid AMF-UE state", amf_ue->supi);
            ogs_assert_if_reached();
        }

        break;

    case OpenAPI_ngap_ie_type_PDU_RES_REL_CMD:
        if (!n2buf) {
            ogs_error("[%s] No N2 SM Content", amf_ue->supi);
            return OGS_ERROR;
        }

        if (CM_IDLE(amf_ue)) {
            if (N1N2MessageTransferReqData->is_skip_ind == true &&
                N1N2MessageTransferReqData->skip_ind == true) {

                if (n1buf)
                    ogs_pkbuf_free(n1buf);
                if (n2buf)
                    ogs_pkbuf_free(n2buf);

                N1N2MessageTransferRspData.cause =
                    OpenAPI_n1_n2_message_transfer_cause_N1_MSG_NOT_TRANSFERRED;

            } else {
                ogs_sbi_server_t *server = NULL;
                ogs_sbi_header_t header;

                status = OGS_SBI_HTTP_STATUS_ACCEPTED;
                N1N2MessageTransferRspData.cause =
                    OpenAPI_n1_n2_message_transfer_cause_ATTEMPTING_TO_REACH_UE;

                /* Location */
                server = ogs_sbi_server_from_stream(stream);
                ogs_assert(server);

                memset(&header, 0, sizeof(header));
                header.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_COMM;
                header.api.version = (char *)OGS_SBI_API_V1;
                header.resource.component[0] =
                    (char *)OGS_SBI_RESOURCE_NAME_UE_CONTEXTS;
                header.resource.component[1] = amf_ue->supi;
                header.resource.component[2] =
                    (char *)OGS_SBI_RESOURCE_NAME_N1_N2_MESSAGES;
                header.resource.component[3] = sess->sm_context.ref;

                sendmsg.http.location = ogs_sbi_server_uri(server, &header);

                /* Store Paging Info */
                AMF_SESS_STORE_PAGING_INFO(sess, sendmsg.http.location, NULL);

                /* Store 5GSM Message */
                AMF_SESS_STORE_5GSM_MESSAGE(sess,
                        OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND,
                        n1buf, n2buf);

                r = ngap_send_paging(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }

        } else if (CM_CONNECTED(amf_ue)) {
            if (CONTEXT_SETUP_ESTABLISHED(amf_ue)) {
                r = nas_send_pdu_session_release_command(sess, n1buf, n2buf);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            } else {
                /* Store 5GSM Message */
                ogs_warn("[Session RELEASE] Context setup is not established");
                AMF_SESS_STORE_5GSM_MESSAGE(sess,
                        OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND,
                        n1buf, n2buf);
            }
        } else {
            ogs_fatal("[%s] Invalid AMF-UE state", amf_ue->supi);
            ogs_assert_if_reached();
        }
        break;

    case OpenAPI_ngap_ie_type_NULL:
        /*
         * No n2InfoContainer. According to TS23.502, this means that SMF has
         * encountered an error and is rejecting the session.
         *
         * TS23.502
         * 6.3.1.7 4.3.2.2 UE Requested PDU Session Establishment
         * p100
         * 11.  ...
         * If the PDU session establishment failed anywhere between step 5
         * and step 11, then the Namf_Communication_N1N2MessageTransfer
         * request shall include the N1 SM container with a PDU Session
         * Establishment Reject message ...
         */
        if (!n1buf) {
            ogs_error("[%s] No N1 SM Content", amf_ue->supi);
            return OGS_ERROR;
        }

        ogs_error("[%d:%d] PDU session establishment reject",
                sess->psi, sess->pti);

        r = nas_5gs_send_gsm_reject(sess->ran_ue, sess,
                OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, n1buf);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        amf_sess_remove(sess);
        break;

    default:
        ogs_error("Not implemented ngapIeType[%d]", ngapIeType);
        ogs_assert_if_reached();
    }

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    if (sendmsg.http.location)
        ogs_free(sendmsg.http.location);

    return OGS_OK;
}

int amf_namf_callback_handle_sm_context_status(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int status = OGS_SBI_HTTP_STATUS_NO_CONTENT;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;

    uint8_t pdu_session_identity;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_sm_context_status_notification_t *SmContextStatusNotification;
    OpenAPI_status_info_t *StatusInfo;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    if (!recvmsg->h.resource.component[0]) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        ogs_error("No SUPI");
        goto cleanup;
    }

    amf_ue = amf_ue_find_by_supi(recvmsg->h.resource.component[0]);
    if (!amf_ue) {
        status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
        ogs_error("Cannot find SUPI [%s]", recvmsg->h.resource.component[0]);
        goto cleanup;
    }

    if (!recvmsg->h.resource.component[2]) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        ogs_error("[%s] No PDU Session Identity", amf_ue->supi);
        goto cleanup;
    }

    pdu_session_identity = atoi(recvmsg->h.resource.component[2]);
    if (pdu_session_identity == OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        ogs_error("[%s] PDU Session Identity is unassigned", amf_ue->supi);
        goto cleanup;
    }

    sess = amf_sess_find_by_psi(amf_ue, pdu_session_identity);
    if (!sess) {
        status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
        ogs_warn("[%s] Cannot find session", amf_ue->supi);
        goto cleanup;
    }

    SmContextStatusNotification = recvmsg->SmContextStatusNotification;
    if (!SmContextStatusNotification) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        ogs_error("[%s:%d] No SmContextStatusNotification",
                amf_ue->supi, sess->psi);
        goto cleanup;
    }

    StatusInfo = SmContextStatusNotification->status_info;
    if (!StatusInfo) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        ogs_error("[%s:%d] No StatusInfo", amf_ue->supi, sess->psi);
        goto cleanup;
    }

    sess->resource_status = StatusInfo->resource_status;

    /*
     * Race condition for PDU session release complete
     *  - CLIENT : /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
     *  - SERVER : /namf-callback/v1/{supi}/sm-context-status/{psi})
     *
     * If NOTIFICATION is received before the CLIENT response is received,
     * CLIENT sync is not finished. In this case, the session context
     * should not be removed.
     *
     * If NOTIFICATION comes after the CLIENT response is received,
     * sync is done. So, the session context can be removed.
     */
    ogs_info("[%s:%d][%d:%d:%s] "
            "/namf-callback/v1/{supi}/sm-context-status/{psi}",
            amf_ue->supi, sess->psi,
            sess->n1_released, sess->n2_released,
            OpenAPI_resource_status_ToString(sess->resource_status));

    if (sess->n1_released == true &&
        sess->n2_released == true &&
        sess->resource_status == OpenAPI_resource_status_RELEASED) {
        amf_nsmf_pdusession_handle_release_sm_context(
                sess, AMF_RELEASE_SM_CONTEXT_NO_STATE);
    }

cleanup:
    memset(&sendmsg, 0, sizeof(sendmsg));

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return OGS_OK;
}

int amf_namf_callback_handle_dereg_notify(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int r, state, status = OGS_SBI_HTTP_STATUS_NO_CONTENT;

    amf_ue_t *amf_ue = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_deregistration_data_t *DeregistrationData;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    if (!recvmsg->h.resource.component[0]) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        ogs_error("No SUPI");
        goto cleanup;
    }

    amf_ue = amf_ue_find_by_supi(recvmsg->h.resource.component[0]);
    if (!amf_ue) {
        status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
        ogs_error("Cannot find SUPI [%s]", recvmsg->h.resource.component[0]);
        goto cleanup;
    }

    DeregistrationData = recvmsg->DeregistrationData;
    if (!DeregistrationData) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        ogs_error("[%s] No DeregistrationData", amf_ue->supi);
        goto cleanup;
    }

    if (DeregistrationData->dereg_reason ==
            OpenAPI_deregistration_reason_NULL) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        ogs_error("[%s] No Deregistraion Reason ", amf_ue->supi);
        goto cleanup;
    }

    if (DeregistrationData->access_type != OpenAPI_access_type_3GPP_ACCESS) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        ogs_error("[%s] Deregistration access type not 3GPP", amf_ue->supi);
        goto cleanup;
    }

    ogs_info("Deregistration notify reason: %s:%s:%s",
        amf_ue->supi,
        OpenAPI_deregistration_reason_ToString(DeregistrationData->dereg_reason),
        OpenAPI_access_type_ToString(DeregistrationData->access_type));

    /*
     * TODO: do not start deregistration if UE has emergency sessions
     * 4.2.2.3.3
     * If the UE has established PDU Session associated with emergency service, the AMF shall not initiate
     * Deregistration procedure. In this case, the AMF performs network requested PDU Session Release for any PDU
     * session associated with non-emergency service as described in clause 4.3.4.
     */

    /*
     * - AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED
     * 1. UDM_UECM_DeregistrationNotification
     * 2. Deregistration request
     * 3. UDM_SDM_Unsubscribe
     * 4. UDM_UECM_Deregisration
     * 5. PDU session release request
     * 6. PDUSessionResourceReleaseCommand +
     *    PDU session release command
     * 7. PDUSessionResourceReleaseResponse
     * 8. AM_Policy_Association_Termination
     * 9.  Deregistration accept
     * 10. Signalling Connecion Release
     */
    if (CM_CONNECTED(amf_ue)) {
        r = nas_5gs_send_de_registration_request(
                amf_ue,
                DeregistrationData->dereg_reason,
                OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        state = AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED;

    } else if (CM_IDLE(amf_ue)) {
        ogs_error("Not implemented : Use Implicit De-registration");

        state = AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED;

    } else {
        ogs_fatal("Invalid State");
        ogs_assert_if_reached();
    }

    if (UDM_SDM_SUBSCRIBED(amf_ue)) {
        r = amf_ue_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NUDM_SDM, NULL,
                amf_nudm_sdm_build_subscription_delete,
                amf_ue, state, NULL);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else if (PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
        r = amf_ue_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                NULL,
                amf_npcf_am_policy_control_build_delete,
                amf_ue, state, NULL);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }

cleanup:
    memset(&sendmsg, 0, sizeof(sendmsg));

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return OGS_OK;
}

static int update_rat_res_add_one(cJSON *restriction,
                                  OpenAPI_list_t *restrictions, long index)
{
    void *restr;

    if (!cJSON_IsString(restriction)) {
        ogs_error("Invalid type of ratRestriction element");
        return OGS_ERROR;
    }

    restr = (void *) OpenAPI_rat_type_FromString(cJSON_GetStringValue(restriction));
    if (!restr) {
        ogs_error("No restr");
        return OGS_ERROR;
    }

    if (index == restrictions->count) {
        OpenAPI_list_add(restrictions, restr);
    } else if (restrictions->count < index && index <= 0) {
        OpenAPI_list_insert_prev(
            restrictions, OpenAPI_list_find(restrictions, index), restr);
    } else {
        ogs_error("Can't add RAT restriction to invalid index");
        return OGS_ERROR;
    }
    return OGS_OK;
}

static int update_rat_res_array(cJSON *json_restrictions,
                                OpenAPI_list_t *restrictions)
{
    cJSON *restriction;

    if (!cJSON_IsArray(json_restrictions)) {
        ogs_error("Invalid type of ratRestrictions");
        return OGS_ERROR;
    }

    OpenAPI_list_clear(restrictions);

    cJSON_ArrayForEach(restriction, json_restrictions) {
        if (update_rat_res_add_one(restriction, restrictions,
                                   restrictions->count) != OGS_OK) {
            return OGS_ERROR;
        }
    }
    return OGS_OK;
}

static int update_rat_res(OpenAPI_change_item_t *item_change,
                          OpenAPI_list_t *restrictions)
{
    cJSON* json = item_change->new_value->json;
    cJSON* json_restrictions;

    if (!item_change->path) {
        return OGS_ERROR;
    }

    switch (item_change->op) {
    case OpenAPI_change_type_REPLACE:
    case OpenAPI_change_type_ADD:
        if (!strcmp(item_change->path, "")) {
            if (!cJSON_IsObject(json)) {
                ogs_error("Invalid type of am-data");
            }
            json_restrictions = cJSON_GetObjectItemCaseSensitive(
                                    json, "ratRestrictions");
            if (json_restrictions) {
                return update_rat_res_array(json_restrictions, restrictions);
            } else {
                return OGS_OK;
            }
        } else if (!strcmp(item_change->path, "/ratRestrictions")) {
            return update_rat_res_array(json, restrictions);
        } else if (strstr(item_change->path, "/ratRestrictions/") ==
                   item_change->path) {
            char *index = item_change->path + strlen("/ratRestrictions/");
            long i = strcmp(index, "-") ? atol(index) : restrictions->count;

            return update_rat_res_add_one(json, restrictions, i);
        }
        return OGS_OK;

    case OpenAPI_change_type__REMOVE:
        if (!strcmp(item_change->path, "")) {
            OpenAPI_list_clear(restrictions);
            return OGS_OK;
        } else if (!strcmp(item_change->path, "/ratRestrictions")) {
            OpenAPI_list_clear(restrictions);
            return OGS_OK;
        } else if (strstr(item_change->path, "/ratRestrictions/") ==
                   item_change->path) {
            char *index = item_change->path + strlen("/ratRestrictions/");
            long i = atol(index);

            if (restrictions->count < i && i <= 0) {
            OpenAPI_list_remove(
                restrictions, OpenAPI_list_find(restrictions, i));
            } else {
                ogs_error("Can't add RAT restriction to invalid index");
                return OGS_ERROR;
            }
        }
        return OGS_OK;

    default:
        return OGS_OK;
    }

}

static int update_ambr_check_one(cJSON *obj, uint64_t *limit,
                                 bool *ambr_changed)
{
    if (!cJSON_IsString(obj)) {
        ogs_error("Invalid type of subscribedUeAmbr");
        return OGS_ERROR;
    }
    *limit = ogs_sbi_bitrate_from_string(obj->valuestring);
    *ambr_changed = true;
    return OGS_OK;
}

static int update_ambr_check_obj(cJSON *obj, ogs_bitrate_t *ambr,
                                 bool *ambr_changed)
{
    if (!cJSON_IsObject(obj)) {
        if (obj == NULL || cJSON_IsNull(obj)) {
            /* Limit of 0 means unlimited. */
            ambr->uplink = 0;
            ambr->downlink = 0;
            *ambr_changed = true;
            return OGS_OK;
        } else {
            ogs_error("Invalid type of subscribedUeAmbr");
            return OGS_ERROR;
        }
    }

    if (update_ambr_check_one(
            cJSON_GetObjectItemCaseSensitive(obj, "uplink"),
            &ambr->uplink, ambr_changed)) {
        return OGS_ERROR;
    }
    if (update_ambr_check_one(
            cJSON_GetObjectItemCaseSensitive(obj, "downlink"),
            &ambr->downlink, ambr_changed)) {
        return OGS_ERROR;
    }
    return OGS_OK;
}

static int update_ambr(OpenAPI_change_item_t *item_change,
                       ogs_bitrate_t *ambr, bool *ambr_changed)
{
    cJSON* json = item_change->new_value->json;

    if (!item_change->path) {
        return OGS_ERROR;
    }

    switch (item_change->op) {
    case OpenAPI_change_type_REPLACE:
    case OpenAPI_change_type_ADD:
        if (!strcmp(item_change->path, "")) {
            if (!cJSON_IsObject(json)) {
                ogs_error("Invalid type of am-data");
            }
            return update_ambr_check_obj(
                cJSON_GetObjectItemCaseSensitive(json, "subscribedUeAmbr"),
                ambr, ambr_changed);
        } else if (!strcmp(item_change->path, "/subscribedUeAmbr")) {
            return update_ambr_check_obj(json, ambr, ambr_changed);
        } else if (!strcmp(item_change->path, "/subscribedUeAmbr/uplink")) {
            return update_ambr_check_one(json, &ambr->uplink, ambr_changed);
        } else if (!strcmp(item_change->path, "/subscribedUeAmbr/downlink")) {
            return update_ambr_check_one(json, &ambr->downlink, ambr_changed);
        }
        return OGS_OK;


    case OpenAPI_change_type__REMOVE:
        if (!strcmp(item_change->path, "/subscribedUeAmbr")) {
            update_ambr_check_obj(NULL, ambr, ambr_changed);
        }
        return OGS_OK;
    default:
        return OGS_OK;
    }
}

int amf_namf_callback_handle_sdm_data_change_notify(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int r, state, status = OGS_SBI_HTTP_STATUS_NO_CONTENT;

    amf_ue_t *amf_ue = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_modification_notification_t *ModificationNotification;
    OpenAPI_lnode_t *node;

    char *ueid = NULL;
    char *res_name = NULL;

    bool ambr_changed = false;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    ModificationNotification = recvmsg->ModificationNotification;
    if (!ModificationNotification) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        ogs_error("No ModificationNotification");
        goto cleanup;
    }

    OpenAPI_list_for_each(ModificationNotification->notify_items, node) {
        OpenAPI_notify_item_t *item = node->data;

        char *saveptr = NULL;

        ueid = ogs_sbi_parse_uri(item->resource_id, "/", &saveptr);
        if (!ueid) {
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            ogs_error("[%s] No UeId", item->resource_id);
            goto cleanup;
        }

        amf_ue = amf_ue_find_by_supi(ueid);
        if (!amf_ue) {
            status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
            ogs_error("Cannot find SUPI [%s]", ueid);
            goto cleanup;
        }

        res_name = ogs_sbi_parse_uri(NULL, "/", &saveptr);
        if (!res_name) {
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            ogs_error("[%s] No Resource Name", item->resource_id);
            goto cleanup;
        }

        SWITCH(res_name)
        CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
            OpenAPI_lnode_t *node_ci;

            OpenAPI_list_for_each(item->changes, node_ci) {
                OpenAPI_change_item_t *change_item = node_ci->data;
                if (update_rat_res(change_item, amf_ue->rat_restrictions) ||
                        update_ambr(change_item, &amf_ue->ue_ambr,
                            &ambr_changed)) {
                    status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
                    goto cleanup;
                }
            }
            break;
        DEFAULT
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            ogs_error("Unknown Resource Name: [%s]", res_name);
            goto cleanup;
        END

        ogs_free(ueid);
        ogs_free(res_name);

        ueid = NULL;
        res_name = NULL;
    }

    if (amf_ue) {
        ran_ue_t *ran_ue = ran_ue_cycle(amf_ue->ran_ue);
        if (!ran_ue) {
            ogs_error("NG context has already been removed");
            /* ran_ue is required for amf_ue_is_rat_restricted() */

            ogs_error("Not implemented : Use Implicit De-registration");
            state = AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED;

        } else if (amf_ue_is_rat_restricted(amf_ue)) {
            /*
             * - AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED
             * 1. UDM_UECM_DeregistrationNotification
             * 2. Deregistration request
             * 3. UDM_SDM_Unsubscribe
             * 4. UDM_UECM_Deregistration
             * 5. PDU session release request
             * 6. PDUSessionResourceReleaseCommand +
             *    PDU session release command
             * 7. PDUSessionResourceReleaseResponse
             * 8. AM_Policy_Association_Termination
             * 9.  Deregistration accept
             * 10. Signalling Connection Release
             */
            r = nas_5gs_send_de_registration_request(
                    amf_ue,
                    OpenAPI_deregistration_reason_REREGISTRATION_REQUIRED, 0);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            state = AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED;

            if (UDM_SDM_SUBSCRIBED(amf_ue)) {
                r = amf_ue_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NUDM_SDM, NULL,
                        amf_nudm_sdm_build_subscription_delete,
                        amf_ue, state, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            } else if (PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                r = amf_ue_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                        NULL,
                        amf_npcf_am_policy_control_build_delete,
                        amf_ue, state, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }

        } else if (ambr_changed) {
            ogs_pkbuf_t *ngapbuf;

            ngapbuf = ngap_build_ue_context_modification_request(amf_ue);
            ogs_assert(ngapbuf);

            r = ngap_send_to_ran_ue(ran_ue, ngapbuf);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
    }

cleanup:
    if (ueid)
        ogs_free(ueid);
    if (res_name)
        ogs_free(res_name);

    memset(&sendmsg, 0, sizeof(sendmsg));

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return OGS_OK;
}

static char *amf_namf_comm_base64_encode_ue_security_capability(
        ogs_nas_ue_security_capability_t ue_security_capability);
static char *amf_namf_comm_base64_encode_5gmm_capability(amf_ue_t *amf_ue);
static OpenAPI_list_t *amf_namf_comm_encode_ue_session_context_list(
        amf_ue_t *amf_ue);
static OpenAPI_list_t *amf_namf_comm_encode_ue_mm_context_list(
        amf_ue_t *amf_ue);

int amf_namf_comm_handle_ue_context_transfer_request(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_message_t sendmsg;
    amf_ue_t *amf_ue = NULL;

    OpenAPI_ambr_t *UeAmbr = NULL;
    OpenAPI_list_t *MmContextList = NULL;
    OpenAPI_mm_context_t *MmContext = NULL;
    OpenAPI_list_t *SessionContextList = NULL;
    OpenAPI_pdu_session_context_t *PduSessionContext = NULL;
    OpenAPI_lnode_t *node = NULL;
    OpenAPI_ue_context_t UeContext;
    OpenAPI_seaf_data_t SeafData;
    OpenAPI_ng_ksi_t Ng_ksi;
    OpenAPI_key_amf_t Key_amf;
    OpenAPI_sc_type_e Tsc_type;

    OpenAPI_ue_context_transfer_rsp_data_t UeContextTransferRspData;

    ogs_sbi_nf_instance_t *pcf_nf_instance = NULL;

    char *ue_context_id = NULL;
    char *encoded_gmm_capability = NULL;
    int status = OGS_SBI_HTTP_STATUS_OK;
    char hxkamf_string[OGS_KEYSTRLEN(OGS_SHA256_DIGEST_SIZE)];
    char *strerror = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    memset(&UeContextTransferRspData, 0, sizeof(UeContextTransferRspData));
    memset(&UeContext, 0, sizeof(UeContext));
    UeContextTransferRspData.ue_context = &UeContext;

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.UeContextTransferRspData = &UeContextTransferRspData;

    ue_context_id = recvmsg->h.resource.component[1];
    if (!ue_context_id) {
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        strerror = ogs_msprintf("No UE context ID");
        goto cleanup;
    }

    amf_ue = amf_ue_find_by_ue_context_id(ue_context_id);
    if (!amf_ue) {
        status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
        strerror = ogs_msprintf("CONTEXT_NOT_FOUND");
        goto cleanup;
    }

    if (amf_ue->supi) {
        UeContext.supi = amf_ue->supi;
        if (amf_ue->auth_result !=
                OpenAPI_auth_result_AUTHENTICATION_SUCCESS) {
            UeContext.is_supi_unauth_ind = true;
            UeContext.supi_unauth_ind = amf_ue->auth_result;
        }
    }

    /* TODO UeContext.gpsi_list */

    if (amf_ue->pei) {
        UeContext.pei = amf_ue->pei;
    }

    if ((amf_ue->ue_ambr.uplink > 0) || (amf_ue->ue_ambr.downlink > 0)) {
        UeAmbr = ogs_calloc(1, sizeof(*UeAmbr));
        ogs_assert(UeAmbr);

        if (amf_ue->ue_ambr.uplink > 0)
            UeAmbr->uplink = ogs_sbi_bitrate_to_string(
                amf_ue->ue_ambr.uplink, OGS_SBI_BITRATE_KBPS);
        if (amf_ue->ue_ambr.downlink > 0)
            UeAmbr->downlink = ogs_sbi_bitrate_to_string(
                amf_ue->ue_ambr.downlink, OGS_SBI_BITRATE_KBPS);
        UeContext.sub_ue_ambr = UeAmbr;
    }

    if ((amf_ue->nas.ue.ksi != 0) && (amf_ue->nas.ue.tsc != 0)) {
        memset(&SeafData, 0, sizeof(SeafData));
        Tsc_type = (amf_ue->nas.ue.tsc == 0) ?
            OpenAPI_sc_type_NATIVE : OpenAPI_sc_type_MAPPED;

        memset(&Ng_ksi, 0, sizeof(Ng_ksi));
        SeafData.ng_ksi = &Ng_ksi;
        Ng_ksi.tsc = Tsc_type;
        Ng_ksi.ksi = (int)amf_ue->nas.ue.ksi;

        memset(&Key_amf, 0, sizeof(Key_amf));
        SeafData.key_amf = &Key_amf;
        OpenAPI_key_amf_type_e temp_key_type =
                (OpenAPI_key_amf_type_e)OpenAPI_key_amf_type_KAMF;
        Key_amf.key_type = temp_key_type;
        ogs_hex_to_ascii(amf_ue->kamf, sizeof(amf_ue->kamf),
                hxkamf_string, sizeof(hxkamf_string));
        Key_amf.key_val = hxkamf_string;
        UeContext.seaf_data = &SeafData;
    }

    encoded_gmm_capability =
        amf_namf_comm_base64_encode_5gmm_capability(amf_ue);
    UeContext._5g_mm_capability = encoded_gmm_capability;

    pcf_nf_instance = OGS_SBI_GET_NF_INSTANCE(
            amf_ue->sbi.service_type_array[
            OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL]);
    if (pcf_nf_instance) {
        UeContext.pcf_id = pcf_nf_instance->id;
    } else {
        ogs_warn("No PCF NF Instnace");
    }

    /* TODO UeContext.pcfAmPolicyUri */
    /* TODO UeContext.pcfUePolicyUri */

    MmContextList = amf_namf_comm_encode_ue_mm_context_list(amf_ue);
    UeContext.mm_context_list = MmContextList;

    if (recvmsg->UeContextTransferReqData->reason ==
            OpenAPI_transfer_reason_MOBI_REG) {
        SessionContextList =
            amf_namf_comm_encode_ue_session_context_list(amf_ue);
        UeContext.session_context_list = SessionContextList;
    }

    /* TODO ueRadioCapability */

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    if (encoded_gmm_capability)
        ogs_free(encoded_gmm_capability);

    if (UeAmbr)
        OpenAPI_ambr_free(UeAmbr);

    if (SessionContextList) {
        OpenAPI_list_for_each(SessionContextList, node) {
            PduSessionContext = node->data;
            OpenAPI_pdu_session_context_free(PduSessionContext);
        }
        OpenAPI_list_free(SessionContextList);
    }

    if (MmContextList) {
        OpenAPI_list_for_each(MmContextList, node) {
            MmContext = node->data;
            OpenAPI_mm_context_free(MmContext);
        }
        OpenAPI_list_free(MmContextList);
    }

    /*
     * Context TRANSFERRED !!!
     * So, we removed UE context.
     */
    if (amf_ue->ran_ue)
        ran_ue_remove(amf_ue->ran_ue);
    amf_ue_remove(amf_ue);

    return OGS_OK;

cleanup:
    ogs_assert(strerror);
    ogs_error("%s", strerror);

    ogs_assert(true ==
        ogs_sbi_server_send_error(stream, status, NULL, strerror, NULL, NULL));
    ogs_free(strerror);

    return OGS_ERROR;
}

static ogs_nas_5gmm_capability_t
        amf_namf_comm_base64_decode_5gmm_capability(char *encoded);
static ogs_nas_ue_security_capability_t
        amf_namf_comm_base64_decode_ue_security_capability(char *encoded);
static void amf_namf_comm_decode_ue_mm_context_list(
            amf_ue_t *amf_ue, OpenAPI_list_t *MmContextList);
static void amf_namf_comm_decode_ue_session_context_list(
            amf_ue_t *amf_ue, OpenAPI_list_t *SessionContextList);

int amf_namf_comm_handle_ue_context_transfer_response(
        ogs_sbi_message_t *recvmsg, amf_ue_t *amf_ue)
{
    OpenAPI_ue_context_t *UeContext = NULL;

    if (!recvmsg->UeContextTransferRspData) {
        ogs_error("No UeContextTransferRspData");
        return OGS_ERROR;
    }

    if (!recvmsg->UeContextTransferRspData->ue_context) {
        ogs_error("No UE context");
        return OGS_ERROR;
    }

    UeContext = recvmsg->UeContextTransferRspData->ue_context;

    if (!UeContext->supi) {
        ogs_error("No SUPI");
        return OGS_ERROR;
    }

    amf_ue_set_supi(amf_ue, UeContext->supi);
    if (!UeContext->supi_unauth_ind){
        amf_ue->auth_result = OpenAPI_auth_result_AUTHENTICATION_SUCCESS;
    }

    if (UeContext->pei) {
        if (amf_ue->pei)
            ogs_free(amf_ue->pei);
        amf_ue->pei = ogs_strdup(UeContext->pei);
    }

    if (UeContext->sub_ue_ambr) {
        amf_ue->ue_ambr.downlink =
            ogs_sbi_bitrate_from_string(UeContext->sub_ue_ambr->downlink);
        amf_ue->ue_ambr.uplink =
            ogs_sbi_bitrate_from_string(UeContext->sub_ue_ambr->uplink);
    }

    if (UeContext->seaf_data) {
        if (UeContext->seaf_data->ng_ksi->tsc != OpenAPI_sc_type_NULL) {
            amf_ue->nas.ue.tsc =
                (UeContext->seaf_data->ng_ksi->tsc ==
                 OpenAPI_sc_type_NATIVE) ? 0 : 1;
            amf_ue->nas.ue.ksi = (uint8_t)UeContext->seaf_data->ng_ksi->ksi;

            ogs_ascii_to_hex(
                UeContext->seaf_data->key_amf->key_val,
                strlen(UeContext->seaf_data->key_amf->key_val),
                amf_ue->kamf,
                sizeof(amf_ue->kamf));
        }
    }

    if (UeContext->_5g_mm_capability) {
        ogs_nas_5gmm_capability_t gmm_capability;

        gmm_capability = amf_namf_comm_base64_decode_5gmm_capability(
                    UeContext->_5g_mm_capability);
        amf_ue->gmm_capability.lte_positioning_protocol_capability =
                (bool)gmm_capability.lte_positioning_protocol_capability;
        amf_ue->gmm_capability.ho_attach = (bool)gmm_capability.ho_attach;
        amf_ue->gmm_capability.s1_mode = (bool)gmm_capability.s1_mode;
    }

    if (UeContext->pcf_id) {
        /* TODO */
    }

    /* TODO UeContext->pcfAmPolicyUri */
    /* TODO UeContext->pcfUePolicyUri */

    if (UeContext->mm_context_list)
        amf_namf_comm_decode_ue_mm_context_list(
                amf_ue, UeContext->mm_context_list);

    if (UeContext->session_context_list)
        amf_namf_comm_decode_ue_session_context_list(
                amf_ue, UeContext->session_context_list);

    /* TODO ueRadioCapability */

    return OGS_OK;
}

static char *amf_namf_comm_base64_encode_ue_security_capability(
        ogs_nas_ue_security_capability_t ue_security_capability)
{
    char *enc = NULL;
    int enc_len = 0;

    char num_of_octets =
            ue_security_capability.length +
            sizeof(ue_security_capability.length) +
            sizeof((uint8_t)OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_TYPE);
    /*
     * size [sizeof(ue_security_capability) + 1] is a sum of lengths:
     *        ue_security_capability (9 octets) +
     *        type (1 octet)
     */
    char security_octets_string[sizeof(ue_security_capability) + 1];

    /* Security guarantee */
    num_of_octets = ogs_min(
            num_of_octets, sizeof(ue_security_capability) + 1);
    enc_len = ogs_base64_encode_len(num_of_octets);

    enc = ogs_calloc(1, enc_len);
    ogs_assert(enc);

    security_octets_string[0] = (uint8_t)
        OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_TYPE;
    memcpy(security_octets_string + 1, &ue_security_capability, num_of_octets);
    ogs_base64_encode(enc , security_octets_string, num_of_octets);

    return enc;
}

static char *amf_namf_comm_base64_encode_5gmm_capability(amf_ue_t *amf_ue)
{
    ogs_nas_5gmm_capability_t nas_gmm_capability;
    int enc_len = 0;
    char *enc = NULL;

    memset(&nas_gmm_capability, 0, sizeof(nas_gmm_capability));

    /* 1 octet is mandatory, n.3 from TS 24.501 V16.12.0, 9.11.3.1 */
    nas_gmm_capability.length = 1;
    nas_gmm_capability.lte_positioning_protocol_capability =
            amf_ue->gmm_capability.lte_positioning_protocol_capability;
    nas_gmm_capability.ho_attach = amf_ue->gmm_capability.ho_attach;
    nas_gmm_capability.s1_mode = amf_ue->gmm_capability.s1_mode;

    uint8_t num_of_octets;

    char gmm_capability_octets_string[sizeof(ogs_nas_5gmm_capability_t) + 1];

    num_of_octets =
            nas_gmm_capability.length +
            sizeof(nas_gmm_capability.length) +
            sizeof((uint8_t)
                    OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_TYPE);

    /* Security guarantee. + 1 stands for 5GMM capability IEI */
    num_of_octets = ogs_min(
            num_of_octets, sizeof(ogs_nas_5gmm_capability_t) + 1);

    enc_len = ogs_base64_encode_len(num_of_octets);
    enc = ogs_calloc(1, enc_len);
    ogs_assert(enc);

    /* Fill the bytes of data */
    gmm_capability_octets_string[0] =
            (uint8_t)OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_TYPE;
    memcpy(gmm_capability_octets_string + 1,
            &nas_gmm_capability, num_of_octets);
    ogs_base64_encode(enc, gmm_capability_octets_string, num_of_octets);

    return enc;
}

static OpenAPI_list_t *amf_namf_comm_encode_ue_session_context_list(
        amf_ue_t *amf_ue)
{
    ogs_assert(amf_ue);

    amf_sess_t *sess = NULL;
    OpenAPI_list_t *PduSessionList = NULL;
    OpenAPI_pdu_session_context_t *PduSessionContext = NULL;
    OpenAPI_snssai_t *sNSSAI = NULL;

    PduSessionList = OpenAPI_list_create();
    ogs_assert(PduSessionList);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        PduSessionContext = ogs_calloc(1, sizeof(*PduSessionContext));
        ogs_assert(PduSessionContext);

        sNSSAI = ogs_calloc(1, sizeof(*sNSSAI));
        ogs_assert(sNSSAI);

        PduSessionContext->pdu_session_id = sess->psi;
        ogs_assert(sess->sm_context.resource_uri);
        PduSessionContext->sm_context_ref =
            ogs_strdup(sess->sm_context.resource_uri);

        sNSSAI->sst = sess->s_nssai.sst;
        sNSSAI->sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
        PduSessionContext->s_nssai = sNSSAI;

        ogs_assert(sess->dnn);
        PduSessionContext->dnn = ogs_strdup(sess->dnn);
        PduSessionContext->access_type =
            (OpenAPI_access_type_e)amf_ue->nas.access_type;

        OpenAPI_list_add(PduSessionList, PduSessionContext);
    }

    return PduSessionList;
}

static OpenAPI_list_t *amf_namf_comm_encode_ue_mm_context_list(amf_ue_t *amf_ue)
{
    OpenAPI_list_t *MmContextList = NULL;
    OpenAPI_mm_context_t *MmContext = NULL;

    int i;

    ogs_assert(amf_ue);

    MmContextList = OpenAPI_list_create();
    ogs_assert(MmContextList);

    MmContext = ogs_malloc(sizeof(*MmContext));
    ogs_assert(MmContext);
    memset(MmContext, 0, sizeof(*MmContext));

    MmContext->access_type = (OpenAPI_access_type_e)amf_ue->nas.access_type;

    if ((OpenAPI_ciphering_algorithm_e)amf_ue->selected_enc_algorithm &&
        (OpenAPI_integrity_algorithm_e)amf_ue->selected_int_algorithm) {

        OpenAPI_nas_security_mode_t *NasSecurityMode;

        NasSecurityMode = ogs_calloc(1, sizeof(*NasSecurityMode));
        ogs_assert(NasSecurityMode);

        NasSecurityMode->ciphering_algorithm =
                (OpenAPI_ciphering_algorithm_e)amf_ue->selected_enc_algorithm;
        NasSecurityMode->integrity_algorithm =
                (OpenAPI_integrity_algorithm_e)amf_ue->selected_int_algorithm;

        MmContext->nas_security_mode = NasSecurityMode;
    }

    if (amf_ue->dl_count > 0) {
        MmContext->is_nas_downlink_count = true;
        MmContext->nas_downlink_count = amf_ue->dl_count;
    }

    if (amf_ue->ul_count.i32 > 0) {
        MmContext->is_nas_uplink_count = true;
        MmContext->nas_uplink_count = amf_ue->ul_count.i32;
    }

    if (amf_ue->ue_security_capability.length > 0) {
        MmContext->ue_security_capability =
                amf_namf_comm_base64_encode_ue_security_capability(
                amf_ue->ue_security_capability);
    }

    if (amf_ue->allowed_nssai.num_of_s_nssai) {

        OpenAPI_list_t *AllowedNssaiList;

        /* This IE shall be present if the source AMF and the target AMF are
        *  in the same PLMN and if available. When present, this IE shall
        * contain the allowed NSSAI for the access type.
        */
        AllowedNssaiList = OpenAPI_list_create();

        ogs_assert(AllowedNssaiList);

        for (i = 0; i < amf_ue->allowed_nssai.num_of_s_nssai; i++) {
            OpenAPI_snssai_t *AllowedNssai;

            AllowedNssai = ogs_calloc(1, sizeof(*AllowedNssai));
            ogs_assert(AllowedNssai);

            AllowedNssai->sst = amf_ue->allowed_nssai.s_nssai[i].sst;
            AllowedNssai->sd = ogs_s_nssai_sd_to_string(
                    amf_ue->allowed_nssai.s_nssai[i].sd);

            OpenAPI_list_add(AllowedNssaiList, AllowedNssai);
        }

        MmContext->allowed_nssai = AllowedNssaiList;
    }

    OpenAPI_list_add(MmContextList, MmContext);

    return MmContextList;
}

static ogs_nas_5gmm_capability_t
        amf_namf_comm_base64_decode_5gmm_capability(char *encoded)
{
    ogs_nas_5gmm_capability_t gmm_capability;
    char *gmm_capability_octets_string = NULL;
    uint8_t gmm_capability_iei = 0;
    int len;

    memset(&gmm_capability, 0, sizeof(gmm_capability));
    gmm_capability_octets_string =
            (char*) ogs_calloc(sizeof(gmm_capability) + 1, sizeof(char));
    ogs_assert(gmm_capability_octets_string);

    len = ogs_base64_decode(gmm_capability_octets_string, encoded);

    if (len == 0)
        ogs_error("Gmm capability not decoded");

    ogs_assert(sizeof(gmm_capability_octets_string) <=
            sizeof(gmm_capability) + 1);

    gmm_capability_iei = // not copied anywhere for now
            gmm_capability_octets_string[0];
    if (gmm_capability_iei !=
            OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_TYPE) {
        ogs_error("Type of 5GMM capability IEI is incorrect");
    }
    memcpy(&gmm_capability,
            gmm_capability_octets_string + 1,
            sizeof(gmm_capability));
    if (gmm_capability_octets_string) {
        ogs_free(gmm_capability_octets_string);
    }

    return gmm_capability;
}

static ogs_nas_ue_security_capability_t
        amf_namf_comm_base64_decode_ue_security_capability(char *encoded)
{
    ogs_nas_ue_security_capability_t ue_security_capability;
    char *ue_security_capability_octets_string = NULL;
    uint8_t ue_security_capability_iei = 0;

    memset(&ue_security_capability, 0, sizeof(ue_security_capability));
    ue_security_capability_octets_string =
            (char*) ogs_calloc(sizeof(ue_security_capability), sizeof(char));
    ogs_assert(ue_security_capability_octets_string);

    ogs_base64_decode(ue_security_capability_octets_string, encoded);

    ogs_assert(sizeof(ue_security_capability_octets_string) <=
            sizeof(ogs_nas_ue_security_capability_t) + 1);

    ue_security_capability_iei = // not copied anywhere for now
            ue_security_capability_octets_string[0];
    if (ue_security_capability_iei !=
            OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_TYPE) {
        ogs_error("UE security capability IEI is incorrect");
    }

    memcpy(&ue_security_capability, ue_security_capability_octets_string + 1,
            sizeof(ue_security_capability));

    if (ue_security_capability_octets_string) {
        ogs_free(ue_security_capability_octets_string);
    }

    return ue_security_capability;
}

static void amf_namf_comm_decode_ue_mm_context_list(
            amf_ue_t *amf_ue, OpenAPI_list_t *MmContextList) {

    OpenAPI_lnode_t *node = NULL;

    OpenAPI_list_for_each(MmContextList, node) {

        OpenAPI_mm_context_t *MmContext = NULL;
        OpenAPI_list_t *AllowedNssaiList = NULL;
        OpenAPI_lnode_t *node1 = NULL;
        OpenAPI_list_t *NssaiMappingList = NULL;
        int num_of_s_nssai = 0;
        int num_of_nssai_mapping = 0;

        MmContext = node->data;

        AllowedNssaiList = MmContext->allowed_nssai;
        NssaiMappingList = MmContext->nssai_mapping_list;

        OpenAPI_list_for_each(AllowedNssaiList, node1) {
            OpenAPI_snssai_t *AllowedNssai = node1->data;

            ogs_assert(num_of_s_nssai < OGS_MAX_NUM_OF_SLICE);

            amf_ue->allowed_nssai.s_nssai[num_of_s_nssai].sst =
                    (uint8_t)AllowedNssai->sst;
            amf_ue->allowed_nssai.s_nssai[num_of_s_nssai].sd =
                    ogs_s_nssai_sd_from_string(AllowedNssai->sd);

            num_of_s_nssai++;
            amf_ue->allowed_nssai.num_of_s_nssai = num_of_s_nssai;
        }

        OpenAPI_list_for_each(NssaiMappingList, node1) {
            OpenAPI_nssai_mapping_t *NssaiMapping = node1->data;
            OpenAPI_snssai_t *HSnssai = NssaiMapping->h_snssai;

            ogs_assert(num_of_nssai_mapping < OGS_MAX_NUM_OF_SLICE);

            amf_ue->allowed_nssai.s_nssai[num_of_nssai_mapping].
                    mapped_hplmn_sst = HSnssai->sst;
            amf_ue->allowed_nssai.s_nssai[num_of_nssai_mapping].
                    mapped_hplmn_sd = ogs_s_nssai_sd_from_string(HSnssai->sd);

            num_of_nssai_mapping++;
        }

        if (MmContext->ue_security_capability) {
            amf_ue->ue_security_capability =
                    amf_namf_comm_base64_decode_ue_security_capability(
                    MmContext->ue_security_capability);
        }
    }
}

static void amf_namf_comm_decode_ue_session_context_list(
            amf_ue_t *amf_ue, OpenAPI_list_t *SessionContextList)
{
    OpenAPI_lnode_t *node = NULL;

    OpenAPI_list_for_each(SessionContextList, node) {
        OpenAPI_pdu_session_context_t *PduSessionContext;
        PduSessionContext = node->data;
        amf_sess_t *sess = NULL;

        int rv;
        ogs_sbi_message_t message;
        ogs_sbi_header_t header;

        bool rc;
        ogs_sbi_client_t *client = NULL;
        OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
        char *fqdn = NULL;
        uint16_t fqdn_port = 0;
        ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

        if (!PduSessionContext->sm_context_ref) {
            ogs_error("No smContextRef [PSI:%d]",
                    PduSessionContext->pdu_session_id);
            continue;
        }

        if (!PduSessionContext->s_nssai) {
            ogs_error("No sNSSI [PSI:%d]", PduSessionContext->pdu_session_id);
            continue;
        }

        if (!PduSessionContext->dnn) {
            ogs_error("No DNN [PSI:%d]", PduSessionContext->pdu_session_id);
            continue;
        }

        if (!PduSessionContext->access_type) {
            ogs_error("No accessType [PSI:%d]",
                    PduSessionContext->pdu_session_id);
            continue;
        }

        memset(&header, 0, sizeof(header));
        header.uri = PduSessionContext->sm_context_ref;

        rv = ogs_sbi_parse_header(&message, &header);
        if (rv != OGS_OK) {
            ogs_error("[%d] Cannot parse sm_context_ref [%s]",
                    PduSessionContext->pdu_session_id,
                    PduSessionContext->sm_context_ref);
            continue;
        }

        if (!message.h.resource.component[1]) {
            ogs_error("[%d] No SmContextRef [%s]",
                    PduSessionContext->pdu_session_id,
                    PduSessionContext->sm_context_ref);

            ogs_sbi_header_free(&header);
            continue;
        }

        sess = amf_sess_add(amf_ue, PduSessionContext->pdu_session_id);
        ogs_assert(sess);

        rc = ogs_sbi_getaddr_from_uri(
                &scheme, &fqdn, &fqdn_port, &addr, &addr6, header.uri);
        if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
            ogs_error("[%s:%d] Invalid URI [%s]",
                    amf_ue->supi, sess->psi, header.uri);

            ogs_sbi_header_free(&header);
            continue;
        }

        client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            ogs_debug("[%s:%d] ogs_sbi_client_add()", amf_ue->supi, sess->psi);
            client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
            if (!client) {
                ogs_error("[%s:%d] ogs_sbi_client_add() failed",
                        amf_ue->supi, sess->psi);

                ogs_sbi_header_free(&header);

                ogs_free(fqdn);
                ogs_freeaddrinfo(addr);
                ogs_freeaddrinfo(addr6);

                continue;
            }
        }
        OGS_SBI_SETUP_CLIENT(&sess->sm_context, client);

        ogs_free(fqdn);
        ogs_freeaddrinfo(addr);
        ogs_freeaddrinfo(addr6);

        sess->sm_context.resource_uri =
            ogs_strdup(PduSessionContext->sm_context_ref);
        sess->sm_context.ref =
            ogs_strdup(message.h.resource.component[1]);

        memset(&sess->s_nssai, 0, sizeof(sess->s_nssai));

        sess->s_nssai.sst = PduSessionContext->s_nssai->sst;
        sess->s_nssai.sd = ogs_s_nssai_sd_from_string(
                PduSessionContext->s_nssai->sd);

        sess->dnn = ogs_strdup(PduSessionContext->dnn);
        amf_ue->nas.access_type = (int)PduSessionContext->access_type;

        ogs_sbi_header_free(&header);
    }
}
