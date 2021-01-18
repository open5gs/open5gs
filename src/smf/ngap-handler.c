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

int ngap_handle_pdu_session_resource_setup_response_transfer(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_pkbuf_t *pkbuf)
{
    smf_ue_t *smf_ue = NULL;
    smf_bearer_t *qos_flow = NULL;
    int rv, i;

    uint32_t gnb_n3_teid;
    ogs_ip_t gnb_n3_ip;

    ogs_pfcp_far_t *dl_far = NULL;
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

    ogs_asn_BIT_STRING_to_ip(
            &gTPTunnel->transportLayerAddress, &gnb_n3_ip);
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
            qos_flow = smf_qos_flow_find_by_qfi(sess,
                    associatedQosFlowItem->qosFlowIdentifier);

            if (qos_flow) {
                dl_far = qos_flow->dl_far;
                ogs_assert(dl_far);
                if (dl_far->apply_action != OGS_PFCP_APPLY_ACTION_FORW) {
                    far_update = true;
                }

                dl_far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;
                ogs_pfcp_ip_to_outer_header_creation(
                        &sess->gnb_n3_ip,
                        &dl_far->outer_header_creation,
                        &dl_far->outer_header_creation_len);
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
        smf_5gc_pfcp_send_session_modification_request(
                sess, stream, OGS_PFCP_MODIFY_ACTIVATE);
    } else {
        /* ACTIVATED Is NOT Included in RESPONSE */
        smf_sbi_send_http_status_no_content(stream);
    }

    rv = OGS_OK;
cleanup:
    ogs_asn_free(
            &asn_DEF_NGAP_PDUSessionResourceSetupResponseTransfer, &message);
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
            }
        }
    }

    if (!qos_flow) {
        ogs_error("[%s:%d] No QoS flow", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No QoS flow", smf_ue->supi, NULL, NULL);
        goto cleanup;
    }

    dl_far = qos_flow->dl_far;
    ogs_assert(dl_far);

    dl_far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;
    ogs_pfcp_ip_to_outer_header_creation(
            &sess->gnb_n3_ip,
            &dl_far->outer_header_creation,
            &dl_far->outer_header_creation_len);
    dl_far->outer_header_creation.teid = sess->gnb_n3_teid;

    smf_5gc_pfcp_send_qos_flow_modification_request(
            qos_flow, stream, OGS_PFCP_MODIFY_ACTIVATE);

    rv = OGS_OK;

cleanup:
    ogs_asn_free(
            &asn_DEF_NGAP_PDUSessionResourceModifyResponseTransfer, &message);
    return rv;
}

int ngap_handle_pdu_session_resource_to_be_switched_dl_transfer(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_pkbuf_t *pkbuf)
{
    smf_ue_t *smf_ue = NULL;
    smf_bearer_t *qos_flow = NULL;
    int rv, i;

    uint32_t gnb_n3_teid;
    ogs_ip_t gnb_n3_ip;

    ogs_pfcp_far_t *dl_far = NULL;
    bool far_update = false;

    NGAP_PathSwitchRequestTransfer_t message;

    NGAP_UPTransportLayerInformation_t *dL_NGU_UP_TNLInformation = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;

    NGAP_QosFlowAcceptedList_t *qosFlowAcceptedList = NULL;

    ogs_assert(pkbuf);
    ogs_assert(stream);

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

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
            "[%s:%d] Unknown NGAP_UPTransportLayerInformation.present [%d]",
            smf_ue->supi, sess->psi, dL_NGU_UP_TNLInformation->present);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "Unknown NGAP_UPTransportLayerInformation.present",
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

    ogs_asn_BIT_STRING_to_ip(&gTPTunnel->transportLayerAddress, &gnb_n3_ip);
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
        NGAP_QosFlowAcceptedItem_t *acceptedQosFlowItem = NULL;
        acceptedQosFlowItem = (NGAP_QosFlowAcceptedItem_t *)
                qosFlowAcceptedList->list.array[i];
        if (acceptedQosFlowItem) {
            qos_flow = smf_qos_flow_find_by_qfi(sess,
                    acceptedQosFlowItem->qosFlowIdentifier);

            if (qos_flow) {
                dl_far = qos_flow->dl_far;
                ogs_assert(dl_far);
                if (dl_far->apply_action != OGS_PFCP_APPLY_ACTION_FORW) {
                    far_update = true;
                }

                dl_far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;
                ogs_pfcp_ip_to_outer_header_creation(
                        &sess->gnb_n3_ip,
                        &dl_far->outer_header_creation,
                        &dl_far->outer_header_creation_len);
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
        smf_5gc_pfcp_send_session_modification_request(
                sess, stream,
                OGS_PFCP_MODIFY_ACTIVATE | OGS_PFCP_MODIFY_PATH_SWITCH |
                OGS_PFCP_MODIFY_END_MARKER);
    } else {
        /* ACTIVATED Is NOT Included in RESPONSE */
        smf_sbi_send_http_status_no_content(stream);
    }

    rv = OGS_OK;

cleanup:
    ogs_asn_free(&asn_DEF_NGAP_PathSwitchRequestTransfer, &message);
    return rv;
}
