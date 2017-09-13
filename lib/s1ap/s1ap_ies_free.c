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
 * Created on: 2017-09-13 23:14:37.604451 by acetcom
 * from ['S1AP-PDU.asn']
 ******************************************************************************/
#define TRACE_MODULE _s1ap_ies_free
#include "core_debug.h"
#include "s1ap_ies_defs.h"

void s1ap_free_s1ap_deactivatetraceies(
    S1ap_DeactivateTraceIEs_t *s1ap_DeactivateTraceIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_DeactivateTraceIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_DeactivateTraceIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_UTRAN_Trace_ID, &s1ap_DeactivateTraceIEs->e_UTRAN_Trace_ID);
}

void s1ap_free_s1ap_enbdirectinformationtransferies(
    S1ap_ENBDirectInformationTransferIEs_t *s1ap_ENBDirectInformationTransferIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Inter_SystemInformationTransferType, &s1ap_ENBDirectInformationTransferIEs->inter_SystemInformationTransferTypeEDT);
}

void s1ap_free_s1ap_e_rabreleaseresponseies(
    S1ap_E_RABReleaseResponseIEs_t *s1ap_E_RABReleaseResponseIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABReleaseResponseIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABReleaseResponseIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    s1ap_free_s1ap_e_rabreleaselistbearerrelcomp(&s1ap_E_RABReleaseResponseIEs->e_RABReleaseListBearerRelComp);
            /* Optional field */
    s1ap_free_s1ap_e_rablist(&s1ap_E_RABReleaseResponseIEs->e_RABFailedToReleaseList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_E_RABReleaseResponseIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_uplinknonueassociatedlppatransport_ies(
    S1ap_UplinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkNonUEAssociatedLPPaTransport_IEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Routing_ID, &s1ap_UplinkNonUEAssociatedLPPaTransport_IEs->routing_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LPPa_PDU, &s1ap_UplinkNonUEAssociatedLPPaTransport_IEs->lpPa_PDU);
}

void s1ap_free_s1ap_tracestarties(
    S1ap_TraceStartIEs_t *s1ap_TraceStartIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_TraceStartIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_TraceStartIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceActivation, &s1ap_TraceStartIEs->traceActivation);
}

void s1ap_free_s1ap_writereplacewarningresponseies(
    S1ap_WriteReplaceWarningResponseIEs_t *s1ap_WriteReplaceWarningResponseIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MessageIdentifier, &s1ap_WriteReplaceWarningResponseIEs->messageIdentifier);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SerialNumber, &s1ap_WriteReplaceWarningResponseIEs->serialNumber);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_BroadcastCompletedAreaList, &s1ap_WriteReplaceWarningResponseIEs->broadcastCompletedAreaList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_WriteReplaceWarningResponseIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_handovercancelies(
    S1ap_HandoverCancelIEs_t *s1ap_HandoverCancelIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverCancelIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverCancelIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_HandoverCancelIEs->cause);
}

void s1ap_free_s1ap_handoverpreparationfailureies(
    S1ap_HandoverPreparationFailureIEs_t *s1ap_HandoverPreparationFailureIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverPreparationFailureIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverPreparationFailureIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_HandoverPreparationFailureIEs->cause);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_HandoverPreparationFailureIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_uecontextreleaserequest_ies(
    S1ap_UEContextReleaseRequest_IEs_t *s1ap_UEContextReleaseRequest_IEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UEContextReleaseRequest_IEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UEContextReleaseRequest_IEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_UEContextReleaseRequest_IEs->cause);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_GWContextReleaseIndication, &s1ap_UEContextReleaseRequest_IEs->gwContextReleaseIndication);
}

void s1ap_free_s1ap_initialuemessage_ies(
    S1ap_InitialUEMessage_IEs_t *s1ap_InitialUEMessage_IEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_InitialUEMessage_IEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NAS_PDU, &s1ap_InitialUEMessage_IEs->nas_pdu);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TAI, &s1ap_InitialUEMessage_IEs->tai);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_EUTRAN_CGI, &s1ap_InitialUEMessage_IEs->eutran_cgi);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_RRC_Establishment_Cause, &s1ap_InitialUEMessage_IEs->rrC_Establishment_Cause);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_S_TMSI, &s1ap_InitialUEMessage_IEs->s_tmsi);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSG_Id, &s1ap_InitialUEMessage_IEs->csG_Id);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_GUMMEI, &s1ap_InitialUEMessage_IEs->gummei_id);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellAccessMode, &s1ap_InitialUEMessage_IEs->cellAccessMode);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TransportLayerAddress, &s1ap_InitialUEMessage_IEs->gW_TransportLayerAddress);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_RelayNode_Indicator, &s1ap_InitialUEMessage_IEs->relayNode_Indicator);
}

void s1ap_free_s1ap_killrequesties(
    S1ap_KillRequestIEs_t *s1ap_KillRequestIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MessageIdentifier, &s1ap_KillRequestIEs->messageIdentifier);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SerialNumber, &s1ap_KillRequestIEs->serialNumber);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WarningAreaList, &s1ap_KillRequestIEs->warningAreaList);
}

void s1ap_free_s1ap_writereplacewarningrequesties(
    S1ap_WriteReplaceWarningRequestIEs_t *s1ap_WriteReplaceWarningRequestIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MessageIdentifier, &s1ap_WriteReplaceWarningRequestIEs->messageIdentifier);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SerialNumber, &s1ap_WriteReplaceWarningRequestIEs->serialNumber);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WarningAreaList, &s1ap_WriteReplaceWarningRequestIEs->warningAreaList);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_RepetitionPeriod, &s1ap_WriteReplaceWarningRequestIEs->repetitionPeriod);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ExtendedRepetitionPeriod, &s1ap_WriteReplaceWarningRequestIEs->extendedRepetitionPeriod);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NumberofBroadcastRequest, &s1ap_WriteReplaceWarningRequestIEs->numberofBroadcastRequest);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WarningType, &s1ap_WriteReplaceWarningRequestIEs->warningType);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WarningSecurityInfo, &s1ap_WriteReplaceWarningRequestIEs->warningSecurityInfo);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_DataCodingScheme, &s1ap_WriteReplaceWarningRequestIEs->dataCodingScheme);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_WarningMessageContents, &s1ap_WriteReplaceWarningRequestIEs->warningMessageContents);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ConcurrentWarningMessageIndicator, &s1ap_WriteReplaceWarningRequestIEs->concurrentWarningMessageIndicator);
}

