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

#include "ngap-build.h"
#include "ogs-ngap.h"

ogs_pkbuf_t *ngap_build_warning_message_broadcast_request_transfer(
        pwsiws_warning_t *warning)
{
    ogs_pkbuf_t *pkbuf = NULL;
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_WriteReplaceWarningRequest_t *WriteReplaceWarningRequest = NULL;
    NGAP_WriteReplaceWarningRequestIEs_t *ie = NULL;
    NGAP_MessageIdentifier_t *MessageIdentifier = NULL;
    NGAP_SerialNumber_t *SerialNumber = NULL;
    NGAP_RepetitionPeriod_t *RepetitionPeriod = NULL;
    NGAP_NumberOfBroadcasts_t *NumberofBroadcastRequest = NULL;
    NGAP_DataCodingScheme_t *DataCodingScheme = NULL;
    NGAP_WarningMessageContents_t *WarningMessageContents = NULL;

    ogs_assert(warning);

    memset(&pdu, 0, sizeof(pdu));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));
    initiatingMessage = pdu.choice.initiatingMessage;

    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_WriteReplaceWarning;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present = NGAP_InitiatingMessage__value_PR_WriteReplaceWarningRequest;

    WriteReplaceWarningRequest = &initiatingMessage->value.choice.WriteReplaceWarningRequest;

    /* Message Identifier */
    ie = CALLOC(1, sizeof(NGAP_WriteReplaceWarningRequestIEs_t));
    ASN_SEQUENCE_ADD(&WriteReplaceWarningRequest->protocolIEs, ie);
    ie->id = NGAP_ProtocolIE_ID_id_MessageIdentifier;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_WriteReplaceWarningRequestIEs__value_PR_MessageIdentifier;

    MessageIdentifier = &ie->value.choice.MessageIdentifier;
    MessageIdentifier->size = (16 / 8);
    MessageIdentifier->buf = CALLOC(MessageIdentifier->size, sizeof(uint8_t));
    MessageIdentifier->bits_unused = 0;
    MessageIdentifier->buf[0] = (warning->message_id >> 8) & 0xFF;
    MessageIdentifier->buf[1] = warning->message_id & 0xFF;

    /* Serial Number */
    ie = CALLOC(1, sizeof(NGAP_WriteReplaceWarningRequestIEs_t));
    ASN_SEQUENCE_ADD(&WriteReplaceWarningRequest->protocolIEs, ie);
    ie->id = NGAP_ProtocolIE_ID_id_SerialNumber;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_WriteReplaceWarningRequestIEs__value_PR_SerialNumber;

    SerialNumber = &ie->value.choice.SerialNumber;
    SerialNumber->size = (16 / 8);
    SerialNumber->buf = CALLOC(SerialNumber->size, sizeof(uint8_t));
    SerialNumber->bits_unused = 0;
    SerialNumber->buf[0] = (warning->warning_data.serial_number >> 8) & 0xFF;
    SerialNumber->buf[1] = warning->warning_data.serial_number & 0xFF;

    /* TODO: optional Warning Area List */

    /* Repetition Period */
    ie = CALLOC(1, sizeof(NGAP_WriteReplaceWarningRequestIEs_t));
    ASN_SEQUENCE_ADD(&WriteReplaceWarningRequest->protocolIEs, ie);
    ie->id = NGAP_ProtocolIE_ID_id_RepetitionPeriod;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_WriteReplaceWarningRequestIEs__value_PR_RepetitionPeriod;

    RepetitionPeriod = &ie->value.choice.RepetitionPeriod;
    *RepetitionPeriod = warning->warning_data.repetition_period;

    /* Number of Broadcasts Requested */
    ie = CALLOC(1, sizeof(NGAP_WriteReplaceWarningRequestIEs_t));
    ASN_SEQUENCE_ADD(&WriteReplaceWarningRequest->protocolIEs, ie);
    ie->id = NGAP_ProtocolIE_ID_id_NumberOfBroadcastsRequested;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_WriteReplaceWarningRequestIEs__value_PR_NumberOfBroadcastsRequested;

    NumberofBroadcastRequest = &ie->value.choice.NumberOfBroadcastsRequested;
    *NumberofBroadcastRequest = warning->warning_data.number_of_broadcast;

    /* Data Coding Scheme */
    ie = CALLOC(1, sizeof(NGAP_WriteReplaceWarningRequestIEs_t));
    ASN_SEQUENCE_ADD(&WriteReplaceWarningRequest->protocolIEs, ie);
    ie->id = NGAP_ProtocolIE_ID_id_DataCodingScheme;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_WriteReplaceWarningRequestIEs__value_PR_DataCodingScheme;

    DataCodingScheme = &ie->value.choice.DataCodingScheme;
    DataCodingScheme->size = (8 / 8);
    DataCodingScheme->buf = CALLOC(DataCodingScheme->size, sizeof(uint8_t));
    DataCodingScheme->bits_unused = 0;
    DataCodingScheme->buf[0] = warning->warning_data.data_coding_scheme & 0xFF;

    /* Warning Message Contents */
    ie = CALLOC(1, sizeof(NGAP_WriteReplaceWarningRequestIEs_t));
    ASN_SEQUENCE_ADD(&WriteReplaceWarningRequest->protocolIEs, ie);
    ie->id = NGAP_ProtocolIE_ID_id_WarningMessageContents;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_WriteReplaceWarningRequestIEs__value_PR_WarningMessageContents;

    WarningMessageContents = &ie->value.choice.WarningMessageContents;
    WarningMessageContents->size = warning->warning_data.message_length;
    WarningMessageContents->buf = CALLOC(WarningMessageContents->size, sizeof(uint8_t));
    memcpy(WarningMessageContents->buf, warning->warning_data.message_contents, 
           WarningMessageContents->size);

    pkbuf = ogs_ngap_encode(&pdu);
    ogs_assert(pkbuf);

    return pkbuf;
}

