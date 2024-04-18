/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "nsmf-handler.h"
#include "nas-path.h"
#include "ngap-path.h"
#include "sbi-path.h"

#include "gmm-build.h"

int amf_nsmf_pdusession_handle_create_sm_context(
        amf_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    int rv, r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(recvmsg);
    ogs_assert(sess);
    sess = amf_sess_cycle(sess);
    if (!sess) {
        ogs_error("Session has already been removed");
        return OGS_ERROR;
    }

    amf_ue = amf_ue_cycle(sess->amf_ue);
    if (!amf_ue) {
        ogs_error("UE(amf_ue) Context has already been removed");
        return OGS_ERROR;
    }

    ran_ue = ran_ue_cycle(sess->ran_ue);
    if (!ran_ue) {
        ogs_error("[%s] RAN-NG Context has already been removed", amf_ue->supi);
        return OGS_ERROR;
    }

    if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_CREATED) {
        ogs_sbi_message_t message;
        ogs_sbi_header_t header;

        bool rc;
        ogs_sbi_client_t *client = NULL;
        OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
        char *fqdn = NULL;
        uint16_t fqdn_port = 0;
        ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

        if (!recvmsg->http.location) {
            ogs_error("[%d:%d] No http.location", sess->psi, sess->pti);
            r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                    OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                    AMF_NAS_BACKOFF_TIME);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return OGS_ERROR;
        }

        memset(&header, 0, sizeof(header));
        header.uri = recvmsg->http.location;

        rv = ogs_sbi_parse_header(&message, &header);
        if (rv != OGS_OK) {
            ogs_error("[%d:%d] Cannot parse http.location [%s]",
                    sess->psi, sess->pti, recvmsg->http.location);
            r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                    OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                    AMF_NAS_BACKOFF_TIME);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return OGS_ERROR;
        }

        if (!message.h.resource.component[1]) {
            ogs_error("[%d:%d] No SmContextRef [%s]",
                    sess->psi, sess->pti, recvmsg->http.location);

            ogs_sbi_header_free(&header);
            r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                    OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                    AMF_NAS_BACKOFF_TIME);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return OGS_ERROR;
        }

        rc = ogs_sbi_getaddr_from_uri(
                &scheme, &fqdn, &fqdn_port, &addr, &addr6, header.uri);
        if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
            ogs_error("[%s:%d] Invalid URI [%s]",
                    amf_ue->supi, sess->psi, header.uri);

            ogs_sbi_header_free(&header);
            r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                    OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                    AMF_NAS_BACKOFF_TIME);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return OGS_ERROR;
        }

        client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            ogs_debug("[%s:%d] ogs_sbi_client_add()", amf_ue->supi, sess->psi);
            client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
            if (!client) {
                ogs_error("[%s:%d] ogs_sbi_client_add() failed",
                        amf_ue->supi, sess->psi);

                ogs_sbi_header_free(&header);
                r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                        OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                        AMF_NAS_BACKOFF_TIME);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

                ogs_free(fqdn);
                ogs_freeaddrinfo(addr);
                ogs_freeaddrinfo(addr6);

                return OGS_ERROR;
            }
        }
        OGS_SBI_SETUP_CLIENT(&sess->sm_context, client);

        ogs_free(fqdn);
        ogs_freeaddrinfo(addr);
        ogs_freeaddrinfo(addr6);

        STORE_SESSION_CONTEXT(
                sess, header.uri, message.h.resource.component[1]);

        ogs_sbi_header_free(&header);

        if (sess->pdu_session_establishment_accept) {
            /*
             * [1-SERVER] /namf-comm/v1/ue-contexts/{supi}/n1-n2-messages
             * [2-CLIENT] /nsmf-pdusession/v1/sm-contexts
             *
             * If [1-SERVER] arrives before [2-CLIENT],
             * there is no sm-context-ref. The PDU session establishment accept
             * stored in [1-SERVER] is now trasnmitted to gNB.
             */
            r = ngap_send_to_ran_ue(ran_ue,
                    sess->pdu_session_establishment_accept);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            sess->pdu_session_establishment_accept = NULL;

            if (rv != OGS_OK) {
                ogs_error("[%d:%d] nas_5gs_send_to_gnb() failed",
                        sess->psi, sess->pti);

                r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                        OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                        AMF_NAS_BACKOFF_TIME);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

                return OGS_ERROR;
            }
        }

    } else {
        OpenAPI_sm_context_create_error_t *SmContextCreateError = NULL;
        OpenAPI_ref_to_binary_data_t *n1SmMsg = NULL;
        ogs_pkbuf_t *n1smbuf = NULL;

        SmContextCreateError = recvmsg->SmContextCreateError;
        if (!SmContextCreateError) {
            ogs_error("[%d:%d] No SmContextCreateError", sess->psi, sess->pti);
            r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                    OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                    AMF_NAS_BACKOFF_TIME);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return OGS_ERROR;
        }
        if (!SmContextCreateError->error) {
            ogs_error("[%d:%d] No Error", sess->psi, sess->pti);
            r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                    OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                    AMF_NAS_BACKOFF_TIME);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return OGS_ERROR;
        }

        n1SmMsg = SmContextCreateError->n1_sm_msg;
        if (n1SmMsg && n1SmMsg->content_id) {
            n1smbuf = ogs_sbi_find_part_by_content_id(
                    recvmsg, n1SmMsg->content_id);
            if (n1smbuf) {
                /*
                 * NOTE : The pkbuf created in the SBI message will be removed
                 *        from ogs_sbi_message_free(), so it must be copied.
                 */
                ogs_error("[%d:%d] PDU session establishment reject",
                        sess->psi, sess->pti);

                n1smbuf = ogs_pkbuf_copy(n1smbuf);
                ogs_assert(n1smbuf);
                r = nas_5gs_send_gsm_reject(ran_ue, sess,
                        OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, n1smbuf);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

                return OGS_ERROR;
            }
        }

        ogs_error("[%d:%d] 5GMM was not forwarded", sess->psi, sess->pti);
        r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                AMF_NAS_BACKOFF_TIME);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        return OGS_ERROR;
    }

    return OGS_OK;
}