void s1ap_free_s1ap_uecapabilityinfoindicationies(
    S1ap_UECapabilityInfoIndicationIEs_t *s1ap_UECapabilityInfoIndicationIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UECapabilityInfoIndicationIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UECapabilityInfoIndicationIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UERadioCapability, &s1ap_UECapabilityInfoIndicationIEs->ueRadioCapability);
}

void s1ap_free_s1ap_locationreportingfailureindicationies(
    S1ap_LocationReportingFailureIndicationIEs_t *s1ap_LocationReportingFailureIndicationIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_LocationReportingFailureIndicationIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_LocationReportingFailureIndicationIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_LocationReportingFailureIndicationIEs->cause);
}

void s1ap_free_s1ap_initialcontextsetuprequesties(
    S1ap_InitialContextSetupRequestIEs_t *s1ap_InitialContextSetupRequestIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_InitialContextSetupRequestIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_InitialContextSetupRequestIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_InitialContextSetupRequestIEs->uEaggregateMaximumBitrate);
    s1ap_free_s1ap_e_rabtobesetuplistctxtsureq(&s1ap_InitialContextSetupRequestIEs->e_RABToBeSetupListCtxtSUReq);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UESecurityCapabilities, &s1ap_InitialContextSetupRequestIEs->ueSecurityCapabilities);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SecurityKey, &s1ap_InitialContextSetupRequestIEs->securityKey);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceActivation, &s1ap_InitialContextSetupRequestIEs->traceActivation);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRestrictionList, &s1ap_InitialContextSetupRequestIEs->handoverRestrictionList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UERadioCapability, &s1ap_InitialContextSetupRequestIEs->ueRadioCapability);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SubscriberProfileIDforRFP, &s1ap_InitialContextSetupRequestIEs->subscriberProfileIDforRFP);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSFallbackIndicator, &s1ap_InitialContextSetupRequestIEs->csFallbackIndicator);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SRVCCOperationPossible, &s1ap_InitialContextSetupRequestIEs->srvccOperationPossible);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSGMembershipStatus, &s1ap_InitialContextSetupRequestIEs->csgMembershipStatus);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LAI, &s1ap_InitialContextSetupRequestIEs->registeredLAI);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_GUMMEI, &s1ap_InitialContextSetupRequestIEs->gummei_id);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_InitialContextSetupRequestIEs->mme_ue_s1ap_id_2);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ManagementBasedMDTAllowed, &s1ap_InitialContextSetupRequestIEs->managementBasedMDTAllowed);
}

void s1ap_free_s1ap_locationreporties(
    S1ap_LocationReportIEs_t *s1ap_LocationReportIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_LocationReportIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_LocationReportIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_EUTRAN_CGI, &s1ap_LocationReportIEs->eutran_cgi);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TAI, &s1ap_LocationReportIEs->tai);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_RequestType, &s1ap_LocationReportIEs->requestType);
}

void s1ap_free_s1ap_uplinknastransport_ies(
    S1ap_UplinkNASTransport_IEs_t *s1ap_UplinkNASTransport_IEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UplinkNASTransport_IEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UplinkNASTransport_IEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NAS_PDU, &s1ap_UplinkNASTransport_IEs->nas_pdu);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_EUTRAN_CGI, &s1ap_UplinkNASTransport_IEs->eutran_cgi);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TAI, &s1ap_UplinkNASTransport_IEs->tai);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TransportLayerAddress, &s1ap_UplinkNASTransport_IEs->gW_TransportLayerAddress);
}

void s1ap_free_s1ap_uecontextmodificationresponseies(
    S1ap_UEContextModificationResponseIEs_t *s1ap_UEContextModificationResponseIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UEContextModificationResponseIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UEContextModificationResponseIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_UEContextModificationResponseIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_uecontextmodificationrequesties(
    S1ap_UEContextModificationRequestIEs_t *s1ap_UEContextModificationRequestIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UEContextModificationRequestIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UEContextModificationRequestIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SecurityKey, &s1ap_UEContextModificationRequestIEs->securityKey);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SubscriberProfileIDforRFP, &s1ap_UEContextModificationRequestIEs->subscriberProfileIDforRFP);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_UEContextModificationRequestIEs->uEaggregateMaximumBitrate);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSFallbackIndicator, &s1ap_UEContextModificationRequestIEs->csFallbackIndicator);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UESecurityCapabilities, &s1ap_UEContextModificationRequestIEs->ueSecurityCapabilities);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSGMembershipStatus, &s1ap_UEContextModificationRequestIEs->csgMembershipStatus);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LAI, &s1ap_UEContextModificationRequestIEs->registeredLAI);
}

void s1ap_free_s1ap_locationreportingcontrolies(
    S1ap_LocationReportingControlIEs_t *s1ap_LocationReportingControlIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_LocationReportingControlIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_LocationReportingControlIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_RequestType, &s1ap_LocationReportingControlIEs->requestType);
}

void s1ap_free_s1ap_uplinks1cdma2000tunnelingies(
    S1ap_UplinkS1cdma2000tunnelingIEs_t *s1ap_UplinkS1cdma2000tunnelingIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UplinkS1cdma2000tunnelingIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UplinkS1cdma2000tunnelingIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cdma2000RATType, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000RATType);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cdma2000SectorID, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000SectorID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cdma2000HORequiredIndication, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000HORequiredIndication);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cdma2000OneXSRVCCInfo, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000OneXSRVCCInfo);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cdma2000OneXRAND, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000OneXRAND);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cdma2000PDU, &s1ap_UplinkS1cdma2000tunnelingIEs->cdma2000PDU);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_EUTRANRoundTripDelayEstimationInfo, &s1ap_UplinkS1cdma2000tunnelingIEs->eutranRoundTripDelayEstimationInfo);
}

