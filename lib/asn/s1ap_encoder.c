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
 * Created on: 2017-02-02 21:30:57.642219 by acetcom
 * from ['S1AP-PDU.asn']
 ******************************************************************************/
#include "s1ap_common.h"
#include "s1ap_ies_defs.h"

int s1ap_encode_s1ap_e_rabfailedtosetupitemhoreqackies(
    S1ap_E_RABFailedtoSetupListHOReqAck_t *s1ap_E_RABFailedtoSetupListHOReqAck,
    S1ap_E_RABFailedtoSetupItemHOReqAckIEs_t *s1ap_E_RABFailedtoSetupItemHOReqAckIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABFailedtoSetupListHOReqAck != NULL);
    assert(s1ap_E_RABFailedtoSetupItemHOReqAckIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABFailedtoSetupItemHOReqAck,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_E_RABFailedToSetupItemHOReqAck,
                          &s1ap_E_RABFailedtoSetupItemHOReqAckIEs->e_RABFailedtoSetupItemHOReqAck)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABFailedtoSetupListHOReqAck->s1ap_E_RABFailedtoSetupListHOReqAck_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_deactivatetraceies(
    S1ap_DeactivateTrace_t *s1ap_DeactivateTrace,
    S1ap_DeactivateTraceIEs_t *s1ap_DeactivateTraceIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_DeactivateTrace != NULL);
    assert(s1ap_DeactivateTraceIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_DeactivateTraceIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DeactivateTrace->s1ap_DeactivateTrace_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_DeactivateTraceIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DeactivateTrace->s1ap_DeactivateTrace_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_UTRAN_Trace_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_E_UTRAN_Trace_ID,
                          &s1ap_DeactivateTraceIEs->e_UTRAN_Trace_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DeactivateTrace->s1ap_DeactivateTrace_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_enbdirectinformationtransferies(
    S1ap_ENBDirectInformationTransfer_t *s1ap_ENBDirectInformationTransfer,
    S1ap_ENBDirectInformationTransferIEs_t *s1ap_ENBDirectInformationTransferIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_ENBDirectInformationTransfer != NULL);
    assert(s1ap_ENBDirectInformationTransferIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Inter_SystemInformationTransferTypeEDT,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Inter_SystemInformationTransferType,
                          &s1ap_ENBDirectInformationTransferIEs->inter_SystemInformationTransferTypeEDT)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_ENBDirectInformationTransfer->s1ap_ENBDirectInformationTransfer_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_e_rabreleaseresponseies(
    S1ap_E_RABReleaseResponse_t *s1ap_E_RABReleaseResponse,
    S1ap_E_RABReleaseResponseIEs_t *s1ap_E_RABReleaseResponseIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABReleaseResponse != NULL);
    assert(s1ap_E_RABReleaseResponseIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_E_RABReleaseResponseIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseResponse->s1ap_E_RABReleaseResponse_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_E_RABReleaseResponseIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseResponse->s1ap_E_RABReleaseResponse_ies.list, ie);

    /* Optional field */
    if (s1ap_E_RABReleaseResponseIEs->presenceMask & S1AP_E_RABRELEASERESPONSEIES_E_RABRELEASELISTBEARERRELCOMP_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABReleaseListBearerRelComp,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABReleaseListBearerRelComp,
                            &s1ap_E_RABReleaseResponseIEs->e_RABReleaseListBearerRelComp)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseResponse->s1ap_E_RABReleaseResponse_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_E_RABReleaseResponseIEs->presenceMask & S1AP_E_RABRELEASERESPONSEIES_E_RABFAILEDTORELEASELIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABFailedToReleaseList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABList,
                            &s1ap_E_RABReleaseResponseIEs->e_RABFailedToReleaseList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseResponse->s1ap_E_RABReleaseResponse_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_E_RABReleaseResponseIEs->presenceMask & S1AP_E_RABRELEASERESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_E_RABReleaseResponseIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseResponse->s1ap_E_RABReleaseResponse_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_uplinknonueassociatedlppatransport_ies(
    S1ap_UplinkNonUEAssociatedLPPaTransport_t *s1ap_UplinkNonUEAssociatedLPPaTransport,
    S1ap_UplinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkNonUEAssociatedLPPaTransport_IEs) {

    S1ap_IE_t *ie;

    assert(s1ap_UplinkNonUEAssociatedLPPaTransport != NULL);
    assert(s1ap_UplinkNonUEAssociatedLPPaTransport_IEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Routing_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Routing_ID,
                          &s1ap_UplinkNonUEAssociatedLPPaTransport_IEs->routing_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkNonUEAssociatedLPPaTransport->s1ap_UplinkNonUEAssociatedLPPaTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_LPPa_PDU,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_LPPa_PDU,
                          &s1ap_UplinkNonUEAssociatedLPPaTransport_IEs->lpPa_PDU)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkNonUEAssociatedLPPaTransport->s1ap_UplinkNonUEAssociatedLPPaTransport_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_e_rabtobesetupitemhoreqies(
    S1ap_E_RABToBeSetupListHOReq_t *s1ap_E_RABToBeSetupListHOReq,
    S1ap_E_RABToBeSetupItemHOReqIEs_t *s1ap_E_RABToBeSetupItemHOReqIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABToBeSetupListHOReq != NULL);
    assert(s1ap_E_RABToBeSetupItemHOReqIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeSetupItemHOReq,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_E_RABToBeSetupItemHOReq,
                          &s1ap_E_RABToBeSetupItemHOReqIEs->e_RABToBeSetupItemHOReq)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABToBeSetupListHOReq->s1ap_E_RABToBeSetupListHOReq_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_tracestarties(
    S1ap_TraceStart_t *s1ap_TraceStart,
    S1ap_TraceStartIEs_t *s1ap_TraceStartIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_TraceStart != NULL);
    assert(s1ap_TraceStartIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_TraceStartIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_TraceStart->s1ap_TraceStart_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_TraceStartIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_TraceStart->s1ap_TraceStart_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TraceActivation,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_TraceActivation,
                          &s1ap_TraceStartIEs->traceActivation)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_TraceStart->s1ap_TraceStart_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_writereplacewarningresponseies(
    S1ap_WriteReplaceWarningResponse_t *s1ap_WriteReplaceWarningResponse,
    S1ap_WriteReplaceWarningResponseIEs_t *s1ap_WriteReplaceWarningResponseIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_WriteReplaceWarningResponse != NULL);
    assert(s1ap_WriteReplaceWarningResponseIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MessageIdentifier,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MessageIdentifier,
                          &s1ap_WriteReplaceWarningResponseIEs->messageIdentifier)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningResponse->s1ap_WriteReplaceWarningResponse_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SerialNumber,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_SerialNumber,
                          &s1ap_WriteReplaceWarningResponseIEs->serialNumber)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningResponse->s1ap_WriteReplaceWarningResponse_ies.list, ie);

    /* Optional field */
    if (s1ap_WriteReplaceWarningResponseIEs->presenceMask & S1AP_WRITEREPLACEWARNINGRESPONSEIES_BROADCASTCOMPLETEDAREALIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_BroadcastCompletedAreaList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_BroadcastCompletedAreaList,
                            &s1ap_WriteReplaceWarningResponseIEs->broadcastCompletedAreaList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningResponse->s1ap_WriteReplaceWarningResponse_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_WriteReplaceWarningResponseIEs->presenceMask & S1AP_WRITEREPLACEWARNINGRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_WriteReplaceWarningResponseIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningResponse->s1ap_WriteReplaceWarningResponse_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_handovercancelies(
    S1ap_HandoverCancel_t *s1ap_HandoverCancel,
    S1ap_HandoverCancelIEs_t *s1ap_HandoverCancelIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_HandoverCancel != NULL);
    assert(s1ap_HandoverCancelIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_HandoverCancelIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverCancel->s1ap_HandoverCancel_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_HandoverCancelIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverCancel->s1ap_HandoverCancel_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_HandoverCancelIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverCancel->s1ap_HandoverCancel_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_handoverpreparationfailureies(
    S1ap_HandoverPreparationFailure_t *s1ap_HandoverPreparationFailure,
    S1ap_HandoverPreparationFailureIEs_t *s1ap_HandoverPreparationFailureIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_HandoverPreparationFailure != NULL);
    assert(s1ap_HandoverPreparationFailureIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_HandoverPreparationFailureIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverPreparationFailure->s1ap_HandoverPreparationFailure_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_HandoverPreparationFailureIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverPreparationFailure->s1ap_HandoverPreparationFailure_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_HandoverPreparationFailureIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverPreparationFailure->s1ap_HandoverPreparationFailure_ies.list, ie);

    /* Optional field */
    if (s1ap_HandoverPreparationFailureIEs->presenceMask & S1AP_HANDOVERPREPARATIONFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_HandoverPreparationFailureIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverPreparationFailure->s1ap_HandoverPreparationFailure_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_uecontextreleaserequest_ies(
    S1ap_UEContextReleaseRequest_t *s1ap_UEContextReleaseRequest,
    S1ap_UEContextReleaseRequest_IEs_t *s1ap_UEContextReleaseRequest_IEs) {

    S1ap_IE_t *ie;

    assert(s1ap_UEContextReleaseRequest != NULL);
    assert(s1ap_UEContextReleaseRequest_IEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_UEContextReleaseRequest_IEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextReleaseRequest->s1ap_UEContextReleaseRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_UEContextReleaseRequest_IEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextReleaseRequest->s1ap_UEContextReleaseRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_UEContextReleaseRequest_IEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextReleaseRequest->s1ap_UEContextReleaseRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_UEContextReleaseRequest_IEs->presenceMask & S1AP_UECONTEXTRELEASEREQUEST_IES_GWCONTEXTRELEASEINDICATION_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_GWContextReleaseIndication,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_GWContextReleaseIndication,
                            &s1ap_UEContextReleaseRequest_IEs->gwContextReleaseIndication)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UEContextReleaseRequest->s1ap_UEContextReleaseRequest_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_e_rabadmitteditemies(
    S1ap_E_RABAdmittedList_t *s1ap_E_RABAdmittedList,
    S1ap_E_RABAdmittedItemIEs_t *s1ap_E_RABAdmittedItemIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABAdmittedList != NULL);
    assert(s1ap_E_RABAdmittedItemIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABAdmittedItem,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_E_RABAdmittedItem,
                          &s1ap_E_RABAdmittedItemIEs->e_RABAdmittedItem)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABAdmittedList->s1ap_E_RABAdmittedList_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_initialuemessage_ies(
    S1ap_InitialUEMessage_t *s1ap_InitialUEMessage,
    S1ap_InitialUEMessage_IEs_t *s1ap_InitialUEMessage_IEs) {

    S1ap_IE_t *ie;

    assert(s1ap_InitialUEMessage != NULL);
    assert(s1ap_InitialUEMessage_IEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_InitialUEMessage_IEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialUEMessage->s1ap_InitialUEMessage_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_NAS_PDU,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_NAS_PDU,
                          &s1ap_InitialUEMessage_IEs->nas_pdu)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialUEMessage->s1ap_InitialUEMessage_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TAI,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_TAI,
                          &s1ap_InitialUEMessage_IEs->tai)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialUEMessage->s1ap_InitialUEMessage_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_EUTRAN_CGI,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_EUTRAN_CGI,
                          &s1ap_InitialUEMessage_IEs->eutran_cgi)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialUEMessage->s1ap_InitialUEMessage_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_RRC_Establishment_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_RRC_Establishment_Cause,
                          &s1ap_InitialUEMessage_IEs->rrC_Establishment_Cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialUEMessage->s1ap_InitialUEMessage_ies.list, ie);

    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_S_TMSI_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_S_TMSI,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_S_TMSI,
                            &s1ap_InitialUEMessage_IEs->s_tmsi)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialUEMessage->s1ap_InitialUEMessage_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_CSG_ID_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSG_Id,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_CSG_Id,
                            &s1ap_InitialUEMessage_IEs->csG_Id)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialUEMessage->s1ap_InitialUEMessage_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_GUMMEI_ID_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_GUMMEI_ID,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_GUMMEI,
                            &s1ap_InitialUEMessage_IEs->gummei_id)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialUEMessage->s1ap_InitialUEMessage_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_CELLACCESSMODE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CellAccessMode,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_CellAccessMode,
                            &s1ap_InitialUEMessage_IEs->cellAccessMode)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialUEMessage->s1ap_InitialUEMessage_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_GW_S1AP_TRANSPORTLAYERADDRESS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_GW_S1ap_TransportLayerAddress,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_TransportLayerAddress,
                            &s1ap_InitialUEMessage_IEs->gW_S1ap_TransportLayerAddress)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialUEMessage->s1ap_InitialUEMessage_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialUEMessage_IEs->presenceMask & S1AP_INITIALUEMESSAGE_IES_RELAYNODE_INDICATOR_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_RelayNode_Indicator,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_RelayNode_Indicator,
                            &s1ap_InitialUEMessage_IEs->relayNode_Indicator)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialUEMessage->s1ap_InitialUEMessage_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_killrequesties(
    S1ap_KillRequest_t *s1ap_KillRequest,
    S1ap_KillRequestIEs_t *s1ap_KillRequestIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_KillRequest != NULL);
    assert(s1ap_KillRequestIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MessageIdentifier,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MessageIdentifier,
                          &s1ap_KillRequestIEs->messageIdentifier)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_KillRequest->s1ap_KillRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SerialNumber,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_SerialNumber,
                          &s1ap_KillRequestIEs->serialNumber)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_KillRequest->s1ap_KillRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_KillRequestIEs->presenceMask & S1AP_KILLREQUESTIES_WARNINGAREALIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_WarningAreaList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_WarningAreaList,
                            &s1ap_KillRequestIEs->warningAreaList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_KillRequest->s1ap_KillRequest_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_writereplacewarningrequesties(
    S1ap_WriteReplaceWarningRequest_t *s1ap_WriteReplaceWarningRequest,
    S1ap_WriteReplaceWarningRequestIEs_t *s1ap_WriteReplaceWarningRequestIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_WriteReplaceWarningRequest != NULL);
    assert(s1ap_WriteReplaceWarningRequestIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MessageIdentifier,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MessageIdentifier,
                          &s1ap_WriteReplaceWarningRequestIEs->messageIdentifier)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningRequest->s1ap_WriteReplaceWarningRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SerialNumber,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_SerialNumber,
                          &s1ap_WriteReplaceWarningRequestIEs->serialNumber)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningRequest->s1ap_WriteReplaceWarningRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGAREALIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_WarningAreaList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_WarningAreaList,
                            &s1ap_WriteReplaceWarningRequestIEs->warningAreaList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningRequest->s1ap_WriteReplaceWarningRequest_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_RepetitionPeriod,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_RepetitionPeriod,
                          &s1ap_WriteReplaceWarningRequestIEs->repetitionPeriod)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningRequest->s1ap_WriteReplaceWarningRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_EXTENDEDREPETITIONPERIOD_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_ExtendedRepetitionPeriod,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_ExtendedRepetitionPeriod,
                            &s1ap_WriteReplaceWarningRequestIEs->extendedRepetitionPeriod)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningRequest->s1ap_WriteReplaceWarningRequest_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_NumberofBroadcastRequest,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_NumberofBroadcastRequest,
                          &s1ap_WriteReplaceWarningRequestIEs->numberofBroadcastRequest)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningRequest->s1ap_WriteReplaceWarningRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGTYPE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_WarningType,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_WarningType,
                            &s1ap_WriteReplaceWarningRequestIEs->warningType)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningRequest->s1ap_WriteReplaceWarningRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGSECURITYINFO_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_WarningSecurityInfo,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_WarningSecurityInfo,
                            &s1ap_WriteReplaceWarningRequestIEs->warningSecurityInfo)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningRequest->s1ap_WriteReplaceWarningRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_DATACODINGSCHEME_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_DataCodingScheme,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_DataCodingScheme,
                            &s1ap_WriteReplaceWarningRequestIEs->dataCodingScheme)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningRequest->s1ap_WriteReplaceWarningRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGMESSAGECONTENTS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_WarningMessageContents,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_WarningMessageContents,
                            &s1ap_WriteReplaceWarningRequestIEs->warningMessageContents)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningRequest->s1ap_WriteReplaceWarningRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_WriteReplaceWarningRequestIEs->presenceMask & S1AP_WRITEREPLACEWARNINGREQUESTIES_CONCURRENTWARNINGMESSAGEINDICATOR_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_ConcurrentWarningMessageIndicator,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_ConcurrentWarningMessageIndicator,
                            &s1ap_WriteReplaceWarningRequestIEs->concurrentWarningMessageIndicator)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_WriteReplaceWarningRequest->s1ap_WriteReplaceWarningRequest_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_uecapabilityinfoindicationies(
    S1ap_UECapabilityInfoIndication_t *s1ap_UECapabilityInfoIndication,
    S1ap_UECapabilityInfoIndicationIEs_t *s1ap_UECapabilityInfoIndicationIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_UECapabilityInfoIndication != NULL);
    assert(s1ap_UECapabilityInfoIndicationIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_UECapabilityInfoIndicationIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UECapabilityInfoIndication->s1ap_UECapabilityInfoIndication_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_UECapabilityInfoIndicationIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UECapabilityInfoIndication->s1ap_UECapabilityInfoIndication_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UERadioCapability,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_UERadioCapability,
                          &s1ap_UECapabilityInfoIndicationIEs->ueRadioCapability)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UECapabilityInfoIndication->s1ap_UECapabilityInfoIndication_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_e_rabdataforwardingitemies(
    S1ap_E_RABDataForwardingList_t *s1ap_E_RABDataForwardingList,
    S1ap_E_RABDataForwardingItemIEs_t *s1ap_E_RABDataForwardingItemIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABDataForwardingList != NULL);
    assert(s1ap_E_RABDataForwardingItemIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABDataForwardingItem,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_E_RABDataForwardingItem,
                          &s1ap_E_RABDataForwardingItemIEs->e_RABDataForwardingItem)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABDataForwardingList->s1ap_E_RABDataForwardingList_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_locationreportingfailureindicationies(
    S1ap_LocationReportingFailureIndication_t *s1ap_LocationReportingFailureIndication,
    S1ap_LocationReportingFailureIndicationIEs_t *s1ap_LocationReportingFailureIndicationIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_LocationReportingFailureIndication != NULL);
    assert(s1ap_LocationReportingFailureIndicationIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_LocationReportingFailureIndicationIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_LocationReportingFailureIndication->s1ap_LocationReportingFailureIndication_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_LocationReportingFailureIndicationIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_LocationReportingFailureIndication->s1ap_LocationReportingFailureIndication_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_LocationReportingFailureIndicationIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_LocationReportingFailureIndication->s1ap_LocationReportingFailureIndication_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_initialcontextsetuprequesties(
    S1ap_InitialContextSetupRequest_t *s1ap_InitialContextSetupRequest,
    S1ap_InitialContextSetupRequestIEs_t *s1ap_InitialContextSetupRequestIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_InitialContextSetupRequest != NULL);
    assert(s1ap_InitialContextSetupRequestIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_InitialContextSetupRequestIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_InitialContextSetupRequestIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_UEAggregateMaximumBitrate,
                          &s1ap_InitialContextSetupRequestIEs->uEaggregateMaximumBitrate)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);

    S1ap_E_RABToBeSetupListCtxtSUReq_t e_RABToBeSetupListCtxtSUReq;

    memset(&e_RABToBeSetupListCtxtSUReq, 0, sizeof(S1ap_E_RABToBeSetupListCtxtSUReq_t));

    if (s1ap_encode_s1ap_e_rabtobesetuplistctxtsureq(&e_RABToBeSetupListCtxtSUReq, &s1ap_InitialContextSetupRequestIEs->e_RABToBeSetupListCtxtSUReq) < 0) return -1;
    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeSetupListCtxtSUReq,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_E_RABToBeSetupListCtxtSUReq,
                          &e_RABToBeSetupListCtxtSUReq)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);

    /* Free any dynamic allocation that is no more used */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABToBeSetupListCtxtSUReq, &e_RABToBeSetupListCtxtSUReq);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UESecurityCapabilities,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_UESecurityCapabilities,
                          &s1ap_InitialContextSetupRequestIEs->ueSecurityCapabilities)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SecurityKey,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_SecurityKey,
                          &s1ap_InitialContextSetupRequestIEs->securityKey)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_TRACEACTIVATION_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TraceActivation,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_TraceActivation,
                            &s1ap_InitialContextSetupRequestIEs->traceActivation)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_HANDOVERRESTRICTIONLIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_HandoverRestrictionList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_HandoverRestrictionList,
                            &s1ap_InitialContextSetupRequestIEs->handoverRestrictionList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_UERADIOCAPABILITY_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UERadioCapability,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_UERadioCapability,
                            &s1ap_InitialContextSetupRequestIEs->ueRadioCapability)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_SUBSCRIBERPROFILEIDFORRFP_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SubscriberProfileIDforRFP,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_SubscriberProfileIDforRFP,
                            &s1ap_InitialContextSetupRequestIEs->subscriberProfileIDforRFP)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_CSFALLBACKINDICATOR_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSFallbackIndicator,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_CSFallbackIndicator,
                            &s1ap_InitialContextSetupRequestIEs->csFallbackIndicator)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_SRVCCOPERATIONPOSSIBLE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SRVCCOperationPossible,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_SRVCCOperationPossible,
                            &s1ap_InitialContextSetupRequestIEs->srvccOperationPossible)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSGMembershipStatus,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CSGMembershipStatus,
                            &s1ap_InitialContextSetupRequestIEs->csgMembershipStatus)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_REGISTEREDLAI_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_RegisteredLAI,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_LAI,
                            &s1ap_InitialContextSetupRequestIEs->registeredLAI)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_GUMMEI_ID_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_GUMMEI_ID,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_GUMMEI,
                            &s1ap_InitialContextSetupRequestIEs->gummei_id)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_MME_UE_S1AP_ID_2_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID_2,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_MME_UE_S1AP_ID,
                            &s1ap_InitialContextSetupRequestIEs->mme_ue_s1ap_id_2)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialContextSetupRequestIEs->presenceMask & S1AP_INITIALCONTEXTSETUPREQUESTIES_MANAGEMENTBASEDMDTALLOWED_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_ManagementBasedMDTAllowed,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_ManagementBasedMDTAllowed,
                            &s1ap_InitialContextSetupRequestIEs->managementBasedMDTAllowed)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupRequest->s1ap_InitialContextSetupRequest_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_locationreporties(
    S1ap_LocationReport_t *s1ap_LocationReport,
    S1ap_LocationReportIEs_t *s1ap_LocationReportIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_LocationReport != NULL);
    assert(s1ap_LocationReportIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_LocationReportIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_LocationReport->s1ap_LocationReport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_LocationReportIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_LocationReport->s1ap_LocationReport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_EUTRAN_CGI,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_EUTRAN_CGI,
                          &s1ap_LocationReportIEs->eutran_cgi)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_LocationReport->s1ap_LocationReport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TAI,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_TAI,
                          &s1ap_LocationReportIEs->tai)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_LocationReport->s1ap_LocationReport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_RequestType,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_RequestType,
                          &s1ap_LocationReportIEs->requestType)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_LocationReport->s1ap_LocationReport_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_uplinknastransport_ies(
    S1ap_UplinkNASTransport_t *s1ap_UplinkNASTransport,
    S1ap_UplinkNASTransport_IEs_t *s1ap_UplinkNASTransport_IEs) {

    S1ap_IE_t *ie;

    assert(s1ap_UplinkNASTransport != NULL);
    assert(s1ap_UplinkNASTransport_IEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_UplinkNASTransport_IEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkNASTransport->s1ap_UplinkNASTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_UplinkNASTransport_IEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkNASTransport->s1ap_UplinkNASTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_NAS_PDU,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_NAS_PDU,
                          &s1ap_UplinkNASTransport_IEs->nas_pdu)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkNASTransport->s1ap_UplinkNASTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_EUTRAN_CGI,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_EUTRAN_CGI,
                          &s1ap_UplinkNASTransport_IEs->eutran_cgi)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkNASTransport->s1ap_UplinkNASTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TAI,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_TAI,
                          &s1ap_UplinkNASTransport_IEs->tai)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkNASTransport->s1ap_UplinkNASTransport_ies.list, ie);

    /* Optional field */
    if (s1ap_UplinkNASTransport_IEs->presenceMask & S1AP_UPLINKNASTRANSPORT_IES_GW_S1AP_TRANSPORTLAYERADDRESS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_GW_S1ap_TransportLayerAddress,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_TransportLayerAddress,
                            &s1ap_UplinkNASTransport_IEs->gW_S1ap_TransportLayerAddress)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UplinkNASTransport->s1ap_UplinkNASTransport_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_uecontextmodificationresponseies(
    S1ap_UEContextModificationResponse_t *s1ap_UEContextModificationResponse,
    S1ap_UEContextModificationResponseIEs_t *s1ap_UEContextModificationResponseIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_UEContextModificationResponse != NULL);
    assert(s1ap_UEContextModificationResponseIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_UEContextModificationResponseIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextModificationResponse->s1ap_UEContextModificationResponse_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_UEContextModificationResponseIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextModificationResponse->s1ap_UEContextModificationResponse_ies.list, ie);

    /* Optional field */
    if (s1ap_UEContextModificationResponseIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_UEContextModificationResponseIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UEContextModificationResponse->s1ap_UEContextModificationResponse_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_uecontextmodificationrequesties(
    S1ap_UEContextModificationRequest_t *s1ap_UEContextModificationRequest,
    S1ap_UEContextModificationRequestIEs_t *s1ap_UEContextModificationRequestIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_UEContextModificationRequest != NULL);
    assert(s1ap_UEContextModificationRequestIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_UEContextModificationRequestIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextModificationRequest->s1ap_UEContextModificationRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_UEContextModificationRequestIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextModificationRequest->s1ap_UEContextModificationRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_SECURITYKEY_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SecurityKey,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_SecurityKey,
                            &s1ap_UEContextModificationRequestIEs->securityKey)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UEContextModificationRequest->s1ap_UEContextModificationRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_SUBSCRIBERPROFILEIDFORRFP_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SubscriberProfileIDforRFP,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_SubscriberProfileIDforRFP,
                            &s1ap_UEContextModificationRequestIEs->subscriberProfileIDforRFP)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UEContextModificationRequest->s1ap_UEContextModificationRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_UEAggregateMaximumBitrate,
                            &s1ap_UEContextModificationRequestIEs->uEaggregateMaximumBitrate)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UEContextModificationRequest->s1ap_UEContextModificationRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_CSFALLBACKINDICATOR_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSFallbackIndicator,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_CSFallbackIndicator,
                            &s1ap_UEContextModificationRequestIEs->csFallbackIndicator)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UEContextModificationRequest->s1ap_UEContextModificationRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_UESECURITYCAPABILITIES_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UESecurityCapabilities,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_UESecurityCapabilities,
                            &s1ap_UEContextModificationRequestIEs->ueSecurityCapabilities)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UEContextModificationRequest->s1ap_UEContextModificationRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSGMembershipStatus,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CSGMembershipStatus,
                            &s1ap_UEContextModificationRequestIEs->csgMembershipStatus)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UEContextModificationRequest->s1ap_UEContextModificationRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_UEContextModificationRequestIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONREQUESTIES_REGISTEREDLAI_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_RegisteredLAI,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_LAI,
                            &s1ap_UEContextModificationRequestIEs->registeredLAI)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UEContextModificationRequest->s1ap_UEContextModificationRequest_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_locationreportingcontrolies(
    S1ap_LocationReportingControl_t *s1ap_LocationReportingControl,
    S1ap_LocationReportingControlIEs_t *s1ap_LocationReportingControlIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_LocationReportingControl != NULL);
    assert(s1ap_LocationReportingControlIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_LocationReportingControlIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_LocationReportingControl->s1ap_LocationReportingControl_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_LocationReportingControlIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_LocationReportingControl->s1ap_LocationReportingControl_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_RequestType,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_RequestType,
                          &s1ap_LocationReportingControlIEs->requestType)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_LocationReportingControl->s1ap_LocationReportingControl_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_uplinks1cdma2000tunnelingies(
    S1ap_UplinkS1cdma2000tunneling_t *s1ap_UplinkS1cdma2000tunneling,
    S1ap_UplinkS1cdma2000tunnelingIEs_t *s1ap_UplinkS1cdma2000tunnelingIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_UplinkS1cdma2000tunneling != NULL);
    assert(s1ap_UplinkS1cdma2000tunnelingIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_UplinkS1cdma2000tunnelingIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkS1cdma2000tunneling->s1ap_UplinkS1cdma2000tunneling_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_UplinkS1cdma2000tunnelingIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkS1cdma2000tunneling->s1ap_UplinkS1cdma2000tunneling_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_cdma2000RATType,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Cdma2000RATType,
                          &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000RATType)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkS1cdma2000tunneling->s1ap_UplinkS1cdma2000tunneling_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_cdma2000SectorID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Cdma2000SectorID,
                          &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000SectorID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkS1cdma2000tunneling->s1ap_UplinkS1cdma2000tunneling_ies.list, ie);

    /* Optional field */
    if (s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask & S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000HOREQUIREDINDICATION_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_cdma2000HORequiredIndication,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_Cdma2000HORequiredIndication,
                            &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000HORequiredIndication)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UplinkS1cdma2000tunneling->s1ap_UplinkS1cdma2000tunneling_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask & S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000ONEXSRVCCINFO_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_cdma2000OneXSRVCCInfo,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_Cdma2000OneXSRVCCInfo,
                            &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000OneXSRVCCInfo)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UplinkS1cdma2000tunneling->s1ap_UplinkS1cdma2000tunneling_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask & S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000ONEXRAND_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_cdma2000OneXRAND,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_Cdma2000OneXRAND,
                            &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000OneXRAND)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UplinkS1cdma2000tunneling->s1ap_UplinkS1cdma2000tunneling_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_cdma2000PDU,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Cdma2000PDU,
                          &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000PDU)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkS1cdma2000tunneling->s1ap_UplinkS1cdma2000tunneling_ies.list, ie);

    /* Optional field */
    if (s1ap_UplinkS1cdma2000tunnelingIEs->presenceMask & S1AP_UPLINKS1CDMA2000TUNNELINGIES_EUTRANROUNDTRIPDELAYESTIMATIONINFO_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_EUTRANRoundTripDelayEstimationInfo,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_EUTRANRoundTripDelayEstimationInfo,
                            &s1ap_UplinkS1cdma2000tunnelingIEs->eutranRoundTripDelayEstimationInfo)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UplinkS1cdma2000tunneling->s1ap_UplinkS1cdma2000tunneling_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_handoverrequesties(
    S1ap_HandoverRequest_t *s1ap_HandoverRequest,
    S1ap_HandoverRequestIEs_t *s1ap_HandoverRequestIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_HandoverRequest != NULL);
    assert(s1ap_HandoverRequestIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_HandoverRequestIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_HandoverType,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_HandoverType,
                          &s1ap_HandoverRequestIEs->handoverType)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_HandoverRequestIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_UEAggregateMaximumBitrate,
                          &s1ap_HandoverRequestIEs->uEaggregateMaximumBitrate)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeSetupListHOReq,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_E_RABToBeSetupListHOReq,
                          &s1ap_HandoverRequestIEs->e_RABToBeSetupListHOReq)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Source_ToTarget_TransparentContainer,
                          &s1ap_HandoverRequestIEs->source_ToTarget_TransparentContainer)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UESecurityCapabilities,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_UESecurityCapabilities,
                          &s1ap_HandoverRequestIEs->ueSecurityCapabilities)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_HANDOVERRESTRICTIONLIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_HandoverRestrictionList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_HandoverRestrictionList,
                            &s1ap_HandoverRequestIEs->handoverRestrictionList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_TRACEACTIVATION_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TraceActivation,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_TraceActivation,
                            &s1ap_HandoverRequestIEs->traceActivation)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_REQUESTTYPE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_RequestType,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_RequestType,
                            &s1ap_HandoverRequestIEs->requestType)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_SRVCCOPERATIONPOSSIBLE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SRVCCOperationPossible,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_SRVCCOperationPossible,
                            &s1ap_HandoverRequestIEs->srvccOperationPossible)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SecurityContext,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_SecurityContext,
                          &s1ap_HandoverRequestIEs->securityContext)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_CSG_ID_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSG_Id,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_CSG_Id,
                            &s1ap_HandoverRequestIEs->csG_Id)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSGMembershipStatus,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CSGMembershipStatus,
                            &s1ap_HandoverRequestIEs->csgMembershipStatus)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_GUMMEI_ID_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_GUMMEI_ID,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_GUMMEI,
                            &s1ap_HandoverRequestIEs->gummei_id)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_MME_UE_S1AP_ID_2_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID_2,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_MME_UE_S1AP_ID,
                            &s1ap_HandoverRequestIEs->mme_ue_s1ap_id_2)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequestIEs->presenceMask & S1AP_HANDOVERREQUESTIES_MANAGEMENTBASEDMDTALLOWED_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_ManagementBasedMDTAllowed,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_ManagementBasedMDTAllowed,
                            &s1ap_HandoverRequestIEs->managementBasedMDTAllowed)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequest->s1ap_HandoverRequest_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_handoverrequiredies(
    S1ap_HandoverRequired_t *s1ap_HandoverRequired,
    S1ap_HandoverRequiredIEs_t *s1ap_HandoverRequiredIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_HandoverRequired != NULL);
    assert(s1ap_HandoverRequiredIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_HandoverRequiredIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_HandoverRequiredIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_HandoverType,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_HandoverType,
                          &s1ap_HandoverRequiredIEs->handoverType)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_HandoverRequiredIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TargetID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_TargetID,
                          &s1ap_HandoverRequiredIEs->targetID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);

    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_DIRECT_FORWARDING_PATH_AVAILABILITY_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Direct_Forwarding_Path_Availability,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_Direct_Forwarding_Path_Availability,
                            &s1ap_HandoverRequiredIEs->direct_Forwarding_Path_Availability)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_SRVCCHOINDICATION_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SRVCCHOIndication,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_SRVCCHOIndication,
                            &s1ap_HandoverRequiredIEs->srvcchoIndication)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Source_ToTarget_TransparentContainer,
                          &s1ap_HandoverRequiredIEs->source_ToTarget_TransparentContainer)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);

    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_SOURCE_TOTARGET_TRANSPARENTCONTAINER_SECONDARY_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer_Secondary,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_Source_ToTarget_TransparentContainer,
                            &s1ap_HandoverRequiredIEs->source_ToTarget_TransparentContainer_Secondary)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);
    }

    /* Conditional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_MSCLASSMARK2_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MSClassmark2,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_MSClassmark2,
                            &s1ap_HandoverRequiredIEs->msClassmark2)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);
    }

    /* Conditional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_MSCLASSMARK3_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MSClassmark3,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_MSClassmark3,
                            &s1ap_HandoverRequiredIEs->msClassmark3)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_CSG_ID_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSG_Id,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_CSG_Id,
                            &s1ap_HandoverRequiredIEs->csG_Id)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_CELLACCESSMODE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CellAccessMode,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_CellAccessMode,
                            &s1ap_HandoverRequiredIEs->cellAccessMode)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequiredIEs->presenceMask & S1AP_HANDOVERREQUIREDIES_PS_SERVICENOTAVAILABLE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_PS_ServiceNotAvailable,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_PS_ServiceNotAvailable,
                            &s1ap_HandoverRequiredIEs->pS_ServiceNotAvailable)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequired->s1ap_HandoverRequired_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_e_rabtobeswitcheddlitemies(
    S1ap_E_RABToBeSwitchedDLList_t *s1ap_E_RABToBeSwitchedDLList,
    S1ap_E_RABToBeSwitchedDLItemIEs_t *s1ap_E_RABToBeSwitchedDLItemIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABToBeSwitchedDLList != NULL);
    assert(s1ap_E_RABToBeSwitchedDLItemIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeSwitchedDLItem,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_E_RABToBeSwitchedDLItem,
                          &s1ap_E_RABToBeSwitchedDLItemIEs->e_RABToBeSwitchedDLItem)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABToBeSwitchedDLList->s1ap_E_RABToBeSwitchedDLList_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_mmeconfigurationupdateacknowledgeies(
    S1ap_MMEConfigurationUpdateAcknowledge_t *s1ap_MMEConfigurationUpdateAcknowledge,
    S1ap_MMEConfigurationUpdateAcknowledgeIEs_t *s1ap_MMEConfigurationUpdateAcknowledgeIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_MMEConfigurationUpdateAcknowledge != NULL);
    assert(s1ap_MMEConfigurationUpdateAcknowledgeIEs != NULL);

    /* Optional field */
    if (s1ap_MMEConfigurationUpdateAcknowledgeIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_MMEConfigurationUpdateAcknowledgeIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_MMEConfigurationUpdateAcknowledge->s1ap_MMEConfigurationUpdateAcknowledge_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_killresponseies(
    S1ap_KillResponse_t *s1ap_KillResponse,
    S1ap_KillResponseIEs_t *s1ap_KillResponseIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_KillResponse != NULL);
    assert(s1ap_KillResponseIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MessageIdentifier,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MessageIdentifier,
                          &s1ap_KillResponseIEs->messageIdentifier)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_KillResponse->s1ap_KillResponse_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SerialNumber,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_SerialNumber,
                          &s1ap_KillResponseIEs->serialNumber)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_KillResponse->s1ap_KillResponse_ies.list, ie);

    /* Optional field */
    if (s1ap_KillResponseIEs->presenceMask & S1AP_KILLRESPONSEIES_BROADCASTCANCELLEDAREALIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_BroadcastCancelledAreaList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_BroadcastCancelledAreaList,
                            &s1ap_KillResponseIEs->broadcastCancelledAreaList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_KillResponse->s1ap_KillResponse_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_KillResponseIEs->presenceMask & S1AP_KILLRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_KillResponseIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_KillResponse->s1ap_KillResponse_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_mmestatustransferies(
    S1ap_MMEStatusTransfer_t *s1ap_MMEStatusTransfer,
    S1ap_MMEStatusTransferIEs_t *s1ap_MMEStatusTransferIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_MMEStatusTransfer != NULL);
    assert(s1ap_MMEStatusTransferIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_MMEStatusTransferIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_MMEStatusTransfer->s1ap_MMEStatusTransfer_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_MMEStatusTransferIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_MMEStatusTransfer->s1ap_MMEStatusTransfer_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_StatusTransfer_TransparentContainer,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_StatusTransfer_TransparentContainer,
                          &s1ap_MMEStatusTransferIEs->eNB_StatusTransfer_TransparentContainer)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_MMEStatusTransfer->s1ap_MMEStatusTransfer_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_mmeconfigurationupdateies(
    S1ap_MMEConfigurationUpdate_t *s1ap_MMEConfigurationUpdate,
    S1ap_MMEConfigurationUpdateIEs_t *s1ap_MMEConfigurationUpdateIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_MMEConfigurationUpdate != NULL);
    assert(s1ap_MMEConfigurationUpdateIEs != NULL);

    /* Optional field */
    if (s1ap_MMEConfigurationUpdateIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEIES_MMENAME_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MMEname,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_MMEname,
                            &s1ap_MMEConfigurationUpdateIEs->mmEname)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_MMEConfigurationUpdate->s1ap_MMEConfigurationUpdate_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_MMEConfigurationUpdateIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEIES_SERVEDGUMMEIS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_ServedGUMMEIs,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_ServedGUMMEIs,
                            &s1ap_MMEConfigurationUpdateIEs->servedGUMMEIs)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_MMEConfigurationUpdate->s1ap_MMEConfigurationUpdate_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_MMEConfigurationUpdateIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEIES_RELATIVEMMECAPACITY_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_RelativeMMECapacity,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_RelativeMMECapacity,
                            &s1ap_MMEConfigurationUpdateIEs->relativeMMECapacity)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_MMEConfigurationUpdate->s1ap_MMEConfigurationUpdate_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_e_rabmodifyrequesties(
    S1ap_E_RABModifyRequest_t *s1ap_E_RABModifyRequest,
    S1ap_E_RABModifyRequestIEs_t *s1ap_E_RABModifyRequestIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABModifyRequest != NULL);
    assert(s1ap_E_RABModifyRequestIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_E_RABModifyRequestIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABModifyRequest->s1ap_E_RABModifyRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_E_RABModifyRequestIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABModifyRequest->s1ap_E_RABModifyRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_E_RABModifyRequestIEs->presenceMask & S1AP_E_RABMODIFYREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_UEAggregateMaximumBitrate,
                            &s1ap_E_RABModifyRequestIEs->uEaggregateMaximumBitrate)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABModifyRequest->s1ap_E_RABModifyRequest_ies.list, ie);
    }

    S1ap_E_RABToBeModifiedListBearerModReq_t e_RABToBeModifiedListBearerModReq;

    memset(&e_RABToBeModifiedListBearerModReq, 0, sizeof(S1ap_E_RABToBeModifiedListBearerModReq_t));

    if (s1ap_encode_s1ap_e_rabtobemodifiedlistbearermodreq(&e_RABToBeModifiedListBearerModReq, &s1ap_E_RABModifyRequestIEs->e_RABToBeModifiedListBearerModReq) < 0) return -1;
    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeModifiedListBearerModReq,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_E_RABToBeModifiedListBearerModReq,
                          &e_RABToBeModifiedListBearerModReq)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABModifyRequest->s1ap_E_RABModifyRequest_ies.list, ie);

    /* Free any dynamic allocation that is no more used */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABToBeModifiedListBearerModReq, &e_RABToBeModifiedListBearerModReq);

    return 0;
}

