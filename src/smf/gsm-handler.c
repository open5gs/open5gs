/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "gsm-handler.h"
#include "sbi-path.h"
#include "pfcp-path.h"

#include "ipfw/ipfw2.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __gsm_log_domain

int gsm_handle_pdu_session_establishment_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        ogs_nas_5gs_pdu_session_establishment_request_t *
            pdu_session_establishment_request)
{
    ogs_nas_integrity_protection_maximum_data_rate_t *
        integrity_protection_maximum_data_rate =
            &pdu_session_establishment_request->
                integrity_protection_maximum_data_rate;
    ogs_nas_pdu_session_type_t *pdu_session_type = NULL;
    ogs_nas_ssc_mode_t *ssc_mode = NULL;
    int r;

    ogs_assert(sess);
    ogs_assert(stream);
    ogs_assert(pdu_session_establishment_request);

    pdu_session_type = &pdu_session_establishment_request->pdu_session_type;
    ogs_assert(pdu_session_type);
    ssc_mode = &pdu_session_establishment_request->ssc_mode;

    switch (integrity_protection_maximum_data_rate->dl) {
    case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_64KBPS:
        sess->integrity_protection.mbr_dl =
            OpenAPI_max_integrity_protected_data_rate__64_KBPS;
        break;
    case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_FULL:
        sess->integrity_protection.mbr_dl =
            OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_FULL;
        break;
    default:
        sess->integrity_protection.mbr_dl =
            OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_NULL;
        break;
    }
    switch (integrity_protection_maximum_data_rate->ul) {
    case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_64KBPS:
        sess->integrity_protection.mbr_ul =
            OpenAPI_max_integrity_protected_data_rate__64_KBPS;
        break;
    case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_FULL:
        sess->integrity_protection.mbr_ul =
            OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_FULL;
        break;
    default:
        sess->integrity_protection.mbr_ul =
            OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_NULL;
        break;
    }

    if (pdu_session_establishment_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_PRESENT)
        sess->ue_session_type = pdu_session_type->value;

    if (pdu_session_establishment_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_PRESENT)
        sess->ue_ssc_mode = ssc_mode->value;

    if (pdu_session_establishment_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        OGS_NAS_STORE_DATA(&sess->nas.ue_epco,
            &pdu_session_establishment_request->
                extended_protocol_configuration_options);
    }

    r = smf_sbi_discover_and_send(OpenAPI_service_name_nudm_sdm, NULL,
            smf_nudm_sdm_build_get,
            sess, stream, 0, (char *)OGS_SBI_RESOURCE_NAME_SM_DATA);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return OGS_OK;
}

