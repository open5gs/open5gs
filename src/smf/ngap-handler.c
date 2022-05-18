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

#include "ngap-handler.h"
#include "sbi-path.h"
#include "pfcp-path.h"
#include "ngap-path.h"

int ngap_handle_pdu_session_resource_setup_response_transfer(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_pkbuf_t *pkbuf)
{
    smf_ue_t *smf_ue = NULL;
    int rv, i;

    uint32_t gnb_n3_teid;
    ogs_ip_t gnb_n3_ip;

    bool far_update = false;

    NGAP_PDUSessionResourceSetupResponseTransfer_t message;

    NGAP_QosFlowPerTNLInformation_t *dLQosFlowPerTNLInformation = NULL;

    NGAP_UPTransportLayerInformation_t *uPTransportLayerInformation = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;

    NGAP_AssociatedQosFlowList_t *associatedQosFlowList = NULL;

    ogs_assert(pkbuf);
    ogs_assert(stream);

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_debug("PDUSessionResourceSetupResponseTransfer");

    rv = ogs_asn_decode(
            &asn_DEF_NGAP_PDUSessionResourceSetupResponseTransfer,
            &message, sizeof(message), pkbuf);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] Cannot decode NGAP message",
                smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N2 SM Info Type", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    rv = OGS_ERROR;

    dLQosFlowPerTNLInformation = &message.dLQosFlowPerTNLInformation;
    uPTransportLayerInformation =
        &dLQosFlowPerTNLInformation->uPTransportLayerInformation;

    if (uPTransportLayerInformation->present !=
        NGAP_UPTransportLayerInformation_PR_gTPTunnel) {
        ogs_error(
            "[%s:%d] Unknown NGAP_UPTransportLayerInformation.present [%d]",
            smf_ue->supi, sess->psi,
            uPTransportLayerInformation->present);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "Unknown NGAP_UPTransportLayerInformation.present",
                smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    gTPTunnel = uPTransportLayerInformation->choice.gTPTunnel;
    if (!gTPTunnel) {
        ogs_error("[%s:%d] No GTPTunnel", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No GTPTunnel", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    rv = ogs_asn_BIT_STRING_to_ip(
            &gTPTunnel->transportLayerAddress, &gnb_n3_ip);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] No transportLayerAddress", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No transportLayerAddress", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }
    ogs_asn_OCTET_STRING_to_uint32(&gTPTunnel->gTP_TEID, &gnb_n3_teid);

    /* Need to Update? */
    if (memcmp(&sess->gnb_n3_ip, &gnb_n3_ip, sizeof(sess->gnb_n3_ip)) != 0 ||
        sess->gnb_n3_teid != gnb_n3_teid)
        far_update = true;

    /* Setup FAR */
    memcpy(&sess->gnb_n3_ip, &gnb_n3_ip, sizeof(sess->gnb_n3_ip));
    sess->gnb_n3_teid = gnb_n3_teid;

    associatedQosFlowList = &dLQosFlowPerTNLInformation->associatedQosFlowList;
    for (i = 0; i < associatedQosFlowList->list.count; i++) {
        NGAP_AssociatedQosFlowItem_t *associatedQosFlowItem = NULL;
        associatedQosFlowItem = (NGAP_AssociatedQosFlowItem_t *)
                associatedQosFlowList->list.array[i];

        if (associatedQosFlowItem) {
            smf_bearer_t *qos_flow = smf_qos_flow_find_by_qfi(
                    sess, associatedQosFlowItem->qosFlowIdentifier);

            if (qos_flow) {
                ogs_pfcp_far_t *dl_far = qos_flow->dl_far;
                ogs_assert(dl_far);
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
            } else {
                ogs_error("[%s:%d] No QoS flow", smf_ue->supi, sess->psi);
                smf_sbi_send_sm_context_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        "No QoS flow", smf_ue->supi, NULL, NULL);
                goto cleanup;
            }
        }
    }

    if (far_update) {
        ogs_assert(OGS_OK ==
            smf_5gc_pfcp_send_all_pdr_modification_request(
                sess, stream, OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE,
                0));
    } else {
        /* ACTIVATED Is NOT Included in RESPONSE */
        ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));
    }

    rv = OGS_OK;
