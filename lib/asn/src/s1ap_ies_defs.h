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
 * Created on: 2017-02-03 21:15:13.355159 by acetcom
 * from ['S1AP-PDU.asn']
 ******************************************************************************/
#include "s1ap_common.h"

#ifndef S1AP_IES_DEFS_H_
#define S1AP_IES_DEFS_H_

/* Define the version of script used to generate this file */
#define S1AP_SCRIPT_VERSION (102)

typedef struct S1ap_E_RABFailedtoSetupListHOReqAck_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABFailedtoSetupItemHOReqAck_s) s1ap_E_RABFailedtoSetupItemHOReqAck;
} S1ap_E_RABFailedtoSetupListHOReqAck_IEs_t;

typedef struct S1ap_E_RABReleaseListBearerRelComp_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABReleaseItemBearerRelComp_s) s1ap_E_RABReleaseItemBearerRelComp;
} S1ap_E_RABReleaseListBearerRelComp_IEs_t;

typedef struct S1ap_E_RABToBeSetupListHOReq_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABToBeSetupItemHOReq_s) s1ap_E_RABToBeSetupItemHOReq;
} S1ap_E_RABToBeSetupListHOReq_IEs_t;

typedef struct S1ap_E_RABToBeModifiedListBearerModReq_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABToBeModifiedItemBearerModReq_s) s1ap_E_RABToBeModifiedItemBearerModReq;
} S1ap_E_RABToBeModifiedListBearerModReq_IEs_t;

typedef struct S1ap_E_RABAdmittedList_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABAdmittedItem_s) s1ap_E_RABAdmittedItem;
} S1ap_E_RABAdmittedList_IEs_t;

typedef struct S1ap_E_RABToBeSetupListBearerSUReq_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABToBeSetupItemBearerSUReq_s) s1ap_E_RABToBeSetupItemBearerSUReq;
} S1ap_E_RABToBeSetupListBearerSUReq_IEs_t;

typedef struct S1ap_E_RABDataForwardingList_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABDataForwardingItem_s) s1ap_E_RABDataForwardingItem;
} S1ap_E_RABDataForwardingList_IEs_t;

typedef struct S1ap_E_RABToBeSwitchedDLList_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABToBeSwitchedDLItem_s) s1ap_E_RABToBeSwitchedDLItem;
} S1ap_E_RABToBeSwitchedDLList_IEs_t;

typedef struct S1ap_E_RABSetupListCtxtSURes_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABSetupItemCtxtSURes_s) s1ap_E_RABSetupItemCtxtSURes;
} S1ap_E_RABSetupListCtxtSURes_IEs_t;

typedef struct S1ap_E_RABToBeSwitchedULList_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABToBeSwitchedULItem_s) s1ap_E_RABToBeSwitchedULItem;
} S1ap_E_RABToBeSwitchedULList_IEs_t;

typedef struct S1ap_Bearers_SubjectToStatusTransfer_List_IEs_s {
    A_SEQUENCE_OF(struct S1ap_Bearers_SubjectToStatusTransfer_Item_s) s1ap_Bearers_SubjectToStatusTransfer_Item;
} S1ap_Bearers_SubjectToStatusTransfer_List_IEs_t;

typedef struct S1ap_E_RABList_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABItem_s) s1ap_E_RABItem;
} S1ap_E_RABList_IEs_t;

typedef struct S1ap_TAIList_IEs_s {
    A_SEQUENCE_OF(struct S1ap_TAIItem_s) s1ap_TAIItem;
} S1ap_TAIList_IEs_t;

typedef struct S1ap_UE_associatedLogicalS1_ConnectionListRes_IEs_s {
    A_SEQUENCE_OF(struct S1ap_UE_associatedLogicalS1_ConnectionItemRes_s) s1ap_UE_associatedLogicalS1_ConnectionItemRes;
} S1ap_UE_associatedLogicalS1_ConnectionListRes_IEs_t;

typedef struct S1ap_E_RABSetupListBearerSURes_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABSetupItemBearerSURes_s) s1ap_E_RABSetupItemBearerSURes;
} S1ap_E_RABSetupListBearerSURes_IEs_t;

typedef struct S1ap_E_RABModifyListBearerModRes_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABModifyItemBearerModRes_s) s1ap_E_RABModifyItemBearerModRes;
} S1ap_E_RABModifyListBearerModRes_IEs_t;

typedef struct S1ap_E_RABToBeSetupListCtxtSUReq_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABToBeSetupItemCtxtSUReq_s) s1ap_E_RABToBeSetupItemCtxtSUReq;
} S1ap_E_RABToBeSetupListCtxtSUReq_IEs_t;

typedef struct S1ap_E_RABInformationList_IEs_s {
    A_SEQUENCE_OF(struct S1ap_E_RABInformationList_s) s1ap_E_RABInformationList;
} S1ap_E_RABInformationList_IEs_t;

typedef struct S1ap_UE_associatedLogicalS1_ConnectionListResAck_IEs_s {
    A_SEQUENCE_OF(struct S1ap_UE_associatedLogicalS1_ConnectionItemResAck_s) s1ap_UE_associatedLogicalS1_ConnectionItemResAck;
} S1ap_UE_associatedLogicalS1_ConnectionListResAck_IEs_t;

typedef struct S1ap_E_RABFailedtoSetupItemHOReqAckIEs_s {
    S1ap_E_RABFailedToSetupItemHOReqAck_t e_RABFailedtoSetupItemHOReqAck;
} S1ap_E_RABFailedtoSetupItemHOReqAckIEs_t;

typedef struct S1ap_DeactivateTraceIEs_s {
    S1ap_MME_UE_S1AP_ID_t   mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t   eNB_UE_S1AP_ID;
    S1ap_E_UTRAN_Trace_ID_t e_UTRAN_Trace_ID;
} S1ap_DeactivateTraceIEs_t;

typedef struct S1ap_E_RABReleaseItemBearerRelCompIEs_s {
    S1ap_E_RABReleaseItemBearerRelComp_t e_RABReleaseItemBearerRelComp;
} S1ap_E_RABReleaseItemBearerRelCompIEs_t;

typedef struct S1ap_ENBDirectInformationTransferIEs_s {
    S1ap_Inter_SystemInformationTransferType_t inter_SystemInformationTransferTypeEDT;
} S1ap_ENBDirectInformationTransferIEs_t;

#define S1AP_E_RABRELEASERESPONSEIES_E_RABRELEASELISTBEARERRELCOMP_PRESENT      (1 << 0)
#define S1AP_E_RABRELEASERESPONSEIES_E_RABFAILEDTORELEASELIST_PRESENT           (1 << 1)
#define S1AP_E_RABRELEASERESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT             (1 << 2)

typedef struct S1ap_E_RABReleaseResponseIEs_s {
    uint16_t                             presenceMask;
    S1ap_MME_UE_S1AP_ID_t                mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t                eNB_UE_S1AP_ID;
    S1ap_E_RABReleaseListBearerRelComp_IEs_t e_RABReleaseListBearerRelComp; ///< Optional field
    S1ap_E_RABList_IEs_t e_RABFailedToReleaseList; ///< Optional field
    S1ap_CriticalityDiagnostics_t        criticalityDiagnostics; ///< Optional field
} S1ap_E_RABReleaseResponseIEs_t;

typedef struct S1ap_UplinkNonUEAssociatedLPPaTransport_IEs_s {
    S1ap_Routing_ID_t routing_ID;
    S1ap_LPPa_PDU_t   lpPa_PDU;
} S1ap_UplinkNonUEAssociatedLPPaTransport_IEs_t;

typedef struct S1ap_E_RABToBeSetupItemHOReqIEs_s {
    S1ap_E_RABToBeSetupItemHOReq_t e_RABToBeSetupItemHOReq;
} S1ap_E_RABToBeSetupItemHOReqIEs_t;

typedef struct S1ap_TraceStartIEs_s {
    S1ap_MME_UE_S1AP_ID_t  mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t  eNB_UE_S1AP_ID;
    S1ap_TraceActivation_t traceActivation;
} S1ap_TraceStartIEs_t;

#define S1AP_WRITEREPLACEWARNINGRESPONSEIES_BROADCASTCOMPLETEDAREALIST_PRESENT      (1 << 0)
#define S1AP_WRITEREPLACEWARNINGRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT          (1 << 1)

typedef struct S1ap_WriteReplaceWarningResponseIEs_s {
    uint16_t                          presenceMask;
    S1ap_MessageIdentifier_t          messageIdentifier;
    S1ap_SerialNumber_t               serialNumber;
    S1ap_BroadcastCompletedAreaList_t broadcastCompletedAreaList; ///< Optional field
    S1ap_CriticalityDiagnostics_t     criticalityDiagnostics; ///< Optional field
} S1ap_WriteReplaceWarningResponseIEs_t;

typedef struct S1ap_HandoverCancelIEs_s {
    S1ap_MME_UE_S1AP_ID_t mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t eNB_UE_S1AP_ID;
    S1ap_Cause_t          cause;
} S1ap_HandoverCancelIEs_t;

#define S1AP_HANDOVERPREPARATIONFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 0)

