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
    ogs_sbi_parse_nr_location(&sess->nr_tai, &sess->nr_cgi, NrLocation);
    if (NrLocation->ue_location_timestamp)
        ogs_sbi_time_from_string(&sess->ue_location_timestamp,
                NrLocation->ue_location_timestamp);

    sess->s_nssai.sst = sNssai->sst;
    sess->s_nssai.sd = ogs_s_nssai_sd_from_string(sNssai->sd);
    if (SmContextCreateData->hplmn_snssai) {
        sess->s_nssai.mapped_hplmn_sst =
            SmContextCreateData->hplmn_snssai->sst;
        sess->s_nssai.mapped_hplmn_sd =
            ogs_s_nssai_sd_from_string(
                    SmContextCreateData->hplmn_snssai->sd);
    }

    if (sess->sm_context_status_uri)
        ogs_free(sess->sm_context_status_uri);
    sess->sm_context_status_uri =
        ogs_strdup(SmContextCreateData->sm_context_status_uri);

    client = ogs_sbi_client_find(addr);
    if (!client) {
        client = ogs_sbi_client_add(addr);
        ogs_assert(client);
    }
    OGS_SETUP_SBI_CLIENT(&sess->namf, client);

    ogs_freeaddrinfo(addr);

    if (SmContextCreateData->dnn) {
        if (sess->dnn) ogs_free(sess->dnn);
        sess->dnn = ogs_strdup(SmContextCreateData->dnn);
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

    } else {
        if (!SmContextUpdateData->up_cnx_state) {
            ogs_error("[%s:%d] No upCnxState", smf_ue->supi, sess->psi);
            smf_sbi_send_sm_context_update_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No upCnxState", smf_ue->supi, NULL, NULL);
            return false;
        }

        if (SmContextUpdateData->up_cnx_state ==
                OpenAPI_up_cnx_state_DEACTIVATED) {

        /*********************************************************
         * Handle DEACTIVATED
         ********************************************************/
            smf_5gc_pfcp_send_session_modification_request(
                    sess, stream, OGS_PFCP_MODIFY_DEACTIVATE);

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
            /* Only ACTIVING & DEACTIVATED is Included */
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
            ogs_sbi_server_send_response(stream, response);

            for (i = 0; i < sendmsg.num_of_part; i++)
                if (sendmsg.part[i].pkbuf)
                    ogs_pkbuf_free(sendmsg.part[i].pkbuf);

        } else {
            char *strerror = ogs_msprintf("[%s:%d] Invalid upCnxState [%d]",
                smf_ue->supi, sess->psi,
                SmContextUpdateData->up_cnx_state);
            ogs_assert(strerror);

            ogs_error("%s", strerror);
            smf_sbi_send_sm_context_update_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, strerror,
                    NULL, NULL, NULL);
            ogs_free(strerror);

            return false;
        }
    }

    return true;
}

bool smf_nsmf_handle_release_sm_context(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    OpenAPI_sm_context_release_data_t *SmContextReleaseData = NULL;

    ogs_assert(stream);
    ogs_assert(message);
    ogs_assert(sess);

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
        }
    }

    smf_5gc_pfcp_send_session_deletion_request(sess, stream,
            OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT);

    return true;
}
