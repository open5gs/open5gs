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

#include "test-common.h"

void testngap_handle_ng_setup_response(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, j;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_NGSetupResponse_t *NGSetupResponse = NULL;
    NGAP_PLMNSupportList_t *PLMNSupportList = NULL;

    NGAP_NGSetupResponseIEs_t *ie = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    NGSetupResponse = &successfulOutcome->value.choice.NGSetupResponse;
    ogs_assert(NGSetupResponse);

    ogs_debug("NG setup response");

    for (i = 0; i < NGSetupResponse->protocolIEs.list.count; i++) {
        ie = NGSetupResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_PLMNSupportList:
            PLMNSupportList = &ie->value.choice.PLMNSupportList;
            break;
        default:
            break;
        }
    }

    for (i = 0; i < PLMNSupportList->list.count; i++) {
        NGAP_PLMNSupportItem_t *NGAP_PLMNSupportItem = NULL;
        NGAP_PLMNIdentity_t *pLMNIdentity = NULL;
        NGAP_SliceSupportList_t *sliceSupportList = NULL;

        sliceSupportList = (NGAP_SliceSupportList_t *)
            PLMNSupportList->list.array[i];
        for (j = 0; j < sliceSupportList->list.count; j++) {
        }
    }
}

void testngap_handle_downlink_nas_transport(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_DownlinkNASTransport_t *DownlinkNASTransport = NULL;

    NGAP_DownlinkNASTransport_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    DownlinkNASTransport =
        &initiatingMessage->value.choice.DownlinkNASTransport;
    ogs_assert(DownlinkNASTransport);

    for (i = 0; i < DownlinkNASTransport->protocolIEs.list.count; i++) {
        ie = DownlinkNASTransport->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = &ie->value.choice.NAS_PDU;
            break;
        default:
            break;
        }
    }

    if (AMF_UE_NGAP_ID) {
        uint64_t amf_ue_ngap_id;
        asn_INTEGER2ulong(AMF_UE_NGAP_ID, (unsigned long *)&amf_ue_ngap_id);
        test_ue->amf_ue_ngap_id = (uint64_t)amf_ue_ngap_id;
    }
    if (RAN_UE_NGAP_ID) {
        test_ue->ran_ue_ngap_id = *RAN_UE_NGAP_ID;
    }

    if (NAS_PDU)
        testngap_send_to_nas(test_ue, NAS_PDU);
}

