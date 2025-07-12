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

/**
 * Fill common QoS flow level parameters: 5QI, ARP, and optional GBR/MBR.
 */
static void fill_qos_level_parameters(
    NGAP_QosFlowLevelQosParameters_t *params,
    const ogs_qos_t *qos,
    bool include_gbr)
{
    NGAP_AllocationAndRetentionPriority_t
        *allocationAndRetentionPriority = NULL;
    NGAP_QosCharacteristics_t *qosCharacteristics = NULL;
    NGAP_NonDynamic5QIDescriptor_t *nonDynamic5QI = NULL;

    /* Allocation and Retention Priority */
    allocationAndRetentionPriority =
        &params->allocationAndRetentionPriority;

    allocationAndRetentionPriority->priorityLevelARP = qos->arp.priority_level;
    if (qos->arp.pre_emption_capability == OGS_5GC_PRE_EMPTION_ENABLED)
        allocationAndRetentionPriority->pre_emptionCapability =
            NGAP_Pre_emptionCapability_may_trigger_pre_emption;
    if (qos->arp.pre_emption_vulnerability == OGS_5GC_PRE_EMPTION_ENABLED)
        allocationAndRetentionPriority->pre_emptionVulnerability =
            NGAP_Pre_emptionVulnerability_pre_emptable;

    /* Non-Dynamic 5QI Descriptor */
    qosCharacteristics = &params->qosCharacteristics;
    qosCharacteristics->choice.nonDynamic5QI = nonDynamic5QI =
        CALLOC(1, sizeof(struct NGAP_NonDynamic5QIDescriptor));
    ogs_assert(nonDynamic5QI);
    qosCharacteristics->present = NGAP_QosCharacteristics_PR_nonDynamic5QI;

    nonDynamic5QI->fiveQI = qos->index;

    /* Optional GBR/MBR Information */
    if (include_gbr &&
        qos->mbr.downlink && qos->mbr.uplink &&
        qos->gbr.downlink && qos->gbr.uplink) {
        NGAP_GBR_QosInformation_t *gBR_QosInformation =
            params->gBR_QosInformation = CALLOC(1, sizeof(*gBR_QosInformation));
        ogs_assert(gBR_QosInformation);

        asn_uint642INTEGER(&gBR_QosInformation->maximumFlowBitRateDL,
                qos->mbr.downlink);
        asn_uint642INTEGER(&gBR_QosInformation->maximumFlowBitRateUL,
                qos->mbr.uplink);
        asn_uint642INTEGER(&gBR_QosInformation->
                guaranteedFlowBitRateDL, qos->gbr.downlink);
        asn_uint642INTEGER(&gBR_QosInformation->
                guaranteedFlowBitRateUL, qos->gbr.uplink);
    } else if (include_gbr &&
               (qos->mbr.downlink || qos->mbr.uplink ||
                qos->gbr.downlink || qos->gbr.uplink)) {
        ogs_error("Missing one or more MBR/GBR parameters; "
                "defaulting to Non-GBR flow ");
        ogs_error("    MBR[DL:%lld,UL:%lld]",
            (long long)qos->mbr.downlink, (long long)qos->mbr.uplink);
        ogs_error("    GBR[DL:%lld,UL:%lld]",
            (long long)qos->gbr.downlink, (long long)qos->gbr.uplink);
    }
}

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
    NGAP_SecurityIndication_t *SecurityIndication = NULL;
    NGAP_QosFlowSetupRequestList_t *QosFlowSetupRequestList = NULL;
    NGAP_QosFlowSetupRequestItem_t *QosFlowSetupRequestItem = NULL;

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
                sess->local_ul_addr, sess->local_ul_addr6, &upf_n3_ip));
    ogs_assert(OGS_OK == ogs_asn_ip_to_BIT_STRING(
                &upf_n3_ip, &gTPTunnel->transportLayerAddress));
    ogs_asn_uint32_to_OCTET_STRING(sess->local_ul_teid, &gTPTunnel->gTP_TEID);

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

    if (smf_self()->security_indication.integrity_protection_indication &&
        smf_self()->security_indication.confidentiality_protection_indication) {

        ie = CALLOC(1,
                sizeof(NGAP_PDUSessionResourceSetupRequestTransferIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_SecurityIndication;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present = NGAP_PDUSessionResourceSetupRequestTransferIEs__value_PR_SecurityIndication;

        SecurityIndication = &ie->value.choice.SecurityIndication;

        SecurityIndication->integrityProtectionIndication =
                smf_integrity_protection_indication_value2enum(
                    smf_self()->security_indication.
                        integrity_protection_indication);
        ogs_assert(SecurityIndication->integrityProtectionIndication >= 0);

        SecurityIndication->confidentialityProtectionIndication =
                smf_confidentiality_protection_indication_value2enum(
                    smf_self()->security_indication.
                        confidentiality_protection_indication);
        ogs_assert(SecurityIndication->
                confidentialityProtectionIndication >= 0);

        if (smf_self()->security_indication.
                maximum_integrity_protected_data_rate_uplink) {

            ogs_assert(
                SecurityIndication->integrityProtectionIndication ==
                    NGAP_IntegrityProtectionIndication_required ||
                SecurityIndication->integrityProtectionIndication ==
                    NGAP_IntegrityProtectionIndication_preferred);

            SecurityIndication->maximumIntegrityProtectedDataRate_UL =
                CALLOC(1, sizeof(NGAP_MaximumIntegrityProtectedDataRate_t));
            ogs_assert(SecurityIndication->
                    maximumIntegrityProtectedDataRate_UL);
            *(SecurityIndication->maximumIntegrityProtectedDataRate_UL) =
                smf_maximum_integrity_protected_data_rate_uplink_value2enum(
                    smf_self()->security_indication.
                        maximum_integrity_protected_data_rate_uplink);
            ogs_assert(
                *(SecurityIndication->
                    maximumIntegrityProtectedDataRate_UL) >= 0);

            if (smf_self()->security_indication.
                    maximum_integrity_protected_data_rate_downlink) {
                NGAP_ProtocolExtensionContainer_11905P297_t
                    *extContainer = NULL;
                NGAP_SecurityIndication_ExtIEs_t *extIe = NULL;
                NGAP_MaximumIntegrityProtectedDataRate_t
                    *MaximumIntegrityProtectedDataRate = NULL;

                extContainer = CALLOC(1,
                        sizeof(NGAP_ProtocolExtensionContainer_11905P297_t));
                ogs_assert(extContainer);
                SecurityIndication->iE_Extensions =
                    (struct NGAP_ProtocolExtensionContainer *)extContainer;

                extIe = CALLOC(1, sizeof(NGAP_SecurityIndication_ExtIEs_t));
                ogs_assert(extIe);
                ASN_SEQUENCE_ADD(&extContainer->list, extIe);

                extIe->id =
                    NGAP_ProtocolIE_ID_id_MaximumIntegrityProtectedDataRate_DL;
                extIe->criticality = NGAP_Criticality_ignore;
                extIe->extensionValue.present = NGAP_SecurityIndication_ExtIEs__extensionValue_PR_MaximumIntegrityProtectedDataRate;

                MaximumIntegrityProtectedDataRate =
                    &extIe->extensionValue.choice.
                        MaximumIntegrityProtectedDataRate;

                *MaximumIntegrityProtectedDataRate =
                smf_maximum_integrity_protected_data_rate_downlink_value2enum(
                        smf_self()->security_indication.
                            maximum_integrity_protected_data_rate_downlink);
                ogs_assert(*MaximumIntegrityProtectedDataRate >= 0);
            }
        }
    }

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_QosFlowSetupRequestList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceSetupRequestTransferIEs__value_PR_QosFlowSetupRequestList;

    QosFlowSetupRequestList = &ie->value.choice.QosFlowSetupRequestList;

    if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
        OpenAPI_list_t *qosFlowsSetupList = NULL;
        OpenAPI_qos_flow_setup_item_t *qosFlowSetupItem = NULL;
        OpenAPI_qos_flow_profile_t *qosFlowProfile = NULL;
        OpenAPI_lnode_t *node = NULL;

        ogs_qos_t qos;

        qosFlowsSetupList = sess->h_smf_qos_flows_setup_list;
        ogs_assert(qosFlowsSetupList);

        node = qosFlowsSetupList->first;
        ogs_assert(node);
        qosFlowSetupItem = node->data;
        ogs_assert(qosFlowSetupItem);

        qosFlowProfile = qosFlowSetupItem->qos_flow_profile;
        ogs_assert(qosFlowProfile);

        memset(&qos, 0, sizeof(qos));

        qos.index = qosFlowProfile->_5qi;
        ogs_assert(qosFlowProfile->arp);
        qos.arp.priority_level = qosFlowProfile->arp->priority_level;
        if (qosFlowProfile->arp->preempt_cap ==
            OpenAPI_preemption_capability_NOT_PREEMPT)
            qos.arp.pre_emption_capability = OGS_5GC_PRE_EMPTION_DISABLED;
        else if (qosFlowProfile->arp->preempt_cap ==
            OpenAPI_preemption_capability_MAY_PREEMPT)
            qos.arp.pre_emption_capability = OGS_5GC_PRE_EMPTION_ENABLED;
        else {
            ogs_error("Invalid preempt_cap [%d]",
                    qosFlowProfile->arp->preempt_cap);
            ogs_assert_if_reached();
        }

        if (qosFlowProfile->arp->preempt_vuln ==
            OpenAPI_preemption_vulnerability_NOT_PREEMPTABLE)
            qos.arp.pre_emption_vulnerability = OGS_5GC_PRE_EMPTION_DISABLED;
        else if (qosFlowProfile->arp->preempt_vuln ==
            OpenAPI_preemption_vulnerability_PREEMPTABLE)
            qos.arp.pre_emption_vulnerability = OGS_5GC_PRE_EMPTION_ENABLED;
        else {
            ogs_error("Invalid preempt_vuln [%d]",
                    qosFlowProfile->arp->preempt_vuln);
            ogs_assert_if_reached();
        }

        QosFlowSetupRequestItem =
            CALLOC(1, sizeof(struct NGAP_QosFlowSetupRequestItem));
        ogs_assert(QosFlowSetupRequestItem);
        ASN_SEQUENCE_ADD(&QosFlowSetupRequestList->list,
            QosFlowSetupRequestItem);

        QosFlowSetupRequestItem->qosFlowIdentifier = qosFlowSetupItem->qfi;

        fill_qos_level_parameters(
                &QosFlowSetupRequestItem->qosFlowLevelQosParameters,
                &qos, true);

    } else {
        ogs_list_for_each(&sess->bearer_list, qos_flow) {
            QosFlowSetupRequestItem =
                CALLOC(1, sizeof(struct NGAP_QosFlowSetupRequestItem));
            ogs_assert(QosFlowSetupRequestItem);
            ASN_SEQUENCE_ADD(&QosFlowSetupRequestList->list,
                QosFlowSetupRequestItem);

            QosFlowSetupRequestItem->qosFlowIdentifier = qos_flow->qfi;

            fill_qos_level_parameters(
                    &QosFlowSetupRequestItem->qosFlowLevelQosParameters,
                    &qos_flow->qos, true);
        }
    }

    return ogs_asn_encode(
            &asn_DEF_NGAP_PDUSessionResourceSetupRequestTransfer, &message);
}

