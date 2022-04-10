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

#include "sbi-path.h"
#include "nas-path.h"
#include "ngap-path.h"
#include "pfcp-path.h"
#include "nsmf-handler.h"

bool smf_nsmf_handle_create_sm_context(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    smf_ue_t *smf_ue = NULL;

    ogs_pkbuf_t *n1smbuf = NULL;

    ogs_sbi_client_t *client = NULL;
    ogs_sockaddr_t *addr = NULL;

    OpenAPI_sm_context_create_data_t *SmContextCreateData = NULL;
    OpenAPI_nr_location_t *NrLocation = NULL;
    OpenAPI_snssai_t *sNssai = NULL;
    OpenAPI_plmn_id_nid_t *servingNetwork = NULL;
    OpenAPI_ref_to_binary_data_t *n1SmMsg = NULL;

    ogs_assert(stream);
    ogs_assert(message);

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    SmContextCreateData = message->SmContextCreateData;
    if (!SmContextCreateData) {
        ogs_error("[%s:%d] No SmContextCreateData",
                smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No SmContextCreateData", smf_ue->supi, n1smbuf);
        return false;
    }

    sNssai = SmContextCreateData->s_nssai;
    if (!sNssai) {
        ogs_error("[%s:%d] No sNssai", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No sNssai", smf_ue->supi, n1smbuf);
        return false;
    }

    servingNetwork = SmContextCreateData->serving_network;
    if (!servingNetwork || !servingNetwork->mnc || !servingNetwork->mcc) {
        ogs_error("[%s:%d] No servingNetwork",
                smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No servingNetwork", smf_ue->supi, n1smbuf);
        return false;
    }

    if (!SmContextCreateData->ue_location ||
        !SmContextCreateData->ue_location->nr_location) {
        ogs_error("[%s:%d] No UeLocation", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No UeLocation", smf_ue->supi, n1smbuf);
        return false;
    }

    NrLocation = SmContextCreateData->ue_location->nr_location;
    if (!NrLocation->tai ||
        !NrLocation->tai->plmn_id || !NrLocation->tai->tac ||
        !NrLocation->ncgi ||
        !NrLocation->ncgi->plmn_id || !NrLocation->ncgi->nr_cell_id) {
        ogs_error("[%s:%d] No NrLocation", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No UeLocation", smf_ue->supi, n1smbuf);
        return false;
    }

    n1SmMsg = SmContextCreateData->n1_sm_msg;
    if (!n1SmMsg || !n1SmMsg->content_id) {
        ogs_error("[%s:%d] No n1SmMsg", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No n1SmMsg", smf_ue->supi, n1smbuf);
        return false;
    }

    n1smbuf = ogs_sbi_find_part_by_content_id(message, n1SmMsg->content_id);
    if (!n1smbuf) {
        ogs_error("[%s:%d] No N1 SM Content [%s]",
                smf_ue->supi, sess->psi, n1SmMsg->content_id);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N1 SM Content", smf_ue->supi, n1smbuf);
        return false;
    }

    if (!SmContextCreateData->sm_context_status_uri) {
        ogs_error("[%s:%d] No SmContextStatusNotification",
                smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No SmContextStatusNotification", smf_ue->supi, n1smbuf);
        return false;
    }

    addr = ogs_sbi_getaddr_from_uri(SmContextCreateData->sm_context_status_uri);
    if (!addr) {
        ogs_error("[%s:%d] Invalid URI [%s]",
                smf_ue->supi, sess->psi,
                SmContextCreateData->sm_context_status_uri);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, "Invalid URI",
                SmContextCreateData->sm_context_status_uri, n1smbuf);
        return false;
    }

    ogs_sbi_parse_plmn_id_nid(&sess->plmn_id, servingNetwork);

    sess->sbi_rat_type = SmContextCreateData->rat_type;

    ogs_sbi_parse_nr_location(&sess->nr_tai, &sess->nr_cgi, NrLocation);
    if (NrLocation->ue_location_timestamp)
        ogs_sbi_time_from_string(&sess->ue_location_timestamp,
                NrLocation->ue_location_timestamp);

    sess->s_nssai.sst = sNssai->sst;
    sess->s_nssai.sd = ogs_s_nssai_sd_from_string(sNssai->sd);
    if (SmContextCreateData->hplmn_snssai) {
        sess->mapped_hplmn.sst = SmContextCreateData->hplmn_snssai->sst;
        sess->mapped_hplmn.sd = ogs_s_nssai_sd_from_string(
                                    SmContextCreateData->hplmn_snssai->sd);
    }

    if (sess->sm_context_status_uri)
        ogs_free(sess->sm_context_status_uri);
    sess->sm_context_status_uri =
        ogs_strdup(SmContextCreateData->sm_context_status_uri);
    ogs_assert(sess->sm_context_status_uri);

    client = ogs_sbi_client_find(addr);
    if (!client) {
        client = ogs_sbi_client_add(addr);
        ogs_assert(client);
    }
    OGS_SBI_SETUP_CLIENT(&sess->namf, client);

    ogs_freeaddrinfo(addr);

    if (SmContextCreateData->dnn) {
        if (sess->session.name) ogs_free(sess->session.name);
        sess->session.name = ogs_strdup(SmContextCreateData->dnn);
        ogs_assert(sess->session.name);
    }

    if (SmContextCreateData->pcf_id) {
        if (sess->pcf_id) ogs_free(sess->pcf_id);
        sess->pcf_id = ogs_strdup(SmContextCreateData->pcf_id);
        ogs_assert(sess->pcf_id);
    }

    /*
     * NOTE : The pkbuf created in the SBI message will be removed
     *        from ogs_sbi_message_free().
     *        So it must be copied and push a event queue.
     */
    n1smbuf = ogs_pkbuf_copy(n1smbuf);
    ogs_assert(n1smbuf);
    nas_5gs_send_to_gsm(sess, stream, n1smbuf);

    return true;
}

bool smf_nsmf_handle_update_sm_context(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    int i;
    smf_ue_t *smf_ue = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_sm_context_update_data_t *SmContextUpdateData = NULL;
    OpenAPI_ref_to_binary_data_t *n1SmMsg = NULL;
    OpenAPI_ref_to_binary_data_t *n2SmMsg = NULL;

    ogs_pkbuf_t *n1smbuf = NULL;
    ogs_pkbuf_t *n2smbuf = NULL;

    ogs_assert(stream);
    ogs_assert(message);

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    SmContextUpdateData = message->SmContextUpdateData;
    if (!SmContextUpdateData) {
        ogs_error("[%s:%d] No SmContextUpdateData",
                smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No SmContextUpdateData", smf_ue->supi, NULL, NULL);
        return false;
    }

    if (SmContextUpdateData->ue_location &&
        SmContextUpdateData->ue_location->nr_location) {
        OpenAPI_nr_location_t *NrLocation =
            SmContextUpdateData->ue_location->nr_location;
        if (NrLocation->tai &&
            NrLocation->tai->plmn_id && NrLocation->tai->tac &&
            NrLocation->ncgi &&
            NrLocation->ncgi->plmn_id && NrLocation->ncgi->nr_cell_id) {

            ogs_sbi_parse_nr_location(&sess->nr_tai, &sess->nr_cgi, NrLocation);
            if (NrLocation->ue_location_timestamp)
                ogs_sbi_time_from_string(&sess->ue_location_timestamp,
                        NrLocation->ue_location_timestamp);

            ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
                ogs_plmn_id_hexdump(&sess->nr_tai.plmn_id), sess->nr_tai.tac.v);
            ogs_debug("    NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]",
                ogs_plmn_id_hexdump(&sess->nr_cgi.plmn_id),
                (long long)sess->nr_cgi.cell_id);
        }
    }

    if (SmContextUpdateData->n1_sm_msg) {
        n1SmMsg = SmContextUpdateData->n1_sm_msg;
        if (!n1SmMsg || !n1SmMsg->content_id) {
            ogs_error("[%s:%d] No n1SmMsg", smf_ue->supi, sess->psi);
            n1smbuf = gsm_build_pdu_session_release_reject(sess,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
            smf_sbi_send_sm_context_update_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No n1SmMsg", smf_ue->supi, n1smbuf, NULL);
            return false;
        }

        n1smbuf = ogs_sbi_find_part_by_content_id(message, n1SmMsg->content_id);
        if (!n1smbuf) {
            ogs_error("[%s:%d] No N1 SM Content [%s]",
                    smf_ue->supi, sess->psi, n1SmMsg->content_id);
            n1smbuf = gsm_build_pdu_session_release_reject(sess,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
            smf_sbi_send_sm_context_update_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No N1 SM Content", smf_ue->supi, n1smbuf, NULL);
            return false;
        }

        /*
         * NOTE : The pkbuf created in the SBI message will be removed
         *        from ogs_sbi_message_free().
         *        So it must be copied and push a event queue.
         */
        n1smbuf = ogs_pkbuf_copy(n1smbuf);
        ogs_assert(n1smbuf);
        nas_5gs_send_to_gsm(sess, stream, n1smbuf);

        return true;
    
    } else if (SmContextUpdateData->n2_sm_info) {

        /*********************************************************
         * Handle ACTIVATED
         ********************************************************/

        if (!SmContextUpdateData->n2_sm_info_type) {
            ogs_error("[%s:%d] No n2SmInfoType", smf_ue->supi, sess->psi);
            smf_sbi_send_sm_context_update_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No n2SmInfoType", smf_ue->supi, NULL, NULL);
            return false;
        }

        n2SmMsg = SmContextUpdateData->n2_sm_info;
        if (!n2SmMsg || !n2SmMsg->content_id) {
            ogs_error("[%s:%d] No N2SmInfo.content_id",
                    smf_ue->supi, sess->psi);
            smf_sbi_send_sm_context_update_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No n2SmInfo.content_id", smf_ue->supi, NULL, NULL);
            return false;
        }

        n2smbuf = ogs_sbi_find_part_by_content_id(message, n2SmMsg->content_id);
        if (!n2smbuf) {
            ogs_error("[%s:%d] No N2 SM Content", smf_ue->supi, sess->psi);
            smf_sbi_send_sm_context_update_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No N2 SM Content", smf_ue->supi, NULL, NULL);
            return false;
        }

        /*
         * NOTE : The pkbuf created in the SBI message will be removed
         *        from ogs_sbi_message_free().
         *        So it must be copied and push a event queue.
         */
        n2smbuf = ogs_pkbuf_copy(n2smbuf);
        ogs_assert(n2smbuf);
        ngap_send_to_n2sm(
                sess, stream, SmContextUpdateData->n2_sm_info_type, n2smbuf);

    } else if (SmContextUpdateData->up_cnx_state) {

        if (SmContextUpdateData->up_cnx_state ==
                OpenAPI_up_cnx_state_DEACTIVATED) {

        /*********************************************************
         * Handle DEACTIVATED
         ********************************************************/
            if (sess->ngap_state.pdu_session_resource_release ==
                    SMF_NGAP_STATE_DELETE_TRIGGER_UE_REQUESTED) {
                /*
                 * 1. UE->SMF: PDU session release request
                 * 2. PFCP Session Deletion Request/Response
                 * 3. AMF/SMF->UE : PDUSessionResourceReleaseCommand +
                 *                  PDU session release command
                 *    sess->ngap_state.pdu_session_resource_release is set
                 *      to SMF_NGAP_STATE_DELETE_TRIGGER_UE_REQUESTED
                 * 4. UE->AMF/SMF : PDUSessionResourceReleaseResponse
                 *
                 * If UE sends UEContextReleaseRequest to the AMF/SMF,
                 * there is no PFCP context in the SMF/UPF.
                 *
                 * So, PFCP deactivation is skipped.
                 */
                smf_sbi_send_sm_context_updated_data_up_cnx_state(
                        sess, stream, OpenAPI_up_cnx_state_DEACTIVATED);
            } else {
                ogs_assert(OGS_OK ==
                    smf_5gc_pfcp_send_session_modification_request(
                        sess, stream,
                        OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_DEACTIVATE, 0));
            }

        } else if (SmContextUpdateData->up_cnx_state ==
                OpenAPI_up_cnx_state_ACTIVATING) {

        /*********************************************************
         * Handle ACTIVATING
         ********************************************************/
            OpenAPI_sm_context_updated_data_t SmContextUpdatedData;
            OpenAPI_ref_to_binary_data_t n2SmInfo;

            /*
             * TODO :
             *
             * TS29.502 5.2.2.3.2.2
             * Activation of User Plane connectivity of PDU session
             *
             * 2b. If the request does not include the "UE presence
             * in LADN service area" indication and the SMF determines
             * that the DNN corresponds to a LADN, then the SMF shall
             * consider that the UE is outside of the LADN service area.
             *
             * The SMF shall reject the request if the UE is outside
             * of the LADN service area. If the SMF cannot proceed
             * with activating the user plane connection of the PDU session
             * (e.g. if the PDU session corresponds to a PDU session
             * of SSC mode 2 and the SMF decides to change
             * the PDU Session Anchor), the SMF shall return an error response,
             * as specified for step 2b of figure 5.2.2.3.1-1.
             *
             * For a 4xx/5xx response, the SmContextUpdateError structure
             * shall include the following additional information:
             *
             * upCnxState attribute set to DEACTIVATED.
             *
             *
             * TS23.502 4.2.3
             * Service Request Procedures
             *
             * 8a. If the SMF find the PDU Session is activated
             * when receiving the Nsmf_PDUSession_UpdateSMContext Request
             * in step 4 with Operation Type set to "UP activate"
             * to indicate establishment of User Plane resources
             * for the PDU Session(s), it deletes the AN Tunnel Info
             * and initiates an N4 Session Modification procedure
             * to remove Tunnel Info of AN in the UPF.
             */

            memset(&sendmsg, 0, sizeof(sendmsg));
            sendmsg.SmContextUpdatedData = &SmContextUpdatedData;

            memset(&SmContextUpdatedData, 0, sizeof(SmContextUpdatedData));
            SmContextUpdatedData.up_cnx_state = OpenAPI_up_cnx_state_ACTIVATING;
            SmContextUpdatedData.n2_sm_info_type =
                OpenAPI_n2_sm_info_type_PDU_RES_SETUP_REQ;
            SmContextUpdatedData.n2_sm_info = &n2SmInfo;

            memset(&n2SmInfo, 0, sizeof(n2SmInfo));
            n2SmInfo.content_id = (char *)OGS_SBI_CONTENT_NGAP_SM_ID;

            sendmsg.num_of_part = 0;

            sendmsg.part[sendmsg.num_of_part].pkbuf =
                ngap_build_pdu_session_resource_setup_request_transfer(sess);
            if (sendmsg.part[sendmsg.num_of_part].pkbuf) {
                sendmsg.part[sendmsg.num_of_part].content_id =
                    (char *)OGS_SBI_CONTENT_NGAP_SM_ID;
                sendmsg.part[sendmsg.num_of_part].content_type =
                    (char *)OGS_SBI_CONTENT_NGAP_TYPE;
                sendmsg.num_of_part++;
            }

            response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));

            for (i = 0; i < sendmsg.num_of_part; i++)
                if (sendmsg.part[i].pkbuf)
                    ogs_pkbuf_free(sendmsg.part[i].pkbuf);

        } else {
            char *strerror = ogs_msprintf("[%s:%d] Invalid upCnxState [%d]",
                smf_ue->supi, sess->psi, SmContextUpdateData->up_cnx_state);
            ogs_assert(strerror);

            ogs_error("%s", strerror);
            smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, strerror, NULL, NULL, NULL);
            ogs_free(strerror);

            return false;
        }
    } else if (SmContextUpdateData->ho_state) {
        if (SmContextUpdateData->ho_state == OpenAPI_ho_state_COMPLETED) {
            bool far_update = false;
            smf_bearer_t *qos_flow = NULL;

            if (sess->handover.prepared == true) {
                /* Need to Update? */
                if (memcmp(&sess->gnb_n3_ip, &sess->handover.gnb_n3_ip,
                            sizeof(sess->gnb_n3_ip)) != 0 ||
                    sess->gnb_n3_teid != sess->handover.gnb_n3_teid)
                    far_update = true;

                memcpy(&sess->gnb_n3_ip,
                        &sess->handover.gnb_n3_ip, sizeof(sess->gnb_n3_ip));
                sess->gnb_n3_teid = sess->handover.gnb_n3_teid;
            }
            sess->handover.prepared = false;

            ogs_list_for_each(&sess->bearer_list, qos_flow) {
                ogs_pfcp_far_t *dl_far = qos_flow->dl_far;
                ogs_assert(dl_far);

                if (dl_far->handover.prepared == true) {

                    if (dl_far->apply_action != OGS_PFCP_APPLY_ACTION_FORW) {
                        far_update = true;
                    }

                    dl_far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;
                    ogs_assert(OGS_OK ==
                        ogs_pfcp_ip_to_outer_header_creation(
                            &sess->gnb_n3_ip,
                            &dl_far->outer_header_creation,
                            &dl_far->outer_header_creation_len));
                    dl_far->outer_header_creation.teid = sess->gnb_n3_teid;
                }
                dl_far->handover.prepared = false;
            }

            if (far_update) {
                ogs_assert(OGS_OK ==
                    smf_5gc_pfcp_send_session_modification_request(
                        sess, stream,
                        OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE|
                        OGS_PFCP_MODIFY_N2_HANDOVER|OGS_PFCP_MODIFY_END_MARKER,
                        0));
            } else {
                char *strerror = ogs_msprintf(
                        "[%s:%d] No FAR Update", smf_ue->supi, sess->psi);
                ogs_assert(strerror);

                ogs_error("%s", strerror);
                smf_sbi_send_sm_context_update_error(
                        stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        strerror, NULL, NULL, NULL);
                ogs_free(strerror);

                return false;
            }
        } else if (SmContextUpdateData->ho_state ==
                OpenAPI_ho_state_CANCELLED) {
            smf_bearer_t *qos_flow = NULL;

            sess->handover.prepared = false;

            ogs_list_for_each(&sess->bearer_list, qos_flow) {
                ogs_pfcp_far_t *dl_far = qos_flow->dl_far;
                ogs_assert(dl_far);

                dl_far->handover.prepared = false;
            }

            if (smf_sess_have_indirect_data_forwarding(sess) == true) {
                ogs_assert(OGS_OK ==
                    smf_5gc_pfcp_send_session_modification_request(
                        sess, stream,
                        OGS_PFCP_MODIFY_INDIRECT|OGS_PFCP_MODIFY_REMOVE|
                        OGS_PFCP_MODIFY_HANDOVER_CANCEL,
                        0));
            } else {
                smf_sbi_send_sm_context_updated_data_ho_state(
                        sess, stream, OpenAPI_ho_state_CANCELLED);
            }

        } else {
            char *strerror = ogs_msprintf("[%s:%d] Invalid hoState [%d]",
                smf_ue->supi, sess->psi, SmContextUpdateData->ho_state);
            ogs_assert(strerror);

            ogs_error("%s", strerror);
            smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, strerror, NULL, NULL, NULL);
            ogs_free(strerror);

            return false;
        }
    } else if (SmContextUpdateData->is_release == true &&
                SmContextUpdateData->release == true) {
        if (sess->policy_association_id) {
            if (sess->ngap_state.pdu_session_resource_release ==
                    SMF_NGAP_STATE_DELETE_TRIGGER_UE_REQUESTED) {

                /* PCF session context has already been removed */
                memset(&sendmsg, 0, sizeof(sendmsg));

                response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
                ogs_assert(response);
                ogs_assert(true ==
                        ogs_sbi_server_send_response(stream, response));
            } else {
                smf_npcf_smpolicycontrol_param_t param;

                memset(&param, 0, sizeof(param));

                param.ue_location = true;
                param.ue_timezone = true;

                ogs_assert(true ==
                    smf_sbi_discover_and_send(OpenAPI_nf_type_PCF, sess, stream,
                        OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT, &param,
                        smf_npcf_smpolicycontrol_build_delete));
            }
        } else {
            ogs_error("No PolicyAssociationId");
            smf_sbi_send_sm_context_update_error(
                    stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                    "No PolicyAssociationId", NULL, NULL, NULL);
        }
    } else {
        ogs_error("[%s:%d] No UpdateData", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No UpdateData", smf_ue->supi, NULL, NULL);
        return false;
    }

    return true;
}

bool smf_nsmf_handle_release_sm_context(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    smf_npcf_smpolicycontrol_param_t param;

    OpenAPI_sm_context_release_data_t *SmContextReleaseData = NULL;

    ogs_assert(stream);
    ogs_assert(message);
    ogs_assert(sess);

    memset(&param, 0, sizeof(param));

    SmContextReleaseData = message->SmContextReleaseData;
    if (SmContextReleaseData) {
        if (SmContextReleaseData->ue_location &&
            SmContextReleaseData->ue_location->nr_location) {
            OpenAPI_nr_location_t *NrLocation =
                SmContextReleaseData->ue_location->nr_location;
            if (NrLocation->tai &&
                NrLocation->tai->plmn_id && NrLocation->tai->tac &&
                NrLocation->ncgi &&
                NrLocation->ncgi->plmn_id && NrLocation->ncgi->nr_cell_id) {

                ogs_sbi_parse_nr_location(
                        &sess->nr_tai, &sess->nr_cgi, NrLocation);
                if (NrLocation->ue_location_timestamp)
                    ogs_sbi_time_from_string(&sess->ue_location_timestamp,
                            NrLocation->ue_location_timestamp);

                ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
                    ogs_plmn_id_hexdump(&sess->nr_tai.plmn_id),
                    sess->nr_tai.tac.v);
                ogs_debug("    NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]",
                    ogs_plmn_id_hexdump(&sess->nr_cgi.plmn_id),
                    (long long)sess->nr_cgi.cell_id);
            }

            param.ue_location = true;
            param.ue_timezone = true;
        }

        if (SmContextReleaseData->ng_ap_cause) {
            param.ran_nas_release.ngap_cause.group =
                SmContextReleaseData->ng_ap_cause->group;
            param.ran_nas_release.ngap_cause.value =
                SmContextReleaseData->ng_ap_cause->value;
        }
        param.ran_nas_release.gmm_cause =
            SmContextReleaseData->_5g_mm_cause_value;
    }

    if (sess->policy_association_id) {
        ogs_assert(true ==
            smf_sbi_discover_and_send(OpenAPI_nf_type_PCF, sess, stream,
                OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT, &param,
                smf_npcf_smpolicycontrol_build_delete));
    } else {
        ogs_error("No PolicyAssociationId");
        ogs_assert(true ==
            ogs_sbi_server_send_error(
                stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                NULL, "No PolicyAssociationId", NULL));
    }

    return true;
}
