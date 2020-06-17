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
    smf_bearer_t *bearer = NULL;
    int rv, i;

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

    rv = ogs_asn_decode(
            &asn_DEF_NGAP_PDUSessionResourceSetupResponseTransfer,
            &message, sizeof(message), pkbuf);
    if (rv != OGS_OK) {
        ogs_error("[%s:%d] Cannot decode NGAP message", sess->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N2 SM Info Type", sess->supi_psi_keybuf, NULL);
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
            sess->supi, sess->psi, uPTransportLayerInformation->present);
        smf_sbi_send_sm_context_update_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "Unknown NGAP_UPTransportLayerInformation.present",
                sess->supi_psi_keybuf, NULL);
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
        ogs_error("[%s:%d] No GTPTunnel", sess->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No GTPTunnel", sess->supi_psi_keybuf, NULL);
        goto cleanup;
    }

    ogs_asn_BIT_STRING_to_ip(
            &gTPTunnel->transportLayerAddress, &bearer->gnb_ip);
    ogs_asn_OCTET_STRING_to_uint32(&gTPTunnel->gTP_TEID, &bearer->gnb_n3_teid);

    bearer->state.outer_header_creation_updated = true;
    smf_pfcp_send_session_modification_request(bearer);

    rv = OGS_OK;
cleanup:
    ogs_asn_free(
            &asn_DEF_NGAP_PDUSessionResourceSetupResponseTransfer, &message);
    return rv;
}
