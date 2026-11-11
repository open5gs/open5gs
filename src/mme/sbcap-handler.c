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

#include "mme-event.h"
#include "mme-timer.h"

#include "sbcap-path.h"
#include "mme-fd-path.h"

//#include "s1ap-build.h"
#include "sbc-handler.h"

#include "stdio.h"

#include "sbcap-handler.h"

#include "mme-path.h"
#include "mme-sm.h"




/* Target area requested by the CBC (List-of-TAIs, TS 29.168) */
static void sbcap_parse_list_of_tais(
        const SBCAP_List_of_TAIs_t *list, sbc_pws_data_t *sbc_pws)
{
    int k;
    for (k = 0; k < list->list.count &&
            sbc_pws->no_of_tai < OGS_ARRAY_SIZE(sbc_pws->tai); k++) {
        SBCAP_TAI_t *tai = &list->list.array[k]->tai;
        ogs_eps_tai_t *dst = &sbc_pws->tai[sbc_pws->no_of_tai];
        uint16_t tac = 0;

        memcpy(&dst->plmn_id, tai->pLMNidentity.buf, sizeof(ogs_plmn_id_t));
        ogs_asn_OCTET_STRING_to_uint16(&tai->tAC, &tac);
        dst->tac = tac;
        ogs_info("    target TAC[%d]", dst->tac);
        sbc_pws->no_of_tai++;
    }
}

/* No List-of-TAIs: every TA served by a connected eNB.
 * Union of all eNB TAIs, without duplicates (each eNB used to overwrite the
 * previous one, so only the last connected eNB received the warning). */
static void sbcap_fill_tais_from_enbs(sbc_pws_data_t *sbc_pws)
{
    mme_enb_t *enb = NULL;
    int i, k;

    ogs_list_for_each(&mme_self()->enb_list, enb) {
        for (i = 0; i < enb->num_of_supported_ta_list; i++) {
            int dup = 0;
            for (k = 0; k < sbc_pws->no_of_tai; k++) {
                if (!memcmp(&sbc_pws->tai[k], &enb->supported_ta_list[i],
                            sizeof(ogs_eps_tai_t))) {
                    dup = 1;
                    break;
                }
            }
            if (dup) continue;
            if (sbc_pws->no_of_tai >= OGS_ARRAY_SIZE(sbc_pws->tai)) {
                ogs_warn("SBc-AP: more than %d TAs, extra TAs ignored",
                        (int)OGS_ARRAY_SIZE(sbc_pws->tai));
                return;
            }
            sbc_pws->tai[sbc_pws->no_of_tai++] = enb->supported_ta_list[i];
        }
    }
}

static uint16_t sbcap_bitstring16(const BIT_STRING_t *bs)
{
    /* BIT STRING (16), network byte order (TS 23.041 9.4.1.2) */
    return (bs && bs->size >= 2) ? (uint16_t)((bs->buf[0] << 8) | bs->buf[1]) : 0;
}

/*
 * Build and send SBc-AP Stop-Warning-Response (TS 29.168 4.3.4.2)
 *   Message Identifier / Serial Number echoed, Cause.
 */