void s1ap_free_s1ap_handoverrequesties(
    S1ap_HandoverRequestIEs_t *s1ap_HandoverRequestIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverRequestIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverType, &s1ap_HandoverRequestIEs->handoverType);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_HandoverRequestIEs->cause);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_HandoverRequestIEs->uEaggregateMaximumBitrate);
    s1ap_free_s1ap_e_rabtobesetuplisthoreq(&s1ap_HandoverRequestIEs->e_RABToBeSetupListHOReq);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Source_ToTarget_TransparentContainer, &s1ap_HandoverRequestIEs->source_ToTarget_TransparentContainer);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UESecurityCapabilities, &s1ap_HandoverRequestIEs->ueSecurityCapabilities);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRestrictionList, &s1ap_HandoverRequestIEs->handoverRestrictionList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TraceActivation, &s1ap_HandoverRequestIEs->traceActivation);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_RequestType, &s1ap_HandoverRequestIEs->requestType);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SRVCCOperationPossible, &s1ap_HandoverRequestIEs->srvccOperationPossible);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SecurityContext, &s1ap_HandoverRequestIEs->securityContext);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSG_Id, &s1ap_HandoverRequestIEs->csG_Id);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSGMembershipStatus, &s1ap_HandoverRequestIEs->csgMembershipStatus);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_GUMMEI, &s1ap_HandoverRequestIEs->gummei_id);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverRequestIEs->mme_ue_s1ap_id_2);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ManagementBasedMDTAllowed, &s1ap_HandoverRequestIEs->managementBasedMDTAllowed);
}

void s1ap_free_s1ap_handoverrequiredies(
    S1ap_HandoverRequiredIEs_t *s1ap_HandoverRequiredIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverRequiredIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverRequiredIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverType, &s1ap_HandoverRequiredIEs->handoverType);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_HandoverRequiredIEs->cause);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TargetID, &s1ap_HandoverRequiredIEs->targetID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Direct_Forwarding_Path_Availability, &s1ap_HandoverRequiredIEs->direct_Forwarding_Path_Availability);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SRVCCHOIndication, &s1ap_HandoverRequiredIEs->srvcchoIndication);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Source_ToTarget_TransparentContainer, &s1ap_HandoverRequiredIEs->source_ToTarget_TransparentContainer);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Source_ToTarget_TransparentContainer, &s1ap_HandoverRequiredIEs->source_ToTarget_TransparentContainer_Secondary);
            /* Conditional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MSClassmark2, &s1ap_HandoverRequiredIEs->msClassmark2);
            /* Conditional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MSClassmark3, &s1ap_HandoverRequiredIEs->msClassmark3);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSG_Id, &s1ap_HandoverRequiredIEs->csG_Id);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellAccessMode, &s1ap_HandoverRequiredIEs->cellAccessMode);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PS_ServiceNotAvailable, &s1ap_HandoverRequiredIEs->pS_ServiceNotAvailable);
}

void s1ap_free_s1ap_mmeconfigurationupdateacknowledgeies(
    S1ap_MMEConfigurationUpdateAcknowledgeIEs_t *s1ap_MMEConfigurationUpdateAcknowledgeIEs)
{

            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_MMEConfigurationUpdateAcknowledgeIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_killresponseies(
    S1ap_KillResponseIEs_t *s1ap_KillResponseIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MessageIdentifier, &s1ap_KillResponseIEs->messageIdentifier);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SerialNumber, &s1ap_KillResponseIEs->serialNumber);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_BroadcastCancelledAreaList, &s1ap_KillResponseIEs->broadcastCancelledAreaList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_KillResponseIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_mmestatustransferies(
    S1ap_MMEStatusTransferIEs_t *s1ap_MMEStatusTransferIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_MMEStatusTransferIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_MMEStatusTransferIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_StatusTransfer_TransparentContainer, &s1ap_MMEStatusTransferIEs->eNB_StatusTransfer_TransparentContainer);
}

void s1ap_free_s1ap_mmeconfigurationupdateies(
    S1ap_MMEConfigurationUpdateIEs_t *s1ap_MMEConfigurationUpdateIEs)
{

            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEname, &s1ap_MMEConfigurationUpdateIEs->mmEname);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ServedGUMMEIs, &s1ap_MMEConfigurationUpdateIEs->servedGUMMEIs);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_RelativeMMECapacity, &s1ap_MMEConfigurationUpdateIEs->relativeMMECapacity);
}

void s1ap_free_s1ap_e_rabmodifyrequesties(
    S1ap_E_RABModifyRequestIEs_t *s1ap_E_RABModifyRequestIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABModifyRequestIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABModifyRequestIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_E_RABModifyRequestIEs->uEaggregateMaximumBitrate);
    s1ap_free_s1ap_e_rabtobemodifiedlistbearermodreq(&s1ap_E_RABModifyRequestIEs->e_RABToBeModifiedListBearerModReq);
}

void s1ap_free_s1ap_enbconfigurationupdatefailureies(
    S1ap_ENBConfigurationUpdateFailureIEs_t *s1ap_ENBConfigurationUpdateFailureIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_ENBConfigurationUpdateFailureIEs->cause);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TimeToWait, &s1ap_ENBConfigurationUpdateFailureIEs->timeToWait);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_ENBConfigurationUpdateFailureIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_reseties(
    S1ap_ResetIEs_t *s1ap_ResetIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_ResetIEs->cause);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ResetType, &s1ap_ResetIEs->resetType);
}

void s1ap_free_s1ap_s1setupresponseies(
    S1ap_S1SetupResponseIEs_t *s1ap_S1SetupResponseIEs)
{

            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMEname, &s1ap_S1SetupResponseIEs->mmEname);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ServedGUMMEIs, &s1ap_S1SetupResponseIEs->servedGUMMEIs);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_RelativeMMECapacity, &s1ap_S1SetupResponseIEs->relativeMMECapacity);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MMERelaySupportIndicator, &s1ap_S1SetupResponseIEs->mmeRelaySupportIndicator);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_S1SetupResponseIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_pagingies(
    S1ap_PagingIEs_t *s1ap_PagingIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEIdentityIndexValue, &s1ap_PagingIEs->ueIdentityIndexValue);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEPagingID, &s1ap_PagingIEs->uePagingID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PagingDRX, &s1ap_PagingIEs->pagingDRX);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CNDomain, &s1ap_PagingIEs->cnDomain);
    s1ap_free_s1ap_tailist(&s1ap_PagingIEs->taiList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSG_IdList, &s1ap_PagingIEs->csG_IdList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PagingPriority, &s1ap_PagingIEs->pagingPriority);
}

void s1ap_free_s1ap_enbconfigurationupdateacknowledgeies(
    S1ap_ENBConfigurationUpdateAcknowledgeIEs_t *s1ap_ENBConfigurationUpdateAcknowledgeIEs)
{

            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_ENBConfigurationUpdateAcknowledgeIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_e_rabreleasecommandies(
    S1ap_E_RABReleaseCommandIEs_t *s1ap_E_RABReleaseCommandIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABReleaseCommandIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABReleaseCommandIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_E_RABReleaseCommandIEs->uEaggregateMaximumBitrate);
    s1ap_free_s1ap_e_rablist(&s1ap_E_RABReleaseCommandIEs->e_RABToBeReleasedList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NAS_PDU, &s1ap_E_RABReleaseCommandIEs->nas_pdu);
}

void s1ap_free_s1ap_pathswitchrequestfailureies(
    S1ap_PathSwitchRequestFailureIEs_t *s1ap_PathSwitchRequestFailureIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_PathSwitchRequestFailureIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_PathSwitchRequestFailureIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_PathSwitchRequestFailureIEs->cause);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_PathSwitchRequestFailureIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_e_rabmodifyresponseies(
    S1ap_E_RABModifyResponseIEs_t *s1ap_E_RABModifyResponseIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABModifyResponseIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABModifyResponseIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    s1ap_free_s1ap_e_rabmodifylistbearermodres(&s1ap_E_RABModifyResponseIEs->e_RABModifyListBearerModRes);
            /* Optional field */
    s1ap_free_s1ap_e_rablist(&s1ap_E_RABModifyResponseIEs->e_RABFailedToModifyList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_E_RABModifyResponseIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_handovernotifyies(
    S1ap_HandoverNotifyIEs_t *s1ap_HandoverNotifyIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverNotifyIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverNotifyIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_EUTRAN_CGI, &s1ap_HandoverNotifyIEs->eutran_cgi);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TAI, &s1ap_HandoverNotifyIEs->tai);
}