cleanup:
    ogs_asn_free(
            &asn_DEF_NGAP_PDUSessionResourceSetupResponseTransfer, &message);
    return rv;
}

int ngap_handle_pdu_session_resource_setup_unsuccessful_transfer(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_pkbuf_t *pkbuf)
{
    smf_ue_t *smf_ue = NULL;
    int rv;

    NGAP_PDUSessionResourceSetupUnsuccessfulTransfer_t message;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(pkbuf);
    ogs_assert(stream);

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_debug("PDUSessionResourceSetupUnsuccessfulTransfer");

    rv = ogs_asn_decode(
            &asn_DEF_NGAP_PDUSessionResourceSetupUnsuccessfulTransfer,
            &message, sizeof(message), pkbuf);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] Cannot decode NGAP message",
                smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N2 SM Info Type", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    rv = OGS_ERROR;

    Cause = &message.cause;

    if (Cause->present == NGAP_Cause_PR_radioNetwork &&
        Cause->choice.radioNetwork ==
            NGAP_CauseRadioNetwork_multiple_PDU_session_ID_instances) {
        ogs_warn("    Cause[Group:%d Cause:%d]",
                Cause->present, (int)Cause->choice.radioNetwork);
    } else {
        ogs_error("    Cause[Group:%d Cause:%d]",
                Cause->present, (int)Cause->choice.radioNetwork);
    }

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
     */

    ogs_assert(OGS_OK ==
        smf_5gc_pfcp_send_all_pdr_modification_request(
            sess, stream,
            OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_DEACTIVATE, 0));

    rv = OGS_OK;
cleanup:
    ogs_asn_free(
        &asn_DEF_NGAP_PDUSessionResourceSetupUnsuccessfulTransfer, &message);
    return rv;
}

int ngap_handle_pdu_session_resource_modify_response_transfer(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_pkbuf_t *pkbuf)
{
    smf_ue_t *smf_ue = NULL;
    smf_bearer_t *qos_flow = NULL;
    int rv, i;

    ogs_pfcp_far_t *dl_far = NULL;

    NGAP_PDUSessionResourceModifyResponseTransfer_t message;

    NGAP_QosFlowAddOrModifyResponseList_t *qosFlowAddOrModifyResponseList;

    ogs_assert(pkbuf);
    ogs_assert(stream);

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_debug("PDUSessionResourceModifyResponseTransfer");

    rv = ogs_asn_decode(
            &asn_DEF_NGAP_PDUSessionResourceModifyResponseTransfer,
            &message, sizeof(message), pkbuf);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] Cannot decode NGAP message",
                smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N2 SM Info Type", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    rv = OGS_ERROR;

    qosFlowAddOrModifyResponseList = message.qosFlowAddOrModifyResponseList;
    if (!qosFlowAddOrModifyResponseList) {
        /* QosFlow Release */
        rv = OGS_OK;

        ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));

        goto cleanup;
    }

    ogs_list_init(&sess->qos_flow_to_modify_list);

    if (qosFlowAddOrModifyResponseList) {
        for (i = 0; i < qosFlowAddOrModifyResponseList->list.count; i++) {
            NGAP_QosFlowAddOrModifyResponseItem_t
                *qosFlowAddOrModifyResponseItem = NULL;
            qosFlowAddOrModifyResponseItem =
                (NGAP_QosFlowAddOrModifyResponseItem_t *)
                    qosFlowAddOrModifyResponseList->list.array[i];

            if (qosFlowAddOrModifyResponseItem) {
                qos_flow = smf_qos_flow_find_by_qfi(sess,
                        qosFlowAddOrModifyResponseItem->qosFlowIdentifier);
                if (qos_flow) {
                    dl_far = qos_flow->dl_far;
                    ogs_assert(dl_far);

                    dl_far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;
                    ogs_assert(OGS_OK ==
                        ogs_pfcp_ip_to_outer_header_creation(
                            &sess->gnb_n3_ip,
                            &dl_far->outer_header_creation,
                            &dl_far->outer_header_creation_len));
                    dl_far->outer_header_creation.teid = sess->gnb_n3_teid;

                    ogs_list_add(&sess->qos_flow_to_modify_list,
                                    &qos_flow->to_modify_node);
                }
            }
        }
    }

    if (ogs_list_count(&sess->qos_flow_to_modify_list) == 0) {
        ogs_error("[%s:%d] No QoS flow", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No QoS flow", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    ogs_assert(OGS_OK ==
            smf_5gc_pfcp_send_qos_flow_list_modification_request(
                sess, stream,
                OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE, 0));

    rv = OGS_OK;

cleanup:
    ogs_asn_free(
            &asn_DEF_NGAP_PDUSessionResourceModifyResponseTransfer, &message);
    return rv;
}

