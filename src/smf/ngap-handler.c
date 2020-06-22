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
        smf_sess_t *sess, ogs_pkbuf_t *pkbuf)
{
    ogs_sbi_session_t *session = NULL;
    smf_ue_t *smf_ue = NULL;
    smf_bearer_t *bearer = NULL;
    int rv, i;

    uint32_t gnb_n3_teid;
    ogs_ip_t gnb_n3_ip;

    ogs_pfcp_far_t *far = NULL;
    bool far_update = false;

    NGAP_PDUSessionResourceSetupResponseTransfer_t message;

    NGAP_QosFlowPerTNLInformation_t *dLQosFlowPerTNLInformation = NULL;

    NGAP_UPTransportLayerInformation_t *uPTransportLayerInformation = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;

    NGAP_AssociatedQosFlowList_t *associatedQosFlowList = NULL;

    ogs_assert(pkbuf);
    ogs_assert(sess);
    session = sess->sbi.session;
    ogs_assert(session);
    bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(bearer);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    rv = ogs_asn_decode(
            &asn_DEF_NGAP_PDUSessionResourceSetupResponseTransfer,
            &message, sizeof(message), pkbuf);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] Cannot decode NGAP message",
                smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N2 SM Info Type", smf_ue->supi, NULL);
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
        smf_sbi_send_sm_context_update_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "Unknown NGAP_UPTransportLayerInformation.present",
                smf_ue->supi, NULL);
        goto cleanup;
    }

    associatedQosFlowList = &dLQosFlowPerTNLInformation->associatedQosFlowList;
    for (i = 0; i < associatedQosFlowList->list.count; i++) {
        NGAP_AssociatedQosFlowItem_t *associatedQosFlowItem = NULL;
        associatedQosFlowItem = (NGAP_AssociatedQosFlowItem_t *)
                associatedQosFlowList->list.array[i];
        if (associatedQosFlowItem) {
            /* TODO : associatedQosFlowItem->qosFlowIdentifier */
        }
    }

    gTPTunnel = uPTransportLayerInformation->choice.gTPTunnel;
    if (!gTPTunnel) {
        ogs_error("[%s:%d] No GTPTunnel", smf_ue->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No GTPTunnel", smf_ue->supi, NULL);
        goto cleanup;
    }

    ogs_asn_BIT_STRING_to_ip(
            &gTPTunnel->transportLayerAddress, &gnb_n3_ip);
    ogs_asn_OCTET_STRING_to_uint32(&gTPTunnel->gTP_TEID, &gnb_n3_teid);

    if (memcmp(&bearer->gnb_n3_ip, &gnb_n3_ip,
                sizeof(bearer->gnb_n3_ip)) != 0 ||
        bearer->gnb_n3_teid != gnb_n3_teid)
        sess->pfcp_5gc_modify.outer_header_creation_update = true;
    else
        sess->pfcp_5gc_modify.outer_header_creation_update = false;

    memcpy(&bearer->gnb_n3_ip, &gnb_n3_ip, sizeof(bearer->gnb_n3_ip));
    bearer->gnb_n3_teid = gnb_n3_teid;

    ogs_list_for_each(&bearer->pfcp.far_list, far) {
        if (far->dst_if == OGS_PFCP_INTERFACE_ACCESS) {
            if (sess->ueUpCnxState == OpenAPI_up_cnx_state_ACTIVATED) {
                if (far->apply_action != OGS_PFCP_APPLY_ACTION_FORW) {
                    far_update = true;
                }
            } else if (sess->ueUpCnxState == OpenAPI_up_cnx_state_DEACTIVATED) {
                far_update = true;
            }
        }
    }

    if (far_update || sess->pfcp_5gc_modify.outer_header_creation_update)
        smf_5gc_pfcp_send_session_modification_request(sess);
    else
        smf_sbi_send_sm_context_updated_data(sess);


    rv = OGS_OK;
cleanup:
    ogs_asn_free(
            &asn_DEF_NGAP_PDUSessionResourceSetupResponseTransfer, &message);
    return rv;
}
