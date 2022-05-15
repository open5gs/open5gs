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
    NGAP_DataForwardingNotPossible_t *DataForwardingNotPossible = NULL;
    NGAP_PDUSessionType_t *PDUSessionType = NULL;
    NGAP_QosFlowSetupRequestList_t *QosFlowSetupRequestList = NULL;
    NGAP_QosFlowSetupRequestItem_t *QosFlowSetupRequestItem = NULL;
    NGAP_QosFlowIdentifier_t *qosFlowIdentifier = NULL;
    NGAP_QosFlowLevelQosParameters_t *qosFlowLevelQosParameters = NULL;
    NGAP_QosCharacteristics_t *qosCharacteristics = NULL;
    NGAP_NonDynamic5QIDescriptor_t *nonDynamic5QI = NULL;
    NGAP_AllocationAndRetentionPriority_t *allocationAndRetentionPriority;
    NGAP_GBR_QosInformation_t *gBR_QosInformation = NULL;

    ogs_assert(sess);

    ogs_debug("PDUSessionResourceSetupRequestTransfer");
    memset(&message, 0, sizeof(NGAP_PDUSessionResourceSetupRequestTransfer_t));

    if (sess->session.ambr.downlink || sess->session.ambr.uplink) {
        ie = CALLOC(1,
                sizeof(NGAP_PDUSessionResourceSetupRequestTransferIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_PDUSessionAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present = NGAP_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionAggregateMaximumBitRate;

        PDUSessionAggregateMaximumBitRate =
            &ie->value.choice.PDUSessionAggregateMaximumBitRate;

        asn_uint642INTEGER(&PDUSessionAggregateMaximumBitRate->
            pDUSessionAggregateMaximumBitRateUL, sess->session.ambr.uplink);
        asn_uint642INTEGER(&PDUSessionAggregateMaximumBitRate->
            pDUSessionAggregateMaximumBitRateDL, sess->session.ambr.downlink);
    }

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceSetupRequestTransferIEs__value_PR_UPTransportLayerInformation;

    UPTransportLayerInformation = &ie->value.choice.UPTransportLayerInformation;

    gTPTunnel = CALLOC(1, sizeof(struct NGAP_GTPTunnel));
    ogs_assert(gTPTunnel);
    UPTransportLayerInformation->present =
        NGAP_UPTransportLayerInformation_PR_gTPTunnel;
    UPTransportLayerInformation->choice.gTPTunnel = gTPTunnel;

    ogs_assert(OGS_OK == ogs_sockaddr_to_ip(
                sess->upf_n3_addr, sess->upf_n3_addr6, &upf_n3_ip));
    ogs_asn_ip_to_BIT_STRING(&upf_n3_ip, &gTPTunnel->transportLayerAddress);
    ogs_asn_uint32_to_OCTET_STRING(sess->upf_n3_teid, &gTPTunnel->gTP_TEID);

    if (sess->handover.data_forwarding_not_possible == true) {
        ie = CALLOC(1,
                sizeof(NGAP_PDUSessionResourceSetupRequestTransferIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_DataForwardingNotPossible;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present = NGAP_PDUSessionResourceSetupRequestTransferIEs__value_PR_DataForwardingNotPossible;

        DataForwardingNotPossible = &ie->value.choice.DataForwardingNotPossible;

        *DataForwardingNotPossible =
            NGAP_DataForwardingNotPossible_data_forwarding_not_possible;
    }

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionType;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionType;

    PDUSessionType = &ie->value.choice.PDUSessionType;

    *PDUSessionType = OGS_PDU_SESSION_TYPE_IPV4;
    switch (sess->session.session_type) {
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
        ogs_fatal("Unknown PDU Session Type [%d]", sess->session.session_type);
        ogs_assert_if_reached();
    }

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_QosFlowSetupRequestList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceSetupRequestTransferIEs__value_PR_QosFlowSetupRequestList;

    QosFlowSetupRequestList = &ie->value.choice.QosFlowSetupRequestList;

    ogs_list_for_each(&sess->bearer_list, qos_flow) {
        QosFlowSetupRequestItem =
            CALLOC(1, sizeof(struct NGAP_QosFlowSetupRequestItem));
        ogs_assert(QosFlowSetupRequestItem);
        ASN_SEQUENCE_ADD(&QosFlowSetupRequestList->list,
            QosFlowSetupRequestItem);

        qosFlowIdentifier = &QosFlowSetupRequestItem->qosFlowIdentifier;
        qosFlowLevelQosParameters =
            &QosFlowSetupRequestItem->qosFlowLevelQosParameters;

        allocationAndRetentionPriority =
            &qosFlowLevelQosParameters->allocationAndRetentionPriority;
        qosCharacteristics = &qosFlowLevelQosParameters->qosCharacteristics;
        nonDynamic5QI = CALLOC(1, sizeof(struct NGAP_NonDynamic5QIDescriptor));
        ogs_assert(nonDynamic5QI);
        qosCharacteristics->choice.nonDynamic5QI = nonDynamic5QI;
        qosCharacteristics->present = NGAP_QosCharacteristics_PR_nonDynamic5QI;

        *qosFlowIdentifier = qos_flow->qfi;

        nonDynamic5QI->fiveQI = qos_flow->qos.index;

        allocationAndRetentionPriority->priorityLevelARP =
            qos_flow->qos.arp.priority_level;
        if (qos_flow->qos.arp.pre_emption_capability ==
                OGS_5GC_PRE_EMPTION_ENABLED)
            allocationAndRetentionPriority->pre_emptionCapability =
                NGAP_Pre_emptionCapability_may_trigger_pre_emption;
        if (qos_flow->qos.arp.pre_emption_vulnerability ==
                OGS_5GC_PRE_EMPTION_ENABLED)
            allocationAndRetentionPriority->pre_emptionVulnerability =
                NGAP_Pre_emptionVulnerability_pre_emptable;

        if (qos_flow->qos.mbr.downlink || qos_flow->qos.mbr.uplink ||
            qos_flow->qos.gbr.downlink || qos_flow->qos.gbr.uplink) {
            ogs_assert(qos_flow->qos.mbr.downlink);
            ogs_assert(qos_flow->qos.mbr.uplink);
            ogs_assert(qos_flow->qos.gbr.downlink);
            ogs_assert(qos_flow->qos.gbr.uplink);

            qosFlowLevelQosParameters->gBR_QosInformation =
                gBR_QosInformation = CALLOC(1, sizeof(*gBR_QosInformation));
            ogs_assert(gBR_QosInformation);

            asn_uint642INTEGER(&gBR_QosInformation->maximumFlowBitRateDL,
                    qos_flow->qos.mbr.downlink);
            asn_uint642INTEGER(&gBR_QosInformation->maximumFlowBitRateUL,
                    qos_flow->qos.mbr.uplink);
            asn_uint642INTEGER(&gBR_QosInformation->
                    guaranteedFlowBitRateDL, qos_flow->qos.gbr.downlink);
            asn_uint642INTEGER(&gBR_QosInformation->
                    guaranteedFlowBitRateUL, qos_flow->qos.gbr.uplink);
        }
    }

    return ogs_asn_encode(
            &asn_DEF_NGAP_PDUSessionResourceSetupRequestTransfer, &message);
}

ogs_pkbuf_t *ngap_build_pdu_session_resource_modify_request_transfer(
        smf_sess_t *sess, bool qos_presence)
{
    NGAP_PDUSessionResourceModifyRequestTransfer_t message;

    NGAP_PDUSessionResourceModifyRequestTransferIEs_t *ie = NULL;

    NGAP_QosFlowAddOrModifyRequestList_t *QosFlowAddOrModifyRequestList = NULL;
    NGAP_QosFlowAddOrModifyRequestItem_t *QosFlowAddOrModifyRequestItem = NULL;
    NGAP_QosFlowIdentifier_t *qosFlowIdentifier = NULL;
    NGAP_QosFlowLevelQosParameters_t *qosFlowLevelQosParameters = NULL;
    NGAP_QosCharacteristics_t *qosCharacteristics = NULL;
    NGAP_NonDynamic5QIDescriptor_t *nonDynamic5QI = NULL;
    NGAP_AllocationAndRetentionPriority_t *allocationAndRetentionPriority;
    NGAP_GBR_QosInformation_t *gBR_QosInformation;

    smf_bearer_t *qos_flow = NULL;

    ogs_assert(sess);

    ogs_debug("PDUSessionResourceModifyRequestTransfer");
    memset(&message, 0, sizeof(NGAP_PDUSessionResourceModifyRequestTransfer_t));

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyRequestTransferIEs_t));
    ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_QosFlowAddOrModifyRequestList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceModifyRequestTransferIEs__value_PR_QosFlowAddOrModifyRequestList;

    QosFlowAddOrModifyRequestList = &ie->value.choice.QosFlowAddOrModifyRequestList;

    ogs_list_for_each_entry(
            &sess->qos_flow_to_modify_list, qos_flow, to_modify_node) {

        QosFlowAddOrModifyRequestItem =
            CALLOC(1, sizeof(*QosFlowAddOrModifyRequestItem));
        ASN_SEQUENCE_ADD(&QosFlowAddOrModifyRequestList->list, QosFlowAddOrModifyRequestItem);

        qosFlowIdentifier = &QosFlowAddOrModifyRequestItem->qosFlowIdentifier;

        QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters =
            qosFlowLevelQosParameters =
                CALLOC(1, sizeof(*qosFlowLevelQosParameters));

        allocationAndRetentionPriority =
            &qosFlowLevelQosParameters->allocationAndRetentionPriority;
        qosCharacteristics = &qosFlowLevelQosParameters->qosCharacteristics;

        qosCharacteristics->present = NGAP_QosCharacteristics_PR_nonDynamic5QI;
        qosCharacteristics->choice.nonDynamic5QI =
            nonDynamic5QI = CALLOC(1, sizeof(struct NGAP_NonDynamic5QIDescriptor));

        *qosFlowIdentifier = qos_flow->qfi;

        nonDynamic5QI->fiveQI = qos_flow->qos.index;

        allocationAndRetentionPriority->priorityLevelARP =
            qos_flow->qos.arp.priority_level;
        if (qos_flow->qos.arp.pre_emption_capability ==
                OGS_5GC_PRE_EMPTION_ENABLED)
            allocationAndRetentionPriority->pre_emptionCapability =
                NGAP_Pre_emptionCapability_may_trigger_pre_emption;
        if (qos_flow->qos.arp.pre_emption_vulnerability ==
                OGS_5GC_PRE_EMPTION_ENABLED)
            allocationAndRetentionPriority->pre_emptionVulnerability =
                NGAP_Pre_emptionVulnerability_pre_emptable;

        if (qos_presence == true &&
            (qos_flow->qos.mbr.downlink || qos_flow->qos.mbr.uplink ||
             qos_flow->qos.gbr.downlink || qos_flow->qos.gbr.uplink)) {
            ogs_assert(qos_flow->qos.mbr.downlink);
            ogs_assert(qos_flow->qos.mbr.uplink);
            ogs_assert(qos_flow->qos.gbr.downlink);
            ogs_assert(qos_flow->qos.gbr.uplink);

            qosFlowLevelQosParameters->gBR_QosInformation =
                gBR_QosInformation = CALLOC(1, sizeof(*gBR_QosInformation));

            asn_uint642INTEGER(&gBR_QosInformation->maximumFlowBitRateDL,
                    qos_flow->qos.mbr.downlink);
            asn_uint642INTEGER(&gBR_QosInformation->maximumFlowBitRateUL,
                    qos_flow->qos.mbr.uplink);
            asn_uint642INTEGER(&gBR_QosInformation->
                    guaranteedFlowBitRateDL, qos_flow->qos.gbr.downlink);
            asn_uint642INTEGER(&gBR_QosInformation->
                    guaranteedFlowBitRateUL, qos_flow->qos.gbr.uplink);
        }
    }

    return ogs_asn_encode(
            &asn_DEF_NGAP_PDUSessionResourceModifyRequestTransfer, &message);
}

ogs_pkbuf_t *ngap_build_pdu_session_resource_release_request_transfer(
        smf_sess_t *sess, NGAP_Cause_PR group, long cause)
{
    NGAP_PDUSessionResourceModifyRequestTransfer_t message;

    NGAP_PDUSessionResourceModifyRequestTransferIEs_t *ie = NULL;

    NGAP_QosFlowListWithCause_t *QosFlowListWithCause = NULL;
    NGAP_QosFlowWithCauseItem_t *QosFlowWithCauseItem = NULL;
    NGAP_QosFlowIdentifier_t *qosFlowIdentifier = NULL;
    NGAP_Cause_t *Cause = NULL;

    smf_bearer_t *qos_flow = NULL;

    ogs_assert(sess);

    ogs_debug("PDUSessionResourceModifyRequestTransfer");
    memset(&message, 0, sizeof(NGAP_PDUSessionResourceModifyRequestTransfer_t));

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyRequestTransferIEs_t));
    ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_QosFlowToReleaseList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceModifyRequestTransferIEs__value_PR_QosFlowListWithCause;

    QosFlowListWithCause = &ie->value.choice.QosFlowListWithCause;

    ogs_list_for_each_entry(
            &sess->qos_flow_to_modify_list, qos_flow, to_modify_node) {

        QosFlowWithCauseItem = CALLOC(1, sizeof(*QosFlowWithCauseItem));
        ASN_SEQUENCE_ADD(&QosFlowListWithCause->list, QosFlowWithCauseItem);

        qosFlowIdentifier = &QosFlowWithCauseItem->qosFlowIdentifier;

        *qosFlowIdentifier = qos_flow->qfi;

        Cause = &QosFlowWithCauseItem->cause;
        Cause->present = group;
        Cause->choice.radioNetwork = cause;

    }

    return ogs_asn_encode(
            &asn_DEF_NGAP_PDUSessionResourceModifyRequestTransfer, &message);
}