int s1ap_encode_s1ap_e_rabtobeswitchedulitemies(
    S1ap_E_RABToBeSwitchedULList_t *s1ap_E_RABToBeSwitchedULList,
    S1ap_E_RABToBeSwitchedULItemIEs_t *s1ap_E_RABToBeSwitchedULItemIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABToBeSwitchedULList != NULL);
    assert(s1ap_E_RABToBeSwitchedULItemIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeSwitchedULItem,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_E_RABToBeSwitchedULItem,
                          &s1ap_E_RABToBeSwitchedULItemIEs->e_RABToBeSwitchedULItem)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABToBeSwitchedULList->s1ap_E_RABToBeSwitchedULList_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_enbconfigurationupdatefailureies(
    S1ap_ENBConfigurationUpdateFailure_t *s1ap_ENBConfigurationUpdateFailure,
    S1ap_ENBConfigurationUpdateFailureIEs_t *s1ap_ENBConfigurationUpdateFailureIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_ENBConfigurationUpdateFailure != NULL);
    assert(s1ap_ENBConfigurationUpdateFailureIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_ENBConfigurationUpdateFailureIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_ENBConfigurationUpdateFailure->s1ap_ENBConfigurationUpdateFailure_ies.list, ie);

    /* Optional field */
    if (s1ap_ENBConfigurationUpdateFailureIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEFAILUREIES_TIMETOWAIT_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TimeToWait,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_TimeToWait,
                            &s1ap_ENBConfigurationUpdateFailureIEs->timeToWait)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ENBConfigurationUpdateFailure->s1ap_ENBConfigurationUpdateFailure_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_ENBConfigurationUpdateFailureIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_ENBConfigurationUpdateFailureIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ENBConfigurationUpdateFailure->s1ap_ENBConfigurationUpdateFailure_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_reseties(
    S1ap_Reset_t *s1ap_Reset,
    S1ap_ResetIEs_t *s1ap_ResetIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_Reset != NULL);
    assert(s1ap_ResetIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_ResetIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_Reset->s1ap_Reset_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_ResetType,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ResetType,
                          &s1ap_ResetIEs->resetType)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_Reset->s1ap_Reset_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_s1setupresponseies(
    S1ap_S1SetupResponse_t *s1ap_S1SetupResponse,
    S1ap_S1SetupResponseIEs_t *s1ap_S1SetupResponseIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_S1SetupResponse != NULL);
    assert(s1ap_S1SetupResponseIEs != NULL);

    /* Optional field */
    if (s1ap_S1SetupResponseIEs->presenceMask & S1AP_S1SETUPRESPONSEIES_MMENAME_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MMEname,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_MMEname,
                            &s1ap_S1SetupResponseIEs->mmEname)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_S1SetupResponse->s1ap_S1SetupResponse_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_ServedGUMMEIs,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ServedGUMMEIs,
                          &s1ap_S1SetupResponseIEs->servedGUMMEIs)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_S1SetupResponse->s1ap_S1SetupResponse_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_RelativeMMECapacity,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_RelativeMMECapacity,
                          &s1ap_S1SetupResponseIEs->relativeMMECapacity)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_S1SetupResponse->s1ap_S1SetupResponse_ies.list, ie);

    /* Optional field */
    if (s1ap_S1SetupResponseIEs->presenceMask & S1AP_S1SETUPRESPONSEIES_MMERELAYSUPPORTINDICATOR_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MMERelaySupportIndicator,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_MMERelaySupportIndicator,
                            &s1ap_S1SetupResponseIEs->mmeRelaySupportIndicator)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_S1SetupResponse->s1ap_S1SetupResponse_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_S1SetupResponseIEs->presenceMask & S1AP_S1SETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_S1SetupResponseIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_S1SetupResponse->s1ap_S1SetupResponse_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_pagingies(
    S1ap_Paging_t *s1ap_Paging,
    S1ap_PagingIEs_t *s1ap_PagingIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_Paging != NULL);
    assert(s1ap_PagingIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UEIdentityIndexValue,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_UEIdentityIndexValue,
                          &s1ap_PagingIEs->ueIdentityIndexValue)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_Paging->s1ap_Paging_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UEPagingID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_UEPagingID,
                          &s1ap_PagingIEs->uePagingID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_Paging->s1ap_Paging_ies.list, ie);

    /* Optional field */
    if (s1ap_PagingIEs->presenceMask & S1AP_PAGINGIES_PAGINGDRX_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_pagingDRX,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_PagingDRX,
                            &s1ap_PagingIEs->pagingDRX)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_Paging->s1ap_Paging_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CNDomain,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_CNDomain,
                          &s1ap_PagingIEs->cnDomain)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_Paging->s1ap_Paging_ies.list, ie);

    S1ap_TAIList_t taiList;

    memset(&taiList, 0, sizeof(S1ap_TAIList_t));

    if (s1ap_encode_s1ap_tailist(&taiList, &s1ap_PagingIEs->taiList) < 0) return -1;
    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TAIList,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_TAIList,
                          &taiList)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_Paging->s1ap_Paging_ies.list, ie);

    /* Free any dynamic allocation that is no more used */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TAIList, &taiList);

    /* Optional field */
    if (s1ap_PagingIEs->presenceMask & S1AP_PAGINGIES_CSG_IDLIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSG_IdList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CSG_IdList,
                            &s1ap_PagingIEs->csG_IdList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_Paging->s1ap_Paging_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_PagingIEs->presenceMask & S1AP_PAGINGIES_PAGINGPRIORITY_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_PagingPriority,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_PagingPriority,
                            &s1ap_PagingIEs->pagingPriority)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_Paging->s1ap_Paging_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_enbconfigurationupdateacknowledgeies(
    S1ap_ENBConfigurationUpdateAcknowledge_t *s1ap_ENBConfigurationUpdateAcknowledge,
    S1ap_ENBConfigurationUpdateAcknowledgeIEs_t *s1ap_ENBConfigurationUpdateAcknowledgeIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_ENBConfigurationUpdateAcknowledge != NULL);
    assert(s1ap_ENBConfigurationUpdateAcknowledgeIEs != NULL);

    /* Optional field */
    if (s1ap_ENBConfigurationUpdateAcknowledgeIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_ENBConfigurationUpdateAcknowledgeIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ENBConfigurationUpdateAcknowledge->s1ap_ENBConfigurationUpdateAcknowledge_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_e_rabreleasecommandies(
    S1ap_E_RABReleaseCommand_t *s1ap_E_RABReleaseCommand,
    S1ap_E_RABReleaseCommandIEs_t *s1ap_E_RABReleaseCommandIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABReleaseCommand != NULL);
    assert(s1ap_E_RABReleaseCommandIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_E_RABReleaseCommandIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseCommand->s1ap_E_RABReleaseCommand_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_E_RABReleaseCommandIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseCommand->s1ap_E_RABReleaseCommand_ies.list, ie);

    /* Optional field */
    if (s1ap_E_RABReleaseCommandIEs->presenceMask & S1AP_E_RABRELEASECOMMANDIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_UEAggregateMaximumBitrate,
                            &s1ap_E_RABReleaseCommandIEs->uEaggregateMaximumBitrate)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseCommand->s1ap_E_RABReleaseCommand_ies.list, ie);
    }

    S1ap_E_RABList_t e_RABToBeReleasedList;

    memset(&e_RABToBeReleasedList, 0, sizeof(S1ap_E_RABList_t));

    if (s1ap_encode_s1ap_e_rablist(&e_RABToBeReleasedList, &s1ap_E_RABReleaseCommandIEs->e_RABToBeReleasedList) < 0) return -1;
    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeReleasedList,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_E_RABList,
                          &e_RABToBeReleasedList)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseCommand->s1ap_E_RABReleaseCommand_ies.list, ie);

    /* Free any dynamic allocation that is no more used */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABList, &e_RABToBeReleasedList);

    /* Optional field */
    if (s1ap_E_RABReleaseCommandIEs->presenceMask & S1AP_E_RABRELEASECOMMANDIES_NAS_PDU_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_NAS_PDU,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_NAS_PDU,
                            &s1ap_E_RABReleaseCommandIEs->nas_pdu)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseCommand->s1ap_E_RABReleaseCommand_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_pathswitchrequestfailureies(
    S1ap_PathSwitchRequestFailure_t *s1ap_PathSwitchRequestFailure,
    S1ap_PathSwitchRequestFailureIEs_t *s1ap_PathSwitchRequestFailureIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_PathSwitchRequestFailure != NULL);
    assert(s1ap_PathSwitchRequestFailureIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_PathSwitchRequestFailureIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestFailure->s1ap_PathSwitchRequestFailure_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_PathSwitchRequestFailureIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestFailure->s1ap_PathSwitchRequestFailure_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_PathSwitchRequestFailureIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestFailure->s1ap_PathSwitchRequestFailure_ies.list, ie);

    /* Optional field */
    if (s1ap_PathSwitchRequestFailureIEs->presenceMask & S1AP_PATHSWITCHREQUESTFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_PathSwitchRequestFailureIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestFailure->s1ap_PathSwitchRequestFailure_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_e_rabmodifyresponseies(
    S1ap_E_RABModifyResponse_t *s1ap_E_RABModifyResponse,
    S1ap_E_RABModifyResponseIEs_t *s1ap_E_RABModifyResponseIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABModifyResponse != NULL);
    assert(s1ap_E_RABModifyResponseIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_E_RABModifyResponseIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABModifyResponse->s1ap_E_RABModifyResponse_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_E_RABModifyResponseIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABModifyResponse->s1ap_E_RABModifyResponse_ies.list, ie);

    /* Optional field */
    if (s1ap_E_RABModifyResponseIEs->presenceMask & S1AP_E_RABMODIFYRESPONSEIES_E_RABMODIFYLISTBEARERMODRES_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABModifyListBearerModRes,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABModifyListBearerModRes,
                            &s1ap_E_RABModifyResponseIEs->e_RABModifyListBearerModRes)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABModifyResponse->s1ap_E_RABModifyResponse_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_E_RABModifyResponseIEs->presenceMask & S1AP_E_RABMODIFYRESPONSEIES_E_RABFAILEDTOMODIFYLIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABFailedToModifyList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABList,
                            &s1ap_E_RABModifyResponseIEs->e_RABFailedToModifyList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABModifyResponse->s1ap_E_RABModifyResponse_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_E_RABModifyResponseIEs->presenceMask & S1AP_E_RABMODIFYRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_E_RABModifyResponseIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABModifyResponse->s1ap_E_RABModifyResponse_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_handovernotifyies(
    S1ap_HandoverNotify_t *s1ap_HandoverNotify,
    S1ap_HandoverNotifyIEs_t *s1ap_HandoverNotifyIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_HandoverNotify != NULL);
    assert(s1ap_HandoverNotifyIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_HandoverNotifyIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverNotify->s1ap_HandoverNotify_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_HandoverNotifyIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverNotify->s1ap_HandoverNotify_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_EUTRAN_CGI,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_EUTRAN_CGI,
                          &s1ap_HandoverNotifyIEs->eutran_cgi)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverNotify->s1ap_HandoverNotify_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TAI,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_TAI,
                          &s1ap_HandoverNotifyIEs->tai)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverNotify->s1ap_HandoverNotify_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_handovercancelacknowledgeies(
    S1ap_HandoverCancelAcknowledge_t *s1ap_HandoverCancelAcknowledge,
    S1ap_HandoverCancelAcknowledgeIEs_t *s1ap_HandoverCancelAcknowledgeIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_HandoverCancelAcknowledge != NULL);
    assert(s1ap_HandoverCancelAcknowledgeIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_HandoverCancelAcknowledgeIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverCancelAcknowledge->s1ap_HandoverCancelAcknowledge_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_HandoverCancelAcknowledgeIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverCancelAcknowledge->s1ap_HandoverCancelAcknowledge_ies.list, ie);

    /* Optional field */
    if (s1ap_HandoverCancelAcknowledgeIEs->presenceMask & S1AP_HANDOVERCANCELACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_HandoverCancelAcknowledgeIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverCancelAcknowledge->s1ap_HandoverCancelAcknowledge_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_initialcontextsetupfailureies(
    S1ap_InitialContextSetupFailure_t *s1ap_InitialContextSetupFailure,
    S1ap_InitialContextSetupFailureIEs_t *s1ap_InitialContextSetupFailureIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_InitialContextSetupFailure != NULL);
    assert(s1ap_InitialContextSetupFailureIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_InitialContextSetupFailureIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupFailure->s1ap_InitialContextSetupFailure_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_InitialContextSetupFailureIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupFailure->s1ap_InitialContextSetupFailure_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_InitialContextSetupFailureIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupFailure->s1ap_InitialContextSetupFailure_ies.list, ie);

    /* Optional field */
    if (s1ap_InitialContextSetupFailureIEs->presenceMask & S1AP_INITIALCONTEXTSETUPFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_InitialContextSetupFailureIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupFailure->s1ap_InitialContextSetupFailure_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_handovercommandies(
    S1ap_HandoverCommand_t *s1ap_HandoverCommand,
    S1ap_HandoverCommandIEs_t *s1ap_HandoverCommandIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_HandoverCommand != NULL);
    assert(s1ap_HandoverCommandIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_HandoverCommandIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverCommand->s1ap_HandoverCommand_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_HandoverCommandIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverCommand->s1ap_HandoverCommand_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_HandoverType,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_HandoverType,
                          &s1ap_HandoverCommandIEs->handoverType)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverCommand->s1ap_HandoverCommand_ies.list, ie);

    /* Optional field */
    if (s1ap_HandoverCommandIEs->presenceMask & S1AP_HANDOVERCOMMANDIES_E_RABSUBJECTTODATAFORWARDINGLIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABSubjecttoDataForwardingList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABSubjecttoDataForwardingList,
                            &s1ap_HandoverCommandIEs->e_RABSubjecttoDataForwardingList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverCommand->s1ap_HandoverCommand_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverCommandIEs->presenceMask & S1AP_HANDOVERCOMMANDIES_E_RABTORELEASELISTHOCMD_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABtoReleaseListHOCmd,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABList,
                            &s1ap_HandoverCommandIEs->e_RABtoReleaseListHOCmd)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverCommand->s1ap_HandoverCommand_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Target_ToSource_TransparentContainer,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Target_ToSource_TransparentContainer,
                          &s1ap_HandoverCommandIEs->target_ToSource_TransparentContainer)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverCommand->s1ap_HandoverCommand_ies.list, ie);

    /* Optional field */
    if (s1ap_HandoverCommandIEs->presenceMask & S1AP_HANDOVERCOMMANDIES_TARGET_TOSOURCE_TRANSPARENTCONTAINER_SECONDARY_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Target_ToSource_TransparentContainer_Secondary,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_Target_ToSource_TransparentContainer,
                            &s1ap_HandoverCommandIEs->target_ToSource_TransparentContainer_Secondary)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverCommand->s1ap_HandoverCommand_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverCommandIEs->presenceMask & S1AP_HANDOVERCOMMANDIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_HandoverCommandIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverCommand->s1ap_HandoverCommand_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_mmedirectinformationtransferies(
    S1ap_MMEDirectInformationTransfer_t *s1ap_MMEDirectInformationTransfer,
    S1ap_MMEDirectInformationTransferIEs_t *s1ap_MMEDirectInformationTransferIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_MMEDirectInformationTransfer != NULL);
    assert(s1ap_MMEDirectInformationTransferIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Inter_SystemInformationTransferTypeMDT,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Inter_SystemInformationTransferType,
                          &s1ap_MMEDirectInformationTransferIEs->inter_SystemInformationTransferTypeMDT)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_MMEDirectInformationTransfer->s1ap_MMEDirectInformationTransfer_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_errorindicationies(
    S1ap_ErrorIndication_t *s1ap_ErrorIndication,
    S1ap_ErrorIndicationIEs_t *s1ap_ErrorIndicationIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_ErrorIndication != NULL);
    assert(s1ap_ErrorIndicationIEs != NULL);

    /* Optional field */
    if (s1ap_ErrorIndicationIEs->presenceMask & S1AP_ERRORINDICATIONIES_MME_UE_S1AP_ID_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_MME_UE_S1AP_ID,
                            &s1ap_ErrorIndicationIEs->mme_ue_s1ap_id)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ErrorIndication->s1ap_ErrorIndication_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_ErrorIndicationIEs->presenceMask & S1AP_ERRORINDICATIONIES_ENB_UE_S1AP_ID_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                            &s1ap_ErrorIndicationIEs->eNB_UE_S1AP_ID)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ErrorIndication->s1ap_ErrorIndication_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_ErrorIndicationIEs->presenceMask & S1AP_ERRORINDICATIONIES_CAUSE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_Cause,
                            &s1ap_ErrorIndicationIEs->cause)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ErrorIndication->s1ap_ErrorIndication_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_ErrorIndicationIEs->presenceMask & S1AP_ERRORINDICATIONIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_ErrorIndicationIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ErrorIndication->s1ap_ErrorIndication_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_overloadstarties(
    S1ap_OverloadStart_t *s1ap_OverloadStart,
    S1ap_OverloadStartIEs_t *s1ap_OverloadStartIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_OverloadStart != NULL);
    assert(s1ap_OverloadStartIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_OverloadResponse,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_OverloadResponse,
                          &s1ap_OverloadStartIEs->overloadResponse)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_OverloadStart->s1ap_OverloadStart_ies.list, ie);

    /* Optional field */
    if (s1ap_OverloadStartIEs->presenceMask & S1AP_OVERLOADSTARTIES_GUMMEILIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_GUMMEIList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_GUMMEIList,
                            &s1ap_OverloadStartIEs->gummeiList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_OverloadStart->s1ap_OverloadStart_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_OverloadStartIEs->presenceMask & S1AP_OVERLOADSTARTIES_TRAFFICLOADREDUCTIONINDICATION_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TrafficLoadReductionIndication,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_TrafficLoadReductionIndication,
                            &s1ap_OverloadStartIEs->trafficLoadReductionIndication)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_OverloadStart->s1ap_OverloadStart_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_downlinknonueassociatedlppatransport_ies(
    S1ap_DownlinkNonUEAssociatedLPPaTransport_t *s1ap_DownlinkNonUEAssociatedLPPaTransport,
    S1ap_DownlinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs) {

    S1ap_IE_t *ie;

    assert(s1ap_DownlinkNonUEAssociatedLPPaTransport != NULL);
    assert(s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Routing_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Routing_ID,
                          &s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs->routing_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkNonUEAssociatedLPPaTransport->s1ap_DownlinkNonUEAssociatedLPPaTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_LPPa_PDU,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_LPPa_PDU,
                          &s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs->lpPa_PDU)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkNonUEAssociatedLPPaTransport->s1ap_DownlinkNonUEAssociatedLPPaTransport_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_handoverrequestacknowledgeies(
    S1ap_HandoverRequestAcknowledge_t *s1ap_HandoverRequestAcknowledge,
    S1ap_HandoverRequestAcknowledgeIEs_t *s1ap_HandoverRequestAcknowledgeIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_HandoverRequestAcknowledge != NULL);
    assert(s1ap_HandoverRequestAcknowledgeIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_HandoverRequestAcknowledgeIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequestAcknowledge->s1ap_HandoverRequestAcknowledge_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_HandoverRequestAcknowledgeIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequestAcknowledge->s1ap_HandoverRequestAcknowledge_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABAdmittedList,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_E_RABAdmittedList,
                          &s1ap_HandoverRequestAcknowledgeIEs->e_RABAdmittedList)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequestAcknowledge->s1ap_HandoverRequestAcknowledge_ies.list, ie);

    /* Optional field */
    if (s1ap_HandoverRequestAcknowledgeIEs->presenceMask & S1AP_HANDOVERREQUESTACKNOWLEDGEIES_E_RABFAILEDTOSETUPLISTHOREQACK_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABFailedToSetupListHOReqAck,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABFailedtoSetupListHOReqAck,
                            &s1ap_HandoverRequestAcknowledgeIEs->e_RABFailedToSetupListHOReqAck)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequestAcknowledge->s1ap_HandoverRequestAcknowledge_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Target_ToSource_TransparentContainer,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Target_ToSource_TransparentContainer,
                          &s1ap_HandoverRequestAcknowledgeIEs->target_ToSource_TransparentContainer)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverRequestAcknowledge->s1ap_HandoverRequestAcknowledge_ies.list, ie);

    /* Optional field */
    if (s1ap_HandoverRequestAcknowledgeIEs->presenceMask & S1AP_HANDOVERREQUESTACKNOWLEDGEIES_CSG_ID_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSG_Id,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CSG_Id,
                            &s1ap_HandoverRequestAcknowledgeIEs->csG_Id)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequestAcknowledge->s1ap_HandoverRequestAcknowledge_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_HandoverRequestAcknowledgeIEs->presenceMask & S1AP_HANDOVERREQUESTACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_HandoverRequestAcknowledgeIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverRequestAcknowledge->s1ap_HandoverRequestAcknowledge_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_downlinkueassociatedlppatransport_ies(
    S1ap_DownlinkUEAssociatedLPPaTransport_t *s1ap_DownlinkUEAssociatedLPPaTransport,
    S1ap_DownlinkUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkUEAssociatedLPPaTransport_IEs) {

    S1ap_IE_t *ie;

    assert(s1ap_DownlinkUEAssociatedLPPaTransport != NULL);
    assert(s1ap_DownlinkUEAssociatedLPPaTransport_IEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkUEAssociatedLPPaTransport->s1ap_DownlinkUEAssociatedLPPaTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkUEAssociatedLPPaTransport->s1ap_DownlinkUEAssociatedLPPaTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Routing_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Routing_ID,
                          &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->routing_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkUEAssociatedLPPaTransport->s1ap_DownlinkUEAssociatedLPPaTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_LPPa_PDU,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_LPPa_PDU,
                          &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->lpPa_PDU)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkUEAssociatedLPPaTransport->s1ap_DownlinkUEAssociatedLPPaTransport_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_uecontextmodificationfailureies(
    S1ap_UEContextModificationFailure_t *s1ap_UEContextModificationFailure,
    S1ap_UEContextModificationFailureIEs_t *s1ap_UEContextModificationFailureIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_UEContextModificationFailure != NULL);
    assert(s1ap_UEContextModificationFailureIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_UEContextModificationFailureIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextModificationFailure->s1ap_UEContextModificationFailure_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_UEContextModificationFailureIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextModificationFailure->s1ap_UEContextModificationFailure_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_UEContextModificationFailureIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextModificationFailure->s1ap_UEContextModificationFailure_ies.list, ie);

    /* Optional field */
    if (s1ap_UEContextModificationFailureIEs->presenceMask & S1AP_UECONTEXTMODIFICATIONFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_UEContextModificationFailureIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UEContextModificationFailure->s1ap_UEContextModificationFailure_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_mmeconfigurationtransferies(
    S1ap_MMEConfigurationTransfer_t *s1ap_MMEConfigurationTransfer,
    S1ap_MMEConfigurationTransferIEs_t *s1ap_MMEConfigurationTransferIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_MMEConfigurationTransfer != NULL);
    assert(s1ap_MMEConfigurationTransferIEs != NULL);

    /* Optional field */
    if (s1ap_MMEConfigurationTransferIEs->presenceMask & S1AP_MMECONFIGURATIONTRANSFERIES_SONCONFIGURATIONTRANSFERMCT_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SONConfigurationTransferMCT,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_SONConfigurationTransfer,
                            &s1ap_MMEConfigurationTransferIEs->sonConfigurationTransferMCT)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_MMEConfigurationTransfer->s1ap_MMEConfigurationTransfer_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_pathswitchrequestacknowledgeies(
    S1ap_PathSwitchRequestAcknowledge_t *s1ap_PathSwitchRequestAcknowledge,
    S1ap_PathSwitchRequestAcknowledgeIEs_t *s1ap_PathSwitchRequestAcknowledgeIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_PathSwitchRequestAcknowledge != NULL);
    assert(s1ap_PathSwitchRequestAcknowledgeIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_PathSwitchRequestAcknowledgeIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestAcknowledge->s1ap_PathSwitchRequestAcknowledge_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_PathSwitchRequestAcknowledgeIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestAcknowledge->s1ap_PathSwitchRequestAcknowledge_ies.list, ie);

    /* Optional field */
    if (s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask & S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_UEAggregateMaximumBitrate,
                            &s1ap_PathSwitchRequestAcknowledgeIEs->uEaggregateMaximumBitrate)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestAcknowledge->s1ap_PathSwitchRequestAcknowledge_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask & S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_E_RABTOBESWITCHEDULLIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeSwitchedULList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABToBeSwitchedULList,
                            &s1ap_PathSwitchRequestAcknowledgeIEs->e_RABToBeSwitchedULList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestAcknowledge->s1ap_PathSwitchRequestAcknowledge_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask & S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_E_RABTOBERELEASEDLIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeReleasedList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABList,
                            &s1ap_PathSwitchRequestAcknowledgeIEs->e_RABToBeReleasedList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestAcknowledge->s1ap_PathSwitchRequestAcknowledge_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SecurityContext,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_SecurityContext,
                          &s1ap_PathSwitchRequestAcknowledgeIEs->securityContext)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestAcknowledge->s1ap_PathSwitchRequestAcknowledge_ies.list, ie);

    /* Optional field */
    if (s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask & S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_PathSwitchRequestAcknowledgeIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestAcknowledge->s1ap_PathSwitchRequestAcknowledge_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_PathSwitchRequestAcknowledgeIEs->presenceMask & S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_MME_UE_S1AP_ID_2_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID_2,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_MME_UE_S1AP_ID,
                            &s1ap_PathSwitchRequestAcknowledgeIEs->mme_ue_s1ap_id_2)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequestAcknowledge->s1ap_PathSwitchRequestAcknowledge_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_s1setupfailureies(
    S1ap_S1SetupFailure_t *s1ap_S1SetupFailure,
    S1ap_S1SetupFailureIEs_t *s1ap_S1SetupFailureIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_S1SetupFailure != NULL);
    assert(s1ap_S1SetupFailureIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_S1SetupFailureIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_S1SetupFailure->s1ap_S1SetupFailure_ies.list, ie);

    /* Optional field */
    if (s1ap_S1SetupFailureIEs->presenceMask & S1AP_S1SETUPFAILUREIES_TIMETOWAIT_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TimeToWait,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_TimeToWait,
                            &s1ap_S1SetupFailureIEs->timeToWait)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_S1SetupFailure->s1ap_S1SetupFailure_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_S1SetupFailureIEs->presenceMask & S1AP_S1SETUPFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_S1SetupFailureIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_S1SetupFailure->s1ap_S1SetupFailure_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_mmeconfigurationupdatefailureies(
    S1ap_MMEConfigurationUpdateFailure_t *s1ap_MMEConfigurationUpdateFailure,
    S1ap_MMEConfigurationUpdateFailureIEs_t *s1ap_MMEConfigurationUpdateFailureIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_MMEConfigurationUpdateFailure != NULL);
    assert(s1ap_MMEConfigurationUpdateFailureIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_MMEConfigurationUpdateFailureIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_MMEConfigurationUpdateFailure->s1ap_MMEConfigurationUpdateFailure_ies.list, ie);

    /* Optional field */
    if (s1ap_MMEConfigurationUpdateFailureIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEFAILUREIES_TIMETOWAIT_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TimeToWait,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_TimeToWait,
                            &s1ap_MMEConfigurationUpdateFailureIEs->timeToWait)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_MMEConfigurationUpdateFailure->s1ap_MMEConfigurationUpdateFailure_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_MMEConfigurationUpdateFailureIEs->presenceMask & S1AP_MMECONFIGURATIONUPDATEFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_MMEConfigurationUpdateFailureIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_MMEConfigurationUpdateFailure->s1ap_MMEConfigurationUpdateFailure_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_initialcontextsetupresponseies(
    S1ap_InitialContextSetupResponse_t *s1ap_InitialContextSetupResponse,
    S1ap_InitialContextSetupResponseIEs_t *s1ap_InitialContextSetupResponseIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_InitialContextSetupResponse != NULL);
    assert(s1ap_InitialContextSetupResponseIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_InitialContextSetupResponseIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupResponse->s1ap_InitialContextSetupResponse_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_InitialContextSetupResponseIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupResponse->s1ap_InitialContextSetupResponse_ies.list, ie);

    S1ap_E_RABSetupListCtxtSURes_t e_RABSetupListCtxtSURes;

    memset(&e_RABSetupListCtxtSURes, 0, sizeof(S1ap_E_RABSetupListCtxtSURes_t));

    if (s1ap_encode_s1ap_e_rabsetuplistctxtsures(&e_RABSetupListCtxtSURes, &s1ap_InitialContextSetupResponseIEs->e_RABSetupListCtxtSURes) < 0) return -1;
    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABSetupListCtxtSURes,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_E_RABSetupListCtxtSURes,
                          &e_RABSetupListCtxtSURes)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupResponse->s1ap_InitialContextSetupResponse_ies.list, ie);

    /* Free any dynamic allocation that is no more used */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABSetupListCtxtSURes, &e_RABSetupListCtxtSURes);

    /* Optional field */
    if (s1ap_InitialContextSetupResponseIEs->presenceMask & S1AP_INITIALCONTEXTSETUPRESPONSEIES_E_RABFAILEDTOSETUPLISTCTXTSURES_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABFailedToSetupListCtxtSURes,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABList,
                            &s1ap_InitialContextSetupResponseIEs->e_RABFailedToSetupListCtxtSURes)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupResponse->s1ap_InitialContextSetupResponse_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_InitialContextSetupResponseIEs->presenceMask & S1AP_INITIALCONTEXTSETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_InitialContextSetupResponseIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_InitialContextSetupResponse->s1ap_InitialContextSetupResponse_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_downlinks1cdma2000tunnelingies(
    S1ap_DownlinkS1cdma2000tunneling_t *s1ap_DownlinkS1cdma2000tunneling,
    S1ap_DownlinkS1cdma2000tunnelingIEs_t *s1ap_DownlinkS1cdma2000tunnelingIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_DownlinkS1cdma2000tunneling != NULL);
    assert(s1ap_DownlinkS1cdma2000tunnelingIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_DownlinkS1cdma2000tunnelingIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkS1cdma2000tunneling->s1ap_DownlinkS1cdma2000tunneling_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_DownlinkS1cdma2000tunnelingIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkS1cdma2000tunneling->s1ap_DownlinkS1cdma2000tunneling_ies.list, ie);

    /* Optional field */
    if (s1ap_DownlinkS1cdma2000tunnelingIEs->presenceMask & S1AP_DOWNLINKS1CDMA2000TUNNELINGIES_E_RABSUBJECTTODATAFORWARDINGLIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABSubjecttoDataForwardingList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABSubjecttoDataForwardingList,
                            &s1ap_DownlinkS1cdma2000tunnelingIEs->e_RABSubjecttoDataForwardingList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_DownlinkS1cdma2000tunneling->s1ap_DownlinkS1cdma2000tunneling_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_DownlinkS1cdma2000tunnelingIEs->presenceMask & S1AP_DOWNLINKS1CDMA2000TUNNELINGIES_CDMA2000HOSTATUS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_cdma2000HOStatus,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_Cdma2000HOStatus,
                            &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000HOStatus)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_DownlinkS1cdma2000tunneling->s1ap_DownlinkS1cdma2000tunneling_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_cdma2000RATType,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Cdma2000RATType,
                          &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000RATType)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkS1cdma2000tunneling->s1ap_DownlinkS1cdma2000tunneling_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_cdma2000PDU,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Cdma2000PDU,
                          &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000PDU)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkS1cdma2000tunneling->s1ap_DownlinkS1cdma2000tunneling_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_overloadstopies(
    S1ap_OverloadStop_t *s1ap_OverloadStop,
    S1ap_OverloadStopIEs_t *s1ap_OverloadStopIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_OverloadStop != NULL);
    assert(s1ap_OverloadStopIEs != NULL);

    /* Optional field */
    if (s1ap_OverloadStopIEs->presenceMask & S1AP_OVERLOADSTOPIES_GUMMEILIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_GUMMEIList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_GUMMEIList,
                            &s1ap_OverloadStopIEs->gummeiList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_OverloadStop->s1ap_OverloadStop_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_enbconfigurationupdateies(
    S1ap_ENBConfigurationUpdate_t *s1ap_ENBConfigurationUpdate,
    S1ap_ENBConfigurationUpdateIEs_t *s1ap_ENBConfigurationUpdateIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_ENBConfigurationUpdate != NULL);
    assert(s1ap_ENBConfigurationUpdateIEs != NULL);

    /* Optional field */
    if (s1ap_ENBConfigurationUpdateIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEIES_ENBNAME_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNBname,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_ENBname,
                            &s1ap_ENBConfigurationUpdateIEs->eNBname)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ENBConfigurationUpdate->s1ap_ENBConfigurationUpdate_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_ENBConfigurationUpdateIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEIES_SUPPORTEDTAS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SupportedTAs,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_SupportedTAs,
                            &s1ap_ENBConfigurationUpdateIEs->supportedTAs)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ENBConfigurationUpdate->s1ap_ENBConfigurationUpdate_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_ENBConfigurationUpdateIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEIES_CSG_IDLIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSG_IdList,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_CSG_IdList,
                            &s1ap_ENBConfigurationUpdateIEs->csG_IdList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ENBConfigurationUpdate->s1ap_ENBConfigurationUpdate_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_ENBConfigurationUpdateIEs->presenceMask & S1AP_ENBCONFIGURATIONUPDATEIES_DEFAULTPAGINGDRX_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_DefaultPagingDRX,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_PagingDRX,
                            &s1ap_ENBConfigurationUpdateIEs->defaultPagingDRX)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ENBConfigurationUpdate->s1ap_ENBConfigurationUpdate_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_tracefailureindicationies(
    S1ap_TraceFailureIndication_t *s1ap_TraceFailureIndication,
    S1ap_TraceFailureIndicationIEs_t *s1ap_TraceFailureIndicationIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_TraceFailureIndication != NULL);
    assert(s1ap_TraceFailureIndicationIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_TraceFailureIndicationIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_TraceFailureIndication->s1ap_TraceFailureIndication_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_TraceFailureIndicationIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_TraceFailureIndication->s1ap_TraceFailureIndication_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_UTRAN_Trace_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_E_UTRAN_Trace_ID,
                          &s1ap_TraceFailureIndicationIEs->e_UTRAN_Trace_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_TraceFailureIndication->s1ap_TraceFailureIndication_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_TraceFailureIndicationIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_TraceFailureIndication->s1ap_TraceFailureIndication_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_enbconfigurationtransferies(
    S1ap_ENBConfigurationTransfer_t *s1ap_ENBConfigurationTransfer,
    S1ap_ENBConfigurationTransferIEs_t *s1ap_ENBConfigurationTransferIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_ENBConfigurationTransfer != NULL);
    assert(s1ap_ENBConfigurationTransferIEs != NULL);

    /* Optional field */
    if (s1ap_ENBConfigurationTransferIEs->presenceMask & S1AP_ENBCONFIGURATIONTRANSFERIES_SONCONFIGURATIONTRANSFERECT_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SONConfigurationTransferECT,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_SONConfigurationTransfer,
                            &s1ap_ENBConfigurationTransferIEs->sonConfigurationTransferECT)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ENBConfigurationTransfer->s1ap_ENBConfigurationTransfer_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_handoverfailureies(
    S1ap_HandoverFailure_t *s1ap_HandoverFailure,
    S1ap_HandoverFailureIEs_t *s1ap_HandoverFailureIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_HandoverFailure != NULL);
    assert(s1ap_HandoverFailureIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_HandoverFailureIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverFailure->s1ap_HandoverFailure_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_HandoverFailureIEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_HandoverFailure->s1ap_HandoverFailure_ies.list, ie);

    /* Optional field */
    if (s1ap_HandoverFailureIEs->presenceMask & S1AP_HANDOVERFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_HandoverFailureIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_HandoverFailure->s1ap_HandoverFailure_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_e_rabsetupresponseies(
    S1ap_E_RABSetupResponse_t *s1ap_E_RABSetupResponse,
    S1ap_E_RABSetupResponseIEs_t *s1ap_E_RABSetupResponseIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABSetupResponse != NULL);
    assert(s1ap_E_RABSetupResponseIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_E_RABSetupResponseIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABSetupResponse->s1ap_E_RABSetupResponse_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_E_RABSetupResponseIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABSetupResponse->s1ap_E_RABSetupResponse_ies.list, ie);

    /* Optional field */
    if (s1ap_E_RABSetupResponseIEs->presenceMask & S1AP_E_RABSETUPRESPONSEIES_E_RABSETUPLISTBEARERSURES_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABSetupListBearerSURes,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABSetupListBearerSURes,
                            &s1ap_E_RABSetupResponseIEs->e_RABSetupListBearerSURes)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABSetupResponse->s1ap_E_RABSetupResponse_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_E_RABSetupResponseIEs->presenceMask & S1AP_E_RABSETUPRESPONSEIES_E_RABFAILEDTOSETUPLISTBEARERSURES_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABFailedToSetupListBearerSURes,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABList,
                            &s1ap_E_RABSetupResponseIEs->e_RABFailedToSetupListBearerSURes)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABSetupResponse->s1ap_E_RABSetupResponse_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_E_RABSetupResponseIEs->presenceMask & S1AP_E_RABSETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_E_RABSetupResponseIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABSetupResponse->s1ap_E_RABSetupResponse_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_uecontextreleasecommand_ies(
    S1ap_UEContextReleaseCommand_t *s1ap_UEContextReleaseCommand,
    S1ap_UEContextReleaseCommand_IEs_t *s1ap_UEContextReleaseCommand_IEs) {

    S1ap_IE_t *ie;

    assert(s1ap_UEContextReleaseCommand != NULL);
    assert(s1ap_UEContextReleaseCommand_IEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UE_S1AP_IDs,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_UE_S1AP_IDs,
                          &s1ap_UEContextReleaseCommand_IEs->uE_S1AP_IDs)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextReleaseCommand->s1ap_UEContextReleaseCommand_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_UEContextReleaseCommand_IEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextReleaseCommand->s1ap_UEContextReleaseCommand_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_pathswitchrequesties(
    S1ap_PathSwitchRequest_t *s1ap_PathSwitchRequest,
    S1ap_PathSwitchRequestIEs_t *s1ap_PathSwitchRequestIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_PathSwitchRequest != NULL);
    assert(s1ap_PathSwitchRequestIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_PathSwitchRequestIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequest->s1ap_PathSwitchRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeSwitchedDLList,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_E_RABToBeSwitchedDLList,
                          &s1ap_PathSwitchRequestIEs->e_RABToBeSwitchedDLList)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequest->s1ap_PathSwitchRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SourceMME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_PathSwitchRequestIEs->sourceMME_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequest->s1ap_PathSwitchRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_EUTRAN_CGI,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_EUTRAN_CGI,
                          &s1ap_PathSwitchRequestIEs->eutran_cgi)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequest->s1ap_PathSwitchRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TAI,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_TAI,
                          &s1ap_PathSwitchRequestIEs->tai)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequest->s1ap_PathSwitchRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UESecurityCapabilities,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_UESecurityCapabilities,
                          &s1ap_PathSwitchRequestIEs->ueSecurityCapabilities)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequest->s1ap_PathSwitchRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_PathSwitchRequestIEs->presenceMask & S1AP_PATHSWITCHREQUESTIES_CSG_ID_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSG_Id,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CSG_Id,
                            &s1ap_PathSwitchRequestIEs->csG_Id)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequest->s1ap_PathSwitchRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_PathSwitchRequestIEs->presenceMask & S1AP_PATHSWITCHREQUESTIES_CELLACCESSMODE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CellAccessMode,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CellAccessMode,
                            &s1ap_PathSwitchRequestIEs->cellAccessMode)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequest->s1ap_PathSwitchRequest_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_PathSwitchRequestIEs->presenceMask & S1AP_PATHSWITCHREQUESTIES_SOURCEMME_S1AP_GUMMEI_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SourceMME_S1ap_GUMMEI,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_GUMMEI,
                            &s1ap_PathSwitchRequestIEs->sourceMME_S1ap_GUMMEI)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_PathSwitchRequest->s1ap_PathSwitchRequest_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_downlinknastransport_ies(
    S1ap_DownlinkNASTransport_t *s1ap_DownlinkNASTransport,
    S1ap_DownlinkNASTransport_IEs_t *s1ap_DownlinkNASTransport_IEs) {

    S1ap_IE_t *ie;

    assert(s1ap_DownlinkNASTransport != NULL);
    assert(s1ap_DownlinkNASTransport_IEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_DownlinkNASTransport_IEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkNASTransport->s1ap_DownlinkNASTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_DownlinkNASTransport_IEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkNASTransport->s1ap_DownlinkNASTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_NAS_PDU,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_NAS_PDU,
                          &s1ap_DownlinkNASTransport_IEs->nas_pdu)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_DownlinkNASTransport->s1ap_DownlinkNASTransport_ies.list, ie);

    /* Optional field */
    if (s1ap_DownlinkNASTransport_IEs->presenceMask & S1AP_DOWNLINKNASTRANSPORT_IES_HANDOVERRESTRICTIONLIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_HandoverRestrictionList,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_HandoverRestrictionList,
                            &s1ap_DownlinkNASTransport_IEs->handoverRestrictionList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_DownlinkNASTransport->s1ap_DownlinkNASTransport_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_DownlinkNASTransport_IEs->presenceMask & S1AP_DOWNLINKNASTRANSPORT_IES_SUBSCRIBERPROFILEIDFORRFP_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SubscriberProfileIDforRFP,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_SubscriberProfileIDforRFP,
                            &s1ap_DownlinkNASTransport_IEs->subscriberProfileIDforRFP)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_DownlinkNASTransport->s1ap_DownlinkNASTransport_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_e_rabreleaseindicationies(
    S1ap_E_RABReleaseIndication_t *s1ap_E_RABReleaseIndication,
    S1ap_E_RABReleaseIndicationIEs_t *s1ap_E_RABReleaseIndicationIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABReleaseIndication != NULL);
    assert(s1ap_E_RABReleaseIndicationIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_E_RABReleaseIndicationIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseIndication->s1ap_E_RABReleaseIndication_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_E_RABReleaseIndicationIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseIndication->s1ap_E_RABReleaseIndication_ies.list, ie);

    S1ap_E_RABList_t e_RABReleasedList;

    memset(&e_RABReleasedList, 0, sizeof(S1ap_E_RABList_t));

    if (s1ap_encode_s1ap_e_rablist(&e_RABReleasedList, &s1ap_E_RABReleaseIndicationIEs->e_RABReleasedList) < 0) return -1;
    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABReleasedList,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_E_RABList,
                          &e_RABReleasedList)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseIndication->s1ap_E_RABReleaseIndication_ies.list, ie);

    /* Free any dynamic allocation that is no more used */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABList, &e_RABReleasedList);

    return 0;
}