static int sbcap_send_stop_warning_response(mme_sbcap_t *sbc,
        const BIT_STRING_t *message_identifier,
        const BIT_STRING_t *serial_number, SBCAP_Cause_t cause)
{
    ogs_sbcap_message_t pdu;
    SBCAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    SBCAP_Stop_Warning_Response_t *Response = NULL;
    SBCAP_Stop_Warning_Response_IEs_t *ie = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    memset(&pdu, 0, sizeof(pdu));
    pdu.present = SBCAP_SBC_AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(SBCAP_SuccessfulOutcome_t));
    ogs_assert(pdu.choice.successfulOutcome);

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = SBCAP_id_Stop_Warning;
    successfulOutcome->criticality = SBCAP_Criticality_reject;
    successfulOutcome->value.present =
        SBCAP_SuccessfulOutcome__value_PR_Stop_Warning_Response;
    Response = &successfulOutcome->value.choice.Stop_Warning_Response;

    ie = CALLOC(1, sizeof(SBCAP_Stop_Warning_Response_IEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&Response->protocolIEs, ie);
    ie->id = SBCAP_id_Message_Identifier;
    ie->criticality = SBCAP_Criticality_reject;
    ie->value.present = SBCAP_Stop_Warning_Response_IEs__value_PR_Message_Identifier;
    ie->value.choice.Message_Identifier.size = message_identifier->size;
    ie->value.choice.Message_Identifier.bits_unused = message_identifier->bits_unused;
    ie->value.choice.Message_Identifier.buf = CALLOC(message_identifier->size, 1);
    ogs_assert(ie->value.choice.Message_Identifier.buf);
    memcpy(ie->value.choice.Message_Identifier.buf,
            message_identifier->buf, message_identifier->size);

    ie = CALLOC(1, sizeof(SBCAP_Stop_Warning_Response_IEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&Response->protocolIEs, ie);
    ie->id = SBCAP_id_Serial_Number;
    ie->criticality = SBCAP_Criticality_reject;
    ie->value.present = SBCAP_Stop_Warning_Response_IEs__value_PR_Serial_Number;
    ie->value.choice.Serial_Number.size = serial_number->size;
    ie->value.choice.Serial_Number.bits_unused = serial_number->bits_unused;
    ie->value.choice.Serial_Number.buf = CALLOC(serial_number->size, 1);
    ogs_assert(ie->value.choice.Serial_Number.buf);
    memcpy(ie->value.choice.Serial_Number.buf,
            serial_number->buf, serial_number->size);

    ie = CALLOC(1, sizeof(SBCAP_Stop_Warning_Response_IEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&Response->protocolIEs, ie);
    ie->id = SBCAP_id_Cause;
    ie->criticality = SBCAP_Criticality_reject;
    ie->value.present = SBCAP_Stop_Warning_Response_IEs__value_PR_Cause;
    ie->value.choice.Cause = cause;

    /* ogs_sbcap_encode() frees the PDU contents (see ogs_asn_encode) */
    pkbuf = ogs_sbcap_encode(&pdu);
    if (!pkbuf) {
        ogs_error("Stop-Warning-Response encode failed");
        return OGS_ERROR;
    }

    ogs_info("[SBc-AP] Tx Stop-Warning-Response (cause=%ld)", (long)cause);
    return sbcap_send_to_sbc(sbc, pkbuf, 0);
}

void sbcap_handle_stop_warning_request(
        mme_sbcap_t *sbc, ogs_sbcap_message_t *message)
{
    int i;
    bool target_area = false;
    SBCAP_InitiatingMessage_t *initiatingMessage = NULL;
    SBCAP_Stop_Warning_Request_t *Request = NULL;
    SBCAP_Message_Identifier_t *message_identifier = NULL;
    SBCAP_Serial_Number_t *serial_number = NULL;
    sbc_pws_data_t *sbc_pws = NULL;

    ogs_assert(sbc);
    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    Request = &initiatingMessage->value.choice.Stop_Warning_Request;

    sbc_pws = ogs_calloc(1, sizeof(sbc_pws_data_t));
    ogs_assert(sbc_pws);

    for (i = 0; i < Request->protocolIEs.list.count; i++) {
        SBCAP_Stop_Warning_Request_IEs_t *ie = Request->protocolIEs.list.array[i];
        switch (ie->id) {
        case SBCAP_id_Message_Identifier:
            message_identifier = &ie->value.choice.Message_Identifier;
            break;
        case SBCAP_id_Serial_Number:
            serial_number = &ie->value.choice.Serial_Number;
            break;
        case SBCAP_id_List_of_TAIs:
            sbcap_parse_list_of_tais(&ie->value.choice.List_of_TAIs, sbc_pws);
            target_area = true;
            break;
        default:
            break;
        }
    }

    if (!message_identifier || !serial_number) {
        ogs_error("Stop-Warning-Request without Message-Identifier or Serial-Number");
        ogs_free(sbc_pws);
        return;
    }

    sbc_pws->message_id = sbcap_bitstring16(message_identifier);
    sbc_pws->serial_number = sbcap_bitstring16(serial_number);
    if (!target_area)
        sbcap_fill_tais_from_enbs(sbc_pws);

    ogs_info("[SBc-AP] Stop-Warning msg_id[%d] serial[0x%04x] for %d TA(s)",
            sbc_pws->message_id, sbc_pws->serial_number, sbc_pws->no_of_tai);

    /* Forward to the eNBs (S1AP Kill Request, TS 36.413 8.12.2) */
    sbc_handle_stop_warning_request(sbc_pws);

    /* Acknowledge to the CBC */
    sbcap_send_stop_warning_response(sbc, message_identifier, serial_number,
            (sbc_pws->no_of_tai > 0) ? SBCAP_Cause_message_accepted
                                     : SBCAP_Cause_tracking_area_not_valid);
    ogs_free(sbc_pws);
}

/*
 * Build and send SBc-AP Write-Replace-Warning-Response (TS 29.168 4.3.4.1.2)
 *   Message Identifier  (M, reject) - echoed from the request
 *   Serial Number       (M, reject) - echoed from the request
 *   Cause               (M, reject)
 * Optional IEs (Unknown TAI list, Criticality Diagnostics) are omitted.
 */
static int sbcap_send_write_replace_warning_response(mme_sbcap_t *sbc,
        const BIT_STRING_t *message_identifier,
        const BIT_STRING_t *serial_number, SBCAP_Cause_t cause)
{
    ogs_sbcap_message_t pdu;
    SBCAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    SBCAP_Write_Replace_Warning_Response_t *Response = NULL;
    SBCAP_Write_Replace_Warning_Response_IEs_t *ie = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sbc);
    ogs_assert(message_identifier);
    ogs_assert(serial_number);

    memset(&pdu, 0, sizeof(pdu));
    pdu.present = SBCAP_SBC_AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(SBCAP_SuccessfulOutcome_t));
    ogs_assert(pdu.choice.successfulOutcome);

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = SBCAP_id_Write_Replace_Warning;
    successfulOutcome->criticality = SBCAP_Criticality_reject;
    successfulOutcome->value.present =
        SBCAP_SuccessfulOutcome__value_PR_Write_Replace_Warning_Response;
    Response = &successfulOutcome->value.choice.Write_Replace_Warning_Response;

    /* Message Identifier */
    ie = CALLOC(1, sizeof(SBCAP_Write_Replace_Warning_Response_IEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&Response->protocolIEs, ie);
    ie->id = SBCAP_id_Message_Identifier;
    ie->criticality = SBCAP_Criticality_reject;
    ie->value.present =
        SBCAP_Write_Replace_Warning_Response_IEs__value_PR_Message_Identifier;
    ie->value.choice.Message_Identifier.size = message_identifier->size;
    ie->value.choice.Message_Identifier.bits_unused =
        message_identifier->bits_unused;
    ie->value.choice.Message_Identifier.buf =
        CALLOC(message_identifier->size, sizeof(uint8_t));
    ogs_assert(ie->value.choice.Message_Identifier.buf);
    memcpy(ie->value.choice.Message_Identifier.buf,
            message_identifier->buf, message_identifier->size);

    /* Serial Number */
    ie = CALLOC(1, sizeof(SBCAP_Write_Replace_Warning_Response_IEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&Response->protocolIEs, ie);
    ie->id = SBCAP_id_Serial_Number;
    ie->criticality = SBCAP_Criticality_reject;
    ie->value.present =
        SBCAP_Write_Replace_Warning_Response_IEs__value_PR_Serial_Number;
    ie->value.choice.Serial_Number.size = serial_number->size;
    ie->value.choice.Serial_Number.bits_unused = serial_number->bits_unused;
    ie->value.choice.Serial_Number.buf =
        CALLOC(serial_number->size, sizeof(uint8_t));
    ogs_assert(ie->value.choice.Serial_Number.buf);
    memcpy(ie->value.choice.Serial_Number.buf,
            serial_number->buf, serial_number->size);

    /* Cause */
    ie = CALLOC(1, sizeof(SBCAP_Write_Replace_Warning_Response_IEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&Response->protocolIEs, ie);
    ie->id = SBCAP_id_Cause;
    ie->criticality = SBCAP_Criticality_reject;
    ie->value.present = SBCAP_Write_Replace_Warning_Response_IEs__value_PR_Cause;
    ie->value.choice.Cause = cause;

    /* ogs_sbcap_encode() frees the PDU contents (see ogs_asn_encode) */
    pkbuf = ogs_sbcap_encode(&pdu);
    if (!pkbuf) {
        ogs_error("Write-Replace-Warning-Response encode failed");
        return OGS_ERROR;
    }

    ogs_info("[SBc-AP] Tx Write-Replace-Warning-Response (cause=%ld)",
            (long)cause);
    return sbcap_send_to_sbc(sbc, pkbuf, 0);
}

void sbcap_handle_write_replace_warning_response(
        mme_sbcap_t *sbc, ogs_sbcap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];

    SBCAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    SBCAP_Write_Replace_Warning_Response_t *SbcapWriteReplaceWarningResponse = NULL;

    ogs_assert(sbc);
    ogs_assert(sbc->sctp.sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    SbcapWriteReplaceWarningResponse =
        &successfulOutcome->value.choice.Write_Replace_Warning_Response;
    ogs_assert(SbcapWriteReplaceWarningResponse);

    ogs_debug("SbcapWriteReplaceWarningResponse");

    ogs_debug("    IP[%s]",
            OGS_ADDR(sbc->sctp.addr, buf));

}

void sbcap_handle_write_replace_warning_request(
        mme_sbcap_t *sbc, ogs_sbcap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    SBCAP_InitiatingMessage_t *initiatingMessage = NULL;
    SBCAP_Write_Replace_Warning_Request_t *SbcapWriteReplaceWarningRequest = NULL;
    SBCAP_Write_Replace_Warning_Request_IEs_t *ie = NULL;
    SBCAP_Message_Identifier_t *message_identifier = NULL;
    SBCAP_Serial_Number_t *serial_number                      = NULL;
    SBCAP_Repetition_Period_t *repetition_period    = NULL;
    SBCAP_NumberOfBroadcasts_t *broadcast_requested    = NULL;
    SBCAP_Data_Coding_Scheme_t *data_coding_scheme    = NULL;
    SBCAP_Warning_Message_Content_t *warning_message_content    = NULL;
    //sbc_handle_write_replace_warning_request(sbc_pws_data_t *sbc_pws)
    sbc_pws_data_t *sbc_pws = NULL; 
    //ogs_debug("Test3");
    //memset(sbc_pws, 0, sizeof(sbc_pws_data_t)); 
    bool target_area = false;
    sbc_pws = ogs_calloc(1, sizeof(sbc_pws_data_t));
    //ogs_debug("Test1");
    ogs_assert(sbc_pws);
    //ogs_debug("Test2");


    ogs_assert(sbc);
    ogs_assert(sbc->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    SbcapWriteReplaceWarningRequest =
        &initiatingMessage->value.choice.Write_Replace_Warning_Request;
    ogs_assert(SbcapWriteReplaceWarningRequest);

    ogs_debug("SbcapWriteReplaceWarningRequest");

    for (i=0; i < SbcapWriteReplaceWarningRequest->protocolIEs.list.count; i++ ){
            ie = SbcapWriteReplaceWarningRequest->protocolIEs.list.array[i];
            switch(ie->id){
                case SBCAP_ProtocolIE_ID_id_Message_Identifier :
                   message_identifier = &ie->value.choice.Message_Identifier;
                   /* BIT STRING (16), network byte order (TS 23.041 9.4.1.2.2) */
                   int message = (message_identifier->buf[0] << 8) |
                                 message_identifier->buf[1];

                   ogs_debug("message_identifier: %ld",message_identifier->size);
                   ogs_info("message_identifier: %d",message);
                   sbc_pws ->message_id = message;
                   ogs_log_hexdump(OGS_LOG_INFO,message_identifier->buf, sizeof(message_identifier->buf));

                break;
                case SBCAP_ProtocolIE_ID_id_Serial_Number :
                   serial_number = &ie->value.choice.Serial_Number;
                   ogs_debug("serial_number: %s",serial_number->buf);
                   /* BIT STRING (16), network byte order (TS 23.041 9.4.1.2.1) */
                   uint16_t serial_number_i = (serial_number->buf[0] << 8) |
                                              serial_number->buf[1];
                   sbc_pws ->serial_number = serial_number_i;

                break;
                case SBCAP_id_List_of_TAIs :
                   sbcap_parse_list_of_tais(&ie->value.choice.List_of_TAIs, sbc_pws);
                   target_area = true;
                break;
                case SBCAP_ProtocolIE_ID_id_Repetition_Period :
                   repetition_period = &ie->value.choice.Repetition_Period;
                   ogs_info("repetition_period: %ld",*repetition_period);
                   sbc_pws -> repetition_period = *repetition_period;

                break;
                case SBCAP_ProtocolIE_ID_id_Number_Of_Broadcast_Requested :
                   broadcast_requested = &ie->value.choice.Number_of_Broadcasts_Requested;
                   ogs_info("broadcast_requested: %ld",*broadcast_requested);
                   sbc_pws -> number_of_broadcast = *broadcast_requested;
                break;
                case SBCAP_ProtocolIE_ID_id_Data_Coding_Scheme :
                   data_coding_scheme = &ie->value.choice.Data_Coding_Scheme;
                   /* BIT STRING (8), TS 23.038 CBS Data Coding Scheme: pass it through */
                   sbc_pws->data_coding_scheme = data_coding_scheme->size ?
                       data_coding_scheme->buf[0] : 0x0F;
                   ogs_info("data_coding_scheme: 0x%02x",
                           sbc_pws->data_coding_scheme);
                break;
               case SBCAP_ProtocolIE_ID_id_Warning_Message_Content :
                   warning_message_content = &ie->value.choice.Warning_Message_Content;
                   ogs_info("warning_message_content: %s",warning_message_content->buf);
                   ogs_log_hexdump(OGS_LOG_INFO,warning_message_content->buf, sizeof(warning_message_content->buf));
                   uint8_t warn_message[1024];
                   memcpy(&warn_message,warning_message_content->buf, warning_message_content->size);
                   memcpy(sbc_pws->message_contents,warning_message_content->buf, warning_message_content->size);                   
                   sbc_pws -> message_length = warning_message_content->size;
                   //sbc_pws -> no_of_tai = 1;
                   //sbc_pws -> tai[0] = 0x0007;
                break;
            }
        
    }

    if (!target_area)
        sbcap_fill_tais_from_enbs(sbc_pws);
    ogs_info("SBc-AP: warning for %d TA(s)%s", sbc_pws->no_of_tai,
            target_area ? " (List-of-TAIs from CBC)" : " (all eNBs)");

    ogs_debug("    IP[%s]",
            OGS_ADDR(sbc->sctp.addr, buf));

    if (!message_identifier || !serial_number) {
        /* Mandatory IEs missing: response cannot be correlated by the CBC */
        ogs_error("Write-Replace-Warning-Request without Message-Identifier "
                "or Serial-Number");
        ogs_free(sbc_pws);
        return;
    }

    /* Forward to the eNBs (S1AP Write-Replace-Warning-Request) */
    sbc_handle_write_replace_warning_request(sbc_pws);

    /* Acknowledge to the CBC (TS 29.168 4.3.4.1.2) */
    sbcap_send_write_replace_warning_response(sbc,
            message_identifier, serial_number,
            (sbc_pws->no_of_tai > 0) ? SBCAP_Cause_message_accepted
                                     : SBCAP_Cause_tracking_area_not_valid);

    ogs_free(sbc_pws);

}

