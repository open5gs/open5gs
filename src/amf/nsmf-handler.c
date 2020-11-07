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

int amf_nsmf_pdu_session_handle_create_sm_context(
        amf_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    int rv;

    ogs_assert(sess);
    ogs_assert(recvmsg);

    if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_CREATED) {
        ogs_sbi_message_t message;
        ogs_sbi_header_t header;

        if (!recvmsg->http.location) {
            ogs_error("[%d:%d] No http.location", sess->psi, sess->pti);
            nas_5gs_send_back_5gsm_message_from_sbi(sess,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
            return OGS_ERROR;
        }

        memset(&header, 0, sizeof(header));
        header.uri = recvmsg->http.location;

        rv = ogs_sbi_parse_header(&message, &header);
        if (rv != OGS_OK) {
            ogs_error("[%d:%d] Cannot parse http.location [%s]",
                    sess->psi, sess->pti, recvmsg->http.location);
            nas_5gs_send_back_5gsm_message_from_sbi(sess,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
            return OGS_ERROR;
        }

        if (!message.h.resource.component[1]) {
            ogs_error("[%d:%d] No SmContextRef [%s]",
                    sess->psi, sess->pti, recvmsg->http.location);

            ogs_sbi_header_free(&header);
            nas_5gs_send_back_5gsm_message_from_sbi(sess,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
            return OGS_ERROR;
        }

        if (sess->pdu_session_establishment_accept) {
            /*
             * [1-SERVER] /namf-comm/v1/ue-contexts/{supi}/n1-n2-messages
             * [2-CLIENT] /nsmf-pdusession/v1/sm-contexts
             *
             * If [1-SERVER] arrives before [2-CLIENT],
             * there is no sm-context-ref. The PDU session establishment accept
             * stored in [1-SERVER] is now trasnmitted to gNB.
             */
            amf_ue_t *amf_ue = sess->amf_ue;
            ogs_assert(amf_ue);

            rv = nas_5gs_send_to_gnb(amf_ue,
                    sess->pdu_session_establishment_accept);

            sess->pdu_session_establishment_accept = NULL;

            if (rv != OGS_OK) {
                ogs_error("[%d:%d] nas_5gs_send_to_gnb() failed",
                        sess->psi, sess->pti);

                ogs_sbi_header_free(&header);
                nas_5gs_send_back_5gsm_message_from_sbi(sess,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
                return OGS_ERROR;
            }
        }

        if (sess->sm_context_ref)
            ogs_free(sess->sm_context_ref);
        sess->sm_context_ref = ogs_strdup(message.h.resource.component[1]);

        ogs_sbi_header_free(&header);

    } else {
        OpenAPI_sm_context_create_error_t *SmContextCreateError = NULL;
        OpenAPI_ref_to_binary_data_t *n1SmMsg = NULL;
        ogs_pkbuf_t *n1smbuf = NULL;

        SmContextCreateError = recvmsg->SmContextCreateError;
        if (!SmContextCreateError) {
            ogs_error("[%d:%d] No SmContextCreateError", sess->psi, sess->pti);
            nas_5gs_send_back_5gsm_message_from_sbi(sess, recvmsg->res_status);
            return OGS_ERROR;
        }
        if (!SmContextCreateError->error) {
            ogs_error("[%d:%d] No Error", sess->psi, sess->pti);
            nas_5gs_send_back_5gsm_message_from_sbi(sess, recvmsg->res_status);
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
                n1smbuf = ogs_pkbuf_copy(n1smbuf);
                ogs_assert(n1smbuf);
                nas_5gs_send_gsm_reject_from_sbi(sess,
                    OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION,
                    n1smbuf, recvmsg->res_status);
                return OGS_ERROR;
            }
        }

        nas_5gs_send_back_5gsm_message_from_sbi(sess, recvmsg->res_status);

        return OGS_ERROR;
    }

    return OGS_OK;
}

int amf_nsmf_pdu_session_handle_update_sm_context(
        amf_sess_t *sess, int state, ogs_sbi_message_t *recvmsg)
{
    amf_ue_t *amf_ue = NULL;
    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ogs_assert(recvmsg);

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
                    nas_5gs_send_gmm_reject(amf_ue,
                            OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED);
                    return OGS_ERROR;
                }

                /*
                 * To Deliver N2 SM Content to gNB Temporarily,
                 * Store N2 SM Context in Session Context
                 */
                if (sess->pdu_session_resource_setup_request_transfer) {
                    /*
                     * It should not be reached this way.
                     * If the problem occurred, free the old n2smbuf
                     */
                    ogs_error("[%s:%d] N2 SM Content is duplicated",
                            amf_ue->supi, sess->psi);
                    ogs_pkbuf_free(
                            sess->pdu_session_resource_setup_request_transfer);
                }
                /*
                 * NOTE : The pkbuf created in the SBI message will be removed
                 *        from ogs_sbi_message_free().
                 *        So it must be copied and push a event queue.
                 */
                sess->pdu_session_resource_setup_request_transfer =
                    ogs_pkbuf_copy(n2smbuf);
                ogs_assert(sess->pdu_session_resource_setup_request_transfer);

                if (SESSION_SYNC_DONE(amf_ue)) {
                    nas_5gs_send_accept(amf_ue);

                /*
                 * After sending accept message, N2 SM context is freed
                 * For checking memory, NULL pointer should be set to n2smbuf.
                 */
                    ogs_list_for_each(&amf_ue->sess_list, sess) {
                        if (sess->pdu_session_resource_setup_request_transfer) {
                            ogs_pkbuf_free(sess->
                                pdu_session_resource_setup_request_transfer);
                            sess->pdu_session_resource_setup_request_transfer =
                                NULL;
                        }
                    }
                }
                break;

            case OpenAPI_n2_sm_info_type_PDU_RES_REL_CMD:
                if (!n1smbuf) {
                    ogs_error("[%s:%d] No N1 SM Content [%s]",
                            amf_ue->supi, sess->psi, n1SmMsg->content_id);
                    nas_5gs_send_back_5gsm_message(sess,
                            OGS_5GMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE);
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

                nas_send_pdu_session_release_command(sess, n1smbuf, n2smbuf);
                break;

            default:
                ogs_error("Not implemented [%d]",
                        SmContextUpdatedData->n2_sm_info_type);
                ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            }

        } else {

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
            } else if (state == AMF_UPDATE_SM_CONTEXT_DEACTIVATED) {
                /*
                 * 1. UEContextReleaseRequest
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 * 3. PFCP Session Modifcation Request (Apply:Buff & NOCP)
                 * 4. PFCP Session Modifcation Response
                 * 5. UEContextReleaseCommand
                 * 6. UEContextReleaseComplete
                 */

                if (SESSION_SYNC_DONE(amf_ue)) {
                    ngap_send_amf_ue_context_release_command(amf_ue,
                            NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                            NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK, 0);
                }

            } else if (state == AMF_UPDATE_SM_CONTEXT_ACTIVATING) {

                /* Not reached here */

            } else if (state == AMF_UPDATE_SM_CONTEXT_N2_RELEASED) {

                /*
                 * 1. PDUSessionResourceReleaseResponse
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 */
                ogs_debug("[%s:%d] Receive Update SM context(N2-RELEASED)",
                        amf_ue->supi, sess->psi);

                sess->n2_released = true;

            } else if (state == AMF_UPDATE_SM_CONTEXT_N1_RELEASED) {
                /*
                 * 1. PDU session release complete
                 *    CLEAR_SM_CONTEXT_REF(sess) in gmm-handler.c
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 */

                ogs_debug("[%s:%d] Receive Update SM context(N1-RELEASED)",
                        amf_ue->supi, sess->psi);

                sess->n1_released = true;

            } else if (state == AMF_UPDATE_SM_CONTEXT_NG_RESET) {
                /*
                 * 1. NG RESET
                 * 2. /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
                 * 3. PFCP Session Modifcation Request (Apply:Buff & NOCP)
                 * 4. PFCP Session Modifcation Response
                 * 5. NG RESET ACKNOWLEDGE
                 */

                /* Nothing to do */
                if (SESSION_SYNC_DONE(amf_ue)) {
                    ran_ue_t *ran_ue = ran_ue_cycle(amf_ue->ran_ue);
                    if (ran_ue) {
                        ogs_debug("    SUPI[%s]", amf_ue->supi);
                        ran_ue_remove(ran_ue);
                    } else {
                        ogs_warn("[%s] RAN-NG Context has already been removed",
                                amf_ue->supi);
                    }
                }

            } else {
                ogs_error("Invalid STATE[%d]", state);
            }

            /*
             * If resource-status has already been updated by
             *   notify(/namf-callback/v1/{supi}/sm-context-status/{psi})
             * Remove 'amf_sess_t' context to call
             *   amf_nsmf_pdu_session_handle_release_sm_context().
             */
            if (sess->n1_released == true &&
                sess->n2_released == true &&
                sess->resource_status == OpenAPI_resource_status_RELEASED) {

                ogs_debug("[%s:%d] SM context remove", amf_ue->supi, sess->psi);
                amf_nsmf_pdu_session_handle_release_sm_context(
                        sess, AMF_RELEASE_SM_CONTEXT_NO_STATE);
            }
        }
    } else {
        amf_ue_t *amf_ue = NULL;

        OpenAPI_sm_context_update_error_t *SmContextUpdateError = NULL;
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
            ogs_error("[%d:%d] No SmContextUpdateError", sess->psi, sess->pti);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return OGS_ERROR;
        }
        if (!SmContextUpdateError->error) {
            ogs_error("[%d:%d] No Error", sess->psi, sess->pti);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return OGS_ERROR;
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
                n1smbuf = ogs_pkbuf_copy(n1smbuf);
                ogs_assert(n1smbuf);
                nas_5gs_send_gsm_reject_from_sbi(sess,
                    OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION,
                    n1smbuf, recvmsg->res_status);
                return OGS_ERROR;
            }
        }