int s1ap_encode_s1ap_s1setuprequesties(
    S1ap_S1SetupRequest_t *s1ap_S1SetupRequest,
    S1ap_S1SetupRequestIEs_t *s1ap_S1SetupRequestIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_S1SetupRequest != NULL);
    assert(s1ap_S1SetupRequestIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Global_ENB_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Global_ENB_ID,
                          &s1ap_S1SetupRequestIEs->global_ENB_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_S1SetupRequest->s1ap_S1SetupRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_S1SetupRequestIEs->presenceMask & S1AP_S1SETUPREQUESTIES_ENBNAME_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNBname,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_ENBname,
                            &s1ap_S1SetupRequestIEs->eNBname)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_S1SetupRequest->s1ap_S1SetupRequest_ies.list, ie);
    }

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_SupportedTAs,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_SupportedTAs,
                          &s1ap_S1SetupRequestIEs->supportedTAs)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_S1SetupRequest->s1ap_S1SetupRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_DefaultPagingDRX,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_PagingDRX,
                          &s1ap_S1SetupRequestIEs->defaultPagingDRX)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_S1SetupRequest->s1ap_S1SetupRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_S1SetupRequestIEs->presenceMask & S1AP_S1SETUPREQUESTIES_CSG_IDLIST_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CSG_IdList,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_CSG_IdList,
                            &s1ap_S1SetupRequestIEs->csG_IdList)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_S1SetupRequest->s1ap_S1SetupRequest_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_nasnondeliveryindication_ies(
    S1ap_NASNonDeliveryIndication_t *s1ap_NASNonDeliveryIndication,
    S1ap_NASNonDeliveryIndication_IEs_t *s1ap_NASNonDeliveryIndication_IEs) {

    S1ap_IE_t *ie;

    assert(s1ap_NASNonDeliveryIndication != NULL);
    assert(s1ap_NASNonDeliveryIndication_IEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_NASNonDeliveryIndication_IEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_NASNonDeliveryIndication->s1ap_NASNonDeliveryIndication_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_NASNonDeliveryIndication_IEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_NASNonDeliveryIndication->s1ap_NASNonDeliveryIndication_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_NAS_PDU,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_NAS_PDU,
                          &s1ap_NASNonDeliveryIndication_IEs->nas_pdu)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_NASNonDeliveryIndication->s1ap_NASNonDeliveryIndication_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Cause,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_Cause,
                          &s1ap_NASNonDeliveryIndication_IEs->cause)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_NASNonDeliveryIndication->s1ap_NASNonDeliveryIndication_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_celltraffictraceies(
    S1ap_CellTrafficTrace_t *s1ap_CellTrafficTrace,
    S1ap_CellTrafficTraceIEs_t *s1ap_CellTrafficTraceIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_CellTrafficTrace != NULL);
    assert(s1ap_CellTrafficTraceIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_CellTrafficTraceIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_CellTrafficTrace->s1ap_CellTrafficTrace_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_CellTrafficTraceIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_CellTrafficTrace->s1ap_CellTrafficTrace_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_UTRAN_Trace_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_E_UTRAN_Trace_ID,
                          &s1ap_CellTrafficTraceIEs->e_UTRAN_Trace_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_CellTrafficTrace->s1ap_CellTrafficTrace_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_EUTRAN_CGI,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_EUTRAN_CGI,
                          &s1ap_CellTrafficTraceIEs->eutran_cgi)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_CellTrafficTrace->s1ap_CellTrafficTrace_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TraceCollectionEntityIPAddress,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_TransportLayerAddress,
                          &s1ap_CellTrafficTraceIEs->traceCollectionEntityIPAddress)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_CellTrafficTrace->s1ap_CellTrafficTrace_ies.list, ie);

    /* Optional field */
    if (s1ap_CellTrafficTraceIEs->presenceMask & S1AP_CELLTRAFFICTRACEIES_PRIVACYINDICATOR_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_PrivacyIndicator,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_PrivacyIndicator,
                            &s1ap_CellTrafficTraceIEs->privacyIndicator)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_CellTrafficTrace->s1ap_CellTrafficTrace_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_e_rabsetuprequesties(
    S1ap_E_RABSetupRequest_t *s1ap_E_RABSetupRequest,
    S1ap_E_RABSetupRequestIEs_t *s1ap_E_RABSetupRequestIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_E_RABSetupRequest != NULL);
    assert(s1ap_E_RABSetupRequestIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_E_RABSetupRequestIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABSetupRequest->s1ap_E_RABSetupRequest_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_E_RABSetupRequestIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABSetupRequest->s1ap_E_RABSetupRequest_ies.list, ie);

    /* Optional field */
    if (s1ap_E_RABSetupRequestIEs->presenceMask & S1AP_E_RABSETUPREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_uEaggregateMaximumBitrate,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_UEAggregateMaximumBitrate,
                            &s1ap_E_RABSetupRequestIEs->uEaggregateMaximumBitrate)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABSetupRequest->s1ap_E_RABSetupRequest_ies.list, ie);
    }

    S1ap_E_RABToBeSetupListBearerSUReq_t e_RABToBeSetupListBearerSUReq;

    memset(&e_RABToBeSetupListBearerSUReq, 0, sizeof(S1ap_E_RABToBeSetupListBearerSUReq_t));

    if (s1ap_encode_s1ap_e_rabtobesetuplistbearersureq(&e_RABToBeSetupListBearerSUReq, &s1ap_E_RABSetupRequestIEs->e_RABToBeSetupListBearerSUReq) < 0) return -1;
    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeSetupListBearerSUReq,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_E_RABToBeSetupListBearerSUReq,
                          &e_RABToBeSetupListBearerSUReq)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_E_RABSetupRequest->s1ap_E_RABSetupRequest_ies.list, ie);

    /* Free any dynamic allocation that is no more used */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_RABToBeSetupListBearerSUReq, &e_RABToBeSetupListBearerSUReq);

    return 0;
}