ogs_pkbuf_t *ngap_build_pdu_session_resource_modify_request_transfer(
        smf_sess_t *sess, bool include_gbr)
{
    NGAP_PDUSessionResourceModifyRequestTransfer_t message;

    NGAP_PDUSessionResourceModifyRequestTransferIEs_t *ie = NULL;

    NGAP_QosFlowAddOrModifyRequestList_t *QosFlowAddOrModifyRequestList = NULL;
    NGAP_QosFlowAddOrModifyRequestItem_t *QosFlowAddOrModifyRequestItem = NULL;

    smf_bearer_t *qos_flow = NULL;

    ogs_assert(sess);

    ogs_debug("PDUSessionResourceModifyRequestTransfer");
    memset(&message, 0, sizeof(NGAP_PDUSessionResourceModifyRequestTransfer_t));

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyRequestTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&message.protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_QosFlowAddOrModifyRequestList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceModifyRequestTransferIEs__value_PR_QosFlowAddOrModifyRequestList;

    QosFlowAddOrModifyRequestList =
        &ie->value.choice.QosFlowAddOrModifyRequestList;

    /* Home-Routed V-SMF: QoS flow */
    if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
        OpenAPI_lnode_t *node = NULL;
        OpenAPI_list_for_each(
                sess->h_smf_qos_flows_add_mod_request_list, node) {
            OpenAPI_qos_flow_add_modify_request_item_t
                *qosFlowAddModRequestItem = node->data;
            if (qosFlowAddModRequestItem) {
                OpenAPI_qos_flow_profile_t *qosFlowProfile =
                    qosFlowAddModRequestItem->qos_flow_profile;
                if (qosFlowProfile) {
                    ogs_qos_t qos;

                    memset(&qos, 0, sizeof(qos));

                    qos.index = qosFlowProfile->_5qi;
                    ogs_assert(qosFlowProfile->arp);
                    qos.arp.priority_level =
                        qosFlowProfile->arp->priority_level;
                    if (qosFlowProfile->arp->preempt_cap ==
                        OpenAPI_preemption_capability_NOT_PREEMPT)
                        qos.arp.pre_emption_capability =
                            OGS_5GC_PRE_EMPTION_DISABLED;
                    else if (qosFlowProfile->arp->preempt_cap ==
                        OpenAPI_preemption_capability_MAY_PREEMPT)
                        qos.arp.pre_emption_capability =
                            OGS_5GC_PRE_EMPTION_ENABLED;
                    else {
                        ogs_error("Invalid preempt_cap [%d]",
                                qosFlowProfile->arp->preempt_cap);
                        ogs_assert_if_reached();
                    }

                    if (qosFlowProfile->arp->preempt_vuln ==
                        OpenAPI_preemption_vulnerability_NOT_PREEMPTABLE)
                        qos.arp.pre_emption_vulnerability =
                            OGS_5GC_PRE_EMPTION_DISABLED;
                    else if (qosFlowProfile->arp->preempt_vuln ==
                        OpenAPI_preemption_vulnerability_PREEMPTABLE)
                        qos.arp.pre_emption_vulnerability =
                            OGS_5GC_PRE_EMPTION_ENABLED;
                    else {
                        ogs_error("Invalid preempt_vuln [%d]",
                                qosFlowProfile->arp->preempt_vuln);
                        ogs_assert_if_reached();
                    }

                    if (qosFlowProfile->gbr_qos_flow_info) {
                        OpenAPI_gbr_qos_flow_information_t *gbrQosFlowInfo =
                                qosFlowProfile->gbr_qos_flow_info;
                        if (gbrQosFlowInfo->max_fbr_dl)
                            qos.mbr.downlink =
                                ogs_sbi_bitrate_from_string(
                                        gbrQosFlowInfo->max_fbr_dl);
                        if (gbrQosFlowInfo->max_fbr_ul)
                            qos.mbr.uplink =
                                ogs_sbi_bitrate_from_string(
                                        gbrQosFlowInfo->max_fbr_ul);
                        if (gbrQosFlowInfo->gua_fbr_dl)
                            qos.gbr.downlink =
                                ogs_sbi_bitrate_from_string(
                                        gbrQosFlowInfo->gua_fbr_dl);
                        if (gbrQosFlowInfo->gua_fbr_ul)
                            qos.gbr.uplink =
                                ogs_sbi_bitrate_from_string(
                                        gbrQosFlowInfo->gua_fbr_ul);
                    }

                    QosFlowAddOrModifyRequestItem =
                        CALLOC(1, sizeof(*QosFlowAddOrModifyRequestItem));
                    ogs_assert(QosFlowAddOrModifyRequestItem);
                    ASN_SEQUENCE_ADD(
                            &QosFlowAddOrModifyRequestList->list,
                            QosFlowAddOrModifyRequestItem);

                    QosFlowAddOrModifyRequestItem->qosFlowIdentifier =
                        qosFlowAddModRequestItem->qfi;

                    QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters =
                            CALLOC(1, sizeof(NGAP_QosFlowLevelQosParameters_t));
                    ogs_assert(QosFlowAddOrModifyRequestItem->
                            qosFlowLevelQosParameters);

                    fill_qos_level_parameters(
                            QosFlowAddOrModifyRequestItem->
                                qosFlowLevelQosParameters, &qos, true);
                }
            }
        }

    } else {
        /* Default: iterate modify-list */
        ogs_list_for_each_entry(&sess->qos_flow_to_modify_list,
                                 qos_flow, to_modify_node) {
            QosFlowAddOrModifyRequestItem =
                CALLOC(1, sizeof(*QosFlowAddOrModifyRequestItem));
            ogs_assert(QosFlowAddOrModifyRequestItem);
            ASN_SEQUENCE_ADD(
                    &QosFlowAddOrModifyRequestList->list,
                    QosFlowAddOrModifyRequestItem);
            QosFlowAddOrModifyRequestItem->qosFlowIdentifier = qos_flow->qfi;

            QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters =
                    CALLOC(1, sizeof(NGAP_QosFlowLevelQosParameters_t));
            ogs_assert(
                    QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters);
            fill_qos_level_parameters(
                    QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters,
                    &qos_flow->qos, include_gbr);
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

    /* Home-Routed V-SMF: QoS flow */
    if (HOME_ROUTED_ROAMING_IN_VSMF(sess)) {
        OpenAPI_lnode_t *node = NULL;
        OpenAPI_list_for_each(sess->h_smf_qos_flows_rel_request_list, node) {
            OpenAPI_qos_flow_release_request_item_t
                *qosFlowRelRequestItem = node->data;
            if (qosFlowRelRequestItem) {

                QosFlowWithCauseItem = CALLOC(1, sizeof(*QosFlowWithCauseItem));
                ASN_SEQUENCE_ADD(&QosFlowListWithCause->list,
                        QosFlowWithCauseItem);

                qosFlowIdentifier = &QosFlowWithCauseItem->qosFlowIdentifier;

                *qosFlowIdentifier = qosFlowRelRequestItem->qfi;

                Cause = &QosFlowWithCauseItem->cause;
                Cause->present = group;
                Cause->choice.radioNetwork = cause;
            }
        }
    } else {
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
                sess->local_ul_addr, sess->local_ul_addr6, &upf_n3_ip));
    ogs_assert(OGS_OK == ogs_asn_ip_to_BIT_STRING(
                &upf_n3_ip, &gTPTunnel->transportLayerAddress));
    ogs_asn_uint32_to_OCTET_STRING(sess->local_ul_teid, &gTPTunnel->gTP_TEID);

#endif

    return ogs_asn_encode(
            &asn_DEF_NGAP_PathSwitchRequestAcknowledgeTransfer, &message);
}

ogs_pkbuf_t *ngap_build_handover_command_transfer(smf_sess_t *sess)
{
    NGAP_HandoverCommandTransfer_t message;

    ogs_ip_t local_dl_ip;

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
                sess->handover.local_dl_addr, sess->handover.local_dl_addr6,
                &local_dl_ip));
        ogs_assert(OGS_OK == ogs_asn_ip_to_BIT_STRING(
                    &local_dl_ip, &gTPTunnel->transportLayerAddress));
        ogs_asn_uint32_to_OCTET_STRING(
                sess->handover.local_dl_teid, &gTPTunnel->gTP_TEID);

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