#if 0 /* Is it needed? */
        n2SmInfo = SmContextUpdateError->n2_sm_info;
        if (!n2SmInfo || !n2SmInfo->content_id) {
            ogs_error("[%d:%d] No N2 SM Message", sess->psi, sess->pti);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return OGS_ERROR;
        }

        n2smbuf = ogs_sbi_find_part_by_content_id(
                recvmsg, n2SmInfo->content_id);
        if (!n2smbuf) {
            ogs_error("[%d:%d] No N2 SM Content [%s]",
                    sess->psi, sess->pti, n2SmInfo->content_id);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return OGS_ERROR;
        }
#endif

        ngap_send_error_indication2(amf_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);

        return OGS_ERROR;
    }

    return OGS_OK;
}

int amf_nsmf_pdu_session_handle_release_sm_context(amf_sess_t *sess, int state)
{
    amf_ue_t *amf_ue = NULL;

    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);

    amf_sess_remove(sess);

    /* Check last session */
    if (ogs_list_count(&amf_ue->sess_list) == 0) {

        if (state == AMF_RELEASE_SM_CONTEXT_NG_CONTEXT_REMOVE) {
            /*
             * 1. Initial context setup failure
             * 2. Release All SM contexts
             * 3. UE Context release command
             * 4. UE Context release complete
             */
            ngap_send_amf_ue_context_release_command(amf_ue,
                    NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                    NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);

        } else if (state == AMF_RELEASE_SM_CONTEXT_REGISTRATION_ACCEPT) {
            /*
             * 1. Registration request
             * 2. Release All SM contexts
             * 3. Registration accept
             */
            nas_5gs_send_registration_accept(amf_ue);

        } else if (state == AMF_RELEASE_SM_CONTEXT_SERVICE_ACCEPT) {
            /*
             * 1. Service request
             * 2. Release All SM contexts
             * 3. Service accept
             */
            nas_5gs_send_service_accept(amf_ue);

        } else {
            /* NO_STATE */

            if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_authentication)) {

                amf_ue_sbi_discover_and_send(OpenAPI_nf_type_AUSF, amf_ue, NULL,
                        amf_nausf_auth_build_authenticate);

            } else if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_de_registered)) {
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

                nas_5gs_send_de_registration_accept(amf_ue);

            } else if (OGS_FSM_CHECK(&amf_ue->sm, gmm_state_registered)) {
                /*
                 * 1. PDU session release request
                 * 2. PDUSessionResourceReleaseCommand +
                 *    PDU session release command
                 * 3. PDUSessionResourceReleaseREsponse
                 * 4. PDU session release complete
                 *
                 * No Deregistration request in the above step
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
                ngap_send_amf_ue_context_release_command(amf_ue,
                        NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                        NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);

            } else if (OGS_FSM_CHECK(&amf_ue->sm,
                            gmm_state_initial_context_setup)) {
                ogs_fatal("Release SM Context in initial-context-setup");
                ogs_assert_if_reached();
            } else if (OGS_FSM_CHECK(
                        &amf_ue->sm, gmm_state_security_mode)) {
                ogs_fatal("Release SM Context in security-mode");
                ogs_assert_if_reached();
            } else {
                ogs_fatal("Release SM Context : INVALID STATE");
                ogs_assert_if_reached();
            }
        }
    }

    return OGS_OK;
}
