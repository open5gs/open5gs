/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under 
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */


/** @defgroup _s1ap_impl_ S1AP Layer Reference Implementation
 * @ingroup _ref_implementation_
 * @{
 */

#if HAVE_CONFIG_H_
# include "config.h"
#endif

#ifndef FILE_S1AP_COMMON_SEEN
#define FILE_S1AP_COMMON_SEEN

/* Defined in asn_internal.h */
// extern int asn_debug_indent;
extern int asn_debug;

#if defined(EMIT_ASN_DEBUG_EXTERN)
inline void ASN_DEBUG(const char *fmt, ...);
#endif

#include "S1ap-Criticality.h"
#include "S1ap-Presence.h"
#include "S1ap-PrivateIE-ID.h"
#include "S1ap-ProcedureCode.h"
#include "S1ap-ProtocolExtensionID.h"
#include "S1ap-ProtocolIE-ID.h"
#include "S1ap-TriggeringMessage.h"
#include "S1ap-AllocationAndRetentionPriority.h"
#include "S1ap-Bearers-SubjectToStatusTransferList.h"
#include "S1ap-Bearers-SubjectToStatusTransfer-Item.h"
#include "S1ap-BitRate.h"
#include "S1ap-BPLMNs.h"
#include "S1ap-BroadcastCompletedAreaList.h"
#include "S1ap-Cause.h"
#include "S1ap-CauseMisc.h"
#include "S1ap-CauseProtocol.h"
#include "S1ap-CauseRadioNetwork.h"
#include "S1ap-CauseTransport.h"
#include "S1ap-CauseNas.h"
#include "S1ap-CellIdentity.h"
#include "S1ap-CellID-Broadcast.h"
#include "S1ap-CellID-Broadcast-Item.h"
#include "S1ap-Cdma2000PDU.h"
#include "S1ap-Cdma2000RATType.h"
#include "S1ap-Cdma2000SectorID.h"
#include "S1ap-Cdma2000HOStatus.h"
#include "S1ap-Cdma2000HORequiredIndication.h"
#include "S1ap-Cdma2000OneXSRVCCInfo.h"
#include "S1ap-Cdma2000OneXMEID.h"
#include "S1ap-Cdma2000OneXMSI.h"
#include "S1ap-Cdma2000OneXPilot.h"
#include "S1ap-Cdma2000OneXRAND.h"
#include "S1ap-Cell-Size.h"
#include "S1ap-CellType.h"
#include "S1ap-CGI.h"
#include "S1ap-CI.h"
#include "S1ap-CNDomain.h"
#include "S1ap-CSFallbackIndicator.h"
#include "S1ap-CSG-Id.h"
#include "S1ap-CSG-IdList.h"
#include "S1ap-CSG-IdList-Item.h"
#include "S1ap-COUNTvalue.h"
#include "S1ap-DataCodingScheme.h"
#include "S1ap-DL-Forwarding.h"
#include "S1ap-Direct-Forwarding-Path-Availability.h"
#include "S1ap-ECGIList.h"
#include "S1ap-EmergencyAreaIDList.h"
#include "S1ap-EmergencyAreaID.h"
#include "S1ap-EmergencyAreaID-Broadcast.h"
#include "S1ap-EmergencyAreaID-Broadcast-Item.h"
#include "S1ap-CompletedCellinEAI.h"
#include "S1ap-CompletedCellinEAI-Item.h"
#include "S1ap-ENB-ID.h"
#include "S1ap-GERAN-Cell-ID.h"
#include "S1ap-Global-ENB-ID.h"
#include "S1ap-ENB-StatusTransfer-TransparentContainer.h"
#include "S1ap-ENBname.h"
#include "S1ap-ENBX2TLAs.h"
#include "S1ap-EncryptionAlgorithms.h"
#include "S1ap-EPLMNs.h"
#include "S1ap-EventType.h"
#include "S1ap-E-RAB-ID.h"
#include "S1ap-E-RABInformationList.h"
#include "S1ap-E-RABInformationListItem.h"
#include "S1ap-E-RABList.h"
#include "S1ap-E-RABItem.h"
#include "S1ap-E-RABLevelQoSParameters.h"
#include "S1ap-EUTRAN-CGI.h"
#include "S1ap-ExtendedRNC-ID.h"
#include "S1ap-ForbiddenInterRATs.h"
#include "S1ap-ForbiddenTAs.h"
#include "S1ap-ForbiddenTAs-Item.h"
#include "S1ap-ForbiddenTACs.h"
#include "S1ap-ForbiddenLAs.h"
#include "S1ap-ForbiddenLAs-Item.h"
#include "S1ap-ForbiddenLACs.h"
#include "S1ap-GBR-QosInformation.h"
#include "S1ap-GTP-TEID.h"
#include "S1ap-GUMMEI.h"
#include "S1ap-HandoverRestrictionList.h"
#include "S1ap-HandoverType.h"
#include "S1ap-HFN.h"
#include "S1ap-IMSI.h"
#include "S1ap-IntegrityProtectionAlgorithms.h"
#include "S1ap-InterfacesToTrace.h"
#include "S1ap-LAC.h"
#include "S1ap-LAI.h"
#include "S1ap-LastVisitedCell-Item.h"
#include "S1ap-LastVisitedEUTRANCellInformation.h"
#include "S1ap-LastVisitedUTRANCellInformation.h"
#include "S1ap-LastVisitedGERANCellInformation.h"
#include "S1ap-L3-Information.h"
#include "S1ap-MessageIdentifier.h"
#include "S1ap-MMEname.h"
#include "S1ap-MME-Group-ID.h"
#include "S1ap-MME-Code.h"
#include "S1ap-M-TMSI.h"
#include "S1ap-MSClassmark2.h"
#include "S1ap-MSClassmark3.h"
#include "S1ap-NAS-PDU.h"
#include "S1ap-NASSecurityParametersfromE-UTRAN.h"
#include "S1ap-NASSecurityParameterstoE-UTRAN.h"
#include "S1ap-NumberofBroadcastRequest.h"
#include "S1ap-NumberOfBroadcasts.h"
#include "S1ap-OldBSS-ToNewBSS-Information.h"
#include "S1ap-OverloadAction.h"
#include "S1ap-OverloadResponse.h"
#include "S1ap-PagingDRX.h"
#include "S1ap-PDCP-SN.h"
#include "S1ap-PLMNidentity.h"
#include "S1ap-Pre-emptionCapability.h"
#include "S1ap-Pre-emptionVulnerability.h"
#include "S1ap-PriorityLevel.h"
#include "S1ap-QCI.h"
#include "S1ap-ReceiveStatusofULPDCPSDUs.h"
#include "S1ap-RelativeMMECapacity.h"
#include "S1ap-RAC.h"
#include "S1ap-RequestType.h"
#include "S1ap-RIMTransfer.h"
#include "S1ap-RIMInformation.h"
#include "S1ap-RIMRoutingAddress.h"
#include "S1ap-ReportArea.h"
#include "S1ap-RepetitionPeriod.h"
#include "S1ap-RNC-ID.h"
#include "S1ap-RRC-Container.h"
#include "S1ap-RRC-Establishment-Cause.h"
#include "S1ap-SecurityKey.h"
#include "S1ap-SecurityContext.h"
#include "S1ap-SerialNumber.h"
#include "S1ap-SONInformation.h"
#include "S1ap-SONInformationRequest.h"
#include "S1ap-SONInformationReply.h"
#include "S1ap-SONConfigurationTransfer.h"
#include "S1ap-Source-ToTarget-TransparentContainer.h"
#include "S1ap-SourceBSS-ToTargetBSS-TransparentContainer.h"
#include "S1ap-SourceeNB-ID.h"
#include "S1ap-SRVCCOperationPossible.h"
#include "S1ap-SRVCCHOIndication.h"
#include "S1ap-SourceeNB-ToTargeteNB-TransparentContainer.h"
#include "S1ap-SourceRNC-ToTargetRNC-TransparentContainer.h"
#include "S1ap-ServedGUMMEIs.h"
#include "S1ap-ServedGUMMEIsItem.h"
#include "S1ap-ServedGroupIDs.h"
#include "S1ap-ServedMMECs.h"
#include "S1ap-ServedPLMNs.h"
#include "S1ap-SubscriberProfileIDforRFP.h"
#include "S1ap-SupportedTAs.h"
#include "S1ap-SupportedTAs-Item.h"
#include "S1ap-S-TMSI.h"
#include "S1ap-TAC.h"
#include "S1ap-TAIItem.h"
#include "S1ap-TAIList.h"
#include "S1ap-TAIListforWarning.h"
#include "S1ap-TAI.h"
#include "S1ap-TAI-Broadcast.h"
#include "S1ap-TAI-Broadcast-Item.h"
#include "S1ap-CompletedCellinTAI.h"
#include "S1ap-CompletedCellinTAI-Item.h"
#include "S1ap-TargetID.h"
#include "S1ap-TargeteNB-ID.h"
#include "S1ap-TargetRNC-ID.h"
#include "S1ap-TargeteNB-ToSourceeNB-TransparentContainer.h"
#include "S1ap-Target-ToSource-TransparentContainer.h"
#include "S1ap-TargetRNC-ToSourceRNC-TransparentContainer.h"
#include "S1ap-TargetBSS-ToSourceBSS-TransparentContainer.h"
#include "S1ap-TimeToWait.h"
#include "S1ap-Time-UE-StayedInCell.h"
#include "S1ap-TransportLayerAddress.h"
#include "S1ap-TraceActivation.h"
#include "S1ap-TraceDepth.h"
#include "S1ap-E-UTRAN-Trace-ID.h"
#include "S1ap-TypeOfError.h"
#include "S1ap-UEAggregateMaximumBitrate.h"
#include "S1ap-UE-associatedLogicalS1-ConnectionItem.h"
#include "S1ap-UEIdentityIndexValue.h"
#include "S1ap-UE-HistoryInformation.h"
#include "S1ap-UEPagingID.h"
#include "S1ap-UERadioCapability.h"
#include "S1ap-UESecurityCapabilities.h"
#include "S1ap-WarningAreaList.h"
#include "S1ap-WarningType.h"
#include "S1ap-WarningSecurityInfo.h"
#include "S1ap-WarningMessageContents.h"
#include "S1ap-X2TNLConfigurationInfo.h"
#include "S1ap-CriticalityDiagnostics.h"
#include "S1ap-CriticalityDiagnostics-IE-List.h"
#include "S1ap-CriticalityDiagnostics-IE-Item.h"
#include "S1ap-ResetType.h"
#include "S1ap-Inter-SystemInformationTransferType.h"
#include "S1ap-UE-S1AP-IDs.h"
#include "S1ap-UE-S1AP-ID-pair.h"
#include "S1ap-MME-UE-S1AP-ID.h"
#include "S1ap-ENB-UE-S1AP-ID.h"
#include "S1ap-TBCD-STRING.h"
#include "S1ap-InitiatingMessage.h"
#include "S1ap-SuccessfulOutcome.h"
#include "S1ap-UnsuccessfulOutcome.h"
#include "S1ap-HandoverRequired.h"
#include "S1ap-HandoverCommand.h"
#include "S1ap-HandoverNotify.h"
#include "S1ap-HandoverPreparationFailure.h"
#include "S1ap-HandoverRequest.h"
#include "S1ap-HandoverRequestAcknowledge.h"
#include "S1ap-HandoverFailure.h"
#include "S1ap-PathSwitchRequest.h"
#include "S1ap-PathSwitchRequestAcknowledge.h"
#include "S1ap-PathSwitchRequestFailure.h"
#include "S1ap-E-RABSetupRequest.h"
#include "S1ap-E-RABSetupResponse.h"
#include "S1ap-E-RABModifyRequest.h"
#include "S1ap-E-RABModifyResponse.h"
#include "S1ap-E-RABReleaseIndication.h"
#include "S1ap-E-RABReleaseCommand.h"
#include "S1ap-E-RABReleaseResponse.h"
#include "S1ap-InitialContextSetupRequest.h"
#include "S1ap-InitialContextSetupResponse.h"
#include "S1ap-InitialContextSetupFailure.h"
#include "S1ap-UEContextReleaseRequest.h"
#include "S1ap-Paging.h"
#include "S1ap-DownlinkNASTransport.h"
#include "S1ap-InitialUEMessage.h"
#include "S1ap-UplinkNASTransport.h"
#include "S1ap-NASNonDeliveryIndication.h"
#include "S1ap-HandoverCancel.h"
#include "S1ap-HandoverCancelAcknowledge.h"
#include "S1ap-Reset.h"
#include "S1ap-ResetAcknowledge.h"
#include "S1ap-S1SetupResponse.h"
#include "S1ap-S1SetupRequest.h"
#include "S1ap-S1SetupFailure.h"
#include "S1ap-ErrorIndication.h"
#include "S1ap-ENBConfigurationUpdate.h"
#include "S1ap-ENBConfigurationUpdateAcknowledge.h"
#include "S1ap-ENBConfigurationUpdateFailure.h"
#include "S1ap-MMEConfigurationUpdate.h"
#include "S1ap-MMEConfigurationUpdateAcknowledge.h"
#include "S1ap-MMEConfigurationUpdateFailure.h"
#include "S1ap-DownlinkS1cdma2000tunneling.h"
#include "S1ap-UplinkS1cdma2000tunneling.h"
#include "S1ap-UEContextModificationRequest.h"
#include "S1ap-UEContextModificationResponse.h"
#include "S1ap-UEContextModificationFailure.h"
#include "S1ap-UECapabilityInfoIndication.h"
#include "S1ap-UEContextReleaseCommand.h"
#include "S1ap-UEContextReleaseComplete.h"
#include "S1ap-ENBStatusTransfer.h"
#include "S1ap-MMEStatusTransfer.h"
#include "S1ap-DeactivateTrace.h"
#include "S1ap-TraceStart.h"
#include "S1ap-TraceFailureIndication.h"
#include "S1ap-CellTrafficTrace.h"
#include "S1ap-LocationReportingControl.h"
#include "S1ap-LocationReportingFailureIndication.h"
#include "S1ap-LocationReport.h"
#include "S1ap-OverloadStart.h"
#include "S1ap-OverloadStop.h"
#include "S1ap-WriteReplaceWarningRequest.h"
#include "S1ap-WriteReplaceWarningResponse.h"
#include "S1ap-ENBDirectInformationTransfer.h"
#include "S1ap-MMEDirectInformationTransfer.h"
#include "S1ap-ENBConfigurationTransfer.h"
#include "S1ap-MMEConfigurationTransfer.h"
#include "S1ap-PrivateMessage.h"
#include "S1ap-E-RABReleaseItemBearerRelComp.h"
#include "S1ap-E-RABToBeSwitchedDLList.h"
#include "S1ap-E-RABToBeSwitchedDLItem.h"
#include "S1ap-E-RABToBeSwitchedULList.h"
#include "S1ap-E-RABToBeSwitchedULItem.h"
#include "S1ap-E-RABToBeSetupListBearerSUReq.h"
#include "S1ap-E-RABToBeSetupItemBearerSUReq.h"
#include "S1ap-E-RABSubjecttoDataForwardingList.h"
#include "S1ap-E-RABDataForwardingItem.h"
#include "S1ap-E-RABToBeSetupListHOReq.h"
#include "S1ap-E-RABToBeSetupItemHOReq.h"
#include "S1ap-E-RABAdmittedList.h"
#include "S1ap-E-RABAdmittedItem.h"
#include "S1ap-E-RABFailedToSetupListHOReqAck.h"
#include "S1ap-E-RABToBeSetupItemCtxtSUReq.h"
#include "S1ap-E-RABToBeSetupListCtxtSUReq.h"
#include "S1ap-E-RABSetupItemBearerSURes.h"
#include "S1ap-E-RABSetupListBearerSURes.h"
#include "S1ap-E-RABSetupItemCtxtSURes.h"
#include "S1ap-E-RABSetupListCtxtSURes.h"
#include "S1ap-E-RABReleaseListBearerRelComp.h"
#include "S1ap-E-RABModifyItemBearerModRes.h"
#include "S1ap-E-RABModifyListBearerModRes.h"
#include "S1ap-E-RABFailedToSetupItemHOReqAck.h"
#include "S1ap-E-RABFailedToSetupListHOReqAck.h"
#include "S1ap-E-RABToBeModifiedItemBearerModReq.h"
#include "S1ap-E-RABToBeModifiedListBearerModReq.h"
#include "S1ap-UE-associatedLogicalS1-ConnectionListResAck.h"
#include "S1ap-IE.h"
#include "S1AP-PDU.h"