int amf_nsmf_pdusession_handle_update_sm_context(
        amf_sess_t *sess, int state, ogs_sbi_message_t *recvmsg)
{
    int r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(recvmsg);
    ogs_assert(sess);
    sess = amf_sess_cycle(sess);
    if (!sess) {
        ogs_error("Session has already been removed");
        return OGS_ERROR;
    }

    amf_ue = amf_ue_cycle(sess->amf_ue);
    if (!amf_ue) {
        ogs_error("UE(amf_ue) Context has already been removed");
        return OGS_ERROR;
    }

    ran_ue = ran_ue_cycle(sess->ran_ue);

    if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT ||
        recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {

        OpenAPI_sm_context_updated_data_t *SmContextUpdatedData = NULL;
        OpenAPI_ref_to_binary_data_t *n1SmMsg = NULL;
        OpenAPI_ref_to_binary_data_t *n2SmInfo = NULL;
        ogs_pkbuf_t *n1smbuf = NULL;
        ogs_pkbuf_t *n2smbuf = NULL;

        if (recvmsg->SmContextUpdatedData &&
            recvmsg->SmContextUpdatedData->n2_sm_info) {

            SmContextUpdatedData = recvmsg->SmContextUpdatedData;
            ogs_assert(SmContextUpdatedData);
            n2SmInfo = SmContextUpdatedData->n2_sm_info;
            ogs_assert(n2SmInfo);

            if (n2SmInfo->content_id) {
                n2smbuf = ogs_sbi_find_part_by_content_id(
                        recvmsg, n2SmInfo->content_id);
            }

            n1SmMsg = SmContextUpdatedData->n1_sm_msg;
            if (n1SmMsg && n1SmMsg->content_id) {
                n1smbuf = ogs_sbi_find_part_by_content_id(
                        recvmsg, n1SmMsg->content_id);
            }

            switch (SmContextUpdatedData->n2_sm_info_type) {
            case OpenAPI_n2_sm_info_type_PDU_RES_SETUP_REQ:
                if (!n2smbuf) {
                    ogs_error("[%s:%d] No N2 SM Content",
                            amf_ue->supi, sess->psi);
                    nas_5gs_send_gmm_reject(ran_ue, amf_ue,
                            OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED);

                    AMF_SESS_CLEAR(sess);

                    return OGS_ERROR;
                }

                if (state == AMF_UPDATE_SM_CONTEXT_REGISTRATION_REQUEST) {
                    AMF_SESS_STORE_N2_TRANSFER(
                            sess, pdu_session_resource_setup_request,
                            ogs_pkbuf_copy(n2smbuf));

                    if (AMF_SESSION_SYNC_DONE(amf_ue,
                            AMF_RELEASE_SM_CONTEXT_REGISTRATION_ACCEPT) &&
                        AMF_SESSION_SYNC_DONE(amf_ue,
                            AMF_UPDATE_SM_CONTEXT_REGISTRATION_REQUEST)) {
                        if (ran_ue) {
                            if (!PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                                r = amf_ue_sbi_discover_and_send(
                                        OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL,
                                        NULL,
                                        amf_npcf_am_policy_control_build_create,
                                        amf_ue, 0, NULL);
                                ogs_expect(r == OGS_OK);
                                ogs_assert(r != OGS_ERROR);
                            } else {
                                CLEAR_AMF_UE_TIMER(amf_ue->t3550);
                                r = nas_5gs_send_registration_accept(amf_ue);
                                ogs_expect(r == OGS_OK);
                                ogs_assert(r != OGS_ERROR);

                                AMF_UE_CLEAR_N2_TRANSFER(amf_ue,
                                        pdu_session_resource_setup_request);
                            }
                        } else {
                            ogs_warn("[%s] RAN-NG Context has already "
                                    "been removed", amf_ue->supi);
                        }
                    }
                } else if (state == AMF_UPDATE_SM_CONTEXT_SERVICE_REQUEST) {
                    AMF_SESS_STORE_N2_TRANSFER(
                            sess, pdu_session_resource_setup_request,
                            ogs_pkbuf_copy(n2smbuf));

                    if (AMF_SESSION_SYNC_DONE(amf_ue,
                                AMF_RELEASE_SM_CONTEXT_SERVICE_ACCEPT) &&
                        AMF_SESSION_SYNC_DONE(amf_ue,
                                AMF_UPDATE_SM_CONTEXT_SERVICE_REQUEST)) {
                        r = nas_5gs_send_service_accept(amf_ue);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                        AMF_UE_CLEAR_N2_TRANSFER(
                                amf_ue, pdu_session_resource_setup_request);
                    }
                } else if (state == AMF_UPDATE_SM_CONTEXT_HANDOVER_REQUIRED) {
                    AMF_SESS_STORE_N2_TRANSFER(
                            sess, handover_request, ogs_pkbuf_copy(n2smbuf));

                    if (AMF_SESSION_SYNC_DONE(amf_ue,
                                AMF_UPDATE_SM_CONTEXT_HANDOVER_REQUIRED)) {
                        r = ngap_send_handover_request(amf_ue);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                        AMF_UE_CLEAR_N2_TRANSFER(amf_ue, handover_request);
                    }
                } else {
                    ogs_error("Invalid STATE[%d]", state);
                    ogs_assert_if_reached();
                }
                break;

            case OpenAPI_n2_sm_info_type_PDU_RES_MOD_REQ:
                if (!n1smbuf) {
                    ogs_error("[%s:%d] No N1 SM Content [%s]",
                            amf_ue->supi, sess->psi, n1SmMsg->content_id);
                    r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                            OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                            AMF_NAS_BACKOFF_TIME);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    return OGS_ERROR;
                }

                if (!n2smbuf) {
                    ogs_error("[%s:%d] No N2 SM Content",
                            amf_ue->supi, sess->psi);
                    r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                            OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                            AMF_NAS_BACKOFF_TIME);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    return OGS_ERROR;
                }

                /*
                 * NOTE : The pkbuf created in the SBI message will be removed
                 *        from ogs_sbi_message_free(), so it must be copied.
                 */
                n1smbuf = ogs_pkbuf_copy(n1smbuf);
                ogs_assert(n1smbuf);

                n2smbuf = ogs_pkbuf_copy(n2smbuf);
                ogs_assert(n2smbuf);

                r = nas_send_pdu_session_modification_command(
                        sess, n1smbuf, n2smbuf);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                break;


            case OpenAPI_n2_sm_info_type_PDU_RES_REL_CMD:
                if (!n1smbuf) {
                    ogs_error("[%s:%d] No N1 SM Content [%s]",
                            amf_ue->supi, sess->psi, n1SmMsg->content_id);
                    r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                            OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                            AMF_NAS_BACKOFF_TIME);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    return OGS_ERROR;
                }

                if (!n2smbuf) {
                    ogs_error("[%s:%d] No N2 SM Content",
                            amf_ue->supi, sess->psi);
                    r = nas_5gs_send_back_gsm_message(ran_ue, sess,
                            OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                            AMF_NAS_BACKOFF_TIME);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    return OGS_ERROR;
                }

                /*
                 * NOTE : The pkbuf created in the SBI message will be removed
                 *        from ogs_sbi_message_free(), so it must be copied.
                 */
                n1smbuf = ogs_pkbuf_copy(n1smbuf);
                ogs_assert(n1smbuf);

                n2smbuf = ogs_pkbuf_copy(n2smbuf);
                ogs_assert(n2smbuf);

                r = nas_send_pdu_session_release_command(
                        sess, n1smbuf, n2smbuf);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                break;

            case OpenAPI_n2_sm_info_type_PATH_SWITCH_REQ_ACK:
                if (!n2smbuf) {
                    ogs_error("[%s:%d] No N2 SM Content",
                            amf_ue->supi, sess->psi);
                    r = ngap_send_error_indication2(ran_ue,
                            NGAP_Cause_PR_protocol,
                            NGAP_CauseProtocol_semantic_error);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    return OGS_ERROR;
                }

                AMF_SESS_STORE_N2_TRANSFER(
                        sess, path_switch_request_ack,
                        ogs_pkbuf_copy(n2smbuf));

                if (AMF_SESSION_SYNC_DONE(amf_ue, state)) {
                    r = ngap_send_path_switch_ack(sess);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    AMF_UE_CLEAR_N2_TRANSFER(amf_ue, path_switch_request_ack);
                }
                break;

            case OpenAPI_n2_sm_info_type_HANDOVER_CMD:
                if (!n2smbuf) {
                    ogs_error("[%s:%d] No N2 SM Content",
                            amf_ue->supi, sess->psi);
                    r = ngap_send_error_indication2(ran_ue,
                            NGAP_Cause_PR_protocol,
                            NGAP_CauseProtocol_semantic_error);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    return OGS_ERROR;
                }

                AMF_SESS_STORE_N2_TRANSFER(
                        sess, handover_command, ogs_pkbuf_copy(n2smbuf));

                if (AMF_SESSION_SYNC_DONE(amf_ue, state)) {
                    r = ngap_send_handover_command(amf_ue);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                    AMF_UE_CLEAR_N2_TRANSFER(amf_ue, handover_command);
                }
                break;

            default:
                ogs_error("Not implemented [%d]",
                        SmContextUpdatedData->n2_sm_info_type);
                r = ngap_send_error_indication2(ran_ue,
                        NGAP_Cause_PR_protocol,
                        NGAP_CauseProtocol_semantic_error);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }

        } else {
            SmContextUpdatedData = recvmsg->SmContextUpdatedData;

            if (state == AMF_UPDATE_SM_CONTEXT_ACTIVATED) {
                /*
                 * 1. PDUSessionResourceSetupResponse
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 * 3. PFCP Session Modifcation Request (OuterHeaderCreation)
                 * 4. PFCP Session Modifcation Response
                 */

                /*
                 * 1. InitialContextSetupResponse
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 * 3. PFCP Session Modifcation Request (Apply: FORWARD)
                 * 4. PFCP Session Modifcation Response
                 */
            } else if (state == AMF_UPDATE_SM_CONTEXT_SETUP_FAIL) {
                /*
                 * TS23.502
                 * 4.2.3 Service Request procedures
                 * 4.2.3.2 UE Triggered Service Request
                 *
                 * 15. ...
                 * If a PDU Session is rejected by the serving NG-RAN
                 * with an indication that the PDU Session was rejected
                 * because User Plane Security Enforcement is not supported
                 * in the serving NG-RAN and the User Plane Enforcement Policy
                 * indicates "Required" as described in clause 5.10.3
                 * of TS 23.501 [2], the SMF shall trigger the release
                 * of this PDU Session.
                 *
                 * In all other cases of PDU Session rejection,
                 * the SMF can decide whether to release the PDU Session
                 * or to deactivate the UP connection of this PDU Session.
                 *
                 *
                 * TS29.502
                 *
                 * 5.2.2.3.2
                 * Activation and Deactivation of the User Plane connection
                 * of a PDU session
                 * 5.2.2.3.2.2
                 * Activation of User Plane connectivity of a PDU session
                 *
                 * 3. ...
                 * N2 SM information received from the 5G-AN
                 * (see PDU Session Resource Setup Unsuccessful Transfer IE
                 * in clause 9.3.4.16 of 3GPP TS 38.413 [9]),
                 * including the Cause of the failure, if resources failed
                 * to be established for the PDU session.
                 *
                 * Upon receipt of this request, the SMF shall:
                 * - consider that the activation of the User Plane connection
                 *   has failed and set the upCnxState attribute to DEACTIVATED"
                 *   otherwise.
                 *
                 * 1. PDUSessionResourceSetupResponse(Unsuccessful)
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 * 3. PFCP Session Modifcation Request (Apply:Buff & NOCP)
                 * 4. PFCP Session Modifcation Response
                 * 5. UEContextReleaseCommand
                 * 6. UEContextReleaseComplete
                 */
                ogs_warn("PDUSessionResourceSetupResponse(Unsuccessful)");
                ogs_assert(amf_ue->deactivation.group);

                r = ngap_send_amf_ue_context_release_command(amf_ue,
                        amf_ue->deactivation.group,
                        amf_ue->deactivation.cause,
                        NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK, 0);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

            } else if (state == AMF_UPDATE_SM_CONTEXT_MODIFIED) {
                /*
                 * 1. PDUSessionResourceModifyResponse
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 * 3. PFCP Session Modifcation Request
                 * 4. PFCP Session Modifcation Response
                 */

                /*
                 * 1. PDU session release request
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 */

                /*
                 * 1. PDU session modification complete
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 */

            } else if (state == AMF_UPDATE_SM_CONTEXT_DEACTIVATED) {
                /*
                 * 1. UEContextReleaseRequest
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 * 3. PFCP Session Modifcation Request (Apply:Buff & NOCP)
                 * 4. PFCP Session Modifcation Response
                 * 5. UEContextReleaseCommand
                 * 6. UEContextReleaseComplete
                 */

                if (AMF_SESSION_SYNC_DONE(amf_ue, state)) {
                    ogs_assert(amf_ue->deactivation.group);

                    r = ngap_send_amf_ue_context_release_command(amf_ue,
                            amf_ue->deactivation.group,
                            amf_ue->deactivation.cause,
                            NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK, 0);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                }

            } else if (state == AMF_UPDATE_SM_CONTEXT_REGISTRATION_REQUEST) {

                /* Not reached here */
                ogs_assert_if_reached();

            } else if (state == AMF_UPDATE_SM_CONTEXT_SERVICE_REQUEST) {

                /* Not reached here */
                ogs_assert_if_reached();

            } else if (state == AMF_UPDATE_SM_CONTEXT_N2_RELEASED) {

                /*
                 * 1. PDUSessionResourceReleaseResponse
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 */
                ogs_info("[%s:%d] Receive Update SM context(N2-RELEASED)",
                        amf_ue->supi, sess->psi);

                sess->n2_released = true;

            } else if (state == AMF_UPDATE_SM_CONTEXT_N1_RELEASED) {
                /*
                 * 1. PDU session release complete
                 *    CLEAR_SM_CONTEXT_REF(sess) in gmm-handler.c
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 */

                ogs_info("[%s:%d] Receive Update SM context(N1-RELEASED)",
                        amf_ue->supi, sess->psi);

                sess->n1_released = true;

            } else if (state ==
                    AMF_UPDATE_SM_CONTEXT_DUPLICATED_PDU_SESSION_ID) {
                /*
                 * 1. PDU session establishment request
                 *    (Duplicated PDU Session ID)
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 * 3. POST /nsmf-pdusession/v1/sm-contexts
                 */

                ogs_warn("[%s:%d] Receive Update SM context"
                        "(DUPLICATED_PDU_SESSION_ID)", amf_ue->supi, sess->psi);

                if (ran_ue) {
                    r = amf_sess_sbi_discover_and_send(
                            OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                            amf_nsmf_pdusession_build_create_sm_context,
                            ran_ue, sess, AMF_CREATE_SM_CONTEXT_NO_STATE, NULL);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else {
                    ogs_warn("[%s] RAN-NG Context has already been removed",
                            amf_ue->supi);
                }

            } else if (state == AMF_UPDATE_SM_CONTEXT_PATH_SWITCH_REQUEST) {

                /* Not reached here */
                ogs_assert_if_reached();

            } else if (state == AMF_UPDATE_SM_CONTEXT_HANDOVER_REQUIRED) {

                /* Not reached here */
                ogs_assert_if_reached();

            } else if (state == AMF_UPDATE_SM_CONTEXT_HANDOVER_REQ_ACK) {

                /* Not reached here */
                ogs_assert_if_reached();

            } else if (state == AMF_UPDATE_SM_CONTEXT_HANDOVER_CANCEL) {

                if (AMF_SESSION_SYNC_DONE(amf_ue, state)) {
                    ran_ue_t *source_ue = NULL, *target_ue = NULL;

                    source_ue = sess->ran_ue;
                    ogs_assert(source_ue);
                    target_ue = ran_ue_cycle(source_ue->target_ue);
                    if (target_ue) {
                        r = ngap_send_ran_ue_context_release_command(
                                target_ue,
                                NGAP_Cause_PR_radioNetwork,
                                NGAP_CauseRadioNetwork_handover_cancelled,
                                NGAP_UE_CTX_REL_NG_HANDOVER_CANCEL, 0);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                    } else {
                        ogs_warn("[%s] RAN-NG Context has already been removed",
                                amf_ue->supi);
                    }
                }

            } else if (state == AMF_UPDATE_SM_CONTEXT_HANDOVER_NOTIFY) {

                /* Nothing to do */

            } else if (state == AMF_REMOVE_S1_CONTEXT_BY_LO_CONNREFUSED) {
                if (AMF_SESSION_SYNC_DONE(amf_ue, state)) {

                    amf_ue_deassociate(amf_ue);

                    if (ran_ue) {
                        ogs_debug("    SUPI[%s]", amf_ue->supi);
                        ran_ue_remove(ran_ue);
                    } else {
                        ogs_warn("[%s] RAN-NG Context has already been removed",
                                amf_ue->supi);
                    }

        /*
         * When AMF release the NAS signalling connection,
         * ran_ue context is removed by ran_ue_remove() and
         * amf_ue/ran_ue is de-associated by amf_ue_deassociate().
         *
         * In this case, implicit deregistration is attempted
         * by the mobile reachable timer according to the standard document,
         * and amf_ue will be removed by amf_ue_remove().
         *
         * TS 24.501
         * 5.3.7 Handling of the periodic registration update timer and
         *
         * Start AMF_TIMER_MOBILE_REACHABLE
         * mobile reachable timer
         * The network supervises the periodic registration update procedure
         * of the UE by means of the mobile reachable timer.
         * If the UE is not registered for emergency services,
         * the mobile reachable timer shall be longer than the value of timer
         * T3512. In this case, by default, the mobile reachable timer is
         * 4 minutes greater than the value of timer T3512.
         * The mobile reachable timer shall be reset and started with the
         * value as indicated above, when the AMF releases the NAS signalling
         * connection for the UE.
         *
         * TODO: If the UE is registered for emergency services, the AMF shall
         * set the mobile reachable timer with a value equal to timer T3512.
         */
                    ogs_timer_start(amf_ue->mobile_reachable.timer,
                            ogs_time_from_sec(
                                amf_self()->time.t3512.value + 240));
                }

            } else if (state == AMF_REMOVE_S1_CONTEXT_BY_RESET_ALL) {
                if (AMF_SESSION_SYNC_DONE(amf_ue, state)) {

                    amf_ue_deassociate(amf_ue);

                    if (ran_ue) {
                        amf_gnb_t *gnb = ran_ue->gnb;
                        ogs_assert(gnb);

                        ogs_debug("    SUPI[%s]", amf_ue->supi);
                        ran_ue_remove(ran_ue);

                        if (ogs_list_count(&gnb->ran_ue_list) == 0) {
                            r = ngap_send_ng_reset_ack(gnb, NULL);
                            ogs_expect(r == OGS_OK);
                            ogs_assert(r != OGS_ERROR);
                        }

                    } else {
                        ogs_warn("[%s] RAN-NG Context has already been removed",
                                amf_ue->supi);
                    }

        /*
         * When AMF release the NAS signalling connection,
         * ran_ue context is removed by ran_ue_remove() and
         * amf_ue/ran_ue is de-associated by amf_ue_deassociate().
         *
         * In this case, implicit deregistration is attempted
         * by the mobile reachable timer according to the standard document,
         * and amf_ue will be removed by amf_ue_remove().
         *
         * TS 24.501
         * 5.3.7 Handling of the periodic registration update timer and
         *
         * Start AMF_TIMER_MOBILE_REACHABLE
         * mobile reachable timer
         * The network supervises the periodic registration update procedure
         * of the UE by means of the mobile reachable timer.
         * If the UE is not registered for emergency services,
         * the mobile reachable timer shall be longer than the value of timer
         * T3512. In this case, by default, the mobile reachable timer is
         * 4 minutes greater than the value of timer T3512.
         * The mobile reachable timer shall be reset and started with the
         * value as indicated above, when the AMF releases the NAS signalling
         * connection for the UE.
         *
         * TODO: If the UE is registered for emergency services, the AMF shall
         * set the mobile reachable timer with a value equal to timer T3512.
         */
                    ogs_timer_start(amf_ue->mobile_reachable.timer,
                            ogs_time_from_sec(
                                amf_self()->time.t3512.value + 240));
                }

            } else if (state == AMF_REMOVE_S1_CONTEXT_BY_RESET_PARTIAL) {
                if (AMF_SESSION_SYNC_DONE(amf_ue, state)) {
                    ran_ue_t *iter = NULL;

                    amf_ue_deassociate(amf_ue);

                    if (ran_ue) {
                        amf_gnb_t *gnb = ran_ue->gnb;
                        ogs_assert(gnb);

                        ogs_debug("    SUPI[%s]", amf_ue->supi);
                        ran_ue_remove(ran_ue);

                        ogs_list_for_each(&gnb->ran_ue_list, iter) {
                            if (iter->part_of_ng_reset_requested == true) {
                                /* The GNB_UE context
                                 * where PartOfNG_interface was requested
                                 * still remains */
                                return OGS_OK;
                            }
                        }

                        /* All GNB_UE context
                         * where PartOfNG_interface was requested
                         * REMOVED */
                        ogs_assert(gnb->ng_reset_ack);
                        r = ngap_send_to_gnb(
                            gnb, gnb->ng_reset_ack, NGAP_NON_UE_SIGNALLING);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);

                        /* Clear NG-Reset Ack Buffer */
                        gnb->ng_reset_ack = NULL;

                    } else {
                        ogs_warn("[%s] RAN-NG Context has already been removed",
                                amf_ue->supi);
                    }
        /*
         * When AMF release the NAS signalling connection,
         * ran_ue context is removed by ran_ue_remove() and
         * amf_ue/ran_ue is de-associated by amf_ue_deassociate().
         *
         * In this case, implicit deregistration is attempted
         * by the mobile reachable timer according to the standard document,
         * and amf_ue will be removed by amf_ue_remove().
         *
         * TS 24.501
         * 5.3.7 Handling of the periodic registration update timer and
         *
         * Start AMF_TIMER_MOBILE_REACHABLE
         * mobile reachable timer
         * The network supervises the periodic registration update procedure
         * of the UE by means of the mobile reachable timer.
         * If the UE is not registered for emergency services,
         * the mobile reachable timer shall be longer than the value of timer
         * T3512. In this case, by default, the mobile reachable timer is
         * 4 minutes greater than the value of timer T3512.
         * The mobile reachable timer shall be reset and started with the
         * value as indicated above, when the AMF releases the NAS signalling
         * connection for the UE.
         *
         * TODO: If the UE is registered for emergency services, the AMF shall
         * set the mobile reachable timer with a value equal to timer T3512.
         */
                    ogs_timer_start(amf_ue->mobile_reachable.timer,
                            ogs_time_from_sec(
                                amf_self()->time.t3512.value + 240));
                }
            } else {
                ogs_error("Invalid STATE[%d]", state);
                ogs_assert_if_reached();
            }

            /*
             * If resource-status has already been updated by
             *   notify(/namf-callback/v1/{supi}/sm-context-status/{psi})
             * Remove 'amf_sess_t' context to call
             *   amf_nsmf_pdusession_handle_release_sm_context().
             */
            ogs_info("[%s:%d:%d][%d:%d:%s] "
                    "/nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify",
                    amf_ue->supi, sess->psi, state,
                    sess->n1_released, sess->n2_released,
                    OpenAPI_resource_status_ToString(sess->resource_status));

            if (sess->n1_released == true &&
                sess->n2_released == true &&
                sess->resource_status == OpenAPI_resource_status_RELEASED) {
                amf_nsmf_pdusession_handle_release_sm_context(
                        sess, AMF_RELEASE_SM_CONTEXT_NO_STATE);
            }
        }
    } else {
        OpenAPI_sm_context_update_error_t *SmContextUpdateError = NULL;
        OpenAPI_ext_problem_details_t *ProblemDetails = NULL;
        OpenAPI_ref_to_binary_data_t *n1SmMsg = NULL;
        ogs_pkbuf_t *n1smbuf = NULL;

#if 0 /* Is it needed? */
        OpenAPI_ref_to_binary_data_t *n2SmInfo = NULL;
        ogs_pkbuf_t *n2smbuf = NULL;
#endif

        amf_ue = sess->amf_ue;
        ogs_assert(amf_ue);

        SmContextUpdateError = recvmsg->SmContextUpdateError;
        if (!SmContextUpdateError) {
            ogs_error("[%d:%d] No SmContextUpdateError [%d]",
                    sess->psi, sess->pti, recvmsg->res_status);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return OGS_ERROR;
        }

        ProblemDetails = SmContextUpdateError->error;
        if (!ProblemDetails) {
            ogs_error("[%d:%d] No ProblemDetails [%d]",
                    sess->psi, sess->pti, recvmsg->res_status);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return OGS_ERROR;
        }

        if (state == AMF_UPDATE_SM_CONTEXT_SERVICE_REQUEST) {
            /*
             * In a situation where the UPF is changed through High Availability
             * it is implemented to send the Service-Accept to the UE
             * even if reactivation for user-resource fails.
             *
             * This is because it cannot be predicted whether a UE will
             * retransmit or leave it as it is, depending on the manufacturer,
             * in case of including reactivation error IE
             * such as Insufficient Resource.
             *
             * TS24.501
             * 5.6 5GMM connection management procedures
             * 5.6.1 Service request procedure
             * 5.6.1.4 Service request procedure accepted by the network
             * 5.6.1.4.1 UE is not using 5GS services
             *           with control plane CIoT 5GS optimization
             *
             * NOTE 1: It is up to UE implementation when to re-send a request
             * for user-plane re-establishment for the associated PDU session
             * after receiving a PDU session reactivation result error cause IE
             * with a 5GMM cause set to #92 "insufficient user-plane resources
             * for the PDU session".
             */
            ogs_error("Service-Request: ACTIVATING FAILED [%d:%s:%s]",
                    recvmsg->res_status,
                    OpenAPI_up_cnx_state_ToString(
                        SmContextUpdateError->up_cnx_state),
                    ProblemDetails->cause ?  ProblemDetails->cause : "Unknown");

            if (AMF_SESSION_SYNC_DONE(amf_ue,
                        AMF_UPDATE_SM_CONTEXT_SERVICE_REQUEST)) {
                r = nas_5gs_send_service_accept(amf_ue);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }

            return OGS_OK;
        }

        n1SmMsg = SmContextUpdateError->n1_sm_msg;
        if (n1SmMsg && n1SmMsg->content_id) {
            n1smbuf = ogs_sbi_find_part_by_content_id(
                    recvmsg, n1SmMsg->content_id);
            if (n1smbuf) {
                /*
                 * NOTE : The pkbuf created in the SBI message will be removed
                 *        from ogs_sbi_message_free(), so it must be copied.
                 */
                ogs_error("[%d:%d] PDU session establishment reject [state:%d]",
                        sess->psi, sess->pti, state);

                n1smbuf = ogs_pkbuf_copy(n1smbuf);
                ogs_assert(n1smbuf);
                r = nas_5gs_send_gsm_reject(ran_ue, sess,
                        OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, n1smbuf);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

                return OGS_ERROR;
            }
        }

#if 0 /* Is it needed? */
        n2SmInfo = SmContextUpdateError->n2_sm_info;
        if (!n2SmInfo || !n2SmInfo->content_id) {
            ogs_error("[%d:%d] No N2 SM Message", sess->psi, sess->pti);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return OGS_ERROR;
        }

        n2smbuf = ogs_sbi_find_part_by_content_id(
                recvmsg, n2SmInfo->content_id);
        if (!n2smbuf) {
            ogs_error("[%d:%d] No N2 SM Content [%s]",
                    sess->psi, sess->pti, n2SmInfo->content_id);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return OGS_ERROR;
        }
#endif

        ogs_error("[%d:%d] Error Indication [state:%d]",
                sess->psi, sess->pti, state);

        r = ngap_send_error_indication2(ran_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        return OGS_ERROR;
    }

    return OGS_OK;
}

