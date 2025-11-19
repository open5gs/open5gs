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

#include "ogs-ngap.h"

ogs_pkbuf_t *ogs_ngap_build_error_indication(
        uint64_t *ran_ue_ngap_id,
        uint64_t *amf_ue_ngap_id,
        NGAP_Cause_PR group, long cause)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_ErrorIndication_t *ErrorIndication = NULL;

    NGAP_ErrorIndicationIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_debug("Error Indication");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_ErrorIndication;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_ErrorIndication;

    ErrorIndication = &initiatingMessage->value.choice.ErrorIndication;

    if (amf_ue_ngap_id) {
        ie = CALLOC(1, sizeof(NGAP_ErrorIndicationIEs_t));
        ASN_SEQUENCE_ADD(&ErrorIndication->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_ErrorIndicationIEs__value_PR_AMF_UE_NGAP_ID;

        AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

        asn_uint642INTEGER(AMF_UE_NGAP_ID, *amf_ue_ngap_id);
        ogs_debug("    AMF_UE_NGAP_ID[%lld]", (long long)*amf_ue_ngap_id);
    }

    if (ran_ue_ngap_id) {
        ie = CALLOC(1, sizeof(NGAP_ErrorIndicationIEs_t));
        ASN_SEQUENCE_ADD(&ErrorIndication->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_ErrorIndicationIEs__value_PR_RAN_UE_NGAP_ID;

        RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

        *RAN_UE_NGAP_ID = *ran_ue_ngap_id;
        ogs_debug("    RAN_UE_NGAP_ID[%lld]", (long long)*ran_ue_ngap_id);
    }

    ie = CALLOC(1, sizeof(NGAP_ErrorIndicationIEs_t));
    ASN_SEQUENCE_ADD(&ErrorIndication->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_ErrorIndicationIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    ogs_debug("    Group[%d] Cause[%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ogs_ngap_build_ng_reset(
    NGAP_Cause_PR group, long cause,
    NGAP_UE_associatedLogicalNG_connectionList_t *partOfNG_Interface)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_NGReset_t *NGReset = NULL;

    NGAP_NGResetIEs_t *ie = NULL;
    NGAP_Cause_t *Cause = NULL;
    NGAP_ResetType_t *ResetType = NULL;

    ogs_debug("NGReset");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_NGReset;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present = NGAP_InitiatingMessage__value_PR_NGReset;

    NGReset = &initiatingMessage->value.choice.NGReset;

    ie = CALLOC(1, sizeof(NGAP_NGResetIEs_t));
    ASN_SEQUENCE_ADD(&NGReset->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_NGResetIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    ie = CALLOC(1, sizeof(NGAP_NGResetIEs_t));
    ASN_SEQUENCE_ADD(&NGReset->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_ResetType;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGResetIEs__value_PR_ResetType;

    ResetType = &ie->value.choice.ResetType;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    ogs_debug("    Group[%d] Cause[%d] partOfNG_Interface[%p]",
        Cause->present, (int)Cause->choice.radioNetwork, partOfNG_Interface);

    if (partOfNG_Interface) {
        ResetType->present = NGAP_ResetType_PR_partOfNG_Interface;
        ResetType->choice.partOfNG_Interface = partOfNG_Interface;
    } else {
        ResetType->present = NGAP_ResetType_PR_nG_Interface;
        ResetType->choice.nG_Interface = NGAP_ResetAll_reset_all;
    }

    return ogs_ngap_encode(&pdu);
}

void ogs_ngap_build_part_of_ng_interface(
    NGAP_UE_associatedLogicalNG_connectionList_t *partOfNG_Interface,
    uint64_t *ran_ue_ngap_id, uint64_t *amf_ue_ngap_id)
{
    NGAP_UE_associatedLogicalNG_connectionItem_t *item = NULL;

    ogs_assert(partOfNG_Interface);
    ogs_assert(ran_ue_ngap_id || amf_ue_ngap_id);

    item = CALLOC(1, sizeof(*item));
    ogs_assert(item);

    if (amf_ue_ngap_id) {
        item->aMF_UE_NGAP_ID = CALLOC(1, sizeof(*item->aMF_UE_NGAP_ID));
        ogs_assert(item->aMF_UE_NGAP_ID);

        asn_uint642INTEGER(item->aMF_UE_NGAP_ID, *amf_ue_ngap_id);
    }

    if (ran_ue_ngap_id) {
        item->rAN_UE_NGAP_ID = CALLOC(1, sizeof(*item->rAN_UE_NGAP_ID));
        ogs_assert(item->rAN_UE_NGAP_ID);

        *item->rAN_UE_NGAP_ID = *ran_ue_ngap_id;
    }

    ASN_SEQUENCE_ADD(&partOfNG_Interface->list, item);
}

ogs_pkbuf_t *ogs_ngap_build_ng_reset_ack(
        NGAP_UE_associatedLogicalNG_connectionList_t *partOfNG_Interface)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_NGResetAcknowledge_t *NGResetAcknowledge = NULL;

    NGAP_NGResetAcknowledgeIEs_t *ie = NULL;

    ogs_debug("Reset acknowledge");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = NGAP_ProcedureCode_id_NGReset;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_NGResetAcknowledge;

    NGResetAcknowledge = &successfulOutcome->value.choice.NGResetAcknowledge;
    ogs_assert(NGResetAcknowledge);

    if (partOfNG_Interface && partOfNG_Interface->list.count) {
        int i = 0;
        NGAP_UE_associatedLogicalNG_connectionList_t *list = NULL;

        ie = CALLOC(1, sizeof(NGAP_NGResetAcknowledgeIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(&NGResetAcknowledge->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UE_associatedLogicalNG_connectionList;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_NGResetAcknowledgeIEs__value_PR_UE_associatedLogicalNG_connectionList;

        list = &ie->value.choice.UE_associatedLogicalNG_connectionList;

        for (i = 0; i < partOfNG_Interface->list.count; i++) {
            NGAP_UE_associatedLogicalNG_connectionItem_t *item = NULL;
            uint64_t amf_ue_ngap_id = 0;
            uint64_t ran_ue_ngap_id = 0;

            item = partOfNG_Interface->list.array[i];
            ogs_assert(item);

            if (item->aMF_UE_NGAP_ID == NULL &&
                item->rAN_UE_NGAP_ID == NULL) {
                ogs_warn("No AMF_UE_NGAP_ID and RAN_UE_NGAP_ID");
                continue;
            }

            if (item->aMF_UE_NGAP_ID)
                asn_INTEGER2uint64(item->aMF_UE_NGAP_ID, &amf_ue_ngap_id);

            if (item->rAN_UE_NGAP_ID)
                ran_ue_ngap_id = *item->rAN_UE_NGAP_ID;

            ogs_ngap_build_part_of_ng_interface(
                list,
                item->rAN_UE_NGAP_ID ? &ran_ue_ngap_id : NULL,
                item->aMF_UE_NGAP_ID ? &amf_ue_ngap_id : NULL);

            ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
                    item->rAN_UE_NGAP_ID ? (long long)ran_ue_ngap_id : -1,
                    item->aMF_UE_NGAP_ID ? (long long)amf_ue_ngap_id : -1);
        }
    }

    return ogs_ngap_encode(&pdu);
}
