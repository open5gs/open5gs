/*
 * Copyright (c) 2015, EURECOM (www.eurecom.fr)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the FreeBSD Project.
 */

#define TRACE_MODULE _decoder

#include "core_debug.h"
#include "s1ap_message.h"


static int s1ap_decode_initiating(s1ap_message *message,
    S1ap_InitiatingMessage_t *initiating_p);
static int s1ap_decode_successfull_outcome(s1ap_message *message,
    S1ap_SuccessfulOutcome_t *successfullOutcome_p);
static int s1ap_decode_unsuccessfull_outcome(s1ap_message *message,
    S1ap_UnsuccessfulOutcome_t *unSuccessfulOutcome_p);

static void s1ap_xer_print_message(
    asn_enc_rval_t (*func)(asn_app_consume_bytes_f *cb,
    void *app_key, s1ap_message *message_p), 
    asn_app_consume_bytes_f *cb, s1ap_message *message_p);

int s1ap_decode_pdu(s1ap_message *message, pkbuf_t *pkb)
{
    S1AP_PDU_t pdu = {0};
    S1AP_PDU_t *pdu_p = &pdu;
    asn_dec_rval_t dec_ret = {0};

    d_assert(pkb, return -1, "Null param");
    d_assert(pkb->payload, return -1, "Null param");
    memset((void *)pdu_p, 0, sizeof(S1AP_PDU_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1AP_PDU, (void **)&pdu_p, 
            pkb->payload, pkb->len, 0, 0);

    if (dec_ret.code != RC_OK) 
    {
        d_error("Failed to decode PDU");
        return -1;
    }

    memset(message, 0, sizeof(s1ap_message));

    message->direction = pdu_p->present;
    switch (pdu_p->present) 
    {
        case S1AP_PDU_PR_initiatingMessage:
            return s1ap_decode_initiating(message, 
                    &pdu_p->choice.initiatingMessage);

        case S1AP_PDU_PR_successfulOutcome:
            return s1ap_decode_successfull_outcome(message, 
                    &pdu_p->choice.successfulOutcome);

        case S1AP_PDU_PR_unsuccessfulOutcome:
            return s1ap_decode_unsuccessfull_outcome(message, 
                    &pdu_p->choice.unsuccessfulOutcome);

        default:
            d_error("Unknown message outcome (%d) or not implemented", 
                    (int)pdu_p->present);
            break;
    }

    return -1;
}


static int s1ap_decode_initiating(s1ap_message *message,
    S1ap_InitiatingMessage_t *initiating_p)
{
    int ret = -1;

    d_assert(initiating_p, return -1, "Null param");

    switch (initiating_p->procedureCode) 
    {
        case S1ap_ProcedureCode_id_uplinkNASTransport: 
            ret = s1ap_decode_s1ap_uplinknastransport_ies(
                    &message->msg.s1ap_UplinkNASTransport_IEs, 
                    &initiating_p->value);
            s1ap_xer_print_message(s1ap_xer_print_s1ap_uplinknastransport, 
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_S1Setup: 
            ret = s1ap_decode_s1ap_s1setuprequesties(
                    &message->msg.s1ap_S1SetupRequestIEs, 
                    &initiating_p->value);
            s1ap_xer_print_message(s1ap_xer_print_s1ap_s1setuprequest,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_initialUEMessage: 
            ret = s1ap_decode_s1ap_initialuemessage_ies(
                    &message->msg.s1ap_InitialUEMessage_IEs, 
                    &initiating_p->value);
            s1ap_xer_print_message(s1ap_xer_print_s1ap_initialuemessage,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_UEContextReleaseRequest: 
            ret = s1ap_decode_s1ap_uecontextreleaserequest_ies(
                    &message->msg.s1ap_UEContextReleaseRequest_IEs, &initiating_p->value);
            s1ap_xer_print_message(s1ap_xer_print_s1ap_uecontextreleaserequest,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_UECapabilityInfoIndication: 
            ret = s1ap_decode_s1ap_uecapabilityinfoindicationies(
                    &message->msg.s1ap_UECapabilityInfoIndicationIEs, 
                    &initiating_p->value);
            s1ap_xer_print_message(
                    s1ap_xer_print_s1ap_uecapabilityinfoindication,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_NASNonDeliveryIndication: 
            ret = s1ap_decode_s1ap_nasnondeliveryindication_ies(
                    &message->msg.s1ap_NASNonDeliveryIndication_IEs, 
                    &initiating_p->value);
            s1ap_xer_print_message(s1ap_xer_print_s1ap_nasnondeliveryindication,
                    s1ap_xer__print2sp, message);
            break;

        default: 
            d_error("Unknown procedure ID (%d) for initiating message", 
                    (int)initiating_p->procedureCode);
            d_assert(0, return -1, 
                    "Unknown procedure ID (%d) for initiating message", 
                    (int)initiating_p->procedureCode);
            break;
    }

    return ret;
}

static int s1ap_decode_successfull_outcome(s1ap_message *message,
    S1ap_SuccessfulOutcome_t *successfullOutcome_p) 
{
    int ret = -1;

    d_assert(successfullOutcome_p, return -1, "Null param");

    switch (successfullOutcome_p->procedureCode) 
    {
        case S1ap_ProcedureCode_id_InitialContextSetup: 
            ret = s1ap_decode_s1ap_initialcontextsetupresponseies(
                    &message->msg.s1ap_InitialContextSetupResponseIEs, 
                    &successfullOutcome_p->value);
            s1ap_xer_print_message(
                    s1ap_xer_print_s1ap_initialcontextsetupresponse,
                    s1ap_xer__print2sp, message);
            break;

        case S1ap_ProcedureCode_id_UEContextRelease: 
            ret = s1ap_decode_s1ap_uecontextreleasecomplete_ies(
                    &message->msg.s1ap_UEContextReleaseComplete_IEs, 
                    &successfullOutcome_p->value);
            s1ap_xer_print_message(
                    s1ap_xer_print_s1ap_uecontextreleasecomplete,
                    s1ap_xer__print2sp, message);
            break;

        default: 
            d_error("Unknown procedure ID (%ld) for successfull "
                    "outcome message", successfullOutcome_p->procedureCode);
            break;
    }

    return ret;
}

static int s1ap_decode_unsuccessfull_outcome(s1ap_message *message,
    S1ap_UnsuccessfulOutcome_t *unSuccessfulOutcome_p) 
{
    int ret = -1;

    d_assert(unSuccessfulOutcome_p, return -1, "Null param");

    switch (unSuccessfulOutcome_p->procedureCode) 
    {
        case S1ap_ProcedureCode_id_InitialContextSetup: 
            ret = s1ap_decode_s1ap_initialcontextsetupfailureies(
                    &message->msg.s1ap_InitialContextSetupFailureIEs, 
                    &unSuccessfulOutcome_p->value);
            s1ap_xer_print_message(
                    s1ap_xer_print_s1ap_initialcontextsetupfailure,
                    s1ap_xer__print2sp, message);
            break;

        default: 
            d_error("Unknown procedure ID (%d) for "
                    "unsuccessfull outcome message", 
                    (int)unSuccessfulOutcome_p->procedureCode);
            break;
    }

    return ret;
}

#define S1AP_MAX_MESSAGE_STRING 2048
static char s1ap_message_string[S1AP_MAX_MESSAGE_STRING];

static void s1ap_xer_print_message(
    asn_enc_rval_t (*func)(asn_app_consume_bytes_f *cb,
    void *app_key, s1ap_message *message_p), 
    asn_app_consume_bytes_f *cb, s1ap_message *message_p)
{
    if (g_trace_mask && TRACE_MODULE >= 3) 
    {
        s1ap_string_total_size = 0;
        memset(s1ap_message_string, 0, S1AP_MAX_MESSAGE_STRING);

        func(cb, s1ap_message_string, message_p);

        printf("%s\n", s1ap_message_string);
    }
}