// UPDATE RELEASE 9
# include "S1ap-BroadcastCancelledAreaList.h"
# include "S1ap-CancelledCellinEAI.h"
# include "S1ap-CancelledCellinEAI-Item.h"
# include "S1ap-CancelledCellinTAI.h"
# include "S1ap-CancelledCellinTAI-Item.h"
# include "S1ap-CellAccessMode.h"
# include "S1ap-CellID-Cancelled.h"
# include "S1ap-CellID-Cancelled-Item.h"
# include "S1ap-ConcurrentWarningMessageIndicator.h"
# include "S1ap-CSGMembershipStatus.h"
# include "S1ap-Data-Forwarding-Not-Possible.h"
# include "S1ap-EmergencyAreaID-Cancelled.h"
# include "S1ap-EmergencyAreaID-Cancelled-Item.h"
# include "S1ap-E-RABList.h"
# include "S1ap-EUTRANRoundTripDelayEstimationInfo.h"
# include "S1ap-ExtendedRepetitionPeriod.h"
# include "S1ap-LPPa-PDU.h"
# include "S1ap-PS-ServiceNotAvailable.h"
# include "S1ap-Routing-ID.h"
# include "S1ap-StratumLevel.h"
# include "S1ap-SynchronizationStatus.h"
# include "S1ap-TimeSynchronizationInfo.h"
# include "S1ap-TAI-Cancelled.h"
# include "S1ap-TAI-Cancelled-Item.h"
# include "S1ap-KillRequest.h"
# include "S1ap-KillResponse.h"
# include "S1ap-DownlinkUEAssociatedLPPaTransport.h"
# include "S1ap-UplinkUEAssociatedLPPaTransport.h"
# include "S1ap-DownlinkNonUEAssociatedLPPaTransport.h"
# include "S1ap-UplinkNonUEAssociatedLPPaTransport.h"