int s1ap_encode_s1ap_enbstatustransferies(
    S1ap_ENBStatusTransfer_t *s1ap_ENBStatusTransfer,
    S1ap_ENBStatusTransferIEs_t *s1ap_ENBStatusTransferIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_ENBStatusTransfer != NULL);
    assert(s1ap_ENBStatusTransferIEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_ENBStatusTransferIEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_ENBStatusTransfer->s1ap_ENBStatusTransfer_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_ENBStatusTransferIEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_ENBStatusTransfer->s1ap_ENBStatusTransfer_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_StatusTransfer_TransparentContainer,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_StatusTransfer_TransparentContainer,
                          &s1ap_ENBStatusTransferIEs->eNB_StatusTransfer_TransparentContainer)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_ENBStatusTransfer->s1ap_ENBStatusTransfer_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_resetacknowledgeies(
    S1ap_ResetAcknowledge_t *s1ap_ResetAcknowledge,
    S1ap_ResetAcknowledgeIEs_t *s1ap_ResetAcknowledgeIEs) {

    S1ap_IE_t *ie;

    assert(s1ap_ResetAcknowledge != NULL);
    assert(s1ap_ResetAcknowledgeIEs != NULL);

    /* Optional field */
    if (s1ap_ResetAcknowledgeIEs->presenceMask & S1AP_RESETACKNOWLEDGEIES_UE_ASSOCIATEDLOGICALS1_CONNECTIONLISTRESACK_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionListResAck,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionListResAck,
                            &s1ap_ResetAcknowledgeIEs->uE_associatedLogicalS1_ConnectionListResAck)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ResetAcknowledge->s1ap_ResetAcknowledge_ies.list, ie);
    }

    /* Optional field */
    if (s1ap_ResetAcknowledgeIEs->presenceMask & S1AP_RESETACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_ResetAcknowledgeIEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_ResetAcknowledge->s1ap_ResetAcknowledge_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_uecontextreleasecomplete_ies(
    S1ap_UEContextReleaseComplete_t *s1ap_UEContextReleaseComplete,
    S1ap_UEContextReleaseComplete_IEs_t *s1ap_UEContextReleaseComplete_IEs) {

    S1ap_IE_t *ie;

    assert(s1ap_UEContextReleaseComplete != NULL);
    assert(s1ap_UEContextReleaseComplete_IEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_UEContextReleaseComplete_IEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextReleaseComplete->s1ap_UEContextReleaseComplete_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_ignore,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_UEContextReleaseComplete_IEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UEContextReleaseComplete->s1ap_UEContextReleaseComplete_ies.list, ie);

    /* Optional field */
    if (s1ap_UEContextReleaseComplete_IEs->presenceMask & S1AP_UECONTEXTRELEASECOMPLETE_IES_CRITICALITYDIAGNOSTICS_PRESENT) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_CriticalityDiagnostics,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_CriticalityDiagnostics,
                            &s1ap_UEContextReleaseComplete_IEs->criticalityDiagnostics)) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UEContextReleaseComplete->s1ap_UEContextReleaseComplete_ies.list, ie);
    }

    return 0;
}