void testngap_handle_initial_context_setup_request(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    test_sess_t *sess = NULL;
    test_bearer_t *qos_flow = NULL;
    int rv, i, j, k, l;
    char buf[OGS_ADDRSTRLEN];

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_InitialContextSetupRequest_t *InitialContextSetupRequest = NULL;

    NGAP_InitialContextSetupRequestIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;

    NGAP_PDUSessionResourceSetupListCxtReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemCxtReq_t *PDUSessionItem = NULL;

    NGAP_PDUSessionResourceSetupRequestTransfer_t n2sm_message;
    NGAP_PDUSessionResourceSetupRequestTransferIEs_t *ie2 = NULL;
    NGAP_UPTransportLayerInformation_t *UPTransportLayerInformation = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;
    NGAP_QosFlowSetupRequestList_t *QosFlowSetupRequestList = NULL;
    NGAP_QosFlowSetupRequestItem_t *QosFlowSetupRequestItem = NULL;
    OCTET_STRING_t *transfer = NULL;
    ogs_pkbuf_t *n2smbuf = NULL;

    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    InitialContextSetupRequest =
        &initiatingMessage->value.choice.InitialContextSetupRequest;
    ogs_assert(InitialContextSetupRequest);

    for (i = 0; i < InitialContextSetupRequest->protocolIEs.list.count; i++) {
        ie = InitialContextSetupRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq:
            PDUSessionList =
                &ie->value.choice.PDUSessionResourceSetupListCxtReq;
            break;
        case NGAP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = &ie->value.choice.NAS_PDU;
            break;
        default:
            break;
        }
    }

    if (AMF_UE_NGAP_ID) {
        uint64_t amf_ue_ngap_id;
        asn_INTEGER2ulong(AMF_UE_NGAP_ID, (unsigned long *)&amf_ue_ngap_id);
        test_ue->amf_ue_ngap_id = (uint64_t)amf_ue_ngap_id;
    }
    if (RAN_UE_NGAP_ID) {
        test_ue->ran_ue_ngap_id = *RAN_UE_NGAP_ID;
    }

    if (PDUSessionList) {
        for (j = 0; j < PDUSessionList->list.count; j++) {
            PDUSessionItem = (NGAP_PDUSessionResourceSetupItemCxtReq_t *)
                PDUSessionList->list.array[j];
            ogs_assert(PDUSessionItem);

            sess = test_sess_find_by_psi(
                        test_ue, PDUSessionItem->pDUSessionID);
            ogs_assert(sess);

            if (PDUSessionItem->nAS_PDU)
                testngap_send_to_nas(test_ue, PDUSessionItem->nAS_PDU);
            transfer = &PDUSessionItem->
                pDUSessionResourceSetupRequestTransfer;
            ogs_assert(transfer);

            n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
            ogs_assert(n2smbuf);
            ogs_pkbuf_put_data(n2smbuf, transfer->buf, transfer->size);

            rv = ogs_asn_decode(
                    &asn_DEF_NGAP_PDUSessionResourceSetupRequestTransfer,
                    &n2sm_message, sizeof(n2sm_message), n2smbuf);
            ogs_assert(rv == OGS_OK);

            for (k = 0; k < n2sm_message.protocolIEs.list.count; k++) {
                ie2 = n2sm_message.protocolIEs.list.array[k];
                switch (ie2->id) {
                case NGAP_ProtocolIE_ID_id_QosFlowSetupRequestList:
                    QosFlowSetupRequestList =
                        &ie2->value.choice.QosFlowSetupRequestList;
                    ogs_assert(QosFlowSetupRequestList);
                    for (l = 0;
                            l < QosFlowSetupRequestList->list.count; l++) {
                        QosFlowSetupRequestItem =
                            (struct NGAP_QosFlowSetupRequestItem *)
                            QosFlowSetupRequestList->list.array[l];
                        ogs_assert(QosFlowSetupRequestItem);
                        qos_flow = test_qos_flow_find_by_qfi(sess,
                                QosFlowSetupRequestItem->qosFlowIdentifier);
                        if (!qos_flow)
                            qos_flow = test_qos_flow_add(sess);

                        qos_flow->qfi =
                            QosFlowSetupRequestItem->qosFlowIdentifier;
                    }
                    break;
                case NGAP_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation:
                    UPTransportLayerInformation =
                        &ie2->value.choice.UPTransportLayerInformation;
                    gTPTunnel =
                        UPTransportLayerInformation->choice.gTPTunnel;
                    ogs_assert(gTPTunnel);

                    ogs_asn_BIT_STRING_to_ip(
                            &gTPTunnel->transportLayerAddress,
                            &sess->upf_n3_ip);
                    ogs_asn_OCTET_STRING_to_uint32(
                            &gTPTunnel->gTP_TEID, &sess->upf_n3_teid);
                    break;
                default:
                    break;
                }
            }

            ogs_asn_free(
                    &asn_DEF_NGAP_PDUSessionResourceSetupRequestTransfer,
                    &n2sm_message);

            ogs_pkbuf_free(n2smbuf);
        }
    }

    if (NAS_PDU)
        testngap_send_to_nas(test_ue, NAS_PDU);
}