static int reconfigure_packet_filter(
        smf_pf_t *pf, ogs_nas_qos_rule_t *qos_rule, int i)
{
    int j;

    ogs_assert(pf);
    ogs_assert(qos_rule);

    memset(&pf->ipfw_rule, 0, sizeof(ogs_ipfw_rule_t));
    pf->direction = qos_rule->pf[i].direction;
    for (j = 0; j < qos_rule->pf[i].content.num_of_component; j++) {
        switch(qos_rule->pf[i].content.component[j].type) {
        case OGS_PACKET_FILTER_MATCH_ALL:
            break;
        case OGS_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE:
            pf->ipfw_rule.proto = qos_rule->pf[i].content.component[j].proto;
            break;
        case OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE:
            pf->ipfw_rule.ipv4_dst = 1;
            pf->ipfw_rule.ip.dst.addr[0] =
                qos_rule->pf[i].content.component[j].ipv4.addr;
            pf->ipfw_rule.ip.dst.mask[0] =
                qos_rule->pf[i].content.component[j].ipv4.mask;
            break;
        case OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE:
            pf->ipfw_rule.ipv4_src = 1;
            pf->ipfw_rule.ip.src.addr[0] =
                qos_rule->pf[i].content.component[j].ipv4.addr;
            pf->ipfw_rule.ip.src.mask[0] =
                qos_rule->pf[i].content.component[j].ipv4.mask;
            break;
        case OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_TYPE:
            pf->ipfw_rule.ipv6_dst = 1;
            memcpy(pf->ipfw_rule.ip.dst.addr,
                qos_rule->pf[i].content.component[j].ipv6_mask.addr,
                sizeof(pf->ipfw_rule.ip.dst.addr));
            memcpy(pf->ipfw_rule.ip.dst.mask,
                qos_rule->pf[i].content.component[j].ipv6_mask.mask,
                sizeof(pf->ipfw_rule.ip.dst.mask));
            break;
        case OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE:
            pf->ipfw_rule.ipv6_dst = 1;
            memcpy(pf->ipfw_rule.ip.dst.addr,
                qos_rule->pf[i].content.component[j].ipv6_mask.addr,
                sizeof(pf->ipfw_rule.ip.dst.addr));
            n2mask((struct in6_addr *)pf->ipfw_rule.ip.dst.mask,
                qos_rule->pf[i].content.component[j].ipv6.prefixlen);
            break;
        case OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_TYPE:
            pf->ipfw_rule.ipv6_src = 1;
            memcpy(pf->ipfw_rule.ip.src.addr,
                qos_rule->pf[i].content.component[j].ipv6_mask.addr,
                sizeof(pf->ipfw_rule.ip.src.addr));
            memcpy(pf->ipfw_rule.ip.src.mask,
                qos_rule->pf[i].content.component[j].ipv6_mask.mask,
                sizeof(pf->ipfw_rule.ip.src.mask));
            break;
        case OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE:
            pf->ipfw_rule.ipv6_src = 1;
            memcpy(pf->ipfw_rule.ip.src.addr,
                qos_rule->pf[i].content.component[j].ipv6_mask.addr,
                sizeof(pf->ipfw_rule.ip.src.addr));
            n2mask((struct in6_addr *)pf->ipfw_rule.ip.src.mask,
                qos_rule->pf[i].content.component[j].ipv6.prefixlen);
            break;
        case OGS_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE:
            pf->ipfw_rule.port.src.low = pf->ipfw_rule.port.src.high =
                    qos_rule->pf[i].content.component[j].port.low;
            break;
        case OGS_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE:
            pf->ipfw_rule.port.dst.low = pf->ipfw_rule.port.dst.high =
                    qos_rule->pf[i].content.component[j].port.low;
            break;
        case OGS_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE:
            pf->ipfw_rule.port.src.low =
                qos_rule->pf[i].content.component[j].port.low;
            pf->ipfw_rule.port.src.high =
                qos_rule->pf[i].content.component[j].port.high;
            break;
        case OGS_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE:
            pf->ipfw_rule.port.dst.low =
                qos_rule->pf[i].content.component[j].port.low;
            pf->ipfw_rule.port.dst.high =
                qos_rule->pf[i].content.component[j].port.high;
            break;
        default:
            ogs_error("Unknown Packet Filter Type(%d)",
                    qos_rule->pf[i].content.component[j].type);
            return OGS_ERROR;
        }
    }

    return j;
}

#define qos_flow_find_or_add(list, node, member)        \
    do {                                                \
        smf_bearer_t *iter = NULL;                      \
        bool found = false;                             \
                                                        \
        ogs_assert(node);                               \
                                                        \
        ogs_list_for_each_entry(list, iter, member) {   \
            if (iter->qfi == node->qfi) {               \
                found = true;                           \
                break;                                  \
            }                                           \
        }                                               \
        if (found == false) {                           \
            ogs_list_add(list, &node->member);          \
        }                                               \
    } while(0);

