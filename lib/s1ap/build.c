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

#include "ogs-s1ap.h"

ogs_pkbuf_t *ogs_s1ap_build_error_indication(
        S1AP_MME_UE_S1AP_ID_t *mme_ue_s1ap_id,
        S1AP_ENB_UE_S1AP_ID_t *enb_ue_s1ap_id,
        S1AP_Cause_PR group, long cause)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_ErrorIndication_t *ErrorIndication = NULL;

    S1AP_ErrorIndicationIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    ogs_debug("Error Indication");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));
    ogs_assert(pdu.choice.initiatingMessage);

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_ErrorIndication;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_ErrorIndication;

    initiatingMessage->value.choice.ErrorIndication =
        CALLOC(1, sizeof(*ErrorIndication));
    ogs_assert(initiatingMessage->value.choice.ErrorIndication);
    ErrorIndication = initiatingMessage->value.choice.ErrorIndication;

    ErrorIndication->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_ErrorIndication);
    ogs_assert(ErrorIndication->protocolIEs);

    if (mme_ue_s1ap_id) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(ErrorIndication->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present = S1AP_ErrorIndicationIEs__value_PR_MME_UE_S1AP_ID;

        ie->value.choice.MME_UE_S1AP_ID =
            CALLOC(1, sizeof(*MME_UE_S1AP_ID));
        MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
        ogs_assert(MME_UE_S1AP_ID);

        *MME_UE_S1AP_ID = *mme_ue_s1ap_id;
        ogs_debug("    MME_UE_S1AP_ID[%d]", (int)*mme_ue_s1ap_id);
    }

    if (enb_ue_s1ap_id) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(ErrorIndication->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present = S1AP_ErrorIndicationIEs__value_PR_ENB_UE_S1AP_ID;

        ie->value.choice.ENB_UE_S1AP_ID =
            CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
        ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
        ogs_assert(ENB_UE_S1AP_ID);

        *ENB_UE_S1AP_ID = *enb_ue_s1ap_id;
        ogs_debug("    ENB_UE_S1AP_ID[%d]", (int)*enb_ue_s1ap_id);
    }

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(ErrorIndication->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_ErrorIndicationIEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    ogs_debug("    Group[%d] Cause[%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *ogs_s1ap_build_s1_reset(
        S1AP_Cause_PR group, long cause,
        S1AP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_Reset_t *Reset = NULL;

    S1AP_ResetIEs_t *ie = NULL;
    S1AP_Cause_t *Cause = NULL;
    S1AP_ResetType_t *ResetType = NULL;

    ogs_debug("Reset");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));
    ogs_assert(pdu.choice.initiatingMessage);

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_Reset;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present = S1AP_InitiatingMessage__value_PR_Reset;

    initiatingMessage->value.choice.Reset =
        CALLOC(1, sizeof(*Reset));
    ogs_assert(initiatingMessage->value.choice.Reset);
    Reset = initiatingMessage->value.choice.Reset;

    Reset->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_Reset);
    ogs_assert(Reset->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(Reset->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_ResetIEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(Reset->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_ResetType;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_ResetIEs__value_PR_ResetType;

    ie->value.choice.ResetType =
        CALLOC(1, sizeof(*ResetType));
    ResetType = ie->value.choice.ResetType;
    ogs_assert(ResetType);

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    ogs_debug("    Group[%d] Cause[%d] partOfS1_Interface[%p]",
        Cause->present, (int)Cause->choice.radioNetwork, partOfS1_Interface);

    if (partOfS1_Interface) {
        ResetType->present = S1AP_ResetType_PR_partOfS1_Interface;
        ResetType->choice.partOfS1_Interface = partOfS1_Interface;
    } else {
        ResetType->present = S1AP_ResetType_PR_s1_Interface;
        ResetType->choice.s1_Interface = S1AP_ResetAll_reset_all;
    }

    return ogs_s1ap_encode(&pdu);
}

void ogs_s1ap_build_part_of_s1_interface(
    S1AP_UE_associatedLogicalS1_ConnectionListRes_t **list,
    uint32_t *mme_ue_s1ap_id,
    uint32_t *enb_ue_s1ap_id)
{
    S1AP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface = NULL;
    S1AP_UE_associatedLogicalS1_ConnectionItemRes_t *ie = NULL;
    S1AP_UE_associatedLogicalS1_ConnectionItem_t *item = NULL;

    ogs_assert(list);
    ogs_assert(mme_ue_s1ap_id || enb_ue_s1ap_id);

    if (!*list)
        *list = CALLOC(1, sizeof(**list));
    ogs_assert(*list);

    partOfS1_Interface = *list;
    ogs_assert(partOfS1_Interface);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(partOfS1_Interface, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionItem;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UE_associatedLogicalS1_ConnectionItemRes__value_PR_UE_associatedLogicalS1_ConnectionItem;

    ie->value.choice.UE_associatedLogicalS1_ConnectionItem =
        CALLOC(1, sizeof(*item));
    item = ie->value.choice.UE_associatedLogicalS1_ConnectionItem;
    ogs_assert(item);
    if (mme_ue_s1ap_id) {
        item->mME_UE_S1AP_ID = CALLOC(1, sizeof(*item->mME_UE_S1AP_ID));
        ogs_assert(item->mME_UE_S1AP_ID);
        *item->mME_UE_S1AP_ID = *mme_ue_s1ap_id;
    }

    if (enb_ue_s1ap_id) {
        item->eNB_UE_S1AP_ID = CALLOC(1, sizeof(*item->eNB_UE_S1AP_ID));
        ogs_assert(item->eNB_UE_S1AP_ID);
        *item->eNB_UE_S1AP_ID = *enb_ue_s1ap_id;
    }
}

ogs_pkbuf_t *ogs_s1ap_build_s1_reset_ack(
        S1AP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_ResetAcknowledge_t *ResetAcknowledge = NULL;

    S1AP_ResetAcknowledgeIEs_t *ie = NULL;

    ogs_debug("Reset acknowledge");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));
    ogs_assert(pdu.choice.successfulOutcome);

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_Reset;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_ResetAcknowledge;

    successfulOutcome->value.choice.ResetAcknowledge =
        CALLOC(1, sizeof(*ResetAcknowledge));
    ogs_assert(successfulOutcome->value.choice.ResetAcknowledge);
    ResetAcknowledge = successfulOutcome->value.choice.ResetAcknowledge;

    ResetAcknowledge->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_ResetAcknowledge);
    ogs_assert(ResetAcknowledge->protocolIEs);

    if (partOfS1_Interface && OGS_ASN_LIST_COUNT(partOfS1_Interface)) {
        int i;
        S1AP_UE_associatedLogicalS1_ConnectionListResAck_t *list = NULL;

        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(ResetAcknowledge->protocolIEs, ie);

        ie->id =
            S1AP_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionListResAck;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_ResetAcknowledgeIEs__value_PR_UE_associatedLogicalS1_ConnectionListResAck;

        ie->value.choice.UE_associatedLogicalS1_ConnectionListResAck =
            CALLOC(1, sizeof(*list));
        list = ie->value.choice.UE_associatedLogicalS1_ConnectionListResAck;
        ogs_assert(list);

        for (i = 0; i < OGS_ASN_LIST_COUNT(partOfS1_Interface); i++) {
            S1AP_UE_associatedLogicalS1_ConnectionItemRes_t *ie1 = NULL;
            S1AP_UE_associatedLogicalS1_ConnectionItem_t *item1 = NULL;

            S1AP_UE_associatedLogicalS1_ConnectionItemResAck_t *ie2 = NULL;
            S1AP_UE_associatedLogicalS1_ConnectionItem_t *item2 = NULL;

            ie1 = OGS_ASN_LIST_GET(partOfS1_Interface, i);
            ogs_assert(ie1);

            item1 = ie1->value.choice.UE_associatedLogicalS1_ConnectionItem;
            ogs_assert(item1);

            if (item1->mME_UE_S1AP_ID == NULL &&
                item1->eNB_UE_S1AP_ID == NULL) {
                ogs_warn("No MME_UE_S1AP_ID & ENB_UE_S1AP_ID");
                continue;
            }

            ie2 = CALLOC(1,
                sizeof(*ie2));
            ogs_assert(ie2);
            ASN_SEQUENCE_ADD(list, ie2);

            ie2->id =
                S1AP_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionItem;
            ie2->criticality = S1AP_Criticality_ignore;
            ie2->value.present =
                S1AP_UE_associatedLogicalS1_ConnectionItemResAck__value_PR_UE_associatedLogicalS1_ConnectionItem;

            ie2->value.choice.UE_associatedLogicalS1_ConnectionItem =
                CALLOC(1, sizeof(*item2));
            item2 = ie2->value.choice.UE_associatedLogicalS1_ConnectionItem;
            ogs_assert(item2);

            if (item1->mME_UE_S1AP_ID) {
                item2->mME_UE_S1AP_ID =
                    CALLOC(1, sizeof(*item2->mME_UE_S1AP_ID));
                ogs_assert(item2->mME_UE_S1AP_ID);
                *item2->mME_UE_S1AP_ID = *item1->mME_UE_S1AP_ID;
            }

            if (item1->eNB_UE_S1AP_ID) {
                item2->eNB_UE_S1AP_ID =
                    CALLOC(1, sizeof(*item2->eNB_UE_S1AP_ID));
                ogs_assert(item2->eNB_UE_S1AP_ID);
                *item2->eNB_UE_S1AP_ID = *item1->eNB_UE_S1AP_ID;
            }

            ogs_debug("    MME_UE_S1AP_ID[%d] ENB_UE_S1AP_ID[%d]",
                item2->mME_UE_S1AP_ID ? (int)*item2->mME_UE_S1AP_ID : -1,
                item2->eNB_UE_S1AP_ID ? (int)*item2->eNB_UE_S1AP_ID : -1);
        }
    }

    return ogs_s1ap_encode(&pdu);
}
