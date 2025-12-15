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

#include "sbi-path.h"
#include "nas-path.h"
#include "ngap-path.h"
#include "pfcp-path.h"
#include "nsmf-handler.h"

bool smf_nsmf_handle_create_sm_context(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    bool rc;
    smf_ue_t *smf_ue = NULL;
    char *type = NULL;

    ogs_nas_5gsm_header_t *gsm_header = NULL;
    ogs_pkbuf_t *n1smbuf = NULL;

    ogs_sbi_client_t *client = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    char *fqdn = NULL;
    uint16_t fqdn_port = 0;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

    OpenAPI_sm_context_create_data_t *SmContextCreateData = NULL;
    OpenAPI_nr_location_t *NrLocation = NULL;
    OpenAPI_snssai_t *sNssai = NULL;
    OpenAPI_plmn_id_nid_t *servingNetwork = NULL;
    OpenAPI_ref_to_binary_data_t *n1SmMsg = NULL;

    ogs_assert(stream);
    ogs_assert(message);

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    SmContextCreateData = message->SmContextCreateData;
    if (!SmContextCreateData) {
        ogs_error("[%s:%d] No SmContextCreateData",
                smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No SmContextCreateData", smf_ue->supi, NULL);
        return false;
    }

    n1SmMsg = SmContextCreateData->n1_sm_msg;
    if (!n1SmMsg || !n1SmMsg->content_id) {
        ogs_error("[%s:%d] No n1SmMsg", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No n1SmMsg", smf_ue->supi, NULL);
        return false;
    }

    n1smbuf = ogs_sbi_find_part_by_content_id(message, n1SmMsg->content_id);
    if (!n1smbuf) {
        ogs_error("[%s:%d] No N1 SM Content [%s]",
                smf_ue->supi, sess->psi, n1SmMsg->content_id);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No N1 SM Content", smf_ue->supi, NULL);
        return false;
    }

    gsm_header = (ogs_nas_5gsm_header_t *)n1smbuf->data;
    ogs_assert(gsm_header);

    sess->pti = gsm_header->procedure_transaction_identity;
    if (sess->pti == OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED) {
        ogs_error("[%s:%d] No PTI", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No PTI", smf_ue->supi, NULL);
        return false;

    }

    sNssai = SmContextCreateData->s_nssai;
    if (!sNssai) {
        ogs_error("[%s:%d] No sNssai", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
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
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No servingNetwork", smf_ue->supi, n1smbuf);
        return false;
    }

    if (!SmContextCreateData->ue_location ||
        !SmContextCreateData->ue_location->nr_location) {
        ogs_error("[%s:%d] No UeLocation", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
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
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No UeLocation", smf_ue->supi, n1smbuf);
        return false;
    }

    if (!SmContextCreateData->sm_context_status_uri) {
        ogs_error("[%s:%d] No SmContextStatusNotification",
                smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No SmContextStatusNotification", smf_ue->supi, n1smbuf);
        return false;
    }

    rc = ogs_sbi_getaddr_from_uri(&scheme, &fqdn, &fqdn_port, &addr, &addr6,
            SmContextCreateData->sm_context_status_uri);
    if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
        ogs_error("[%s:%d] Invalid URI [%s]",
                smf_ue->supi, sess->psi,
                SmContextCreateData->sm_context_status_uri);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "Invalid URI",
                SmContextCreateData->sm_context_status_uri, n1smbuf);
        return false;
    }

    if (SmContextCreateData->supi) {
        type = ogs_id_get_type(SmContextCreateData->supi);
        if (type) {
            if (strncmp(type, OGS_ID_SUPI_TYPE_IMSI,
                        strlen(OGS_ID_SUPI_TYPE_IMSI)) == 0) {
                char *imsi_bcd = ogs_id_get_value(SmContextCreateData->supi);

                ogs_cpystrn(smf_ue->imsi_bcd, imsi_bcd,
                        ogs_min(strlen(imsi_bcd), OGS_MAX_IMSI_BCD_LEN)+1);
                ogs_bcd_to_buffer(smf_ue->imsi_bcd,
                        smf_ue->imsi, &smf_ue->imsi_len);

                ogs_free(imsi_bcd);
            }
            ogs_free(type);
        }
    }

    if (SmContextCreateData->pei) {
        type = ogs_id_get_type(SmContextCreateData->pei);
        if (type) {
            if (strncmp(type, OGS_ID_SUPI_TYPE_IMEISV,
                        strlen(OGS_ID_SUPI_TYPE_IMEISV)) == 0) {
                char *imeisv_bcd = ogs_id_get_value(SmContextCreateData->pei);

                ogs_cpystrn(smf_ue->imeisv_bcd, imeisv_bcd,
                        ogs_min(strlen(imeisv_bcd), OGS_MAX_IMEISV_BCD_LEN)+1);
                ogs_bcd_to_buffer(smf_ue->imeisv_bcd,
                        smf_ue->imeisv, &smf_ue->imeisv_len);

                ogs_free(imeisv_bcd);
            }
            ogs_free(type);
        }
    }

    if (SmContextCreateData->gpsi) {
        type = ogs_id_get_type(SmContextCreateData->gpsi);
        if (type) {
            if (strncmp(type, OGS_ID_GPSI_TYPE_MSISDN,
                        strlen(OGS_ID_GPSI_TYPE_MSISDN)) == 0) {
                char *msisdn_bcd = ogs_id_get_value(SmContextCreateData->gpsi);

                ogs_cpystrn(smf_ue->msisdn_bcd, msisdn_bcd,
                        ogs_min(strlen(msisdn_bcd), OGS_MAX_MSISDN_BCD_LEN)+1);
                ogs_bcd_to_buffer(smf_ue->msisdn_bcd,
                        smf_ue->msisdn, &smf_ue->msisdn_len);

                ogs_free(msisdn_bcd);
            }
            ogs_free(type);
        }
    }

    /* Serving PLMN & Home PLMN */
    ogs_sbi_parse_plmn_id_nid(&sess->serving_plmn_id, servingNetwork);
    memcpy(&sess->home_plmn_id, &sess->serving_plmn_id, OGS_PLMN_ID_LEN);

    sess->sbi_rat_type = SmContextCreateData->rat_type;

    ogs_sbi_parse_nr_location(&sess->nr_tai, &sess->nr_cgi, NrLocation);
    if (NrLocation->ue_location_timestamp)
        ogs_sbi_time_from_string(&sess->ue_location_timestamp,
                NrLocation->ue_location_timestamp);

    sess->s_nssai.sst = sNssai->sst;
    sess->s_nssai.sd = ogs_s_nssai_sd_from_string(sNssai->sd);
    if (SmContextCreateData->hplmn_snssai) {
        sess->mapped_hplmn_presence = true;
        sess->mapped_hplmn.sst = SmContextCreateData->hplmn_snssai->sst;
        sess->mapped_hplmn.sd = ogs_s_nssai_sd_from_string(
                                    SmContextCreateData->hplmn_snssai->sd);
    }

    smf_metrics_inst_by_slice_add(&sess->serving_plmn_id, &sess->s_nssai,
            SMF_METR_GAUGE_SM_SESSIONNBR, 1);
    smf_metrics_inst_by_slice_add(&sess->serving_plmn_id, &sess->s_nssai,
            SMF_METR_CTR_SM_PDUSESSIONCREATIONREQ, 1);

    if (sess->sm_context_status_uri)
        ogs_free(sess->sm_context_status_uri);
    sess->sm_context_status_uri =
        ogs_strdup(SmContextCreateData->sm_context_status_uri);
    ogs_assert(sess->sm_context_status_uri);

    client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
    if (!client) {
        ogs_debug("%s: ogs_sbi_client_add()", OGS_FUNC);
        client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            ogs_error("%s: ogs_sbi_client_add() failed", OGS_FUNC);

            ogs_free(fqdn);
            ogs_freeaddrinfo(addr);
            ogs_freeaddrinfo(addr6);

            return false;
        }
    }
    OGS_SBI_SETUP_CLIENT(&sess->namf, client);

    ogs_free(fqdn);
    ogs_freeaddrinfo(addr);
    ogs_freeaddrinfo(addr6);

    /*
     * TS29.502
     * 6.1 Nsmf_PDUSession Service API
     * Table 6.1.6.2.2-1: Definition of type SmContextCreateData
     *
     * NAME: dnn
     * Data type: Dnn
     * P: C
     * Cardinality: 0..1
     *
     * This IE shall be present, except during an EPS to 5GS Idle mode mobility
     * or handover using the N26 interface.
     *
     * When present, it shall contain the requested DNN; the DNN shall
     * be the full DNN (i.e. with both the Network Identifier and
     * Operator Identifier) for a HR PDU session, and it should be
     * the full DNN in LBO and non-roaming scenarios. If the Operator Identifier
     * is absent, the serving core network operator shall be assumed.
     */
    if (SmContextCreateData->dnn) {
        char *home_network_domain =
            ogs_home_network_domain_from_fqdn(SmContextCreateData->dnn);

        if (home_network_domain) {
            char dnn_network_identifer[OGS_MAX_DNN_LEN+1];
            uint16_t mcc = 0, mnc = 0;

            ogs_assert(home_network_domain > SmContextCreateData->dnn);

            ogs_cpystrn(dnn_network_identifer, SmContextCreateData->dnn,
                ogs_min(OGS_MAX_DNN_LEN,
                    home_network_domain - SmContextCreateData->dnn));

            if (sess->session.name)
                ogs_free(sess->session.name);
            sess->session.name = ogs_strdup(dnn_network_identifer);
            ogs_assert(sess->session.name);

            if (sess->full_dnn)
                ogs_free(sess->full_dnn);
            sess->full_dnn = ogs_strdup(SmContextCreateData->dnn);
            ogs_assert(sess->full_dnn);

            mcc = ogs_plmn_id_mcc_from_fqdn(sess->full_dnn);
            mnc = ogs_plmn_id_mnc_from_fqdn(sess->full_dnn);

            /*
             * To generate the Home PLMN ID of the SMF-UE,
             * the length of the MNC is obtained
             * by comparing the MNC part of the SUPI and full-DNN.
             */
            if (mcc && mnc &&
                strncmp(smf_ue->supi, "imsi-", strlen("imsi-")) == 0) {
                int mnc_len = 0;
                char buf[OGS_PLMNIDSTRLEN];

                ogs_snprintf(buf, OGS_PLMNIDSTRLEN, "%03d%02d", mcc, mnc);
                if (strncmp(smf_ue->supi + 5, buf, strlen(buf)) == 0)
                    mnc_len = 2;

                ogs_snprintf(buf, OGS_PLMNIDSTRLEN, "%03d%03d", mcc, mnc);
                if (strncmp(smf_ue->supi + 5, buf, strlen(buf)) == 0)
                    mnc_len = 3;

                /* Change Home PLMN for VPLMN */
                if (mnc_len == 2 || mnc_len == 3)
                    ogs_plmn_id_build(&sess->home_plmn_id, mcc, mnc, mnc_len);
            }
        } else {
            if (sess->session.name)
                ogs_free(sess->session.name);
            sess->session.name = ogs_strdup(SmContextCreateData->dnn);
            ogs_assert(sess->session.name);

            if (sess->full_dnn)
                ogs_free(sess->full_dnn);
            sess->full_dnn = NULL;
        }
    }

    if (SmContextCreateData->pcf_id) {
        if (sess->pcf_id) ogs_free(sess->pcf_id);
        sess->pcf_id = ogs_strdup(SmContextCreateData->pcf_id);
        ogs_assert(sess->pcf_id);
    }

    if (SmContextCreateData->serving_nf_id) {
        if (sess->serving_nf_id) ogs_free(sess->serving_nf_id);
        sess->serving_nf_id = ogs_strdup(SmContextCreateData->serving_nf_id);
        ogs_assert(sess->serving_nf_id);
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
    int r;
    smf_ue_t *smf_ue = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_sm_context_update_data_t *SmContextUpdateData = NULL;
    OpenAPI_ref_to_binary_data_t *n1SmMsg = NULL;
    OpenAPI_ref_to_binary_data_t *n2SmMsg = NULL;

    ogs_nas_5gsm_header_t *gsm_header = NULL;
    ogs_pkbuf_t *n1smbuf = NULL;
    ogs_pkbuf_t *n2smbuf = NULL;

    ogs_assert(stream);
    ogs_assert(message);

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    SmContextUpdateData = message->SmContextUpdateData;
    if (!SmContextUpdateData) {
        ogs_error("[%s:%d] No SmContextUpdateData",
                smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error_log(
                stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No SmContextUpdateData", smf_ue->supi);
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
            smf_sbi_send_sm_context_update_error_log(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No n1SmMsg", smf_ue->supi);
            return false;
        }

        n1smbuf = ogs_sbi_find_part_by_content_id(message, n1SmMsg->content_id);
        if (!n1smbuf) {
            ogs_error("[%s:%d] No N1 SM Content [%s]",
                    smf_ue->supi, sess->psi, n1SmMsg->content_id);
            smf_sbi_send_sm_context_update_error_log(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No N1 SM Content", smf_ue->supi);
            return false;
        }

        gsm_header = (ogs_nas_5gsm_header_t *)n1smbuf->data;
        ogs_assert(gsm_header);
        sess->pti = gsm_header->procedure_transaction_identity;

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
            smf_sbi_send_sm_context_update_error_log(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No n2SmInfoType", smf_ue->supi);
            return false;
        }

        n2SmMsg = SmContextUpdateData->n2_sm_info;
        if (!n2SmMsg || !n2SmMsg->content_id) {
            ogs_error("[%s:%d] No N2SmInfo.content_id",
                    smf_ue->supi, sess->psi);
            smf_sbi_send_sm_context_update_error_log(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No n2SmInfo.content_id", smf_ue->supi);
            return false;
        }

        n2smbuf = ogs_sbi_find_part_by_content_id(message, n2SmMsg->content_id);
        if (!n2smbuf) {
            ogs_error("[%s:%d] No N2 SM Content", smf_ue->supi, sess->psi);
            smf_sbi_send_sm_context_update_error_log(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No N2 SM Content", smf_ue->supi);
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

        sess->up_cnx_state = SmContextUpdateData->up_cnx_state;

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
                    smf_5gc_pfcp_send_all_pdr_modification_request(
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

            if (!OGS_FSM_CHECK(&sess->sm, smf_gsm_state_operational)) {
            /*
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
             * 11. ...
             * If the SMF decided to change the PSA UPF for the requested
             * PDU Session as described in step 5b. In this case, after sending
             * Nsmf_PDUSession_UpdateSMContext Response, the SMF triggers
             * another procedure to instruct UE to re-establish the PDU Session
             * as described in clause 4.3.5.1 for SSC mode 2.
             */
                ogs_error("[%s:%d] Service-Request: ACTIVATING FAILED",
                        smf_ue->supi, sess->psi);

                smf_sbi_send_sm_context_update_app_error(
                        stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                        OGS_SBI_APP_ERRNO_PDU_SESSION_ANCHOR_CHANGE,
                        OpenAPI_up_cnx_state_DEACTIVATED);

                return false;
            }

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
            smf_sbi_send_sm_context_update_error_log(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST, strerror, NULL);
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
                    smf_5gc_pfcp_send_all_pdr_modification_request(
                        sess, stream,
                        OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE|
                        OGS_PFCP_MODIFY_N2_HANDOVER|OGS_PFCP_MODIFY_END_MARKER,
                        0));
            } else {
                char *strerror = ogs_msprintf(
                        "[%s:%d] No FAR Update", smf_ue->supi, sess->psi);
                ogs_assert(strerror);

                ogs_error("%s", strerror);
                smf_sbi_send_sm_context_update_error_log(
                        stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        strerror, NULL);
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
                    smf_5gc_pfcp_send_all_pdr_modification_request(
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
            smf_sbi_send_sm_context_update_error_log(
                stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST, strerror, NULL);
            ogs_free(strerror);

            return false;
        }
    } else if (SmContextUpdateData->is_release == true &&
                SmContextUpdateData->release == true) {

        /* First of all, it checks for REL_DUE_TO_DUPLICATE_SESSION_ID */
        if (sess->ngap_state.pdu_session_resource_release ==
                SMF_NGAP_STATE_DELETE_TRIGGER_UE_REQUESTED) {

            /* PCF session context has already been removed */
            memset(&sendmsg, 0, sizeof(sendmsg));

            response = ogs_sbi_build_response(
                &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_assert(true ==
                    ogs_sbi_server_send_response(stream, response));

        } else if (PCF_SM_POLICY_ASSOCIATED(sess)) {
            smf_npcf_smpolicycontrol_param_t param;

            memset(&param, 0, sizeof(param));

            param.ue_location = true;
            param.ue_timezone = true;

            r = smf_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NPCF_SMPOLICYCONTROL, NULL,
                    smf_npcf_smpolicycontrol_build_delete,
                    sess, stream,
                    OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT, &param);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        } else if (UDM_SDM_SUBSCRIBED(sess)) {
            ogs_warn("[%s:%d] No PolicyAssociationId. Forcibly remove SESSION",
                    smf_ue->supi, sess->psi);
            r = smf_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NUDM_SDM, NULL,
                    smf_nudm_sdm_build_subscription_delete, sess, stream,
                    SMF_UECM_STATE_DEREGISTERED_BY_AMF, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        } else {
            ogs_warn("[%s:%d] No UDM Subscription. Forcibly remove SESSION",
                    smf_ue->supi, sess->psi);
            r = smf_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NUDM_UECM, NULL,
                    smf_nudm_uecm_build_deregistration, sess, stream,
                    SMF_UECM_STATE_DEREGISTERED_BY_AMF, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
    } else if (SmContextUpdateData->serving_nf_id) {
        ogs_debug("Old serving_nf_id: %s, new serving_nf_id: %s",
            sess->serving_nf_id, SmContextUpdateData->serving_nf_id);

        if (sess->serving_nf_id)
            ogs_free(sess->serving_nf_id);
        sess->serving_nf_id = ogs_strdup(SmContextUpdateData->serving_nf_id);
        ogs_assert(sess->serving_nf_id);

        memset(&sendmsg, 0, sizeof(sendmsg));
        response = ogs_sbi_build_response(
            &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
    } else {
        ogs_error("[%s:%d] No UpdateData", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error_log(
                stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No UpdateData", smf_ue->supi);
        return false;
    }

    return true;
}

bool smf_nsmf_handle_release_sm_context(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    int r;
    smf_npcf_smpolicycontrol_param_t param;
    smf_ue_t *smf_ue = NULL;

    OpenAPI_sm_context_release_data_t *SmContextReleaseData = NULL;

    ogs_assert(stream);
    ogs_assert(message);
    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

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

    if (PCF_SM_POLICY_ASSOCIATED(sess)) {
        r = smf_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NPCF_SMPOLICYCONTROL, NULL,
                smf_npcf_smpolicycontrol_build_delete,
                sess, stream,
                OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else if (UDM_SDM_SUBSCRIBED(sess)) {
        ogs_warn("[%s:%d] No PolicyAssociationId. Forcibly remove SESSION",
                smf_ue->supi, sess->psi);
        r = smf_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NUDM_SDM, NULL,
                smf_nudm_sdm_build_subscription_delete, sess, stream,
                SMF_UECM_STATE_DEREGISTERED_BY_AMF, NULL);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else {
        ogs_warn("[%s:%d] No UDM Subscription. Forcibly remove SESSION",
                smf_ue->supi, sess->psi);
        r = smf_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NUDM_UECM, NULL,
                smf_nudm_uecm_build_deregistration, sess, stream,
                SMF_UECM_STATE_DEREGISTERED_BY_AMF, NULL);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }

    return true;
}


bool smf_nsmf_callback_handle_sdm_data_change_notify(
    ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int status = OGS_SBI_HTTP_STATUS_NO_CONTENT;
    smf_ue_t *smf_ue = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_modification_notification_t *ModificationNotification;
    OpenAPI_lnode_t *node;

    char *ueid = NULL;
    char *res_name = NULL;

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

        smf_ue = smf_ue_find_by_supi(ueid);
        if (!smf_ue) {
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
        CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
            OpenAPI_lnode_t *node_ci;

            OpenAPI_list_for_each(item->changes, node_ci) {
                /*
                TODO:
                OpenAPI_change_item_t *change_item = node_ci->data;
                */
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


cleanup:
    if (ueid)
        ogs_free(ueid);
    if (res_name)
        ogs_free(res_name);

    memset(&sendmsg, 0, sizeof(sendmsg));

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return true;
}