void testngap_handle_ue_release_context_command(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    int i, rv;
    test_bearer_t *bearer = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UEContextReleaseCommand_t *UEContextReleaseCommand = NULL;

    NGAP_UEContextReleaseCommand_IEs_t *ie = NULL;
    NGAP_UE_NGAP_IDs_t *UE_NGAP_IDs = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UEContextReleaseCommand =
        &initiatingMessage->value.choice.UEContextReleaseCommand;
    ogs_assert(UEContextReleaseCommand);

    for (i = 0; i < UEContextReleaseCommand->protocolIEs.list.count; i++) {
        ie = UEContextReleaseCommand->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_UE_NGAP_IDs:
            UE_NGAP_IDs = &ie->value.choice.UE_NGAP_IDs;
            break;
        default:
            break;
        }
    }

    if (UE_NGAP_IDs) {
        if (UE_NGAP_IDs->present == NGAP_UE_NGAP_IDs_PR_uE_NGAP_ID_pair) {
            uint64_t amf_ue_ngap_id;
            uint64_t ran_ue_ngap_id;
            asn_INTEGER2ulong(
                &UE_NGAP_IDs->choice.uE_NGAP_ID_pair->aMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id);
            test_ue->amf_ue_ngap_id = (uint64_t)amf_ue_ngap_id;

            test_ue->ran_ue_ngap_id =
                UE_NGAP_IDs->choice.uE_NGAP_ID_pair->rAN_UE_NGAP_ID;
        }
    }
}

void testngap_handle_pdu_session_resource_setup_request(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    test_sess_t *sess = NULL;
    test_bearer_t *qos_flow = NULL;
    int rv, i, j, k, l;
    char buf[OGS_ADDRSTRLEN];

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PDUSessionResourceSetupRequest_t *PDUSessionResourceSetupRequest;

    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;

    NGAP_PDUSessionResourceSetupListSUReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemSUReq_t *PDUSessionItem = NULL;

    NGAP_PDUSessionResourceSetupRequestIEs_t *ie = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    NGAP_PDUSessionResourceSetupRequestTransfer_t n2sm_message;
    NGAP_PDUSessionResourceSetupRequestTransferIEs_t *ie2 = NULL;
    NGAP_UPTransportLayerInformation_t *UPTransportLayerInformation = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;
    NGAP_QosFlowSetupRequestList_t *QosFlowSetupRequestList = NULL;
    NGAP_QosFlowSetupRequestItem_t *QosFlowSetupRequestItem = NULL;
    OCTET_STRING_t *transfer = NULL;
    ogs_pkbuf_t *n2smbuf = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    PDUSessionResourceSetupRequest =
        &initiatingMessage->value.choice.PDUSessionResourceSetupRequest;
    ogs_assert(PDUSessionResourceSetupRequest);

    for (i = 0; i < PDUSessionResourceSetupRequest->protocolIEs.list.count;
            i++) {
        ie = PDUSessionResourceSetupRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListSUReq:
            PDUSessionList = &ie->value.choice.PDUSessionResourceSetupListSUReq;
            break;
        default:
            break;
        }
    }

    if (AMF_UE_NGAP_ID) {
        uint64_t amf_ue_ngap_id;
        asn_INTEGER2ulong(AMF_UE_NGAP_ID, (unsigned long *)&amf_ue_ngap_id);
        test_ue->amf_ue_ngap_id = (uint64_t)amf_ue_ngap_id;
    }
    if (RAN_UE_NGAP_ID) {
        test_ue->ran_ue_ngap_id = *RAN_UE_NGAP_ID;
    }

    if (PDUSessionList) {
        for (j = 0; j < PDUSessionList->list.count; j++) {
            PDUSessionItem = (NGAP_PDUSessionResourceSetupItemSUReq_t *)
                PDUSessionList->list.array[j];
            ogs_assert(PDUSessionItem);

            sess = test_sess_find_by_psi(
                        test_ue, PDUSessionItem->pDUSessionID);
            ogs_assert(sess);

            if (PDUSessionItem->pDUSessionNAS_PDU)
                testngap_send_to_nas(
                        test_ue, PDUSessionItem->pDUSessionNAS_PDU);
            transfer = &PDUSessionItem->
                pDUSessionResourceSetupRequestTransfer;
            ogs_assert(transfer);

            n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
            ogs_assert(n2smbuf);
            ogs_pkbuf_put_data(n2smbuf, transfer->buf, transfer->size);

            rv = ogs_asn_decode(
                    &asn_DEF_NGAP_PDUSessionResourceSetupRequestTransfer,
                    &n2sm_message, sizeof(n2sm_message), n2smbuf);
            ogs_assert(rv == OGS_OK);

            for (k = 0; k < n2sm_message.protocolIEs.list.count; k++) {
                ie2 = n2sm_message.protocolIEs.list.array[k];
                switch (ie2->id) {
                case NGAP_ProtocolIE_ID_id_QosFlowSetupRequestList:
                    QosFlowSetupRequestList =
                        &ie2->value.choice.QosFlowSetupRequestList;
                    ogs_assert(QosFlowSetupRequestList);
                    for (l = 0;
                            l < QosFlowSetupRequestList->list.count; l++) {
                        QosFlowSetupRequestItem =
                            (struct NGAP_QosFlowSetupRequestItem *)
                            QosFlowSetupRequestList->list.array[l];
                        ogs_assert(QosFlowSetupRequestItem);
                        qos_flow = test_qos_flow_find_by_qfi(sess,
                                QosFlowSetupRequestItem->qosFlowIdentifier);
                        if (!qos_flow)
                            qos_flow = test_qos_flow_add(sess);

                        qos_flow->qfi =
                            QosFlowSetupRequestItem->qosFlowIdentifier;
                    }
                    break;
                case NGAP_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation:
                    UPTransportLayerInformation =
                        &ie2->value.choice.UPTransportLayerInformation;
                    gTPTunnel =
                        UPTransportLayerInformation->choice.gTPTunnel;
                    ogs_assert(gTPTunnel);

                    ogs_asn_BIT_STRING_to_ip(
                            &gTPTunnel->transportLayerAddress,
                            &sess->upf_n3_ip);
                    ogs_asn_OCTET_STRING_to_uint32(
                            &gTPTunnel->gTP_TEID, &sess->upf_n3_teid);
                    break;
                default:
                    break;
                }
            }

            ogs_asn_free(
                    &asn_DEF_NGAP_PDUSessionResourceSetupRequestTransfer,
                    &n2sm_message);

            ogs_pkbuf_free(n2smbuf);
        }
    }
}