void s1ap_free_s1ap_handovercancelacknowledgeies(
    S1ap_HandoverCancelAcknowledgeIEs_t *s1ap_HandoverCancelAcknowledgeIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverCancelAcknowledgeIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverCancelAcknowledgeIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_HandoverCancelAcknowledgeIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_initialcontextsetupfailureies(
    S1ap_InitialContextSetupFailureIEs_t *s1ap_InitialContextSetupFailureIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_InitialContextSetupFailureIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_InitialContextSetupFailureIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_InitialContextSetupFailureIEs->cause);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_InitialContextSetupFailureIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_handovercommandies(
    S1ap_HandoverCommandIEs_t *s1ap_HandoverCommandIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverCommandIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverCommandIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverType, &s1ap_HandoverCommandIEs->handoverType);
            /* Optional field */
    s1ap_free_s1ap_e_rabdataforwardinglist(&s1ap_HandoverCommandIEs->e_RABDataForwardingList);
            /* Optional field */
    s1ap_free_s1ap_e_rablist(&s1ap_HandoverCommandIEs->e_RABtoReleaseListHOCmd);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Target_ToSource_TransparentContainer, &s1ap_HandoverCommandIEs->target_ToSource_TransparentContainer);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Target_ToSource_TransparentContainer, &s1ap_HandoverCommandIEs->target_ToSource_TransparentContainer_Secondary);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_HandoverCommandIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_mmedirectinformationtransferies(
    S1ap_MMEDirectInformationTransferIEs_t *s1ap_MMEDirectInformationTransferIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Inter_SystemInformationTransferType, &s1ap_MMEDirectInformationTransferIEs->inter_SystemInformationTransferTypeMDT);
}

void s1ap_free_s1ap_errorindicationies(
    S1ap_ErrorIndicationIEs_t *s1ap_ErrorIndicationIEs)
{

            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_ErrorIndicationIEs->mme_ue_s1ap_id);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_ErrorIndicationIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_ErrorIndicationIEs->cause);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_ErrorIndicationIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_overloadstarties(
    S1ap_OverloadStartIEs_t *s1ap_OverloadStartIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_OverloadResponse, &s1ap_OverloadStartIEs->overloadResponse);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_GUMMEIList, &s1ap_OverloadStartIEs->gummeiList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TrafficLoadReductionIndication, &s1ap_OverloadStartIEs->trafficLoadReductionIndication);
}

void s1ap_free_s1ap_downlinknonueassociatedlppatransport_ies(
    S1ap_DownlinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Routing_ID, &s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs->routing_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LPPa_PDU, &s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs->lpPa_PDU);
}

void s1ap_free_s1ap_handoverrequestacknowledgeies(
    S1ap_HandoverRequestAcknowledgeIEs_t *s1ap_HandoverRequestAcknowledgeIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverRequestAcknowledgeIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_HandoverRequestAcknowledgeIEs->eNB_UE_S1AP_ID);
    s1ap_free_s1ap_e_rabadmittedlist(&s1ap_HandoverRequestAcknowledgeIEs->e_RABAdmittedList);
            /* Optional field */
    s1ap_free_s1ap_e_rabfailedtosetuplisthoreqack(&s1ap_HandoverRequestAcknowledgeIEs->e_RABFailedToSetupListHOReqAck);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Target_ToSource_TransparentContainer, &s1ap_HandoverRequestAcknowledgeIEs->target_ToSource_TransparentContainer);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSG_Id, &s1ap_HandoverRequestAcknowledgeIEs->csG_Id);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_HandoverRequestAcknowledgeIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_downlinkueassociatedlppatransport_ies(
    S1ap_DownlinkUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkUEAssociatedLPPaTransport_IEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Routing_ID, &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->routing_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LPPa_PDU, &s1ap_DownlinkUEAssociatedLPPaTransport_IEs->lpPa_PDU);
}

void s1ap_free_s1ap_uecontextmodificationfailureies(
    S1ap_UEContextModificationFailureIEs_t *s1ap_UEContextModificationFailureIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UEContextModificationFailureIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UEContextModificationFailureIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_UEContextModificationFailureIEs->cause);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_UEContextModificationFailureIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_mmeconfigurationtransferies(
    S1ap_MMEConfigurationTransferIEs_t *s1ap_MMEConfigurationTransferIEs)
{

            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SONConfigurationTransfer, &s1ap_MMEConfigurationTransferIEs->sonConfigurationTransferMCT);
}