int gsm_handle_pdu_session_modification_qos_rules(
        smf_sess_t *sess,
        ogs_nas_qos_rules_t *requested_qos_rules,
        uint64_t *pfcp_flags)
{
    smf_ue_t *smf_ue = NULL;
    int i, j, num_of_rule = 0;

    ogs_nas_qos_rule_t qos_rule[OGS_NAS_MAX_NUM_OF_QOS_RULE];

    ogs_assert(requested_qos_rules);
    ogs_assert(pfcp_flags);

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    num_of_rule = ogs_nas_parse_qos_rules(qos_rule, requested_qos_rules);
    if (!num_of_rule) {
        ogs_error("[%s:%d] Invalid modification request",
                smf_ue->supi, sess->psi);
        return OGS_ERROR;
    }

    for (i = 0; i < num_of_rule; i++) {
        smf_pf_t *pf = NULL;
        smf_bearer_t *qos_flow =
            smf_qos_flow_find_by_qfi(sess, qos_rule[i].identifier);

        ogs_info("[%s:%d] Requested QoS rule "
                "[code:%d id:%d num_of_packet_filter:%d]",
                smf_ue->supi, sess->psi,
                qos_rule[i].code, qos_rule[i].identifier,
                qos_rule[i].num_of_packet_filter);

        if (!qos_flow) {
            ogs_error("[%s:%d] No QoS flow [QRI/QFI:%d]",
                    smf_ue->supi, sess->psi, qos_rule[i].identifier);
            continue;
        }

        ogs_list_init(&qos_flow->pf_to_add_list);

        if (qos_rule[i].code == OGS_NAS_QOS_CODE_DELETE_EXISTING_QOS_RULE) {
            smf_pf_remove_all(qos_flow);

            *pfcp_flags |= OGS_PFCP_MODIFY_REMOVE;
            qos_flow_find_or_add(&sess->qos_flow_to_modify_list,
                                    qos_flow, to_modify_node);
        } else if (qos_rule[i].code ==
            OGS_NAS_QOS_CODE_CREATE_NEW_QOS_RULE ||
                    qos_rule[i].code ==
            OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS ||
                    qos_rule[i].code ==
            OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_REPLACE_ALL_PACKET_FILTERS) {

            if (qos_rule[i].code == OGS_NAS_QOS_CODE_CREATE_NEW_QOS_RULE ||
                qos_rule[i].code == OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_REPLACE_ALL_PACKET_FILTERS)
                smf_pf_remove_all(qos_flow);

            for (j = 0; j < qos_rule[i].num_of_packet_filter &&
                        j < OGS_MAX_NUM_OF_FLOW_IN_NAS; j++) {

                pf = smf_pf_add(qos_flow);
                ogs_assert(pf);

                if (reconfigure_packet_filter(pf, &qos_rule[i], j) <= 0) {
                    ogs_error("[%s:%d] Invalid packet filter",
                            smf_ue->supi, sess->psi);
                    smf_pf_remove(pf);
                    return OGS_ERROR;
                }
/*
 * Refer to lib/ipfw/ogs-ipfw.h
 * Issue #338
 *
 * <DOWNLINK/BI-DIRECTIONAL>
 * TFT : Local <UE_IP> <UE_PORT> REMOTE <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 * -->
 * RULE : Source <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> Destination <UE_IP> <UE_PORT>
 *
 * <UPLINK>
 * TFT : Local <UE_IP> <UE_PORT> REMOTE <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 * -->
 * RULE : Source <UE_IP> <UE_PORT> Destination <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 */
                if (pf->direction == OGS_FLOW_DOWNLINK_ONLY)
                    ogs_ipfw_rule_swap(&pf->ipfw_rule);

                if (pf->flow_description)
                    ogs_free(pf->flow_description);

/*
 * Issue #338
 *
 * <DOWNLINK/BI-DIRECTIONAL>
 * RULE : Source <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> Destination <UE_IP> <UE_PORT>
 * -->
 * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * PFCP : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 *
 * <UPLINK>
 * RULE : Source <UE_IP> <UE_PORT> Destination <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 * -->
 * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * PFCP : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 */
                if (pf->direction == OGS_FLOW_UPLINK_ONLY) {
                    ogs_ipfw_rule_t tmp;
                    ogs_ipfw_copy_and_swap(&tmp, &pf->ipfw_rule);
                    pf->flow_description =
                        ogs_ipfw_encode_flow_description(&tmp);
                    ogs_assert(pf->flow_description);
                } else {
                    pf->flow_description =
                        ogs_ipfw_encode_flow_description(&pf->ipfw_rule);
                    ogs_assert(pf->flow_description);
                }

                if (qos_rule[i].code ==
                        OGS_NAS_QOS_CODE_CREATE_NEW_QOS_RULE) {
                    *pfcp_flags |= OGS_PFCP_MODIFY_TFT_NEW;
                } else if (qos_rule[i].code ==
                        OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS) {
                    *pfcp_flags |= OGS_PFCP_MODIFY_TFT_ADD;
                } else if (qos_rule[i].code ==
                        OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_REPLACE_ALL_PACKET_FILTERS) {
                    *pfcp_flags |= OGS_PFCP_MODIFY_TFT_REPLACE;
                } else
                    ogs_assert_if_reached();

                qos_flow_find_or_add(&sess->qos_flow_to_modify_list,
                                        qos_flow, to_modify_node);

                ogs_list_add(
                        &qos_flow->pf_to_add_list, &pf->to_add_node);
            }
        } else if (qos_rule[i].code ==
            OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_DELETE_PACKET_FILTERS) {

            qos_flow->num_of_pf_to_delete = 0;
            for (j = 0; j < qos_rule[i].num_of_packet_filter &&
                        j < OGS_MAX_NUM_OF_FLOW_IN_NAS; j++) {

                pf = smf_pf_find_by_identifier(
                        qos_flow, qos_rule[i].pf[j].identifier);
                if (pf) {
                    qos_flow->pf_to_delete
                        [qos_flow->num_of_pf_to_delete++] =
                            qos_rule[i].pf[j].identifier;
                    smf_pf_remove(pf);
                }
            }

            if (ogs_list_count(&qos_flow->pf_list)) {
                *pfcp_flags |= OGS_PFCP_MODIFY_TFT_DELETE;
                qos_flow_find_or_add(&sess->qos_flow_to_modify_list,
                                        qos_flow, to_modify_node);
            } else {
                *pfcp_flags |= OGS_PFCP_MODIFY_REMOVE;
                qos_flow_find_or_add(&sess->qos_flow_to_modify_list,
                                        qos_flow, to_modify_node);
            }
        }

        if (*pfcp_flags &
                (OGS_PFCP_MODIFY_TFT_NEW|OGS_PFCP_MODIFY_TFT_ADD|
                OGS_PFCP_MODIFY_TFT_REPLACE|OGS_PFCP_MODIFY_TFT_DELETE))
            smf_bearer_tft_update(qos_flow);
    }

    return OGS_OK;
}