int s1ap_encode_s1ap_uplinkueassociatedlppatransport_ies(
    S1ap_UplinkUEAssociatedLPPaTransport_t *s1ap_UplinkUEAssociatedLPPaTransport,
    S1ap_UplinkUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkUEAssociatedLPPaTransport_IEs) {

    S1ap_IE_t *ie;

    assert(s1ap_UplinkUEAssociatedLPPaTransport != NULL);
    assert(s1ap_UplinkUEAssociatedLPPaTransport_IEs != NULL);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_MME_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_MME_UE_S1AP_ID,
                          &s1ap_UplinkUEAssociatedLPPaTransport_IEs->mme_ue_s1ap_id)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkUEAssociatedLPPaTransport->s1ap_UplinkUEAssociatedLPPaTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_eNB_UE_S1AP_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_ENB_UE_S1AP_ID,
                          &s1ap_UplinkUEAssociatedLPPaTransport_IEs->eNB_UE_S1AP_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkUEAssociatedLPPaTransport->s1ap_UplinkUEAssociatedLPPaTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Routing_ID,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_Routing_ID,
                          &s1ap_UplinkUEAssociatedLPPaTransport_IEs->routing_ID)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkUEAssociatedLPPaTransport->s1ap_UplinkUEAssociatedLPPaTransport_ies.list, ie);

    if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_LPPa_PDU,
                        S1ap_Criticality_reject,
                        &asn_DEF_S1ap_LPPa_PDU,
                          &s1ap_UplinkUEAssociatedLPPaTransport_IEs->lpPa_PDU)) == NULL) {
        return -1;
    }
    ASN_SEQUENCE_ADD(&s1ap_UplinkUEAssociatedLPPaTransport->s1ap_UplinkUEAssociatedLPPaTransport_ies.list, ie);

    return 0;
}