void s1ap_free_s1ap_pathswitchrequestacknowledgeies(
    S1ap_PathSwitchRequestAcknowledgeIEs_t *s1ap_PathSwitchRequestAcknowledgeIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_PathSwitchRequestAcknowledgeIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_PathSwitchRequestAcknowledgeIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_PathSwitchRequestAcknowledgeIEs->uEaggregateMaximumBitrate);
            /* Optional field */
    s1ap_free_s1ap_e_rabtobeswitchedullist(&s1ap_PathSwitchRequestAcknowledgeIEs->e_RABToBeSwitchedULList);
            /* Optional field */
    s1ap_free_s1ap_e_rablist(&s1ap_PathSwitchRequestAcknowledgeIEs->e_RABToBeReleasedList);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SecurityContext, &s1ap_PathSwitchRequestAcknowledgeIEs->securityContext);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_PathSwitchRequestAcknowledgeIEs->criticalityDiagnostics);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_PathSwitchRequestAcknowledgeIEs->mme_ue_s1ap_id_2);
}

void s1ap_free_s1ap_s1setupfailureies(
    S1ap_S1SetupFailureIEs_t *s1ap_S1SetupFailureIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_S1SetupFailureIEs->cause);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TimeToWait, &s1ap_S1SetupFailureIEs->timeToWait);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_S1SetupFailureIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_mmeconfigurationupdatefailureies(
    S1ap_MMEConfigurationUpdateFailureIEs_t *s1ap_MMEConfigurationUpdateFailureIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_MMEConfigurationUpdateFailureIEs->cause);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TimeToWait, &s1ap_MMEConfigurationUpdateFailureIEs->timeToWait);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_MMEConfigurationUpdateFailureIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_initialcontextsetupresponseies(
    S1ap_InitialContextSetupResponseIEs_t *s1ap_InitialContextSetupResponseIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_InitialContextSetupResponseIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_InitialContextSetupResponseIEs->eNB_UE_S1AP_ID);
    s1ap_free_s1ap_e_rabsetuplistctxtsures(&s1ap_InitialContextSetupResponseIEs->e_RABSetupListCtxtSURes);
            /* Optional field */
    s1ap_free_s1ap_e_rablist(&s1ap_InitialContextSetupResponseIEs->e_RABFailedToSetupListCtxtSURes);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_InitialContextSetupResponseIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_downlinks1cdma2000tunnelingies(
    S1ap_DownlinkS1cdma2000tunnelingIEs_t *s1ap_DownlinkS1cdma2000tunnelingIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_DownlinkS1cdma2000tunnelingIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_DownlinkS1cdma2000tunnelingIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    s1ap_free_s1ap_e_rabdataforwardinglist(&s1ap_DownlinkS1cdma2000tunnelingIEs->e_RABDataForwardingList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cdma2000HOStatus, &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000HOStatus);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cdma2000RATType, &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000RATType);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cdma2000PDU, &s1ap_DownlinkS1cdma2000tunnelingIEs->cdma2000PDU);
}

void s1ap_free_s1ap_overloadstopies(
    S1ap_OverloadStopIEs_t *s1ap_OverloadStopIEs)
{

            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_GUMMEIList, &s1ap_OverloadStopIEs->gummeiList);
}

void s1ap_free_s1ap_enbconfigurationupdateies(
    S1ap_ENBConfigurationUpdateIEs_t *s1ap_ENBConfigurationUpdateIEs)
{

            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBname, &s1ap_ENBConfigurationUpdateIEs->eNBname);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SupportedTAs, &s1ap_ENBConfigurationUpdateIEs->supportedTAs);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSG_IdList, &s1ap_ENBConfigurationUpdateIEs->csG_IdList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PagingDRX, &s1ap_ENBConfigurationUpdateIEs->defaultPagingDRX);
}

void s1ap_free_s1ap_tracefailureindicationies(
    S1ap_TraceFailureIndicationIEs_t *s1ap_TraceFailureIndicationIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_TraceFailureIndicationIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_TraceFailureIndicationIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_UTRAN_Trace_ID, &s1ap_TraceFailureIndicationIEs->e_UTRAN_Trace_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_TraceFailureIndicationIEs->cause);
}

void s1ap_free_s1ap_enbconfigurationtransferies(
    S1ap_ENBConfigurationTransferIEs_t *s1ap_ENBConfigurationTransferIEs)
{

            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SONConfigurationTransfer, &s1ap_ENBConfigurationTransferIEs->sonConfigurationTransferECT);
}

void s1ap_free_s1ap_handoverfailureies(
    S1ap_HandoverFailureIEs_t *s1ap_HandoverFailureIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_HandoverFailureIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_HandoverFailureIEs->cause);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_HandoverFailureIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_e_rabsetupresponseies(
    S1ap_E_RABSetupResponseIEs_t *s1ap_E_RABSetupResponseIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABSetupResponseIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABSetupResponseIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    s1ap_free_s1ap_e_rabsetuplistbearersures(&s1ap_E_RABSetupResponseIEs->e_RABSetupListBearerSURes);
            /* Optional field */
    s1ap_free_s1ap_e_rablist(&s1ap_E_RABSetupResponseIEs->e_RABFailedToSetupListBearerSURes);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_E_RABSetupResponseIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_uecontextreleasecommand_ies(
    S1ap_UEContextReleaseCommand_IEs_t *s1ap_UEContextReleaseCommand_IEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UE_S1AP_IDs, &s1ap_UEContextReleaseCommand_IEs->uE_S1AP_IDs);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_UEContextReleaseCommand_IEs->cause);
}

void s1ap_free_s1ap_pathswitchrequesties(
    S1ap_PathSwitchRequestIEs_t *s1ap_PathSwitchRequestIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_PathSwitchRequestIEs->eNB_UE_S1AP_ID);
    s1ap_free_s1ap_e_rabtobeswitcheddllist(&s1ap_PathSwitchRequestIEs->e_RABToBeSwitchedDLList);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_PathSwitchRequestIEs->sourceMME_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_EUTRAN_CGI, &s1ap_PathSwitchRequestIEs->eutran_cgi);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TAI, &s1ap_PathSwitchRequestIEs->tai);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UESecurityCapabilities, &s1ap_PathSwitchRequestIEs->ueSecurityCapabilities);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSG_Id, &s1ap_PathSwitchRequestIEs->csG_Id);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CellAccessMode, &s1ap_PathSwitchRequestIEs->cellAccessMode);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_GUMMEI, &s1ap_PathSwitchRequestIEs->sourceMME_GUMMEI);
}