int amf_nsmf_pdusession_handle_release_sm_context(amf_sess_t *sess, int state)
{
    int r;
    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(sess);
    sess = amf_sess_cycle(sess);
    if (!sess) {
        ogs_error("Session has already been removed");
        return OGS_ERROR;
    }

    amf_ue = amf_ue_cycle(sess->amf_ue);
    if (!amf_ue) {
        ogs_error("UE(amf_ue) Context has already been removed");
        return OGS_ERROR;
    }

    ran_ue = ran_ue_cycle(sess->ran_ue);

    /*
     * To check if Reactivation Request has been used.
     *
     * During the PFCP recovery process,
     * when a Reactivation Request is sent to PDU session release command,
     * the UE simultaneously sends PDU session release complete and
     * PDU session establishment request.
     *
     * In this case, old_gsm_type is PDU session release command and
     * current_gsm_type is PDU session establishment request.
     */
    if (sess->old_gsm_type == OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE &&
        sess->current_gsm_type ==
            OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST) {
        ogs_error("[%s:%d] Do not remove Session due to Reactivation-requested",
                amf_ue->supi, sess->psi);

        /*
         * Issue #2917
         *
         * These are variables that need to be initialized
         * when a PDU session release command with a Reactivation Request
         * and a PDU session release complete are sent at the same time.
         *
         * It is important to note that XXX_gsm_type, which is initialized now,
         * has a different initialization location than
         * pdu_session_release_complete_received/
         * pdu_session_resource_release_response_received
         *
         * XXX_received is initialized in the ESTABLISHMENT phase,
         * but XXX_gsm_type must be initialized in the RELEASE phase.
         */
        sess->old_gsm_type = 0;
        sess->current_gsm_type = 0;

        sess->n1_released = false;
        sess->n2_released = false;
        sess->resource_status = OpenAPI_resource_status_NULL;

    } else {
        ogs_info("[%s:%d] Release SM Context [state:%d]",
                amf_ue->supi, sess->psi, state);
        AMF_SESS_CLEAR(sess);
    }

    if (state == AMF_RELEASE_SM_CONTEXT_REGISTRATION_ACCEPT) {
        /*
         * 1. Registration request
         * 2. Release All SM contexts
         * 3. Registration accept
         */
        if (AMF_SESSION_SYNC_DONE(
                amf_ue, AMF_RELEASE_SM_CONTEXT_REGISTRATION_ACCEPT) &&
            AMF_SESSION_SYNC_DONE(
                amf_ue, AMF_UPDATE_SM_CONTEXT_REGISTRATION_REQUEST)) {
            if (ran_ue) {
                if (!PCF_AM_POLICY_ASSOCIATED(amf_ue)) {
                    r = amf_ue_sbi_discover_and_send(
                            OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL, NULL,
                            amf_npcf_am_policy_control_build_create,
                            amf_ue, 0, NULL);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                } else {
                    CLEAR_AMF_UE_TIMER(amf_ue->t3550);
                    r = nas_5gs_send_registration_accept(amf_ue);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                }
            } else {
                ogs_warn("[%s] RAN-NG Context has already been removed",
                        amf_ue->supi);
            }
        }

    } else if (state == AMF_RELEASE_SM_CONTEXT_SERVICE_ACCEPT) {
        /*
         * 1. Service request
         * 2. Release All SM contexts
         * 3. Service accept
         */
        if (AMF_SESSION_SYNC_DONE(amf_ue,
                AMF_RELEASE_SM_CONTEXT_SERVICE_ACCEPT) &&
            AMF_SESSION_SYNC_DONE(amf_ue,
                AMF_UPDATE_SM_CONTEXT_SERVICE_REQUEST)) {
            r = nas_5gs_send_service_accept(amf_ue);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }

    } else {

        if (AMF_SESSION_SYNC_DONE(amf_ue, state)) {

            if (state == AMF_RELEASE_SM_CONTEXT_NG_CONTEXT_REMOVE) {
                /*
                 * 1. Initial context setup failure
                 * 2. Release All SM contexts
                 * 3. UE Context release command
                 * 4. UE Context release complete
                 */
                r = ngap_send_amf_ue_context_release_command(amf_ue,
                        NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                        NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

            } else if (state == AMF_RELEASE_SM_CONTEXT_REGISTRATION_ACCEPT) {

                /* Not reached here */
                ogs_assert_if_reached();

            } else if (state == AMF_RELEASE_SM_CONTEXT_SERVICE_ACCEPT) {

                /* Not reached here */
                ogs_assert_if_reached();

            } else if (state == AMF_UE_INITIATED_DE_REGISTERED) {

                if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_de_registered)) {
                    /*
                     * 1. PDU session establishment request
                     * 2. PDUSessionResourceSetupRequest +
                     *    PDU session establishment accept
                     * 3. PDUSessionResourceSetupResponse
                     * 4. Deregistration request
                     * 5. UEContextReleaseCommand
                     * 6. UEContextReleaseComplete
                     */
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
                    } else {
                        r = nas_5gs_send_de_registration_accept(amf_ue);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                    }

                } else if (OGS_FSM_CHECK(&amf_ue->sm,
                            gmm_state_authentication)) {
                    ogs_fatal("Release SM Context in authentication");
                    ogs_assert_if_reached();
                } else if (OGS_FSM_CHECK(
                            &amf_ue->sm, gmm_state_security_mode)) {
                    ogs_fatal("Release SM Context in security-mode");
                    ogs_assert_if_reached();
                } else if (OGS_FSM_CHECK(&amf_ue->sm,
                                gmm_state_initial_context_setup)) {
                    ogs_fatal("Release SM Context in initial-context-setup");
                    ogs_assert_if_reached();
                } else if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_registered)) {
                    ogs_fatal("Release SM Context in registered");
                    ogs_assert_if_reached();
                } else if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_exception)) {
                    ogs_fatal("Release SM Context in exception");
                    ogs_assert_if_reached();
                } else {
                    ogs_fatal("Release SM Context : INVALID STATE");
                    ogs_assert_if_reached();
                }

            } else if (state == AMF_NETWORK_INITIATED_IMPLICIT_DE_REGISTERED ||
                        state == AMF_NETWORK_INITIATED_EXPLICIT_DE_REGISTERED) {

                if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_de_registered)) {
                    ogs_fatal("Release SM Context in de-registered");
                    ogs_assert_if_reached();
                } else if (OGS_FSM_CHECK(&amf_ue->sm,
                            gmm_state_authentication)) {
                    ogs_fatal("Release SM Context in authentication");
                    ogs_assert_if_reached();
                } else if (OGS_FSM_CHECK(
                            &amf_ue->sm, gmm_state_security_mode)) {
                    ogs_fatal("Release SM Context in security-mode");
                    ogs_assert_if_reached();
                } else if (OGS_FSM_CHECK(&amf_ue->sm,
                                gmm_state_initial_context_setup)) {
                    ogs_fatal("Release SM Context in initial-context-setup");
                    ogs_assert_if_reached();
                } else if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_registered)) {
                    /*
                     * 1. Network-Initiated Implict-Explicit De-Registered
                     * 5. Deregistration request
                     * 5. Deregistration accept
                     * 6. UEContextReleaseCommand
                     * 7. UEContextReleaseComplete
                     */
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

                } else if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_exception)) {
                    ogs_fatal("Release SM Context in exception");
                    ogs_assert_if_reached();
                } else {
                    ogs_fatal("Release SM Context : INVALID STATE");
                    ogs_assert_if_reached();
                }
            } else if (state == AMF_RELEASE_SM_CONTEXT_NO_STATE) {
                /* NO_STATE */

                if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_de_registered)) {
                    /*
                     * 1. PDU session release request
                     * 2. PDUSessionResourceReleaseCommand +
                     *    PDU session release command
                     * 3. PDUSessionResourceReleaseREsponse
                     * 4. PDU session release complete
                     * 5. Deregistration request
                     * 6. UEContextReleaseCommand
                     * 7. UEContextReleaseComplete
                     */
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
                    } else {
                        r = nas_5gs_send_de_registration_accept(amf_ue);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                    }
                } else if (OGS_FSM_CHECK(&amf_ue->sm,
                            gmm_state_authentication)) {

                    r = amf_ue_sbi_discover_and_send(
                            OGS_SBI_SERVICE_TYPE_NAUSF_AUTH, NULL,
                            amf_nausf_auth_build_authenticate,
                            amf_ue, 0, NULL);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                } else if (OGS_FSM_CHECK(
                            &amf_ue->sm, gmm_state_security_mode)) {
                    ogs_fatal("Release SM Context in security-mode");
                    ogs_assert_if_reached();
                } else if (OGS_FSM_CHECK(&amf_ue->sm,
                                gmm_state_initial_context_setup)) {
                    ogs_fatal("Release SM Context in initial-context-setup");
                    ogs_assert_if_reached();
                } else if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_registered)) {
                    /*
                     * 1. PDU session release request
                     * 2. PDUSessionResourceReleaseCommand +
                     *    PDU session release command
                     * 3. PDUSessionResourceReleaseREsponse
                     * 4. PDU session release complete
                     *
                     * So, Nothing to do!
                     */
                } else if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_exception)) {
                    /*
                     * 1. GMM Exception
                     * 2. Release All SM contexts
                     * 3. UE Context release command
                     * 4. UE Context release complete
                     */
                    r = ngap_send_amf_ue_context_release_command(amf_ue,
                            NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                            NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                } else {
                    ogs_fatal("Release SM Context : INVALID STATE");
                    ogs_assert_if_reached();
                }
            } else {
                ogs_fatal("Release SM STATE undefined");
                ogs_assert_if_reached();
            }
        }
    }

    return OGS_OK;
}