void testngap_handle_pdu_session_resource_modify_request(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    test_sess_t *sess = NULL;
    test_bearer_t *qos_flow = NULL;
    int rv, i, j, k, l;
    char buf[OGS_ADDRSTRLEN];

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PDUSessionResourceModifyRequest_t *PDUSessionResourceModifyRequest;

    NGAP_PDUSessionResourceModifyListModReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceModifyItemModReq_t *PDUSessionItem = NULL;

    NGAP_PDUSessionResourceModifyRequestIEs_t *ie = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    NGAP_PDUSessionResourceModifyRequestTransfer_t n2sm_message;
    NGAP_PDUSessionResourceModifyRequestTransferIEs_t *ie2 = NULL;
    NGAP_UPTransportLayerInformation_t *UPTransportLayerInformation = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;
    NGAP_QosFlowAddOrModifyRequestList_t *QosFlowAddOrModifyRequestList = NULL;
    NGAP_QosFlowAddOrModifyRequestItem_t *QosFlowAddOrModifyRequestItem = NULL;
    OCTET_STRING_t *transfer = NULL;
    ogs_pkbuf_t *n2smbuf = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    PDUSessionResourceModifyRequest =
        &initiatingMessage->value.choice.PDUSessionResourceModifyRequest;
    ogs_assert(PDUSessionResourceModifyRequest);

    for (i = 0; i < PDUSessionResourceModifyRequest->protocolIEs.list.count;
            i++) {
        ie = PDUSessionResourceModifyRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceModifyListModReq:
            PDUSessionList =
                &ie->value.choice.PDUSessionResourceModifyListModReq;
            ogs_assert(PDUSessionList);
            for (j = 0; j < PDUSessionList->list.count; j++) {
                PDUSessionItem = (NGAP_PDUSessionResourceModifyItemModReq_t *)
                    PDUSessionList->list.array[j];
                ogs_assert(PDUSessionItem);

                sess = test_sess_find_by_psi(
                            test_ue, PDUSessionItem->pDUSessionID);
                ogs_assert(sess);

                if (PDUSessionItem->nAS_PDU)
                    testngap_send_to_nas(test_ue, PDUSessionItem->nAS_PDU);
                transfer = &PDUSessionItem->
                    pDUSessionResourceModifyRequestTransfer;
                ogs_assert(transfer);

                n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
                ogs_assert(n2smbuf);
                ogs_pkbuf_put_data(n2smbuf, transfer->buf, transfer->size);

                rv = ogs_asn_decode(
                        &asn_DEF_NGAP_PDUSessionResourceModifyRequestTransfer,
                        &n2sm_message, sizeof(n2sm_message), n2smbuf);
                ogs_assert(rv == OGS_OK);

                for (k = 0; k < n2sm_message.protocolIEs.list.count; k++) {
                    ie2 = n2sm_message.protocolIEs.list.array[k];
                    switch (ie2->id) {
                    case NGAP_ProtocolIE_ID_id_QosFlowAddOrModifyRequestList:
                        QosFlowAddOrModifyRequestList =
                            &ie2->value.choice.QosFlowAddOrModifyRequestList;
                        ogs_assert(QosFlowAddOrModifyRequestList);
                        for (l = 0;
                                l < QosFlowAddOrModifyRequestList->list.count;
                                l++) {
                            QosFlowAddOrModifyRequestItem =
                                (struct NGAP_QosFlowAddOrModifyRequestItem *)
                                QosFlowAddOrModifyRequestList->list.array[l];
                            ogs_assert(QosFlowAddOrModifyRequestItem);
                            qos_flow = test_qos_flow_find_by_qfi(sess,
                                    QosFlowAddOrModifyRequestItem->
                                        qosFlowIdentifier);
                            if (!qos_flow)
                                qos_flow = test_qos_flow_add(sess);

                            qos_flow->qfi = QosFlowAddOrModifyRequestItem->
                                                qosFlowIdentifier;
                        }
                        break;
                    default:
                        break;
                    }
                }

                ogs_asn_free(
                        &asn_DEF_NGAP_PDUSessionResourceModifyRequestTransfer,
                        &n2sm_message);

                ogs_pkbuf_free(n2smbuf);
            }
            break;
        default:
            break;
        }
    }
}

