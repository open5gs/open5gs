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

    ogs_assert(sess);
    ogs_assert(stream);
    ogs_assert(pdu_session_establishment_request);

    pdu_session_type = &pdu_session_establishment_request->pdu_session_type;
    ogs_assert(pdu_session_type);
    ssc_mode = &pdu_session_establishment_request->ssc_mode;

    sess->integrity_protection.mbr_dl =
            integrity_protection_maximum_data_rate->dl;
    sess->integrity_protection.mbr_ul =
            integrity_protection_maximum_data_rate->ul;

    if (pdu_session_establishment_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_PRESENT)
        sess->ue_session_type = pdu_session_type->value;

    if (pdu_session_establishment_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_PRESENT)
        sess->ue_ssc_mode = ssc_mode->value;

    if (pdu_session_establishment_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        OGS_NAS_STORE_DATA(&sess->nas.ue_pco,
            &pdu_session_establishment_request->
                extended_protocol_configuration_options);
    }

    ogs_assert(true ==
        smf_sbi_discover_and_send(OpenAPI_nf_type_UDM, sess, stream,
                0, (char *)OGS_SBI_RESOURCE_NAME_SM_DATA,
                smf_nudm_sdm_build_get));

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

int gsm_handle_pdu_session_modification_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        ogs_nas_5gs_pdu_session_modification_request_t *
            pdu_session_modification_request)
{
    char *strerror = NULL;

    int i, j;

    uint64_t pfcp_flags = 0;

    smf_bearer_t *qos_flow = NULL;
    smf_pf_t *pf = NULL;

    ogs_nas_qos_rule_t qos_rule[OGS_NAS_MAX_NUM_OF_QOS_RULE];
    ogs_nas_qos_flow_description_t
        qos_flow_description[OGS_NAS_MAX_NUM_OF_QOS_FLOW_DESCRIPTION];

    ogs_nas_qos_rules_t *requested_qos_rules =
        &pdu_session_modification_request->requested_qos_rules;
    ogs_nas_qos_flow_descriptions_t *requested_qos_flow_descriptions =
        &pdu_session_modification_request->requested_qos_flow_descriptions;

    smf_ue_t *smf_ue = NULL;
    ogs_pkbuf_t *n1smbuf = NULL;

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
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
        int num_of_rule = 0;

        num_of_rule = ogs_nas_parse_qos_rules(qos_rule, requested_qos_rules);
        ogs_assert(num_of_rule > 0);

        for (i = 0; i < num_of_rule; i++) {
            qos_flow = smf_qos_flow_find_by_qfi(
                    sess, qos_rule[i].identifier);
            if (!qos_flow) {
                ogs_error("No Qos Flow");
                continue;
            }

            ogs_list_init(&qos_flow->pf_to_add_list);

            if (qos_rule[i].code == OGS_NAS_QOS_CODE_DELETE_EXISTING_QOS_RULE) {
                smf_pf_remove_all(qos_flow);

                pfcp_flags |= OGS_PFCP_MODIFY_REMOVE;
                qos_flow_find_or_add(&sess->qos_flow_to_modify_list,
                                        qos_flow, to_modify_node);
            } else if (qos_rule[i].code ==
                OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_REPLACE_PACKET_FILTERS) {
                for (j = 0; j < qos_rule[i].num_of_packet_filter &&
                            j < OGS_MAX_NUM_OF_FLOW_IN_NAS; j++) {

                    pf = smf_pf_find_by_id(
                            qos_flow, qos_rule[i].pf[j].identifier+1);
                    if (pf) {
                        ogs_assert(
                            reconfigure_packet_filter(pf, &qos_rule[i], i) > 0);
            /*
             * Refer to lib/ipfw/ogs-ipfw.h
             * Issue #338
             *
             * <DOWNLINK>
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
             * <DOWNLINK>
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
                                ogs_ipfw_encode_flow_description(
                                        &pf->ipfw_rule);
                            ogs_assert(pf->flow_description);
                        }

                        pfcp_flags |= OGS_PFCP_MODIFY_TFT_REPLACE;
                        qos_flow_find_or_add(&sess->qos_flow_to_modify_list,
                                                qos_flow, to_modify_node);

                        ogs_list_add(
                                &qos_flow->pf_to_add_list, &pf->to_add_node);
                    }
                }
            } else if (qos_rule[i].code ==
                OGS_NAS_QOS_CODE_CREATE_NEW_QOS_RULE ||
                        qos_rule[i].code ==
                OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS) {

                if (qos_rule[i].code == OGS_NAS_QOS_CODE_CREATE_NEW_QOS_RULE)
                    smf_pf_remove_all(qos_flow);

                for (j = 0; j < qos_rule[i].num_of_packet_filter &&
                            j < OGS_MAX_NUM_OF_FLOW_IN_NAS; j++) {

                    pf = smf_pf_find_by_id(
                            qos_flow, qos_rule[i].pf[j].identifier+1);
                    if (!pf)
                        pf = smf_pf_add(qos_flow);
                    ogs_assert(pf);

                    ogs_assert(
                        reconfigure_packet_filter(pf, &qos_rule[i], i) > 0);
        /*
         * Refer to lib/ipfw/ogs-ipfw.h
         * Issue #338
         *
         * <DOWNLINK>
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
         * <DOWNLINK>
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
                        pfcp_flags |= OGS_PFCP_MODIFY_TFT_NEW;
                    } else if (qos_rule[i].code ==
                            OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS) {
                        pfcp_flags |= OGS_PFCP_MODIFY_TFT_ADD;
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

                    pf = smf_pf_find_by_id(
                            qos_flow, qos_rule[i].pf[j].identifier+1);
                    if (pf) {
                        qos_flow->pf_to_delete
                            [qos_flow->num_of_pf_to_delete++] =
                                qos_rule[i].pf[j].identifier;
                        smf_pf_remove(pf);
                    }
                }

                if (ogs_list_count(&qos_flow->pf_list)) {
                    pfcp_flags |= OGS_PFCP_MODIFY_TFT_DELETE;
                    qos_flow_find_or_add(&sess->qos_flow_to_modify_list,
                                            qos_flow, to_modify_node);
                } else {
                    pfcp_flags |= OGS_PFCP_MODIFY_REMOVE;
                    qos_flow_find_or_add(&sess->qos_flow_to_modify_list,
                                            qos_flow, to_modify_node);
                }
            }
        }
    }

    if (pdu_session_modification_request->presencemask &
        OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT) {
        int num_of_description = 0;

        num_of_description = ogs_nas_parse_qos_flow_descriptions(
                qos_flow_description, requested_qos_flow_descriptions);
        ogs_assert(num_of_description > 0);

        for (i = 0; i < num_of_description; i++) {
            qos_flow = smf_qos_flow_find_by_qfi(
                    sess, qos_flow_description[i].identifier);
            if (!qos_flow) {
                ogs_error("No Qos Flow");
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
                    ogs_fatal("Unknown qos_flow parameter identifier [%d]",
                            qos_flow_description[i].param[i].identifier);
                    ogs_assert_if_reached();
                }
            }

            pfcp_flags |= OGS_PFCP_MODIFY_QOS_MODIFY;
            qos_flow_find_or_add(&sess->qos_flow_to_modify_list,
                                    qos_flow, to_modify_node);
        }
    }

    if (ogs_list_count(&sess->qos_flow_to_modify_list) != 1) {
        strerror = ogs_msprintf("[%s:%d] Invalid modification request "
                "[modify:%d]", smf_ue->supi, sess->psi,
                ogs_list_count(&sess->qos_flow_to_modify_list));
        ogs_assert(strerror);

        ogs_error("%s", strerror);
        n1smbuf = gsm_build_pdu_session_modification_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        ogs_assert(n1smbuf);

        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                strerror, smf_ue->supi, n1smbuf, NULL);
        ogs_free(strerror);

        return OGS_ERROR;
    }

    if (pfcp_flags & OGS_PFCP_MODIFY_REMOVE) {
        ogs_assert((pfcp_flags &
                    (OGS_PFCP_MODIFY_TFT_NEW|OGS_PFCP_MODIFY_TFT_ADD|
                    OGS_PFCP_MODIFY_TFT_REPLACE|OGS_PFCP_MODIFY_TFT_DELETE|
                    OGS_PFCP_MODIFY_QOS_MODIFY)) == 0);

    } else if (pfcp_flags &
                (OGS_PFCP_MODIFY_TFT_NEW|OGS_PFCP_MODIFY_TFT_ADD|
                OGS_PFCP_MODIFY_TFT_REPLACE|OGS_PFCP_MODIFY_TFT_DELETE|
                OGS_PFCP_MODIFY_QOS_MODIFY)) {

        ogs_assert((pfcp_flags & OGS_PFCP_MODIFY_REMOVE) == 0);

        if (pfcp_flags &
                (OGS_PFCP_MODIFY_TFT_NEW|OGS_PFCP_MODIFY_TFT_ADD|
                OGS_PFCP_MODIFY_TFT_REPLACE|OGS_PFCP_MODIFY_TFT_DELETE))
            smf_bearer_tft_update(qos_flow);

        if (pfcp_flags & OGS_PFCP_MODIFY_QOS_MODIFY)
            smf_bearer_qos_update(qos_flow);

    } else {
        ogs_fatal("Unknown PFCP-Flags : [0x%llx]", (long long)pfcp_flags);
        ogs_assert_if_reached();
    }

    ogs_assert(OGS_OK ==
            smf_5gc_pfcp_send_qos_flow_list_modification_request(
                sess, stream,
                OGS_PFCP_MODIFY_UE_REQUESTED|pfcp_flags, 0));

    return OGS_OK;
}