int gsm_handle_pdu_session_modification_qos_flow_descriptions(
        smf_sess_t *sess,
        ogs_nas_qos_flow_descriptions_t *requested_qos_flow_descriptions,
        uint64_t *pfcp_flags)
{
    smf_ue_t *smf_ue = NULL;
    int i, j, num_of_description = 0;

    ogs_nas_qos_flow_description_t
        qos_flow_description[OGS_NAS_MAX_NUM_OF_QOS_FLOW_DESCRIPTION];

    ogs_assert(requested_qos_flow_descriptions);
    ogs_assert(pfcp_flags);

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    num_of_description = ogs_nas_parse_qos_flow_descriptions(
            qos_flow_description, requested_qos_flow_descriptions);
    if (!num_of_description) {
        ogs_error("[%s:%d] Invalid modification request",
                smf_ue->supi, sess->psi);
        return OGS_ERROR;
    }

    for (i = 0; i < num_of_description; i++) {

        smf_bearer_t *qos_flow =
            smf_qos_flow_find_by_qfi(
                    sess, qos_flow_description[i].identifier);

        ogs_info("[%s:%d] Requested QoS flow description "
                "[code:%d qfi:%d E_bit:%d num_of_parameter:%d]",
                smf_ue->supi, sess->psi,
                qos_flow_description[i].code,
                qos_flow_description[i].identifier,
                qos_flow_description[i].E_bit,
                qos_flow_description[i].num_of_parameter);

        if (!qos_flow) {
            ogs_error("[%s:%d] No QoS flow [QFI:%d]",
                    smf_ue->supi, sess->psi,
                    qos_flow_description[i].identifier);
            continue;
        }

        if (qos_flow_description[i].code ==
                OGS_NAS_DELETE_NEW_QOS_FLOW_DESCRIPTION) {
            /*
             * Issue #4672: smfd aborted while tearing down a VoNR call.
             *
             * Root cause
             * ----------
             * The QoS rules IE (TS24.501 9.11.4.13) and the QoS flow
             * descriptions IE (9.11.4.12) are independent IEs, each with
             * its own operation codes. Deleting a QoS flow therefore has
             * three valid encodings:
             *   1) QoS rule delete only,
             *   2) QoS flow description delete only,
             *   3) QoS rule delete + QoS flow description delete, paired
             *      on the same QFI.
             * For a GBR QoS flow (e.g. VoNR voice, 5QI 1) form 3) is the
             * one conformant UEs use: per 9.11.4.13/9.11.4.12 leaving a
             * GBR QoS rule whose QFI has no associated QoS flow
             * description (or vice versa) is a syntactical error in the
             * QoS operation, so the rule and the flow description must be
             * deleted together.
             *
             * In Open5GS, removal of a QoS flow is driven solely by
             * OGS_PFCP_MODIFY_REMOVE, which only the QoS rules handler
             * sets (via smf_pf_remove_all() + adding the flow to
             * qos_flow_to_modify_list; the flow is finally freed by
             * smf_bearer_remove() in the N4 handler once REMOVE is set).
             * Previously this handler ignored the operation code and set
             * OGS_PFCP_MODIFY_QOS_MODIFY for every flow description. On
             * the paired delete (form 3) that left both REMOVE (from the
             * rule) and QOS_MODIFY (from this description) set for the
             * same flow, tripping the invariant asserted in
             * gsm_handle_pdu_session_modification_request()
             * (pfcp_flags 0x8080 = REMOVE | QOS_MODIFY) and aborting the
             * SMF on an otherwise standards-compliant request.
             *
             * Fix
             * ---
             * A "Delete existing QoS flow description" carries no
             * parameters (E bit = 0, number of parameters = 0), so there
             * is nothing to modify here. Skip it and let the paired QoS
             * rule delete drive the removal. This keeps forms 1) and 3)
             * working and no longer manufactures QOS_MODIFY out of a
             * delete.
             *
             * Note on form 2) (flow description delete without the paired
             * rule delete): with this skip, nothing is added to
             * qos_flow_to_modify_list, so the caller's count check
             * rejects the request. We deliberately do not treat a lone
             * flow description delete as a removal: removal in Open5GS is
             * keyed off the QoS rule, and for a GBR flow a lone flow
             * description delete is itself a syntactical error under
             * 9.11.4.12. If future requirements need form 2) to actually
             * remove the flow, this branch would instead set
             * OGS_PFCP_MODIFY_REMOVE and add the flow to the modify list.
             */
            continue;
        }

        for (j = 0; j < qos_flow_description[i].num_of_parameter; j++) {
            switch(qos_flow_description[i].param[j].identifier) {
            case OGS_NAX_QOS_FLOW_PARAMETER_ID_5QI:
                /* Nothing */
                break;
            case OGS_NAX_QOS_FLOW_PARAMETER_ID_GFBR_UPLINK:
                qos_flow->qos.gbr.uplink = ogs_nas_bitrate_to_uint64(
                        &qos_flow_description[i].param[j].br);
                break;
            case OGS_NAX_QOS_FLOW_PARAMETER_ID_GFBR_DOWNLINK:
                qos_flow->qos.gbr.downlink = ogs_nas_bitrate_to_uint64(
                        &qos_flow_description[i].param[j].br);
                break;
            case OGS_NAX_QOS_FLOW_PARAMETER_ID_MFBR_UPLINK:
                qos_flow->qos.mbr.uplink = ogs_nas_bitrate_to_uint64(
                        &qos_flow_description[i].param[j].br);
                break;
            case OGS_NAX_QOS_FLOW_PARAMETER_ID_MFBR_DOWNLINK:
                qos_flow->qos.mbr.downlink = ogs_nas_bitrate_to_uint64(
                        &qos_flow_description[i].param[j].br);
                break;
            default:
                ogs_error("Unknown qos_flow parameter identifier [%d]",
                        qos_flow_description[i].param[j].identifier);
                return OGS_ERROR;
            }
        }

        *pfcp_flags |= OGS_PFCP_MODIFY_QOS_MODIFY;
        qos_flow_find_or_add(&sess->qos_flow_to_modify_list,
                                qos_flow, to_modify_node);

        if (*pfcp_flags & OGS_PFCP_MODIFY_QOS_MODIFY)
            smf_bearer_qos_update(qos_flow);
    }

    return OGS_OK;
}