void s1ap_free_s1ap_downlinknastransport_ies(
    S1ap_DownlinkNASTransport_IEs_t *s1ap_DownlinkNASTransport_IEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_DownlinkNASTransport_IEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_DownlinkNASTransport_IEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NAS_PDU, &s1ap_DownlinkNASTransport_IEs->nas_pdu);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_HandoverRestrictionList, &s1ap_DownlinkNASTransport_IEs->handoverRestrictionList);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SubscriberProfileIDforRFP, &s1ap_DownlinkNASTransport_IEs->subscriberProfileIDforRFP);
}

void s1ap_free_s1ap_e_rabreleaseindicationies(
    S1ap_E_RABReleaseIndicationIEs_t *s1ap_E_RABReleaseIndicationIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABReleaseIndicationIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABReleaseIndicationIEs->eNB_UE_S1AP_ID);
    s1ap_free_s1ap_e_rablist(&s1ap_E_RABReleaseIndicationIEs->e_RABReleasedList);
}

void s1ap_free_s1ap_s1setuprequesties(
    S1ap_S1SetupRequestIEs_t *s1ap_S1SetupRequestIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Global_ENB_ID, &s1ap_S1SetupRequestIEs->global_ENB_ID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENBname, &s1ap_S1SetupRequestIEs->eNBname);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SupportedTAs, &s1ap_S1SetupRequestIEs->supportedTAs);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PagingDRX, &s1ap_S1SetupRequestIEs->defaultPagingDRX);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CSG_IdList, &s1ap_S1SetupRequestIEs->csG_IdList);
}

void s1ap_free_s1ap_nasnondeliveryindication_ies(
    S1ap_NASNonDeliveryIndication_IEs_t *s1ap_NASNonDeliveryIndication_IEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_NASNonDeliveryIndication_IEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_NASNonDeliveryIndication_IEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_NAS_PDU, &s1ap_NASNonDeliveryIndication_IEs->nas_pdu);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Cause, &s1ap_NASNonDeliveryIndication_IEs->cause);
}

void s1ap_free_s1ap_celltraffictraceies(
    S1ap_CellTrafficTraceIEs_t *s1ap_CellTrafficTraceIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_CellTrafficTraceIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_CellTrafficTraceIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_E_UTRAN_Trace_ID, &s1ap_CellTrafficTraceIEs->e_UTRAN_Trace_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_EUTRAN_CGI, &s1ap_CellTrafficTraceIEs->eutran_cgi);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_TransportLayerAddress, &s1ap_CellTrafficTraceIEs->traceCollectionEntityIPAddress);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_PrivacyIndicator, &s1ap_CellTrafficTraceIEs->privacyIndicator);
}

void s1ap_free_s1ap_e_rabsetuprequesties(
    S1ap_E_RABSetupRequestIEs_t *s1ap_E_RABSetupRequestIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_E_RABSetupRequestIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_E_RABSetupRequestIEs->eNB_UE_S1AP_ID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_UEAggregateMaximumBitrate, &s1ap_E_RABSetupRequestIEs->uEaggregateMaximumBitrate);
    s1ap_free_s1ap_e_rabtobesetuplistbearersureq(&s1ap_E_RABSetupRequestIEs->e_RABToBeSetupListBearerSUReq);
}

void s1ap_free_s1ap_enbstatustransferies(
    S1ap_ENBStatusTransferIEs_t *s1ap_ENBStatusTransferIEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_ENBStatusTransferIEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_ENBStatusTransferIEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_StatusTransfer_TransparentContainer, &s1ap_ENBStatusTransferIEs->eNB_StatusTransfer_TransparentContainer);
}

void s1ap_free_s1ap_resetacknowledgeies(
    S1ap_ResetAcknowledgeIEs_t *s1ap_ResetAcknowledgeIEs)
{

            /* Optional field */
    s1ap_free_s1ap_ue_associatedlogicals1_connectionlistresack(&s1ap_ResetAcknowledgeIEs->uE_associatedLogicalS1_ConnectionListResAck);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_ResetAcknowledgeIEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_uecontextreleasecomplete_ies(
    S1ap_UEContextReleaseComplete_IEs_t *s1ap_UEContextReleaseComplete_IEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UEContextReleaseComplete_IEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UEContextReleaseComplete_IEs->eNB_UE_S1AP_ID);
            /* Optional field */
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_CriticalityDiagnostics, &s1ap_UEContextReleaseComplete_IEs->criticalityDiagnostics);
}

void s1ap_free_s1ap_uplinkueassociatedlppatransport_ies(
    S1ap_UplinkUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkUEAssociatedLPPaTransport_IEs)
{

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_MME_UE_S1AP_ID, &s1ap_UplinkUEAssociatedLPPaTransport_IEs->mme_ue_s1ap_id);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_ENB_UE_S1AP_ID, &s1ap_UplinkUEAssociatedLPPaTransport_IEs->eNB_UE_S1AP_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Routing_ID, &s1ap_UplinkUEAssociatedLPPaTransport_IEs->routing_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_LPPa_PDU, &s1ap_UplinkUEAssociatedLPPaTransport_IEs->lpPa_PDU);
}

void s1ap_free_s1ap_e_rabfailedtosetuplisthoreqack(
    S1ap_E_RABFailedtoSetupListHOReqAck_IEs_t *s1ap_E_RABFailedtoSetupListHOReqAckIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABFailedtoSetupListHOReqAckIEs->s1ap_E_RABFailedtoSetupItemHOReqAck.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABFailedToSetupItemHOReqAck, s1ap_E_RABFailedtoSetupListHOReqAckIEs->s1ap_E_RABFailedtoSetupItemHOReqAck.array[i]);
    }

    if (s1ap_E_RABFailedtoSetupListHOReqAckIEs->s1ap_E_RABFailedtoSetupItemHOReqAck.array)
       FREEMEM(s1ap_E_RABFailedtoSetupListHOReqAckIEs->s1ap_E_RABFailedtoSetupItemHOReqAck.array);
}