ogs_pkbuf_t *ngap_build_warning_message_stop_request_transfer(
        pwsiws_warning_t *warning)
{
    ogs_pkbuf_t *pkbuf = NULL;
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PWSCancelRequest_t *NGAP_PWSCancelRequest = NULL;
    NGAP_PWSCancelRequestIEs_t *ie = NULL;
    NGAP_MessageIdentifier_t *MessageIdentifier = NULL;
    NGAP_SerialNumber_t *SerialNumber = NULL;

    ogs_assert(warning);

    memset(&pdu, 0, sizeof(pdu));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));
    initiatingMessage = pdu.choice.initiatingMessage;

    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_PWSCancel;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present = NGAP_InitiatingMessage__value_PR_PWSCancelRequest;

    NGAP_PWSCancelRequest = &initiatingMessage->value.choice.PWSCancelRequest;

    /* Message Identifier */
    ie = CALLOC(1, sizeof(NGAP_PWSCancelRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGAP_PWSCancelRequest->protocolIEs, ie);
    ie->id = NGAP_ProtocolIE_ID_id_MessageIdentifier;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PWSCancelRequestIEs__value_PR_MessageIdentifier;

    MessageIdentifier = &ie->value.choice.MessageIdentifier;
    MessageIdentifier->size = (16 / 8);
    MessageIdentifier->buf = CALLOC(MessageIdentifier->size, sizeof(uint8_t));
    MessageIdentifier->bits_unused = 0;
    MessageIdentifier->buf[0] = (warning->message_id >> 8) & 0xFF;
    MessageIdentifier->buf[1] = warning->message_id & 0xFF;

    /* Serial Number */
    ie = CALLOC(1, sizeof(NGAP_PWSCancelRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGAP_PWSCancelRequest->protocolIEs, ie);
    ie->id = NGAP_ProtocolIE_ID_id_SerialNumber;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PWSCancelRequestIEs__value_PR_SerialNumber;

    SerialNumber = &ie->value.choice.SerialNumber;
    SerialNumber->size = (16 / 8);
    SerialNumber->buf = CALLOC(SerialNumber->size, sizeof(uint8_t));
    SerialNumber->bits_unused = 0;
    SerialNumber->buf[0] = (warning->warning_data.serial_number >> 8) & 0xFF;
    SerialNumber->buf[1] = warning->warning_data.serial_number & 0xFF;

    /* TODO: optional Warning Area List */

    pkbuf = ogs_ngap_encode(&pdu);
    ogs_assert(pkbuf);

    return pkbuf;
}

ogs_pkbuf_t *ngap_build_warning_message_cancel_request_transfer(
        pwsiws_warning_t *warning)
{
    /* For now, use the same as stop request */
    return ngap_build_warning_message_stop_request_transfer(warning);
} 