int ngap_handle_path_switch_request_transfer(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_pkbuf_t *pkbuf)
{
    smf_ue_t *smf_ue = NULL;
    int rv, i;

    uint32_t gnb_n3_teid;
    ogs_ip_t gnb_n3_ip;

    bool far_update = false;

    NGAP_PathSwitchRequestTransfer_t message;

    NGAP_UPTransportLayerInformation_t *dL_NGU_UP_TNLInformation = NULL;
    NGAP_QosFlowAcceptedItem_t *acceptedQosFlowItem = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;

    NGAP_QosFlowAcceptedList_t *qosFlowAcceptedList = NULL;

    ogs_assert(pkbuf);
    ogs_assert(stream);

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_debug("PathSwitchRequestTransfer");

    rv = ogs_asn_decode(&asn_DEF_NGAP_PathSwitchRequestTransfer,
            &message, sizeof(message), pkbuf);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] Cannot decode NGAP message",
                smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N2 SM Info Type", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    rv = OGS_ERROR;

    dL_NGU_UP_TNLInformation = &message.dL_NGU_UP_TNLInformation;
    if (dL_NGU_UP_TNLInformation->present !=
        NGAP_UPTransportLayerInformation_PR_gTPTunnel) {
        ogs_error(
            "[%s:%d] Unknown dL_NGU_UP_TNLInformation->present [%d]",
            smf_ue->supi, sess->psi, dL_NGU_UP_TNLInformation->present);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "Unknown dL_NGU_UP_TNLInformation->present",
                smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    gTPTunnel = dL_NGU_UP_TNLInformation->choice.gTPTunnel;
    if (!gTPTunnel) {
        ogs_error("[%s:%d] No GTPTunnel", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No GTPTunnel", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    rv = ogs_asn_BIT_STRING_to_ip(
            &gTPTunnel->transportLayerAddress, &gnb_n3_ip);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] No transportLayerAddress", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No transportLayerAddress", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }
    ogs_asn_OCTET_STRING_to_uint32(&gTPTunnel->gTP_TEID, &gnb_n3_teid);

    /* Need to Update? */
    if (memcmp(&sess->gnb_n3_ip, &gnb_n3_ip, sizeof(sess->gnb_n3_ip)) != 0 ||
        sess->gnb_n3_teid != gnb_n3_teid)
        far_update = true;

    /* Setup FAR */
    memcpy(&sess->gnb_n3_ip, &gnb_n3_ip, sizeof(sess->gnb_n3_ip));
    sess->gnb_n3_teid = gnb_n3_teid;

    qosFlowAcceptedList = &message.qosFlowAcceptedList;
    for (i = 0; i < qosFlowAcceptedList->list.count; i++) {
        acceptedQosFlowItem = (NGAP_QosFlowAcceptedItem_t *)
                qosFlowAcceptedList->list.array[i];
        if (acceptedQosFlowItem) {
            smf_bearer_t *qos_flow = smf_qos_flow_find_by_qfi(
                    sess, acceptedQosFlowItem->qosFlowIdentifier);

            if (qos_flow) {
                ogs_pfcp_far_t *dl_far = qos_flow->dl_far;
                ogs_assert(dl_far);
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
            } else {
                ogs_error("[%s:%d] No QoS flow", smf_ue->supi, sess->psi);
                smf_sbi_send_sm_context_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        "No QoS flow", smf_ue->supi, NULL, NULL);
                goto cleanup;
            }
        }
    }

    if (far_update) {
        ogs_assert(OGS_OK ==
            smf_5gc_pfcp_send_all_pdr_modification_request(
                sess, stream,
                OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE|
                OGS_PFCP_MODIFY_XN_HANDOVER|OGS_PFCP_MODIFY_END_MARKER,
                0));
    } else {
        /* ACTIVATED Is NOT Included in RESPONSE */
        ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));
    }

    rv = OGS_OK;