void s1ap_free_s1ap_e_rabreleaselistbearerrelcomp(
    S1ap_E_RABReleaseListBearerRelComp_IEs_t *s1ap_E_RABReleaseListBearerRelCompIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABReleaseListBearerRelCompIEs->s1ap_E_RABReleaseItemBearerRelComp.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABReleaseItemBearerRelComp, s1ap_E_RABReleaseListBearerRelCompIEs->s1ap_E_RABReleaseItemBearerRelComp.array[i]);
    }

    if (s1ap_E_RABReleaseListBearerRelCompIEs->s1ap_E_RABReleaseItemBearerRelComp.array)
       FREEMEM(s1ap_E_RABReleaseListBearerRelCompIEs->s1ap_E_RABReleaseItemBearerRelComp.array);
}

void s1ap_free_s1ap_e_rabtobesetuplisthoreq(
    S1ap_E_RABToBeSetupListHOReq_IEs_t *s1ap_E_RABToBeSetupListHOReqIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABToBeSetupListHOReqIEs->s1ap_E_RABToBeSetupItemHOReq.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupItemHOReq, s1ap_E_RABToBeSetupListHOReqIEs->s1ap_E_RABToBeSetupItemHOReq.array[i]);
    }

    if (s1ap_E_RABToBeSetupListHOReqIEs->s1ap_E_RABToBeSetupItemHOReq.array)
       FREEMEM(s1ap_E_RABToBeSetupListHOReqIEs->s1ap_E_RABToBeSetupItemHOReq.array);
}

void s1ap_free_s1ap_e_rabtobemodifiedlistbearermodreq(
    S1ap_E_RABToBeModifiedListBearerModReq_IEs_t *s1ap_E_RABToBeModifiedListBearerModReqIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABToBeModifiedListBearerModReqIEs->s1ap_E_RABToBeModifiedItemBearerModReq.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeModifiedItemBearerModReq, s1ap_E_RABToBeModifiedListBearerModReqIEs->s1ap_E_RABToBeModifiedItemBearerModReq.array[i]);
    }

    if (s1ap_E_RABToBeModifiedListBearerModReqIEs->s1ap_E_RABToBeModifiedItemBearerModReq.array)
       FREEMEM(s1ap_E_RABToBeModifiedListBearerModReqIEs->s1ap_E_RABToBeModifiedItemBearerModReq.array);
}

void s1ap_free_s1ap_e_rabadmittedlist(
    S1ap_E_RABAdmittedList_IEs_t *s1ap_E_RABAdmittedListIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABAdmittedListIEs->s1ap_E_RABAdmittedItem.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABAdmittedItem, s1ap_E_RABAdmittedListIEs->s1ap_E_RABAdmittedItem.array[i]);
    }

    if (s1ap_E_RABAdmittedListIEs->s1ap_E_RABAdmittedItem.array)
       FREEMEM(s1ap_E_RABAdmittedListIEs->s1ap_E_RABAdmittedItem.array);
}

void s1ap_free_s1ap_e_rabtobesetuplistbearersureq(
    S1ap_E_RABToBeSetupListBearerSUReq_IEs_t *s1ap_E_RABToBeSetupListBearerSUReqIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABToBeSetupListBearerSUReqIEs->s1ap_E_RABToBeSetupItemBearerSUReq.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupItemBearerSUReq, s1ap_E_RABToBeSetupListBearerSUReqIEs->s1ap_E_RABToBeSetupItemBearerSUReq.array[i]);
    }

    if (s1ap_E_RABToBeSetupListBearerSUReqIEs->s1ap_E_RABToBeSetupItemBearerSUReq.array)
       FREEMEM(s1ap_E_RABToBeSetupListBearerSUReqIEs->s1ap_E_RABToBeSetupItemBearerSUReq.array);
}

void s1ap_free_s1ap_e_rabdataforwardinglist(
    S1ap_E_RABDataForwardingList_IEs_t *s1ap_E_RABDataForwardingListIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABDataForwardingListIEs->s1ap_E_RABDataForwardingItem.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABDataForwardingItem, s1ap_E_RABDataForwardingListIEs->s1ap_E_RABDataForwardingItem.array[i]);
    }

    if (s1ap_E_RABDataForwardingListIEs->s1ap_E_RABDataForwardingItem.array)
       FREEMEM(s1ap_E_RABDataForwardingListIEs->s1ap_E_RABDataForwardingItem.array);
}

void s1ap_free_s1ap_e_rabtobeswitcheddllist(
    S1ap_E_RABToBeSwitchedDLList_IEs_t *s1ap_E_RABToBeSwitchedDLListIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABToBeSwitchedDLListIEs->s1ap_E_RABToBeSwitchedDLItem.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSwitchedDLItem, s1ap_E_RABToBeSwitchedDLListIEs->s1ap_E_RABToBeSwitchedDLItem.array[i]);
    }

    if (s1ap_E_RABToBeSwitchedDLListIEs->s1ap_E_RABToBeSwitchedDLItem.array)
       FREEMEM(s1ap_E_RABToBeSwitchedDLListIEs->s1ap_E_RABToBeSwitchedDLItem.array);
}

void s1ap_free_s1ap_e_rabsetuplistctxtsures(
    S1ap_E_RABSetupListCtxtSURes_IEs_t *s1ap_E_RABSetupListCtxtSUResIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABSetupListCtxtSUResIEs->s1ap_E_RABSetupItemCtxtSURes.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABSetupItemCtxtSURes, s1ap_E_RABSetupListCtxtSUResIEs->s1ap_E_RABSetupItemCtxtSURes.array[i]);
    }

    if (s1ap_E_RABSetupListCtxtSUResIEs->s1ap_E_RABSetupItemCtxtSURes.array)
       FREEMEM(s1ap_E_RABSetupListCtxtSUResIEs->s1ap_E_RABSetupItemCtxtSURes.array);
}

void s1ap_free_s1ap_e_rabtobeswitchedullist(
    S1ap_E_RABToBeSwitchedULList_IEs_t *s1ap_E_RABToBeSwitchedULListIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABToBeSwitchedULListIEs->s1ap_E_RABToBeSwitchedULItem.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSwitchedULItem, s1ap_E_RABToBeSwitchedULListIEs->s1ap_E_RABToBeSwitchedULItem.array[i]);
    }

    if (s1ap_E_RABToBeSwitchedULListIEs->s1ap_E_RABToBeSwitchedULItem.array)
       FREEMEM(s1ap_E_RABToBeSwitchedULListIEs->s1ap_E_RABToBeSwitchedULItem.array);
}