void testngap_handle_pdu_session_resource_release_command(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    int rv, i, j, k;
    char buf[OGS_ADDRSTRLEN];

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PDUSessionResourceReleaseCommand_t *PDUSessionResourceReleaseCommand;

    NGAP_PDUSessionResourceReleaseCommandIEs_t *ie = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    PDUSessionResourceReleaseCommand =
        &initiatingMessage->value.choice.PDUSessionResourceReleaseCommand;
    ogs_assert(PDUSessionResourceReleaseCommand);

    for (i = 0; i < PDUSessionResourceReleaseCommand->protocolIEs.list.count;
            i++) {
        ie = PDUSessionResourceReleaseCommand->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = &ie->value.choice.NAS_PDU;
            break;
        default:
            break;
        }
    }

    if (NAS_PDU)
        testngap_send_to_nas(test_ue, NAS_PDU);
}

void testngap_handle_handover_request(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    int rv, i, j, k;
    char buf[OGS_ADDRSTRLEN];

    test_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverRequest_t *HandoverRequest = NULL;

    NGAP_HandoverRequestIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceSetupListHOReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemHOReq_t *PDUSessionItem = NULL;

    NGAP_PDUSessionResourceSetupRequestTransfer_t n2sm_message;
    NGAP_PDUSessionResourceSetupRequestTransferIEs_t *ie2 = NULL;
    OCTET_STRING_t *transfer = NULL;
    ogs_pkbuf_t *n2smbuf = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverRequest = &initiatingMessage->value.choice.HandoverRequest;
    ogs_assert(HandoverRequest);

    for (i = 0; i < HandoverRequest->protocolIEs.list.count; i++) {
        ie = HandoverRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListHOReq:
            PDUSessionList = &ie->value.choice.PDUSessionResourceSetupListHOReq;
        default:
            break;
        }
    }

    if (AMF_UE_NGAP_ID) {
        uint64_t amf_ue_ngap_id;
        asn_INTEGER2ulong(AMF_UE_NGAP_ID, (unsigned long *)&amf_ue_ngap_id);
        test_ue->amf_ue_ngap_id = (uint64_t)amf_ue_ngap_id;
    }

    if (PDUSessionList) {
        for (j = 0; j < PDUSessionList->list.count; j++) {
            PDUSessionItem = (NGAP_PDUSessionResourceSetupItemHOReq_t *)
                PDUSessionList->list.array[j];
            ogs_assert(PDUSessionItem);

            sess = test_sess_find_by_psi(
                        test_ue, PDUSessionItem->pDUSessionID);
            ogs_assert(sess);

            transfer = &PDUSessionItem->handoverRequestTransfer;
            ogs_assert(transfer);

            n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
            ogs_assert(n2smbuf);
            ogs_pkbuf_put_data(n2smbuf, transfer->buf, transfer->size);

            rv = ogs_asn_decode(
                    &asn_DEF_NGAP_PDUSessionResourceSetupRequestTransfer,
                    &n2sm_message, sizeof(n2sm_message), n2smbuf);
            ogs_assert(rv == OGS_OK);

            for (k = 0; k < n2sm_message.protocolIEs.list.count; k++) {
                ie2 = n2sm_message.protocolIEs.list.array[k];
                switch (ie2->id) {
                case NGAP_ProtocolIE_ID_id_DataForwardingNotPossible:
                    sess->handover.data_forwarding_not_possible = true;
                    break;
                default:
                    break;
                }
            }

            ogs_asn_free(
                    &asn_DEF_NGAP_PDUSessionResourceSetupRequestTransfer,
                    &n2sm_message);

            ogs_pkbuf_free(n2smbuf);
        }
    }
}