int gsm_handle_pdu_session_modification_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        ogs_nas_5gs_pdu_session_modification_request_t *
            pdu_session_modification_request)
{
    int rv;
    uint64_t pfcp_flags = 0;

    ogs_nas_qos_rules_t *requested_qos_rules =
        &pdu_session_modification_request->requested_qos_rules;
    ogs_nas_qos_flow_descriptions_t *requested_qos_flow_descriptions =
        &pdu_session_modification_request->requested_qos_flow_descriptions;

    smf_ue_t *smf_ue = NULL;
    ogs_pkbuf_t *n1smbuf = NULL;

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);
    ogs_assert(stream);
    ogs_assert(pdu_session_modification_request);

    ogs_list_init(&sess->qos_flow_to_modify_list);

    if (pdu_session_modification_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_PRESENT) {
        /* Nothing to do */
    }

    if (pdu_session_modification_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT) {
        rv = gsm_handle_pdu_session_modification_qos_rules(
                sess, requested_qos_rules, &pfcp_flags);
        if (rv != OGS_OK) {
            ogs_error("gsm_handle_pdu_session_modification_qos_rules() failed");
            goto cleanup;
        }
    }

    if (pdu_session_modification_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT) {
        rv = gsm_handle_pdu_session_modification_qos_flow_descriptions(
                sess, requested_qos_flow_descriptions, &pfcp_flags);
        if (rv != OGS_OK) {
            ogs_error("gsm_handle_pdu_session_modification_"
                    "qos_flow_descriptions() failed");
            goto cleanup;
        }
    }

    if (ogs_list_count(&sess->qos_flow_to_modify_list) != 1) {
        ogs_error("[%s:%d] Invalid modification request [modify:%d]",
                smf_ue->supi, sess->psi,
                ogs_list_count(&sess->qos_flow_to_modify_list));
        goto cleanup;
    }

    if (pfcp_flags & OGS_PFCP_MODIFY_REMOVE) {
        /*
         * Issue #4672
         *
         * pfcp_flags is derived from the content of the UE's
         * PDU Session Modification Request, so a misbehaving UE can
         * request a removal combined with a TFT/QoS modification for
         * the same QoS flow. This is invalid input, not an internal
         * error: log the exact flag combination and reject the
         * request instead of aborting the SMF.
         */
        if (pfcp_flags &
                (OGS_PFCP_MODIFY_TFT_NEW|OGS_PFCP_MODIFY_TFT_ADD|
                OGS_PFCP_MODIFY_TFT_REPLACE|OGS_PFCP_MODIFY_TFT_DELETE|
                OGS_PFCP_MODIFY_QOS_MODIFY)) {
            ogs_error("[%s:%d] Invalid modification request "
                    "[REMOVE combined with TFT/QOS-MODIFY "
                    "pfcp_flags:0x%llx]",
                    smf_ue->supi, sess->psi, (long long)pfcp_flags);
            ogs_assert_if_reached();
        }

    } else if (pfcp_flags &
                (OGS_PFCP_MODIFY_TFT_NEW|OGS_PFCP_MODIFY_TFT_ADD|
                OGS_PFCP_MODIFY_TFT_REPLACE|OGS_PFCP_MODIFY_TFT_DELETE|
                OGS_PFCP_MODIFY_QOS_MODIFY)) {

        /* TFT/QoS modification without removal */
        if (pfcp_flags & OGS_PFCP_MODIFY_REMOVE) {
            ogs_error("[%s:%d] Invalid modification request "
                    "[TFT/QOS-MODIFY combined with REMOVE "
                    "pfcp_flags:0x%llx]",
                    smf_ue->supi, sess->psi, (long long)pfcp_flags);
            ogs_assert_if_reached();
        }

    } else {
        ogs_error("[%s:%d] Invalid modification request "
                "[Unknown pfcp_flags:0x%llx]",
                smf_ue->supi, sess->psi, (long long)pfcp_flags);
        ogs_assert_if_reached();
    }

    ogs_assert(OGS_OK ==
            smf_5gc_pfcp_send_qos_flow_list_modification_request(
                sess, stream,
                OGS_PFCP_MODIFY_UE_REQUESTED|pfcp_flags, 0));

    return OGS_OK;

cleanup:
    n1smbuf = gsm_build_pdu_session_modification_reject(sess,
        OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
    ogs_assert(n1smbuf);

    smf_sbi_send_sm_context_update_error_n1_n2_message(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            n1smbuf, OpenAPI_n2_sm_info_type_NULL, NULL);

    return OGS_ERROR;
}