void s1ap_free_s1ap_bearers_subjecttostatustransfer_list(
    S1ap_Bearers_SubjectToStatusTransfer_List_IEs_t *s1ap_Bearers_SubjectToStatusTransfer_ListIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_Bearers_SubjectToStatusTransfer_ListIEs->s1ap_Bearers_SubjectToStatusTransfer_Item.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_Bearers_SubjectToStatusTransfer_Item, s1ap_Bearers_SubjectToStatusTransfer_ListIEs->s1ap_Bearers_SubjectToStatusTransfer_Item.array[i]);
    }

    if (s1ap_Bearers_SubjectToStatusTransfer_ListIEs->s1ap_Bearers_SubjectToStatusTransfer_Item.array)
       FREEMEM(s1ap_Bearers_SubjectToStatusTransfer_ListIEs->s1ap_Bearers_SubjectToStatusTransfer_Item.array);
}

void s1ap_free_s1ap_e_rablist(
    S1ap_E_RABList_IEs_t *s1ap_E_RABListIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABListIEs->s1ap_E_RABItem.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABItem, s1ap_E_RABListIEs->s1ap_E_RABItem.array[i]);
    }

    if (s1ap_E_RABListIEs->s1ap_E_RABItem.array)
       FREEMEM(s1ap_E_RABListIEs->s1ap_E_RABItem.array);
}

void s1ap_free_s1ap_tailist(
    S1ap_TAIList_IEs_t *s1ap_TAIListIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_TAIListIEs->s1ap_TAIItem.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_TAIItem, s1ap_TAIListIEs->s1ap_TAIItem.array[i]);
    }

    if (s1ap_TAIListIEs->s1ap_TAIItem.array)
       FREEMEM(s1ap_TAIListIEs->s1ap_TAIItem.array);
}

void s1ap_free_s1ap_ue_associatedlogicals1_connectionlistres(
    S1ap_UE_associatedLogicalS1_ConnectionListRes_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_UE_associatedLogicalS1_ConnectionListResIEs->s1ap_UE_associatedLogicalS1_ConnectionItemRes.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionItem, s1ap_UE_associatedLogicalS1_ConnectionListResIEs->s1ap_UE_associatedLogicalS1_ConnectionItemRes.array[i]);
    }

    if (s1ap_UE_associatedLogicalS1_ConnectionListResIEs->s1ap_UE_associatedLogicalS1_ConnectionItemRes.array)
       FREEMEM(s1ap_UE_associatedLogicalS1_ConnectionListResIEs->s1ap_UE_associatedLogicalS1_ConnectionItemRes.array);
}

void s1ap_free_s1ap_e_rabsetuplistbearersures(
    S1ap_E_RABSetupListBearerSURes_IEs_t *s1ap_E_RABSetupListBearerSUResIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABSetupListBearerSUResIEs->s1ap_E_RABSetupItemBearerSURes.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABSetupItemBearerSURes, s1ap_E_RABSetupListBearerSUResIEs->s1ap_E_RABSetupItemBearerSURes.array[i]);
    }

    if (s1ap_E_RABSetupListBearerSUResIEs->s1ap_E_RABSetupItemBearerSURes.array)
       FREEMEM(s1ap_E_RABSetupListBearerSUResIEs->s1ap_E_RABSetupItemBearerSURes.array);
}

void s1ap_free_s1ap_e_rabmodifylistbearermodres(
    S1ap_E_RABModifyListBearerModRes_IEs_t *s1ap_E_RABModifyListBearerModResIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABModifyListBearerModResIEs->s1ap_E_RABModifyItemBearerModRes.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABModifyItemBearerModRes, s1ap_E_RABModifyListBearerModResIEs->s1ap_E_RABModifyItemBearerModRes.array[i]);
    }

    if (s1ap_E_RABModifyListBearerModResIEs->s1ap_E_RABModifyItemBearerModRes.array)
       FREEMEM(s1ap_E_RABModifyListBearerModResIEs->s1ap_E_RABModifyItemBearerModRes.array);
}

void s1ap_free_s1ap_e_rabtobesetuplistctxtsureq(
    S1ap_E_RABToBeSetupListCtxtSUReq_IEs_t *s1ap_E_RABToBeSetupListCtxtSUReqIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABToBeSetupListCtxtSUReqIEs->s1ap_E_RABToBeSetupItemCtxtSUReq.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABToBeSetupItemCtxtSUReq, s1ap_E_RABToBeSetupListCtxtSUReqIEs->s1ap_E_RABToBeSetupItemCtxtSUReq.array[i]);
    }

    if (s1ap_E_RABToBeSetupListCtxtSUReqIEs->s1ap_E_RABToBeSetupItemCtxtSUReq.array)
       FREEMEM(s1ap_E_RABToBeSetupListCtxtSUReqIEs->s1ap_E_RABToBeSetupItemCtxtSUReq.array);
}

void s1ap_free_s1ap_e_rabinformationlist(
    S1ap_E_RABInformationList_IEs_t *s1ap_E_RABInformationListIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_E_RABInformationListIEs->s1ap_E_RABInformationList.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_E_RABInformationListItem, s1ap_E_RABInformationListIEs->s1ap_E_RABInformationList.array[i]);
    }

    if (s1ap_E_RABInformationListIEs->s1ap_E_RABInformationList.array)
       FREEMEM(s1ap_E_RABInformationListIEs->s1ap_E_RABInformationList.array);
}

void s1ap_free_s1ap_ue_associatedlogicals1_connectionlistresack(
    S1ap_UE_associatedLogicalS1_ConnectionListResAck_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs)
{
    int i = 0;

    for (i = 0; i < s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs->s1ap_UE_associatedLogicalS1_ConnectionItemResAck.count; i++)
    {
        ASN_STRUCT_FREE(asn_DEF_S1ap_UE_associatedLogicalS1_ConnectionItem, s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs->s1ap_UE_associatedLogicalS1_ConnectionItemResAck.array[i]);
    }

    if (s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs->s1ap_UE_associatedLogicalS1_ConnectionItemResAck.array)
       FREEMEM(s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs->s1ap_UE_associatedLogicalS1_ConnectionItemResAck.array);
}