void testngap_handle_handover_command(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    int rv, i, j;
    char buf[OGS_ADDRSTRLEN];

    test_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_HandoverCommand_t *HandoverCommand = NULL;

    NGAP_HandoverCommandIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceHandoverList_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceHandoverItem_t *PDUSessionItem = NULL;

    NGAP_HandoverCommandTransfer_t n2sm_message;
    OCTET_STRING_t *transfer = NULL;
    ogs_pkbuf_t *n2smbuf = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    HandoverCommand = &successfulOutcome->value.choice.HandoverCommand;
    ogs_assert(HandoverCommand);

    for (i = 0; i < HandoverCommand->protocolIEs.list.count; i++) {
        ie = HandoverCommand->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceHandoverList:
            PDUSessionList =
                &ie->value.choice.PDUSessionResourceHandoverList;
        default:
            break;
        }
    }

    if (AMF_UE_NGAP_ID) {
        uint64_t amf_ue_ngap_id;
        asn_INTEGER2ulong(AMF_UE_NGAP_ID, (unsigned long *)&amf_ue_ngap_id);
        test_ue->amf_ue_ngap_id = (uint64_t)amf_ue_ngap_id;
    }

    if (RAN_UE_NGAP_ID) {
        test_ue->ran_ue_ngap_id = *RAN_UE_NGAP_ID;
    }

    if (PDUSessionList) {
        for (j = 0; j < PDUSessionList->list.count; j++) {
            NGAP_UPTransportLayerInformation_t *dLForwardingUP_TNLInformation;

            PDUSessionItem = (NGAP_PDUSessionResourceHandoverItem_t *)
                PDUSessionList->list.array[j];
            ogs_assert(PDUSessionItem);

            sess = test_sess_find_by_psi(
                        test_ue, PDUSessionItem->pDUSessionID);
            ogs_assert(sess);

            transfer = &PDUSessionItem->handoverCommandTransfer;
            ogs_assert(transfer);

            n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
            ogs_assert(n2smbuf);
            ogs_pkbuf_put_data(n2smbuf, transfer->buf, transfer->size);

            rv = ogs_asn_decode(
                    &asn_DEF_NGAP_HandoverCommandTransfer,
                    &n2sm_message, sizeof(n2sm_message), n2smbuf);
            ogs_assert(rv == OGS_OK);

            dLForwardingUP_TNLInformation =
                n2sm_message.dLForwardingUP_TNLInformation;
            if (dLForwardingUP_TNLInformation) {
                if (dLForwardingUP_TNLInformation->present ==
                    NGAP_UPTransportLayerInformation_PR_gTPTunnel) {
                    NGAP_GTPTunnel_t *gTPTunnel =
                        dLForwardingUP_TNLInformation->choice.gTPTunnel;
                    if (gTPTunnel) {
                        ogs_asn_BIT_STRING_to_ip(
                                &gTPTunnel->transportLayerAddress,
                                &sess->handover.upf_dl_ip);
                        ogs_asn_OCTET_STRING_to_uint32(
                                &gTPTunnel->gTP_TEID,
                                &sess->handover.upf_dl_teid);
                    }
                }
            }

            ogs_asn_free(
                    &asn_DEF_NGAP_HandoverCommandTransfer,
                    &n2sm_message);

            ogs_pkbuf_free(n2smbuf);
        }
    }
}

