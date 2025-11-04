/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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
#include "local-path.h"
#include "gsm-handler.h"
#include "nsmf-handler.h"

bool smf_nsmf_handle_create_sm_context(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    bool rc;
    smf_ue_t *smf_ue = NULL;
    smf_bearer_t *qos_flow = NULL;
    ogs_pfcp_pdr_t *dl_pdr = NULL;
    ogs_pfcp_pdr_t *ul_pdr = NULL;
    char *type = NULL;

    ogs_nas_5gsm_header_t *gsm_header = NULL;
    ogs_pkbuf_t *n1smbuf = NULL;

    ogs_sbi_client_t *client = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    char *fqdn = NULL;
    uint16_t fqdn_port = 0;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
    char *home_network_domain = NULL;

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

    if (!SmContextCreateData->dnn) {
        ogs_error("[%s:%d] No DNN", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No DNN", smf_ue->supi, n1smbuf);
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

    if (!SmContextCreateData->serving_nf_id) {
        ogs_error("[%s:%d] No Serving AMF NF-ID", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No Serving AMF NF-ID", smf_ue->supi, n1smbuf);
        return false;
    }

    if (!SmContextCreateData->guami) {
        ogs_error("[%s:%d] No Guami", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No Guami", smf_ue->supi, n1smbuf);
        return false;
    }

    servingNetwork = SmContextCreateData->serving_network;
    if (!servingNetwork || !servingNetwork->mnc || !servingNetwork->mcc) {
        ogs_error("[%s:%d] No servingNetwork", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No servingNetwork", smf_ue->supi, n1smbuf);
        return false;
    }

    if (!SmContextCreateData->an_type) {
        ogs_error("[%s:%d] No anType", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No anType", smf_ue->supi, n1smbuf);
        return false;
    }

    if (!SmContextCreateData->rat_type) {
        ogs_error("[%s:%d] No ratType", smf_ue->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                "No ratType", smf_ue->supi, n1smbuf);
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

        if (smf_ue->gpsi)
            ogs_free(smf_ue->gpsi);
        smf_ue->gpsi = ogs_strdup(SmContextCreateData->gpsi);
    }

    sess->request_type = SmContextCreateData->request_type;

    ogs_sbi_parse_plmn_id_nid(&sess->serving_plmn_id, servingNetwork);
    memcpy(&sess->home_plmn_id, &sess->serving_plmn_id, OGS_PLMN_ID_LEN);

    sess->an_type = SmContextCreateData->an_type;
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
    home_network_domain =
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

    ogs_assert(SmContextCreateData->serving_nf_id);
    if (sess->amf_nf_id) ogs_free(sess->amf_nf_id);
    sess->amf_nf_id = ogs_strdup(SmContextCreateData->serving_nf_id);
    ogs_assert(sess->amf_nf_id);

    ogs_assert(SmContextCreateData->guami);
    ogs_sbi_parse_guami(&sess->guami, SmContextCreateData->guami);

    if (SmContextCreateData->pcf_id) {
        if (sess->pcf_id) ogs_free(sess->pcf_id);
        sess->pcf_id = ogs_strdup(SmContextCreateData->pcf_id);
        ogs_assert(sess->pcf_id);
    }

    if (SmContextCreateData->h_smf_uri) {
        if (sess->h_smf_uri) ogs_free(sess->h_smf_uri);
        sess->h_smf_uri = ogs_strdup(SmContextCreateData->h_smf_uri);
        ogs_assert(sess->h_smf_uri);

        rc = ogs_sbi_getaddr_from_uri(
                &scheme, &fqdn, &fqdn_port, &addr, &addr6, sess->h_smf_uri);
        if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
            ogs_error("[%s:%d] Invalid URI [%s]",
                    smf_ue->supi, sess->psi, sess->h_smf_uri);
            return OGS_ERROR;
        }

        client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            ogs_debug("%s: ogs_sbi_client_add()", OGS_FUNC);
            client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
            if (!client) {
                ogs_error("%s: ogs_sbi_client_add() failed", OGS_FUNC);

                ogs_free(fqdn);
                ogs_freeaddrinfo(addr);
                ogs_freeaddrinfo(addr6);

                return OGS_ERROR;
            }
        }
        OGS_SBI_SETUP_CLIENT(&sess->h_smf, client);

        ogs_free(fqdn);
        ogs_freeaddrinfo(addr);
        ogs_freeaddrinfo(addr6);
    }

    if (!HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
    /***********************************************************************
     * CASE 1 - Non-Roaming or LBO
     * Send N1 Message to the GSM Message Handler
     ***********************************************************************/
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

    /***********************************************************************
     * CASE 2 - Home-Routed Roaming (In VSMF)
     * Send PFCP Session Establiashment Request to the UPF
     ***********************************************************************/

    /*********************************************************************
     * If Home-Routed Roaming,
     * Send HTTP_STATUS_CREATED(/nsmf-pdusession/v1/sm-context) to the AMF
     *********************************************************************/
    smf_sbi_send_sm_context_created_data(sess, stream);

    /* Select UPF based on UE Location Information */
    smf_sess_select_upf(sess);

    /* Check if UPF selection was successful */
    if (!sess->pfcp_node) {
        ogs_error("[%s:%d] No UPF available for session",
                  smf_ue->supi, sess->psi);
        return false;
    }

    /* Check if selected UPF is associated with SMF */
    if (!OGS_FSM_CHECK(&sess->pfcp_node->sm, smf_pfcp_state_associated)) {
        ogs_error("[%s:%d] No associated UPF", smf_ue->supi, sess->psi);
        return false;
    }

    /* Remove all previous QoS flow */
    smf_bearer_remove_all(sess);

    /* Setup Visited CN Tunnel */
    qos_flow = smf_vcn_tunnel_add(sess);
    ogs_assert(qos_flow);

    /* Setup PDR */
    dl_pdr = qos_flow->dl_pdr;
    ogs_assert(dl_pdr);
    ul_pdr = qos_flow->ul_pdr;
    ogs_assert(ul_pdr);

    ogs_assert(sess->pfcp_node);
    if (sess->pfcp_node->up_function_features.ftup) {
        dl_pdr->f_teid.ipv4 = 1;
        dl_pdr->f_teid.ipv6 = 1;
        dl_pdr->f_teid.ch = 1;
        dl_pdr->f_teid_len = 1;

        ul_pdr->f_teid.ipv4 = 1;
        ul_pdr->f_teid.ipv6 = 1;
        ul_pdr->f_teid.ch = 1;
        ul_pdr->f_teid_len = 1;
    } else {
        ogs_gtpu_resource_t *resource = NULL;

        resource = ogs_pfcp_find_gtpu_resource(
                &sess->pfcp_node->gtpu_resource_list,
                sess->session.name, dl_pdr->src_if);
        if (resource) {
            ogs_user_plane_ip_resource_info_to_sockaddr(&resource->info,
                &sess->local_dl_addr, &sess->local_dl_addr6);
            if (resource->info.teidri)
                sess->local_dl_teid = OGS_PFCP_GTPU_INDEX_TO_TEID(
                        dl_pdr->teid, resource->info.teidri,
                        resource->info.teid_range);
            else
                sess->local_dl_teid = dl_pdr->teid;
        } else {
            ogs_assert(sess->pfcp_node->addr_list);
            if (sess->pfcp_node->addr_list->ogs_sa_family == AF_INET)
                ogs_assert(OGS_OK ==
                    ogs_copyaddrinfo(
                        &sess->local_dl_addr, sess->pfcp_node->addr_list));
            else if (sess->pfcp_node->addr_list->ogs_sa_family == AF_INET6)
                ogs_assert(OGS_OK ==
                    ogs_copyaddrinfo(
                        &sess->local_dl_addr6, sess->pfcp_node->addr_list));
            else
                ogs_assert_if_reached();

            sess->local_dl_teid = dl_pdr->teid;
        }

        ogs_assert(OGS_OK ==
            ogs_pfcp_sockaddr_to_f_teid(
                sess->local_ul_addr, sess->local_ul_addr6,
                &dl_pdr->f_teid, &dl_pdr->f_teid_len));
        dl_pdr->f_teid.teid = sess->local_ul_teid;

        resource = ogs_pfcp_find_gtpu_resource(
                &sess->pfcp_node->gtpu_resource_list,
                sess->session.name, ul_pdr->src_if);
        if (resource) {
            ogs_user_plane_ip_resource_info_to_sockaddr(&resource->info,
                &sess->local_ul_addr, &sess->local_ul_addr6);
            if (resource->info.teidri)
                sess->local_ul_teid = OGS_PFCP_GTPU_INDEX_TO_TEID(
                        ul_pdr->teid, resource->info.teidri,
                        resource->info.teid_range);
            else
                sess->local_ul_teid = ul_pdr->teid;
        } else {
            ogs_assert(sess->pfcp_node->addr_list);
            if (sess->pfcp_node->addr_list->ogs_sa_family == AF_INET)
                ogs_assert(OGS_OK ==
                    ogs_copyaddrinfo(
                        &sess->local_ul_addr, sess->pfcp_node->addr_list));
            else if (sess->pfcp_node->addr_list->ogs_sa_family == AF_INET6)
                ogs_assert(OGS_OK ==
                    ogs_copyaddrinfo(
                        &sess->local_ul_addr6, sess->pfcp_node->addr_list));
            else
                ogs_assert_if_reached();

            sess->local_ul_teid = ul_pdr->teid;
        }

        ogs_assert(OGS_OK ==
            ogs_pfcp_sockaddr_to_f_teid(
                sess->local_ul_addr, sess->local_ul_addr6,
                &ul_pdr->f_teid, &ul_pdr->f_teid_len));
        ul_pdr->f_teid.teid = sess->local_ul_teid;
    }

    dl_pdr->precedence = OGS_PFCP_DEFAULT_PDR_PRECEDENCE;
    ul_pdr->precedence = OGS_PFCP_DEFAULT_PDR_PRECEDENCE;

    /* Save N1 SM Message and send it to H-SMF */
    if (sess->n1SmBufFromUe) ogs_pkbuf_free(sess->n1SmBufFromUe);
    sess->n1SmBufFromUe = ogs_pkbuf_copy(n1smbuf);
    ogs_assert(sess->n1SmBufFromUe);

    ogs_assert(OGS_OK ==
            smf_5gc_pfcp_send_session_establishment_request(sess, NULL, 0));

    return true;
}

bool smf_nsmf_handle_update_sm_context(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
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

    memset(&sess->nsmf_param, 0, sizeof(sess->nsmf_param));

    /* Remove N1 SM Message From UE */
    if (sess->n1SmBufFromUe) {
        ogs_pkbuf_free(sess->n1SmBufFromUe);
        sess->n1SmBufFromUe = NULL;
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

        sess->nsmf_param.ue_location = true;
        sess->nsmf_param.ue_timezone = true;
    }

    if (SmContextUpdateData->ng_ap_cause) {
        sess->nsmf_param.ngap_cause.group =
            SmContextUpdateData->ng_ap_cause->group;
        sess->nsmf_param.ngap_cause.value =
            SmContextUpdateData->ng_ap_cause->value;
    }
    sess->nsmf_param.gmm_cause =
        SmContextUpdateData->_5g_mm_cause_value;
    sess->nsmf_param.cause = SmContextUpdateData->cause;

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

        if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
            switch (gsm_header->message_type) {
            case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST:
            case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST:
                /* Save N1 SM Message and send it to H-SMF */
                sess->n1SmBufFromUe = ogs_pkbuf_copy(n1smbuf);
                ogs_assert(sess->n1SmBufFromUe);
                break;

            default:
                break;
            }
        }

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

        if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
    /*
     * UE-requested PDU Session Modification(ACTIVATED)
     *
     * 1.  V: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_DL_ONLY|
     *        OGS_PFCP_MODIFY_OUTER_HEADER_REMOVAL|OGS_PFCP_MODIFY_ACTIVATE
     * 2.  V: if (sess->up_cnx_state == OpenAPI_up_cnx_state_ACTIVATING)
     *            pfcp_flags |= OGS_PFCP_MODIFY_FROM_ACTIVATING;
     * 3.  V: flags & OGS_PFCP_MODIFY_FROM_ACTIVATING ?
     *           SMF_UPDATE_STATE_HR_ACTIVATED_FROM_ACTIVATING :
     *           SMF_UPDATE_STATE_HR_ACTIVATED_FROM_NON_ACTIVATING,
     * 4.  V: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 5.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 6.  H: smf_nsmf_handle_update_data_in_hsmf
     * 7.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 8.  H: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_DL_ONLY|
     *        OGS_PFCP_MODIFY_ACTIVATE
     * 9.  H: ogs_sbi_send_http_status_no_content
     * 10. V: case SMF_UPDATE_STATE_HR_ACTIVATED_FROM_ACTIVATING:
     *           sess->up_cnx_state = OpenAPI_up_cnx_state_ACTIVATED;
     *           smf_sbi_send_sm_context_updated_data_up_cnx_state(
     *               OpenAPI_up_cnx_state_ACTIVATED);
     *        case SMF_UPDATE_STATE_HR_ACTIVATED_FROM_NON_ACTIVATING:
     *           ogs_sbi_send_http_status_no_content
     */
            if (SmContextUpdateData->ue_location)
                sess->nsmf_param.ue_location = true;
            if (SmContextUpdateData->ue_time_zone)
                sess->nsmf_param.ue_timezone = true;
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
 * If UE initiates PDU Session Release, PFCP context is already removed.
 * In this case, skip PFCP deactivation and only send UP_CNX_STATE=DEACTIVATED.
 *
 * Typical flow:
 * 1. UE -> SMF: PDU Session Release Request
 * 2. SMF -> UPF: PFCP Session Deletion
 * 3. SMF -> UE : ReleaseCommand (NAS + NGAP)
 * 4. UE -> SMF: PDU Session Release Response
 *
 * If UE sends UEContextReleaseRequest after step 4,
 * PFCP session no longer exists in UPF.
 */
                smf_sbi_send_sm_context_updated_data_up_cnx_state(
                        sess, stream, OpenAPI_up_cnx_state_DEACTIVATED);
            } else {
                if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
    /*
     * UE-requested PDU Session Modification(DEACTIVATED)
     *
     * For Home Routed Roaming, delegate PFCP deactivation to H-SMF by
     * sending UP_CNX_STATE=DEACTIVATED via HsmfUpdateData.
     *
     * 1.  V*: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 2.  V*: smf_nsmf_pdusession_build_hsmf_update_data
     *         SMF_UPDATE_STATE_HR_DEACTIVATED
     * 3.  H: smf_nsmf_handle_update_data_in_hsmf
     * 4.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 5.  H: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_DL_ONLY|
     *        OGS_PFCP_MODIFY_DEACTIVATE
     * 6.  H: ogs_sbi_send_http_status_no_content
     * 7.  V: case SMF_UPDATE_STATE_HR_DEACTIVATED:
     * 8.  V: smf_sbi_send_sm_context_updated_data_up_cnx_state(
     *          OpenAPI_up_cnx_state_DEACTIVATED)
     */
                    sess->nsmf_param.request_indication =
                        OpenAPI_request_indication_UE_REQ_PDU_SES_MOD;

                    sess->nsmf_param.up_cnx_state =
                        OpenAPI_up_cnx_state_DEACTIVATED;

                    if (SmContextUpdateData->ue_location)
                        sess->nsmf_param.ue_location = true;
                    if (SmContextUpdateData->ue_time_zone)
                        sess->nsmf_param.ue_timezone = true;

                    if (SmContextUpdateData->ng_ap_cause) {
                        OpenAPI_ng_ap_cause_t *ngApCause =
                            SmContextUpdateData->ng_ap_cause;
                        sess->nsmf_param.ngap_cause.group = ngApCause->group;
                        sess->nsmf_param.ngap_cause.value = ngApCause->value;
                    }

                    r = smf_sbi_discover_and_send(
                            OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                            smf_nsmf_pdusession_build_hsmf_update_data,
                            sess, stream, SMF_UPDATE_STATE_DEACTIVATED, NULL);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);

                } else {
/*
 * For non-HRR sessions, directly send PFCP PDR modification with
 * DL-only deactivation to the local UPF.
 */
                    ogs_assert(OGS_OK ==
                        smf_5gc_pfcp_send_all_pdr_modification_request(
                            sess, stream,
                            OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_DEACTIVATE,
                            0, 0));
                }
            }

        } else if (SmContextUpdateData->up_cnx_state ==
                OpenAPI_up_cnx_state_ACTIVATING) {

        /*********************************************************
         * Handle ACTIVATING
         ********************************************************/
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

            if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
    /*
     * UE-requested PDU Session Modification(ACTIVATING)
     *
     * 1.  V*: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 2.  V*: smf_nsmf_pdusession_build_hsmf_update_data
     *         SMF_UPDATE_STATE_HR_ACTIVATING
     * 3.  H: smf_nsmf_handle_update_data_in_hsmf
     * 4.  H: OpenAPI_request_indication_UE_REQ_PDU_SES_MOD
     * 5.  H: ogs_sbi_send_http_status_no_content
     * 6.  V: ngap_build_pdu_session_resource_setup_request_transfer
     * 7.  V: smf_sbi_send_sm_context_updated_data(
     *          OpenAPI_up_cnx_state_ACTIVATING,
     *          OpenAPI_n2_sm_info_type_PDU_RES_SETUP_REQ, n2smbuf)
     */
                sess->nsmf_param.request_indication =
                    OpenAPI_request_indication_UE_REQ_PDU_SES_MOD;

                sess->nsmf_param.up_cnx_state =
                    SmContextUpdateData->up_cnx_state;

                r = smf_sbi_discover_and_send(
                        OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                        smf_nsmf_pdusession_build_hsmf_update_data,
                        sess, stream, SMF_UPDATE_STATE_ACTIVATING, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

            } else {
                ogs_pkbuf_t *n2smbuf =
                    ngap_build_pdu_session_resource_setup_request_transfer(
                            sess);
                ogs_assert(n2smbuf);

                smf_sbi_send_sm_context_updated_data(
                        sess, stream,
                        OpenAPI_up_cnx_state_ACTIVATING, 0, NULL,
                        OpenAPI_n2_sm_info_type_PDU_RES_SETUP_REQ, n2smbuf);
            }
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
                if (memcmp(&sess->remote_dl_ip, &sess->handover.remote_dl_ip,
                            sizeof(sess->remote_dl_ip)) != 0 ||
                    sess->remote_dl_teid != sess->handover.remote_dl_teid)
                    far_update = true;

                memcpy(&sess->remote_dl_ip,
                        &sess->handover.remote_dl_ip, sizeof(sess->remote_dl_ip));
                sess->remote_dl_teid = sess->handover.remote_dl_teid;
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
                            &sess->remote_dl_ip,
                            &dl_far->outer_header_creation,
                            &dl_far->outer_header_creation_len));
                    dl_far->outer_header_creation.teid = sess->remote_dl_teid;
                }
                dl_far->handover.prepared = false;
            }

            if (far_update) {
                uint64_t pfcp_flags =
                    OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE|
                    OGS_PFCP_MODIFY_N2_HANDOVER|OGS_PFCP_MODIFY_END_MARKER;

                if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
                    pfcp_flags |= OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING;
                    pfcp_flags |= OGS_PFCP_MODIFY_OUTER_HEADER_REMOVAL;
                }

                ogs_assert(OGS_OK ==
                    smf_5gc_pfcp_send_all_pdr_modification_request(
                        sess, stream, pfcp_flags, 0, 0));
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
                        0, 0));
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

        } else {
            if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
    /*
     * Network-requested PDU Session Release(DUPLICATED)
     *
     * 1.  V*: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|OGS_PFCP_MODIFY_UL_ONLY|
     *         OGS_PFCP_MODIFY_DEACTIVATE
     * 2.  V*: OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT,
     * 3.  V: OpenAPI_request_indication_NW_REQ_PDU_SES_REL
     * 4.  V: smf_nsmf_pdusession_build_hsmf_update_data
     * 5.  H: smf_nsmf_handle_update_data_in_hsmf
     * 6.  H: OpenAPI_request_indication_NW_REQ_PDU_SES_REL
     * 6.  H: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT
     * 7.  H: ogs_sbi_send_http_status_no_content
     * 8.  H: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 9.  H: smf_sbi_cleanup_session(SMF_UECM_STATE_DEREG_BY_AMF_HR
     *                                SMF_SBI_CLEANUP_MODE_POLICY_FIRST);
     * 10. H: OGS_FSM_TRAN(s, smf_gsm_state_5gc_session_will_deregister);
     * 11. H: SMF_SESS_CLEAR(sess)
     * 12. V: e->h.sbi.state = OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT
     * 13. V: OGS_FSM_TRAN(s, smf_gsm_state_wait_pfcp_deletion)
     * 14. V: ogs_sbi_send_http_status_no_content
     * 15. V: OGS_FSM_TRAN(s, smf_gsm_state_session_will_release);
     */
                ogs_assert(OGS_OK ==
                    smf_5gc_pfcp_send_all_pdr_modification_request(
                        sess, stream,
                        OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
                        OGS_PFCP_MODIFY_UL_ONLY|
                        OGS_PFCP_MODIFY_DEACTIVATE,
                        OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT, 0));
            } else {
                smf_trigger_session_release(
                        sess, stream,
                        OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT);
            }
        }
    } else if (SmContextUpdateData->serving_nf_id) {
        ogs_debug("Old amf_nf_id: %s, new amf_nf_id: %s",
            sess->amf_nf_id, SmContextUpdateData->serving_nf_id);

        if (sess->amf_nf_id)
            ogs_free(sess->amf_nf_id);
        sess->amf_nf_id = ogs_strdup(SmContextUpdateData->serving_nf_id);
        ogs_assert(sess->amf_nf_id);

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
    smf_ue_t *smf_ue = NULL;

    OpenAPI_sm_context_release_data_t *SmContextReleaseData = NULL;

    ogs_assert(stream);
    ogs_assert(message);
    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    memset(&sess->nsmf_param, 0, sizeof(sess->nsmf_param));

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

            sess->nsmf_param.ue_location = true;
            sess->nsmf_param.ue_timezone = true;
        }

        if (SmContextReleaseData->ng_ap_cause) {
            sess->nsmf_param.ngap_cause.group =
                SmContextReleaseData->ng_ap_cause->group;
            sess->nsmf_param.ngap_cause.value =
                SmContextReleaseData->ng_ap_cause->value;
        }
        sess->nsmf_param.gmm_cause =
            SmContextReleaseData->_5g_mm_cause_value;
        sess->nsmf_param.cause = SmContextReleaseData->cause;
    }

    return true;
}