ogs_pkbuf_t *ngap_build_pdu_session_resource_release_command_transfer(
        smf_sess_t *sess, int state, NGAP_Cause_PR group, long cause)
{
    NGAP_PDUSessionResourceReleaseCommandTransfer_t message;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(sess);
    ogs_assert(state);
    sess->ngap_state.pdu_session_resource_release = state;

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

ogs_pkbuf_t *ngap_build_path_switch_request_ack_transfer(smf_sess_t *sess)
{
    NGAP_PathSwitchRequestAcknowledgeTransfer_t message;

#if 0 /* The following is optional. So I've removed */
    ogs_ip_t upf_n3_ip;

    NGAP_UPTransportLayerInformation_t *UPTransportLayerInformation = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;
#endif

    ogs_assert(sess);

    ogs_debug("PathSwitchRequestAcknowledgeTransfer");
    memset(&message, 0, sizeof(NGAP_PathSwitchRequestAcknowledgeTransfer_t));

#if 0 /* The following is optional. So I've removed */

    message.uL_NGU_UP_TNLInformation = CALLOC(1, sizeof(NGAP_UPTransportLayerInformation_t));
    UPTransportLayerInformation = message.uL_NGU_UP_TNLInformation;

    gTPTunnel = CALLOC(1, sizeof(struct NGAP_GTPTunnel));
    UPTransportLayerInformation->present =
        NGAP_UPTransportLayerInformation_PR_gTPTunnel;
    UPTransportLayerInformation->choice.gTPTunnel = gTPTunnel;

    ogs_assert(OGS_OK == ogs_sockaddr_to_ip(
                sess->upf_n3_addr, sess->upf_n3_addr6, &upf_n3_ip));
    ogs_asn_ip_to_BIT_STRING(&upf_n3_ip, &gTPTunnel->transportLayerAddress);
    ogs_asn_uint32_to_OCTET_STRING(sess->upf_n3_teid, &gTPTunnel->gTP_TEID);

#endif

    return ogs_asn_encode(
            &asn_DEF_NGAP_PathSwitchRequestAcknowledgeTransfer, &message);
}

ogs_pkbuf_t *ngap_build_handover_command_transfer(smf_sess_t *sess)
{
    NGAP_HandoverCommandTransfer_t message;

    ogs_ip_t upf_dl_ip;

    ogs_assert(sess);

    ogs_debug("HandoverCommandTransfer");
    memset(&message, 0, sizeof(NGAP_HandoverCommandTransfer_t));

    if (sess->handover.indirect_data_forwarding == true) {
        ogs_pfcp_pdr_t *pdr = NULL;

        NGAP_UPTransportLayerInformation_t
            *dLForwardingUP_TNLInformation = NULL;
        NGAP_GTPTunnel_t *gTPTunnel = NULL;
        NGAP_QosFlowToBeForwardedList_t *qosFlowToBeForwardedList = NULL;

        message.dLForwardingUP_TNLInformation = dLForwardingUP_TNLInformation =
            CALLOC(1, sizeof(*dLForwardingUP_TNLInformation));
        ogs_assert(dLForwardingUP_TNLInformation);

        dLForwardingUP_TNLInformation->present =
            NGAP_UPTransportLayerInformation_PR_gTPTunnel;
        dLForwardingUP_TNLInformation->choice.gTPTunnel = gTPTunnel =
            CALLOC(1, sizeof(*gTPTunnel));
        ogs_assert(gTPTunnel);

        ogs_assert(OGS_OK == ogs_sockaddr_to_ip(
                sess->handover.upf_dl_addr, sess->handover.upf_dl_addr6,
                &upf_dl_ip));
        ogs_asn_ip_to_BIT_STRING(&upf_dl_ip, &gTPTunnel->transportLayerAddress);
        ogs_asn_uint32_to_OCTET_STRING(
                sess->handover.upf_dl_teid, &gTPTunnel->gTP_TEID);

        ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
            ogs_pfcp_far_t *far = pdr->far;
            ogs_assert(far);

            if (pdr->src_if == OGS_PFCP_INTERFACE_ACCESS &&
                far->dst_if == OGS_PFCP_INTERFACE_ACCESS) {
                NGAP_QosFlowToBeForwardedItem_t *qosFlowToBeForwardedItem;
                NGAP_QosFlowIdentifier_t *qosFlowIdentifier = NULL;

                if (!qosFlowToBeForwardedList) {
                    message.qosFlowToBeForwardedList =
                        qosFlowToBeForwardedList =
                            CALLOC(1, sizeof(*qosFlowToBeForwardedList));
                    ogs_assert(qosFlowToBeForwardedList);
                }

                qosFlowToBeForwardedItem =
                    CALLOC(1, sizeof(*qosFlowToBeForwardedItem));
                ogs_assert(qosFlowToBeForwardedItem);

                ASN_SEQUENCE_ADD(&qosFlowToBeForwardedList->list,
                        qosFlowToBeForwardedItem);

                qosFlowIdentifier =
                    &qosFlowToBeForwardedItem->qosFlowIdentifier;

                *qosFlowIdentifier = pdr->qfi;
            }
        }
    }

    return ogs_asn_encode(&asn_DEF_NGAP_HandoverCommandTransfer, &message);
}