// UPDATE RELEASE 10
# include "S1ap-PagingPriority.h"
# include "S1ap-RelayNode-Indicator.h"
# include "S1ap-GWContextReleaseIndication.h"
# include "S1ap-MMERelaySupportIndicator.h"
# include "S1ap-ManagementBasedMDTAllowed.h"
# include "S1ap-PrivacyIndicator.h"
# include "S1ap-TrafficLoadReductionIndication.h"
# include "S1ap-GUMMEIList.h"

/* Checking version of ASN1C compiler */
#if (ASN1C_ENVIRONMENT_VERSION < ASN1C_MINIMUM_VERSION)
# error "You are compiling s1ap with the wrong version of ASN1C"
#endif


extern int asn_debug;
extern int asn1_xer_print;

# include "mme_default_values.h"

//Forward declaration
struct s1ap_message_s;

/** \brief Encode a successfull outcome message
 \param buffer pointer to buffer in which data will be encoded
 \param length pointer to the length of buffer
 \param procedureCode Procedure code for the message
 \param criticality Criticality of the message
 \param td ASN1C type descriptor of the sptr
 \param sptr Dereferenced pointer to the structure to encode
 @returns size in bytes encded on success or 0 on failure
 **/
ssize_t s1ap_generate_successfull_outcome(
  uint8_t               **buffer,
  uint32_t               *length,
  S1ap_ProcedureCode_t    procedureCode,
  S1ap_Criticality_t      criticality,
  asn_TYPE_descriptor_t  *td,
  void                   *sptr);