cleanup:
    ogs_asn_free(&asn_DEF_NGAP_PathSwitchRequestTransfer, &message);
    return rv;
}

int ngap_handle_handover_required_transfer(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_pkbuf_t *pkbuf)
{
    smf_ue_t *smf_ue = NULL;
    int rv;

    NGAP_HandoverRequiredTransfer_t message;

    NGAP_DirectForwardingPathAvailability_t
        *directForwardingPathAvailability = NULL;

    ogs_pkbuf_t *n2smbuf = NULL;

    ogs_assert(pkbuf);
    ogs_assert(stream);

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_debug("HandoverRequiredTransfer");

    rv = ogs_asn_decode(&asn_DEF_NGAP_HandoverRequiredTransfer,
            &message, sizeof(message), pkbuf);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] Cannot decode NGAP message",
                smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N2 SM Info Type", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    directForwardingPathAvailability = message.directForwardingPathAvailability;
    if (!directForwardingPathAvailability)
        sess->handover.data_forwarding_not_possible = true;

    n2smbuf = ngap_build_pdu_session_resource_setup_request_transfer(sess);
    ogs_assert(n2smbuf);

    smf_sbi_send_sm_context_updated_data(
        sess, stream, 0, OpenAPI_ho_state_PREPARING,
        NULL, OpenAPI_n2_sm_info_type_PDU_RES_SETUP_REQ, n2smbuf);

    rv = OGS_OK;

cleanup:
    ogs_asn_free(&asn_DEF_NGAP_HandoverRequiredTransfer, &message);
    return rv;
}