typedef struct S1ap_HandoverPreparationFailureIEs_s {
    uint16_t                      presenceMask;
    S1ap_MME_UE_S1AP_ID_t         mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t         eNB_UE_S1AP_ID;
    S1ap_Cause_t                  cause;
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_HandoverPreparationFailureIEs_t;

typedef struct S1ap_E_RABToBeModifiedItemBearerModReqIEs_s {
    S1ap_E_RABToBeModifiedItemBearerModReq_t e_RABToBeModifiedItemBearerModReq;
} S1ap_E_RABToBeModifiedItemBearerModReqIEs_t;

#define S1AP_UECONTEXTRELEASEREQUEST_IES_GWCONTEXTRELEASEINDICATION_PRESENT      (1 << 0)

typedef struct S1ap_UEContextReleaseRequest_IEs_s {
    uint16_t                          presenceMask;
    S1ap_MME_UE_S1AP_ID_t             mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t             eNB_UE_S1AP_ID;
    S1ap_Cause_t                      cause;
    S1ap_GWContextReleaseIndication_t gwContextReleaseIndication; ///< Optional field
} S1ap_UEContextReleaseRequest_IEs_t;

typedef struct S1ap_E_RABAdmittedItemIEs_s {
    S1ap_E_RABAdmittedItem_t e_RABAdmittedItem;
} S1ap_E_RABAdmittedItemIEs_t;

#define S1AP_INITIALUEMESSAGE_IES_S_TMSI_PRESENT                       (1 << 0)
#define S1AP_INITIALUEMESSAGE_IES_CSG_ID_PRESENT                       (1 << 1)
#define S1AP_INITIALUEMESSAGE_IES_GUMMEI_ID_PRESENT                    (1 << 2)
#define S1AP_INITIALUEMESSAGE_IES_CELLACCESSMODE_PRESENT               (1 << 3)
#define S1AP_INITIALUEMESSAGE_IES_GW_TRANSPORTLAYERADDRESS_PRESENT     (1 << 4)
#define S1AP_INITIALUEMESSAGE_IES_RELAYNODE_INDICATOR_PRESENT          (1 << 5)

typedef struct S1ap_InitialUEMessage_IEs_s {
    uint16_t                       presenceMask;
    S1ap_ENB_UE_S1AP_ID_t          eNB_UE_S1AP_ID;
    S1ap_NAS_PDU_t                 nas_pdu;
    S1ap_TAI_t                     tai;
    S1ap_EUTRAN_CGI_t              eutran_cgi;
    S1ap_RRC_Establishment_Cause_t rrC_Establishment_Cause;
    S1ap_S_TMSI_t                  s_tmsi; ///< Optional field
    S1ap_CSG_Id_t                  csG_Id; ///< Optional field
    S1ap_GUMMEI_t                  gummei_id; ///< Optional field
    S1ap_CellAccessMode_t          cellAccessMode; ///< Optional field
    S1ap_TransportLayerAddress_t   gW_TransportLayerAddress; ///< Optional field
    S1ap_RelayNode_Indicator_t     relayNode_Indicator; ///< Optional field
} S1ap_InitialUEMessage_IEs_t;

#define S1AP_KILLREQUESTIES_WARNINGAREALIST_PRESENT        (1 << 0)

typedef struct S1ap_KillRequestIEs_s {
    uint16_t                 presenceMask;
    S1ap_MessageIdentifier_t messageIdentifier;
    S1ap_SerialNumber_t      serialNumber;
    S1ap_WarningAreaList_t   warningAreaList; ///< Optional field
} S1ap_KillRequestIEs_t;

#define S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGAREALIST_PRESENT                        (1 << 0)
#define S1AP_WRITEREPLACEWARNINGREQUESTIES_EXTENDEDREPETITIONPERIOD_PRESENT               (1 << 1)
#define S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGTYPE_PRESENT                            (1 << 2)
#define S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGSECURITYINFO_PRESENT                    (1 << 3)
#define S1AP_WRITEREPLACEWARNINGREQUESTIES_DATACODINGSCHEME_PRESENT                       (1 << 4)
#define S1AP_WRITEREPLACEWARNINGREQUESTIES_WARNINGMESSAGECONTENTS_PRESENT                 (1 << 5)
#define S1AP_WRITEREPLACEWARNINGREQUESTIES_CONCURRENTWARNINGMESSAGEINDICATOR_PRESENT      (1 << 6)

typedef struct S1ap_WriteReplaceWarningRequestIEs_s {
    uint16_t                                 presenceMask;
    S1ap_MessageIdentifier_t                 messageIdentifier;
    S1ap_SerialNumber_t                      serialNumber;
    S1ap_WarningAreaList_t                   warningAreaList; ///< Optional field
    S1ap_RepetitionPeriod_t                  repetitionPeriod;
    S1ap_ExtendedRepetitionPeriod_t          extendedRepetitionPeriod; ///< Optional field
    S1ap_NumberofBroadcastRequest_t          numberofBroadcastRequest;
    S1ap_WarningType_t                       warningType; ///< Optional field
    S1ap_WarningSecurityInfo_t               warningSecurityInfo; ///< Optional field
    S1ap_DataCodingScheme_t                  dataCodingScheme; ///< Optional field
    S1ap_WarningMessageContents_t            warningMessageContents; ///< Optional field
    S1ap_ConcurrentWarningMessageIndicator_t concurrentWarningMessageIndicator; ///< Optional field
} S1ap_WriteReplaceWarningRequestIEs_t;

typedef struct S1ap_UECapabilityInfoIndicationIEs_s {
    S1ap_MME_UE_S1AP_ID_t    mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t    eNB_UE_S1AP_ID;
    S1ap_UERadioCapability_t ueRadioCapability;
} S1ap_UECapabilityInfoIndicationIEs_t;

typedef struct S1ap_E_RABToBeSetupItemBearerSUReqIEs_s {
    S1ap_E_RABToBeSetupItemBearerSUReq_t e_RABToBeSetupItemBearerSUReq;
} S1ap_E_RABToBeSetupItemBearerSUReqIEs_t;

typedef struct S1ap_E_RABDataForwardingItemIEs_s {
    S1ap_E_RABDataForwardingItem_t e_RABDataForwardingItem;
} S1ap_E_RABDataForwardingItemIEs_t;

typedef struct S1ap_LocationReportingFailureIndicationIEs_s {
    S1ap_MME_UE_S1AP_ID_t mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t eNB_UE_S1AP_ID;
    S1ap_Cause_t          cause;
} S1ap_LocationReportingFailureIndicationIEs_t;

#define S1AP_INITIALCONTEXTSETUPREQUESTIES_TRACEACTIVATION_PRESENT                  (1 << 0)
#define S1AP_INITIALCONTEXTSETUPREQUESTIES_HANDOVERRESTRICTIONLIST_PRESENT          (1 << 1)
#define S1AP_INITIALCONTEXTSETUPREQUESTIES_UERADIOCAPABILITY_PRESENT                (1 << 2)
#define S1AP_INITIALCONTEXTSETUPREQUESTIES_SUBSCRIBERPROFILEIDFORRFP_PRESENT        (1 << 3)
#define S1AP_INITIALCONTEXTSETUPREQUESTIES_CSFALLBACKINDICATOR_PRESENT              (1 << 4)
#define S1AP_INITIALCONTEXTSETUPREQUESTIES_SRVCCOPERATIONPOSSIBLE_PRESENT           (1 << 5)
#define S1AP_INITIALCONTEXTSETUPREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT              (1 << 6)
#define S1AP_INITIALCONTEXTSETUPREQUESTIES_REGISTEREDLAI_PRESENT                    (1 << 7)
#define S1AP_INITIALCONTEXTSETUPREQUESTIES_GUMMEI_ID_PRESENT                        (1 << 8)
#define S1AP_INITIALCONTEXTSETUPREQUESTIES_MME_UE_S1AP_ID_2_PRESENT                 (1 << 9)
#define S1AP_INITIALCONTEXTSETUPREQUESTIES_MANAGEMENTBASEDMDTALLOWED_PRESENT        (1 << 10)

typedef struct S1ap_InitialContextSetupRequestIEs_s {
    uint16_t                           presenceMask;
    S1ap_MME_UE_S1AP_ID_t              mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t              eNB_UE_S1AP_ID;
    S1ap_UEAggregateMaximumBitrate_t   uEaggregateMaximumBitrate;
    S1ap_E_RABToBeSetupListCtxtSUReq_IEs_t e_RABToBeSetupListCtxtSUReq;
    S1ap_UESecurityCapabilities_t      ueSecurityCapabilities;
    S1ap_SecurityKey_t                 securityKey;
    S1ap_TraceActivation_t             traceActivation; ///< Optional field
    S1ap_HandoverRestrictionList_t     handoverRestrictionList; ///< Optional field
    S1ap_UERadioCapability_t           ueRadioCapability; ///< Optional field
    S1ap_SubscriberProfileIDforRFP_t   subscriberProfileIDforRFP; ///< Optional field
    S1ap_CSFallbackIndicator_t         csFallbackIndicator; ///< Optional field
    S1ap_SRVCCOperationPossible_t      srvccOperationPossible; ///< Optional field
    S1ap_CSGMembershipStatus_t         csgMembershipStatus; ///< Optional field
    S1ap_LAI_t                         registeredLAI; ///< Optional field
    S1ap_GUMMEI_t                      gummei_id; ///< Optional field
    S1ap_MME_UE_S1AP_ID_t              mme_ue_s1ap_id_2; ///< Optional field
    S1ap_ManagementBasedMDTAllowed_t   managementBasedMDTAllowed; ///< Optional field
} S1ap_InitialContextSetupRequestIEs_t;

typedef struct S1ap_LocationReportIEs_s {
    S1ap_MME_UE_S1AP_ID_t mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t eNB_UE_S1AP_ID;
    S1ap_EUTRAN_CGI_t     eutran_cgi;
    S1ap_TAI_t            tai;
    S1ap_RequestType_t    requestType;
} S1ap_LocationReportIEs_t;

#define S1AP_UPLINKNASTRANSPORT_IES_GW_TRANSPORTLAYERADDRESS_PRESENT   (1 << 0)

typedef struct S1ap_UplinkNASTransport_IEs_s {
    uint16_t                     presenceMask;
    S1ap_MME_UE_S1AP_ID_t        mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t        eNB_UE_S1AP_ID;
    S1ap_NAS_PDU_t               nas_pdu;
    S1ap_EUTRAN_CGI_t            eutran_cgi;
    S1ap_TAI_t                   tai;
    S1ap_TransportLayerAddress_t gW_TransportLayerAddress; ///< Optional field
} S1ap_UplinkNASTransport_IEs_t;

#define S1AP_UECONTEXTMODIFICATIONRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 0)

typedef struct S1ap_UEContextModificationResponseIEs_s {
    uint16_t                      presenceMask;
    S1ap_MME_UE_S1AP_ID_t         mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t         eNB_UE_S1AP_ID;
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_UEContextModificationResponseIEs_t;

#define S1AP_UECONTEXTMODIFICATIONREQUESTIES_SECURITYKEY_PRESENT                    (1 << 0)
#define S1AP_UECONTEXTMODIFICATIONREQUESTIES_SUBSCRIBERPROFILEIDFORRFP_PRESENT      (1 << 1)
#define S1AP_UECONTEXTMODIFICATIONREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT      (1 << 2)
#define S1AP_UECONTEXTMODIFICATIONREQUESTIES_CSFALLBACKINDICATOR_PRESENT            (1 << 3)
#define S1AP_UECONTEXTMODIFICATIONREQUESTIES_UESECURITYCAPABILITIES_PRESENT         (1 << 4)
#define S1AP_UECONTEXTMODIFICATIONREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT            (1 << 5)
#define S1AP_UECONTEXTMODIFICATIONREQUESTIES_REGISTEREDLAI_PRESENT                  (1 << 6)

typedef struct S1ap_UEContextModificationRequestIEs_s {
    uint16_t                         presenceMask;
    S1ap_MME_UE_S1AP_ID_t            mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t            eNB_UE_S1AP_ID;
    S1ap_SecurityKey_t               securityKey; ///< Optional field
    S1ap_SubscriberProfileIDforRFP_t subscriberProfileIDforRFP; ///< Optional field
    S1ap_UEAggregateMaximumBitrate_t uEaggregateMaximumBitrate; ///< Optional field
    S1ap_CSFallbackIndicator_t       csFallbackIndicator; ///< Optional field
    S1ap_UESecurityCapabilities_t    ueSecurityCapabilities; ///< Optional field
    S1ap_CSGMembershipStatus_t       csgMembershipStatus; ///< Optional field
    S1ap_LAI_t                       registeredLAI; ///< Optional field
} S1ap_UEContextModificationRequestIEs_t;

typedef struct S1ap_LocationReportingControlIEs_s {
    S1ap_MME_UE_S1AP_ID_t mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t eNB_UE_S1AP_ID;
    S1ap_RequestType_t    requestType;
} S1ap_LocationReportingControlIEs_t;

#define S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000HOREQUIREDINDICATION_PRESENT            (1 << 0)
#define S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000ONEXSRVCCINFO_PRESENT                   (1 << 1)
#define S1AP_UPLINKS1CDMA2000TUNNELINGIES_CDMA2000ONEXRAND_PRESENT                        (1 << 2)
#define S1AP_UPLINKS1CDMA2000TUNNELINGIES_EUTRANROUNDTRIPDELAYESTIMATIONINFO_PRESENT      (1 << 3)

typedef struct S1ap_UplinkS1cdma2000tunnelingIEs_s {
    uint16_t                                  presenceMask;
    S1ap_MME_UE_S1AP_ID_t                     mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t                     eNB_UE_S1AP_ID;
    S1ap_Cdma2000RATType_t                    cdma2000RATType;
    S1ap_Cdma2000SectorID_t                   cdma2000SectorID;
    S1ap_Cdma2000HORequiredIndication_t       cdma2000HORequiredIndication; ///< Optional field
    S1ap_Cdma2000OneXSRVCCInfo_t              cdma2000OneXSRVCCInfo; ///< Optional field
    S1ap_Cdma2000OneXRAND_t                   cdma2000OneXRAND; ///< Optional field
    S1ap_Cdma2000PDU_t                        cdma2000PDU;
    S1ap_EUTRANRoundTripDelayEstimationInfo_t eutranRoundTripDelayEstimationInfo; ///< Optional field
} S1ap_UplinkS1cdma2000tunnelingIEs_t;

#define S1AP_HANDOVERREQUESTIES_HANDOVERRESTRICTIONLIST_PRESENT                   (1 << 0)
#define S1AP_HANDOVERREQUESTIES_TRACEACTIVATION_PRESENT                           (1 << 1)
#define S1AP_HANDOVERREQUESTIES_REQUESTTYPE_PRESENT                               (1 << 2)
#define S1AP_HANDOVERREQUESTIES_SRVCCOPERATIONPOSSIBLE_PRESENT                    (1 << 3)
#define S1AP_HANDOVERREQUESTIES_CSG_ID_PRESENT                                    (1 << 4)
#define S1AP_HANDOVERREQUESTIES_CSGMEMBERSHIPSTATUS_PRESENT                       (1 << 5)
#define S1AP_HANDOVERREQUESTIES_GUMMEI_ID_PRESENT                                 (1 << 6)
#define S1AP_HANDOVERREQUESTIES_MME_UE_S1AP_ID_2_PRESENT                          (1 << 7)
#define S1AP_HANDOVERREQUESTIES_MANAGEMENTBASEDMDTALLOWED_PRESENT                 (1 << 8)

typedef struct S1ap_HandoverRequestIEs_s {
    uint16_t                                    presenceMask;
    S1ap_MME_UE_S1AP_ID_t                       mme_ue_s1ap_id;
    S1ap_HandoverType_t                         handoverType;
    S1ap_Cause_t                                cause;
    S1ap_UEAggregateMaximumBitrate_t            uEaggregateMaximumBitrate;
    S1ap_E_RABToBeSetupListHOReq_IEs_t e_RABToBeSetupListHOReq;
    S1ap_Source_ToTarget_TransparentContainer_t source_ToTarget_TransparentContainer;
    S1ap_UESecurityCapabilities_t               ueSecurityCapabilities;
    S1ap_HandoverRestrictionList_t              handoverRestrictionList; ///< Optional field
    S1ap_TraceActivation_t                      traceActivation; ///< Optional field
    S1ap_RequestType_t                          requestType; ///< Optional field
    S1ap_SRVCCOperationPossible_t               srvccOperationPossible; ///< Optional field
    S1ap_SecurityContext_t                      securityContext;
    S1ap_CSG_Id_t                               csG_Id; ///< Optional field
    S1ap_CSGMembershipStatus_t                  csgMembershipStatus; ///< Optional field
    S1ap_GUMMEI_t                               gummei_id; ///< Optional field
    S1ap_MME_UE_S1AP_ID_t                       mme_ue_s1ap_id_2; ///< Optional field
    S1ap_ManagementBasedMDTAllowed_t            managementBasedMDTAllowed; ///< Optional field
} S1ap_HandoverRequestIEs_t;

#define S1AP_HANDOVERREQUIREDIES_DIRECT_FORWARDING_PATH_AVAILABILITY_PRESENT       (1 << 0)
#define S1AP_HANDOVERREQUIREDIES_SRVCCHOINDICATION_PRESENT                         (1 << 1)
#define S1AP_HANDOVERREQUIREDIES_SOURCE_TOTARGET_TRANSPARENTCONTAINER_SECONDARY_PRESENT (1 << 2)
#define S1AP_HANDOVERREQUIREDIES_MSCLASSMARK2_PRESENT                              (1 << 3)
#define S1AP_HANDOVERREQUIREDIES_MSCLASSMARK3_PRESENT                              (1 << 4)
#define S1AP_HANDOVERREQUIREDIES_CSG_ID_PRESENT                                    (1 << 5)
#define S1AP_HANDOVERREQUIREDIES_CELLACCESSMODE_PRESENT                            (1 << 6)
#define S1AP_HANDOVERREQUIREDIES_PS_SERVICENOTAVAILABLE_PRESENT                    (1 << 7)

typedef struct S1ap_HandoverRequiredIEs_s {
    uint16_t                                    presenceMask;
    S1ap_MME_UE_S1AP_ID_t                       mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t                       eNB_UE_S1AP_ID;
    S1ap_HandoverType_t                         handoverType;
    S1ap_Cause_t                                cause;
    S1ap_TargetID_t                             targetID;
    S1ap_Direct_Forwarding_Path_Availability_t  direct_Forwarding_Path_Availability; ///< Optional field
    S1ap_SRVCCHOIndication_t                    srvcchoIndication; ///< Optional field
    S1ap_Source_ToTarget_TransparentContainer_t source_ToTarget_TransparentContainer;
    S1ap_Source_ToTarget_TransparentContainer_t source_ToTarget_TransparentContainer_Secondary; ///< Optional field
    S1ap_MSClassmark2_t                         msClassmark2; ///< Conditional field
    S1ap_MSClassmark3_t                         msClassmark3; ///< Conditional field
    S1ap_CSG_Id_t                               csG_Id; ///< Optional field
    S1ap_CellAccessMode_t                       cellAccessMode; ///< Optional field
    S1ap_PS_ServiceNotAvailable_t               pS_ServiceNotAvailable; ///< Optional field
} S1ap_HandoverRequiredIEs_t;

typedef struct S1ap_E_RABToBeSwitchedDLItemIEs_s {
    S1ap_E_RABToBeSwitchedDLItem_t e_RABToBeSwitchedDLItem;
} S1ap_E_RABToBeSwitchedDLItemIEs_t;

#define S1AP_MMECONFIGURATIONUPDATEACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 0)

typedef struct S1ap_MMEConfigurationUpdateAcknowledgeIEs_s {
    uint16_t                      presenceMask;
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_MMEConfigurationUpdateAcknowledgeIEs_t;

#define S1AP_KILLRESPONSEIES_BROADCASTCANCELLEDAREALIST_PRESENT      (1 << 0)
#define S1AP_KILLRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT          (1 << 1)

typedef struct S1ap_KillResponseIEs_s {
    uint16_t                          presenceMask;
    S1ap_MessageIdentifier_t          messageIdentifier;
    S1ap_SerialNumber_t               serialNumber;
    S1ap_BroadcastCancelledAreaList_t broadcastCancelledAreaList; ///< Optional field
    S1ap_CriticalityDiagnostics_t     criticalityDiagnostics; ///< Optional field
} S1ap_KillResponseIEs_t;

typedef struct S1ap_MMEStatusTransferIEs_s {
    S1ap_MME_UE_S1AP_ID_t                          mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t                          eNB_UE_S1AP_ID;
    S1ap_ENB_StatusTransfer_TransparentContainer_t eNB_StatusTransfer_TransparentContainer;
} S1ap_MMEStatusTransferIEs_t;

#define S1AP_MMECONFIGURATIONUPDATEIES_MMENAME_PRESENT                  (1 << 0)
#define S1AP_MMECONFIGURATIONUPDATEIES_SERVEDGUMMEIS_PRESENT            (1 << 1)
#define S1AP_MMECONFIGURATIONUPDATEIES_RELATIVEMMECAPACITY_PRESENT      (1 << 2)

typedef struct S1ap_MMEConfigurationUpdateIEs_s {
    uint16_t                   presenceMask;
    S1ap_MMEname_t             mmEname; ///< Optional field
    S1ap_ServedGUMMEIs_t       servedGUMMEIs; ///< Optional field
    S1ap_RelativeMMECapacity_t relativeMMECapacity; ///< Optional field
} S1ap_MMEConfigurationUpdateIEs_t;

#define S1AP_E_RABMODIFYREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT              (1 << 0)

typedef struct S1ap_E_RABModifyRequestIEs_s {
    uint16_t                                 presenceMask;
    S1ap_MME_UE_S1AP_ID_t                    mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t                    eNB_UE_S1AP_ID;
    S1ap_UEAggregateMaximumBitrate_t         uEaggregateMaximumBitrate; ///< Optional field
    S1ap_E_RABToBeModifiedListBearerModReq_IEs_t e_RABToBeModifiedListBearerModReq;
} S1ap_E_RABModifyRequestIEs_t;

typedef struct S1ap_E_RABSetupItemCtxtSUResIEs_s {
    S1ap_E_RABSetupItemCtxtSURes_t e_RABSetupItemCtxtSURes;
} S1ap_E_RABSetupItemCtxtSUResIEs_t;

typedef struct S1ap_E_RABToBeSwitchedULItemIEs_s {
    S1ap_E_RABToBeSwitchedULItem_t e_RABToBeSwitchedULItem;
} S1ap_E_RABToBeSwitchedULItemIEs_t;

#define S1AP_ENBCONFIGURATIONUPDATEFAILUREIES_TIMETOWAIT_PRESENT                  (1 << 0)
#define S1AP_ENBCONFIGURATIONUPDATEFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 1)

typedef struct S1ap_ENBConfigurationUpdateFailureIEs_s {
    uint16_t                      presenceMask;
    S1ap_Cause_t                  cause;
    S1ap_TimeToWait_t             timeToWait; ///< Optional field
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_ENBConfigurationUpdateFailureIEs_t;

typedef struct S1ap_ResetIEs_s {
    S1ap_Cause_t     cause;
    S1ap_ResetType_t resetType;
} S1ap_ResetIEs_t;

typedef struct S1ap_Bearers_SubjectToStatusTransfer_ItemIEs_s {
    S1ap_Bearers_SubjectToStatusTransfer_Item_t bearers_SubjectToStatusTransfer_Item;
} S1ap_Bearers_SubjectToStatusTransfer_ItemIEs_t;

typedef struct S1ap_E_RABItemIEs_s {
    S1ap_E_RABItem_t e_RABItem;
} S1ap_E_RABItemIEs_t;

#define S1AP_S1SETUPRESPONSEIES_MMENAME_PRESENT                       (1 << 0)
#define S1AP_S1SETUPRESPONSEIES_MMERELAYSUPPORTINDICATOR_PRESENT      (1 << 1)
#define S1AP_S1SETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT        (1 << 2)

typedef struct S1ap_S1SetupResponseIEs_s {
    uint16_t                        presenceMask;
    S1ap_MMEname_t                  mmEname; ///< Optional field
    S1ap_ServedGUMMEIs_t            servedGUMMEIs;
    S1ap_RelativeMMECapacity_t      relativeMMECapacity;
    S1ap_MMERelaySupportIndicator_t mmeRelaySupportIndicator; ///< Optional field
    S1ap_CriticalityDiagnostics_t   criticalityDiagnostics; ///< Optional field
} S1ap_S1SetupResponseIEs_t;

#define S1AP_PAGINGIES_PAGINGDRX_PRESENT                 (1 << 0)
#define S1AP_PAGINGIES_CSG_IDLIST_PRESENT                (1 << 1)
#define S1AP_PAGINGIES_PAGINGPRIORITY_PRESENT            (1 << 2)

typedef struct S1ap_PagingIEs_s {
    uint16_t                    presenceMask;
    S1ap_UEIdentityIndexValue_t ueIdentityIndexValue;
    S1ap_UEPagingID_t           uePagingID;
    S1ap_PagingDRX_t            pagingDRX; ///< Optional field
    S1ap_CNDomain_t             cnDomain;
    S1ap_TAIList_IEs_t taiList;
    S1ap_CSG_IdList_t           csG_IdList; ///< Optional field
    S1ap_PagingPriority_t       pagingPriority; ///< Optional field
} S1ap_PagingIEs_t;

#define S1AP_ENBCONFIGURATIONUPDATEACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 0)

typedef struct S1ap_ENBConfigurationUpdateAcknowledgeIEs_s {
    uint16_t                      presenceMask;
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_ENBConfigurationUpdateAcknowledgeIEs_t;

#define S1AP_E_RABRELEASECOMMANDIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT      (1 << 0)
#define S1AP_E_RABRELEASECOMMANDIES_NAS_PDU_PRESENT                        (1 << 1)

typedef struct S1ap_E_RABReleaseCommandIEs_s {
    uint16_t                         presenceMask;
    S1ap_MME_UE_S1AP_ID_t            mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t            eNB_UE_S1AP_ID;
    S1ap_UEAggregateMaximumBitrate_t uEaggregateMaximumBitrate; ///< Optional field
    S1ap_E_RABList_IEs_t e_RABToBeReleasedList;
    S1ap_NAS_PDU_t                   nas_pdu; ///< Optional field
} S1ap_E_RABReleaseCommandIEs_t;

#define S1AP_PATHSWITCHREQUESTFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 0)

typedef struct S1ap_PathSwitchRequestFailureIEs_s {
    uint16_t                      presenceMask;
    S1ap_MME_UE_S1AP_ID_t         mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t         eNB_UE_S1AP_ID;
    S1ap_Cause_t                  cause;
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_PathSwitchRequestFailureIEs_t;

#define S1AP_E_RABMODIFYRESPONSEIES_E_RABMODIFYLISTBEARERMODRES_PRESENT      (1 << 0)
#define S1AP_E_RABMODIFYRESPONSEIES_E_RABFAILEDTOMODIFYLIST_PRESENT          (1 << 1)
#define S1AP_E_RABMODIFYRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT           (1 << 2)

typedef struct S1ap_E_RABModifyResponseIEs_s {
    uint16_t                           presenceMask;
    S1ap_MME_UE_S1AP_ID_t              mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t              eNB_UE_S1AP_ID;
    S1ap_E_RABModifyListBearerModRes_IEs_t e_RABModifyListBearerModRes; ///< Optional field
    S1ap_E_RABList_IEs_t e_RABFailedToModifyList; ///< Optional field
    S1ap_CriticalityDiagnostics_t      criticalityDiagnostics; ///< Optional field
} S1ap_E_RABModifyResponseIEs_t;

typedef struct S1ap_HandoverNotifyIEs_s {
    S1ap_MME_UE_S1AP_ID_t mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t eNB_UE_S1AP_ID;
    S1ap_EUTRAN_CGI_t     eutran_cgi;
    S1ap_TAI_t            tai;
} S1ap_HandoverNotifyIEs_t;

#define S1AP_HANDOVERCANCELACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 0)

typedef struct S1ap_HandoverCancelAcknowledgeIEs_s {
    uint16_t                      presenceMask;
    S1ap_MME_UE_S1AP_ID_t         mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t         eNB_UE_S1AP_ID;
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_HandoverCancelAcknowledgeIEs_t;

#define S1AP_INITIALCONTEXTSETUPFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 0)

typedef struct S1ap_InitialContextSetupFailureIEs_s {
    uint16_t                      presenceMask;
    S1ap_MME_UE_S1AP_ID_t         mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t         eNB_UE_S1AP_ID;
    S1ap_Cause_t                  cause;
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_InitialContextSetupFailureIEs_t;

#define S1AP_HANDOVERCOMMANDIES_E_RABDATAFORWARDINGLIST_PRESENT                   (1 << 0)
#define S1AP_HANDOVERCOMMANDIES_E_RABTORELEASELISTHOCMD_PRESENT                   (1 << 1)
#define S1AP_HANDOVERCOMMANDIES_TARGET_TOSOURCE_TRANSPARENTCONTAINER_SECONDARY_PRESENT (1 << 2)
#define S1AP_HANDOVERCOMMANDIES_CRITICALITYDIAGNOSTICS_PRESENT                    (1 << 3)

typedef struct S1ap_HandoverCommandIEs_s {
    uint16_t                                    presenceMask;
    S1ap_MME_UE_S1AP_ID_t                       mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t                       eNB_UE_S1AP_ID;
    S1ap_HandoverType_t                         handoverType;
    S1ap_E_RABDataForwardingList_IEs_t e_RABDataForwardingList; ///< Optional field
    S1ap_E_RABList_IEs_t e_RABtoReleaseListHOCmd; ///< Optional field
    S1ap_Target_ToSource_TransparentContainer_t target_ToSource_TransparentContainer;
    S1ap_Target_ToSource_TransparentContainer_t target_ToSource_TransparentContainer_Secondary; ///< Optional field
    S1ap_CriticalityDiagnostics_t               criticalityDiagnostics; ///< Optional field
} S1ap_HandoverCommandIEs_t;

typedef struct S1ap_MMEDirectInformationTransferIEs_s {
    S1ap_Inter_SystemInformationTransferType_t inter_SystemInformationTransferTypeMDT;
} S1ap_MMEDirectInformationTransferIEs_t;

#define S1AP_ERRORINDICATIONIES_MME_UE_S1AP_ID_PRESENT              (1 << 0)
#define S1AP_ERRORINDICATIONIES_ENB_UE_S1AP_ID_PRESENT              (1 << 1)
#define S1AP_ERRORINDICATIONIES_CAUSE_PRESENT                       (1 << 2)
#define S1AP_ERRORINDICATIONIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 3)

typedef struct S1ap_ErrorIndicationIEs_s {
    uint16_t                      presenceMask;
    S1ap_MME_UE_S1AP_ID_t         mme_ue_s1ap_id; ///< Optional field
    S1ap_ENB_UE_S1AP_ID_t         eNB_UE_S1AP_ID; ///< Optional field
    S1ap_Cause_t                  cause; ///< Optional field
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_ErrorIndicationIEs_t;

typedef struct S1ap_TAIItemIEs_s {
    S1ap_TAIItem_t taiItem;
} S1ap_TAIItemIEs_t;

#define S1AP_OVERLOADSTARTIES_GUMMEILIST_PRESENT                          (1 << 0)
#define S1AP_OVERLOADSTARTIES_TRAFFICLOADREDUCTIONINDICATION_PRESENT      (1 << 1)

typedef struct S1ap_OverloadStartIEs_s {
    uint16_t                              presenceMask;
    S1ap_OverloadResponse_t               overloadResponse;
    S1ap_GUMMEIList_t                     gummeiList; ///< Optional field
    S1ap_TrafficLoadReductionIndication_t trafficLoadReductionIndication; ///< Optional field
} S1ap_OverloadStartIEs_t;

typedef struct S1ap_DownlinkNonUEAssociatedLPPaTransport_IEs_s {
    S1ap_Routing_ID_t routing_ID;
    S1ap_LPPa_PDU_t   lpPa_PDU;
} S1ap_DownlinkNonUEAssociatedLPPaTransport_IEs_t;

#define S1AP_HANDOVERREQUESTACKNOWLEDGEIES_E_RABFAILEDTOSETUPLISTHOREQACK_PRESENT            (1 << 0)
#define S1AP_HANDOVERREQUESTACKNOWLEDGEIES_CSG_ID_PRESENT                                    (1 << 1)
#define S1AP_HANDOVERREQUESTACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT                    (1 << 2)

typedef struct S1ap_HandoverRequestAcknowledgeIEs_s {
    uint16_t                                    presenceMask;
    S1ap_MME_UE_S1AP_ID_t                       mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t                       eNB_UE_S1AP_ID;
    S1ap_E_RABAdmittedList_IEs_t e_RABAdmittedList;
    S1ap_E_RABFailedtoSetupListHOReqAck_IEs_t e_RABFailedToSetupListHOReqAck; ///< Optional field
    S1ap_Target_ToSource_TransparentContainer_t target_ToSource_TransparentContainer;
    S1ap_CSG_Id_t                               csG_Id; ///< Optional field
    S1ap_CriticalityDiagnostics_t               criticalityDiagnostics; ///< Optional field
} S1ap_HandoverRequestAcknowledgeIEs_t;

typedef struct S1ap_DownlinkUEAssociatedLPPaTransport_IEs_s {
    S1ap_MME_UE_S1AP_ID_t mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t eNB_UE_S1AP_ID;
    S1ap_Routing_ID_t     routing_ID;
    S1ap_LPPa_PDU_t       lpPa_PDU;
} S1ap_DownlinkUEAssociatedLPPaTransport_IEs_t;

#define S1AP_UECONTEXTMODIFICATIONFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 0)

typedef struct S1ap_UEContextModificationFailureIEs_s {
    uint16_t                      presenceMask;
    S1ap_MME_UE_S1AP_ID_t         mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t         eNB_UE_S1AP_ID;
    S1ap_Cause_t                  cause;
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_UEContextModificationFailureIEs_t;

#define S1AP_MMECONFIGURATIONTRANSFERIES_SONCONFIGURATIONTRANSFERMCT_PRESENT   (1 << 0)

typedef struct S1ap_MMEConfigurationTransferIEs_s {
    uint16_t                        presenceMask;
    S1ap_SONConfigurationTransfer_t sonConfigurationTransferMCT; ///< Optional field
} S1ap_MMEConfigurationTransferIEs_t;

#define S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT      (1 << 0)
#define S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_E_RABTOBESWITCHEDULLIST_PRESENT        (1 << 1)
#define S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_E_RABTOBERELEASEDLIST_PRESENT          (1 << 2)
#define S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT         (1 << 3)
#define S1AP_PATHSWITCHREQUESTACKNOWLEDGEIES_MME_UE_S1AP_ID_2_PRESENT               (1 << 4)

typedef struct S1ap_PathSwitchRequestAcknowledgeIEs_s {
    uint16_t                         presenceMask;
    S1ap_MME_UE_S1AP_ID_t            mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t            eNB_UE_S1AP_ID;
    S1ap_UEAggregateMaximumBitrate_t uEaggregateMaximumBitrate; ///< Optional field
    S1ap_E_RABToBeSwitchedULList_IEs_t e_RABToBeSwitchedULList; ///< Optional field
    S1ap_E_RABList_IEs_t e_RABToBeReleasedList; ///< Optional field
    S1ap_SecurityContext_t           securityContext;
    S1ap_CriticalityDiagnostics_t    criticalityDiagnostics; ///< Optional field
    S1ap_MME_UE_S1AP_ID_t            mme_ue_s1ap_id_2; ///< Optional field
} S1ap_PathSwitchRequestAcknowledgeIEs_t;

typedef struct S1ap_UE_associatedLogicalS1_ConnectionItemRes_s {
    S1ap_UE_associatedLogicalS1_ConnectionItem_t uE_associatedLogicalS1_ConnectionItem;
} S1ap_UE_associatedLogicalS1_ConnectionItemRes_t;

#define S1AP_S1SETUPFAILUREIES_TIMETOWAIT_PRESENT                  (1 << 0)
#define S1AP_S1SETUPFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 1)

typedef struct S1ap_S1SetupFailureIEs_s {
    uint16_t                      presenceMask;
    S1ap_Cause_t                  cause;
    S1ap_TimeToWait_t             timeToWait; ///< Optional field
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_S1SetupFailureIEs_t;

#define S1AP_MMECONFIGURATIONUPDATEFAILUREIES_TIMETOWAIT_PRESENT                  (1 << 0)
#define S1AP_MMECONFIGURATIONUPDATEFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 1)

typedef struct S1ap_MMEConfigurationUpdateFailureIEs_s {
    uint16_t                      presenceMask;
    S1ap_Cause_t                  cause;
    S1ap_TimeToWait_t             timeToWait; ///< Optional field
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_MMEConfigurationUpdateFailureIEs_t;

#define S1AP_INITIALCONTEXTSETUPRESPONSEIES_E_RABFAILEDTOSETUPLISTCTXTSURES_PRESENT (1 << 0)
#define S1AP_INITIALCONTEXTSETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT       (1 << 1)

typedef struct S1ap_InitialContextSetupResponseIEs_s {
    uint16_t                       presenceMask;
    S1ap_MME_UE_S1AP_ID_t          mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t          eNB_UE_S1AP_ID;
    S1ap_E_RABSetupListCtxtSURes_IEs_t e_RABSetupListCtxtSURes;
    S1ap_E_RABList_IEs_t e_RABFailedToSetupListCtxtSURes; ///< Optional field
    S1ap_CriticalityDiagnostics_t  criticalityDiagnostics; ///< Optional field
} S1ap_InitialContextSetupResponseIEs_t;

#define S1AP_DOWNLINKS1CDMA2000TUNNELINGIES_E_RABDATAFORWARDINGLIST_PRESENT      (1 << 0)
#define S1AP_DOWNLINKS1CDMA2000TUNNELINGIES_CDMA2000HOSTATUS_PRESENT             (1 << 1)

typedef struct S1ap_DownlinkS1cdma2000tunnelingIEs_s {
    uint16_t                       presenceMask;
    S1ap_MME_UE_S1AP_ID_t          mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t          eNB_UE_S1AP_ID;
    S1ap_E_RABDataForwardingList_IEs_t e_RABDataForwardingList; ///< Optional field
    S1ap_Cdma2000HOStatus_t        cdma2000HOStatus; ///< Optional field
    S1ap_Cdma2000RATType_t         cdma2000RATType;
    S1ap_Cdma2000PDU_t             cdma2000PDU;
} S1ap_DownlinkS1cdma2000tunnelingIEs_t;

#define S1AP_OVERLOADSTOPIES_GUMMEILIST_PRESENT      (1 << 0)

typedef struct S1ap_OverloadStopIEs_s {
    uint16_t          presenceMask;
    S1ap_GUMMEIList_t gummeiList; ///< Optional field
} S1ap_OverloadStopIEs_t;

typedef struct S1ap_E_RABSetupItemBearerSUResIEs_s {
    S1ap_E_RABSetupItemBearerSURes_t e_RABSetupItemBearerSURes;
} S1ap_E_RABSetupItemBearerSUResIEs_t;

typedef struct S1ap_E_RABModifyItemBearerModResIEs_s {
    S1ap_E_RABModifyItemBearerModRes_t e_RABModifyItemBearerModRes;
} S1ap_E_RABModifyItemBearerModResIEs_t;

typedef struct S1ap_E_RABToBeSetupItemCtxtSUReqIEs_s {
    S1ap_E_RABToBeSetupItemCtxtSUReq_t e_RABToBeSetupItemCtxtSUReq;
} S1ap_E_RABToBeSetupItemCtxtSUReqIEs_t;

#define S1AP_ENBCONFIGURATIONUPDATEIES_ENBNAME_PRESENT           (1 << 0)
#define S1AP_ENBCONFIGURATIONUPDATEIES_SUPPORTEDTAS_PRESENT      (1 << 1)
#define S1AP_ENBCONFIGURATIONUPDATEIES_CSG_IDLIST_PRESENT        (1 << 2)
#define S1AP_ENBCONFIGURATIONUPDATEIES_DEFAULTPAGINGDRX_PRESENT  (1 << 3)

typedef struct S1ap_ENBConfigurationUpdateIEs_s {
    uint16_t            presenceMask;
    S1ap_ENBname_t      eNBname; ///< Optional field
    S1ap_SupportedTAs_t supportedTAs; ///< Optional field
    S1ap_CSG_IdList_t   csG_IdList; ///< Optional field
    S1ap_PagingDRX_t    defaultPagingDRX; ///< Optional field
} S1ap_ENBConfigurationUpdateIEs_t;

typedef struct S1ap_TraceFailureIndicationIEs_s {
    S1ap_MME_UE_S1AP_ID_t   mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t   eNB_UE_S1AP_ID;
    S1ap_E_UTRAN_Trace_ID_t e_UTRAN_Trace_ID;
    S1ap_Cause_t            cause;
} S1ap_TraceFailureIndicationIEs_t;

typedef struct S1ap_E_RABInformationListIEs_s {
    S1ap_E_RABInformationListItem_t e_RABInformationListItem;
} S1ap_E_RABInformationListIEs_t;

#define S1AP_ENBCONFIGURATIONTRANSFERIES_SONCONFIGURATIONTRANSFERECT_PRESENT   (1 << 0)

typedef struct S1ap_ENBConfigurationTransferIEs_s {
    uint16_t                        presenceMask;
    S1ap_SONConfigurationTransfer_t sonConfigurationTransferECT; ///< Optional field
} S1ap_ENBConfigurationTransferIEs_t;

#define S1AP_HANDOVERFAILUREIES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 0)

typedef struct S1ap_HandoverFailureIEs_s {
    uint16_t                      presenceMask;
    S1ap_MME_UE_S1AP_ID_t         mme_ue_s1ap_id;
    S1ap_Cause_t                  cause;
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_HandoverFailureIEs_t;

typedef struct S1ap_UE_associatedLogicalS1_ConnectionItemResAck_s {
    S1ap_UE_associatedLogicalS1_ConnectionItem_t uE_associatedLogicalS1_ConnectionItem;
} S1ap_UE_associatedLogicalS1_ConnectionItemResAck_t;

#define S1AP_E_RABSETUPRESPONSEIES_E_RABSETUPLISTBEARERSURES_PRESENT      (1 << 0)
#define S1AP_E_RABSETUPRESPONSEIES_E_RABFAILEDTOSETUPLISTBEARERSURES_PRESENT (1 << 1)
#define S1AP_E_RABSETUPRESPONSEIES_CRITICALITYDIAGNOSTICS_PRESENT         (1 << 2)

typedef struct S1ap_E_RABSetupResponseIEs_s {
    uint16_t                         presenceMask;
    S1ap_MME_UE_S1AP_ID_t            mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t            eNB_UE_S1AP_ID;
    S1ap_E_RABSetupListBearerSURes_IEs_t e_RABSetupListBearerSURes; ///< Optional field
    S1ap_E_RABList_IEs_t e_RABFailedToSetupListBearerSURes; ///< Optional field
    S1ap_CriticalityDiagnostics_t    criticalityDiagnostics; ///< Optional field
} S1ap_E_RABSetupResponseIEs_t;

typedef struct S1ap_UEContextReleaseCommand_IEs_s {
    S1ap_UE_S1AP_IDs_t uE_S1AP_IDs;
    S1ap_Cause_t       cause;
} S1ap_UEContextReleaseCommand_IEs_t;

#define S1AP_PATHSWITCHREQUESTIES_CSG_ID_PRESENT                       (1 << 0)
#define S1AP_PATHSWITCHREQUESTIES_CELLACCESSMODE_PRESENT               (1 << 1)
#define S1AP_PATHSWITCHREQUESTIES_SOURCEMME_GUMMEI_PRESENT             (1 << 2)

typedef struct S1ap_PathSwitchRequestIEs_s {
    uint16_t                       presenceMask;
    S1ap_ENB_UE_S1AP_ID_t          eNB_UE_S1AP_ID;
    S1ap_E_RABToBeSwitchedDLList_IEs_t e_RABToBeSwitchedDLList;
    S1ap_MME_UE_S1AP_ID_t          sourceMME_UE_S1AP_ID;
    S1ap_EUTRAN_CGI_t              eutran_cgi;
    S1ap_TAI_t                     tai;
    S1ap_UESecurityCapabilities_t  ueSecurityCapabilities;
    S1ap_CSG_Id_t                  csG_Id; ///< Optional field
    S1ap_CellAccessMode_t          cellAccessMode; ///< Optional field
    S1ap_GUMMEI_t                  sourceMME_GUMMEI; ///< Optional field
} S1ap_PathSwitchRequestIEs_t;

#define S1AP_DOWNLINKNASTRANSPORT_IES_HANDOVERRESTRICTIONLIST_PRESENT        (1 << 0)
#define S1AP_DOWNLINKNASTRANSPORT_IES_SUBSCRIBERPROFILEIDFORRFP_PRESENT      (1 << 1)

typedef struct S1ap_DownlinkNASTransport_IEs_s {
    uint16_t                         presenceMask;
    S1ap_MME_UE_S1AP_ID_t            mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t            eNB_UE_S1AP_ID;
    S1ap_NAS_PDU_t                   nas_pdu;
    S1ap_HandoverRestrictionList_t   handoverRestrictionList; ///< Optional field
    S1ap_SubscriberProfileIDforRFP_t subscriberProfileIDforRFP; ///< Optional field
} S1ap_DownlinkNASTransport_IEs_t;

typedef struct S1ap_E_RABReleaseIndicationIEs_s {
    S1ap_MME_UE_S1AP_ID_t mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t eNB_UE_S1AP_ID;
    S1ap_E_RABList_IEs_t e_RABReleasedList;
} S1ap_E_RABReleaseIndicationIEs_t;

#define S1AP_S1SETUPREQUESTIES_ENBNAME_PRESENT            (1 << 0)
#define S1AP_S1SETUPREQUESTIES_CSG_IDLIST_PRESENT         (1 << 1)

typedef struct S1ap_S1SetupRequestIEs_s {
    uint16_t             presenceMask;
    S1ap_Global_ENB_ID_t global_ENB_ID;
    S1ap_ENBname_t       eNBname; ///< Optional field
    S1ap_SupportedTAs_t  supportedTAs;
    S1ap_PagingDRX_t     defaultPagingDRX;
    S1ap_CSG_IdList_t    csG_IdList; ///< Optional field
} S1ap_S1SetupRequestIEs_t;

typedef struct S1ap_NASNonDeliveryIndication_IEs_s {
    S1ap_MME_UE_S1AP_ID_t mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t eNB_UE_S1AP_ID;
    S1ap_NAS_PDU_t        nas_pdu;
    S1ap_Cause_t          cause;
} S1ap_NASNonDeliveryIndication_IEs_t;

#define S1AP_CELLTRAFFICTRACEIES_PRIVACYINDICATOR_PRESENT           (1 << 0)

typedef struct S1ap_CellTrafficTraceIEs_s {
    uint16_t                     presenceMask;
    S1ap_MME_UE_S1AP_ID_t        mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t        eNB_UE_S1AP_ID;
    S1ap_E_UTRAN_Trace_ID_t      e_UTRAN_Trace_ID;
    S1ap_EUTRAN_CGI_t            eutran_cgi;
    S1ap_TransportLayerAddress_t traceCollectionEntityIPAddress;
    S1ap_PrivacyIndicator_t      privacyIndicator; ///< Optional field
} S1ap_CellTrafficTraceIEs_t;

#define S1AP_E_RABSETUPREQUESTIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT          (1 << 0)

typedef struct S1ap_E_RABSetupRequestIEs_s {
    uint16_t                             presenceMask;
    S1ap_MME_UE_S1AP_ID_t                mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t                eNB_UE_S1AP_ID;
    S1ap_UEAggregateMaximumBitrate_t     uEaggregateMaximumBitrate; ///< Optional field
    S1ap_E_RABToBeSetupListBearerSUReq_IEs_t e_RABToBeSetupListBearerSUReq;
} S1ap_E_RABSetupRequestIEs_t;

typedef struct S1ap_ENBStatusTransferIEs_s {
    S1ap_MME_UE_S1AP_ID_t                          mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t                          eNB_UE_S1AP_ID;
    S1ap_ENB_StatusTransfer_TransparentContainer_t eNB_StatusTransfer_TransparentContainer;
} S1ap_ENBStatusTransferIEs_t;

#define S1AP_RESETACKNOWLEDGEIES_UE_ASSOCIATEDLOGICALS1_CONNECTIONLISTRESACK_PRESENT      (1 << 0)
#define S1AP_RESETACKNOWLEDGEIES_CRITICALITYDIAGNOSTICS_PRESENT                           (1 << 1)

typedef struct S1ap_ResetAcknowledgeIEs_s {
    uint16_t                                           presenceMask;
    S1ap_UE_associatedLogicalS1_ConnectionListResAck_IEs_t uE_associatedLogicalS1_ConnectionListResAck; ///< Optional field
    S1ap_CriticalityDiagnostics_t                      criticalityDiagnostics; ///< Optional field
} S1ap_ResetAcknowledgeIEs_t;

#define S1AP_UECONTEXTRELEASECOMPLETE_IES_CRITICALITYDIAGNOSTICS_PRESENT      (1 << 0)

typedef struct S1ap_UEContextReleaseComplete_IEs_s {
    uint16_t                      presenceMask;
    S1ap_MME_UE_S1AP_ID_t         mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t         eNB_UE_S1AP_ID;
    S1ap_CriticalityDiagnostics_t criticalityDiagnostics; ///< Optional field
} S1ap_UEContextReleaseComplete_IEs_t;

typedef struct S1ap_UplinkUEAssociatedLPPaTransport_IEs_s {
    S1ap_MME_UE_S1AP_ID_t mme_ue_s1ap_id;
    S1ap_ENB_UE_S1AP_ID_t eNB_UE_S1AP_ID;
    S1ap_Routing_ID_t     routing_ID;
    S1ap_LPPa_PDU_t       lpPa_PDU;
} S1ap_UplinkUEAssociatedLPPaTransport_IEs_t;

typedef struct s1ap_message_s {
    S1ap_ProcedureCode_t procedureCode;
    S1ap_Criticality_t   criticality;
    uint8_t            direction;
    union {
        S1ap_CellTrafficTraceIEs_t s1ap_CellTrafficTraceIEs;
        S1ap_DeactivateTraceIEs_t s1ap_DeactivateTraceIEs;
        S1ap_DownlinkNASTransport_IEs_t s1ap_DownlinkNASTransport_IEs;
        S1ap_DownlinkNonUEAssociatedLPPaTransport_IEs_t s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs;
        S1ap_DownlinkS1cdma2000tunnelingIEs_t s1ap_DownlinkS1cdma2000tunnelingIEs;
        S1ap_DownlinkUEAssociatedLPPaTransport_IEs_t s1ap_DownlinkUEAssociatedLPPaTransport_IEs;
        S1ap_E_RABModifyRequestIEs_t s1ap_E_RABModifyRequestIEs;
        S1ap_E_RABModifyResponseIEs_t s1ap_E_RABModifyResponseIEs;
        S1ap_E_RABReleaseCommandIEs_t s1ap_E_RABReleaseCommandIEs;
        S1ap_E_RABReleaseIndicationIEs_t s1ap_E_RABReleaseIndicationIEs;
        S1ap_E_RABReleaseResponseIEs_t s1ap_E_RABReleaseResponseIEs;
        S1ap_E_RABSetupRequestIEs_t s1ap_E_RABSetupRequestIEs;
        S1ap_E_RABSetupResponseIEs_t s1ap_E_RABSetupResponseIEs;
        S1ap_ENBConfigurationTransferIEs_t s1ap_ENBConfigurationTransferIEs;
        S1ap_ENBConfigurationUpdateAcknowledgeIEs_t s1ap_ENBConfigurationUpdateAcknowledgeIEs;
        S1ap_ENBConfigurationUpdateFailureIEs_t s1ap_ENBConfigurationUpdateFailureIEs;
        S1ap_ENBConfigurationUpdateIEs_t s1ap_ENBConfigurationUpdateIEs;
        S1ap_ENBDirectInformationTransferIEs_t s1ap_ENBDirectInformationTransferIEs;
        S1ap_ENBStatusTransferIEs_t s1ap_ENBStatusTransferIEs;
        S1ap_ErrorIndicationIEs_t s1ap_ErrorIndicationIEs;
        S1ap_HandoverCancelAcknowledgeIEs_t s1ap_HandoverCancelAcknowledgeIEs;
        S1ap_HandoverCancelIEs_t s1ap_HandoverCancelIEs;
        S1ap_HandoverCommandIEs_t s1ap_HandoverCommandIEs;
        S1ap_HandoverFailureIEs_t s1ap_HandoverFailureIEs;
        S1ap_HandoverNotifyIEs_t s1ap_HandoverNotifyIEs;
        S1ap_HandoverPreparationFailureIEs_t s1ap_HandoverPreparationFailureIEs;
        S1ap_HandoverRequestAcknowledgeIEs_t s1ap_HandoverRequestAcknowledgeIEs;
        S1ap_HandoverRequestIEs_t s1ap_HandoverRequestIEs;
        S1ap_HandoverRequiredIEs_t s1ap_HandoverRequiredIEs;
        S1ap_InitialContextSetupFailureIEs_t s1ap_InitialContextSetupFailureIEs;
        S1ap_InitialContextSetupRequestIEs_t s1ap_InitialContextSetupRequestIEs;
        S1ap_InitialContextSetupResponseIEs_t s1ap_InitialContextSetupResponseIEs;
        S1ap_InitialUEMessage_IEs_t s1ap_InitialUEMessage_IEs;
        S1ap_KillRequestIEs_t s1ap_KillRequestIEs;
        S1ap_KillResponseIEs_t s1ap_KillResponseIEs;
        S1ap_LocationReportIEs_t s1ap_LocationReportIEs;
        S1ap_LocationReportingControlIEs_t s1ap_LocationReportingControlIEs;
        S1ap_LocationReportingFailureIndicationIEs_t s1ap_LocationReportingFailureIndicationIEs;
        S1ap_MMEConfigurationTransferIEs_t s1ap_MMEConfigurationTransferIEs;
        S1ap_MMEConfigurationUpdateAcknowledgeIEs_t s1ap_MMEConfigurationUpdateAcknowledgeIEs;
        S1ap_MMEConfigurationUpdateFailureIEs_t s1ap_MMEConfigurationUpdateFailureIEs;
        S1ap_MMEConfigurationUpdateIEs_t s1ap_MMEConfigurationUpdateIEs;
        S1ap_MMEDirectInformationTransferIEs_t s1ap_MMEDirectInformationTransferIEs;
        S1ap_MMEStatusTransferIEs_t s1ap_MMEStatusTransferIEs;
        S1ap_NASNonDeliveryIndication_IEs_t s1ap_NASNonDeliveryIndication_IEs;
        S1ap_OverloadStartIEs_t s1ap_OverloadStartIEs;
        S1ap_OverloadStopIEs_t s1ap_OverloadStopIEs;
        S1ap_PagingIEs_t s1ap_PagingIEs;
        S1ap_PathSwitchRequestAcknowledgeIEs_t s1ap_PathSwitchRequestAcknowledgeIEs;
        S1ap_PathSwitchRequestFailureIEs_t s1ap_PathSwitchRequestFailureIEs;
        S1ap_PathSwitchRequestIEs_t s1ap_PathSwitchRequestIEs;
        S1ap_ResetAcknowledgeIEs_t s1ap_ResetAcknowledgeIEs;
        S1ap_ResetIEs_t s1ap_ResetIEs;
        S1ap_S1SetupFailureIEs_t s1ap_S1SetupFailureIEs;
        S1ap_S1SetupRequestIEs_t s1ap_S1SetupRequestIEs;
        S1ap_S1SetupResponseIEs_t s1ap_S1SetupResponseIEs;
        S1ap_TraceFailureIndicationIEs_t s1ap_TraceFailureIndicationIEs;
        S1ap_TraceStartIEs_t s1ap_TraceStartIEs;
        S1ap_UECapabilityInfoIndicationIEs_t s1ap_UECapabilityInfoIndicationIEs;
        S1ap_UEContextModificationFailureIEs_t s1ap_UEContextModificationFailureIEs;
        S1ap_UEContextModificationRequestIEs_t s1ap_UEContextModificationRequestIEs;
        S1ap_UEContextModificationResponseIEs_t s1ap_UEContextModificationResponseIEs;
        S1ap_UEContextReleaseCommand_IEs_t s1ap_UEContextReleaseCommand_IEs;
        S1ap_UEContextReleaseComplete_IEs_t s1ap_UEContextReleaseComplete_IEs;
        S1ap_UEContextReleaseRequest_IEs_t s1ap_UEContextReleaseRequest_IEs;
        S1ap_UplinkNASTransport_IEs_t s1ap_UplinkNASTransport_IEs;
        S1ap_UplinkNonUEAssociatedLPPaTransport_IEs_t s1ap_UplinkNonUEAssociatedLPPaTransport_IEs;
        S1ap_UplinkS1cdma2000tunnelingIEs_t s1ap_UplinkS1cdma2000tunnelingIEs;
        S1ap_UplinkUEAssociatedLPPaTransport_IEs_t s1ap_UplinkUEAssociatedLPPaTransport_IEs;
        S1ap_WriteReplaceWarningRequestIEs_t s1ap_WriteReplaceWarningRequestIEs;
        S1ap_WriteReplaceWarningResponseIEs_t s1ap_WriteReplaceWarningResponseIEs;
    } msg;
} s1ap_message;

/** \brief Decode function for S1ap-DeactivateTraceIEs ies.
 * \param s1ap_DeactivateTraceIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_deactivatetraceies(
    S1ap_DeactivateTraceIEs_t *s1ap_DeactivateTraceIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-DeactivateTraceIEs ies.
 *  \param s1ap_DeactivateTrace Pointer to the ASN1 structure.
 *  \param s1ap_DeactivateTraceIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_deactivatetraceies(
    S1ap_DeactivateTrace_t *s1ap_DeactivateTrace,
    S1ap_DeactivateTraceIEs_t *s1ap_DeactivateTraceIEs);

/** \brief Decode function for S1ap-ENBDirectInformationTransferIEs ies.
 * \param s1ap_ENBDirectInformationTransferIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_enbdirectinformationtransferies(
    S1ap_ENBDirectInformationTransferIEs_t *s1ap_ENBDirectInformationTransferIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-ENBDirectInformationTransferIEs ies.
 *  \param s1ap_ENBDirectInformationTransfer Pointer to the ASN1 structure.
 *  \param s1ap_ENBDirectInformationTransferIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_enbdirectinformationtransferies(
    S1ap_ENBDirectInformationTransfer_t *s1ap_ENBDirectInformationTransfer,
    S1ap_ENBDirectInformationTransferIEs_t *s1ap_ENBDirectInformationTransferIEs);

/** \brief Decode function for S1ap-E-RABReleaseResponseIEs ies.
 * \param s1ap_E_RABReleaseResponseIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_e_rabreleaseresponseies(
    S1ap_E_RABReleaseResponseIEs_t *s1ap_E_RABReleaseResponseIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-E-RABReleaseResponseIEs ies.
 *  \param s1ap_E_RABReleaseResponse Pointer to the ASN1 structure.
 *  \param s1ap_E_RABReleaseResponseIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabreleaseresponseies(
    S1ap_E_RABReleaseResponse_t *s1ap_E_RABReleaseResponse,
    S1ap_E_RABReleaseResponseIEs_t *s1ap_E_RABReleaseResponseIEs);

/** \brief Decode function for S1ap-UplinkNonUEAssociatedLPPaTransport-IEs ies.
 * \param s1ap_UplinkNonUEAssociatedLPPaTransport_IEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_uplinknonueassociatedlppatransport_ies(
    S1ap_UplinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkNonUEAssociatedLPPaTransport_IEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-UplinkNonUEAssociatedLPPaTransport-IEs ies.
 *  \param s1ap_UplinkNonUEAssociatedLPPaTransport Pointer to the ASN1 structure.
 *  \param s1ap_UplinkNonUEAssociatedLPPaTransport_IEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_uplinknonueassociatedlppatransport_ies(
    S1ap_UplinkNonUEAssociatedLPPaTransport_t *s1ap_UplinkNonUEAssociatedLPPaTransport,
    S1ap_UplinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkNonUEAssociatedLPPaTransport_IEs);

/** \brief Decode function for S1ap-TraceStartIEs ies.
 * \param s1ap_TraceStartIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_tracestarties(
    S1ap_TraceStartIEs_t *s1ap_TraceStartIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-TraceStartIEs ies.
 *  \param s1ap_TraceStart Pointer to the ASN1 structure.
 *  \param s1ap_TraceStartIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_tracestarties(
    S1ap_TraceStart_t *s1ap_TraceStart,
    S1ap_TraceStartIEs_t *s1ap_TraceStartIEs);

/** \brief Decode function for S1ap-WriteReplaceWarningResponseIEs ies.
 * \param s1ap_WriteReplaceWarningResponseIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_writereplacewarningresponseies(
    S1ap_WriteReplaceWarningResponseIEs_t *s1ap_WriteReplaceWarningResponseIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-WriteReplaceWarningResponseIEs ies.
 *  \param s1ap_WriteReplaceWarningResponse Pointer to the ASN1 structure.
 *  \param s1ap_WriteReplaceWarningResponseIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_writereplacewarningresponseies(
    S1ap_WriteReplaceWarningResponse_t *s1ap_WriteReplaceWarningResponse,
    S1ap_WriteReplaceWarningResponseIEs_t *s1ap_WriteReplaceWarningResponseIEs);

/** \brief Decode function for S1ap-HandoverCancelIEs ies.
 * \param s1ap_HandoverCancelIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_handovercancelies(
    S1ap_HandoverCancelIEs_t *s1ap_HandoverCancelIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-HandoverCancelIEs ies.
 *  \param s1ap_HandoverCancel Pointer to the ASN1 structure.
 *  \param s1ap_HandoverCancelIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_handovercancelies(
    S1ap_HandoverCancel_t *s1ap_HandoverCancel,
    S1ap_HandoverCancelIEs_t *s1ap_HandoverCancelIEs);

/** \brief Decode function for S1ap-HandoverPreparationFailureIEs ies.
 * \param s1ap_HandoverPreparationFailureIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_handoverpreparationfailureies(
    S1ap_HandoverPreparationFailureIEs_t *s1ap_HandoverPreparationFailureIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-HandoverPreparationFailureIEs ies.
 *  \param s1ap_HandoverPreparationFailure Pointer to the ASN1 structure.
 *  \param s1ap_HandoverPreparationFailureIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_handoverpreparationfailureies(
    S1ap_HandoverPreparationFailure_t *s1ap_HandoverPreparationFailure,
    S1ap_HandoverPreparationFailureIEs_t *s1ap_HandoverPreparationFailureIEs);

/** \brief Decode function for S1ap-UEContextReleaseRequest-IEs ies.
 * \param s1ap_UEContextReleaseRequest_IEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_uecontextreleaserequest_ies(
    S1ap_UEContextReleaseRequest_IEs_t *s1ap_UEContextReleaseRequest_IEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-UEContextReleaseRequest-IEs ies.
 *  \param s1ap_UEContextReleaseRequest Pointer to the ASN1 structure.
 *  \param s1ap_UEContextReleaseRequest_IEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_uecontextreleaserequest_ies(
    S1ap_UEContextReleaseRequest_t *s1ap_UEContextReleaseRequest,
    S1ap_UEContextReleaseRequest_IEs_t *s1ap_UEContextReleaseRequest_IEs);

/** \brief Decode function for S1ap-InitialUEMessage-IEs ies.
 * \param s1ap_InitialUEMessage_IEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_initialuemessage_ies(
    S1ap_InitialUEMessage_IEs_t *s1ap_InitialUEMessage_IEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-InitialUEMessage-IEs ies.
 *  \param s1ap_InitialUEMessage Pointer to the ASN1 structure.
 *  \param s1ap_InitialUEMessage_IEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_initialuemessage_ies(
    S1ap_InitialUEMessage_t *s1ap_InitialUEMessage,
    S1ap_InitialUEMessage_IEs_t *s1ap_InitialUEMessage_IEs);

/** \brief Decode function for S1ap-KillRequestIEs ies.
 * \param s1ap_KillRequestIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_killrequesties(
    S1ap_KillRequestIEs_t *s1ap_KillRequestIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-KillRequestIEs ies.
 *  \param s1ap_KillRequest Pointer to the ASN1 structure.
 *  \param s1ap_KillRequestIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_killrequesties(
    S1ap_KillRequest_t *s1ap_KillRequest,
    S1ap_KillRequestIEs_t *s1ap_KillRequestIEs);

/** \brief Decode function for S1ap-WriteReplaceWarningRequestIEs ies.
 * \param s1ap_WriteReplaceWarningRequestIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_writereplacewarningrequesties(
    S1ap_WriteReplaceWarningRequestIEs_t *s1ap_WriteReplaceWarningRequestIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-WriteReplaceWarningRequestIEs ies.
 *  \param s1ap_WriteReplaceWarningRequest Pointer to the ASN1 structure.
 *  \param s1ap_WriteReplaceWarningRequestIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_writereplacewarningrequesties(
    S1ap_WriteReplaceWarningRequest_t *s1ap_WriteReplaceWarningRequest,
    S1ap_WriteReplaceWarningRequestIEs_t *s1ap_WriteReplaceWarningRequestIEs);

/** \brief Decode function for S1ap-UECapabilityInfoIndicationIEs ies.
 * \param s1ap_UECapabilityInfoIndicationIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_uecapabilityinfoindicationies(
    S1ap_UECapabilityInfoIndicationIEs_t *s1ap_UECapabilityInfoIndicationIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-UECapabilityInfoIndicationIEs ies.
 *  \param s1ap_UECapabilityInfoIndication Pointer to the ASN1 structure.
 *  \param s1ap_UECapabilityInfoIndicationIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_uecapabilityinfoindicationies(
    S1ap_UECapabilityInfoIndication_t *s1ap_UECapabilityInfoIndication,
    S1ap_UECapabilityInfoIndicationIEs_t *s1ap_UECapabilityInfoIndicationIEs);

/** \brief Decode function for S1ap-LocationReportingFailureIndicationIEs ies.
 * \param s1ap_LocationReportingFailureIndicationIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_locationreportingfailureindicationies(
    S1ap_LocationReportingFailureIndicationIEs_t *s1ap_LocationReportingFailureIndicationIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-LocationReportingFailureIndicationIEs ies.
 *  \param s1ap_LocationReportingFailureIndication Pointer to the ASN1 structure.
 *  \param s1ap_LocationReportingFailureIndicationIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_locationreportingfailureindicationies(
    S1ap_LocationReportingFailureIndication_t *s1ap_LocationReportingFailureIndication,
    S1ap_LocationReportingFailureIndicationIEs_t *s1ap_LocationReportingFailureIndicationIEs);

/** \brief Decode function for S1ap-InitialContextSetupRequestIEs ies.
 * \param s1ap_InitialContextSetupRequestIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_initialcontextsetuprequesties(
    S1ap_InitialContextSetupRequestIEs_t *s1ap_InitialContextSetupRequestIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-InitialContextSetupRequestIEs ies.
 *  \param s1ap_InitialContextSetupRequest Pointer to the ASN1 structure.
 *  \param s1ap_InitialContextSetupRequestIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_initialcontextsetuprequesties(
    S1ap_InitialContextSetupRequest_t *s1ap_InitialContextSetupRequest,
    S1ap_InitialContextSetupRequestIEs_t *s1ap_InitialContextSetupRequestIEs);

/** \brief Decode function for S1ap-LocationReportIEs ies.
 * \param s1ap_LocationReportIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_locationreporties(
    S1ap_LocationReportIEs_t *s1ap_LocationReportIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-LocationReportIEs ies.
 *  \param s1ap_LocationReport Pointer to the ASN1 structure.
 *  \param s1ap_LocationReportIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_locationreporties(
    S1ap_LocationReport_t *s1ap_LocationReport,
    S1ap_LocationReportIEs_t *s1ap_LocationReportIEs);

/** \brief Decode function for S1ap-UplinkNASTransport-IEs ies.
 * \param s1ap_UplinkNASTransport_IEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_uplinknastransport_ies(
    S1ap_UplinkNASTransport_IEs_t *s1ap_UplinkNASTransport_IEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-UplinkNASTransport-IEs ies.
 *  \param s1ap_UplinkNASTransport Pointer to the ASN1 structure.
 *  \param s1ap_UplinkNASTransport_IEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_uplinknastransport_ies(
    S1ap_UplinkNASTransport_t *s1ap_UplinkNASTransport,
    S1ap_UplinkNASTransport_IEs_t *s1ap_UplinkNASTransport_IEs);

/** \brief Decode function for S1ap-UEContextModificationResponseIEs ies.
 * \param s1ap_UEContextModificationResponseIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_uecontextmodificationresponseies(
    S1ap_UEContextModificationResponseIEs_t *s1ap_UEContextModificationResponseIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-UEContextModificationResponseIEs ies.
 *  \param s1ap_UEContextModificationResponse Pointer to the ASN1 structure.
 *  \param s1ap_UEContextModificationResponseIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_uecontextmodificationresponseies(
    S1ap_UEContextModificationResponse_t *s1ap_UEContextModificationResponse,
    S1ap_UEContextModificationResponseIEs_t *s1ap_UEContextModificationResponseIEs);

/** \brief Decode function for S1ap-UEContextModificationRequestIEs ies.
 * \param s1ap_UEContextModificationRequestIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_uecontextmodificationrequesties(
    S1ap_UEContextModificationRequestIEs_t *s1ap_UEContextModificationRequestIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-UEContextModificationRequestIEs ies.
 *  \param s1ap_UEContextModificationRequest Pointer to the ASN1 structure.
 *  \param s1ap_UEContextModificationRequestIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_uecontextmodificationrequesties(
    S1ap_UEContextModificationRequest_t *s1ap_UEContextModificationRequest,
    S1ap_UEContextModificationRequestIEs_t *s1ap_UEContextModificationRequestIEs);

/** \brief Decode function for S1ap-LocationReportingControlIEs ies.
 * \param s1ap_LocationReportingControlIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_locationreportingcontrolies(
    S1ap_LocationReportingControlIEs_t *s1ap_LocationReportingControlIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-LocationReportingControlIEs ies.
 *  \param s1ap_LocationReportingControl Pointer to the ASN1 structure.
 *  \param s1ap_LocationReportingControlIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_locationreportingcontrolies(
    S1ap_LocationReportingControl_t *s1ap_LocationReportingControl,
    S1ap_LocationReportingControlIEs_t *s1ap_LocationReportingControlIEs);

/** \brief Decode function for S1ap-UplinkS1cdma2000tunnelingIEs ies.
 * \param s1ap_UplinkS1cdma2000tunnelingIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_uplinks1cdma2000tunnelingies(
    S1ap_UplinkS1cdma2000tunnelingIEs_t *s1ap_UplinkS1cdma2000tunnelingIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-UplinkS1cdma2000tunnelingIEs ies.
 *  \param s1ap_UplinkS1cdma2000tunneling Pointer to the ASN1 structure.
 *  \param s1ap_UplinkS1cdma2000tunnelingIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_uplinks1cdma2000tunnelingies(
    S1ap_UplinkS1cdma2000tunneling_t *s1ap_UplinkS1cdma2000tunneling,
    S1ap_UplinkS1cdma2000tunnelingIEs_t *s1ap_UplinkS1cdma2000tunnelingIEs);

/** \brief Decode function for S1ap-HandoverRequestIEs ies.
 * \param s1ap_HandoverRequestIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_handoverrequesties(
    S1ap_HandoverRequestIEs_t *s1ap_HandoverRequestIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-HandoverRequestIEs ies.
 *  \param s1ap_HandoverRequest Pointer to the ASN1 structure.
 *  \param s1ap_HandoverRequestIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_handoverrequesties(
    S1ap_HandoverRequest_t *s1ap_HandoverRequest,
    S1ap_HandoverRequestIEs_t *s1ap_HandoverRequestIEs);

/** \brief Decode function for S1ap-HandoverRequiredIEs ies.
 * \param s1ap_HandoverRequiredIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_handoverrequiredies(
    S1ap_HandoverRequiredIEs_t *s1ap_HandoverRequiredIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-HandoverRequiredIEs ies.
 *  \param s1ap_HandoverRequired Pointer to the ASN1 structure.
 *  \param s1ap_HandoverRequiredIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_handoverrequiredies(
    S1ap_HandoverRequired_t *s1ap_HandoverRequired,
    S1ap_HandoverRequiredIEs_t *s1ap_HandoverRequiredIEs);

/** \brief Decode function for S1ap-MMEConfigurationUpdateAcknowledgeIEs ies.
 * \param s1ap_MMEConfigurationUpdateAcknowledgeIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_mmeconfigurationupdateacknowledgeies(
    S1ap_MMEConfigurationUpdateAcknowledgeIEs_t *s1ap_MMEConfigurationUpdateAcknowledgeIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-MMEConfigurationUpdateAcknowledgeIEs ies.
 *  \param s1ap_MMEConfigurationUpdateAcknowledge Pointer to the ASN1 structure.
 *  \param s1ap_MMEConfigurationUpdateAcknowledgeIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_mmeconfigurationupdateacknowledgeies(
    S1ap_MMEConfigurationUpdateAcknowledge_t *s1ap_MMEConfigurationUpdateAcknowledge,
    S1ap_MMEConfigurationUpdateAcknowledgeIEs_t *s1ap_MMEConfigurationUpdateAcknowledgeIEs);

/** \brief Decode function for S1ap-KillResponseIEs ies.
 * \param s1ap_KillResponseIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_killresponseies(
    S1ap_KillResponseIEs_t *s1ap_KillResponseIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-KillResponseIEs ies.
 *  \param s1ap_KillResponse Pointer to the ASN1 structure.
 *  \param s1ap_KillResponseIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_killresponseies(
    S1ap_KillResponse_t *s1ap_KillResponse,
    S1ap_KillResponseIEs_t *s1ap_KillResponseIEs);

/** \brief Decode function for S1ap-MMEStatusTransferIEs ies.
 * \param s1ap_MMEStatusTransferIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_mmestatustransferies(
    S1ap_MMEStatusTransferIEs_t *s1ap_MMEStatusTransferIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-MMEStatusTransferIEs ies.
 *  \param s1ap_MMEStatusTransfer Pointer to the ASN1 structure.
 *  \param s1ap_MMEStatusTransferIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_mmestatustransferies(
    S1ap_MMEStatusTransfer_t *s1ap_MMEStatusTransfer,
    S1ap_MMEStatusTransferIEs_t *s1ap_MMEStatusTransferIEs);

/** \brief Decode function for S1ap-MMEConfigurationUpdateIEs ies.
 * \param s1ap_MMEConfigurationUpdateIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_mmeconfigurationupdateies(
    S1ap_MMEConfigurationUpdateIEs_t *s1ap_MMEConfigurationUpdateIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-MMEConfigurationUpdateIEs ies.
 *  \param s1ap_MMEConfigurationUpdate Pointer to the ASN1 structure.
 *  \param s1ap_MMEConfigurationUpdateIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_mmeconfigurationupdateies(
    S1ap_MMEConfigurationUpdate_t *s1ap_MMEConfigurationUpdate,
    S1ap_MMEConfigurationUpdateIEs_t *s1ap_MMEConfigurationUpdateIEs);

/** \brief Decode function for S1ap-E-RABModifyRequestIEs ies.
 * \param s1ap_E_RABModifyRequestIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_e_rabmodifyrequesties(
    S1ap_E_RABModifyRequestIEs_t *s1ap_E_RABModifyRequestIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-E-RABModifyRequestIEs ies.
 *  \param s1ap_E_RABModifyRequest Pointer to the ASN1 structure.
 *  \param s1ap_E_RABModifyRequestIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabmodifyrequesties(
    S1ap_E_RABModifyRequest_t *s1ap_E_RABModifyRequest,
    S1ap_E_RABModifyRequestIEs_t *s1ap_E_RABModifyRequestIEs);

/** \brief Decode function for S1ap-ENBConfigurationUpdateFailureIEs ies.
 * \param s1ap_ENBConfigurationUpdateFailureIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_enbconfigurationupdatefailureies(
    S1ap_ENBConfigurationUpdateFailureIEs_t *s1ap_ENBConfigurationUpdateFailureIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-ENBConfigurationUpdateFailureIEs ies.
 *  \param s1ap_ENBConfigurationUpdateFailure Pointer to the ASN1 structure.
 *  \param s1ap_ENBConfigurationUpdateFailureIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_enbconfigurationupdatefailureies(
    S1ap_ENBConfigurationUpdateFailure_t *s1ap_ENBConfigurationUpdateFailure,
    S1ap_ENBConfigurationUpdateFailureIEs_t *s1ap_ENBConfigurationUpdateFailureIEs);

/** \brief Decode function for S1ap-ResetIEs ies.
 * \param s1ap_ResetIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_reseties(
    S1ap_ResetIEs_t *s1ap_ResetIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-ResetIEs ies.
 *  \param s1ap_Reset Pointer to the ASN1 structure.
 *  \param s1ap_ResetIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_reseties(
    S1ap_Reset_t *s1ap_Reset,
    S1ap_ResetIEs_t *s1ap_ResetIEs);

/** \brief Decode function for S1ap-S1SetupResponseIEs ies.
 * \param s1ap_S1SetupResponseIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_s1setupresponseies(
    S1ap_S1SetupResponseIEs_t *s1ap_S1SetupResponseIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-S1SetupResponseIEs ies.
 *  \param s1ap_S1SetupResponse Pointer to the ASN1 structure.
 *  \param s1ap_S1SetupResponseIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_s1setupresponseies(
    S1ap_S1SetupResponse_t *s1ap_S1SetupResponse,
    S1ap_S1SetupResponseIEs_t *s1ap_S1SetupResponseIEs);

/** \brief Decode function for S1ap-PagingIEs ies.
 * \param s1ap_PagingIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_pagingies(
    S1ap_PagingIEs_t *s1ap_PagingIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-PagingIEs ies.
 *  \param s1ap_Paging Pointer to the ASN1 structure.
 *  \param s1ap_PagingIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_pagingies(
    S1ap_Paging_t *s1ap_Paging,
    S1ap_PagingIEs_t *s1ap_PagingIEs);

/** \brief Decode function for S1ap-ENBConfigurationUpdateAcknowledgeIEs ies.
 * \param s1ap_ENBConfigurationUpdateAcknowledgeIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_enbconfigurationupdateacknowledgeies(
    S1ap_ENBConfigurationUpdateAcknowledgeIEs_t *s1ap_ENBConfigurationUpdateAcknowledgeIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-ENBConfigurationUpdateAcknowledgeIEs ies.
 *  \param s1ap_ENBConfigurationUpdateAcknowledge Pointer to the ASN1 structure.
 *  \param s1ap_ENBConfigurationUpdateAcknowledgeIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_enbconfigurationupdateacknowledgeies(
    S1ap_ENBConfigurationUpdateAcknowledge_t *s1ap_ENBConfigurationUpdateAcknowledge,
    S1ap_ENBConfigurationUpdateAcknowledgeIEs_t *s1ap_ENBConfigurationUpdateAcknowledgeIEs);

/** \brief Decode function for S1ap-E-RABReleaseCommandIEs ies.
 * \param s1ap_E_RABReleaseCommandIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_e_rabreleasecommandies(
    S1ap_E_RABReleaseCommandIEs_t *s1ap_E_RABReleaseCommandIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-E-RABReleaseCommandIEs ies.
 *  \param s1ap_E_RABReleaseCommand Pointer to the ASN1 structure.
 *  \param s1ap_E_RABReleaseCommandIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabreleasecommandies(
    S1ap_E_RABReleaseCommand_t *s1ap_E_RABReleaseCommand,
    S1ap_E_RABReleaseCommandIEs_t *s1ap_E_RABReleaseCommandIEs);

/** \brief Decode function for S1ap-PathSwitchRequestFailureIEs ies.
 * \param s1ap_PathSwitchRequestFailureIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_pathswitchrequestfailureies(
    S1ap_PathSwitchRequestFailureIEs_t *s1ap_PathSwitchRequestFailureIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-PathSwitchRequestFailureIEs ies.
 *  \param s1ap_PathSwitchRequestFailure Pointer to the ASN1 structure.
 *  \param s1ap_PathSwitchRequestFailureIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_pathswitchrequestfailureies(
    S1ap_PathSwitchRequestFailure_t *s1ap_PathSwitchRequestFailure,
    S1ap_PathSwitchRequestFailureIEs_t *s1ap_PathSwitchRequestFailureIEs);

/** \brief Decode function for S1ap-E-RABModifyResponseIEs ies.
 * \param s1ap_E_RABModifyResponseIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_e_rabmodifyresponseies(
    S1ap_E_RABModifyResponseIEs_t *s1ap_E_RABModifyResponseIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-E-RABModifyResponseIEs ies.
 *  \param s1ap_E_RABModifyResponse Pointer to the ASN1 structure.
 *  \param s1ap_E_RABModifyResponseIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabmodifyresponseies(
    S1ap_E_RABModifyResponse_t *s1ap_E_RABModifyResponse,
    S1ap_E_RABModifyResponseIEs_t *s1ap_E_RABModifyResponseIEs);

/** \brief Decode function for S1ap-HandoverNotifyIEs ies.
 * \param s1ap_HandoverNotifyIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_handovernotifyies(
    S1ap_HandoverNotifyIEs_t *s1ap_HandoverNotifyIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-HandoverNotifyIEs ies.
 *  \param s1ap_HandoverNotify Pointer to the ASN1 structure.
 *  \param s1ap_HandoverNotifyIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_handovernotifyies(
    S1ap_HandoverNotify_t *s1ap_HandoverNotify,
    S1ap_HandoverNotifyIEs_t *s1ap_HandoverNotifyIEs);

/** \brief Decode function for S1ap-HandoverCancelAcknowledgeIEs ies.
 * \param s1ap_HandoverCancelAcknowledgeIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_handovercancelacknowledgeies(
    S1ap_HandoverCancelAcknowledgeIEs_t *s1ap_HandoverCancelAcknowledgeIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-HandoverCancelAcknowledgeIEs ies.
 *  \param s1ap_HandoverCancelAcknowledge Pointer to the ASN1 structure.
 *  \param s1ap_HandoverCancelAcknowledgeIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_handovercancelacknowledgeies(
    S1ap_HandoverCancelAcknowledge_t *s1ap_HandoverCancelAcknowledge,
    S1ap_HandoverCancelAcknowledgeIEs_t *s1ap_HandoverCancelAcknowledgeIEs);

/** \brief Decode function for S1ap-InitialContextSetupFailureIEs ies.
 * \param s1ap_InitialContextSetupFailureIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_initialcontextsetupfailureies(
    S1ap_InitialContextSetupFailureIEs_t *s1ap_InitialContextSetupFailureIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-InitialContextSetupFailureIEs ies.
 *  \param s1ap_InitialContextSetupFailure Pointer to the ASN1 structure.
 *  \param s1ap_InitialContextSetupFailureIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_initialcontextsetupfailureies(
    S1ap_InitialContextSetupFailure_t *s1ap_InitialContextSetupFailure,
    S1ap_InitialContextSetupFailureIEs_t *s1ap_InitialContextSetupFailureIEs);

/** \brief Decode function for S1ap-HandoverCommandIEs ies.
 * \param s1ap_HandoverCommandIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_handovercommandies(
    S1ap_HandoverCommandIEs_t *s1ap_HandoverCommandIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-HandoverCommandIEs ies.
 *  \param s1ap_HandoverCommand Pointer to the ASN1 structure.
 *  \param s1ap_HandoverCommandIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_handovercommandies(
    S1ap_HandoverCommand_t *s1ap_HandoverCommand,
    S1ap_HandoverCommandIEs_t *s1ap_HandoverCommandIEs);

/** \brief Decode function for S1ap-MMEDirectInformationTransferIEs ies.
 * \param s1ap_MMEDirectInformationTransferIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_mmedirectinformationtransferies(
    S1ap_MMEDirectInformationTransferIEs_t *s1ap_MMEDirectInformationTransferIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-MMEDirectInformationTransferIEs ies.
 *  \param s1ap_MMEDirectInformationTransfer Pointer to the ASN1 structure.
 *  \param s1ap_MMEDirectInformationTransferIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_mmedirectinformationtransferies(
    S1ap_MMEDirectInformationTransfer_t *s1ap_MMEDirectInformationTransfer,
    S1ap_MMEDirectInformationTransferIEs_t *s1ap_MMEDirectInformationTransferIEs);

/** \brief Decode function for S1ap-ErrorIndicationIEs ies.
 * \param s1ap_ErrorIndicationIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_errorindicationies(
    S1ap_ErrorIndicationIEs_t *s1ap_ErrorIndicationIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-ErrorIndicationIEs ies.
 *  \param s1ap_ErrorIndication Pointer to the ASN1 structure.
 *  \param s1ap_ErrorIndicationIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_errorindicationies(
    S1ap_ErrorIndication_t *s1ap_ErrorIndication,
    S1ap_ErrorIndicationIEs_t *s1ap_ErrorIndicationIEs);

/** \brief Decode function for S1ap-OverloadStartIEs ies.
 * \param s1ap_OverloadStartIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_overloadstarties(
    S1ap_OverloadStartIEs_t *s1ap_OverloadStartIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-OverloadStartIEs ies.
 *  \param s1ap_OverloadStart Pointer to the ASN1 structure.
 *  \param s1ap_OverloadStartIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_overloadstarties(
    S1ap_OverloadStart_t *s1ap_OverloadStart,
    S1ap_OverloadStartIEs_t *s1ap_OverloadStartIEs);

/** \brief Decode function for S1ap-DownlinkNonUEAssociatedLPPaTransport-IEs ies.
 * \param s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_downlinknonueassociatedlppatransport_ies(
    S1ap_DownlinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-DownlinkNonUEAssociatedLPPaTransport-IEs ies.
 *  \param s1ap_DownlinkNonUEAssociatedLPPaTransport Pointer to the ASN1 structure.
 *  \param s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_downlinknonueassociatedlppatransport_ies(
    S1ap_DownlinkNonUEAssociatedLPPaTransport_t *s1ap_DownlinkNonUEAssociatedLPPaTransport,
    S1ap_DownlinkNonUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkNonUEAssociatedLPPaTransport_IEs);

/** \brief Decode function for S1ap-HandoverRequestAcknowledgeIEs ies.
 * \param s1ap_HandoverRequestAcknowledgeIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_handoverrequestacknowledgeies(
    S1ap_HandoverRequestAcknowledgeIEs_t *s1ap_HandoverRequestAcknowledgeIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-HandoverRequestAcknowledgeIEs ies.
 *  \param s1ap_HandoverRequestAcknowledge Pointer to the ASN1 structure.
 *  \param s1ap_HandoverRequestAcknowledgeIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_handoverrequestacknowledgeies(
    S1ap_HandoverRequestAcknowledge_t *s1ap_HandoverRequestAcknowledge,
    S1ap_HandoverRequestAcknowledgeIEs_t *s1ap_HandoverRequestAcknowledgeIEs);

/** \brief Decode function for S1ap-DownlinkUEAssociatedLPPaTransport-IEs ies.
 * \param s1ap_DownlinkUEAssociatedLPPaTransport_IEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_downlinkueassociatedlppatransport_ies(
    S1ap_DownlinkUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkUEAssociatedLPPaTransport_IEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-DownlinkUEAssociatedLPPaTransport-IEs ies.
 *  \param s1ap_DownlinkUEAssociatedLPPaTransport Pointer to the ASN1 structure.
 *  \param s1ap_DownlinkUEAssociatedLPPaTransport_IEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_downlinkueassociatedlppatransport_ies(
    S1ap_DownlinkUEAssociatedLPPaTransport_t *s1ap_DownlinkUEAssociatedLPPaTransport,
    S1ap_DownlinkUEAssociatedLPPaTransport_IEs_t *s1ap_DownlinkUEAssociatedLPPaTransport_IEs);

/** \brief Decode function for S1ap-UEContextModificationFailureIEs ies.
 * \param s1ap_UEContextModificationFailureIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_uecontextmodificationfailureies(
    S1ap_UEContextModificationFailureIEs_t *s1ap_UEContextModificationFailureIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-UEContextModificationFailureIEs ies.
 *  \param s1ap_UEContextModificationFailure Pointer to the ASN1 structure.
 *  \param s1ap_UEContextModificationFailureIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_uecontextmodificationfailureies(
    S1ap_UEContextModificationFailure_t *s1ap_UEContextModificationFailure,
    S1ap_UEContextModificationFailureIEs_t *s1ap_UEContextModificationFailureIEs);

/** \brief Decode function for S1ap-MMEConfigurationTransferIEs ies.
 * \param s1ap_MMEConfigurationTransferIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_mmeconfigurationtransferies(
    S1ap_MMEConfigurationTransferIEs_t *s1ap_MMEConfigurationTransferIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-MMEConfigurationTransferIEs ies.
 *  \param s1ap_MMEConfigurationTransfer Pointer to the ASN1 structure.
 *  \param s1ap_MMEConfigurationTransferIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_mmeconfigurationtransferies(
    S1ap_MMEConfigurationTransfer_t *s1ap_MMEConfigurationTransfer,
    S1ap_MMEConfigurationTransferIEs_t *s1ap_MMEConfigurationTransferIEs);

/** \brief Decode function for S1ap-PathSwitchRequestAcknowledgeIEs ies.
 * \param s1ap_PathSwitchRequestAcknowledgeIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_pathswitchrequestacknowledgeies(
    S1ap_PathSwitchRequestAcknowledgeIEs_t *s1ap_PathSwitchRequestAcknowledgeIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-PathSwitchRequestAcknowledgeIEs ies.
 *  \param s1ap_PathSwitchRequestAcknowledge Pointer to the ASN1 structure.
 *  \param s1ap_PathSwitchRequestAcknowledgeIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_pathswitchrequestacknowledgeies(
    S1ap_PathSwitchRequestAcknowledge_t *s1ap_PathSwitchRequestAcknowledge,
    S1ap_PathSwitchRequestAcknowledgeIEs_t *s1ap_PathSwitchRequestAcknowledgeIEs);

/** \brief Decode function for S1ap-S1SetupFailureIEs ies.
 * \param s1ap_S1SetupFailureIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_s1setupfailureies(
    S1ap_S1SetupFailureIEs_t *s1ap_S1SetupFailureIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-S1SetupFailureIEs ies.
 *  \param s1ap_S1SetupFailure Pointer to the ASN1 structure.
 *  \param s1ap_S1SetupFailureIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_s1setupfailureies(
    S1ap_S1SetupFailure_t *s1ap_S1SetupFailure,
    S1ap_S1SetupFailureIEs_t *s1ap_S1SetupFailureIEs);

/** \brief Decode function for S1ap-MMEConfigurationUpdateFailureIEs ies.
 * \param s1ap_MMEConfigurationUpdateFailureIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_mmeconfigurationupdatefailureies(
    S1ap_MMEConfigurationUpdateFailureIEs_t *s1ap_MMEConfigurationUpdateFailureIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-MMEConfigurationUpdateFailureIEs ies.
 *  \param s1ap_MMEConfigurationUpdateFailure Pointer to the ASN1 structure.
 *  \param s1ap_MMEConfigurationUpdateFailureIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_mmeconfigurationupdatefailureies(
    S1ap_MMEConfigurationUpdateFailure_t *s1ap_MMEConfigurationUpdateFailure,
    S1ap_MMEConfigurationUpdateFailureIEs_t *s1ap_MMEConfigurationUpdateFailureIEs);

/** \brief Decode function for S1ap-InitialContextSetupResponseIEs ies.
 * \param s1ap_InitialContextSetupResponseIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_initialcontextsetupresponseies(
    S1ap_InitialContextSetupResponseIEs_t *s1ap_InitialContextSetupResponseIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-InitialContextSetupResponseIEs ies.
 *  \param s1ap_InitialContextSetupResponse Pointer to the ASN1 structure.
 *  \param s1ap_InitialContextSetupResponseIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_initialcontextsetupresponseies(
    S1ap_InitialContextSetupResponse_t *s1ap_InitialContextSetupResponse,
    S1ap_InitialContextSetupResponseIEs_t *s1ap_InitialContextSetupResponseIEs);

/** \brief Decode function for S1ap-DownlinkS1cdma2000tunnelingIEs ies.
 * \param s1ap_DownlinkS1cdma2000tunnelingIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_downlinks1cdma2000tunnelingies(
    S1ap_DownlinkS1cdma2000tunnelingIEs_t *s1ap_DownlinkS1cdma2000tunnelingIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-DownlinkS1cdma2000tunnelingIEs ies.
 *  \param s1ap_DownlinkS1cdma2000tunneling Pointer to the ASN1 structure.
 *  \param s1ap_DownlinkS1cdma2000tunnelingIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_downlinks1cdma2000tunnelingies(
    S1ap_DownlinkS1cdma2000tunneling_t *s1ap_DownlinkS1cdma2000tunneling,
    S1ap_DownlinkS1cdma2000tunnelingIEs_t *s1ap_DownlinkS1cdma2000tunnelingIEs);

/** \brief Decode function for S1ap-OverloadStopIEs ies.
 * \param s1ap_OverloadStopIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_overloadstopies(
    S1ap_OverloadStopIEs_t *s1ap_OverloadStopIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-OverloadStopIEs ies.
 *  \param s1ap_OverloadStop Pointer to the ASN1 structure.
 *  \param s1ap_OverloadStopIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_overloadstopies(
    S1ap_OverloadStop_t *s1ap_OverloadStop,
    S1ap_OverloadStopIEs_t *s1ap_OverloadStopIEs);

/** \brief Decode function for S1ap-ENBConfigurationUpdateIEs ies.
 * \param s1ap_ENBConfigurationUpdateIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_enbconfigurationupdateies(
    S1ap_ENBConfigurationUpdateIEs_t *s1ap_ENBConfigurationUpdateIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-ENBConfigurationUpdateIEs ies.
 *  \param s1ap_ENBConfigurationUpdate Pointer to the ASN1 structure.
 *  \param s1ap_ENBConfigurationUpdateIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_enbconfigurationupdateies(
    S1ap_ENBConfigurationUpdate_t *s1ap_ENBConfigurationUpdate,
    S1ap_ENBConfigurationUpdateIEs_t *s1ap_ENBConfigurationUpdateIEs);

/** \brief Decode function for S1ap-TraceFailureIndicationIEs ies.
 * \param s1ap_TraceFailureIndicationIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_tracefailureindicationies(
    S1ap_TraceFailureIndicationIEs_t *s1ap_TraceFailureIndicationIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-TraceFailureIndicationIEs ies.
 *  \param s1ap_TraceFailureIndication Pointer to the ASN1 structure.
 *  \param s1ap_TraceFailureIndicationIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_tracefailureindicationies(
    S1ap_TraceFailureIndication_t *s1ap_TraceFailureIndication,
    S1ap_TraceFailureIndicationIEs_t *s1ap_TraceFailureIndicationIEs);

/** \brief Decode function for S1ap-ENBConfigurationTransferIEs ies.
 * \param s1ap_ENBConfigurationTransferIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_enbconfigurationtransferies(
    S1ap_ENBConfigurationTransferIEs_t *s1ap_ENBConfigurationTransferIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-ENBConfigurationTransferIEs ies.
 *  \param s1ap_ENBConfigurationTransfer Pointer to the ASN1 structure.
 *  \param s1ap_ENBConfigurationTransferIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_enbconfigurationtransferies(
    S1ap_ENBConfigurationTransfer_t *s1ap_ENBConfigurationTransfer,
    S1ap_ENBConfigurationTransferIEs_t *s1ap_ENBConfigurationTransferIEs);

/** \brief Decode function for S1ap-HandoverFailureIEs ies.
 * \param s1ap_HandoverFailureIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_handoverfailureies(
    S1ap_HandoverFailureIEs_t *s1ap_HandoverFailureIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-HandoverFailureIEs ies.
 *  \param s1ap_HandoverFailure Pointer to the ASN1 structure.
 *  \param s1ap_HandoverFailureIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_handoverfailureies(
    S1ap_HandoverFailure_t *s1ap_HandoverFailure,
    S1ap_HandoverFailureIEs_t *s1ap_HandoverFailureIEs);

/** \brief Decode function for S1ap-E-RABSetupResponseIEs ies.
 * \param s1ap_E_RABSetupResponseIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_e_rabsetupresponseies(
    S1ap_E_RABSetupResponseIEs_t *s1ap_E_RABSetupResponseIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-E-RABSetupResponseIEs ies.
 *  \param s1ap_E_RABSetupResponse Pointer to the ASN1 structure.
 *  \param s1ap_E_RABSetupResponseIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabsetupresponseies(
    S1ap_E_RABSetupResponse_t *s1ap_E_RABSetupResponse,
    S1ap_E_RABSetupResponseIEs_t *s1ap_E_RABSetupResponseIEs);

/** \brief Decode function for S1ap-UEContextReleaseCommand-IEs ies.
 * \param s1ap_UEContextReleaseCommand_IEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_uecontextreleasecommand_ies(
    S1ap_UEContextReleaseCommand_IEs_t *s1ap_UEContextReleaseCommand_IEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-UEContextReleaseCommand-IEs ies.
 *  \param s1ap_UEContextReleaseCommand Pointer to the ASN1 structure.
 *  \param s1ap_UEContextReleaseCommand_IEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_uecontextreleasecommand_ies(
    S1ap_UEContextReleaseCommand_t *s1ap_UEContextReleaseCommand,
    S1ap_UEContextReleaseCommand_IEs_t *s1ap_UEContextReleaseCommand_IEs);

/** \brief Decode function for S1ap-PathSwitchRequestIEs ies.
 * \param s1ap_PathSwitchRequestIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_pathswitchrequesties(
    S1ap_PathSwitchRequestIEs_t *s1ap_PathSwitchRequestIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-PathSwitchRequestIEs ies.
 *  \param s1ap_PathSwitchRequest Pointer to the ASN1 structure.
 *  \param s1ap_PathSwitchRequestIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_pathswitchrequesties(
    S1ap_PathSwitchRequest_t *s1ap_PathSwitchRequest,
    S1ap_PathSwitchRequestIEs_t *s1ap_PathSwitchRequestIEs);

/** \brief Decode function for S1ap-DownlinkNASTransport-IEs ies.
 * \param s1ap_DownlinkNASTransport_IEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_downlinknastransport_ies(
    S1ap_DownlinkNASTransport_IEs_t *s1ap_DownlinkNASTransport_IEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-DownlinkNASTransport-IEs ies.
 *  \param s1ap_DownlinkNASTransport Pointer to the ASN1 structure.
 *  \param s1ap_DownlinkNASTransport_IEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_downlinknastransport_ies(
    S1ap_DownlinkNASTransport_t *s1ap_DownlinkNASTransport,
    S1ap_DownlinkNASTransport_IEs_t *s1ap_DownlinkNASTransport_IEs);

/** \brief Decode function for S1ap-E-RABReleaseIndicationIEs ies.
 * \param s1ap_E_RABReleaseIndicationIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_e_rabreleaseindicationies(
    S1ap_E_RABReleaseIndicationIEs_t *s1ap_E_RABReleaseIndicationIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-E-RABReleaseIndicationIEs ies.
 *  \param s1ap_E_RABReleaseIndication Pointer to the ASN1 structure.
 *  \param s1ap_E_RABReleaseIndicationIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabreleaseindicationies(
    S1ap_E_RABReleaseIndication_t *s1ap_E_RABReleaseIndication,
    S1ap_E_RABReleaseIndicationIEs_t *s1ap_E_RABReleaseIndicationIEs);

/** \brief Decode function for S1ap-S1SetupRequestIEs ies.
 * \param s1ap_S1SetupRequestIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_s1setuprequesties(
    S1ap_S1SetupRequestIEs_t *s1ap_S1SetupRequestIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-S1SetupRequestIEs ies.
 *  \param s1ap_S1SetupRequest Pointer to the ASN1 structure.
 *  \param s1ap_S1SetupRequestIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_s1setuprequesties(
    S1ap_S1SetupRequest_t *s1ap_S1SetupRequest,
    S1ap_S1SetupRequestIEs_t *s1ap_S1SetupRequestIEs);

/** \brief Decode function for S1ap-NASNonDeliveryIndication-IEs ies.
 * \param s1ap_NASNonDeliveryIndication_IEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_nasnondeliveryindication_ies(
    S1ap_NASNonDeliveryIndication_IEs_t *s1ap_NASNonDeliveryIndication_IEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-NASNonDeliveryIndication-IEs ies.
 *  \param s1ap_NASNonDeliveryIndication Pointer to the ASN1 structure.
 *  \param s1ap_NASNonDeliveryIndication_IEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_nasnondeliveryindication_ies(
    S1ap_NASNonDeliveryIndication_t *s1ap_NASNonDeliveryIndication,
    S1ap_NASNonDeliveryIndication_IEs_t *s1ap_NASNonDeliveryIndication_IEs);

/** \brief Decode function for S1ap-CellTrafficTraceIEs ies.
 * \param s1ap_CellTrafficTraceIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_celltraffictraceies(
    S1ap_CellTrafficTraceIEs_t *s1ap_CellTrafficTraceIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-CellTrafficTraceIEs ies.
 *  \param s1ap_CellTrafficTrace Pointer to the ASN1 structure.
 *  \param s1ap_CellTrafficTraceIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_celltraffictraceies(
    S1ap_CellTrafficTrace_t *s1ap_CellTrafficTrace,
    S1ap_CellTrafficTraceIEs_t *s1ap_CellTrafficTraceIEs);

/** \brief Decode function for S1ap-E-RABSetupRequestIEs ies.
 * \param s1ap_E_RABSetupRequestIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_e_rabsetuprequesties(
    S1ap_E_RABSetupRequestIEs_t *s1ap_E_RABSetupRequestIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-E-RABSetupRequestIEs ies.
 *  \param s1ap_E_RABSetupRequest Pointer to the ASN1 structure.
 *  \param s1ap_E_RABSetupRequestIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabsetuprequesties(
    S1ap_E_RABSetupRequest_t *s1ap_E_RABSetupRequest,
    S1ap_E_RABSetupRequestIEs_t *s1ap_E_RABSetupRequestIEs);

/** \brief Decode function for S1ap-ENBStatusTransferIEs ies.
 * \param s1ap_ENBStatusTransferIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_enbstatustransferies(
    S1ap_ENBStatusTransferIEs_t *s1ap_ENBStatusTransferIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-ENBStatusTransferIEs ies.
 *  \param s1ap_ENBStatusTransfer Pointer to the ASN1 structure.
 *  \param s1ap_ENBStatusTransferIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_enbstatustransferies(
    S1ap_ENBStatusTransfer_t *s1ap_ENBStatusTransfer,
    S1ap_ENBStatusTransferIEs_t *s1ap_ENBStatusTransferIEs);

/** \brief Decode function for S1ap-ResetAcknowledgeIEs ies.
 * \param s1ap_ResetAcknowledgeIEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_resetacknowledgeies(
    S1ap_ResetAcknowledgeIEs_t *s1ap_ResetAcknowledgeIEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-ResetAcknowledgeIEs ies.
 *  \param s1ap_ResetAcknowledge Pointer to the ASN1 structure.
 *  \param s1ap_ResetAcknowledgeIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_resetacknowledgeies(
    S1ap_ResetAcknowledge_t *s1ap_ResetAcknowledge,
    S1ap_ResetAcknowledgeIEs_t *s1ap_ResetAcknowledgeIEs);

/** \brief Decode function for S1ap-UEContextReleaseComplete-IEs ies.
 * \param s1ap_UEContextReleaseComplete_IEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_uecontextreleasecomplete_ies(
    S1ap_UEContextReleaseComplete_IEs_t *s1ap_UEContextReleaseComplete_IEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-UEContextReleaseComplete-IEs ies.
 *  \param s1ap_UEContextReleaseComplete Pointer to the ASN1 structure.
 *  \param s1ap_UEContextReleaseComplete_IEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_uecontextreleasecomplete_ies(
    S1ap_UEContextReleaseComplete_t *s1ap_UEContextReleaseComplete,
    S1ap_UEContextReleaseComplete_IEs_t *s1ap_UEContextReleaseComplete_IEs);

/** \brief Decode function for S1ap-UplinkUEAssociatedLPPaTransport-IEs ies.
 * \param s1ap_UplinkUEAssociatedLPPaTransport_IEs Pointer to ASN1 structure in which data will be stored
 *  \param any_p Pointer to the ANY value to decode.
 **/
int s1ap_decode_s1ap_uplinkueassociatedlppatransport_ies(
    S1ap_UplinkUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkUEAssociatedLPPaTransport_IEs,
    ANY_t *any_p);

/** \brief Encode function for S1ap-UplinkUEAssociatedLPPaTransport-IEs ies.
 *  \param s1ap_UplinkUEAssociatedLPPaTransport Pointer to the ASN1 structure.
 *  \param s1ap_UplinkUEAssociatedLPPaTransport_IEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_uplinkueassociatedlppatransport_ies(
    S1ap_UplinkUEAssociatedLPPaTransport_t *s1ap_UplinkUEAssociatedLPPaTransport,
    S1ap_UplinkUEAssociatedLPPaTransport_IEs_t *s1ap_UplinkUEAssociatedLPPaTransport_IEs);

/** \brief Encode function for S1ap-E-RABFailedtoSetupItemHOReqAckIEs ies.
 *  \param s1ap_E_RABFailedtoSetupListHOReqAck Pointer to the ASN1 structure.
 *  \param s1ap_E_RABFailedtoSetupItemHOReqAckIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabfailedtosetuplisthoreqack(
    S1ap_E_RABFailedtoSetupListHOReqAck_t *s1ap_E_RABFailedtoSetupListHOReqAck,
    S1ap_E_RABFailedtoSetupListHOReqAck_IEs_t *s1ap_E_RABFailedtoSetupListHOReqAckIEs);

/** \brief Decode function for S1ap-E-RABFailedtoSetupItemHOReqAckIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabfailedtosetuplisthoreqack(
    S1ap_E_RABFailedtoSetupListHOReqAck_IEs_t *s1ap_E_RABFailedtoSetupListHOReqAckIEs,
    S1ap_E_RABFailedtoSetupListHOReqAck_t *s1ap_E_RABFailedtoSetupListHOReqAck);

/** \brief Encode function for S1ap-E-RABReleaseItemBearerRelCompIEs ies.
 *  \param s1ap_E_RABReleaseListBearerRelComp Pointer to the ASN1 structure.
 *  \param s1ap_E_RABReleaseItemBearerRelCompIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabreleaselistbearerrelcomp(
    S1ap_E_RABReleaseListBearerRelComp_t *s1ap_E_RABReleaseListBearerRelComp,
    S1ap_E_RABReleaseListBearerRelComp_IEs_t *s1ap_E_RABReleaseListBearerRelCompIEs);

/** \brief Decode function for S1ap-E-RABReleaseItemBearerRelCompIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabreleaselistbearerrelcomp(
    S1ap_E_RABReleaseListBearerRelComp_IEs_t *s1ap_E_RABReleaseListBearerRelCompIEs,
    S1ap_E_RABReleaseListBearerRelComp_t *s1ap_E_RABReleaseListBearerRelComp);

/** \brief Encode function for S1ap-E-RABToBeSetupItemHOReqIEs ies.
 *  \param s1ap_E_RABToBeSetupListHOReq Pointer to the ASN1 structure.
 *  \param s1ap_E_RABToBeSetupItemHOReqIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabtobesetuplisthoreq(
    S1ap_E_RABToBeSetupListHOReq_t *s1ap_E_RABToBeSetupListHOReq,
    S1ap_E_RABToBeSetupListHOReq_IEs_t *s1ap_E_RABToBeSetupListHOReqIEs);

/** \brief Decode function for S1ap-E-RABToBeSetupItemHOReqIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabtobesetuplisthoreq(
    S1ap_E_RABToBeSetupListHOReq_IEs_t *s1ap_E_RABToBeSetupListHOReqIEs,
    S1ap_E_RABToBeSetupListHOReq_t *s1ap_E_RABToBeSetupListHOReq);

/** \brief Encode function for S1ap-E-RABToBeModifiedItemBearerModReqIEs ies.
 *  \param s1ap_E_RABToBeModifiedListBearerModReq Pointer to the ASN1 structure.
 *  \param s1ap_E_RABToBeModifiedItemBearerModReqIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabtobemodifiedlistbearermodreq(
    S1ap_E_RABToBeModifiedListBearerModReq_t *s1ap_E_RABToBeModifiedListBearerModReq,
    S1ap_E_RABToBeModifiedListBearerModReq_IEs_t *s1ap_E_RABToBeModifiedListBearerModReqIEs);

/** \brief Decode function for S1ap-E-RABToBeModifiedItemBearerModReqIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabtobemodifiedlistbearermodreq(
    S1ap_E_RABToBeModifiedListBearerModReq_IEs_t *s1ap_E_RABToBeModifiedListBearerModReqIEs,
    S1ap_E_RABToBeModifiedListBearerModReq_t *s1ap_E_RABToBeModifiedListBearerModReq);

/** \brief Encode function for S1ap-E-RABAdmittedItemIEs ies.
 *  \param s1ap_E_RABAdmittedList Pointer to the ASN1 structure.
 *  \param s1ap_E_RABAdmittedItemIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabadmittedlist(
    S1ap_E_RABAdmittedList_t *s1ap_E_RABAdmittedList,
    S1ap_E_RABAdmittedList_IEs_t *s1ap_E_RABAdmittedListIEs);

/** \brief Decode function for S1ap-E-RABAdmittedItemIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabadmittedlist(
    S1ap_E_RABAdmittedList_IEs_t *s1ap_E_RABAdmittedListIEs,
    S1ap_E_RABAdmittedList_t *s1ap_E_RABAdmittedList);

/** \brief Encode function for S1ap-E-RABToBeSetupItemBearerSUReqIEs ies.
 *  \param s1ap_E_RABToBeSetupListBearerSUReq Pointer to the ASN1 structure.
 *  \param s1ap_E_RABToBeSetupItemBearerSUReqIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabtobesetuplistbearersureq(
    S1ap_E_RABToBeSetupListBearerSUReq_t *s1ap_E_RABToBeSetupListBearerSUReq,
    S1ap_E_RABToBeSetupListBearerSUReq_IEs_t *s1ap_E_RABToBeSetupListBearerSUReqIEs);

/** \brief Decode function for S1ap-E-RABToBeSetupItemBearerSUReqIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabtobesetuplistbearersureq(
    S1ap_E_RABToBeSetupListBearerSUReq_IEs_t *s1ap_E_RABToBeSetupListBearerSUReqIEs,
    S1ap_E_RABToBeSetupListBearerSUReq_t *s1ap_E_RABToBeSetupListBearerSUReq);

/** \brief Encode function for S1ap-E-RABDataForwardingItemIEs ies.
 *  \param s1ap_E_RABDataForwardingList Pointer to the ASN1 structure.
 *  \param s1ap_E_RABDataForwardingItemIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabdataforwardinglist(
    S1ap_E_RABDataForwardingList_t *s1ap_E_RABDataForwardingList,
    S1ap_E_RABDataForwardingList_IEs_t *s1ap_E_RABDataForwardingListIEs);

/** \brief Decode function for S1ap-E-RABDataForwardingItemIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabdataforwardinglist(
    S1ap_E_RABDataForwardingList_IEs_t *s1ap_E_RABDataForwardingListIEs,
    S1ap_E_RABDataForwardingList_t *s1ap_E_RABDataForwardingList);

/** \brief Encode function for S1ap-E-RABToBeSwitchedDLItemIEs ies.
 *  \param s1ap_E_RABToBeSwitchedDLList Pointer to the ASN1 structure.
 *  \param s1ap_E_RABToBeSwitchedDLItemIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabtobeswitcheddllist(
    S1ap_E_RABToBeSwitchedDLList_t *s1ap_E_RABToBeSwitchedDLList,
    S1ap_E_RABToBeSwitchedDLList_IEs_t *s1ap_E_RABToBeSwitchedDLListIEs);

/** \brief Decode function for S1ap-E-RABToBeSwitchedDLItemIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabtobeswitcheddllist(
    S1ap_E_RABToBeSwitchedDLList_IEs_t *s1ap_E_RABToBeSwitchedDLListIEs,
    S1ap_E_RABToBeSwitchedDLList_t *s1ap_E_RABToBeSwitchedDLList);

/** \brief Encode function for S1ap-E-RABSetupItemCtxtSUResIEs ies.
 *  \param s1ap_E_RABSetupListCtxtSURes Pointer to the ASN1 structure.
 *  \param s1ap_E_RABSetupItemCtxtSUResIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabsetuplistctxtsures(
    S1ap_E_RABSetupListCtxtSURes_t *s1ap_E_RABSetupListCtxtSURes,
    S1ap_E_RABSetupListCtxtSURes_IEs_t *s1ap_E_RABSetupListCtxtSUResIEs);

/** \brief Decode function for S1ap-E-RABSetupItemCtxtSUResIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabsetuplistctxtsures(
    S1ap_E_RABSetupListCtxtSURes_IEs_t *s1ap_E_RABSetupListCtxtSUResIEs,
    S1ap_E_RABSetupListCtxtSURes_t *s1ap_E_RABSetupListCtxtSURes);

/** \brief Encode function for S1ap-E-RABToBeSwitchedULItemIEs ies.
 *  \param s1ap_E_RABToBeSwitchedULList Pointer to the ASN1 structure.
 *  \param s1ap_E_RABToBeSwitchedULItemIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabtobeswitchedullist(
    S1ap_E_RABToBeSwitchedULList_t *s1ap_E_RABToBeSwitchedULList,
    S1ap_E_RABToBeSwitchedULList_IEs_t *s1ap_E_RABToBeSwitchedULListIEs);

/** \brief Decode function for S1ap-E-RABToBeSwitchedULItemIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabtobeswitchedullist(
    S1ap_E_RABToBeSwitchedULList_IEs_t *s1ap_E_RABToBeSwitchedULListIEs,
    S1ap_E_RABToBeSwitchedULList_t *s1ap_E_RABToBeSwitchedULList);

/** \brief Encode function for S1ap-Bearers-SubjectToStatusTransfer-ItemIEs ies.
 *  \param s1ap_Bearers_SubjectToStatusTransfer_List Pointer to the ASN1 structure.
 *  \param s1ap_Bearers_SubjectToStatusTransfer_ItemIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_bearers_subjecttostatustransfer_list(
    S1ap_Bearers_SubjectToStatusTransfer_List_t *s1ap_Bearers_SubjectToStatusTransfer_List,
    S1ap_Bearers_SubjectToStatusTransfer_List_IEs_t *s1ap_Bearers_SubjectToStatusTransfer_ListIEs);

/** \brief Decode function for S1ap-Bearers-SubjectToStatusTransfer-ItemIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_bearers_subjecttostatustransfer_list(
    S1ap_Bearers_SubjectToStatusTransfer_List_IEs_t *s1ap_Bearers_SubjectToStatusTransfer_ListIEs,
    S1ap_Bearers_SubjectToStatusTransfer_List_t *s1ap_Bearers_SubjectToStatusTransfer_List);

/** \brief Encode function for S1ap-E-RABItemIEs ies.
 *  \param s1ap_E_RABList Pointer to the ASN1 structure.
 *  \param s1ap_E_RABItemIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rablist(
    S1ap_E_RABList_t *s1ap_E_RABList,
    S1ap_E_RABList_IEs_t *s1ap_E_RABListIEs);

/** \brief Decode function for S1ap-E-RABItemIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rablist(
    S1ap_E_RABList_IEs_t *s1ap_E_RABListIEs,
    S1ap_E_RABList_t *s1ap_E_RABList);

/** \brief Encode function for S1ap-TAIItemIEs ies.
 *  \param s1ap_TAIList Pointer to the ASN1 structure.
 *  \param s1ap_TAIItemIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_tailist(
    S1ap_TAIList_t *s1ap_TAIList,
    S1ap_TAIList_IEs_t *s1ap_TAIListIEs);

/** \brief Decode function for S1ap-TAIItemIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_tailist(
    S1ap_TAIList_IEs_t *s1ap_TAIListIEs,
    S1ap_TAIList_t *s1ap_TAIList);

/** \brief Encode function for S1ap-UE-associatedLogicalS1-ConnectionItemRes ies.
 *  \param s1ap_UE_associatedLogicalS1_ConnectionListRes Pointer to the ASN1 structure.
 *  \param s1ap_UE_associatedLogicalS1_ConnectionItemRes Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_ue_associatedlogicals1_connectionlistres(
    S1ap_UE_associatedLogicalS1_ConnectionListRes_t *s1ap_UE_associatedLogicalS1_ConnectionListRes,
    S1ap_UE_associatedLogicalS1_ConnectionListRes_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResIEs);

/** \brief Decode function for S1ap-UE-associatedLogicalS1-ConnectionItemRes ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_ue_associatedlogicals1_connectionlistres(
    S1ap_UE_associatedLogicalS1_ConnectionListRes_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResIEs,
    S1ap_UE_associatedLogicalS1_ConnectionListRes_t *s1ap_UE_associatedLogicalS1_ConnectionListRes);

/** \brief Encode function for S1ap-E-RABSetupItemBearerSUResIEs ies.
 *  \param s1ap_E_RABSetupListBearerSURes Pointer to the ASN1 structure.
 *  \param s1ap_E_RABSetupItemBearerSUResIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabsetuplistbearersures(
    S1ap_E_RABSetupListBearerSURes_t *s1ap_E_RABSetupListBearerSURes,
    S1ap_E_RABSetupListBearerSURes_IEs_t *s1ap_E_RABSetupListBearerSUResIEs);

/** \brief Decode function for S1ap-E-RABSetupItemBearerSUResIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabsetuplistbearersures(
    S1ap_E_RABSetupListBearerSURes_IEs_t *s1ap_E_RABSetupListBearerSUResIEs,
    S1ap_E_RABSetupListBearerSURes_t *s1ap_E_RABSetupListBearerSURes);

/** \brief Encode function for S1ap-E-RABModifyItemBearerModResIEs ies.
 *  \param s1ap_E_RABModifyListBearerModRes Pointer to the ASN1 structure.
 *  \param s1ap_E_RABModifyItemBearerModResIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabmodifylistbearermodres(
    S1ap_E_RABModifyListBearerModRes_t *s1ap_E_RABModifyListBearerModRes,
    S1ap_E_RABModifyListBearerModRes_IEs_t *s1ap_E_RABModifyListBearerModResIEs);

/** \brief Decode function for S1ap-E-RABModifyItemBearerModResIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabmodifylistbearermodres(
    S1ap_E_RABModifyListBearerModRes_IEs_t *s1ap_E_RABModifyListBearerModResIEs,
    S1ap_E_RABModifyListBearerModRes_t *s1ap_E_RABModifyListBearerModRes);

/** \brief Encode function for S1ap-E-RABToBeSetupItemCtxtSUReqIEs ies.
 *  \param s1ap_E_RABToBeSetupListCtxtSUReq Pointer to the ASN1 structure.
 *  \param s1ap_E_RABToBeSetupItemCtxtSUReqIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabtobesetuplistctxtsureq(
    S1ap_E_RABToBeSetupListCtxtSUReq_t *s1ap_E_RABToBeSetupListCtxtSUReq,
    S1ap_E_RABToBeSetupListCtxtSUReq_IEs_t *s1ap_E_RABToBeSetupListCtxtSUReqIEs);

/** \brief Decode function for S1ap-E-RABToBeSetupItemCtxtSUReqIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabtobesetuplistctxtsureq(
    S1ap_E_RABToBeSetupListCtxtSUReq_IEs_t *s1ap_E_RABToBeSetupListCtxtSUReqIEs,
    S1ap_E_RABToBeSetupListCtxtSUReq_t *s1ap_E_RABToBeSetupListCtxtSUReq);

/** \brief Encode function for S1ap-E-RABInformationListIEs ies.
 *  \param s1ap_E_RABInformationList Pointer to the ASN1 structure.
 *  \param s1ap_E_RABInformationListIEs Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_e_rabinformationlist(
    S1ap_E_RABInformationList_t *s1ap_E_RABInformationList,
    S1ap_E_RABInformationList_IEs_t *s1ap_E_RABInformationListIEs);

/** \brief Decode function for S1ap-E-RABInformationListIEs ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_e_rabinformationlist(
    S1ap_E_RABInformationList_IEs_t *s1ap_E_RABInformationListIEs,
    S1ap_E_RABInformationList_t *s1ap_E_RABInformationList);

/** \brief Encode function for S1ap-UE-associatedLogicalS1-ConnectionItemResAck ies.
 *  \param s1ap_UE_associatedLogicalS1_ConnectionListResAck Pointer to the ASN1 structure.
 *  \param s1ap_UE_associatedLogicalS1_ConnectionItemResAck Pointer to the IES structure.
 **/
int s1ap_encode_s1ap_ue_associatedlogicals1_connectionlistresack(
    S1ap_UE_associatedLogicalS1_ConnectionListResAck_t *s1ap_UE_associatedLogicalS1_ConnectionListResAck,
    S1ap_UE_associatedLogicalS1_ConnectionListResAck_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs);

/** \brief Decode function for S1ap-UE-associatedLogicalS1-ConnectionItemResAck ies.
 *  \param any_p Pointer to the ANY value to decode.
 *  \param callback Callback function called when any_p is successfully decoded.
 **/
int s1ap_decode_s1ap_ue_associatedlogicals1_connectionlistresack(
    S1ap_UE_associatedLogicalS1_ConnectionListResAck_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs,
    S1ap_UE_associatedLogicalS1_ConnectionListResAck_t *s1ap_UE_associatedLogicalS1_ConnectionListResAck);

/** \brief Display S1ap_E_RABFailedtoSetupListHOReqAck encapsulated IE using XER encoding.
 *  \param s1ap_E_RABFailedtoSetupItemHOReqAckIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabfailedtosetuplisthoreqack(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABFailedtoSetupListHOReqAck_IEs_t *s1ap_E_RABFailedtoSetupListHOReqAckIEs);

/** \brief Display S1ap_DeactivateTrace message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_deactivatetrace(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABReleaseListBearerRelComp encapsulated IE using XER encoding.
 *  \param s1ap_E_RABReleaseItemBearerRelCompIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabreleaselistbearerrelcomp(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABReleaseListBearerRelComp_IEs_t *s1ap_E_RABReleaseListBearerRelCompIEs);

/** \brief Display S1ap_ENBDirectInformationTransfer message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_enbdirectinformationtransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABReleaseResponse message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabreleaseresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UplinkNonUEAssociatedLPPaTransport_ message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_uplinknonueassociatedlppatransport_(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABToBeSetupListHOReq encapsulated IE using XER encoding.
 *  \param s1ap_E_RABToBeSetupItemHOReqIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobesetuplisthoreq(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeSetupListHOReq_IEs_t *s1ap_E_RABToBeSetupListHOReqIEs);

/** \brief Display S1ap_TraceStart message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_tracestart(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_WriteReplaceWarningResponse message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_writereplacewarningresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_HandoverCancel message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_handovercancel(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_HandoverPreparationFailure message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_handoverpreparationfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABToBeModifiedListBearerModReq encapsulated IE using XER encoding.
 *  \param s1ap_E_RABToBeModifiedItemBearerModReqIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobemodifiedlistbearermodreq(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeModifiedListBearerModReq_IEs_t *s1ap_E_RABToBeModifiedListBearerModReqIEs);

/** \brief Display S1ap_UEContextReleaseRequest_ message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_uecontextreleaserequest_(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABAdmittedList encapsulated IE using XER encoding.
 *  \param s1ap_E_RABAdmittedItemIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabadmittedlist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABAdmittedList_IEs_t *s1ap_E_RABAdmittedListIEs);

/** \brief Display S1ap_InitialUEMessage_ message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_initialuemessage_(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_KillRequest message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_killrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_WriteReplaceWarningRequest message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_writereplacewarningrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UECapabilityInfoIndication message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_uecapabilityinfoindication(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABToBeSetupListBearerSUReq encapsulated IE using XER encoding.
 *  \param s1ap_E_RABToBeSetupItemBearerSUReqIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobesetuplistbearersureq(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeSetupListBearerSUReq_IEs_t *s1ap_E_RABToBeSetupListBearerSUReqIEs);

/** \brief Display S1ap_E_RABDataForwardingList encapsulated IE using XER encoding.
 *  \param s1ap_E_RABDataForwardingItemIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabdataforwardinglist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABDataForwardingList_IEs_t *s1ap_E_RABDataForwardingListIEs);

/** \brief Display S1ap_LocationReportingFailureIndication message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_locationreportingfailureindication(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_InitialContextSetupRequest message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_initialcontextsetuprequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_LocationReport message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_locationreport(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UplinkNASTransport_ message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_uplinknastransport_(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UEContextModificationResponse message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_uecontextmodificationresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UEContextModificationRequest message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_uecontextmodificationrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_LocationReportingControl message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_locationreportingcontrol(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UplinkS1cdma2000tunneling message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_uplinks1cdma2000tunneling(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_HandoverRequest message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_handoverrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_HandoverRequired message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_handoverrequired(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABToBeSwitchedDLList encapsulated IE using XER encoding.
 *  \param s1ap_E_RABToBeSwitchedDLItemIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobeswitcheddllist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeSwitchedDLList_IEs_t *s1ap_E_RABToBeSwitchedDLListIEs);

/** \brief Display S1ap_MMEConfigurationUpdateAcknowledge message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_mmeconfigurationupdateacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_KillResponse message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_killresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_MMEStatusTransfer message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_mmestatustransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_MMEConfigurationUpdate message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_mmeconfigurationupdate(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABModifyRequest message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabmodifyrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABSetupListCtxtSURes encapsulated IE using XER encoding.
 *  \param s1ap_E_RABSetupItemCtxtSUResIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabsetuplistctxtsures(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABSetupListCtxtSURes_IEs_t *s1ap_E_RABSetupListCtxtSUResIEs);

/** \brief Display S1ap_E_RABToBeSwitchedULList encapsulated IE using XER encoding.
 *  \param s1ap_E_RABToBeSwitchedULItemIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobeswitchedullist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeSwitchedULList_IEs_t *s1ap_E_RABToBeSwitchedULListIEs);

/** \brief Display S1ap_ENBConfigurationUpdateFailure message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_enbconfigurationupdatefailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_Reset message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_reset(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_Bearers_SubjectToStatusTransfer_List encapsulated IE using XER encoding.
 *  \param s1ap_Bearers_SubjectToStatusTransfer_ItemIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_bearers_subjecttostatustransfer_list(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_Bearers_SubjectToStatusTransfer_List_IEs_t *s1ap_Bearers_SubjectToStatusTransfer_ListIEs);

/** \brief Display S1ap_E_RABList encapsulated IE using XER encoding.
 *  \param s1ap_E_RABItemIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rablist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABList_IEs_t *s1ap_E_RABListIEs);

/** \brief Display S1ap_S1SetupResponse message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_s1setupresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_Paging message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_paging(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_ENBConfigurationUpdateAcknowledge message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_enbconfigurationupdateacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABReleaseCommand message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabreleasecommand(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_PathSwitchRequestFailure message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_pathswitchrequestfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABModifyResponse message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabmodifyresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_HandoverNotify message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_handovernotify(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_HandoverCancelAcknowledge message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_handovercancelacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_InitialContextSetupFailure message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_initialcontextsetupfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_HandoverCommand message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_handovercommand(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_MMEDirectInformationTransfer message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_mmedirectinformationtransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_ErrorIndication message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_errorindication(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_TAIList encapsulated IE using XER encoding.
 *  \param s1ap_TAIItemIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_tailist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_TAIList_IEs_t *s1ap_TAIListIEs);

/** \brief Display S1ap_OverloadStart message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_overloadstart(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_DownlinkNonUEAssociatedLPPaTransport_ message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_downlinknonueassociatedlppatransport_(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_HandoverRequestAcknowledge message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_handoverrequestacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_DownlinkUEAssociatedLPPaTransport_ message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_downlinkueassociatedlppatransport_(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UEContextModificationFailure message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_uecontextmodificationfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_MMEConfigurationTransfer message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_mmeconfigurationtransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_PathSwitchRequestAcknowledge message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_pathswitchrequestacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UE_associatedLogicalS1_ConnectionListRes encapsulated IE using XER encoding.
 *  \param s1ap_UE_associatedLogicalS1_ConnectionItemRes Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_ue_associatedlogicals1_connectionlistres(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_UE_associatedLogicalS1_ConnectionListRes_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResIEs);

/** \brief Display S1ap_S1SetupFailure message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_s1setupfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_MMEConfigurationUpdateFailure message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_mmeconfigurationupdatefailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_InitialContextSetupResponse message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_initialcontextsetupresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_DownlinkS1cdma2000tunneling message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_downlinks1cdma2000tunneling(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_OverloadStop message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_overloadstop(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABSetupListBearerSURes encapsulated IE using XER encoding.
 *  \param s1ap_E_RABSetupItemBearerSUResIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabsetuplistbearersures(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABSetupListBearerSURes_IEs_t *s1ap_E_RABSetupListBearerSUResIEs);

/** \brief Display S1ap_E_RABModifyListBearerModRes encapsulated IE using XER encoding.
 *  \param s1ap_E_RABModifyItemBearerModResIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabmodifylistbearermodres(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABModifyListBearerModRes_IEs_t *s1ap_E_RABModifyListBearerModResIEs);

/** \brief Display S1ap_E_RABToBeSetupListCtxtSUReq encapsulated IE using XER encoding.
 *  \param s1ap_E_RABToBeSetupItemCtxtSUReqIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabtobesetuplistctxtsureq(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABToBeSetupListCtxtSUReq_IEs_t *s1ap_E_RABToBeSetupListCtxtSUReqIEs);

/** \brief Display S1ap_ENBConfigurationUpdate message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_enbconfigurationupdate(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_TraceFailureIndication message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_tracefailureindication(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABInformationList encapsulated IE using XER encoding.
 *  \param s1ap_E_RABInformationListIEs Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabinformationlist(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_E_RABInformationList_IEs_t *s1ap_E_RABInformationListIEs);

/** \brief Display S1ap_ENBConfigurationTransfer message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_enbconfigurationtransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_HandoverFailure message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_handoverfailure(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UE_associatedLogicalS1_ConnectionListResAck encapsulated IE using XER encoding.
 *  \param s1ap_UE_associatedLogicalS1_ConnectionItemResAck Pointer to the IES structure.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_ue_associatedlogicals1_connectionlistresack(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    S1ap_UE_associatedLogicalS1_ConnectionListResAck_IEs_t *s1ap_UE_associatedLogicalS1_ConnectionListResAckIEs);

/** \brief Display S1ap_E_RABSetupResponse message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabsetupresponse(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UEContextReleaseCommand_ message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_uecontextreleasecommand_(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_PathSwitchRequest message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_pathswitchrequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_DownlinkNASTransport_ message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_downlinknastransport_(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABReleaseIndication message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabreleaseindication(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_S1SetupRequest message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_s1setuprequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_NASNonDeliveryIndication_ message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_nasnondeliveryindication_(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_CellTrafficTrace message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_celltraffictrace(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_E_RABSetupRequest message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_e_rabsetuprequest(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_ENBStatusTransfer message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_enbstatustransfer(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_ResetAcknowledge message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_resetacknowledge(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UEContextReleaseComplete_ message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_uecontextreleasecomplete_(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

/** \brief Display S1ap_UplinkUEAssociatedLPPaTransport_ message using XER encoding.
 *  \param message_p Pointer to root message.
 *  \param file File descriptor to write output.
 **/
asn_enc_rval_t s1ap_xer_print_s1ap_uplinkueassociatedlppatransport_(
    asn_app_consume_bytes_f *cb,
    void *app_key,
    s1ap_message *message_p);

int s1ap_xer__print2sp(const void *buffer, size_t size, void *app_key);

int s1ap_xer__print2fp(const void *buffer, size_t size, void *app_key);

extern size_t s1ap_string_total_size;

#endif /* S1AP_IES_DEFS_H_ */

