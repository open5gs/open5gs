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

/*******************************************************************************
 * This file had been created by asn1tostruct.py script v1.0.2
 * Please do not modify this file but regenerate it via script.
 * Created on: 2017-02-04 23:11:39.444144 by acetcom
 * from ['S1AP-PDU.asn']
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include <asn_application.h>
#include <asn_internal.h>

#include "s1ap_ies_defs.h"

size_t s1ap_string_total_size = 0;

int
s1ap_xer__print2fp(const void *buffer, size_t size, void *app_key) {
    FILE *stream = (FILE *)app_key;

    if(fwrite(buffer, 1, size, stream) != size)
        return -1;

    return 0;
}

int s1ap_xer__print2sp(const void *buffer, size_t size, void *app_key) {
    char *string = (char *)app_key;

    /* Copy buffer to the formatted string */
    memcpy(&string[s1ap_string_total_size], buffer, size);

    s1ap_string_total_size += size;

    return 0;
}

static asn_enc_rval_t
xer_encode_local(asn_TYPE_descriptor_t *td, void *sptr,
        asn_app_consume_bytes_f *cb, void *app_key, int indent) {
    asn_enc_rval_t er, tmper;
    const char *mname;
    size_t mlen;
    int xcan = 2;

    if(!td || !sptr) goto cb_failed;

    mname = td->xml_tag;
    mlen = strlen(mname);

    ASN__TEXT_INDENT(0, indent);
    ASN__CALLBACK3("<", 1, mname, mlen, ">", 1);

    tmper = td->xer_encoder(td, sptr, indent + 1, XER_F_BASIC, cb, app_key);
    if(tmper.encoded == -1) return tmper;

    ASN__CALLBACK3("</", 2, mname, mlen, ">\n", xcan);

    er.encoded = 4 + xcan + (2 * mlen) + tmper.encoded;

    ASN__ENCODED_OK(er);
cb_failed:
    ASN__ENCODE_FAILED;
}
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabfailedtosetuplisthoreqack(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABFailedtoSetupListHOReqAck_IEs_t *s1ap_E_RABFailedtoSetupListHOReqAckIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABFailedtoSetupListHOReqAckIEs->s1ap_E_RABFailedtoSetupItemHOReqAck.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABFailedToSetupItemHOReqAck, s1ap_E_RABFailedtoSetupListHOReqAckIEs->s1ap_E_RABFailedtoSetupItemHOReqAck.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_deactivatetrace(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_DeactivateTraceIEs_t *s1ap_DeactivateTraceIEs;
    asn_enc_rval_t er;
    s1ap_DeactivateTraceIEs = &message_p->msg.s1ap_DeactivateTraceIEs;

    cb("<S1ap-DeactivateTraceIEs-PDU>\n", 30, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-DeactivateTraceIEs>\n", 30, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_DeactivateTraceIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_DeactivateTraceIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_E_UTRAN_Trace_ID, &s1ap_DeactivateTraceIEs->e_UTRAN_Trace_ID, cb, app_key, 2);
    cb("    </S1ap-DeactivateTraceIEs>\n", 31, app_key);
    cb("</S1ap-DeactivateTraceIEs-PDU>\n", 31, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabreleaselistbearerrelcomp(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABReleaseListBearerRelComp_IEs_t *s1ap_E_RABReleaseListBearerRelCompIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABReleaseListBearerRelCompIEs->s1ap_E_RABReleaseItemBearerRelComp.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABReleaseItemBearerRelComp, s1ap_E_RABReleaseListBearerRelCompIEs->s1ap_E_RABReleaseItemBearerRelComp.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_enbdirectinformationtransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_ENBDirectInformationTransferIEs_t *s1ap_ENBDirectInformationTransferIEs;
    asn_enc_rval_t er;
    s1ap_ENBDirectInformationTransferIEs = &message_p->msg.s1ap_ENBDirectInformationTransferIEs;

    cb("<S1ap-ENBDirectInformationTransferIEs-PDU>\n", 43, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-ENBDirectInformationTransferIEs>\n", 43, app_key);
    xer_encode_local(&asn_DEF_S1ap_Inter_SystemInformationTransferType, &s1ap_ENBDirectInformationTransferIEs->inter_SystemInformationTransferTypeEDT, cb, app_key, 2);
    cb("    </S1ap-ENBDirectInformationTransferIEs>\n", 44, app_key);
    cb("</S1ap-ENBDirectInformationTransferIEs-PDU>\n", 44, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabreleaseresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_E_RABReleaseResponseIEs_t *s1ap_E_RABReleaseResponseIEs;
    asn_enc_rval_t er;
    s1ap_E_RABReleaseResponseIEs = &message_p->msg.s1ap_E_RABReleaseResponseIEs;

    cb("<S1ap-E-RABReleaseResponseIEs-PDU>\n", 35, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-E-RABReleaseResponseIEs>\n", 35, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABReleaseResponseIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABReleaseResponseIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_E_RABReleaseResponseIEs->presenceMask & S1AP_E_RABRELEASERESPONSEIES_E_RABRELEASELISTBEARERRELCOMP_PRESENT)
        s1ap_xer_print_s1ap_e_rabreleaselistbearerrelcomp(cb, app_key, &s1ap_E_RABReleaseResponseIEs->e_RABReleaseListBearerRelComp);
    /* Optional field */
    if (s1ap_E_RABReleaseResponseIEs->presenceMask & S1AP_E_RABRELEASERESPONSEIES_E_RABFAILEDTORELEASELIST_PRESENT)
        s1ap_xer_print_s1ap_e_rablist(cb, app_key, &s1ap_E_RABReleaseResponseIEs->e_RABFailedToReleaseList);
    /* Optional field */
    if (s1ap_E_RABReleaseResponseIEs->presenceMask & S1AP_E_RABRELEASERESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_E_RABReleaseResponseIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-E-RABReleaseResponseIEs>\n", 36, app_key);
    cb("</S1ap-E-RABReleaseResponseIEs-PDU>\n", 36, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_uplinknonueassociatedlppatransport(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_UplinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkNonUEAssociatedLPPaTransport_IEs;
    asn_enc_rval_t er;
    s1ap_UplinkNonUEAssociatedLPPaTransport_IEs = &message_p->msg.s1ap_UplinkNonUEAssociatedLPPaTransport_IEs;

    cb("<S1ap-UplinkNonUEAssociatedLPPaTransport-IEs-PDU>\n", 50, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-UplinkNonUEAssociatedLPPaTransport-IEs>\n", 50, app_key);
    xer_encode_local(&asn_DEF_S1ap_Routing_ID, &s1ap_UplinkNonUEAssociatedLPPaTransport_IEs->routing_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_LPPa_PDU, &s1ap_UplinkNonUEAssociatedLPPaTransport_IEs->lpPa_PDU, cb, app_key, 2);
    cb("    </S1ap-UplinkNonUEAssociatedLPPaTransport-IEs>\n", 51, app_key);
    cb("</S1ap-UplinkNonUEAssociatedLPPaTransport-IEs-PDU>\n", 51, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobesetuplisthoreq(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeSetupListHOReq_IEs_t *s1ap_E_RABToBeSetupListHOReqIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABToBeSetupListHOReqIEs->s1ap_E_RABToBeSetupItemHOReq.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABToBeSetupItemHOReq, s1ap_E_RABToBeSetupListHOReqIEs->s1ap_E_RABToBeSetupItemHOReq.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_tracestart(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_TraceStartIEs_t *s1ap_TraceStartIEs;
    asn_enc_rval_t er;
    s1ap_TraceStartIEs = &message_p->msg.s1ap_TraceStartIEs;

    cb("<S1ap-TraceStartIEs-PDU>\n", 25, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-TraceStartIEs>\n", 25, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_TraceStartIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_TraceStartIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_TraceActivation, &s1ap_TraceStartIEs->traceActivation, cb, app_key, 2);
    cb("    </S1ap-TraceStartIEs>\n", 26, app_key);
    cb("</S1ap-TraceStartIEs-PDU>\n", 26, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_writereplacewarningresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_WriteReplaceWarningResponseIEs_t *s1ap_WriteReplaceWarningResponseIEs;
    asn_enc_rval_t er;
    s1ap_WriteReplaceWarningResponseIEs = &message_p->msg.s1ap_WriteReplaceWarningResponseIEs;

    cb("<S1ap-WriteReplaceWarningResponseIEs-PDU>\n", 42, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-WriteReplaceWarningResponseIEs>\n", 42, app_key);
    xer_encode_local(&asn_DEF_S1ap_MessageIdentifier, &s1ap_WriteReplaceWarningResponseIEs->messageIdentifier, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_SerialNumber, &s1ap_WriteReplaceWarningResponseIEs->serialNumber, cb, app_key, 2);
    /* Optional field */
    if (s1ap_WriteReplaceWarningResponseIEs->presenceMask & S1AP_WRITEREPLACEWARNINGRESPONSEIES_BROADCASTCOMPLETEDAREALIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_BroadcastCompletedAreaList, &s1ap_WriteReplaceWarningResponseIEs->broadcastCompletedAreaList, cb, app_key, 2);
    /* Optional field */
    if (s1ap_WriteReplaceWarningResponseIEs->presenceMask & S1AP_WRITEREPLACEWARNINGRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_WriteReplaceWarningResponseIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-WriteReplaceWarningResponseIEs>\n", 43, app_key);
    cb("</S1ap-WriteReplaceWarningResponseIEs-PDU>\n", 43, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_handovercancel(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_HandoverCancelIEs_t *s1ap_HandoverCancelIEs;
    asn_enc_rval_t er;
    s1ap_HandoverCancelIEs = &message_p->msg.s1ap_HandoverCancelIEs;

    cb("<S1ap-HandoverCancelIEs-PDU>\n", 29, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-HandoverCancelIEs>\n", 29, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverCancelIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverCancelIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_HandoverCancelIEs->cause, cb, app_key, 2);
    cb("    </S1ap-HandoverCancelIEs>\n", 30, app_key);
    cb("</S1ap-HandoverCancelIEs-PDU>\n", 30, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_handoverpreparationfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_HandoverPreparationFailureIEs_t *s1ap_HandoverPreparationFailureIEs;
    asn_enc_rval_t er;
    s1ap_HandoverPreparationFailureIEs = &message_p->msg.s1ap_HandoverPreparationFailureIEs;

    cb("<S1ap-HandoverPreparationFailureIEs-PDU>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-HandoverPreparationFailureIEs>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverPreparationFailureIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverPreparationFailureIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_HandoverPreparationFailureIEs->cause, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverPreparationFailureIEs->presenceMask & S1AP_HANDOVERPREPARATIONFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_HandoverPreparationFailureIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-HandoverPreparationFailureIEs>\n", 42, app_key);
    cb("</S1ap-HandoverPreparationFailureIEs-PDU>\n", 42, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobemodifiedlistbearermodreq(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeModifiedListBearerModReq_IEs_t *s1ap_E_RABToBeModifiedListBearerModReqIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABToBeModifiedListBearerModReqIEs->s1ap_E_RABToBeModifiedItemBearerModReq.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABToBeModifiedItemBearerModReq, s1ap_E_RABToBeModifiedListBearerModReqIEs->s1ap_E_RABToBeModifiedItemBearerModReq.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_uecontextreleaserequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_UEContextReleaseRequest_IEs_t *s1ap_UEContextReleaseRequest_IEs;
    asn_enc_rval_t er;
    s1ap_UEContextReleaseRequest_IEs = &message_p->msg.s1ap_UEContextReleaseRequest_IEs;

    cb("<S1ap-UEContextReleaseRequest-IEs-PDU>\n", 39, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-UEContextReleaseRequest-IEs>\n", 39, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UEContextReleaseRequest_IEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UEContextReleaseRequest_IEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_UEContextReleaseRequest_IEs->cause, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UEContextReleaseRequest_IEs->presenceMask & S1AP_UECONTEXTRELEASEREQUEST_IES_GWCONTEXTRELEASEINDICATION_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_GWContextReleaseIndication, &s1ap_UEContextReleaseRequest_IEs->gwContextReleaseIndication, cb, app_key, 2);
    cb("    </S1ap-UEContextReleaseRequest-IEs>\n", 40, app_key);
    cb("</S1ap-UEContextReleaseRequest-IEs-PDU>\n", 40, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabadmittedlist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABAdmittedList_IEs_t *s1ap_E_RABAdmittedListIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABAdmittedListIEs->s1ap_E_RABAdmittedItem.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABAdmittedItem, s1ap_E_RABAdmittedListIEs->s1ap_E_RABAdmittedItem.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_initialuemessage(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_InitialUEMessage_IEs_t *s1ap_InitialUEMessage_IEs;
    asn_enc_rval_t er;
    s1ap_InitialUEMessage_IEs = &message_p->msg.s1ap_InitialUEMessage_IEs;

    cb("<S1ap-InitialUEMessage-IEs-PDU>\n", 32, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-InitialUEMessage-IEs>\n", 32, app_key);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_InitialUEMessage_IEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_NAS_PDU, &s1ap_InitialUEMessage_IEs->nas_pdu, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_TAI, &s1ap_InitialUEMessage_IEs->tai, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_EUTRAN_CGI, &s1ap_InitialUEMessage_IEs->eutran_cgi, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_RRC_Establishment_Cause, &s1ap_InitialUEMessage_IEs->rrC_Establishment_Cause, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_S_TMSI_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_S_TMSI, &s1ap_InitialUEMessage_IEs->s_tmsi, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_CSG_ID_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSG_Id, &s1ap_InitialUEMessage_IEs->csG_Id, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_GUMMEI_ID_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_GUMMEI, &s1ap_InitialUEMessage_IEs->gummei_id, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_CELLACCESSMODE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CellAccessMode, &s1ap_InitialUEMessage_IEs->cellAccessMode, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_GW_TRANSPORTLAYERADDRESS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_TransportLayerAddress, &s1ap_InitialUEMessage_IEs->gW_TransportLayerAddress, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_RELAYNODE_INDICATOR_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_RelayNode_Indicator, &s1ap_InitialUEMessage_IEs->relayNode_Indicator, cb, app_key, 2);
    cb("    </S1ap-InitialUEMessage-IEs>\n", 33, app_key);
    cb("</S1ap-InitialUEMessage-IEs-PDU>\n", 33, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_killrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_KillRequestIEs_t *s1ap_KillRequestIEs;
    asn_enc_rval_t er;
    s1ap_KillRequestIEs = &message_p->msg.s1ap_KillRequestIEs;

    cb("<S1ap-KillRequestIEs-PDU>\n", 26, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-KillRequestIEs>\n", 26, app_key);
    xer_encode_local(&asn_DEF_S1ap_MessageIdentifier, &s1ap_KillRequestIEs->messageIdentifier, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_SerialNumber, &s1ap_KillRequestIEs->serialNumber, cb, app_key, 2);
    /* Optional field */
    if (s1ap_KillRequestIEs->presenceMask & S1AP_KILLREQUESTIES_WARNINGAREALIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_WarningAreaList, &s1ap_KillRequestIEs->warningAreaList, cb, app_key, 2);
    cb("    </S1ap-KillRequestIEs>\n", 27, app_key);
    cb("</S1ap-KillRequestIEs-PDU>\n", 27, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_writereplacewarningrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_WriteReplaceWarningRequestIEs_t *s1ap_WriteReplaceWarningRequestIEs;
    asn_enc_rval_t er;
    s1ap_WriteReplaceWarningRequestIEs = &message_p->msg.s1ap_WriteReplaceWarningRequestIEs;

    cb("<S1ap-WriteReplaceWarningRequestIEs-PDU>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-WriteReplaceWarningRequestIEs>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_MessageIdentifier, &s1ap_WriteReplaceWarningRequestIEs->messageIdentifier, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_SerialNumber, &s1ap_WriteReplaceWarningRequestIEs->serialNumber, cb, app_key, 2);
    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGAREALIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_WarningAreaList, &s1ap_WriteReplaceWarningRequestIEs->warningAreaList, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_RepetitionPeriod, &s1ap_WriteReplaceWarningRequestIEs->repetitionPeriod, cb, app_key, 2);
    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_EXTENDEDREPETITIONPERIOD_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_ExtendedRepetitionPeriod, &s1ap_WriteReplaceWarningRequestIEs->extendedRepetitionPeriod, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_NumberofBroadcastRequest, &s1ap_WriteReplaceWarningRequestIEs->numberofBroadcastRequest, cb, app_key, 2);
    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGTYPE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_WarningType, &s1ap_WriteReplaceWarningRequestIEs->warningType, cb, app_key, 2);
    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGSECURITYINFO_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_WarningSecurityInfo, &s1ap_WriteReplaceWarningRequestIEs->warningSecurityInfo, cb, app_key, 2);
    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_DATACODINGSCHEME_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_DataCodingScheme, &s1ap_WriteReplaceWarningRequestIEs->dataCodingScheme, cb, app_key, 2);
    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGMESSAGECONTENTS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_WarningMessageContents, &s1ap_WriteReplaceWarningRequestIEs->warningMessageContents, cb, app_key, 2);
    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_CONCURRENTWARNINGMESSAGEINDICATOR_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_ConcurrentWarningMessageIndicator, &s1ap_WriteReplaceWarningRequestIEs->concurrentWarningMessageIndicator, cb, app_key, 2);
    cb("    </S1ap-WriteReplaceWarningRequestIEs>\n", 42, app_key);
    cb("</S1ap-WriteReplaceWarningRequestIEs-PDU>\n", 42, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_uecapabilityinfoindication(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_UECapabilityInfoIndicationIEs_t *s1ap_UECapabilityInfoIndicationIEs;
    asn_enc_rval_t er;
    s1ap_UECapabilityInfoIndicationIEs = &message_p->msg.s1ap_UECapabilityInfoIndicationIEs;

    cb("<S1ap-UECapabilityInfoIndicationIEs-PDU>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-UECapabilityInfoIndicationIEs>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UECapabilityInfoIndicationIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UECapabilityInfoIndicationIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_UERadioCapability, &s1ap_UECapabilityInfoIndicationIEs->ueRadioCapability, cb, app_key, 2);
    cb("    </S1ap-UECapabilityInfoIndicationIEs>\n", 42, app_key);
    cb("</S1ap-UECapabilityInfoIndicationIEs-PDU>\n", 42, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobesetuplistbearersureq(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeSetupListBearerSUReq_IEs_t *s1ap_E_RABToBeSetupListBearerSUReqIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABToBeSetupListBearerSUReqIEs->s1ap_E_RABToBeSetupItemBearerSUReq.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABToBeSetupItemBearerSUReq, s1ap_E_RABToBeSetupListBearerSUReqIEs->s1ap_E_RABToBeSetupItemBearerSUReq.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabdataforwardinglist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABDataForwardingList_IEs_t *s1ap_E_RABDataForwardingListIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABDataForwardingListIEs->s1ap_E_RABDataForwardingItem.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABDataForwardingItem, s1ap_E_RABDataForwardingListIEs->s1ap_E_RABDataForwardingItem.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_locationreportingfailureindication(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_LocationReportingFailureIndicationIEs_t *s1ap_LocationReportingFailureIndicationIEs;
    asn_enc_rval_t er;
    s1ap_LocationReportingFailureIndicationIEs = &message_p->msg.s1ap_LocationReportingFailureIndicationIEs;

    cb("<S1ap-LocationReportingFailureIndicationIEs-PDU>\n", 49, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-LocationReportingFailureIndicationIEs>\n", 49, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_LocationReportingFailureIndicationIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_LocationReportingFailureIndicationIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_LocationReportingFailureIndicationIEs->cause, cb, app_key, 2);
    cb("    </S1ap-LocationReportingFailureIndicationIEs>\n", 50, app_key);
    cb("</S1ap-LocationReportingFailureIndicationIEs-PDU>\n", 50, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_initialcontextsetuprequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_InitialContextSetupRequestIEs_t *s1ap_InitialContextSetupRequestIEs;
    asn_enc_rval_t er;
    s1ap_InitialContextSetupRequestIEs = &message_p->msg.s1ap_InitialContextSetupRequestIEs;

    cb("<S1ap-InitialContextSetupRequestIEs-PDU>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-InitialContextSetupRequestIEs>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_InitialContextSetupRequestIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_InitialContextSetupRequestIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_InitialContextSetupRequestIEs->uEaggregateMaximumBitrate, cb, app_key, 2);
    s1ap_xer_print_s1ap_e_rabtobesetuplistctxtsureq(cb, app_key, &s1ap_InitialContextSetupRequestIEs->e_RABToBeSetupListCtxtSUReq);
    xer_encode_local(&asn_DEF_S1ap_UESecurityCapabilities, &s1ap_InitialContextSetupRequestIEs->ueSecurityCapabilities, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_SecurityKey, &s1ap_InitialContextSetupRequestIEs->securityKey, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_TRACEACTIVATION_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_TraceActivation, &s1ap_InitialContextSetupRequestIEs->traceActivation, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_HANDOVERRESTRICTIONLIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_HandoverRestrictionList, &s1ap_InitialContextSetupRequestIEs->handoverRestrictionList, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_UERADIOCAPABILITY_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_UERadioCapability, &s1ap_InitialContextSetupRequestIEs->ueRadioCapability, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_SUBSCRIBERPROFILEIDFORRFP_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_SubscriberProfileIDforRFP, &s1ap_InitialContextSetupRequestIEs->subscriberProfileIDforRFP, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_CSFALLBACKINDICATOR_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSFallbackIndicator, &s1ap_InitialContextSetupRequestIEs->csFallbackIndicator, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_SRVCCOPERATIONPOSSIBLE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_SRVCCOperationPossible, &s1ap_InitialContextSetupRequestIEs->srvccOperationPossible, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSGMembershipStatus, &s1ap_InitialContextSetupRequestIEs->csgMembershipStatus, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_REGISTEREDLAI_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_LAI, &s1ap_InitialContextSetupRequestIEs->registeredLAI, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_GUMMEI_ID_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_GUMMEI, &s1ap_InitialContextSetupRequestIEs->gummei_id, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_MME_UE_S1AP_ID_2_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_InitialContextSetupRequestIEs->mme_ue_s1ap_id_2, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_MANAGEMENTBASEDMDTALLOWED_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_ManagementBasedMDTAllowed, &s1ap_InitialContextSetupRequestIEs->managementBasedMDTAllowed, cb, app_key, 2);
    cb("    </S1ap-InitialContextSetupRequestIEs>\n", 42, app_key);
    cb("</S1ap-InitialContextSetupRequestIEs-PDU>\n", 42, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_locationreport(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_LocationReportIEs_t *s1ap_LocationReportIEs;
    asn_enc_rval_t er;
    s1ap_LocationReportIEs = &message_p->msg.s1ap_LocationReportIEs;

    cb("<S1ap-LocationReportIEs-PDU>\n", 29, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-LocationReportIEs>\n", 29, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_LocationReportIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_LocationReportIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_EUTRAN_CGI, &s1ap_LocationReportIEs->eutran_cgi, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_TAI, &s1ap_LocationReportIEs->tai, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_RequestType, &s1ap_LocationReportIEs->requestType, cb, app_key, 2);
    cb("    </S1ap-LocationReportIEs>\n", 30, app_key);
    cb("</S1ap-LocationReportIEs-PDU>\n", 30, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_uplinknastransport(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_UplinkNASTransport_IEs_t *s1ap_UplinkNASTransport_IEs;
    asn_enc_rval_t er;
    s1ap_UplinkNASTransport_IEs = &message_p->msg.s1ap_UplinkNASTransport_IEs;

    cb("<S1ap-UplinkNASTransport-IEs-PDU>\n", 34, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-UplinkNASTransport-IEs>\n", 34, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UplinkNASTransport_IEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UplinkNASTransport_IEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_NAS_PDU, &s1ap_UplinkNASTransport_IEs->nas_pdu, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_EUTRAN_CGI, &s1ap_UplinkNASTransport_IEs->eutran_cgi, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_TAI, &s1ap_UplinkNASTransport_IEs->tai, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UplinkNASTransport_IEs->presenceMask & S1AP_UPLINKNASTRANSPORT_IES_GW_TRANSPORTLAYERADDRESS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_TransportLayerAddress, &s1ap_UplinkNASTransport_IEs->gW_TransportLayerAddress, cb, app_key, 2);
    cb("    </S1ap-UplinkNASTransport-IEs>\n", 35, app_key);
    cb("</S1ap-UplinkNASTransport-IEs-PDU>\n", 35, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_uecontextmodificationresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_UEContextModificationResponseIEs_t *s1ap_UEContextModificationResponseIEs;
    asn_enc_rval_t er;
    s1ap_UEContextModificationResponseIEs = &message_p->msg.s1ap_UEContextModificationResponseIEs;

    cb("<S1ap-UEContextModificationResponseIEs-PDU>\n", 44, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-UEContextModificationResponseIEs>\n", 44, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UEContextModificationResponseIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UEContextModificationResponseIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UEContextModificationResponseIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_UEContextModificationResponseIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-UEContextModificationResponseIEs>\n", 45, app_key);
    cb("</S1ap-UEContextModificationResponseIEs-PDU>\n", 45, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_uecontextmodificationrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_UEContextModificationRequestIEs_t *s1ap_UEContextModificationRequestIEs;
    asn_enc_rval_t er;
    s1ap_UEContextModificationRequestIEs = &message_p->msg.s1ap_UEContextModificationRequestIEs;

    cb("<S1ap-UEContextModificationRequestIEs-PDU>\n", 43, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-UEContextModificationRequestIEs>\n", 43, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UEContextModificationRequestIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UEContextModificationRequestIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_SECURITYKEY_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_SecurityKey, &s1ap_UEContextModificationRequestIEs->securityKey, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_SUBSCRIBERPROFILEIDFORRFP_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_SubscriberProfileIDforRFP, &s1ap_UEContextModificationRequestIEs->subscriberProfileIDforRFP, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_UEContextModificationRequestIEs->uEaggregateMaximumBitrate, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_CSFALLBACKINDICATOR_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSFallbackIndicator, &s1ap_UEContextModificationRequestIEs->csFallbackIndicator, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_UESECURITYCAPABILITIES_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_UESecurityCapabilities, &s1ap_UEContextModificationRequestIEs->ueSecurityCapabilities, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSGMembershipStatus, &s1ap_UEContextModificationRequestIEs->csgMembershipStatus, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_REGISTEREDLAI_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_LAI, &s1ap_UEContextModificationRequestIEs->registeredLAI, cb, app_key, 2);
    cb("    </S1ap-UEContextModificationRequestIEs>\n", 44, app_key);
    cb("</S1ap-UEContextModificationRequestIEs-PDU>\n", 44, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_locationreportingcontrol(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_LocationReportingControlIEs_t *s1ap_LocationReportingControlIEs;
    asn_enc_rval_t er;
    s1ap_LocationReportingControlIEs = &message_p->msg.s1ap_LocationReportingControlIEs;

    cb("<S1ap-LocationReportingControlIEs-PDU>\n", 39, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-LocationReportingControlIEs>\n", 39, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_LocationReportingControlIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_LocationReportingControlIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_RequestType, &s1ap_LocationReportingControlIEs->requestType, cb, app_key, 2);
    cb("    </S1ap-LocationReportingControlIEs>\n", 40, app_key);
    cb("</S1ap-LocationReportingControlIEs-PDU>\n", 40, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_uplinks1cdma2000tunneling(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_UplinkS1cdma2000tunnelingIEs_t *s1ap_UplinkS1cdma2000tunnelingIEs;
    asn_enc_rval_t er;
    s1ap_UplinkS1cdma2000tunnelingIEs = &message_p->msg.s1ap_UplinkS1cdma2000tunnelingIEs;

    cb("<S1ap-UplinkS1cdma2000tunnelingIEs-PDU>\n", 40, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-UplinkS1cdma2000tunnelingIEs>\n", 40, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UplinkS1cdma2000tunnelingIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UplinkS1cdma2000tunnelingIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cdma2000RATType, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000RATType, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cdma2000SectorID, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000SectorID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask & S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000HOREQUIREDINDICATION_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_Cdma2000HORequiredIndication, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000HORequiredIndication, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask & S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000ONEXSRVCCINFO_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_Cdma2000OneXSRVCCInfo, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000OneXSRVCCInfo, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask & S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000ONEXRAND_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_Cdma2000OneXRAND, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000OneXRAND, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cdma2000PDU, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000PDU, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask & S1AP_UPLINKS1CDMA2000TUNNELINGIES_EUTRANROUNDTRIPDELAYESTIMATIONINFO_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_EUTRANRoundTripDelayEstimationInfo, &s1ap_UplinkS1cdma2000tunnelingIEs->eutranRoundTripDelayEstimationInfo, cb, app_key, 2);
    cb("    </S1ap-UplinkS1cdma2000tunnelingIEs>\n", 41, app_key);
    cb("</S1ap-UplinkS1cdma2000tunnelingIEs-PDU>\n", 41, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_handoverrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_HandoverRequestIEs_t *s1ap_HandoverRequestIEs;
    asn_enc_rval_t er;
    s1ap_HandoverRequestIEs = &message_p->msg.s1ap_HandoverRequestIEs;

    cb("<S1ap-HandoverRequestIEs-PDU>\n", 30, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-HandoverRequestIEs>\n", 30, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverRequestIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_HandoverType, &s1ap_HandoverRequestIEs->handoverType, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_HandoverRequestIEs->cause, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_HandoverRequestIEs->uEaggregateMaximumBitrate, cb, app_key, 2);
    s1ap_xer_print_s1ap_e_rabtobesetuplisthoreq(cb, app_key, &s1ap_HandoverRequestIEs->e_RABToBeSetupListHOReq);
    xer_encode_local(&asn_DEF_S1ap_Source_ToTarget_TransparentContainer, &s1ap_HandoverRequestIEs->source_ToTarget_TransparentContainer, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_UESecurityCapabilities, &s1ap_HandoverRequestIEs->ueSecurityCapabilities, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_HANDOVERRESTRICTIONLIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_HandoverRestrictionList, &s1ap_HandoverRequestIEs->handoverRestrictionList, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_TRACEACTIVATION_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_TraceActivation, &s1ap_HandoverRequestIEs->traceActivation, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_REQUESTTYPE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_RequestType, &s1ap_HandoverRequestIEs->requestType, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_SRVCCOPERATIONPOSSIBLE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_SRVCCOperationPossible, &s1ap_HandoverRequestIEs->srvccOperationPossible, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_SecurityContext, &s1ap_HandoverRequestIEs->securityContext, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_CSG_ID_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSG_Id, &s1ap_HandoverRequestIEs->csG_Id, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSGMembershipStatus, &s1ap_HandoverRequestIEs->csgMembershipStatus, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_GUMMEI_ID_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_GUMMEI, &s1ap_HandoverRequestIEs->gummei_id, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_MME_UE_S1AP_ID_2_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverRequestIEs->mme_ue_s1ap_id_2, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_MANAGEMENTBASEDMDTALLOWED_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_ManagementBasedMDTAllowed, &s1ap_HandoverRequestIEs->managementBasedMDTAllowed, cb, app_key, 2);
    cb("    </S1ap-HandoverRequestIEs>\n", 31, app_key);
    cb("</S1ap-HandoverRequestIEs-PDU>\n", 31, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_handoverrequired(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_HandoverRequiredIEs_t *s1ap_HandoverRequiredIEs;
    asn_enc_rval_t er;
    s1ap_HandoverRequiredIEs = &message_p->msg.s1ap_HandoverRequiredIEs;

    cb("<S1ap-HandoverRequiredIEs-PDU>\n", 31, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-HandoverRequiredIEs>\n", 31, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverRequiredIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverRequiredIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_HandoverType, &s1ap_HandoverRequiredIEs->handoverType, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_HandoverRequiredIEs->cause, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_TargetID, &s1ap_HandoverRequiredIEs->targetID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_DIRECT_FORWARDING_PATH_AVAILABILITY_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_Direct_Forwarding_Path_Availability, &s1ap_HandoverRequiredIEs->direct_Forwarding_Path_Availability, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_SRVCCHOINDICATION_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_SRVCCHOIndication, &s1ap_HandoverRequiredIEs->srvcchoIndication, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Source_ToTarget_TransparentContainer, &s1ap_HandoverRequiredIEs->source_ToTarget_TransparentContainer, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_SOURCE_TOTARGET_TRANSPARENTCONTAINER_SECONDARY_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_Source_ToTarget_TransparentContainer, &s1ap_HandoverRequiredIEs->source_ToTarget_TransparentContainer_Secondary, cb, app_key, 2);
    /* Conditional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_MSCLASSMARK2_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_MSClassmark2, &s1ap_HandoverRequiredIEs->msClassmark2, cb, app_key, 2);
    /* Conditional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_MSCLASSMARK3_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_MSClassmark3, &s1ap_HandoverRequiredIEs->msClassmark3, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_CSG_ID_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSG_Id, &s1ap_HandoverRequiredIEs->csG_Id, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_CELLACCESSMODE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CellAccessMode, &s1ap_HandoverRequiredIEs->cellAccessMode, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_PS_SERVICENOTAVAILABLE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_PS_ServiceNotAvailable, &s1ap_HandoverRequiredIEs->pS_ServiceNotAvailable, cb, app_key, 2);
    cb("    </S1ap-HandoverRequiredIEs>\n", 32, app_key);
    cb("</S1ap-HandoverRequiredIEs-PDU>\n", 32, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobeswitcheddllist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeSwitchedDLList_IEs_t *s1ap_E_RABToBeSwitchedDLListIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABToBeSwitchedDLListIEs->s1ap_E_RABToBeSwitchedDLItem.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABToBeSwitchedDLItem, s1ap_E_RABToBeSwitchedDLListIEs->s1ap_E_RABToBeSwitchedDLItem.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_mmeconfigurationupdateacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_MMEConfigurationUpdateAcknowledgeIEs_t *s1ap_MMEConfigurationUpdateAcknowledgeIEs;
    asn_enc_rval_t er;
    s1ap_MMEConfigurationUpdateAcknowledgeIEs = &message_p->msg.s1ap_MMEConfigurationUpdateAcknowledgeIEs;

    cb("<S1ap-MMEConfigurationUpdateAcknowledgeIEs-PDU>\n", 48, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-MMEConfigurationUpdateAcknowledgeIEs>\n", 48, app_key);
    /* Optional field */
    if (s1ap_MMEConfigurationUpdateAcknowledgeIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_MMEConfigurationUpdateAcknowledgeIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-MMEConfigurationUpdateAcknowledgeIEs>\n", 49, app_key);
    cb("</S1ap-MMEConfigurationUpdateAcknowledgeIEs-PDU>\n", 49, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_killresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_KillResponseIEs_t *s1ap_KillResponseIEs;
    asn_enc_rval_t er;
    s1ap_KillResponseIEs = &message_p->msg.s1ap_KillResponseIEs;

    cb("<S1ap-KillResponseIEs-PDU>\n", 27, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-KillResponseIEs>\n", 27, app_key);
    xer_encode_local(&asn_DEF_S1ap_MessageIdentifier, &s1ap_KillResponseIEs->messageIdentifier, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_SerialNumber, &s1ap_KillResponseIEs->serialNumber, cb, app_key, 2);
    /* Optional field */
    if (s1ap_KillResponseIEs->presenceMask & S1AP_KILLRESPONSEIES_BROADCASTCANCELLEDAREALIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_BroadcastCancelledAreaList, &s1ap_KillResponseIEs->broadcastCancelledAreaList, cb, app_key, 2);
    /* Optional field */
    if (s1ap_KillResponseIEs->presenceMask & S1AP_KILLRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_KillResponseIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-KillResponseIEs>\n", 28, app_key);
    cb("</S1ap-KillResponseIEs-PDU>\n", 28, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_mmestatustransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_MMEStatusTransferIEs_t *s1ap_MMEStatusTransferIEs;
    asn_enc_rval_t er;
    s1ap_MMEStatusTransferIEs = &message_p->msg.s1ap_MMEStatusTransferIEs;

    cb("<S1ap-MMEStatusTransferIEs-PDU>\n", 32, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-MMEStatusTransferIEs>\n", 32, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_MMEStatusTransferIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_MMEStatusTransferIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_StatusTransfer_TransparentContainer, &s1ap_MMEStatusTransferIEs->eNB_StatusTransfer_TransparentContainer, cb, app_key, 2);
    cb("    </S1ap-MMEStatusTransferIEs>\n", 33, app_key);
    cb("</S1ap-MMEStatusTransferIEs-PDU>\n", 33, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_mmeconfigurationupdate(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_MMEConfigurationUpdateIEs_t *s1ap_MMEConfigurationUpdateIEs;
    asn_enc_rval_t er;
    s1ap_MMEConfigurationUpdateIEs = &message_p->msg.s1ap_MMEConfigurationUpdateIEs;

    cb("<S1ap-MMEConfigurationUpdateIEs-PDU>\n", 37, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-MMEConfigurationUpdateIEs>\n", 37, app_key);
    /* Optional field */
    if (s1ap_MMEConfigurationUpdateIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEIES_MMENAME_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_MMEname, &s1ap_MMEConfigurationUpdateIEs->mmEname, cb, app_key, 2);
    /* Optional field */
    if (s1ap_MMEConfigurationUpdateIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEIES_SERVEDGUMMEIS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_ServedGUMMEIs, &s1ap_MMEConfigurationUpdateIEs->servedGUMMEIs, cb, app_key, 2);
    /* Optional field */
    if (s1ap_MMEConfigurationUpdateIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEIES_RELATIVEMMECAPACITY_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_RelativeMMECapacity, &s1ap_MMEConfigurationUpdateIEs->relativeMMECapacity, cb, app_key, 2);
    cb("    </S1ap-MMEConfigurationUpdateIEs>\n", 38, app_key);
    cb("</S1ap-MMEConfigurationUpdateIEs-PDU>\n", 38, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabmodifyrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_E_RABModifyRequestIEs_t *s1ap_E_RABModifyRequestIEs;
    asn_enc_rval_t er;
    s1ap_E_RABModifyRequestIEs = &message_p->msg.s1ap_E_RABModifyRequestIEs;

    cb("<S1ap-E-RABModifyRequestIEs-PDU>\n", 33, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-E-RABModifyRequestIEs>\n", 33, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABModifyRequestIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABModifyRequestIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_E_RABModifyRequestIEs->presenceMask & S1AP_E_RABMODIFYREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_E_RABModifyRequestIEs->uEaggregateMaximumBitrate, cb, app_key, 2);
    s1ap_xer_print_s1ap_e_rabtobemodifiedlistbearermodreq(cb, app_key, &s1ap_E_RABModifyRequestIEs->e_RABToBeModifiedListBearerModReq);
    cb("    </S1ap-E-RABModifyRequestIEs>\n", 34, app_key);
    cb("</S1ap-E-RABModifyRequestIEs-PDU>\n", 34, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabsetuplistctxtsures(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABSetupListCtxtSURes_IEs_t *s1ap_E_RABSetupListCtxtSUResIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABSetupListCtxtSUResIEs->s1ap_E_RABSetupItemCtxtSURes.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABSetupItemCtxtSURes, s1ap_E_RABSetupListCtxtSUResIEs->s1ap_E_RABSetupItemCtxtSURes.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobeswitchedullist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeSwitchedULList_IEs_t *s1ap_E_RABToBeSwitchedULListIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABToBeSwitchedULListIEs->s1ap_E_RABToBeSwitchedULItem.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABToBeSwitchedULItem, s1ap_E_RABToBeSwitchedULListIEs->s1ap_E_RABToBeSwitchedULItem.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_enbconfigurationupdatefailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_ENBConfigurationUpdateFailureIEs_t *s1ap_ENBConfigurationUpdateFailureIEs;
    asn_enc_rval_t er;
    s1ap_ENBConfigurationUpdateFailureIEs = &message_p->msg.s1ap_ENBConfigurationUpdateFailureIEs;

    cb("<S1ap-ENBConfigurationUpdateFailureIEs-PDU>\n", 44, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-ENBConfigurationUpdateFailureIEs>\n", 44, app_key);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_ENBConfigurationUpdateFailureIEs->cause, cb, app_key, 2);
    /* Optional field */
    if (s1ap_ENBConfigurationUpdateFailureIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEFAILUREIES_TIMETOWAIT_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_TimeToWait, &s1ap_ENBConfigurationUpdateFailureIEs->timeToWait, cb, app_key, 2);
    /* Optional field */
    if (s1ap_ENBConfigurationUpdateFailureIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_ENBConfigurationUpdateFailureIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-ENBConfigurationUpdateFailureIEs>\n", 45, app_key);
    cb("</S1ap-ENBConfigurationUpdateFailureIEs-PDU>\n", 45, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_reset(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_ResetIEs_t *s1ap_ResetIEs;
    asn_enc_rval_t er;
    s1ap_ResetIEs = &message_p->msg.s1ap_ResetIEs;

    cb("<S1ap-ResetIEs-PDU>\n", 20, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-ResetIEs>\n", 20, app_key);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_ResetIEs->cause, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ResetType, &s1ap_ResetIEs->resetType, cb, app_key, 2);
    cb("    </S1ap-ResetIEs>\n", 21, app_key);
    cb("</S1ap-ResetIEs-PDU>\n", 21, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_bearers_subjecttostatustransfer_list(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_Bearers_SubjectToStatusTransfer_List_IEs_t *s1ap_Bearers_SubjectToStatusTransfer_ListIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_Bearers_SubjectToStatusTransfer_ListIEs->s1ap_Bearers_SubjectToStatusTransfer_Item.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_Bearers_SubjectToStatusTransfer_Item, s1ap_Bearers_SubjectToStatusTransfer_ListIEs->s1ap_Bearers_SubjectToStatusTransfer_Item.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rablist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABList_IEs_t *s1ap_E_RABListIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABListIEs->s1ap_E_RABItem.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABItem, s1ap_E_RABListIEs->s1ap_E_RABItem.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_s1setupresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_S1SetupResponseIEs_t *s1ap_S1SetupResponseIEs;
    asn_enc_rval_t er;
    s1ap_S1SetupResponseIEs = &message_p->msg.s1ap_S1SetupResponseIEs;

    cb("<S1ap-S1SetupResponseIEs-PDU>\n", 30, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-S1SetupResponseIEs>\n", 30, app_key);
    /* Optional field */
    if (s1ap_S1SetupResponseIEs->presenceMask & S1AP_S1SETUPRESPONSEIES_MMENAME_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_MMEname, &s1ap_S1SetupResponseIEs->mmEname, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ServedGUMMEIs, &s1ap_S1SetupResponseIEs->servedGUMMEIs, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_RelativeMMECapacity, &s1ap_S1SetupResponseIEs->relativeMMECapacity, cb, app_key, 2);
    /* Optional field */
    if (s1ap_S1SetupResponseIEs->presenceMask & S1AP_S1SETUPRESPONSEIES_MMERELAYSUPPORTINDICATOR_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_MMERelaySupportIndicator, &s1ap_S1SetupResponseIEs->mmeRelaySupportIndicator, cb, app_key, 2);
    /* Optional field */
    if (s1ap_S1SetupResponseIEs->presenceMask & S1AP_S1SETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_S1SetupResponseIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-S1SetupResponseIEs>\n", 31, app_key);
    cb("</S1ap-S1SetupResponseIEs-PDU>\n", 31, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_paging(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_PagingIEs_t *s1ap_PagingIEs;
    asn_enc_rval_t er;
    s1ap_PagingIEs = &message_p->msg.s1ap_PagingIEs;

    cb("<S1ap-PagingIEs-PDU>\n", 21, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-PagingIEs>\n", 21, app_key);
    xer_encode_local(&asn_DEF_S1ap_UEIdentityIndexValue, &s1ap_PagingIEs->ueIdentityIndexValue, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_UEPagingID, &s1ap_PagingIEs->uePagingID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_PagingIEs->presenceMask & S1AP_PAGINGIES_PAGINGDRX_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_PagingDRX, &s1ap_PagingIEs->pagingDRX, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_CNDomain, &s1ap_PagingIEs->cnDomain, cb, app_key, 2);
    s1ap_xer_print_s1ap_tailist(cb, app_key, &s1ap_PagingIEs->taiList);
    /* Optional field */
    if (s1ap_PagingIEs->presenceMask & S1AP_PAGINGIES_CSG_IDLIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSG_IdList, &s1ap_PagingIEs->csG_IdList, cb, app_key, 2);
    /* Optional field */
    if (s1ap_PagingIEs->presenceMask & S1AP_PAGINGIES_PAGINGPRIORITY_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_PagingPriority, &s1ap_PagingIEs->pagingPriority, cb, app_key, 2);
    cb("    </S1ap-PagingIEs>\n", 22, app_key);
    cb("</S1ap-PagingIEs-PDU>\n", 22, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_enbconfigurationupdateacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_ENBConfigurationUpdateAcknowledgeIEs_t *s1ap_ENBConfigurationUpdateAcknowledgeIEs;
    asn_enc_rval_t er;
    s1ap_ENBConfigurationUpdateAcknowledgeIEs = &message_p->msg.s1ap_ENBConfigurationUpdateAcknowledgeIEs;

    cb("<S1ap-ENBConfigurationUpdateAcknowledgeIEs-PDU>\n", 48, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-ENBConfigurationUpdateAcknowledgeIEs>\n", 48, app_key);
    /* Optional field */
    if (s1ap_ENBConfigurationUpdateAcknowledgeIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_ENBConfigurationUpdateAcknowledgeIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-ENBConfigurationUpdateAcknowledgeIEs>\n", 49, app_key);
    cb("</S1ap-ENBConfigurationUpdateAcknowledgeIEs-PDU>\n", 49, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabreleasecommand(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_E_RABReleaseCommandIEs_t *s1ap_E_RABReleaseCommandIEs;
    asn_enc_rval_t er;
    s1ap_E_RABReleaseCommandIEs = &message_p->msg.s1ap_E_RABReleaseCommandIEs;

    cb("<S1ap-E-RABReleaseCommandIEs-PDU>\n", 34, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-E-RABReleaseCommandIEs>\n", 34, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABReleaseCommandIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABReleaseCommandIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_E_RABReleaseCommandIEs->presenceMask & S1AP_E_RABRELEASECOMMANDIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_E_RABReleaseCommandIEs->uEaggregateMaximumBitrate, cb, app_key, 2);
    s1ap_xer_print_s1ap_e_rablist(cb, app_key, &s1ap_E_RABReleaseCommandIEs->e_RABToBeReleasedList);
    /* Optional field */
    if (s1ap_E_RABReleaseCommandIEs->presenceMask & S1AP_E_RABRELEASECOMMANDIES_NAS_PDU_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_NAS_PDU, &s1ap_E_RABReleaseCommandIEs->nas_pdu, cb, app_key, 2);
    cb("    </S1ap-E-RABReleaseCommandIEs>\n", 35, app_key);
    cb("</S1ap-E-RABReleaseCommandIEs-PDU>\n", 35, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_pathswitchrequestfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_PathSwitchRequestFailureIEs_t *s1ap_PathSwitchRequestFailureIEs;
    asn_enc_rval_t er;
    s1ap_PathSwitchRequestFailureIEs = &message_p->msg.s1ap_PathSwitchRequestFailureIEs;

    cb("<S1ap-PathSwitchRequestFailureIEs-PDU>\n", 39, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-PathSwitchRequestFailureIEs>\n", 39, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_PathSwitchRequestFailureIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_PathSwitchRequestFailureIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_PathSwitchRequestFailureIEs->cause, cb, app_key, 2);
    /* Optional field */
    if (s1ap_PathSwitchRequestFailureIEs->presenceMask & S1AP_PATHSWITCHREQUESTFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_PathSwitchRequestFailureIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-PathSwitchRequestFailureIEs>\n", 40, app_key);
    cb("</S1ap-PathSwitchRequestFailureIEs-PDU>\n", 40, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabmodifyresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_E_RABModifyResponseIEs_t *s1ap_E_RABModifyResponseIEs;
    asn_enc_rval_t er;
    s1ap_E_RABModifyResponseIEs = &message_p->msg.s1ap_E_RABModifyResponseIEs;

    cb("<S1ap-E-RABModifyResponseIEs-PDU>\n", 34, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-E-RABModifyResponseIEs>\n", 34, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABModifyResponseIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABModifyResponseIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_E_RABModifyResponseIEs->presenceMask & S1AP_E_RABMODIFYRESPONSEIES_E_RABMODIFYLISTBEARERMODRES_PRESENT)
        s1ap_xer_print_s1ap_e_rabmodifylistbearermodres(cb, app_key, &s1ap_E_RABModifyResponseIEs->e_RABModifyListBearerModRes);
    /* Optional field */
    if (s1ap_E_RABModifyResponseIEs->presenceMask & S1AP_E_RABMODIFYRESPONSEIES_E_RABFAILEDTOMODIFYLIST_PRESENT)
        s1ap_xer_print_s1ap_e_rablist(cb, app_key, &s1ap_E_RABModifyResponseIEs->e_RABFailedToModifyList);
    /* Optional field */
    if (s1ap_E_RABModifyResponseIEs->presenceMask & S1AP_E_RABMODIFYRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_E_RABModifyResponseIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-E-RABModifyResponseIEs>\n", 35, app_key);
    cb("</S1ap-E-RABModifyResponseIEs-PDU>\n", 35, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_handovernotify(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_HandoverNotifyIEs_t *s1ap_HandoverNotifyIEs;
    asn_enc_rval_t er;
    s1ap_HandoverNotifyIEs = &message_p->msg.s1ap_HandoverNotifyIEs;

    cb("<S1ap-HandoverNotifyIEs-PDU>\n", 29, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-HandoverNotifyIEs>\n", 29, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverNotifyIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverNotifyIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_EUTRAN_CGI, &s1ap_HandoverNotifyIEs->eutran_cgi, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_TAI, &s1ap_HandoverNotifyIEs->tai, cb, app_key, 2);
    cb("    </S1ap-HandoverNotifyIEs>\n", 30, app_key);
    cb("</S1ap-HandoverNotifyIEs-PDU>\n", 30, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_handovercancelacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_HandoverCancelAcknowledgeIEs_t *s1ap_HandoverCancelAcknowledgeIEs;
    asn_enc_rval_t er;
    s1ap_HandoverCancelAcknowledgeIEs = &message_p->msg.s1ap_HandoverCancelAcknowledgeIEs;

    cb("<S1ap-HandoverCancelAcknowledgeIEs-PDU>\n", 40, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-HandoverCancelAcknowledgeIEs>\n", 40, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverCancelAcknowledgeIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverCancelAcknowledgeIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverCancelAcknowledgeIEs->presenceMask & S1AP_HANDOVERCANCELACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_HandoverCancelAcknowledgeIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-HandoverCancelAcknowledgeIEs>\n", 41, app_key);
    cb("</S1ap-HandoverCancelAcknowledgeIEs-PDU>\n", 41, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_initialcontextsetupfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_InitialContextSetupFailureIEs_t *s1ap_InitialContextSetupFailureIEs;
    asn_enc_rval_t er;
    s1ap_InitialContextSetupFailureIEs = &message_p->msg.s1ap_InitialContextSetupFailureIEs;

    cb("<S1ap-InitialContextSetupFailureIEs-PDU>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-InitialContextSetupFailureIEs>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_InitialContextSetupFailureIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_InitialContextSetupFailureIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_InitialContextSetupFailureIEs->cause, cb, app_key, 2);
    /* Optional field */
    if (s1ap_InitialContextSetupFailureIEs->presenceMask & S1AP_INITIALCONTEXTSETUPFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_InitialContextSetupFailureIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-InitialContextSetupFailureIEs>\n", 42, app_key);
    cb("</S1ap-InitialContextSetupFailureIEs-PDU>\n", 42, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_handovercommand(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_HandoverCommandIEs_t *s1ap_HandoverCommandIEs;
    asn_enc_rval_t er;
    s1ap_HandoverCommandIEs = &message_p->msg.s1ap_HandoverCommandIEs;

    cb("<S1ap-HandoverCommandIEs-PDU>\n", 30, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-HandoverCommandIEs>\n", 30, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverCommandIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverCommandIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_HandoverType, &s1ap_HandoverCommandIEs->handoverType, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverCommandIEs->presenceMask & S1AP_HANDOVERCOMMANDIES_E_RABDATAFORWARDINGLIST_PRESENT)
        s1ap_xer_print_s1ap_e_rabdataforwardinglist(cb, app_key, &s1ap_HandoverCommandIEs->e_RABDataForwardingList);
    /* Optional field */
    if (s1ap_HandoverCommandIEs->presenceMask & S1AP_HANDOVERCOMMANDIES_E_RABTORELEASELISTHOCMD_PRESENT)
        s1ap_xer_print_s1ap_e_rablist(cb, app_key, &s1ap_HandoverCommandIEs->e_RABtoReleaseListHOCmd);
    xer_encode_local(&asn_DEF_S1ap_Target_ToSource_TransparentContainer, &s1ap_HandoverCommandIEs->target_ToSource_TransparentContainer, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverCommandIEs->presenceMask & S1AP_HANDOVERCOMMANDIES_TARGET_TOSOURCE_TRANSPARENTCONTAINER_SECONDARY_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_Target_ToSource_TransparentContainer, &s1ap_HandoverCommandIEs->target_ToSource_TransparentContainer_Secondary, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverCommandIEs->presenceMask & S1AP_HANDOVERCOMMANDIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_HandoverCommandIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-HandoverCommandIEs>\n", 31, app_key);
    cb("</S1ap-HandoverCommandIEs-PDU>\n", 31, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_mmedirectinformationtransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_MMEDirectInformationTransferIEs_t *s1ap_MMEDirectInformationTransferIEs;
    asn_enc_rval_t er;
    s1ap_MMEDirectInformationTransferIEs = &message_p->msg.s1ap_MMEDirectInformationTransferIEs;

    cb("<S1ap-MMEDirectInformationTransferIEs-PDU>\n", 43, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-MMEDirectInformationTransferIEs>\n", 43, app_key);
    xer_encode_local(&asn_DEF_S1ap_Inter_SystemInformationTransferType, &s1ap_MMEDirectInformationTransferIEs->inter_SystemInformationTransferTypeMDT, cb, app_key, 2);
    cb("    </S1ap-MMEDirectInformationTransferIEs>\n", 44, app_key);
    cb("</S1ap-MMEDirectInformationTransferIEs-PDU>\n", 44, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_errorindication(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_ErrorIndicationIEs_t *s1ap_ErrorIndicationIEs;
    asn_enc_rval_t er;
    s1ap_ErrorIndicationIEs = &message_p->msg.s1ap_ErrorIndicationIEs;

    cb("<S1ap-ErrorIndicationIEs-PDU>\n", 30, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-ErrorIndicationIEs>\n", 30, app_key);
    /* Optional field */
    if (s1ap_ErrorIndicationIEs->presenceMask & S1AP_ERRORINDICATIONIES_MME_UE_S1AP_ID_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_ErrorIndicationIEs->mme_ue_s1ap_id, cb, app_key, 2);
    /* Optional field */
    if (s1ap_ErrorIndicationIEs->presenceMask & S1AP_ERRORINDICATIONIES_ENB_UE_S1AP_ID_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_ErrorIndicationIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_ErrorIndicationIEs->presenceMask & S1AP_ERRORINDICATIONIES_CAUSE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_ErrorIndicationIEs->cause, cb, app_key, 2);
    /* Optional field */
    if (s1ap_ErrorIndicationIEs->presenceMask & S1AP_ERRORINDICATIONIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_ErrorIndicationIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-ErrorIndicationIEs>\n", 31, app_key);
    cb("</S1ap-ErrorIndicationIEs-PDU>\n", 31, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_tailist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_TAIList_IEs_t *s1ap_TAIListIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_TAIListIEs->s1ap_TAIItem.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_TAIItem, s1ap_TAIListIEs->s1ap_TAIItem.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_overloadstart(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_OverloadStartIEs_t *s1ap_OverloadStartIEs;
    asn_enc_rval_t er;
    s1ap_OverloadStartIEs = &message_p->msg.s1ap_OverloadStartIEs;

    cb("<S1ap-OverloadStartIEs-PDU>\n", 28, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-OverloadStartIEs>\n", 28, app_key);
    xer_encode_local(&asn_DEF_S1ap_OverloadResponse, &s1ap_OverloadStartIEs->overloadResponse, cb, app_key, 2);
    /* Optional field */
    if (s1ap_OverloadStartIEs->presenceMask & S1AP_OVERLOADSTARTIES_GUMMEILIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_GUMMEIList, &s1ap_OverloadStartIEs->gummeiList, cb, app_key, 2);
    /* Optional field */
    if (s1ap_OverloadStartIEs->presenceMask & S1AP_OVERLOADSTARTIES_TRAFFICLOADREDUCTIONINDICATION_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_TrafficLoadReductionIndication, &s1ap_OverloadStartIEs->trafficLoadReductionIndication, cb, app_key, 2);
    cb("    </S1ap-OverloadStartIEs>\n", 29, app_key);
    cb("</S1ap-OverloadStartIEs-PDU>\n", 29, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_downlinknonueassociatedlppatransport(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_DownlinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs;
    asn_enc_rval_t er;
    s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs = &message_p->msg.s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs;

    cb("<S1ap-DownlinkNonUEAssociatedLPPaTransport-IEs-PDU>\n", 52, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-DownlinkNonUEAssociatedLPPaTransport-IEs>\n", 52, app_key);
    xer_encode_local(&asn_DEF_S1ap_Routing_ID, &s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs->routing_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_LPPa_PDU, &s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs->lpPa_PDU, cb, app_key, 2);
    cb("    </S1ap-DownlinkNonUEAssociatedLPPaTransport-IEs>\n", 53, app_key);
    cb("</S1ap-DownlinkNonUEAssociatedLPPaTransport-IEs-PDU>\n", 53, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_handoverrequestacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_HandoverRequestAcknowledgeIEs_t *s1ap_HandoverRequestAcknowledgeIEs;
    asn_enc_rval_t er;
    s1ap_HandoverRequestAcknowledgeIEs = &message_p->msg.s1ap_HandoverRequestAcknowledgeIEs;

    cb("<S1ap-HandoverRequestAcknowledgeIEs-PDU>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-HandoverRequestAcknowledgeIEs>\n", 41, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverRequestAcknowledgeIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverRequestAcknowledgeIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    s1ap_xer_print_s1ap_e_rabadmittedlist(cb, app_key, &s1ap_HandoverRequestAcknowledgeIEs->e_RABAdmittedList);
    /* Optional field */
    if (s1ap_HandoverRequestAcknowledgeIEs->presenceMask & S1AP_HANDOVERREQUESTACKNOWLEDGEIES_E_RABFAILEDTOSETUPLISTHOREQACK_PRESENT)
        s1ap_xer_print_s1ap_e_rabfailedtosetuplisthoreqack(cb, app_key, &s1ap_HandoverRequestAcknowledgeIEs->e_RABFailedToSetupListHOReqAck);
    xer_encode_local(&asn_DEF_S1ap_Target_ToSource_TransparentContainer, &s1ap_HandoverRequestAcknowledgeIEs->target_ToSource_TransparentContainer, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequestAcknowledgeIEs->presenceMask & S1AP_HANDOVERREQUESTACKNOWLEDGEIES_CSG_ID_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSG_Id, &s1ap_HandoverRequestAcknowledgeIEs->csG_Id, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverRequestAcknowledgeIEs->presenceMask & S1AP_HANDOVERREQUESTACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_HandoverRequestAcknowledgeIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-HandoverRequestAcknowledgeIEs>\n", 42, app_key);
    cb("</S1ap-HandoverRequestAcknowledgeIEs-PDU>\n", 42, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_downlinkueassociatedlppatransport(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_DownlinkUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkUEAssociatedLPPaTransport_IEs;
    asn_enc_rval_t er;
    s1ap_DownlinkUEAssociatedLPPaTransport_IEs = &message_p->msg.s1ap_DownlinkUEAssociatedLPPaTransport_IEs;

    cb("<S1ap-DownlinkUEAssociatedLPPaTransport-IEs-PDU>\n", 49, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-DownlinkUEAssociatedLPPaTransport-IEs>\n", 49, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Routing_ID, &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->routing_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_LPPa_PDU, &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->lpPa_PDU, cb, app_key, 2);
    cb("    </S1ap-DownlinkUEAssociatedLPPaTransport-IEs>\n", 50, app_key);
    cb("</S1ap-DownlinkUEAssociatedLPPaTransport-IEs-PDU>\n", 50, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_uecontextmodificationfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_UEContextModificationFailureIEs_t *s1ap_UEContextModificationFailureIEs;
    asn_enc_rval_t er;
    s1ap_UEContextModificationFailureIEs = &message_p->msg.s1ap_UEContextModificationFailureIEs;

    cb("<S1ap-UEContextModificationFailureIEs-PDU>\n", 43, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-UEContextModificationFailureIEs>\n", 43, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UEContextModificationFailureIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UEContextModificationFailureIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_UEContextModificationFailureIEs->cause, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UEContextModificationFailureIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_UEContextModificationFailureIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-UEContextModificationFailureIEs>\n", 44, app_key);
    cb("</S1ap-UEContextModificationFailureIEs-PDU>\n", 44, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_mmeconfigurationtransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_MMEConfigurationTransferIEs_t *s1ap_MMEConfigurationTransferIEs;
    asn_enc_rval_t er;
    s1ap_MMEConfigurationTransferIEs = &message_p->msg.s1ap_MMEConfigurationTransferIEs;

    cb("<S1ap-MMEConfigurationTransferIEs-PDU>\n", 39, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-MMEConfigurationTransferIEs>\n", 39, app_key);
    /* Optional field */
    if (s1ap_MMEConfigurationTransferIEs->presenceMask & S1AP_MMECONFIGURATIONTRANSFERIES_SONCONFIGURATIONTRANSFERMCT_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_SONConfigurationTransfer, &s1ap_MMEConfigurationTransferIEs->sonConfigurationTransferMCT, cb, app_key, 2);
    cb("    </S1ap-MMEConfigurationTransferIEs>\n", 40, app_key);
    cb("</S1ap-MMEConfigurationTransferIEs-PDU>\n", 40, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_pathswitchrequestacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_PathSwitchRequestAcknowledgeIEs_t *s1ap_PathSwitchRequestAcknowledgeIEs;
    asn_enc_rval_t er;
    s1ap_PathSwitchRequestAcknowledgeIEs = &message_p->msg.s1ap_PathSwitchRequestAcknowledgeIEs;

    cb("<S1ap-PathSwitchRequestAcknowledgeIEs-PDU>\n", 43, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-PathSwitchRequestAcknowledgeIEs>\n", 43, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_PathSwitchRequestAcknowledgeIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_PathSwitchRequestAcknowledgeIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask & S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_PathSwitchRequestAcknowledgeIEs->uEaggregateMaximumBitrate, cb, app_key, 2);
    /* Optional field */
    if (s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask & S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_E_RABTOBESWITCHEDULLIST_PRESENT)
        s1ap_xer_print_s1ap_e_rabtobeswitchedullist(cb, app_key, &s1ap_PathSwitchRequestAcknowledgeIEs->e_RABToBeSwitchedULList);
    /* Optional field */
    if (s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask & S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_E_RABTOBERELEASEDLIST_PRESENT)
        s1ap_xer_print_s1ap_e_rablist(cb, app_key, &s1ap_PathSwitchRequestAcknowledgeIEs->e_RABToBeReleasedList);
    xer_encode_local(&asn_DEF_S1ap_SecurityContext, &s1ap_PathSwitchRequestAcknowledgeIEs->securityContext, cb, app_key, 2);
    /* Optional field */
    if (s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask & S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_PathSwitchRequestAcknowledgeIEs->criticalityDiagnostics, cb, app_key, 2);
    /* Optional field */
    if (s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask & S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_MME_UE_S1AP_ID_2_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_PathSwitchRequestAcknowledgeIEs->mme_ue_s1ap_id_2, cb, app_key, 2);
    cb("    </S1ap-PathSwitchRequestAcknowledgeIEs>\n", 44, app_key);
    cb("</S1ap-PathSwitchRequestAcknowledgeIEs-PDU>\n", 44, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_ue_associatedlogicals1_connectionlistres(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_UE_associatedLogicalS1_ConnectionListRes_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListRes) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_UE_associatedLogicalS1_ConnectionListRes->s1ap_UE_associatedLogicalS1_ConnectionItemRes.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionItem, s1ap_UE_associatedLogicalS1_ConnectionListRes->s1ap_UE_associatedLogicalS1_ConnectionItemRes.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_s1setupfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_S1SetupFailureIEs_t *s1ap_S1SetupFailureIEs;
    asn_enc_rval_t er;
    s1ap_S1SetupFailureIEs = &message_p->msg.s1ap_S1SetupFailureIEs;

    cb("<S1ap-S1SetupFailureIEs-PDU>\n", 29, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-S1SetupFailureIEs>\n", 29, app_key);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_S1SetupFailureIEs->cause, cb, app_key, 2);
    /* Optional field */
    if (s1ap_S1SetupFailureIEs->presenceMask & S1AP_S1SETUPFAILUREIES_TIMETOWAIT_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_TimeToWait, &s1ap_S1SetupFailureIEs->timeToWait, cb, app_key, 2);
    /* Optional field */
    if (s1ap_S1SetupFailureIEs->presenceMask & S1AP_S1SETUPFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_S1SetupFailureIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-S1SetupFailureIEs>\n", 30, app_key);
    cb("</S1ap-S1SetupFailureIEs-PDU>\n", 30, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_mmeconfigurationupdatefailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_MMEConfigurationUpdateFailureIEs_t *s1ap_MMEConfigurationUpdateFailureIEs;
    asn_enc_rval_t er;
    s1ap_MMEConfigurationUpdateFailureIEs = &message_p->msg.s1ap_MMEConfigurationUpdateFailureIEs;

    cb("<S1ap-MMEConfigurationUpdateFailureIEs-PDU>\n", 44, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-MMEConfigurationUpdateFailureIEs>\n", 44, app_key);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_MMEConfigurationUpdateFailureIEs->cause, cb, app_key, 2);
    /* Optional field */
    if (s1ap_MMEConfigurationUpdateFailureIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEFAILUREIES_TIMETOWAIT_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_TimeToWait, &s1ap_MMEConfigurationUpdateFailureIEs->timeToWait, cb, app_key, 2);
    /* Optional field */
    if (s1ap_MMEConfigurationUpdateFailureIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_MMEConfigurationUpdateFailureIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-MMEConfigurationUpdateFailureIEs>\n", 45, app_key);
    cb("</S1ap-MMEConfigurationUpdateFailureIEs-PDU>\n", 45, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_initialcontextsetupresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_InitialContextSetupResponseIEs_t *s1ap_InitialContextSetupResponseIEs;
    asn_enc_rval_t er;
    s1ap_InitialContextSetupResponseIEs = &message_p->msg.s1ap_InitialContextSetupResponseIEs;

    cb("<S1ap-InitialContextSetupResponseIEs-PDU>\n", 42, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-InitialContextSetupResponseIEs>\n", 42, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_InitialContextSetupResponseIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_InitialContextSetupResponseIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    s1ap_xer_print_s1ap_e_rabsetuplistctxtsures(cb, app_key, &s1ap_InitialContextSetupResponseIEs->e_RABSetupListCtxtSURes);
    /* Optional field */
    if (s1ap_InitialContextSetupResponseIEs->presenceMask & S1AP_INITIALCONTEXTSETUPRESPONSEIES_E_RABFAILEDTOSETUPLISTCTXTSURES_PRESENT)
        s1ap_xer_print_s1ap_e_rablist(cb, app_key, &s1ap_InitialContextSetupResponseIEs->e_RABFailedToSetupListCtxtSURes);
    /* Optional field */
    if (s1ap_InitialContextSetupResponseIEs->presenceMask & S1AP_INITIALCONTEXTSETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_InitialContextSetupResponseIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-InitialContextSetupResponseIEs>\n", 43, app_key);
    cb("</S1ap-InitialContextSetupResponseIEs-PDU>\n", 43, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_downlinks1cdma2000tunneling(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_DownlinkS1cdma2000tunnelingIEs_t *s1ap_DownlinkS1cdma2000tunnelingIEs;
    asn_enc_rval_t er;
    s1ap_DownlinkS1cdma2000tunnelingIEs = &message_p->msg.s1ap_DownlinkS1cdma2000tunnelingIEs;

    cb("<S1ap-DownlinkS1cdma2000tunnelingIEs-PDU>\n", 42, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-DownlinkS1cdma2000tunnelingIEs>\n", 42, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_DownlinkS1cdma2000tunnelingIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_DownlinkS1cdma2000tunnelingIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_DownlinkS1cdma2000tunnelingIEs->presenceMask & S1AP_DOWNLINKS1CDMA2000TUNNELINGIES_E_RABDATAFORWARDINGLIST_PRESENT)
        s1ap_xer_print_s1ap_e_rabdataforwardinglist(cb, app_key, &s1ap_DownlinkS1cdma2000tunnelingIEs->e_RABDataForwardingList);
    /* Optional field */
    if (s1ap_DownlinkS1cdma2000tunnelingIEs->presenceMask & S1AP_DOWNLINKS1CDMA2000TUNNELINGIES_CDMA2000HOSTATUS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_Cdma2000HOStatus, &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000HOStatus, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cdma2000RATType, &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000RATType, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cdma2000PDU, &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000PDU, cb, app_key, 2);
    cb("    </S1ap-DownlinkS1cdma2000tunnelingIEs>\n", 43, app_key);
    cb("</S1ap-DownlinkS1cdma2000tunnelingIEs-PDU>\n", 43, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_overloadstop(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_OverloadStopIEs_t *s1ap_OverloadStopIEs;
    asn_enc_rval_t er;
    s1ap_OverloadStopIEs = &message_p->msg.s1ap_OverloadStopIEs;

    cb("<S1ap-OverloadStopIEs-PDU>\n", 27, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-OverloadStopIEs>\n", 27, app_key);
    /* Optional field */
    if (s1ap_OverloadStopIEs->presenceMask & S1AP_OVERLOADSTOPIES_GUMMEILIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_GUMMEIList, &s1ap_OverloadStopIEs->gummeiList, cb, app_key, 2);
    cb("    </S1ap-OverloadStopIEs>\n", 28, app_key);
    cb("</S1ap-OverloadStopIEs-PDU>\n", 28, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabsetuplistbearersures(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABSetupListBearerSURes_IEs_t *s1ap_E_RABSetupListBearerSUResIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABSetupListBearerSUResIEs->s1ap_E_RABSetupItemBearerSURes.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABSetupItemBearerSURes, s1ap_E_RABSetupListBearerSUResIEs->s1ap_E_RABSetupItemBearerSURes.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabmodifylistbearermodres(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABModifyListBearerModRes_IEs_t *s1ap_E_RABModifyListBearerModResIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABModifyListBearerModResIEs->s1ap_E_RABModifyItemBearerModRes.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABModifyItemBearerModRes, s1ap_E_RABModifyListBearerModResIEs->s1ap_E_RABModifyItemBearerModRes.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobesetuplistctxtsureq(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeSetupListCtxtSUReq_IEs_t *s1ap_E_RABToBeSetupListCtxtSUReqIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABToBeSetupListCtxtSUReqIEs->s1ap_E_RABToBeSetupItemCtxtSUReq.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABToBeSetupItemCtxtSUReq, s1ap_E_RABToBeSetupListCtxtSUReqIEs->s1ap_E_RABToBeSetupItemCtxtSUReq.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_enbconfigurationupdate(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_ENBConfigurationUpdateIEs_t *s1ap_ENBConfigurationUpdateIEs;
    asn_enc_rval_t er;
    s1ap_ENBConfigurationUpdateIEs = &message_p->msg.s1ap_ENBConfigurationUpdateIEs;

    cb("<S1ap-ENBConfigurationUpdateIEs-PDU>\n", 37, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-ENBConfigurationUpdateIEs>\n", 37, app_key);
    /* Optional field */
    if (s1ap_ENBConfigurationUpdateIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEIES_ENBNAME_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_ENBname, &s1ap_ENBConfigurationUpdateIEs->eNBname, cb, app_key, 2);
    /* Optional field */
    if (s1ap_ENBConfigurationUpdateIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEIES_SUPPORTEDTAS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_SupportedTAs, &s1ap_ENBConfigurationUpdateIEs->supportedTAs, cb, app_key, 2);
    /* Optional field */
    if (s1ap_ENBConfigurationUpdateIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEIES_CSG_IDLIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSG_IdList, &s1ap_ENBConfigurationUpdateIEs->csG_IdList, cb, app_key, 2);
    /* Optional field */
    if (s1ap_ENBConfigurationUpdateIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEIES_DEFAULTPAGINGDRX_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_PagingDRX, &s1ap_ENBConfigurationUpdateIEs->defaultPagingDRX, cb, app_key, 2);
    cb("    </S1ap-ENBConfigurationUpdateIEs>\n", 38, app_key);
    cb("</S1ap-ENBConfigurationUpdateIEs-PDU>\n", 38, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_tracefailureindication(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_TraceFailureIndicationIEs_t *s1ap_TraceFailureIndicationIEs;
    asn_enc_rval_t er;
    s1ap_TraceFailureIndicationIEs = &message_p->msg.s1ap_TraceFailureIndicationIEs;

    cb("<S1ap-TraceFailureIndicationIEs-PDU>\n", 37, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-TraceFailureIndicationIEs>\n", 37, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_TraceFailureIndicationIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_TraceFailureIndicationIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_E_UTRAN_Trace_ID, &s1ap_TraceFailureIndicationIEs->e_UTRAN_Trace_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_TraceFailureIndicationIEs->cause, cb, app_key, 2);
    cb("    </S1ap-TraceFailureIndicationIEs>\n", 38, app_key);
    cb("</S1ap-TraceFailureIndicationIEs-PDU>\n", 38, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabinformationlist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABInformationList_IEs_t *s1ap_E_RABInformationListIEs) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_E_RABInformationListIEs->s1ap_E_RABInformationList.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_E_RABInformationListItem, s1ap_E_RABInformationListIEs->s1ap_E_RABInformationList.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_enbconfigurationtransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_ENBConfigurationTransferIEs_t *s1ap_ENBConfigurationTransferIEs;
    asn_enc_rval_t er;
    s1ap_ENBConfigurationTransferIEs = &message_p->msg.s1ap_ENBConfigurationTransferIEs;

    cb("<S1ap-ENBConfigurationTransferIEs-PDU>\n", 39, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-ENBConfigurationTransferIEs>\n", 39, app_key);
    /* Optional field */
    if (s1ap_ENBConfigurationTransferIEs->presenceMask & S1AP_ENBCONFIGURATIONTRANSFERIES_SONCONFIGURATIONTRANSFERECT_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_SONConfigurationTransfer, &s1ap_ENBConfigurationTransferIEs->sonConfigurationTransferECT, cb, app_key, 2);
    cb("    </S1ap-ENBConfigurationTransferIEs>\n", 40, app_key);
    cb("</S1ap-ENBConfigurationTransferIEs-PDU>\n", 40, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_handoverfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_HandoverFailureIEs_t *s1ap_HandoverFailureIEs;
    asn_enc_rval_t er;
    s1ap_HandoverFailureIEs = &message_p->msg.s1ap_HandoverFailureIEs;

    cb("<S1ap-HandoverFailureIEs-PDU>\n", 30, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-HandoverFailureIEs>\n", 30, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverFailureIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_HandoverFailureIEs->cause, cb, app_key, 2);
    /* Optional field */
    if (s1ap_HandoverFailureIEs->presenceMask & S1AP_HANDOVERFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_HandoverFailureIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-HandoverFailureIEs>\n", 31, app_key);
    cb("</S1ap-HandoverFailureIEs-PDU>\n", 31, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_ue_associatedlogicals1_connectionlistresack(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_UE_associatedLogicalS1_ConnectionListResAck_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResAck) {

    int i;
    asn_enc_rval_t er;
    for (i = 0; i < s1ap_UE_associatedLogicalS1_ConnectionListResAck->s1ap_UE_associatedLogicalS1_ConnectionItemResAck.count; i++) {
        er = xer_encode(&asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionItem, s1ap_UE_associatedLogicalS1_ConnectionListResAck->s1ap_UE_associatedLogicalS1_ConnectionItemResAck.array[i], XER_F_BASIC, cb, app_key);
    }
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabsetupresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_E_RABSetupResponseIEs_t *s1ap_E_RABSetupResponseIEs;
    asn_enc_rval_t er;
    s1ap_E_RABSetupResponseIEs = &message_p->msg.s1ap_E_RABSetupResponseIEs;

    cb("<S1ap-E-RABSetupResponseIEs-PDU>\n", 33, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-E-RABSetupResponseIEs>\n", 33, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABSetupResponseIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABSetupResponseIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_E_RABSetupResponseIEs->presenceMask & S1AP_E_RABSETUPRESPONSEIES_E_RABSETUPLISTBEARERSURES_PRESENT)
        s1ap_xer_print_s1ap_e_rabsetuplistbearersures(cb, app_key, &s1ap_E_RABSetupResponseIEs->e_RABSetupListBearerSURes);
    /* Optional field */
    if (s1ap_E_RABSetupResponseIEs->presenceMask & S1AP_E_RABSETUPRESPONSEIES_E_RABFAILEDTOSETUPLISTBEARERSURES_PRESENT)
        s1ap_xer_print_s1ap_e_rablist(cb, app_key, &s1ap_E_RABSetupResponseIEs->e_RABFailedToSetupListBearerSURes);
    /* Optional field */
    if (s1ap_E_RABSetupResponseIEs->presenceMask & S1AP_E_RABSETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_E_RABSetupResponseIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-E-RABSetupResponseIEs>\n", 34, app_key);
    cb("</S1ap-E-RABSetupResponseIEs-PDU>\n", 34, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_uecontextreleasecommand(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_UEContextReleaseCommand_IEs_t *s1ap_UEContextReleaseCommand_IEs;
    asn_enc_rval_t er;
    s1ap_UEContextReleaseCommand_IEs = &message_p->msg.s1ap_UEContextReleaseCommand_IEs;

    cb("<S1ap-UEContextReleaseCommand-IEs-PDU>\n", 39, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-UEContextReleaseCommand-IEs>\n", 39, app_key);
    xer_encode_local(&asn_DEF_S1ap_UE_S1AP_IDs, &s1ap_UEContextReleaseCommand_IEs->uE_S1AP_IDs, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_UEContextReleaseCommand_IEs->cause, cb, app_key, 2);
    cb("    </S1ap-UEContextReleaseCommand-IEs>\n", 40, app_key);
    cb("</S1ap-UEContextReleaseCommand-IEs-PDU>\n", 40, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_pathswitchrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_PathSwitchRequestIEs_t *s1ap_PathSwitchRequestIEs;
    asn_enc_rval_t er;
    s1ap_PathSwitchRequestIEs = &message_p->msg.s1ap_PathSwitchRequestIEs;

    cb("<S1ap-PathSwitchRequestIEs-PDU>\n", 32, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-PathSwitchRequestIEs>\n", 32, app_key);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_PathSwitchRequestIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    s1ap_xer_print_s1ap_e_rabtobeswitcheddllist(cb, app_key, &s1ap_PathSwitchRequestIEs->e_RABToBeSwitchedDLList);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_PathSwitchRequestIEs->sourceMME_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_EUTRAN_CGI, &s1ap_PathSwitchRequestIEs->eutran_cgi, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_TAI, &s1ap_PathSwitchRequestIEs->tai, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_UESecurityCapabilities, &s1ap_PathSwitchRequestIEs->ueSecurityCapabilities, cb, app_key, 2);
    /* Optional field */
    if (s1ap_PathSwitchRequestIEs->presenceMask & S1AP_PATHSWITCHREQUESTIES_CSG_ID_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSG_Id, &s1ap_PathSwitchRequestIEs->csG_Id, cb, app_key, 2);
    /* Optional field */
    if (s1ap_PathSwitchRequestIEs->presenceMask & S1AP_PATHSWITCHREQUESTIES_CELLACCESSMODE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CellAccessMode, &s1ap_PathSwitchRequestIEs->cellAccessMode, cb, app_key, 2);
    /* Optional field */
    if (s1ap_PathSwitchRequestIEs->presenceMask & S1AP_PATHSWITCHREQUESTIES_SOURCEMME_GUMMEI_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_GUMMEI, &s1ap_PathSwitchRequestIEs->sourceMME_GUMMEI, cb, app_key, 2);
    cb("    </S1ap-PathSwitchRequestIEs>\n", 33, app_key);
    cb("</S1ap-PathSwitchRequestIEs-PDU>\n", 33, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_downlinknastransport(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_DownlinkNASTransport_IEs_t *s1ap_DownlinkNASTransport_IEs;
    asn_enc_rval_t er;
    s1ap_DownlinkNASTransport_IEs = &message_p->msg.s1ap_DownlinkNASTransport_IEs;

    cb("<S1ap-DownlinkNASTransport-IEs-PDU>\n", 36, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-DownlinkNASTransport-IEs>\n", 36, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_DownlinkNASTransport_IEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_DownlinkNASTransport_IEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_NAS_PDU, &s1ap_DownlinkNASTransport_IEs->nas_pdu, cb, app_key, 2);
    /* Optional field */
    if (s1ap_DownlinkNASTransport_IEs->presenceMask & S1AP_DOWNLINKNASTRANSPORT_IES_HANDOVERRESTRICTIONLIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_HandoverRestrictionList, &s1ap_DownlinkNASTransport_IEs->handoverRestrictionList, cb, app_key, 2);
    /* Optional field */
    if (s1ap_DownlinkNASTransport_IEs->presenceMask & S1AP_DOWNLINKNASTRANSPORT_IES_SUBSCRIBERPROFILEIDFORRFP_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_SubscriberProfileIDforRFP, &s1ap_DownlinkNASTransport_IEs->subscriberProfileIDforRFP, cb, app_key, 2);
    cb("    </S1ap-DownlinkNASTransport-IEs>\n", 37, app_key);
    cb("</S1ap-DownlinkNASTransport-IEs-PDU>\n", 37, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabreleaseindication(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_E_RABReleaseIndicationIEs_t *s1ap_E_RABReleaseIndicationIEs;
    asn_enc_rval_t er;
    s1ap_E_RABReleaseIndicationIEs = &message_p->msg.s1ap_E_RABReleaseIndicationIEs;

    cb("<S1ap-E-RABReleaseIndicationIEs-PDU>\n", 37, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-E-RABReleaseIndicationIEs>\n", 37, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABReleaseIndicationIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABReleaseIndicationIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    s1ap_xer_print_s1ap_e_rablist(cb, app_key, &s1ap_E_RABReleaseIndicationIEs->e_RABReleasedList);
    cb("    </S1ap-E-RABReleaseIndicationIEs>\n", 38, app_key);
    cb("</S1ap-E-RABReleaseIndicationIEs-PDU>\n", 38, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_s1setuprequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_S1SetupRequestIEs_t *s1ap_S1SetupRequestIEs;
    asn_enc_rval_t er;
    s1ap_S1SetupRequestIEs = &message_p->msg.s1ap_S1SetupRequestIEs;

    cb("<S1ap-S1SetupRequestIEs-PDU>\n", 29, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-S1SetupRequestIEs>\n", 29, app_key);
    xer_encode_local(&asn_DEF_S1ap_Global_ENB_ID, &s1ap_S1SetupRequestIEs->global_ENB_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_S1SetupRequestIEs->presenceMask & S1AP_S1SETUPREQUESTIES_ENBNAME_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_ENBname, &s1ap_S1SetupRequestIEs->eNBname, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_SupportedTAs, &s1ap_S1SetupRequestIEs->supportedTAs, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_PagingDRX, &s1ap_S1SetupRequestIEs->defaultPagingDRX, cb, app_key, 2);
    /* Optional field */
    if (s1ap_S1SetupRequestIEs->presenceMask & S1AP_S1SETUPREQUESTIES_CSG_IDLIST_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CSG_IdList, &s1ap_S1SetupRequestIEs->csG_IdList, cb, app_key, 2);
    cb("    </S1ap-S1SetupRequestIEs>\n", 30, app_key);
    cb("</S1ap-S1SetupRequestIEs-PDU>\n", 30, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_nasnondeliveryindication(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_NASNonDeliveryIndication_IEs_t *s1ap_NASNonDeliveryIndication_IEs;
    asn_enc_rval_t er;
    s1ap_NASNonDeliveryIndication_IEs = &message_p->msg.s1ap_NASNonDeliveryIndication_IEs;

    cb("<S1ap-NASNonDeliveryIndication-IEs-PDU>\n", 40, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-NASNonDeliveryIndication-IEs>\n", 40, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_NASNonDeliveryIndication_IEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_NASNonDeliveryIndication_IEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_NAS_PDU, &s1ap_NASNonDeliveryIndication_IEs->nas_pdu, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Cause, &s1ap_NASNonDeliveryIndication_IEs->cause, cb, app_key, 2);
    cb("    </S1ap-NASNonDeliveryIndication-IEs>\n", 41, app_key);
    cb("</S1ap-NASNonDeliveryIndication-IEs-PDU>\n", 41, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_celltraffictrace(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_CellTrafficTraceIEs_t *s1ap_CellTrafficTraceIEs;
    asn_enc_rval_t er;
    s1ap_CellTrafficTraceIEs = &message_p->msg.s1ap_CellTrafficTraceIEs;

    cb("<S1ap-CellTrafficTraceIEs-PDU>\n", 31, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-CellTrafficTraceIEs>\n", 31, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_CellTrafficTraceIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_CellTrafficTraceIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_E_UTRAN_Trace_ID, &s1ap_CellTrafficTraceIEs->e_UTRAN_Trace_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_EUTRAN_CGI, &s1ap_CellTrafficTraceIEs->eutran_cgi, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_TransportLayerAddress, &s1ap_CellTrafficTraceIEs->traceCollectionEntityIPAddress, cb, app_key, 2);
    /* Optional field */
    if (s1ap_CellTrafficTraceIEs->presenceMask & S1AP_CELLTRAFFICTRACEIES_PRIVACYINDICATOR_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_PrivacyIndicator, &s1ap_CellTrafficTraceIEs->privacyIndicator, cb, app_key, 2);
    cb("    </S1ap-CellTrafficTraceIEs>\n", 32, app_key);
    cb("</S1ap-CellTrafficTraceIEs-PDU>\n", 32, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_e_rabsetuprequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_E_RABSetupRequestIEs_t *s1ap_E_RABSetupRequestIEs;
    asn_enc_rval_t er;
    s1ap_E_RABSetupRequestIEs = &message_p->msg.s1ap_E_RABSetupRequestIEs;

    cb("<S1ap-E-RABSetupRequestIEs-PDU>\n", 32, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-E-RABSetupRequestIEs>\n", 32, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABSetupRequestIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABSetupRequestIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_E_RABSetupRequestIEs->presenceMask & S1AP_E_RABSETUPREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_E_RABSetupRequestIEs->uEaggregateMaximumBitrate, cb, app_key, 2);
    s1ap_xer_print_s1ap_e_rabtobesetuplistbearersureq(cb, app_key, &s1ap_E_RABSetupRequestIEs->e_RABToBeSetupListBearerSUReq);
    cb("    </S1ap-E-RABSetupRequestIEs>\n", 33, app_key);
    cb("</S1ap-E-RABSetupRequestIEs-PDU>\n", 33, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_enbstatustransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_ENBStatusTransferIEs_t *s1ap_ENBStatusTransferIEs;
    asn_enc_rval_t er;
    s1ap_ENBStatusTransferIEs = &message_p->msg.s1ap_ENBStatusTransferIEs;

    cb("<S1ap-ENBStatusTransferIEs-PDU>\n", 32, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-ENBStatusTransferIEs>\n", 32, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_ENBStatusTransferIEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_ENBStatusTransferIEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_StatusTransfer_TransparentContainer, &s1ap_ENBStatusTransferIEs->eNB_StatusTransfer_TransparentContainer, cb, app_key, 2);
    cb("    </S1ap-ENBStatusTransferIEs>\n", 33, app_key);
    cb("</S1ap-ENBStatusTransferIEs-PDU>\n", 33, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_resetacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_ResetAcknowledgeIEs_t *s1ap_ResetAcknowledgeIEs;
    asn_enc_rval_t er;
    s1ap_ResetAcknowledgeIEs = &message_p->msg.s1ap_ResetAcknowledgeIEs;

    cb("<S1ap-ResetAcknowledgeIEs-PDU>\n", 31, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-ResetAcknowledgeIEs>\n", 31, app_key);
    /* Optional field */
    if (s1ap_ResetAcknowledgeIEs->presenceMask & S1AP_RESETACKNOWLEDGEIES_UE_ASSOCIATEDLOGICALS1_CONNECTIONLISTRESACK_PRESENT)
        s1ap_xer_print_s1ap_ue_associatedlogicals1_connectionlistresack(cb, app_key, &s1ap_ResetAcknowledgeIEs->uE_associatedLogicalS1_ConnectionListResAck);
    /* Optional field */
    if (s1ap_ResetAcknowledgeIEs->presenceMask & S1AP_RESETACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_ResetAcknowledgeIEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-ResetAcknowledgeIEs>\n", 32, app_key);
    cb("</S1ap-ResetAcknowledgeIEs-PDU>\n", 32, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_uecontextreleasecomplete(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_UEContextReleaseComplete_IEs_t *s1ap_UEContextReleaseComplete_IEs;
    asn_enc_rval_t er;
    s1ap_UEContextReleaseComplete_IEs = &message_p->msg.s1ap_UEContextReleaseComplete_IEs;

    cb("<S1ap-UEContextReleaseComplete-IEs-PDU>\n", 40, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-UEContextReleaseComplete-IEs>\n", 40, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UEContextReleaseComplete_IEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UEContextReleaseComplete_IEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    /* Optional field */
    if (s1ap_UEContextReleaseComplete_IEs->presenceMask & S1AP_UECONTEXTRELEASECOMPLETE_IES_CRITICALITYDIAGNOSTICS_PRESENT)
        xer_encode_local(&asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_UEContextReleaseComplete_IEs->criticalityDiagnostics, cb, app_key, 2);
    cb("    </S1ap-UEContextReleaseComplete-IEs>\n", 41, app_key);
    cb("</S1ap-UEContextReleaseComplete-IEs-PDU>\n", 41, app_key);
    ASN__ENCODED_OK(er);
}

asn_enc_rval_t s1ap_xer_print_s1ap_uplinkueassociatedlppatransport(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p)
{
    S1ap_UplinkUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkUEAssociatedLPPaTransport_IEs;
    asn_enc_rval_t er;
    s1ap_UplinkUEAssociatedLPPaTransport_IEs = &message_p->msg.s1ap_UplinkUEAssociatedLPPaTransport_IEs;

    cb("<S1ap-UplinkUEAssociatedLPPaTransport-IEs-PDU>\n", 47, app_key);
    xer_encode_local(&asn_DEF_S1ap_Criticality, &message_p->criticality, cb, app_key, 1);
    xer_encode_local(&asn_DEF_S1ap_ProcedureCode, &message_p->procedureCode, cb, app_key, 1);
    cb("    <S1ap-UplinkUEAssociatedLPPaTransport-IEs>\n", 47, app_key);
    xer_encode_local(&asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UplinkUEAssociatedLPPaTransport_IEs->mme_ue_s1ap_id, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UplinkUEAssociatedLPPaTransport_IEs->eNB_UE_S1AP_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_Routing_ID, &s1ap_UplinkUEAssociatedLPPaTransport_IEs->routing_ID, cb, app_key, 2);
    xer_encode_local(&asn_DEF_S1ap_LPPa_PDU, &s1ap_UplinkUEAssociatedLPPaTransport_IEs->lpPa_PDU, cb, app_key, 2);
    cb("    </S1ap-UplinkUEAssociatedLPPaTransport-IEs>\n", 48, app_key);
    cb("</S1ap-UplinkUEAssociatedLPPaTransport-IEs-PDU>\n", 48, app_key);
    ASN__ENCODED_OK(er);
}