bool smf_nsmf_handle_create_data_in_hsmf(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    bool rc;
    smf_ue_t *smf_ue = NULL;
    char *type = NULL;

    int rv, r;
    ogs_nas_5gs_message_t nas_message;
    ogs_pkbuf_t *n1SmBufFromUe = NULL;
    ogs_nas_pdu_session_type_t *pdu_session_type = NULL;

    ogs_nas_5gs_pdu_session_establishment_request_t
        *pdu_session_establishment_request = NULL;
    ogs_nas_ssc_mode_t *ssc_mode = NULL;

    ogs_sbi_client_t *client = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    char *fqdn = NULL;
    uint16_t fqdn_port = 0;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
    char *home_network_domain = NULL;

    OpenAPI_pdu_session_create_data_t *PduSessionCreateData = NULL;
    OpenAPI_nr_location_t *NrLocation = NULL;
    OpenAPI_snssai_t *sNssai = NULL;
    OpenAPI_plmn_id_nid_t *servingNetwork = NULL;
    OpenAPI_ref_to_binary_data_t *n1SmInfoFromUe = NULL;
    OpenAPI_tunnel_info_t *vcnTunnelInfo = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    PduSessionCreateData = recvmsg->PduSessionCreateData;
    if (!PduSessionCreateData) {
        ogs_error("[%s:%d] No PduSessionCreateData",
                smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No PduSessionCreateData", smf_ue->supi, NULL);
        return false;
    }

    n1SmInfoFromUe = PduSessionCreateData->n1_sm_info_from_ue;
    if (!n1SmInfoFromUe || !n1SmInfoFromUe->content_id) {
        ogs_error("[%s:%d] No n1SmInfoFromUe", smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No n1SmInfoFromUe", smf_ue->supi, NULL);
        return false;
    }

    n1SmBufFromUe = ogs_sbi_find_part_by_content_id(
            recvmsg, n1SmInfoFromUe->content_id);
    if (!n1SmBufFromUe) {
        ogs_error("[%s:%d] No N1 SM Content [%s]",
                smf_ue->supi, sess->psi, n1SmInfoFromUe->content_id);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No N1 SM Content", smf_ue->supi, NULL);
        return false;
    }

    rv = gsmue_decode_n1_sm_info(&nas_message, n1SmBufFromUe);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] cannot decode N1 SM Content [%s]",
                smf_ue->supi, sess->psi, n1SmInfoFromUe->content_id);
        ogs_log_hexdump(OGS_LOG_ERROR, n1SmBufFromUe->data, n1SmBufFromUe->len);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,
                "cannot decode N1 SM Content", smf_ue->supi, NULL);
        return false;
    }

    if (!PduSessionCreateData->dnn) {
        ogs_error("[%s:%d] No DNN", smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No DNN", smf_ue->supi, NULL);
        return false;
    }

    home_network_domain =
        ogs_home_network_domain_from_fqdn(PduSessionCreateData->dnn);

    if (home_network_domain) {
        char dnn_network_identifer[OGS_MAX_DNN_LEN+1];

        ogs_assert(home_network_domain > PduSessionCreateData->dnn);

        ogs_cpystrn(dnn_network_identifer, PduSessionCreateData->dnn,
            ogs_min(OGS_MAX_DNN_LEN,
                home_network_domain - PduSessionCreateData->dnn));

        if (sess->session.name)
            ogs_free(sess->session.name);
        sess->session.name = ogs_strdup(dnn_network_identifer);
        ogs_assert(sess->session.name);

        if (sess->full_dnn)
            ogs_free(sess->full_dnn);
        sess->full_dnn = ogs_strdup(PduSessionCreateData->dnn);
        ogs_assert(sess->full_dnn);
    } else {
        if (sess->session.name)
            ogs_free(sess->session.name);
        sess->session.name = ogs_strdup(PduSessionCreateData->dnn);
        ogs_assert(sess->session.name);

        if (sess->full_dnn)
            ogs_free(sess->full_dnn);
        sess->full_dnn = NULL;
    }

    sNssai = PduSessionCreateData->s_nssai;
    if (!sNssai) {
        ogs_error("[%s:%d] No sNssai", smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No sNssai", smf_ue->supi, NULL);
        return false;
    }

    if (!PduSessionCreateData->vsmf_id) {
        ogs_error("[%s:%d] No vsmfId", smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No vsmfId", smf_ue->supi, NULL);
        return false;
    }

    servingNetwork = PduSessionCreateData->serving_network;
    if (!servingNetwork || !servingNetwork->mnc || !servingNetwork->mcc) {
        ogs_error("[%s:%d] No servingNetwork",
                smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No servingNetwork", smf_ue->supi, NULL);
        return false;
    }

    if (!PduSessionCreateData->an_type) {
        ogs_error("[%s:%d] No anType", smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No anType", smf_ue->supi, NULL);
        return false;
    }

    if (!PduSessionCreateData->rat_type) {
        ogs_error("[%s:%d] No ratType", smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No ratType", smf_ue->supi, NULL);
        return false;
    }

    if (!PduSessionCreateData->ue_location ||
        !PduSessionCreateData->ue_location->nr_location) {
        ogs_error("[%s:%d] No UeLocation", smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No UeLocation", smf_ue->supi, NULL);
        return false;
    }

    NrLocation = PduSessionCreateData->ue_location->nr_location;
    if (!NrLocation->tai ||
        !NrLocation->tai->plmn_id || !NrLocation->tai->tac ||
        !NrLocation->ncgi ||
        !NrLocation->ncgi->plmn_id || !NrLocation->ncgi->nr_cell_id) {
        ogs_error("[%s:%d] No NrLocation", smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No UeLocation", smf_ue->supi, NULL);
        return false;
    }

    if (!PduSessionCreateData->amf_nf_id) {
        ogs_error("[%s:%d] No Serving AMF NF-ID", smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No Serving AMF NF-ID", smf_ue->supi, NULL);
        return false;
    }

    if (!PduSessionCreateData->guami) {
        ogs_error("[%s:%d] No Guami", smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No Guami", smf_ue->supi, NULL);
        return false;
    }

    vcnTunnelInfo = PduSessionCreateData->vcn_tunnel_info;
    if (!vcnTunnelInfo ||
        !(vcnTunnelInfo->ipv4_addr || vcnTunnelInfo->ipv6_addr) ||
        !vcnTunnelInfo->gtp_teid) {
        ogs_error("[%s:%d] No vcnTunnelInfo [%s:%s:%s]",
                smf_ue->supi, sess->psi,
                vcnTunnelInfo->ipv4_addr ? vcnTunnelInfo->ipv4_addr : "NULL",
                vcnTunnelInfo->ipv6_addr ? vcnTunnelInfo->ipv6_addr : "NULL",
                vcnTunnelInfo->gtp_teid ? vcnTunnelInfo->gtp_teid : "NULL");
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No vcnTunnelInfo", smf_ue->supi, NULL);
        return false;
    }

    memset(&sess->remote_dl_ip, 0, sizeof(sess->remote_dl_ip));
    if (vcnTunnelInfo->ipv4_addr) {
        rv = ogs_ipv4_from_string(
                &sess->remote_dl_ip.addr, vcnTunnelInfo->ipv4_addr);
        if (rv != OGS_OK) {
            ogs_error("ogs_ipv4_from_string() [%s] failed",
                    vcnTunnelInfo->ipv4_addr);
            smf_sbi_send_pdu_session_create_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                    OGS_5GSM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,
                    "ogs_ipv4_from_string() failed",
                    vcnTunnelInfo->ipv4_addr, NULL);
            return false;
        }
        sess->remote_dl_ip.ipv4 = 1;
        sess->remote_dl_ip.len = OGS_IPV4_LEN;
    }
    if (vcnTunnelInfo->ipv6_addr) {
        rv = ogs_ipv6addr_from_string(
                sess->remote_dl_ip.addr6, vcnTunnelInfo->ipv6_addr);
        if (rv != OGS_OK) {
            ogs_error("ogs_ipv6addr_from_string() [%s] failed",
                    vcnTunnelInfo->ipv6_addr);
            smf_sbi_send_pdu_session_create_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                    OGS_5GSM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,
                    "ogs_ipv6addr_from_string() failed",
                    vcnTunnelInfo->ipv6_addr, NULL);
            return false;
        }
        sess->remote_dl_ip.ipv6 = 1;
        sess->remote_dl_ip.len = OGS_IPV6_LEN;
    }

    if (sess->remote_dl_ip.ipv4 && sess->remote_dl_ip.ipv6)
        sess->remote_dl_ip.len = OGS_IPV4V6_LEN;

    if (vcnTunnelInfo->gtp_teid)
        sess->remote_dl_teid =
            ogs_uint64_from_string_hexadecimal(vcnTunnelInfo->gtp_teid);
    ogs_debug("vcnTunnelInfo->ipv4 = 0x%x", sess->remote_dl_ip.addr);
    ogs_log_hexdump(OGS_LOG_DEBUG, sess->remote_dl_ip.addr6, OGS_IPV6_LEN);
    ogs_debug("vcnTunnelInfo->gtp_teid = 0x%x", sess->remote_dl_teid);

    if (!PduSessionCreateData->max_integrity_protected_data_rate_ul ||
        !PduSessionCreateData->max_integrity_protected_data_rate_dl) {
        ogs_error("[%s:%d] No Max Integrity Protected Data",
                smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No Max Integrity Protected Data", smf_ue->supi, NULL);
        return false;
    }

    sess->integrity_protection.mbr_dl =
        PduSessionCreateData->max_integrity_protected_data_rate_dl;
    sess->integrity_protection.mbr_ul =
        PduSessionCreateData->max_integrity_protected_data_rate_ul;

    if (!PduSessionCreateData->vsmf_pdu_session_uri) {
        ogs_error("[%s:%d] No PduSessionStatusNotification",
                smf_ue->supi, sess->psi);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No PduSessionStatusNotification", smf_ue->supi, NULL);
        return false;
    }

    rc = ogs_sbi_getaddr_from_uri(&scheme, &fqdn, &fqdn_port, &addr, &addr6,
            PduSessionCreateData->vsmf_pdu_session_uri);
    if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
        ogs_error("[%s:%d] Invalid URI [%s]",
                smf_ue->supi, sess->psi,
                PduSessionCreateData->vsmf_pdu_session_uri);
        smf_sbi_send_pdu_session_create_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,
                "Invalid URI",
                PduSessionCreateData->vsmf_pdu_session_uri, NULL);
        return false;
    }

    if (sess->vsmf_pdu_session_uri)
        ogs_free(sess->vsmf_pdu_session_uri);
    sess->vsmf_pdu_session_uri =
        ogs_strdup(PduSessionCreateData->vsmf_pdu_session_uri);
    ogs_assert(sess->vsmf_pdu_session_uri);

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
    OGS_SBI_SETUP_CLIENT(&sess->v_smf, client);

    ogs_free(fqdn);
    ogs_freeaddrinfo(addr);
    ogs_freeaddrinfo(addr6);

    if (PduSessionCreateData->supi) {
        type = ogs_id_get_type(PduSessionCreateData->supi);
        if (type) {
            if (strncmp(type, OGS_ID_SUPI_TYPE_IMSI,
                        strlen(OGS_ID_SUPI_TYPE_IMSI)) == 0) {
                char *imsi_bcd = ogs_id_get_value(PduSessionCreateData->supi);

                ogs_cpystrn(smf_ue->imsi_bcd, imsi_bcd,
                        ogs_min(strlen(imsi_bcd), OGS_MAX_IMSI_BCD_LEN)+1);
                ogs_bcd_to_buffer(smf_ue->imsi_bcd,
                        smf_ue->imsi, &smf_ue->imsi_len);

                ogs_free(imsi_bcd);
            }
            ogs_free(type);
        }
    }

    if (PduSessionCreateData->pei) {
        type = ogs_id_get_type(PduSessionCreateData->pei);
        if (type) {
            if (strncmp(type, OGS_ID_SUPI_TYPE_IMEISV,
                        strlen(OGS_ID_SUPI_TYPE_IMEISV)) == 0) {
                char *imeisv_bcd = ogs_id_get_value(PduSessionCreateData->pei);

                ogs_cpystrn(smf_ue->imeisv_bcd, imeisv_bcd,
                        ogs_min(strlen(imeisv_bcd), OGS_MAX_IMEISV_BCD_LEN)+1);
                ogs_bcd_to_buffer(smf_ue->imeisv_bcd,
                        smf_ue->imeisv, &smf_ue->imeisv_len);

                ogs_free(imeisv_bcd);
            }
            ogs_free(type);
        }
    }

    if (PduSessionCreateData->gpsi) {
        type = ogs_id_get_type(PduSessionCreateData->gpsi);
        if (type) {
            if (strncmp(type, OGS_ID_GPSI_TYPE_MSISDN,
                        strlen(OGS_ID_GPSI_TYPE_MSISDN)) == 0) {
                char *msisdn_bcd = ogs_id_get_value(PduSessionCreateData->gpsi);

                ogs_cpystrn(smf_ue->msisdn_bcd, msisdn_bcd,
                        ogs_min(strlen(msisdn_bcd), OGS_MAX_MSISDN_BCD_LEN)+1);
                ogs_bcd_to_buffer(smf_ue->msisdn_bcd,
                        smf_ue->msisdn, &smf_ue->msisdn_len);

                ogs_free(msisdn_bcd);
            }
            ogs_free(type);
        }
    }

    sess->request_type = PduSessionCreateData->request_type;

    ogs_sbi_parse_plmn_id_nid(&sess->serving_plmn_id, servingNetwork);
    memcpy(&sess->home_plmn_id, &sess->serving_plmn_id, OGS_PLMN_ID_LEN);

    sess->an_type = PduSessionCreateData->an_type;
    sess->sbi_rat_type = PduSessionCreateData->rat_type;

    ogs_sbi_parse_nr_location(&sess->nr_tai, &sess->nr_cgi, NrLocation);
    if (NrLocation->ue_location_timestamp)
        ogs_sbi_time_from_string(&sess->ue_location_timestamp,
                NrLocation->ue_location_timestamp);

    if (sess->amf_nf_id)
        ogs_free(sess->amf_nf_id);
    sess->amf_nf_id = ogs_strdup(PduSessionCreateData->amf_nf_id);
    ogs_assert(sess->amf_nf_id);

    ogs_sbi_parse_guami(&sess->guami, PduSessionCreateData->guami);

    sess->s_nssai.sst = sNssai->sst;
    sess->s_nssai.sd = ogs_s_nssai_sd_from_string(sNssai->sd);
    if (PduSessionCreateData->hplmn_snssai) {
        sess->mapped_hplmn.sst = PduSessionCreateData->hplmn_snssai->sst;
        sess->mapped_hplmn.sd = ogs_s_nssai_sd_from_string(
                                    PduSessionCreateData->hplmn_snssai->sd);
    }

    /* Handle GSM Message from n1SmInfoFromUe */
    pdu_session_establishment_request =
        &nas_message.gsm.pdu_session_establishment_request;
    pdu_session_type = &pdu_session_establishment_request->pdu_session_type;
    ssc_mode = &pdu_session_establishment_request->ssc_mode;

    if (pdu_session_establishment_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_PRESENT)
        sess->ue_session_type = pdu_session_type->value;

    if (pdu_session_establishment_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_PRESENT)
        sess->ue_ssc_mode = ssc_mode->value;

    if (pdu_session_establishment_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        OGS_NAS_STORE_DATA(&sess->nas.ue_epco,
            &pdu_session_establishment_request->
                extended_protocol_configuration_options);
    }

    smf_metrics_inst_by_slice_add(&sess->serving_plmn_id, &sess->s_nssai,
            SMF_METR_GAUGE_SM_SESSIONNBR, 1);
    smf_metrics_inst_by_slice_add(&sess->serving_plmn_id, &sess->s_nssai,
            SMF_METR_CTR_SM_PDUSESSIONCREATIONREQ, 1);

    r = smf_sbi_discover_and_send(OGS_SBI_SERVICE_TYPE_NUDM_SDM, NULL,
            smf_nudm_sdm_build_get,
            sess, stream, 0, (char *)OGS_SBI_RESOURCE_NAME_SM_DATA);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return true;
}

bool smf_nsmf_handle_created_data_in_vsmf(
    smf_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    int rv;

    smf_ue_t *smf_ue = NULL;

    ogs_assert(recvmsg);

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_CREATED) {
        OpenAPI_pdu_session_created_data_t *PduSessionCreatedData = NULL;

        ogs_sbi_message_t message;
        ogs_sbi_header_t header;

        bool rc;
        ogs_sbi_client_t *client = NULL;
        OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
        char *fqdn = NULL;
        uint16_t fqdn_port = 0;
        ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

        ogs_ip_t ue_ip;
        uint8_t prefixlen;

        OpenAPI_tunnel_info_t *hcnTunnelInfo = NULL;
        OpenAPI_ambr_t *sessionAmbr = NULL;
        OpenAPI_list_t *qosFlowsSetupList = NULL;
        OpenAPI_qos_flow_profile_t *qosFlowProfile = NULL;
        OpenAPI_lnode_t *node = NULL;

        ogs_nas_5gs_message_t nas_message;
        ogs_pkbuf_t *n1SmBufToUe = NULL;
        OpenAPI_ref_to_binary_data_t *n1SmInfoToUe = NULL;

        ogs_nas_5gs_pdu_session_establishment_accept_t
            *pdu_session_establishment_accept = NULL;

        smf_bearer_t *qos_flow = NULL;
        ogs_pfcp_pdr_t *dl_pdr = NULL;
        ogs_pfcp_pdr_t *ul_pdr = NULL;
        ogs_pfcp_far_t *ul_far = NULL;

        qos_flow = smf_default_bearer_in_sess(sess);
        ogs_assert(qos_flow);
        ogs_assert(ogs_list_next(qos_flow) == NULL);

        PduSessionCreatedData = recvmsg->PduSessionCreatedData;

        n1SmInfoToUe = PduSessionCreatedData->n1_sm_info_to_ue;
        if (!n1SmInfoToUe || !n1SmInfoToUe->content_id) {
            ogs_error("[%s:%d] No n1SmInfoToUe", smf_ue->supi, sess->psi);
            return false;
        }

        n1SmBufToUe = ogs_sbi_find_part_by_content_id(
                recvmsg, n1SmInfoToUe->content_id);
        if (!n1SmBufToUe) {
            ogs_error("[%s:%d] No N1 SM Content [%s]",
                    smf_ue->supi, sess->psi, n1SmInfoToUe->content_id);
            return false;
        }

        rv = gsmue_decode_n1_sm_info(&nas_message, n1SmBufToUe);
        if (rv != OGS_OK) {
            ogs_error("[%s:%d] cannot decode N1 SM Content [%s]",
                    smf_ue->supi, sess->psi, n1SmInfoToUe->content_id);
            ogs_log_hexdump(OGS_LOG_ERROR, n1SmBufToUe->data, n1SmBufToUe->len);
            return false;
        }

        if (!PduSessionCreatedData->pdu_session_type) {
            ogs_error("[%s:%d] No pduSessionType", smf_ue->supi, sess->psi);
            return false;
        }

        sess->paa.session_type = sess->session.session_type =
            PduSessionCreatedData->pdu_session_type;

        memset(&ue_ip, 0, sizeof(ue_ip));
        prefixlen = 0;

        if (PduSessionCreatedData->ue_ipv4_address) {
            rv = ogs_ipv4_from_string(
                &ue_ip.addr, PduSessionCreatedData->ue_ipv4_address);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Invalid ue_ipv4_address [%s]",
                        smf_ue->supi, sess->psi,
                        PduSessionCreatedData->ue_ipv4_address);
                return false;
            }
            ue_ip.ipv4 = 1;
        }
        if (PduSessionCreatedData->ue_ipv6_prefix &&
            PduSessionCreatedData->ue_ipv6_interface_id) {
            rv = ogs_ipv6prefix_from_string(
                ue_ip.addr6, &prefixlen, PduSessionCreatedData->ue_ipv6_prefix);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Invalid ue_ipv6_prefix [%s]",
                        smf_ue->supi, sess->psi,
                        PduSessionCreatedData->ue_ipv6_prefix);
                return false;
            }
            ogs_hex_from_string(
                    PduSessionCreatedData->ue_ipv6_interface_id,
                    ue_ip.addr6+(OGS_IPV6_LEN>>1), OGS_IPV6_LEN>>1);
            ue_ip.ipv6 = 1;
        }

        if (sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV4) {
            if (ue_ip.ipv4 == 0) {
                ogs_error("[%s:%d] No ue_ipv4_address",
                        smf_ue->supi, sess->psi);
                return false;
            }
            sess->paa.addr = ue_ip.addr;
        } else if (sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV6) {
            if (ue_ip.ipv6 == 0) {
                ogs_error("[%s:%d] No ue_ipv6_prefix", smf_ue->supi, sess->psi);
                return false;
            }
            sess->paa.len = prefixlen;
            memcpy(sess->paa.addr6, ue_ip.addr6, OGS_IPV6_LEN);
        } else if (sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
            if (ue_ip.ipv4 == 0) {
                ogs_error("[%s:%d] No ue_ipv4_address",
                        smf_ue->supi, sess->psi);
                return false;
            }
            if (ue_ip.ipv6 == 0) {
                ogs_error("[%s:%d] No ue_ipv6_prefix", smf_ue->supi, sess->psi);
                return false;
            }
            sess->paa.both.addr = ue_ip.addr;
            sess->paa.both.len = prefixlen;
            memcpy(sess->paa.both.addr6, ue_ip.addr6, OGS_IPV6_LEN);
        } else {
            ogs_error("[%s:%d] Invalid sess->session.session_type [%d]",
                    smf_ue->supi, sess->psi, sess->session.session_type);
            return false;
        }

        if (!PduSessionCreatedData->ssc_mode) {
            ogs_error("[%s:%d] No sscMode", smf_ue->supi, sess->psi);
            return false;
        }

        sess->session.ssc_mode =
            ogs_from_hex(PduSessionCreatedData->ssc_mode[0]);
        if (sess->session.ssc_mode < OGS_SSC_MODE_1 ||
                sess->session.ssc_mode > OGS_SSC_MODE_3) {
            ogs_error("[%s:%d] Invalid sscMode [%d:%s]",
                    smf_ue->supi, sess->psi, sess->session.ssc_mode,
                    PduSessionCreatedData->ssc_mode);
            return false;
        }

        hcnTunnelInfo = PduSessionCreatedData->hcn_tunnel_info;
        if (!hcnTunnelInfo ||
            !(hcnTunnelInfo->ipv4_addr || hcnTunnelInfo->ipv6_addr) ||
            !hcnTunnelInfo->gtp_teid) {
            ogs_error("[%s:%d] No hcnTunnelInfo [%s:%s:%s]",
                smf_ue->supi, sess->psi,
                hcnTunnelInfo->ipv4_addr ? hcnTunnelInfo->ipv4_addr : "NULL",
                hcnTunnelInfo->ipv6_addr ? hcnTunnelInfo->ipv6_addr : "NULL",
                hcnTunnelInfo->gtp_teid ? hcnTunnelInfo->gtp_teid : "NULL");
            return false;
        }

        memset(&sess->remote_ul_ip, 0, sizeof(sess->remote_ul_ip));

        if (hcnTunnelInfo->ipv4_addr) {
            rv = ogs_ipv4_from_string(
                    &sess->remote_ul_ip.addr, hcnTunnelInfo->ipv4_addr);
            if (rv != OGS_OK) {
                ogs_error("ogs_ipv4_from_string() [%s] failed",
                        hcnTunnelInfo->ipv4_addr);
                return false;
            }
            sess->remote_ul_ip.ipv4 = 1;
            sess->remote_ul_ip.len = OGS_IPV4_LEN;
        }
        if (hcnTunnelInfo->ipv6_addr) {
            rv = ogs_ipv6addr_from_string(
                    sess->remote_ul_ip.addr6, hcnTunnelInfo->ipv6_addr);
            if (rv != OGS_OK) {
                ogs_error("ogs_ipv6addr_from_string() [%s] failed",
                        hcnTunnelInfo->ipv6_addr);
                return false;
            }
            sess->remote_ul_ip.ipv6 = 1;
            sess->remote_ul_ip.len = OGS_IPV6_LEN;
        }

        if (sess->remote_ul_ip.ipv4 && sess->remote_ul_ip.ipv6)
            sess->remote_ul_ip.len = OGS_IPV4V6_LEN;

        if (hcnTunnelInfo->gtp_teid) {
            sess->remote_ul_teid =
                ogs_uint64_from_string_hexadecimal(hcnTunnelInfo->gtp_teid);
            ogs_debug("hcnTunnelInfo->ipv4 = 0x%x", sess->remote_ul_ip.addr);
            ogs_log_hexdump(OGS_LOG_DEBUG,
                    sess->remote_ul_ip.addr6, OGS_IPV6_LEN);
            ogs_debug("hcnTunnelInfo->gtp_teid = 0x%x", sess->remote_ul_teid);
        }

        dl_pdr = qos_flow->dl_pdr;
        ogs_assert(dl_pdr);

        dl_pdr->outer_header_removal_len = 1;
        dl_pdr->outer_header_removal.description =
            OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IP;

        ul_pdr = qos_flow->ul_pdr;
        ogs_assert(ul_pdr);

        ul_pdr->outer_header_removal_len = 1;
        ul_pdr->outer_header_removal.description =
            OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IP;

        ul_far = qos_flow->ul_far;
        ogs_assert(ul_far);

        ul_far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;
        ogs_assert(OGS_OK ==
            ogs_pfcp_ip_to_outer_header_creation(
                &sess->remote_ul_ip,
                &ul_far->outer_header_creation,
                &ul_far->outer_header_creation_len));
        ul_far->outer_header_creation.teid = sess->remote_ul_teid;

        CLEAR_QOS_FLOWS_SETUP_LIST(sess->h_smf_qos_flows_setup_list);

        qosFlowsSetupList = OpenAPI_list_create();
        ogs_assert(qosFlowsSetupList);
        OpenAPI_list_for_each(
                PduSessionCreatedData->qos_flows_setup_list, node) {
            OpenAPI_qos_flow_setup_item_t *dst = NULL, *src = NULL;

            src = node->data;
            if (!src ||
                !src->qfi ||
                !src->qos_rules ||
                !src->qos_flow_description ||
                !src->qos_flow_profile) {
                ogs_error("[%s:%d] No src [%d:%s:%s]",
                        smf_ue->supi, sess->psi, src->qfi,
                        src->qos_rules ?
                            src->qos_rules : "NULL",
                        src->qos_flow_description ?
                            src->qos_flow_description : "NULL");
                return false;
            }

            qosFlowProfile = src->qos_flow_profile;
            if (!qosFlowProfile ||
                !qosFlowProfile->_5qi ||
                !qosFlowProfile->arp ||
                !qosFlowProfile->arp->priority_level ||
                !(qosFlowProfile->arp->preempt_cap ==
                    OpenAPI_preemption_capability_NOT_PREEMPT ||
                 qosFlowProfile->arp->preempt_cap ==
                    OpenAPI_preemption_capability_MAY_PREEMPT) ||
                !(qosFlowProfile->arp->preempt_vuln ==
                     OpenAPI_preemption_vulnerability_NOT_PREEMPTABLE ||
                 qosFlowProfile->arp->preempt_vuln ==
                     OpenAPI_preemption_vulnerability_PREEMPTABLE)) {
                ogs_error("[%s:%d] Invalid qosFlowProfile [%d:%p:%d]",
                        smf_ue->supi, sess->psi,
                        qosFlowProfile->_5qi,
                        qosFlowProfile->arp,
                        qosFlowProfile->arp ?
                            qosFlowProfile->arp->priority_level : 0);
                return false;
            }

            dst = OpenAPI_qos_flow_setup_item_copy(dst, src);
            ogs_assert(dst);
            OpenAPI_list_add(qosFlowsSetupList, dst);
        }

        if (qosFlowsSetupList->count)
            sess->h_smf_qos_flows_setup_list = qosFlowsSetupList;
        else
            OpenAPI_list_free(qosFlowsSetupList);

        sessionAmbr = PduSessionCreatedData->session_ambr;
        if (sessionAmbr) {
            if (sessionAmbr->uplink)
                sess->session.ambr.uplink =
                    ogs_sbi_bitrate_from_string(sessionAmbr->uplink);
            if (sessionAmbr->downlink)
                sess->session.ambr.downlink =
                    ogs_sbi_bitrate_from_string(sessionAmbr->downlink);
        }

        sess->h_smf_id = PduSessionCreatedData->h_smf_instance_id;

        if (!recvmsg->http.location) {
            ogs_error("[%s:%d] No http.location", smf_ue->supi, sess->psi);
            return false;
        }

        memset(&header, 0, sizeof(header));
        header.uri = recvmsg->http.location;

        rv = ogs_sbi_parse_header(&message, &header);
        if (rv != OGS_OK) {
            ogs_error("[%s:%d] Cannot parse http.location [%s]",
                    smf_ue->supi, sess->psi, recvmsg->http.location);
            return false;
        }

        if (!message.h.resource.component[1]) {
            ogs_error("[%s:%d] No PduSessionRef [%s]",
                    smf_ue->supi, sess->psi, recvmsg->http.location);

            ogs_sbi_header_free(&header);
            return false;
        }

        rc = ogs_sbi_getaddr_from_uri(
                &scheme, &fqdn, &fqdn_port, &addr, &addr6, header.uri);
        if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
            ogs_error("[%s:%d] Invalid URI [%s]",
                    smf_ue->supi, sess->psi, header.uri);

            ogs_sbi_header_free(&header);
            return false;
        }

        client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            ogs_debug("[%s:%d] ogs_sbi_client_add()", smf_ue->supi, sess->psi);
            client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
            if (!client) {
                ogs_error("[%s:%d] ogs_sbi_client_add() failed",
                        smf_ue->supi, sess->psi);

                ogs_sbi_header_free(&header);

                ogs_free(fqdn);
                ogs_freeaddrinfo(addr);
                ogs_freeaddrinfo(addr6);

                return false;
            }
        }
        OGS_SBI_SETUP_CLIENT(&sess->pdu_session, client);

        ogs_free(fqdn);
        ogs_freeaddrinfo(addr);
        ogs_freeaddrinfo(addr6);

        STORE_PDU_SESSION(
                sess, header.uri, message.h.resource.component[1]);

        ogs_sbi_header_free(&header);

        /* Handle GSM Message from n1SmInfoToUe */
        pdu_session_establishment_accept =
            &nas_message.gsm.pdu_session_establishment_accept;

        if (pdu_session_establishment_accept->presencemask &
            OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_PRESENT) {
            ogs_nas_5gsm_cause_t *gsm_cause =
                &pdu_session_establishment_accept->gsm_cause;
            sess->h_smf_gsm_cause = *gsm_cause;
        }

        if (pdu_session_establishment_accept->presencemask &
            OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
            OGS_NAS_STORE_DATA(
                &sess->h_smf_extended_protocol_configuration_options,
                &pdu_session_establishment_accept->
                    extended_protocol_configuration_options);
        }

        ogs_assert(OGS_OK ==
                smf_5gc_pfcp_send_one_qos_flow_modification_request(
                    qos_flow, NULL,
                    OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
                    OGS_PFCP_MODIFY_UL_ONLY|
                    OGS_PFCP_MODIFY_OUTER_HEADER_REMOVAL|
                    OGS_PFCP_MODIFY_ACTIVATE, 0));

        ogs_info("UE SUPI[%s] DNN[%s] S_NSSAI[SST:%d SD:0x%x] "
                "pduSessionRef[%s] pduSessionResourceURI[%s]",
            smf_ue->supi, sess->session.name,
            sess->s_nssai.sst, sess->s_nssai.sd.v,
            sess->pdu_session_ref ? sess->pdu_session_ref : "NULL",
            sess->pdu_session_resource_uri ?
                sess->pdu_session_resource_uri : "NULL");

    } else {
        OpenAPI_pdu_session_create_error_t *PduSessionCreateError = NULL;
        ogs_nas_5gsm_cause_t gsm_cause = 0;

        PduSessionCreateError = recvmsg->PduSessionCreateError;
        if (!PduSessionCreateError) {
            ogs_error("[%s:%d] No PduSessionCreateError",
                    smf_ue->supi, sess->psi);
            return false;
        }

        if (!PduSessionCreateError->error) {
            ogs_error("[%s:%d] No Error", smf_ue->supi, sess->psi);
            return false;
        }

        if (PduSessionCreateError->n1sm_cause)
            gsm_cause = ogs_uint64_from_string_hexadecimal(
                    PduSessionCreateError->n1sm_cause);

        ogs_error("CreatePduSession() failed [%d] cause [%d]",
                recvmsg->res_status, gsm_cause);

        return false;
    }

    return true;
}