void testngap_handle_handover_preparation_failure(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    int rv, i, j;
    char buf[OGS_ADDRSTRLEN];

    test_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_HandoverPreparationFailure_t *HandoverPreparationFailure = NULL;

    NGAP_HandoverPreparationFailureIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    unsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    HandoverPreparationFailure =
        &unsuccessfulOutcome->value.choice.HandoverPreparationFailure;
    ogs_assert(HandoverPreparationFailure);

    for (i = 0; i < HandoverPreparationFailure->protocolIEs.list.count; i++) {
        ie = HandoverPreparationFailure->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        default:
            break;
        }
    }

    if (AMF_UE_NGAP_ID) {
        uint64_t amf_ue_ngap_id;
        asn_INTEGER2ulong(AMF_UE_NGAP_ID, (unsigned long *)&amf_ue_ngap_id);
        test_ue->amf_ue_ngap_id = (uint64_t)amf_ue_ngap_id;
    }

    if (RAN_UE_NGAP_ID) {
        test_ue->ran_ue_ngap_id = *RAN_UE_NGAP_ID;
    }
}

void testngap_handle_handover_cancel_ack(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    int rv, i, j;
    char buf[OGS_ADDRSTRLEN];

    test_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_HandoverCancelAcknowledge_t *HandoverCancelAcknowledge = NULL;

    NGAP_HandoverCancelAcknowledgeIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    HandoverCancelAcknowledge = &successfulOutcome->value.choice.HandoverCancelAcknowledge;
    ogs_assert(HandoverCancelAcknowledge);

    for (i = 0; i < HandoverCancelAcknowledge->protocolIEs.list.count; i++) {
        ie = HandoverCancelAcknowledge->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        default:
            break;
        }
    }

    if (AMF_UE_NGAP_ID) {
        uint64_t amf_ue_ngap_id;
        asn_INTEGER2ulong(AMF_UE_NGAP_ID, (unsigned long *)&amf_ue_ngap_id);
        test_ue->amf_ue_ngap_id = (uint64_t)amf_ue_ngap_id;
    }

    if (RAN_UE_NGAP_ID) {
        test_ue->ran_ue_ngap_id = *RAN_UE_NGAP_ID;
    }
}

void testngap_handle_downlink_ran_status_transfer(
        test_ue_t *test_ue, ogs_ngap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_DownlinkRANStatusTransfer_t *DownlinkRANStatusTransfer = NULL;

    NGAP_DownlinkRANStatusTransferIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    DownlinkRANStatusTransfer =
        &initiatingMessage->value.choice.DownlinkRANStatusTransfer;
    ogs_assert(DownlinkRANStatusTransfer);

    for (i = 0; i < DownlinkRANStatusTransfer->protocolIEs.list.count; i++) {
        ie = DownlinkRANStatusTransfer->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        default:
            break;
        }
    }

    if (AMF_UE_NGAP_ID) {
        uint64_t amf_ue_ngap_id;
        asn_INTEGER2ulong(AMF_UE_NGAP_ID, (unsigned long *)&amf_ue_ngap_id);
        test_ue->amf_ue_ngap_id = (uint64_t)amf_ue_ngap_id;
    }
    if (RAN_UE_NGAP_ID) {
        test_ue->ran_ue_ngap_id = *RAN_UE_NGAP_ID;
    }
}