int s1ap_encode_s1ap_e_rabreleaselistbearerrelcomp(
    S1ap_E_RABReleaseListBearerRelComp_t *s1ap_E_RABReleaseListBearerRelComp,
    S1ap_E_RABReleaseListBearerRelCompIEs_t *s1ap_E_RABReleaseListBearerRelCompIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_E_RABReleaseListBearerRelComp != NULL);
    assert(s1ap_E_RABReleaseListBearerRelCompIEs != NULL);

    for (i = 0; i < s1ap_E_RABReleaseListBearerRelCompIEs->s1ap_E_RABReleaseItemBearerRelComp.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABReleaseItemBearerRelComp,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABReleaseItemBearerRelComp,
                            s1ap_E_RABReleaseListBearerRelCompIEs->s1ap_E_RABReleaseItemBearerRelComp.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABReleaseListBearerRelComp->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_e_rabtobemodifiedlistbearermodreq(
    S1ap_E_RABToBeModifiedListBearerModReq_t *s1ap_E_RABToBeModifiedListBearerModReq,
    S1ap_E_RABToBeModifiedListBearerModReqIEs_t *s1ap_E_RABToBeModifiedListBearerModReqIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_E_RABToBeModifiedListBearerModReq != NULL);
    assert(s1ap_E_RABToBeModifiedListBearerModReqIEs != NULL);

    for (i = 0; i < s1ap_E_RABToBeModifiedListBearerModReqIEs->s1ap_E_RABToBeModifiedItemBearerModReq.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeModifiedItemBearerModReq,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_E_RABToBeModifiedItemBearerModReq,
                            s1ap_E_RABToBeModifiedListBearerModReqIEs->s1ap_E_RABToBeModifiedItemBearerModReq.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABToBeModifiedListBearerModReq->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_e_rabtobesetuplistbearersureq(
    S1ap_E_RABToBeSetupListBearerSUReq_t *s1ap_E_RABToBeSetupListBearerSUReq,
    S1ap_E_RABToBeSetupListBearerSUReqIEs_t *s1ap_E_RABToBeSetupListBearerSUReqIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_E_RABToBeSetupListBearerSUReq != NULL);
    assert(s1ap_E_RABToBeSetupListBearerSUReqIEs != NULL);

    for (i = 0; i < s1ap_E_RABToBeSetupListBearerSUReqIEs->s1ap_E_RABToBeSetupItemBearerSUReq.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeSetupItemBearerSUReq,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_E_RABToBeSetupItemBearerSUReq,
                            s1ap_E_RABToBeSetupListBearerSUReqIEs->s1ap_E_RABToBeSetupItemBearerSUReq.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABToBeSetupListBearerSUReq->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_e_rabsetuplistctxtsures(
    S1ap_E_RABSetupListCtxtSURes_t *s1ap_E_RABSetupListCtxtSURes,
    S1ap_E_RABSetupListCtxtSUResIEs_t *s1ap_E_RABSetupListCtxtSUResIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_E_RABSetupListCtxtSURes != NULL);
    assert(s1ap_E_RABSetupListCtxtSUResIEs != NULL);

    for (i = 0; i < s1ap_E_RABSetupListCtxtSUResIEs->s1ap_E_RABSetupItemCtxtSURes.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABSetupItemCtxtSURes,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABSetupItemCtxtSURes,
                            s1ap_E_RABSetupListCtxtSUResIEs->s1ap_E_RABSetupItemCtxtSURes.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABSetupListCtxtSURes->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_bearers_subjecttostatustransferlist(
    S1ap_Bearers_SubjectToStatusTransfer_List_t *s1ap_Bearers_SubjectToStatusTransfer_List,
    S1ap_Bearers_SubjectToStatusTransferListIEs_t *s1ap_Bearers_SubjectToStatusTransferListIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_Bearers_SubjectToStatusTransfer_List != NULL);
    assert(s1ap_Bearers_SubjectToStatusTransferListIEs != NULL);

    for (i = 0; i < s1ap_Bearers_SubjectToStatusTransfer_ListIEs->s1ap_Bearers_SubjectToStatusTransfer_Item.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_Bearers_SubjectToStatusTransfer_Item,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_Bearers_SubjectToStatusTransfer_Item,
                            s1ap_Bearers_SubjectToStatusTransfer_ListIEs->s1ap_Bearers_SubjectToStatusTransfer_Item.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_Bearers_SubjectToStatusTransfer_List->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_e_rablist(
    S1ap_E_RABList_t *s1ap_E_RABList,
    S1ap_E_RABListIEs_t *s1ap_E_RABListIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_E_RABList != NULL);
    assert(s1ap_E_RABListIEs != NULL);

    for (i = 0; i < s1ap_E_RABListIEs->s1ap_E_RABItem.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABItem,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABItem,
                            s1ap_E_RABListIEs->s1ap_E_RABItem.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABList->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_tailist(
    S1ap_TAIList_t *s1ap_TAIList,
    S1ap_TAIListIEs_t *s1ap_TAIListIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_TAIList != NULL);
    assert(s1ap_TAIListIEs != NULL);

    for (i = 0; i < s1ap_TAIListIEs->s1ap_TAIItem.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_TAIItem,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_TAIItem,
                            s1ap_TAIListIEs->s1ap_TAIItem.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_TAIList->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_ue_associatedlogicals1_connectionlistres(
    S1ap_UE_associatedLogicalS1_ConnectionListRes_t *s1ap_UE_associatedLogicalS1_ConnectionListRes,
    S1ap_UE_associatedLogicalS1_ConnectionListResIEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_UE_associatedLogicalS1_ConnectionListRes != NULL);
    assert(s1ap_UE_associatedLogicalS1_ConnectionListResIEs != NULL);

    for (i = 0; i < s1ap_UE_associatedLogicalS1_ConnectionListResIEs->s1ap_UE_associatedLogicalS1_ConnectionItemRes.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionItem,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionItem,
                            s1ap_UE_associatedLogicalS1_ConnectionListResIEs->s1ap_UE_associatedLogicalS1_ConnectionItemRes.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UE_associatedLogicalS1_ConnectionListRes->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_e_rabsetuplistbearersures(
    S1ap_E_RABSetupListBearerSURes_t *s1ap_E_RABSetupListBearerSURes,
    S1ap_E_RABSetupListBearerSUResIEs_t *s1ap_E_RABSetupListBearerSUResIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_E_RABSetupListBearerSURes != NULL);
    assert(s1ap_E_RABSetupListBearerSUResIEs != NULL);

    for (i = 0; i < s1ap_E_RABSetupListBearerSUResIEs->s1ap_E_RABSetupItemBearerSURes.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABSetupItemBearerSURes,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABSetupItemBearerSURes,
                            s1ap_E_RABSetupListBearerSUResIEs->s1ap_E_RABSetupItemBearerSURes.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABSetupListBearerSURes->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_e_rabmodifylistbearermodres(
    S1ap_E_RABModifyListBearerModRes_t *s1ap_E_RABModifyListBearerModRes,
    S1ap_E_RABModifyListBearerModResIEs_t *s1ap_E_RABModifyListBearerModResIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_E_RABModifyListBearerModRes != NULL);
    assert(s1ap_E_RABModifyListBearerModResIEs != NULL);

    for (i = 0; i < s1ap_E_RABModifyListBearerModResIEs->s1ap_E_RABModifyItemBearerModRes.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABModifyItemBearerModRes,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABModifyItemBearerModRes,
                            s1ap_E_RABModifyListBearerModResIEs->s1ap_E_RABModifyItemBearerModRes.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABModifyListBearerModRes->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_e_rabtobesetuplistctxtsureq(
    S1ap_E_RABToBeSetupListCtxtSUReq_t *s1ap_E_RABToBeSetupListCtxtSUReq,
    S1ap_E_RABToBeSetupListCtxtSUReqIEs_t *s1ap_E_RABToBeSetupListCtxtSUReqIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_E_RABToBeSetupListCtxtSUReq != NULL);
    assert(s1ap_E_RABToBeSetupListCtxtSUReqIEs != NULL);

    for (i = 0; i < s1ap_E_RABToBeSetupListCtxtSUReqIEs->s1ap_E_RABToBeSetupItemCtxtSUReq.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABToBeSetupItemCtxtSUReq,
                            S1ap_Criticality_reject,
                            &asn_DEF_S1ap_E_RABToBeSetupItemCtxtSUReq,
                            s1ap_E_RABToBeSetupListCtxtSUReqIEs->s1ap_E_RABToBeSetupItemCtxtSUReq.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABToBeSetupListCtxtSUReq->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_e_rabinformationlist(
    S1ap_E_RABInformationList_t *s1ap_E_RABInformationList,
    S1ap_E_RABInformationListIEs_t *s1ap_E_RABInformationListIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_E_RABInformationList != NULL);
    assert(s1ap_E_RABInformationListIEs != NULL);

    for (i = 0; i < s1ap_E_RABInformationListIEs->s1ap_E_RABInformationList.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_E_RABInformationListItem,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_E_RABInformationListItem,
                            s1ap_E_RABInformationListIEs->s1ap_E_RABInformationList.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_E_RABInformationList->list, ie);
    }
    return 0;
}

int s1ap_encode_s1ap_ue_associatedlogicals1_connectionlistresack(
    S1ap_UE_associatedLogicalS1_ConnectionListResAck_t *s1ap_UE_associatedLogicalS1_ConnectionListResAck,
    S1ap_UE_associatedLogicalS1_ConnectionListResAckIEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs) {

    int i;
    S1ap_IE_t *ie;

    assert(s1ap_UE_associatedLogicalS1_ConnectionListResAck != NULL);
    assert(s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs != NULL);

    for (i = 0; i < s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs->s1ap_UE_associatedLogicalS1_ConnectionItemResAck.count; i++) {
        if ((ie = s1ap_new_ie(S1ap_ProtocolIE_ID_id_UE_associatedLogicalS1_ConnectionItem,
                            S1ap_Criticality_ignore,
                            &asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionItem,
                            s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs->s1ap_UE_associatedLogicalS1_ConnectionItemResAck.array[i])) == NULL) {
            return -1;
        }
        ASN_SEQUENCE_ADD(&s1ap_UE_associatedLogicalS1_ConnectionListResAck->list, ie);
    }
    return 0;
}