bool smf_nsmf_handle_update_data_in_hsmf(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    int rv;
    smf_ue_t *smf_ue = NULL;

    OpenAPI_hsmf_update_data_t *HsmfUpdateData = NULL;

    OpenAPI_plmn_id_nid_t *servingNetwork = NULL;
    OpenAPI_tunnel_info_t *vcnTunnelInfo = NULL;
    OpenAPI_ref_to_binary_data_t *n1SmInfoFromUe = NULL;

    ogs_nas_5gs_message_t nas_message;
    ogs_pkbuf_t *n1SmBufFromUe = NULL;

    ogs_nas_5gs_pdu_session_modification_request_t
        *pdu_session_modification_request = NULL;

    ogs_assert(stream);
    ogs_assert(message);
    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    memset(&sess->nsmf_param, 0, sizeof(sess->nsmf_param));

    HsmfUpdateData = message->HsmfUpdateData;
    if (!HsmfUpdateData) {
        ogs_error("[%s:%d] No HsmfUpdateData",
                smf_ue->supi, sess->psi);
        smf_sbi_send_hsmf_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No HsmfUpdateData", smf_ue->supi, NULL);
        return false;
    }

    if (!HsmfUpdateData->request_indication) {
        ogs_error("[%s:%d] No requestIndication",
                smf_ue->supi, sess->psi);
        smf_sbi_send_hsmf_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No requestIndication", smf_ue->supi, NULL);
        return false;
    }

    sess->nsmf_param.request_indication = HsmfUpdateData->request_indication;

    sess->nsmf_param.up_cnx_state = HsmfUpdateData->up_cnx_state;

    vcnTunnelInfo = HsmfUpdateData->vcn_tunnel_info;
    if (vcnTunnelInfo) {
        if (vcnTunnelInfo->ipv4_addr) {
            rv = ogs_ipv4_from_string(
                    &sess->nsmf_param.dl_ip.addr, vcnTunnelInfo->ipv4_addr);
            if (rv != OGS_OK) {
                ogs_error("ogs_ipv4_from_string() [%s] failed",
                        vcnTunnelInfo->ipv4_addr);
                smf_sbi_send_hsmf_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                        OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                        "ogs_ipv4_from_string() failed",
                        vcnTunnelInfo->ipv4_addr, NULL);
                return false;
            }
            sess->nsmf_param.dl_ip.ipv4 = 1;
            sess->nsmf_param.dl_ip.len = OGS_IPV4_LEN;
        }
        if (vcnTunnelInfo->ipv6_addr) {
            rv = ogs_ipv6addr_from_string(
                    sess->nsmf_param.dl_ip.addr6, vcnTunnelInfo->ipv6_addr);
            if (rv != OGS_OK) {
                ogs_error("ogs_ipv6addr_from_string() [%s] failed",
                        vcnTunnelInfo->ipv6_addr);
                smf_sbi_send_hsmf_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                        OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                        "ogs_ipv6addr_from_string() failed",
                        vcnTunnelInfo->ipv6_addr, NULL);
                return false;
            }
            sess->nsmf_param.dl_ip.ipv6 = 1;
            sess->nsmf_param.dl_ip.len = OGS_IPV6_LEN;
        }
        if (sess->remote_dl_ip.ipv4 && sess->remote_dl_ip.ipv6)
            sess->remote_dl_ip.len = OGS_IPV4V6_LEN;

        if (vcnTunnelInfo->gtp_teid)
            sess->nsmf_param.dl_teid =
                ogs_uint64_from_string_hexadecimal(vcnTunnelInfo->gtp_teid);
    }

    sess->nsmf_param.an_type = HsmfUpdateData->an_type;
    sess->nsmf_param.rat_type = HsmfUpdateData->rat_type;

    servingNetwork = HsmfUpdateData->serving_network;
    if (servingNetwork && servingNetwork->mcc && servingNetwork->mnc)
        sess->nsmf_param.serving_network = true;

    if (HsmfUpdateData->ue_location &&
        HsmfUpdateData->ue_location->nr_location) {
        OpenAPI_nr_location_t *NrLocation =
            HsmfUpdateData->ue_location->nr_location;
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

        sess->nsmf_param.ue_location = true;
        sess->nsmf_param.ue_timezone = true;
    }

    if (HsmfUpdateData->ng_ap_cause) {
        sess->nsmf_param.ngap_cause.group =
            HsmfUpdateData->ng_ap_cause->group;
        sess->nsmf_param.ngap_cause.value =
            HsmfUpdateData->ng_ap_cause->value;
    }
    sess->nsmf_param.gmm_cause = HsmfUpdateData->_5g_mm_cause_value;
    sess->nsmf_param.cause = HsmfUpdateData->cause;

    n1SmInfoFromUe = HsmfUpdateData->n1_sm_info_from_ue;
    if (n1SmInfoFromUe) {
        n1SmBufFromUe = ogs_sbi_find_part_by_content_id(
                message, n1SmInfoFromUe->content_id);

        if (n1SmBufFromUe) {
            rv = gsmue_decode_n1_sm_info(&nas_message, n1SmBufFromUe);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] cannot decode N1 SM Content [%s]",
                        smf_ue->supi, sess->psi, n1SmInfoFromUe->content_id);
                ogs_log_hexdump(OGS_LOG_ERROR,
                        n1SmBufFromUe->data, n1SmBufFromUe->len);
                smf_sbi_send_hsmf_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                        OGS_5GSM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,
                        "cannot decode N1 SM Content", smf_ue->supi, NULL);
                return false;
            }
        }
    }

    switch(nas_message.gsm.h.message_type) {
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST:
        pdu_session_modification_request =
            &nas_message.gsm.pdu_session_modification_request;

        if (pdu_session_modification_request->presencemask &
            (OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT|
             OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT))
            ogs_list_init(&sess->qos_flow_to_modify_list);

        if (pdu_session_modification_request->presencemask &
            OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT) {
            rv = gsm_handle_pdu_session_modification_qos_rules(
                    sess,
                    &pdu_session_modification_request->requested_qos_rules,
                    &sess->nsmf_param.pfcp_flags);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Invalid requested_qos_rules",
                        smf_ue->supi, sess->psi);
                smf_sbi_send_hsmf_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                        OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                        "Invalid requested_qos_rules", smf_ue->supi, NULL);
                return false;
            }
        }

        if (pdu_session_modification_request->presencemask &
            OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT) {
            rv = gsm_handle_pdu_session_modification_qos_flow_descriptions(
                    sess,
                    &pdu_session_modification_request->
                        requested_qos_flow_descriptions,
                    &sess->nsmf_param.pfcp_flags);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Invalid requested_qos_flow_descriptions",
                        smf_ue->supi, sess->psi);
                smf_sbi_send_hsmf_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                        OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                        "Invalid qos_flow_descriptions", smf_ue->supi, NULL);
                return false;
            }
        }
        break;
    default:
        break;
    }

    return true;
}

