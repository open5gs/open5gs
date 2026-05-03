/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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

void tests1ap_handle_s1_setup_response(ogs_s1ap_message_t *message)
{
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_S1SetupResponse_t *S1SetupResponse = NULL;

    S1AP_S1SetupResponseIEs_t *ie = NULL;

    ogs_assert(message);

    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    S1SetupResponse = successfulOutcome->value.choice.S1SetupResponse;
    ogs_assert(S1SetupResponse);

    ogs_debug("S1 setup response");
}

void tests1ap_handle_downlink_nas_transport(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int i;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_DownlinkNASTransport_t *DownlinkNASTransport = NULL;

    S1AP_DownlinkNASTransport_IEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    DownlinkNASTransport = initiatingMessage->value.choice.DownlinkNASTransport;
    ogs_assert(DownlinkNASTransport);

    for (i = 0; i < OGS_ASN_LIST_COUNT(DownlinkNASTransport->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(DownlinkNASTransport->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = ie->value.choice.NAS_PDU;
            break;
        default:
            break;
        }
    }

    if (MME_UE_S1AP_ID)
        test_ue->mme_ue_s1ap_id = *MME_UE_S1AP_ID;
    if (ENB_UE_S1AP_ID)
        test_ue->enb_ue_s1ap_id = *ENB_UE_S1AP_ID;

    if (NAS_PDU)
        tests1ap_send_to_nas(test_ue, NAS_PDU);
}

void tests1ap_handle_initial_context_setup_request(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int i, rv;
    test_bearer_t *bearer = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_InitialContextSetupRequest_t *InitialContextSetupRequest = NULL;

    S1AP_InitialContextSetupRequestIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;
    S1AP_E_RABToBeSetupListCtxtSUReq_t *E_RABToBeSetupListCtxtSUReq = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    InitialContextSetupRequest = initiatingMessage->value.choice.InitialContextSetupRequest;
    ogs_assert(InitialContextSetupRequest);

    for (i = 0; i < OGS_ASN_LIST_COUNT(
                InitialContextSetupRequest->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(InitialContextSetupRequest->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABToBeSetupListCtxtSUReq:
            E_RABToBeSetupListCtxtSUReq = ie->value.choice.E_RABToBeSetupListCtxtSUReq;
            break;
        default:
            break;
        }
    }

    if (MME_UE_S1AP_ID)
        test_ue->mme_ue_s1ap_id = *MME_UE_S1AP_ID;
    if (ENB_UE_S1AP_ID)
        test_ue->enb_ue_s1ap_id = *ENB_UE_S1AP_ID;

    ogs_assert(E_RABToBeSetupListCtxtSUReq);
    for (i = 0; i < OGS_ASN_LIST_COUNT(E_RABToBeSetupListCtxtSUReq); i++) {
        S1AP_E_RABToBeSetupItemCtxtSUReqIEs_t *ie2 = NULL;
        S1AP_E_RABToBeSetupItemCtxtSUReq_t *e_rab = NULL;

        ie2 = OGS_ASN_LIST_GET(E_RABToBeSetupListCtxtSUReq, i);
        ogs_assert(ie2);
        e_rab = ie2->value.choice.E_RABToBeSetupItemCtxtSUReq;

        if (e_rab->nAS_PDU)
            tests1ap_send_to_nas(test_ue, e_rab->nAS_PDU);

        bearer = test_bearer_find_by_ue_ebi(test_ue, e_rab->e_RAB_ID);
        ogs_assert(bearer);

        memcpy(&bearer->sgw_s1u_teid, e_rab->gTP_TEID.buf,
                sizeof(bearer->sgw_s1u_teid));
        bearer->sgw_s1u_teid = be32toh(bearer->sgw_s1u_teid);
        rv = ogs_asn_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->sgw_s1u_ip);
        ogs_assert(rv == OGS_OK);
    }
}

void tests1ap_handle_ue_release_context_command(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int i, rv;
    test_bearer_t *bearer = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_UEContextReleaseCommand_t *UEContextReleaseCommand = NULL;

    S1AP_UEContextReleaseCommand_IEs_t *ie = NULL;
    S1AP_UE_S1AP_IDs_t *UE_S1AP_IDs = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UEContextReleaseCommand = initiatingMessage->value.choice.UEContextReleaseCommand;
    ogs_assert(UEContextReleaseCommand);

    for (i = 0; i < OGS_ASN_LIST_COUNT(
                UEContextReleaseCommand->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(UEContextReleaseCommand->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_UE_S1AP_IDs:
            UE_S1AP_IDs = ie->value.choice.UE_S1AP_IDs;
            break;
        default:
            break;
        }
    }

    if (UE_S1AP_IDs) {
        if (UE_S1AP_IDs->present == S1AP_UE_S1AP_IDs_PR_uE_S1AP_ID_pair) {
            test_ue->mme_ue_s1ap_id =
                UE_S1AP_IDs->choice.uE_S1AP_ID_pair->mME_UE_S1AP_ID;
            test_ue->enb_ue_s1ap_id =
                UE_S1AP_IDs->choice.uE_S1AP_ID_pair->eNB_UE_S1AP_ID;
        }
    }
}

void tests1ap_handle_e_rab_setup_request(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int i, rv;
    test_bearer_t *bearer = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_E_RABSetupRequest_t *E_RABSetupRequest = NULL;

    S1AP_E_RABSetupRequestIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;
    S1AP_E_RABToBeSetupListBearerSUReq_t *E_RABToBeSetupListBearerSUReq = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    E_RABSetupRequest = initiatingMessage->value.choice.E_RABSetupRequest;
    ogs_assert(E_RABSetupRequest);

    for (i = 0; i < OGS_ASN_LIST_COUNT(E_RABSetupRequest->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(E_RABSetupRequest->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABToBeSetupListBearerSUReq:
            E_RABToBeSetupListBearerSUReq = ie->value.choice.E_RABToBeSetupListBearerSUReq;
            break;
        default:
            break;
        }
    }

    if (MME_UE_S1AP_ID)
        test_ue->mme_ue_s1ap_id = *MME_UE_S1AP_ID;

    ogs_assert(E_RABToBeSetupListBearerSUReq);
    for (i = 0; i < OGS_ASN_LIST_COUNT(E_RABToBeSetupListBearerSUReq); i++) {
        S1AP_E_RABToBeSetupItemBearerSUReqIEs_t *ie2 = NULL;
        S1AP_E_RABToBeSetupItemBearerSUReq_t *e_rab = NULL;

        ie2 = OGS_ASN_LIST_GET(E_RABToBeSetupListBearerSUReq, i);
        ogs_assert(ie2);
        e_rab = ie2->value.choice.E_RABToBeSetupItemBearerSUReq;

        tests1ap_send_to_nas(test_ue, &e_rab->nAS_PDU);

        bearer = test_bearer_find_by_ue_ebi(test_ue, e_rab->e_RAB_ID);
        ogs_assert(bearer);

        memcpy(&bearer->sgw_s1u_teid, e_rab->gTP_TEID.buf,
                sizeof(bearer->sgw_s1u_teid));
        bearer->sgw_s1u_teid = be32toh(bearer->sgw_s1u_teid);
        rv = ogs_asn_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->sgw_s1u_ip);
        ogs_assert(rv == OGS_OK);
    }
}

void tests1ap_handle_e_rab_modify_request(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int i, rv;
    test_bearer_t *bearer = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_E_RABModifyRequest_t *E_RABModifyRequest = NULL;

    S1AP_E_RABModifyRequestIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;
    S1AP_E_RABToBeModifiedListBearerModReq_t *E_RABToBeModifiedListBearerModReq
        = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    E_RABModifyRequest = initiatingMessage->value.choice.E_RABModifyRequest;
    ogs_assert(E_RABModifyRequest);

    for (i = 0; i < OGS_ASN_LIST_COUNT(E_RABModifyRequest->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(E_RABModifyRequest->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABToBeModifiedListBearerModReq:
            E_RABToBeModifiedListBearerModReq = ie->value.choice.E_RABToBeModifiedListBearerModReq;
            break;
        default:
            break;
        }
    }

    if (MME_UE_S1AP_ID)
        test_ue->mme_ue_s1ap_id = *MME_UE_S1AP_ID;

    ogs_assert(E_RABToBeModifiedListBearerModReq);
    for (i = 0; i < OGS_ASN_LIST_COUNT(E_RABToBeModifiedListBearerModReq); i++) {
        S1AP_E_RABToBeModifiedItemBearerModReqIEs_t *ie2 = NULL;
        S1AP_E_RABToBeModifiedItemBearerModReq_t *e_rab = NULL;

        ie2 = OGS_ASN_LIST_GET(E_RABToBeModifiedListBearerModReq, i);
        ogs_assert(ie2);
        e_rab = ie2->value.choice.E_RABToBeModifiedItemBearerModReq;

        tests1ap_send_to_nas(test_ue, &e_rab->nAS_PDU);
    }
}

void tests1ap_handle_e_rab_release_command(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int i, rv;
    test_bearer_t *bearer = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_E_RABReleaseCommand_t *E_RABReleaseCommand = NULL;

    S1AP_E_RABReleaseCommandIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;
    S1AP_E_RABList_t *E_RABList = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    E_RABReleaseCommand = initiatingMessage->value.choice.E_RABReleaseCommand;
    ogs_assert(E_RABReleaseCommand);

    for (i = 0; i < OGS_ASN_LIST_COUNT(E_RABReleaseCommand->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(E_RABReleaseCommand->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABToBeReleasedList:
            E_RABList = ie->value.choice.E_RABList;
            break;
        case S1AP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = ie->value.choice.NAS_PDU;
            break;
        default:
            break;
        }
    }

    if (MME_UE_S1AP_ID)
        test_ue->mme_ue_s1ap_id = *MME_UE_S1AP_ID;

    ogs_assert(E_RABList);
    for (i = 0; i < OGS_ASN_LIST_COUNT(E_RABList); i++) {
        S1AP_E_RABItemIEs_t *ie2 = NULL;
        S1AP_E_RABItem_t *e_rab = NULL;

        ie2 = OGS_ASN_LIST_GET(E_RABList, i);
        ogs_assert(ie2);
        e_rab = ie2->value.choice.E_RABItem;
        ogs_assert(e_rab);
    }

    if (NAS_PDU)
        tests1ap_send_to_nas(test_ue, NAS_PDU);
}

void tests1ap_handle_handover_request(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int i, rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_HandoverRequest_t *HandoverRequest = NULL;

    S1AP_HandoverRequestIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverRequest = initiatingMessage->value.choice.HandoverRequest;
    ogs_assert(HandoverRequest);

    for (i = 0; i < OGS_ASN_LIST_COUNT(HandoverRequest->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(HandoverRequest->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
            break;
        default:
            break;
        }
    }

    if (MME_UE_S1AP_ID)
        test_ue->mme_ue_s1ap_id = *MME_UE_S1AP_ID;
}

void tests1ap_handle_path_switch_request_ack(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int rv, i;
    char buf[OGS_ADDRSTRLEN];

    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_PathSwitchRequestAcknowledge_t *PathSwitchRequestAcknowledge = NULL;

    S1AP_PathSwitchRequestAcknowledgeIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABToBeSwitchedULList_t *E_RABToBeSwitchedULList = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    PathSwitchRequestAcknowledge = successfulOutcome->value.choice.PathSwitchRequestAcknowledge;
    ogs_assert(PathSwitchRequestAcknowledge);

    for (i = 0; i < OGS_ASN_LIST_COUNT(PathSwitchRequestAcknowledge->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(PathSwitchRequestAcknowledge->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedULList:
            E_RABToBeSwitchedULList = ie->value.choice.E_RABToBeSwitchedULList;
            break;
        default:
            break;
        }
    }

    if (MME_UE_S1AP_ID)
        test_ue->mme_ue_s1ap_id = *MME_UE_S1AP_ID;
    if (ENB_UE_S1AP_ID)
        test_ue->enb_ue_s1ap_id = *ENB_UE_S1AP_ID;

    if (E_RABToBeSwitchedULList) {
        for (i = 0; i < OGS_ASN_LIST_COUNT(E_RABToBeSwitchedULList); i++) {
            S1AP_E_RABToBeSwitchedULItemIEs_t *ie2 = NULL;
            S1AP_E_RABToBeSwitchedULItem_t *e_rab = NULL;

            ie2 = OGS_ASN_LIST_GET(E_RABToBeSwitchedULList, i);
            ogs_assert(ie2);
            e_rab = ie2->value.choice.E_RABToBeSwitchedULItem;

            bearer = test_bearer_find_by_ue_ebi(test_ue, e_rab->e_RAB_ID);
            ogs_assert(bearer);

            memcpy(&bearer->sgw_s1u_teid, e_rab->gTP_TEID.buf,
                    sizeof(bearer->sgw_s1u_teid));
            bearer->sgw_s1u_teid = be32toh(bearer->sgw_s1u_teid);
            rv = ogs_asn_BIT_STRING_to_ip(
                    &e_rab->transportLayerAddress, &bearer->sgw_s1u_ip);
            ogs_assert(rv == OGS_OK);
        }
    }
}

void tests1ap_handle_handover_command(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];

    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_HandoverCommand_t *HandoverCommand = NULL;

    S1AP_HandoverCommandIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABSubjecttoDataForwardingList_t
        *E_RABSubjecttoDataForwardingList = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    HandoverCommand = successfulOutcome->value.choice.HandoverCommand;
    ogs_assert(HandoverCommand);

    for (i = 0; i < OGS_ASN_LIST_COUNT(HandoverCommand->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(HandoverCommand->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABSubjecttoDataForwardingList:
            E_RABSubjecttoDataForwardingList =
                ie->value.choice.E_RABSubjecttoDataForwardingList;
            break;
        default:
            break;
        }
    }

    if (MME_UE_S1AP_ID)
        test_ue->mme_ue_s1ap_id = *MME_UE_S1AP_ID;
    if (ENB_UE_S1AP_ID)
        test_ue->enb_ue_s1ap_id = *ENB_UE_S1AP_ID;

    if (E_RABSubjecttoDataForwardingList) {
        for (i = 0;
                i < OGS_ASN_LIST_COUNT(E_RABSubjecttoDataForwardingList);
                i++) {
            S1AP_E_RABDataForwardingItemIEs_t *ie = NULL;
            S1AP_E_RABDataForwardingItem_t *e_rab = NULL;

            ie = OGS_ASN_LIST_GET(E_RABSubjecttoDataForwardingList, i);
            ogs_assert(ie);
            e_rab = ie->value.choice.E_RABDataForwardingItem;

            bearer = test_bearer_find_by_ue_ebi(test_ue, e_rab->e_RAB_ID);
            ogs_assert(bearer);

            if (e_rab->dL_gTP_TEID) {
                memcpy(&bearer->handover.dl_teid, e_rab->dL_gTP_TEID->buf,
                        sizeof(bearer->handover.dl_teid));
                bearer->handover.dl_teid = be32toh(bearer->handover.dl_teid);
            }
            if (e_rab->dL_transportLayerAddress) {
                ogs_assert(OGS_OK ==
                        ogs_asn_BIT_STRING_to_ip(
                            e_rab->dL_transportLayerAddress,
                            &bearer->handover.dl_ip));
            }
            if (e_rab->uL_GTP_TEID) {
                memcpy(&bearer->handover.ul_teid, e_rab->uL_GTP_TEID->buf,
                        sizeof(bearer->handover.ul_teid));
                bearer->handover.ul_teid = be32toh(bearer->handover.ul_teid);
            }
            if (e_rab->uL_TransportLayerAddress) {
                ogs_assert(OGS_OK ==
                        ogs_asn_BIT_STRING_to_ip(
                            e_rab->uL_TransportLayerAddress,
                            &bearer->handover.ul_ip));
            }
        }
    }
}

void tests1ap_handle_handover_preparation_failure(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];

    test_sess_t *sess = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    S1AP_HandoverPreparationFailure_t *HandoverPreparationFailure = NULL;

    S1AP_HandoverPreparationFailureIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    unsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    HandoverPreparationFailure = unsuccessfulOutcome->value.choice.HandoverPreparationFailure;
    ogs_assert(HandoverPreparationFailure);

    for (i = 0; i < OGS_ASN_LIST_COUNT(HandoverPreparationFailure->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(HandoverPreparationFailure->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
            break;
        default:
            break;
        }
    }

    if (MME_UE_S1AP_ID)
        test_ue->mme_ue_s1ap_id = *MME_UE_S1AP_ID;
    if (ENB_UE_S1AP_ID)
        test_ue->enb_ue_s1ap_id = *ENB_UE_S1AP_ID;
}

void tests1ap_handle_handover_cancel_ack(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];

    test_sess_t *sess = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_HandoverCancelAcknowledge_t *HandoverCancelAcknowledge = NULL;

    S1AP_HandoverCancelAcknowledgeIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    HandoverCancelAcknowledge = successfulOutcome->value.choice.HandoverCancelAcknowledge;
    ogs_assert(HandoverCancelAcknowledge);

    for (i = 0; i < OGS_ASN_LIST_COUNT(HandoverCancelAcknowledge->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(HandoverCancelAcknowledge->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
            break;
        default:
            break;
        }
    }

    if (MME_UE_S1AP_ID)
        test_ue->mme_ue_s1ap_id = *MME_UE_S1AP_ID;
    if (ENB_UE_S1AP_ID)
        test_ue->enb_ue_s1ap_id = *ENB_UE_S1AP_ID;
}

void tests1ap_handle_mme_status_transfer(
        test_ue_t *test_ue, ogs_s1ap_message_t *message)
{
    int i;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_MMEStatusTransfer_t *MMEStatusTransfer = NULL;

    S1AP_MMEStatusTransferIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;

    ogs_assert(test_ue);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    MMEStatusTransfer = initiatingMessage->value.choice.MMEStatusTransfer;
    ogs_assert(MMEStatusTransfer);

    for (i = 0; i < OGS_ASN_LIST_COUNT(MMEStatusTransfer->protocolIEs); i++) {
        ie = OGS_ASN_LIST_GET(MMEStatusTransfer->protocolIEs, i);
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
            break;
        default:
            break;
        }
    }

    if (MME_UE_S1AP_ID)
        test_ue->mme_ue_s1ap_id = *MME_UE_S1AP_ID;
    if (ENB_UE_S1AP_ID)
        test_ue->enb_ue_s1ap_id = *ENB_UE_S1AP_ID;
}