/** \brief Encode an initiating message
 \param buffer pointer to buffer in which data will be encoded
 \param length pointer to the length of buffer
 \param procedureCode Procedure code for the message
 \param criticality Criticality of the message
 \param td ASN1C type descriptor of the sptr
 \param sptr Dereferenced pointer to the structure to encode
 @returns size in bytes encded on success or 0 on failure
 **/
ssize_t s1ap_generate_initiating_message(
  uint8_t               **buffer,
  uint32_t               *length,
  S1ap_ProcedureCode_t    procedureCode,
  S1ap_Criticality_t      criticality,
  asn_TYPE_descriptor_t  *td,
  void                   *sptr);

/** \brief Encode an unsuccessfull outcome message
 \param buffer pointer to buffer in which data will be encoded
 \param length pointer to the length of buffer
 \param procedureCode Procedure code for the message
 \param criticality Criticality of the message
 \param td ASN1C type descriptor of the sptr
 \param sptr Dereferenced pointer to the structure to encode
 @returns size in bytes encded on success or 0 on failure
 **/
ssize_t s1ap_generate_unsuccessfull_outcome(
  uint8_t               **buffer,
  uint32_t               *length,
  S1ap_ProcedureCode_t    procedureCode,
  S1ap_Criticality_t      criticality,
  asn_TYPE_descriptor_t  *td,
  void                   *sptr);

/** \brief Generate a new IE
 \param id Protocol ie id of the IE
 \param criticality Criticality of the IE
 \param type ASN1 type descriptor of the IE value
 \param sptr Structure to be encoded in the value field
 @returns a pointer to the newly created IE structure or NULL in case of failure
 **/
S1ap_IE_t *s1ap_new_ie(S1ap_ProtocolIE_ID_t   id,
                       S1ap_Criticality_t     criticality,
                       asn_TYPE_descriptor_t *type,
                       void                  *sptr);

/** \brief Handle criticality
 \param criticality Criticality of the IE
 @returns void
 **/
void s1ap_handle_criticality(S1ap_Criticality_t criticality);

#endif /* FILE_S1AP_COMMON_SEEN */