bool smf_nsmf_handle_update_data_in_vsmf(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    int rv;
    smf_ue_t *smf_ue = NULL;

    OpenAPI_vsmf_update_data_t *VsmfUpdateData = NULL;

    OpenAPI_list_t *qosFlowsAddModRequestList = NULL;
    OpenAPI_list_t *qosFlowsRelRequestList = NULL;
    OpenAPI_qos_flow_profile_t *qosFlowProfile = NULL;
    OpenAPI_lnode_t *node = NULL;

    ogs_nas_5gs_message_t nas_message;
    ogs_pkbuf_t *n1SmBufToUe = NULL;
    OpenAPI_ref_to_binary_data_t *n1SmInfoToUe = NULL;

    smf_n1_n2_message_transfer_param_t param;

    ogs_assert(stream);
    ogs_assert(message);
    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    memset(&sess->nsmf_param, 0, sizeof(sess->nsmf_param));

    VsmfUpdateData = message->VsmfUpdateData;
    if (!VsmfUpdateData) {
        ogs_error("[%s:%d] No VsmfUpdateData",
                smf_ue->supi, sess->psi);
        smf_sbi_send_vsmf_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No VsmfUpdateData", smf_ue->supi, NULL);
        return false;
    }

    if (!VsmfUpdateData->request_indication) {
        ogs_error("[%s:%d] No requestIndication",
                smf_ue->supi, sess->psi);
        smf_sbi_send_vsmf_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "No requestIndication", smf_ue->supi, NULL);
        return false;
    }

    sess->nsmf_param.request_indication = VsmfUpdateData->request_indication;

    n1SmInfoToUe = VsmfUpdateData->n1_sm_info_to_ue;
    if (n1SmInfoToUe) {
        n1SmBufToUe = ogs_sbi_find_part_by_content_id(
                message, n1SmInfoToUe->content_id);

        if (n1SmBufToUe) {
            rv = gsmue_decode_n1_sm_info(&nas_message, n1SmBufToUe);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] cannot decode N1 SM Content [%s]",
                        smf_ue->supi, sess->psi, n1SmInfoToUe->content_id);
                ogs_log_hexdump(OGS_LOG_ERROR,
                        n1SmBufToUe->data, n1SmBufToUe->len);
                smf_sbi_send_vsmf_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                        OGS_5GSM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,
                        "cannot decode N1 SM Content", smf_ue->supi, NULL);
                return false;
            }
        }
    }

    switch (sess->nsmf_param.request_indication) {
    case OpenAPI_request_indication_UE_REQ_PDU_SES_MOD:
    case OpenAPI_request_indication_NW_REQ_PDU_SES_MOD:
        CLEAR_QOS_FLOWS_ADD_MOD_REQUEST_LIST(
                sess->h_smf_qos_flows_add_mod_request_list);

        qosFlowsAddModRequestList = OpenAPI_list_create();
        ogs_assert(qosFlowsAddModRequestList);
        OpenAPI_list_for_each(
                VsmfUpdateData->qos_flows_add_mod_request_list, node) {
            OpenAPI_qos_flow_add_modify_request_item_t *dst = NULL, *src = NULL;

            src = node->data;
            if (!src ||
                !src->qfi ||
                !(src->qos_flow_description || src->qos_flow_profile)) {
                ogs_error("[%s:%d] No src [%d:%s:%p]",
                        smf_ue->supi, sess->psi, src->qfi,
                        src->qos_flow_description ?
                            src->qos_flow_description : "NULL",
                        src->qos_flow_profile ?
                            src->qos_flow_profile : NULL);
                smf_sbi_send_vsmf_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                        OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                        "No src", smf_ue->supi, NULL);
                return false;
            }

            qosFlowProfile = src->qos_flow_profile;
            if (!qosFlowProfile ||
                !qosFlowProfile->_5qi ||
                !qosFlowProfile->arp ||
                !qosFlowProfile->arp->priority_level ||
                !(qosFlowProfile->arp->preempt_cap ==
                    OpenAPI_preemption_capability_NOT_PREEMPT ||
                 qosFlowProfile->arp->preempt_cap ==
                    OpenAPI_preemption_capability_MAY_PREEMPT) ||
                !(qosFlowProfile->arp->preempt_vuln ==
                     OpenAPI_preemption_vulnerability_NOT_PREEMPTABLE ||
                 qosFlowProfile->arp->preempt_vuln ==
                     OpenAPI_preemption_vulnerability_PREEMPTABLE)) {
                ogs_error("[%s:%d] Invalid qosFlowProfile [%d:%p:%d]",
                        smf_ue->supi, sess->psi,
                        qosFlowProfile->_5qi,
                        qosFlowProfile->arp,
                        qosFlowProfile->arp ?
                            qosFlowProfile->arp->priority_level : 0);
                smf_sbi_send_vsmf_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                        OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                        "Invalid qosFlowProfile", smf_ue->supi, NULL);
                return false;
            }

            dst = OpenAPI_qos_flow_add_modify_request_item_copy(dst, src);
            ogs_assert(dst);
            OpenAPI_list_add(qosFlowsAddModRequestList, dst);
        }

        if (qosFlowsAddModRequestList->count)
            sess->h_smf_qos_flows_add_mod_request_list =
                qosFlowsAddModRequestList;
        else
            OpenAPI_list_free(qosFlowsAddModRequestList);

        CLEAR_QOS_FLOWS_REL_REQUEST_LIST(
                sess->h_smf_qos_flows_rel_request_list);

        qosFlowsRelRequestList = OpenAPI_list_create();
        ogs_assert(qosFlowsRelRequestList);
        OpenAPI_list_for_each(VsmfUpdateData->qos_flows_rel_request_list, node) {
            OpenAPI_qos_flow_release_request_item_t *dst = NULL, *src = NULL;

            src = node->data;
            if (!src ||
                !src->qfi ||
                !src->qos_rules ||
                !src->qos_flow_description) {
                ogs_error("[%s:%d] No src [%d:%s:%s]",
                        smf_ue->supi, sess->psi, src->qfi,
                        src->qos_rules ?
                            src->qos_rules : "NULL",
                        src->qos_flow_description ?
                            src->qos_flow_description : "NULL");
                smf_sbi_send_vsmf_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                        OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                        "No src", smf_ue->supi, NULL);
                return false;
            }

            dst = OpenAPI_qos_flow_release_request_item_copy(dst, src);
            ogs_assert(dst);
            OpenAPI_list_add(qosFlowsRelRequestList, dst);
        }

        if (qosFlowsRelRequestList->count)
            sess->h_smf_qos_flows_rel_request_list = qosFlowsRelRequestList;
        else
            OpenAPI_list_free(qosFlowsRelRequestList);

        sess->pti = OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;

    /*
     * Network-requested PDU Session Modification
     *
     * 1.  H: OpenAPI_request_indication_NW_REQ_PDU_SES_MOD
     *        QOS_RULE_CODE_FROM_PFCP_FLAGS
     *        QOS_RULE_FLOW_DESCRIPTION_CODE_FROM_PFCP_FLAGS
     * 2.  H: smf_nsmf_pdusession_build_vsmf_update_data
     * 3.  V: smf_nsmf_handle_update_data_in_vsmf
     * 4.  V*: gsm_build_pdu_session_modification_command+
     *         ngap_build_pdu_session_resource_modify_request_transfer
     * 5.  V: OpenAPI_n2_sm_info_type_PDU_RES_MOD_RSP
     *        if (sess->up_cnx_state == OpenAPI_up_cnx_state_ACTIVATING)
     *            sess->up_cnx_state = OpenAPI_up_cnx_state_ACTIVATED;
     *            smf_sbi_send_sm_context_updated_data_up_cnx_state(
     *                  OpenAPI_up_cnx_state_ACTIVATED)
     *        else
     *            ogs_sbi_send_http_status_no_content(stream)
     * 6.  V: ogs_sbi_send_http_status_no_content(stream)
     *        OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
     *        ogs_sbi_send_http_status_no_content(n1_n2_modified_stream));
     * 7.  V: case OGS_EVENT_SBI_CLIENT
     *        CASE(OGS_SBI_RESOURCE_NAME_VSMF_PDU_SESSIONS)
     * 8.  H: OGS_PFCP_MODIFY_HOME_ROUTED_ROAMING|
     *        OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE
     */
        memset(&param, 0, sizeof(param));

        param.state = SMF_NETWORK_REQUESTED_QOS_FLOW_MODIFICATION;
        param.n1smbuf = gsm_build_pdu_session_modification_command(sess, 0, 0);
        ogs_assert(param.n1smbuf);

        if (sess->h_smf_qos_flows_rel_request_list) {
            param.n2smbuf =
                ngap_build_pdu_session_resource_release_request_transfer(
                        sess, NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release);
            ogs_assert(param.n2smbuf);
        } else if (sess->h_smf_qos_flows_add_mod_request_list) {
            param.n2smbuf =
                ngap_build_pdu_session_resource_modify_request_transfer(sess,
                        qosFlowProfile->gbr_qos_flow_info ? true : false);
            ogs_assert(param.n2smbuf);
        } else
            ogs_error("No LIST");

        if (sess->nsmf_param.request_indication ==
                OpenAPI_request_indication_NW_REQ_PDU_SES_MOD) {
            if (sess->establishment_accept_sent == true) {
                smf_namf_comm_send_n1_n2_message_transfer(sess, stream, &param);
            } else {
                if (sess->pending_modification_xact)
                    ogs_sbi_xact_remove(sess->pending_modification_xact);

                sess->pending_modification_xact =
                    smf_namf_comm_create_n1_n2_message_xact(
                            sess, stream, &param);
            }
        } else if (sess->nsmf_param.request_indication ==
                OpenAPI_request_indication_UE_REQ_PDU_SES_MOD) {
            ogs_sbi_stream_t *amf_to_vsmf_modify_stream = NULL;
            if (sess->amf_to_vsmf_modify_stream_id >= OGS_MIN_POOL_ID &&
                sess->amf_to_vsmf_modify_stream_id <= OGS_MAX_POOL_ID)
                amf_to_vsmf_modify_stream =
                    ogs_sbi_stream_find_by_id(
                            sess->amf_to_vsmf_modify_stream_id);

            if (amf_to_vsmf_modify_stream) {
                smf_sbi_send_sm_context_updated_data_n1_n2_message(
                        sess, amf_to_vsmf_modify_stream, param.n1smbuf,
                        OpenAPI_n2_sm_info_type_PDU_RES_MOD_REQ, param.n2smbuf);

                sess->amf_to_vsmf_modify_stream_id = OGS_INVALID_POOL_ID;

                if (sess->vsmf_to_hsmf_modify_stream_id >= OGS_MIN_POOL_ID &&
                    sess->vsmf_to_hsmf_modify_stream_id <= OGS_MAX_POOL_ID)
                    ogs_error("N1 N2 modified stream ID [%d]"
                            "has not been used yet",
                            sess->vsmf_to_hsmf_modify_stream_id);
                sess->vsmf_to_hsmf_modify_stream_id =
                    ogs_sbi_id_from_stream(stream);
            }
        }
        break;

    case OpenAPI_request_indication_UE_REQ_PDU_SES_REL:
    case OpenAPI_request_indication_NW_REQ_PDU_SES_REL:
        break;
    default:
        ogs_error("[%s:%d] Unknown requestIndication [%d]",
                smf_ue->supi, sess->psi, sess->nsmf_param.request_indication);
        smf_sbi_send_vsmf_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST, OGS_SBI_APP_ERRNO_NULL,
                OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION,
                "Unknown requestIndication", smf_ue->supi, NULL);
        return false;
    }

    return true;
}

bool smf_nsmf_handle_release_data_in_hsmf(
    smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    smf_ue_t *smf_ue = NULL;

    OpenAPI_release_data_t *ReleaseData = NULL;

    ogs_assert(stream);
    ogs_assert(message);
    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    memset(&sess->nsmf_param, 0, sizeof(sess->nsmf_param));

    ReleaseData = message->ReleaseData;
    if (ReleaseData) {
        if (ReleaseData->ue_location &&
            ReleaseData->ue_location->nr_location) {
            OpenAPI_nr_location_t *NrLocation =
                ReleaseData->ue_location->nr_location;
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

            sess->nsmf_param.ue_location = true;
            sess->nsmf_param.ue_timezone = true;
        }

        if (ReleaseData->ng_ap_cause) {
            sess->nsmf_param.ngap_cause.group =
                ReleaseData->ng_ap_cause->group;
            sess->nsmf_param.ngap_cause.value =
                ReleaseData->ng_ap_cause->value;
        }
        sess->nsmf_param.gmm_cause = ReleaseData->_5g_mm_cause_value;
        sess->nsmf_param.cause = ReleaseData->cause;
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
