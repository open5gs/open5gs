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

ogs_pkbuf_t *ngap_build_pdu_session_resource_setup_request_transfer(
        smf_sess_t *sess)
{
    smf_bearer_t *qos_flow = NULL;
    ogs_ip_t upf_n3_ip;

    NGAP_PDUSessionResourceSetupRequestTransfer_t message;

    NGAP_PDUSessionResourceSetupRequestTransferIEs_t *ie = NULL;
    NGAP_PDUSessionAggregateMaximumBitRate_t *PDUSessionAggregateMaximumBitRate;
    NGAP_UPTransportLayerInformation_t *UPTransportLayerInformation = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;
    NGAP_PDUSessionType_t *PDUSessionType = NULL;
    NGAP_QosFlowSetupRequestList_t *QosFlowSetupRequestList = NULL;
    NGAP_QosFlowSetupRequestItem_t *QosFlowSetupRequestItem = NULL;
    NGAP_QosFlowIdentifier_t *qosFlowIdentifier = NULL;
    NGAP_QosFlowLevelQosParameters_t *qosFlowLevelQosParameters = NULL;
    NGAP_QosCharacteristics_t *qosCharacteristics = NULL;
    NGAP_NonDynamic5QIDescriptor_t *nonDynamic5QI = NULL;
    NGAP_AllocationAndRetentionPriority_t *allocationAndRetentionPriority;

    ogs_assert(sess);
    qos_flow = smf_default_bearer_in_sess(sess);
    ogs_assert(qos_flow);

    ogs_debug("PDUSessionResourceSetupRequestTransfer");
    memset(&message, 0, sizeof(NGAP_PDUSessionResourceSetupRequestTransfer_t));

    if (sess->pdn.ambr.downlink || sess->pdn.ambr.uplink) {
        ie = CALLOC(1,
                sizeof(NGAP_PDUSessionResourceSetupRequestTransferIEs_t));
        ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_PDUSessionAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present = NGAP_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionAggregateMaximumBitRate;

        PDUSessionAggregateMaximumBitRate =
            &ie->value.choice.PDUSessionAggregateMaximumBitRate;

        asn_uint642INTEGER(&PDUSessionAggregateMaximumBitRate->
            pDUSessionAggregateMaximumBitRateUL, sess->pdn.ambr.uplink);
        asn_uint642INTEGER(&PDUSessionAggregateMaximumBitRate->
            pDUSessionAggregateMaximumBitRateDL, sess->pdn.ambr.downlink);
    }

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestTransferIEs_t));
    ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceSetupRequestTransferIEs__value_PR_UPTransportLayerInformation;

    UPTransportLayerInformation = &ie->value.choice.UPTransportLayerInformation;

    gTPTunnel = CALLOC(1, sizeof(struct NGAP_GTPTunnel));
    UPTransportLayerInformation->present =
        NGAP_UPTransportLayerInformation_PR_gTPTunnel;
    UPTransportLayerInformation->choice.gTPTunnel = gTPTunnel;

    ogs_assert(sess->upf_n3_addr || sess->upf_n3_addr6);
    ogs_sockaddr_to_ip(sess->upf_n3_addr, sess->upf_n3_addr6, &upf_n3_ip);
    ogs_asn_ip_to_BIT_STRING(&upf_n3_ip, &gTPTunnel->transportLayerAddress);
    ogs_asn_uint32_to_OCTET_STRING(sess->upf_n3_teid, &gTPTunnel->gTP_TEID);

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestTransferIEs_t));
    ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionType;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionType;

    PDUSessionType = &ie->value.choice.PDUSessionType;

    *PDUSessionType = OGS_PDU_SESSION_TYPE_IPV4;
    switch (sess->pdn.pdn_type) {
    case OGS_PDU_SESSION_TYPE_IPV4 :
        *PDUSessionType = NGAP_PDUSessionType_ipv4;
        break;
    case OGS_PDU_SESSION_TYPE_IPV6 :
        *PDUSessionType = NGAP_PDUSessionType_ipv6;
        break;
    case OGS_PDU_SESSION_TYPE_IPV4V6 :
        *PDUSessionType = NGAP_PDUSessionType_ipv4v6;
        break;
    default:
        ogs_fatal("Unknown PDU Session Type [%d]", sess->pdn.pdn_type);
        ogs_assert_if_reached();
    }

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestTransferIEs_t));
    ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_QosFlowSetupRequestList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceSetupRequestTransferIEs__value_PR_QosFlowSetupRequestList;

    QosFlowSetupRequestList = &ie->value.choice.QosFlowSetupRequestList;

    QosFlowSetupRequestItem =
        CALLOC(1, sizeof(struct NGAP_QosFlowSetupRequestItem));
    ASN_SEQUENCE_ADD(&QosFlowSetupRequestList->list, QosFlowSetupRequestItem);

    qosFlowIdentifier = &QosFlowSetupRequestItem->qosFlowIdentifier;
    qosFlowLevelQosParameters =
        &QosFlowSetupRequestItem->qosFlowLevelQosParameters;

    allocationAndRetentionPriority =
        &qosFlowLevelQosParameters->allocationAndRetentionPriority;
    qosCharacteristics = &qosFlowLevelQosParameters->qosCharacteristics;
    nonDynamic5QI = CALLOC(1, sizeof(struct NGAP_NonDynamic5QIDescriptor));
    qosCharacteristics->choice.nonDynamic5QI = nonDynamic5QI;
    qosCharacteristics->present = NGAP_QosCharacteristics_PR_nonDynamic5QI;

    *qosFlowIdentifier = qos_flow->qfi;

    nonDynamic5QI->fiveQI = sess->pdn.qos.qci;

    allocationAndRetentionPriority->priorityLevelARP =
        sess->pdn.qos.arp.priority_level;;
    if (sess->pdn.qos.arp.pre_emption_capability ==
            OGS_PDN_PRE_EMPTION_CAPABILITY_DISABLED)
        allocationAndRetentionPriority->pre_emptionCapability =
            NGAP_Pre_emptionCapability_shall_not_trigger_pre_emption;
    else
        allocationAndRetentionPriority->pre_emptionCapability =
            NGAP_Pre_emptionCapability_may_trigger_pre_emption;

    if (sess->pdn.qos.arp.pre_emption_vulnerability ==
            OGS_PDN_PRE_EMPTION_VULNERABILITY_DISABLED)
        allocationAndRetentionPriority->pre_emptionVulnerability =
            NGAP_Pre_emptionVulnerability_not_pre_emptable;
    else
        allocationAndRetentionPriority->pre_emptionVulnerability =
            NGAP_Pre_emptionVulnerability_pre_emptable;

    return ogs_asn_encode(
            &asn_DEF_NGAP_PDUSessionResourceSetupRequestTransfer, &message);
}

ogs_pkbuf_t *ngap_build_pdu_session_resource_release_command_transfer(
        NGAP_Cause_PR group, long cause)
{
    NGAP_PDUSessionResourceReleaseCommandTransfer_t message;
    NGAP_Cause_t *Cause = NULL;

    ogs_debug("PDUSessionResourceReleaseCommandTransfer");
    memset(&message, 0,
            sizeof(NGAP_PDUSessionResourceReleaseCommandTransfer_t));

    ogs_debug("    Group[%d] Cause[%d]", group, (int)cause);

    Cause = &message.cause;
    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_asn_encode(
            &asn_DEF_NGAP_PDUSessionResourceReleaseCommandTransfer, &message);
}