int ngap_handle_handover_request_ack(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_pkbuf_t *pkbuf)
{
    smf_ue_t *smf_ue = NULL;
    int rv, i;

    NGAP_HandoverRequestAcknowledgeTransfer_t message;

    NGAP_UPTransportLayerInformation_t *dL_NGU_UP_TNLInformation = NULL;
    NGAP_UPTransportLayerInformation_t *dLForwardingUP_TNLInformation = NULL;
    NGAP_QosFlowListWithDataForwarding_t *qosFlowSetupResponseList = NULL;
    NGAP_QosFlowItemWithDataForwarding_t *qosFlowSetupResponseItem = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;

    ogs_assert(pkbuf);
    ogs_assert(stream);

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_debug("HandoverRequestAcknowledgeTransfer");

    rv = ogs_asn_decode(&asn_DEF_NGAP_HandoverRequestAcknowledgeTransfer,
            &message, sizeof(message), pkbuf);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] Cannot decode NGAP message",
                smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N2 SM Info Type", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    rv = OGS_ERROR;

    dL_NGU_UP_TNLInformation = &message.dL_NGU_UP_TNLInformation;
    if (dL_NGU_UP_TNLInformation->present !=
        NGAP_UPTransportLayerInformation_PR_gTPTunnel) {
        ogs_error(
            "[%s:%d] Unknown dL_NGU_UP_TNLInformation->present [%d]",
            smf_ue->supi, sess->psi, dL_NGU_UP_TNLInformation->present);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "Unknown dL_NGU_UP_TNLInformation->present",
                smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    gTPTunnel = dL_NGU_UP_TNLInformation->choice.gTPTunnel;
    if (!gTPTunnel) {
        ogs_error("[%s:%d] No GTPTunnel", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No GTPTunnel", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    rv = ogs_asn_BIT_STRING_to_ip(&gTPTunnel->transportLayerAddress,
            &sess->handover.gnb_n3_ip);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] No transportLayerAddress", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No transportLayerAddress", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }
    ogs_asn_OCTET_STRING_to_uint32(&gTPTunnel->gTP_TEID,
            &sess->handover.gnb_n3_teid);

    qosFlowSetupResponseList = &message.qosFlowSetupResponseList;
    for (i = 0; i < qosFlowSetupResponseList->list.count; i++) {
        qosFlowSetupResponseItem = (NGAP_QosFlowItemWithDataForwarding_t *)
                qosFlowSetupResponseList->list.array[i];
        if (qosFlowSetupResponseItem) {
            smf_bearer_t *qos_flow = smf_qos_flow_find_by_qfi(
                    sess, qosFlowSetupResponseItem->qosFlowIdentifier);

            if (qos_flow) {
                ogs_pfcp_far_t *dl_far = qos_flow->dl_far;
                ogs_assert(dl_far);

                dl_far->handover.prepared = true;

            } else {
                ogs_error("[%s:%d] No QoS flow", smf_ue->supi, sess->psi);
                smf_sbi_send_sm_context_update_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        "No QoS flow", smf_ue->supi, NULL, NULL);
                goto cleanup;
            }
        }
    }

    dLForwardingUP_TNLInformation = message.dLForwardingUP_TNLInformation;
    if (dLForwardingUP_TNLInformation) {
        if (dLForwardingUP_TNLInformation->present !=
                NGAP_UPTransportLayerInformation_PR_gTPTunnel) {
            ogs_error(
                "[%s:%d] Unknown dLForwardingUP_TNLInformation->present [%d]",
                smf_ue->supi, sess->psi, dL_NGU_UP_TNLInformation->present);
            smf_sbi_send_sm_context_update_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "Unknown dLForwardingUP_TNLInformation->present",
                    smf_ue->supi, NULL, NULL);
            goto cleanup;
        }

        gTPTunnel = dLForwardingUP_TNLInformation->choice.gTPTunnel;
        if (!gTPTunnel) {
            ogs_error("[%s:%d] No GTPTunnel", smf_ue->supi, sess->psi);
            smf_sbi_send_sm_context_update_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No GTPTunnel", smf_ue->supi, NULL, NULL);
            goto cleanup;
        }

        rv = ogs_asn_BIT_STRING_to_ip(&gTPTunnel->transportLayerAddress,
                &sess->handover.gnb_dl_ip);
        if (rv != OGS_OK) {
            ogs_error("[%s:%d] No transportLayerAddress",
                    smf_ue->supi, sess->psi);
            smf_sbi_send_sm_context_update_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    "No transportLayerAddress", smf_ue->supi, NULL, NULL);
            goto cleanup;
        }
        ogs_asn_OCTET_STRING_to_uint32(&gTPTunnel->gTP_TEID,
                &sess->handover.gnb_dl_teid);

        sess->handover.indirect_data_forwarding = true;
    }

    sess->handover.prepared = true;

    if (sess->handover.indirect_data_forwarding == true) {
        if (smf_sess_have_indirect_data_forwarding(sess) == true) {
            ogs_error("We found redundant INDIRECT Tunnel");
            ogs_error("It will be automatically removed");

            ogs_assert(OGS_OK ==
                smf_5gc_pfcp_send_all_pdr_modification_request(
                    sess, stream,
                    OGS_PFCP_MODIFY_INDIRECT|
                    /*
                     * Firstly, OGS_PFCP_MODIFY_REMOVE is only appled.
                     * And then, after receiving PFCP response message,
                     * we can apply OGS_PFCP_MODIFY_CREATE.
                     *
                     * PFCP build is implemented as below.
                     *
                     * if OGS_PFCP_MODIFY_REMOVE
                     * else if OGS_PFCP_MODIFY_CREATE
                     * else if ..
                     * ...
                     */
                    OGS_PFCP_MODIFY_REMOVE|OGS_PFCP_MODIFY_CREATE,
                    0));
        } else {

            smf_sess_create_indirect_data_forwarding(sess);

            ogs_assert(OGS_OK ==
                smf_5gc_pfcp_send_all_pdr_modification_request(
                    sess, stream,
                    OGS_PFCP_MODIFY_INDIRECT|OGS_PFCP_MODIFY_CREATE,
                    0));
        }
    } else {
        ogs_pkbuf_t *n2smbuf = ngap_build_handover_command_transfer(sess);
        ogs_assert(n2smbuf);

        smf_sbi_send_sm_context_updated_data(
            sess, stream, 0, OpenAPI_ho_state_PREPARED,
            NULL, OpenAPI_n2_sm_info_type_HANDOVER_CMD, n2smbuf);
    }

    rv = OGS_OK;

cleanup:
    ogs_asn_free(&asn_DEF_NGAP_HandoverRequestAcknowledgeTransfer, &message);
    return rv;
}
