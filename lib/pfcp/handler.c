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

#include "ogs-pfcp.h"

bool ogs_pfcp_handle_heartbeat_request(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_heartbeat_request_t *req)
{
    int rv;
    ogs_assert(node);
    ogs_assert(xact);
    ogs_assert(req);

    if (req->recovery_time_stamp.presence == 0) {
        ogs_error("No Recovery Time Stamp");
        return false;
    }

    if (node->remote_recovery == 0 ||
        node->remote_recovery == req->recovery_time_stamp.u32) {
    } else if (node->remote_recovery < req->recovery_time_stamp.u32) {
        ogs_error("Remote PFCP restarted [%u<%u] in Heartbeat REQ",
            node->remote_recovery, req->recovery_time_stamp.u32);
        node->restoration_required = true;
    } else if (node->remote_recovery > req->recovery_time_stamp.u32) {
        ogs_error("Invalid Recovery Time Stamp [%u>%u] in Heartbeat REQ",
        node->remote_recovery, req->recovery_time_stamp.u32);
    }

    node->remote_recovery = req->recovery_time_stamp.u32;

    rv = ogs_pfcp_send_heartbeat_response(xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_send_heartbeat_response() failed");
        return false;
    }

    return true;
}

bool ogs_pfcp_handle_heartbeat_response(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_heartbeat_response_t *rsp)
{
    ogs_assert(node);
    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_pfcp_xact_commit(xact);

    if (rsp->recovery_time_stamp.presence == 0) {
        ogs_error("No Recovery Time Stamp");
        return false;
    }

    if (node->remote_recovery == 0 ||
        node->remote_recovery == rsp->recovery_time_stamp.u32) {
    } else if (node->remote_recovery < rsp->recovery_time_stamp.u32) {
        ogs_error("Remote PFCP restarted [%u<%u] in Heartbeat RSP",
            node->remote_recovery, rsp->recovery_time_stamp.u32);
        node->restoration_required = true;
    } else if (node->remote_recovery > rsp->recovery_time_stamp.u32) {
        ogs_error("Invalid Recovery Time Stamp [%u>%u] in Heartbeat RSP",
        node->remote_recovery, rsp->recovery_time_stamp.u32);
    }

    node->remote_recovery = rsp->recovery_time_stamp.u32;

    ogs_timer_start(node->t_no_heartbeat,
            ogs_local_conf()->time.message.pfcp.no_heartbeat_duration);

    return true;
}

bool ogs_pfcp_cp_handle_association_setup_request(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_association_setup_request_t *req)
{
    int i;
    int16_t decoded;

    ogs_assert(xact);
    ogs_assert(node);
    ogs_assert(req);

    ogs_pfcp_cp_send_association_setup_response(
            xact, OGS_PFCP_CAUSE_REQUEST_ACCEPTED);

    ogs_gtpu_resource_remove_all(&node->gtpu_resource_list);

    for (i = 0; i < OGS_MAX_NUM_OF_GTPU_RESOURCE; i++) {
        ogs_pfcp_tlv_user_plane_ip_resource_information_t *message =
            &req->user_plane_ip_resource_information[i];
        ogs_user_plane_ip_resource_info_t info;

        if (message->presence == 0)
            break;

        decoded = ogs_pfcp_parse_user_plane_ip_resource_info(&info, message);
        if (message->len == decoded)
            ogs_gtpu_resource_add(&node->gtpu_resource_list, &info);
        else
            ogs_error("Invalid User Plane IP Resource Info");
    }

    if (req->up_function_features.presence) {
        if (req->up_function_features.data && req->up_function_features.len) {
            node->up_function_features_len =
                ogs_min(req->up_function_features.len,
                        sizeof(node->up_function_features));
            memcpy(&node->up_function_features, req->up_function_features.data,
                    node->up_function_features_len);
        }
    }

    if (node->up_function_features.ftup == 0)
        ogs_warn("F-TEID allocation/release not supported with peer %s",
                ogs_sockaddr_to_string_static(node->addr_list));

    return true;
}

bool ogs_pfcp_cp_handle_association_setup_response(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_association_setup_response_t *rsp)
{
    int i;
    int16_t decoded;

    ogs_assert(xact);
    ogs_pfcp_xact_commit(xact);

    ogs_assert(node);
    ogs_assert(rsp);

    ogs_gtpu_resource_remove_all(&node->gtpu_resource_list);

    for (i = 0; i < OGS_MAX_NUM_OF_GTPU_RESOURCE; i++) {
        ogs_pfcp_tlv_user_plane_ip_resource_information_t *message =
            &rsp->user_plane_ip_resource_information[i];
        ogs_user_plane_ip_resource_info_t info;

        if (message->presence == 0)
            break;

        decoded = ogs_pfcp_parse_user_plane_ip_resource_info(&info, message);
        if (message->len == decoded)
            ogs_gtpu_resource_add(&node->gtpu_resource_list, &info);
        else
            ogs_error("Invalid User Plane IP Resource Info");
    }

    if (rsp->up_function_features.presence) {
        if (rsp->up_function_features.data && rsp->up_function_features.len) {
            node->up_function_features_len =
                ogs_min(rsp->up_function_features.len,
                        sizeof(node->up_function_features));
            memcpy(&node->up_function_features, rsp->up_function_features.data,
                    node->up_function_features_len);
        }
    }

    if (node->up_function_features.ftup == 0)
        ogs_warn("F-TEID allocation/release not supported with peer %s",
                ogs_sockaddr_to_string_static(node->addr_list));

    return true;
}

bool ogs_pfcp_up_handle_association_setup_request(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_association_setup_request_t *req)
{
    ogs_assert(xact);
    ogs_pfcp_up_send_association_setup_response(
            xact, OGS_PFCP_CAUSE_REQUEST_ACCEPTED);

    if (req->cp_function_features.presence) {
        ogs_pfcp_self()->cp_function_features.octet5 =
            req->cp_function_features.u8;
    }

    return true;
}

bool ogs_pfcp_up_handle_association_setup_response(
        ogs_pfcp_node_t *node, ogs_pfcp_xact_t *xact,
        ogs_pfcp_association_setup_response_t *rsp)
{
    ogs_assert(xact);
    ogs_pfcp_xact_commit(xact);

    if (rsp->cp_function_features.presence) {
        ogs_pfcp_self()->cp_function_features.octet5 =
            rsp->cp_function_features.u8;
    }

    return true;
}

bool ogs_pfcp_up_handle_pdr(
        ogs_pfcp_pdr_t *pdr, uint8_t type,
        ogs_gtp2_header_desc_t *recvhdr, ogs_pkbuf_t *sendbuf,
        ogs_pfcp_user_plane_report_t *report)
{
    ogs_pfcp_far_t *far = NULL;
    bool buffering;

    ogs_assert(sendbuf);
    ogs_assert(type);
    ogs_assert(pdr);
    ogs_assert(report);

    far = pdr->far;
    ogs_assert(far);

    memset(report, 0, sizeof(*report));

    buffering = false;

    if (!far->gnode) {
        buffering = true;

    } else {
        if (far->apply_action & OGS_PFCP_APPLY_ACTION_FORW) {
            ogs_gtp2_header_desc_t sendhdr;

            /* Forward packet */
            memset(&sendhdr, 0, sizeof(sendhdr));
            sendhdr.type = type;

            if (recvhdr) {
                /*
                 * Issue #2584
                 * Discussion #2477
                 *
                 * Forward PDCP Number via Indirect Tunnel during Handover
                 */
                if (recvhdr->pdcp_number_presence == true) {
                    sendhdr.pdcp_number_presence =
                        recvhdr->pdcp_number_presence;
                    sendhdr.pdcp_number = recvhdr->pdcp_number;
                }
            }

            ogs_pfcp_send_g_pdu(pdr, &sendhdr, sendbuf);

        } else if (far->apply_action & OGS_PFCP_APPLY_ACTION_BUFF) {

            buffering = true;

        } else {
            ogs_error("Not implemented = %d", far->apply_action);
            ogs_pkbuf_free(sendbuf);
        }
    }

    if (buffering == true) {

        if (far->num_of_buffered_packet == 0) {
            /* Only the first time a packet is buffered,
             * it reports downlink notifications. */
            report->type.downlink_data_report = 1;
        }

        if (far->num_of_buffered_packet < OGS_MAX_NUM_OF_PACKET_BUFFER) {
            far->buffered_packet[far->num_of_buffered_packet++] = sendbuf;
        } else {
            ogs_pkbuf_free(sendbuf);
        }
    }

    return true;
}

bool ogs_pfcp_up_handle_error_indication(
        ogs_pfcp_far_t *far, ogs_pfcp_user_plane_report_t *report)
{
    uint16_t len;

    ogs_assert(far);
    ogs_assert(far->hash.f_teid.len);

    ogs_assert(report);

    memset(report, 0, sizeof(*report));

    /* Remove TEID size, Only use ADDR size */
    len = far->hash.f_teid.len - 4;

    report->error_indication.remote_f_teid_len = 5 + len;
    report->error_indication.remote_f_teid.teid =
        htobe32(far->hash.f_teid.key.teid);
    if (len == OGS_IPV4_LEN) {
        report->error_indication.remote_f_teid.ipv4 = 1;
        memcpy(&report->error_indication.remote_f_teid.addr,
            far->hash.f_teid.key.addr,
            ogs_min(sizeof(report->error_indication.remote_f_teid.addr), len));
    } else if (len == OGS_IPV6_LEN) {
        report->error_indication.remote_f_teid.ipv6 = 1;
        memcpy(report->error_indication.remote_f_teid.addr6,
            far->hash.f_teid.key.addr,
            ogs_min(sizeof(report->error_indication.remote_f_teid.addr6), len));
    } else {
        ogs_error("Invalid Length [%d]", len);
        return false;
    }

    report->type.error_indication_report = 1;

    return true;
}

ogs_pfcp_pdr_t *ogs_pfcp_handle_create_pdr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_create_pdr_t *message,
        ogs_pfcp_sereq_flags_t *sereq_flags,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_urr_t *urr = NULL;
    ogs_pfcp_qer_t *qer = NULL;
    int i, len;
    int rv;

    ogs_assert(sess);
    ogs_assert(message);

    if (message->presence == 0)
        return NULL;

    if (message->pdr_id.presence == 0) {
        ogs_error("No PDR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_PDR_ID_TYPE;
        return NULL;
    }

    pdr = ogs_pfcp_pdr_find_or_add(sess, message->pdr_id.u16);
    ogs_assert(pdr);

    if (message->precedence.presence) {
        ogs_pfcp_pdr_reorder_by_precedence(pdr, message->precedence.u32);
        pdr->precedence = message->precedence.u32;
    }

    if (message->pdi.presence == 0) {
        ogs_error("No PDI in PDR");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_PDI_TYPE;
        return NULL;
    }

    if (message->pdi.source_interface.presence == 0) {
        ogs_error("No Source Interface in PDI");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_SOURCE_INTERFACE_TYPE;
        return NULL;
    }

    if (message->pdi.local_f_teid.presence) {
        ogs_pfcp_f_teid_t f_teid;

        memcpy(&f_teid, message->pdi.local_f_teid.data,
                ogs_min(sizeof(f_teid), message->pdi.local_f_teid.len));
        if (f_teid.ipv4 == 0 && f_teid.ipv6 == 0) {
            ogs_error("One of the IPv4 and IPv6 flags should be 1 "
                        "in the local F-TEID");
            *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
            *offending_ie_value = OGS_PFCP_F_TEID_TYPE;
            return NULL;
        }

        if (f_teid.ch == 0) {
            if (sereq_flags && sereq_flags->restoration_indication == 1) {
                f_teid.teid = be32toh(f_teid.teid);
                if (ogs_pfcp_object_find_by_teid(f_teid.teid)) {
                    ogs_error("TEID:%x had already been allocated", f_teid.teid);
                    *cause_value = OGS_PFCP_CAUSE_INVALID_F_TEID_ALLOCATION_OPTION;
                    *offending_ie_value = OGS_PFCP_F_TEID_TYPE;
                    return NULL;
                }
            }
        }
    }

    pdr->src_if = message->pdi.source_interface.u8;

    if (message->pdi.source_interface_type.presence) {
        pdr->src_if_type_presence = true;
        pdr->src_if_type = message->pdi.source_interface_type.u8;
    }

    ogs_pfcp_rule_remove_all(pdr);

    for (i = 0; i < OGS_MAX_NUM_OF_FLOW_IN_PDR; i++) {
        ogs_pfcp_sdf_filter_t sdf_filter;
        ogs_pfcp_rule_t *rule = NULL;
        ogs_pfcp_rule_t *oppsite_direction_rule = NULL;

        if (message->pdi.sdf_filter[i].presence == 0)
            break;

        len = ogs_pfcp_parse_sdf_filter(
                &sdf_filter, &message->pdi.sdf_filter[i]);
        if (message->pdi.sdf_filter[i].len != len) {
            ogs_error("Invalid SDF Filter");
            break;
        }

        /* Check Previous SDF Filter ID */
        if (sdf_filter.bid) {
            oppsite_direction_rule = ogs_pfcp_rule_find_by_sdf_filter_id(
                        sess, sdf_filter.sdf_filter_id);
        }

        if (!oppsite_direction_rule && !sdf_filter.fd) {
            ogs_error("Not Supported SDF Filter [Flags:0x%x, Len:%d]",
                    sdf_filter.flags, message->pdi.sdf_filter[i].len);
            ogs_log_hexdump(OGS_LOG_ERROR,
                    message->pdi.sdf_filter[i].data,
                    message->pdi.sdf_filter[i].len);
            continue;
        }

        rule = ogs_pfcp_rule_add(pdr);
        ogs_assert(rule);

        /* Set All Flags (BID, FL, SPI, TTC, FD) */
        rule->flags = sdf_filter.flags;

        if (oppsite_direction_rule) {
            /* Copy oppsite direction rule and Swap */
            memcpy(&rule->ipfw, &oppsite_direction_rule->ipfw,
                    sizeof(rule->ipfw));
            ogs_ipfw_rule_swap(&rule->ipfw);
        }

        /* If BID, Store SDF Filter ID */
        if (rule->bid)
            rule->sdf_filter_id = sdf_filter.sdf_filter_id;

        /* If FD, Apply Flow-Description to the RULE */
        if (rule->fd) {
            char *flow_description = NULL;

            flow_description = ogs_calloc(
                    1, sdf_filter.flow_description_len+1);
            ogs_assert(flow_description);
            ogs_cpystrn(flow_description,
                    sdf_filter.flow_description,
                    sdf_filter.flow_description_len+1);

            rv = ogs_ipfw_compile_rule(&rule->ipfw, flow_description);
            ogs_assert(rv == OGS_OK);

            ogs_free(flow_description);
/*
 *
 * TS29.244 Ch 5.2.1A.2A
 *
 * The UP function shall apply the SDF filter based on the Source Interface
 * of the PDR as follows (see also clause 8.2.5):
 *
 * - when the Source Interface is CORE, this indicates that the filter is
 *   for downlink data flow, so the UP function shall apply
 *   the Flow Description as is;
 *
 * - when the Source Interface is ACCESS, this indicates that the filter is
 *   for uplink data flow, so the UP function shall swap the source and
 *   destination address/port in the Flow Description;
 *
 * - when the Source Interface is CP-function or SGi-LAN,
 *   the UP function shall use the Flow Description as is.
 *
 *
 * Refer to lib/ipfw/ogs-ipfw.h
 * Issue #338
 *
 * <DOWNLINK>
 * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * RULE : Source <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> Destination <UE_IP> <UE_PORT>
 *
 * <UPLINK>
 * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * RULE : Source <UE_IP> <UE_PORT> Destination <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 */

            /* Uplink data flow */
            if (pdr->src_if == OGS_PFCP_INTERFACE_ACCESS)
                ogs_ipfw_rule_swap(&rule->ipfw);
        }
    }

    if (pdr->dnn) {
        ogs_free(pdr->dnn);
        pdr->dnn = NULL;
    }

    if (message->pdi.network_instance.presence) {
        char dnn[OGS_MAX_DNN_LEN+1];

        if (ogs_fqdn_parse(dnn, message->pdi.network_instance.data,
            ogs_min(message->pdi.network_instance.len, OGS_MAX_DNN_LEN)) > 0) {
            pdr->dnn = ogs_strdup(dnn);
            ogs_assert(pdr->dnn);
        } else {
            ogs_error("Invalid pdi.network_instance");
        }
    }

    pdr->chid = false;
    pdr->choose_id = 0;

    memset(&pdr->f_teid, 0, sizeof(pdr->f_teid));
    pdr->f_teid_len = 0;

    if (message->pdi.local_f_teid.presence) {
        pdr->f_teid_len =
            ogs_min(message->pdi.local_f_teid.len, sizeof(pdr->f_teid));
        memcpy(&pdr->f_teid, message->pdi.local_f_teid.data, pdr->f_teid_len);
        ogs_assert(pdr->f_teid.ipv4 || pdr->f_teid.ipv6);
        pdr->f_teid.teid = be32toh(pdr->f_teid.teid);
    }

    pdr->qfi = 0;

    if (message->pdi.qfi.presence) {
        pdr->qfi = message->pdi.qfi.u8;
    }

    memset(&pdr->ue_ip_addr, 0, sizeof(pdr->ue_ip_addr));
    pdr->ue_ip_addr_len = 0;

    if (message->pdi.ue_ip_address.presence) {
        pdr->ue_ip_addr_len =
            ogs_min(message->pdi.ue_ip_address.len, sizeof(pdr->ue_ip_addr));
        memcpy(&pdr->ue_ip_addr, message->pdi.ue_ip_address.data,
                pdr->ue_ip_addr_len);
    }

    for (i = 0; i < OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI; i++) {
        if (!pdr->ipv4_framed_routes || !pdr->ipv4_framed_routes[i])
            break;
        ogs_free(pdr->ipv4_framed_routes[i]);
        pdr->ipv4_framed_routes[i] = NULL;
    }

    for (i = 0; i < OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI; i++) {
        if (!pdr->ipv6_framed_routes || !pdr->ipv6_framed_routes[i])
            break;
        ogs_free(pdr->ipv6_framed_routes[i]);
        pdr->ipv6_framed_routes[i] = NULL;
    }

    for (i = 0; i < OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI; i++) {
        char *route;

        if (!message->pdi.framed_route[i].presence)
            break;

        if (!pdr->ipv4_framed_routes) {
            pdr->ipv4_framed_routes = ogs_calloc(
                    OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI, sizeof(pdr->ipv4_framed_routes[0]));
            ogs_assert(pdr->ipv4_framed_routes);
        }
        route = ogs_malloc(message->pdi.framed_route[i].len + 1);
        ogs_assert(route);
        memcpy(route, message->pdi.framed_route[i].data,
               message->pdi.framed_route[i].len);
        route[message->pdi.framed_route[i].len] = '\0';
        pdr->ipv4_framed_routes[i] = route;
    }

    for (i = 0; i < OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI; i++) {
        char *route;

        if (!message->pdi.framed_ipv6_route[i].presence)
            break;

        if (!pdr->ipv6_framed_routes) {
            pdr->ipv6_framed_routes = ogs_calloc(
                    OGS_MAX_NUM_OF_FRAMED_ROUTES_IN_PDI, sizeof(pdr->ipv6_framed_routes[0]));
            ogs_assert(pdr->ipv6_framed_routes);
        }
        route = ogs_malloc(message->pdi.framed_ipv6_route[i].len + 1);
        ogs_assert(route);
        memcpy(route, message->pdi.framed_ipv6_route[i].data,
               message->pdi.framed_ipv6_route[i].len);
        route[message->pdi.framed_ipv6_route[i].len] = '\0';
        pdr->ipv6_framed_routes[i] = route;
    }

    memset(&pdr->outer_header_removal, 0, sizeof(pdr->outer_header_removal));
    pdr->outer_header_removal_len = 0;

    if (message->outer_header_removal.presence) {
        pdr->outer_header_removal_len =
            ogs_min(message->outer_header_removal.len,
                    sizeof(pdr->outer_header_removal));
        memcpy(&pdr->outer_header_removal, message->outer_header_removal.data,
                pdr->outer_header_removal_len);
    }

    pdr->far = NULL;

    if (message->far_id.presence) {
        far = ogs_pfcp_far_find_or_add(sess, message->far_id.u32);
        ogs_assert(far);
        ogs_pfcp_pdr_associate_far(pdr, far);
    }

    for (i = 0; i < OGS_ARRAY_SIZE(pdr->urr); i++)
        pdr->urr[i] = NULL;
    pdr->num_of_urr = 0;
    for (i = 0; i < OGS_ARRAY_SIZE(message->urr_id); i++) {
        if (message->urr_id[i].presence) {
            urr = ogs_pfcp_urr_find_or_add(sess, message->urr_id[i].u32);
            ogs_assert(urr);
            ogs_pfcp_pdr_associate_urr(pdr,urr);
        }
    }

    pdr->qer = NULL;

    if (message->qer_id.presence) {
        qer = ogs_pfcp_qer_find_or_add(sess, message->qer_id.u32);
        ogs_assert(qer);
        ogs_pfcp_pdr_associate_qer(pdr, qer);
    }

    return pdr;
}

ogs_pfcp_pdr_t *ogs_pfcp_handle_created_pdr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_created_pdr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_pdr_t *pdr = NULL;

    ogs_assert(sess);
    ogs_assert(message);

    if (message->presence == 0)
        return NULL;

    if (message->pdr_id.presence == 0) {
        ogs_error("No PDR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_PDR_ID_TYPE;
        return NULL;
    }

    pdr = ogs_pfcp_pdr_find(sess, message->pdr_id.u16);
    if (!pdr) {
        ogs_error("Cannot find PDR-ID[%d] in PDR", message->pdr_id.u16);
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
        *offending_ie_value = OGS_PFCP_PDR_ID_TYPE;
        return NULL;
    }

    if (message->local_f_teid.presence) {
        ogs_pfcp_f_teid_t f_teid;

        memcpy(&f_teid, message->local_f_teid.data,
                ogs_min(sizeof(f_teid), message->local_f_teid.len));
        if (f_teid.ipv4 == 0 && f_teid.ipv6 == 0) {
            ogs_error("One of the IPv4 and IPv6 flags should be 1 "
                        "in the local F-TEID");
            *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
            *offending_ie_value = OGS_PFCP_F_TEID_TYPE;

            return NULL;
        }

        pdr->f_teid_len = message->local_f_teid.len;
        memcpy(&pdr->f_teid, message->local_f_teid.data,
                ogs_min(sizeof(pdr->f_teid), pdr->f_teid_len));
        ogs_assert(pdr->f_teid.ipv4 || pdr->f_teid.ipv6);
        pdr->f_teid.teid = be32toh(pdr->f_teid.teid);
    }

    return pdr;
}

ogs_pfcp_pdr_t *ogs_pfcp_handle_update_pdr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_update_pdr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_pdr_t *pdr = NULL;
    int i, len;
    int rv;

    ogs_assert(message);
    ogs_assert(sess);

    if (message->presence == 0)
        return NULL;

    if (message->pdr_id.presence == 0) {
        ogs_error("No PDR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_PDR_ID_TYPE;
        return NULL;
    }

    pdr = ogs_pfcp_pdr_find(sess, message->pdr_id.u16);
    if (!pdr) {
        ogs_error("Cannot find PDR-ID[%d] in PDR", message->pdr_id.u16);
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
        *offending_ie_value = OGS_PFCP_PDR_ID_TYPE;
        return NULL;
    }

    if (message->pdi.presence) {
        if (message->pdi.source_interface.presence == 0) {
            ogs_error("No Source Interface in PDI");
            *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
            *offending_ie_value = OGS_PFCP_SOURCE_INTERFACE_TYPE;
            return NULL;
        }

        if (message->pdi.local_f_teid.presence) {
            ogs_pfcp_f_teid_t f_teid;

            memcpy(&f_teid, message->pdi.local_f_teid.data,
                    ogs_min(sizeof(f_teid), message->pdi.local_f_teid.len));
            if (f_teid.ipv4 == 0 && f_teid.ipv6 == 0) {
                ogs_error("One of the IPv4 and IPv6 flags should be 1 "
                            "in the local F-TEID");
                *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
                *offending_ie_value = OGS_PFCP_F_TEID_TYPE;
                return NULL;
            }
        }

        pdr->src_if = message->pdi.source_interface.u8;

        if (message->pdi.source_interface_type.presence) {
            pdr->src_if_type_presence = true;
            pdr->src_if_type = message->pdi.source_interface_type.u8;
        }

        ogs_pfcp_rule_remove_all(pdr);

        for (i = 0; i < OGS_MAX_NUM_OF_FLOW_IN_PDR; i++) {
            ogs_pfcp_sdf_filter_t sdf_filter;
            ogs_pfcp_rule_t *rule = NULL;
            ogs_pfcp_rule_t *oppsite_direction_rule = NULL;

            if (message->pdi.sdf_filter[i].presence == 0)
                break;

            len = ogs_pfcp_parse_sdf_filter(
                    &sdf_filter, &message->pdi.sdf_filter[i]);
            if (message->pdi.sdf_filter[i].len != len) {
                ogs_error("Invalid SDF Filter");
                break;
            }

            /* Check Previous SDF Filter ID */
            if (sdf_filter.bid) {
                oppsite_direction_rule = ogs_pfcp_rule_find_by_sdf_filter_id(
                            sess, sdf_filter.sdf_filter_id);

            }

            if (!oppsite_direction_rule && !sdf_filter.fd) {
                ogs_error("Not Supported SDF Filter [Flags:0x%x, Len:%d]",
                        sdf_filter.flags, message->pdi.sdf_filter[i].len);
                ogs_log_hexdump(OGS_LOG_ERROR,
                        message->pdi.sdf_filter[i].data,
                        message->pdi.sdf_filter[i].len);
                continue;
            }

            rule = ogs_pfcp_rule_add(pdr);
            ogs_assert(rule);

            /* Set All Flags (BID, FL, SPI, TTC, FD) */
            rule->flags = sdf_filter.flags;

            if (oppsite_direction_rule) {
                /* Copy oppsite direction rule and Swap */
                memcpy(&rule->ipfw, &oppsite_direction_rule->ipfw,
                        sizeof(rule->ipfw));
                ogs_ipfw_rule_swap(&rule->ipfw);
            }

            /* If BID, Store SDF Filter ID */
            if (rule->bid)
                rule->sdf_filter_id = sdf_filter.sdf_filter_id;

            /* If FD, Apply Flow-Description to the RULE */
            if (rule->fd) {
                char *flow_description = NULL;

                flow_description = ogs_calloc(
                        1, sdf_filter.flow_description_len+1);
                ogs_assert(flow_description);
                ogs_cpystrn(flow_description,
                        sdf_filter.flow_description,
                        sdf_filter.flow_description_len+1);

                rv = ogs_ipfw_compile_rule(&rule->ipfw, flow_description);
                ogs_assert(rv == OGS_OK);

                ogs_free(flow_description);
    /*
     *
     * TS29.244 Ch 5.2.1A.2A
     *
     * The UP function shall apply the SDF filter based on the Source Interface
     * of the PDR as follows (see also clause 8.2.5):
     *
     * - when the Source Interface is CORE, this indicates that the filter is
     *   for downlink data flow, so the UP function shall apply
     *   the Flow Description as is;
     *
     * - when the Source Interface is ACCESS, this indicates that the filter is
     *   for uplink data flow, so the UP function shall swap the source and
     *   destination address/port in the Flow Description;
     *
     * - when the Source Interface is CP-function or SGi-LAN,
     *   the UP function shall use the Flow Description as is.
     *
     *
     * Refer to lib/ipfw/ogs-ipfw.h
     * Issue #338
     *
     * <DOWNLINK>
     * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
     * RULE : Source <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> Destination <UE_IP> <UE_PORT>
     *
     * <UPLINK>
     * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
     * RULE : Source <UE_IP> <UE_PORT> Destination <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
     */

                /* Uplink data flow */
                if (pdr->src_if == OGS_PFCP_INTERFACE_ACCESS)
                    ogs_ipfw_rule_swap(&rule->ipfw);
            }
        }

        if (message->pdi.network_instance.presence) {
            char dnn[OGS_MAX_DNN_LEN+1];

            if (ogs_fqdn_parse(dnn, message->pdi.network_instance.data,
                ogs_min(message->pdi.network_instance.len,
                    OGS_MAX_DNN_LEN)) > 0) {
                if (pdr->dnn)
                    ogs_free(pdr->dnn);
                pdr->dnn = ogs_strdup(dnn);
                ogs_assert(pdr->dnn);
            } else {
                ogs_error("Invalid pdi.network_instance");
            }
        }

        if (message->pdi.local_f_teid.presence) {
            pdr->f_teid_len = message->pdi.local_f_teid.len;
            memcpy(&pdr->f_teid, message->pdi.local_f_teid.data,
                    ogs_min(sizeof(pdr->f_teid), pdr->f_teid_len));
            pdr->f_teid.teid = be32toh(pdr->f_teid.teid);
        }

        if (message->pdi.qfi.presence) {
            pdr->qfi = message->pdi.qfi.u8;
        }
    }

    return pdr;
}

bool ogs_pfcp_handle_remove_pdr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_remove_pdr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_pdr_t *pdr = NULL;

    ogs_assert(sess);
    ogs_assert(message);

    if (message->presence == 0)
        return false;

    if (message->pdr_id.presence == 0) {
        ogs_error("No PDR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_PDR_ID_TYPE;
        return false;
    }

    pdr = ogs_pfcp_pdr_find(sess, message->pdr_id.u16);
    if (!pdr) {
        ogs_error("Unknown PDR-ID[%d]", message->pdr_id.u16);
        *cause_value = OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND;
        return false;
    }

    ogs_pfcp_pdr_remove(pdr);

    return true;
}

ogs_pfcp_far_t *ogs_pfcp_handle_create_far(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_create_far_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_far_t *far = NULL;

    ogs_assert(message);
    ogs_assert(sess);

    if (message->presence == 0)
        return NULL;

    if (message->far_id.presence == 0) {
        ogs_error("No FAR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_FAR_ID_TYPE;
        return NULL;
    }

    far = ogs_pfcp_far_find(sess, message->far_id.u32);
    if (!far) {
        ogs_error("Cannot find FAR-ID[%d] in PDR", message->far_id.u32);
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
        *offending_ie_value = OGS_PFCP_FAR_ID_TYPE;
        return NULL;
    }

    if (message->apply_action.presence == 0) {
        ogs_error("No Apply Action");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_APPLY_ACTION_TYPE;
        return NULL;
    }

    far->apply_action = message->apply_action.u16;

    far->dst_if = 0;
    memset(&far->outer_header_creation, 0, sizeof(far->outer_header_creation));

    if (far->dnn) {
        ogs_free(far->dnn);
        far->dnn = NULL;
    }

    if (message->forwarding_parameters.presence) {
        if (message->forwarding_parameters.destination_interface.presence) {
            far->dst_if =
                message->forwarding_parameters.destination_interface.u8;
        }

        if (message->forwarding_parameters.destination_interface_type.
                presence) {
            far->dst_if_type_presence = true;
            far->dst_if_type = message->forwarding_parameters.
                destination_interface_type.u8;
        }

        if (message->forwarding_parameters.network_instance.presence) {
            char dnn[OGS_MAX_DNN_LEN+1];

            if (ogs_fqdn_parse(dnn,
                message->forwarding_parameters.network_instance.data,
                ogs_min(message->forwarding_parameters.network_instance.len,
                    OGS_MAX_DNN_LEN)) > 0) {
                far->dnn = ogs_strdup(dnn);
                ogs_assert(far->dnn);
            } else {
                ogs_error("Invalid forwarding_parameters.network_instance");
            }
        }

        if (message->forwarding_parameters.outer_header_creation.presence) {
            ogs_pfcp_tlv_outer_header_creation_t *outer_header_creation =
                &message->forwarding_parameters.outer_header_creation;

            ogs_assert(outer_header_creation->data);
            ogs_assert(outer_header_creation->len);

            memcpy(&far->outer_header_creation, outer_header_creation->data,
                    ogs_min(sizeof(far->outer_header_creation),
                            outer_header_creation->len));
            far->outer_header_creation.teid =
                    be32toh(far->outer_header_creation.teid);
        }
    }

    return far;
}

ogs_pfcp_far_t *ogs_pfcp_handle_update_far_flags(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_update_far_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_far_t *far = NULL;

    ogs_assert(message);
    ogs_assert(sess);

    if (message->presence == 0)
        return NULL;

    if (message->far_id.presence == 0) {
        ogs_error("No FAR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_FAR_ID_TYPE;
        return NULL;
    }

    far = ogs_pfcp_far_find(sess, message->far_id.u32);
    if (!far) {
        ogs_error("Cannot find FAR-ID[%d] in PDR", message->far_id.u32);
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
        *offending_ie_value = OGS_PFCP_FAR_ID_TYPE;
        return NULL;
    }

    if (message->update_forwarding_parameters.presence) {

        if (message->update_forwarding_parameters.pfcpsmreq_flags.presence) {
            far->smreq_flags.value =
                message->update_forwarding_parameters.pfcpsmreq_flags.u8;
        }
    }

    return far;
}

ogs_pfcp_far_t *ogs_pfcp_handle_update_far(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_update_far_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_far_t *far = NULL;

    ogs_assert(message);
    ogs_assert(sess);

    if (message->presence == 0)
        return NULL;

    if (message->far_id.presence == 0) {
        ogs_error("No FAR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_FAR_ID_TYPE;
        return NULL;
    }

    far = ogs_pfcp_far_find(sess, message->far_id.u32);
    if (!far) {
        ogs_error("Cannot find FAR-ID[%d] in PDR", message->far_id.u32);
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
        *offending_ie_value = OGS_PFCP_FAR_ID_TYPE;
        return NULL;
    }

    if (message->apply_action.presence)
        far->apply_action = message->apply_action.u16;

    if (message->update_forwarding_parameters.presence) {
        if (message->update_forwarding_parameters.
                destination_interface.presence) {
            far->dst_if =
                message->update_forwarding_parameters.destination_interface.u8;
        }

        if (message->update_forwarding_parameters.destination_interface_type.
                presence) {
            far->dst_if_type_presence = true;
            far->dst_if_type = message->update_forwarding_parameters.
                destination_interface_type.u8;
        }

        if (message->update_forwarding_parameters.network_instance.presence) {
            char dnn[OGS_MAX_DNN_LEN+1];

            if (ogs_fqdn_parse(dnn,
                message->update_forwarding_parameters.network_instance.data,
                ogs_min(message->update_forwarding_parameters.
                    network_instance.len, OGS_MAX_DNN_LEN)) > 0) {
                if (far->dnn)
                    ogs_free(far->dnn);
                far->dnn = ogs_strdup(dnn);
                ogs_assert(far->dnn);
            } else {
                ogs_error("Invalid "
                        "update_forwarding_parameters.network_instance");
            }
        }

        if (message->update_forwarding_parameters.
                outer_header_creation.presence) {
            ogs_pfcp_tlv_outer_header_creation_t *outer_header_creation =
                &message->update_forwarding_parameters.outer_header_creation;

            ogs_assert(outer_header_creation->data);
            ogs_assert(outer_header_creation->len);

            memcpy(&far->outer_header_creation, outer_header_creation->data,
                    ogs_min(sizeof(far->outer_header_creation),
                            outer_header_creation->len));
            far->outer_header_creation.teid =
                    be32toh(far->outer_header_creation.teid);
        }
    }

    return far;
}

bool ogs_pfcp_handle_remove_far(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_remove_far_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_far_t *far = NULL;

    ogs_assert(sess);
    ogs_assert(message);

    if (message->presence == 0)
        return false;

    if (message->far_id.presence == 0) {
        ogs_error("No FAR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_FAR_ID_TYPE;
        return false;
    }

    far = ogs_pfcp_far_find(sess, message->far_id.u32);
    if (!far) {
        ogs_error("Unknown FAR-ID[%d]", message->far_id.u32);
        *cause_value = OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND;
        return false;
    }

    ogs_pfcp_far_remove(far);

    return true;
}

ogs_pfcp_qer_t *ogs_pfcp_handle_create_qer(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_create_qer_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_qer_t *qer = NULL;

    ogs_assert(message);
    ogs_assert(sess);

    if (message->presence == 0)
        return NULL;

    if (message->qer_id.presence == 0) {
        ogs_error("No QER-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_QER_ID_TYPE;
        return NULL;
    }

    qer = ogs_pfcp_qer_find(sess, message->qer_id.u32);
    if (!qer) {
        ogs_error("Cannot find QER-ID[%d] in PDR", message->qer_id.u32);
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
        *offending_ie_value = OGS_PFCP_QER_ID_TYPE;
        return NULL;
    }

    if (message->gate_status.presence == 0) {
        ogs_error("No Gate Status");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_APPLY_ACTION_TYPE;
        return NULL;
    }

    qer->gate_status.value = message->gate_status.u8;

    memset(&qer->mbr, 0, sizeof(qer->mbr));
    memset(&qer->gbr, 0, sizeof(qer->gbr));

    if (message->maximum_bitrate.presence)
        ogs_pfcp_parse_bitrate(&qer->mbr, &message->maximum_bitrate);
    if (message->guaranteed_bitrate.presence)
        ogs_pfcp_parse_bitrate(&qer->gbr, &message->guaranteed_bitrate);

    qer->qfi = 0;

    if (message->qos_flow_identifier.presence)
        qer->qfi = message->qos_flow_identifier.u8;

    return qer;
}

ogs_pfcp_qer_t *ogs_pfcp_handle_update_qer(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_update_qer_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_qer_t *qer = NULL;

    ogs_assert(message);
    ogs_assert(sess);

    if (message->presence == 0)
        return NULL;

    if (message->qer_id.presence == 0) {
        ogs_error("No QER-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_QER_ID_TYPE;
        return NULL;
    }

    qer = ogs_pfcp_qer_find(sess, message->qer_id.u32);
    if (!qer) {
        ogs_error("Cannot find QER-ID[%d] in PDR", message->qer_id.u32);
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
        *offending_ie_value = OGS_PFCP_QER_ID_TYPE;
        return NULL;
    }

    if (message->maximum_bitrate.presence)
        ogs_pfcp_parse_bitrate(&qer->mbr, &message->maximum_bitrate);
    if (message->guaranteed_bitrate.presence)
        ogs_pfcp_parse_bitrate(&qer->gbr, &message->guaranteed_bitrate);

    return qer;
}

bool ogs_pfcp_handle_remove_qer(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_remove_qer_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_qer_t *qer = NULL;

    ogs_assert(sess);
    ogs_assert(message);

    if (message->presence == 0)
        return false;

    if (message->qer_id.presence == 0) {
        ogs_error("No QER-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_QER_ID_TYPE;
        return false;
    }

    qer = ogs_pfcp_qer_find(sess, message->qer_id.u32);
    if (!qer) {
        ogs_error("Unknown QER-ID[%d]", message->qer_id.u32);
        *cause_value = OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND;
        return false;
    }

    ogs_pfcp_qer_remove(qer);

    return true;
}

ogs_pfcp_bar_t *ogs_pfcp_handle_create_bar(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_create_bar_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_assert(message);
    ogs_assert(sess);

    if (message->presence == 0)
        return NULL;

    if (message->bar_id.presence == 0) {
        ogs_error("No BAR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_BAR_ID_TYPE;
        return NULL;
    }

    if (sess->bar)
        ogs_pfcp_bar_delete(sess->bar);

    ogs_pfcp_bar_new(sess);
    ogs_assert(sess->bar);

    sess->bar->id = message->bar_id.u8;

    return sess->bar;
}

bool ogs_pfcp_handle_remove_bar(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_remove_bar_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_assert(sess);
    ogs_assert(message);

    if (message->presence == 0)
        return false;

    if (message->bar_id.presence == 0) {
        ogs_error("No BAR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_BAR_ID_TYPE;
        return false;
    }

    if (sess->bar && sess->bar->id == message->bar_id.u8) {
        ogs_pfcp_bar_delete(sess->bar);
        return true;
    }

    ogs_error("[%p] Unknown BAR-ID[%d]", sess->bar, message->bar_id.u8);
    *cause_value = OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND;
    return false;
}

ogs_pfcp_urr_t *ogs_pfcp_handle_create_urr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_create_urr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    int16_t decoded;
    ogs_pfcp_urr_t *urr = NULL;

    ogs_assert(message);
    ogs_assert(sess);

    if (message->presence == 0)
        return NULL;

    if (message->urr_id.presence == 0) {
        ogs_error("No URR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_URR_ID_TYPE;
        return NULL;
    }

    urr = ogs_pfcp_urr_find(sess, message->urr_id.u32);
    if (!urr) {
        ogs_error("Cannot find URR-ID[%d] in PDR", message->urr_id.u32);
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
        *offending_ie_value = OGS_PFCP_URR_ID_TYPE;
        return NULL;
    }

    if (message->measurement_method.presence == 0) {
        ogs_error("No Measurement Method");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_MEASUREMENT_METHOD_TYPE;
        return NULL;
    }

    if (message->reporting_triggers.presence == 0) {
        ogs_error("No Reporting Triggers");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_REPORTING_TRIGGERS_TYPE;
        return NULL;
    }

    urr->meas_method = message->measurement_method.u8;
    urr->rep_triggers.reptri_5 = (message->reporting_triggers.u24 >> 16) & 0xFF;
    urr->rep_triggers.reptri_6 = (message->reporting_triggers.u24 >> 8) & 0xFF;
    urr->rep_triggers.reptri_7 = message->reporting_triggers.u24 & 0xFF;

    if (message->measurement_period.presence) {
        urr->meas_period = message->measurement_period.u32;
    }

    if (message->volume_threshold.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_VOLUME)) {
        decoded = ogs_pfcp_parse_volume(
                &urr->vol_threshold, &message->volume_threshold);
        if (message->volume_threshold.len != decoded) {
            ogs_error("Invalid Volume Threshold");
            *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
            *offending_ie_value = OGS_PFCP_VOLUME_THRESHOLD_TYPE;
            return NULL;
        }
    }

    if (message->volume_quota.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_VOLUME)) {
        decoded = ogs_pfcp_parse_volume(
                &urr->vol_quota, &message->volume_quota);
        if (message->volume_quota.len != decoded) {
            ogs_error("Invalid Volume Quota");
            *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
            *offending_ie_value = OGS_PFCP_VOLUME_QUOTA_TYPE;
            return NULL;
        }
    }

    if (message->event_threshold.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_EVENT)) {
        urr->event_threshold = message->event_threshold.u32;
    }

    if (message->event_quota.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_EVENT)) {
        urr->event_quota = message->event_quota.u32;
    }

    if (message->time_threshold.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_DURATION)) {
        urr->time_threshold = message->time_threshold.u32;
    }

    if (message->time_quota.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_DURATION)) {
        urr->time_quota = message->time_quota.u32;
    }

    if (message->quota_holding_time.presence) {
        urr->quota_holding_time = message->quota_holding_time.u32;
    }

    if (message->dropped_dl_traffic_threshold.presence) {
        ogs_pfcp_parse_dropped_dl_traffic_threshold(
                &urr->dropped_dl_traffic_threshold,
                &message->dropped_dl_traffic_threshold);
    }

    if (message->quota_validity_time.presence) {
        urr->quota_validity_time = message->quota_validity_time.u32;
    }

    if (message->measurement_information.presence &&
        message->measurement_information.len >= 1) {
        urr->meas_info.octet5 = *((unsigned char *)message->measurement_information.data);
    }

    return urr;
}

ogs_pfcp_urr_t *ogs_pfcp_handle_update_urr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_update_urr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    int16_t decoded;
    ogs_pfcp_urr_t *urr = NULL;

    ogs_assert(message);
    ogs_assert(sess);

    if (message->presence == 0)
        return NULL;

    if (message->urr_id.presence == 0) {
        ogs_error("No URR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_URR_ID_TYPE;
        return NULL;
    }

    urr = ogs_pfcp_urr_find(sess, message->urr_id.u32);
    if (!urr) {
        ogs_error("Cannot find URR-ID[%d] in PDR", message->urr_id.u32);
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
        *offending_ie_value = OGS_PFCP_URR_ID_TYPE;
        return NULL;
    }

    if (message->measurement_method.presence)
        urr->meas_method = message->measurement_method.u8;

    if (message->reporting_triggers.presence) {
        urr->rep_triggers.reptri_5 = message->reporting_triggers.u24 & 0xFF;
        urr->rep_triggers.reptri_6 = (message->reporting_triggers.u24 >> 8) & 0xFF;
        urr->rep_triggers.reptri_7 = (message->reporting_triggers.u24 >> 16) & 0xFF;
    }

    if (message->measurement_period.presence) {
        urr->meas_period = message->measurement_period.u32;
    }

    if (message->volume_threshold.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_VOLUME)) {
        decoded = ogs_pfcp_parse_volume(
                &urr->vol_threshold, &message->volume_threshold);
        if (message->volume_threshold.len != decoded) {
            ogs_error("Invalid Volume Threshold");
            *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
            *offending_ie_value = OGS_PFCP_VOLUME_THRESHOLD_TYPE;
            return NULL;
        }
    }

    if (message->volume_quota.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_VOLUME)) {
        decoded = ogs_pfcp_parse_volume(
                &urr->vol_quota, &message->volume_quota);
        if (message->volume_quota.len != decoded) {
            ogs_error("Invalid Volume Quota");
            *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
            *offending_ie_value = OGS_PFCP_VOLUME_QUOTA_TYPE;
            return NULL;
        }
    }

    if (message->event_threshold.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_EVENT)) {
        urr->event_threshold = message->event_threshold.u32;
    }

    if (message->event_quota.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_EVENT)) {
        urr->event_quota = message->event_quota.u32;
    }

    if (message->time_threshold.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_DURATION)) {
        urr->time_threshold = message->time_threshold.u32;
    }

    if (message->time_quota.presence &&
        (urr->meas_method & OGS_PFCP_MEASUREMENT_METHOD_DURATION)) {
        urr->time_quota = message->time_quota.u32;
    }

    if (message->quota_holding_time.presence) {
        urr->quota_holding_time = message->quota_holding_time.u32;
    }

    if (message->dropped_dl_traffic_threshold.presence) {
        ogs_pfcp_parse_dropped_dl_traffic_threshold(
                &urr->dropped_dl_traffic_threshold,
                &message->dropped_dl_traffic_threshold);
    }

    if (message->quota_validity_time.presence) {
        urr->quota_validity_time = message->quota_validity_time.u32;
    }

    if (message->measurement_information.presence &&
        message->measurement_information.len >= 1) {
        urr->meas_info.octet5 = *((unsigned char *)message->measurement_information.data);
    }

    return urr;
}

bool ogs_pfcp_handle_remove_urr(ogs_pfcp_sess_t *sess,
        ogs_pfcp_tlv_remove_urr_t *message,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    ogs_pfcp_urr_t *urr = NULL;

    ogs_assert(sess);
    ogs_assert(message);

    if (message->presence == 0)
        return false;

    if (message->urr_id.presence == 0) {
        ogs_error("No URR-ID");
        *cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
        *offending_ie_value = OGS_PFCP_URR_ID_TYPE;
        return false;
    }

    urr = ogs_pfcp_urr_find(sess, message->urr_id.u32);
    if (!urr) {
        ogs_error("Unknown URR-ID[%d]", message->urr_id.u32);
        *cause_value = OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND;
        return false;
    }

    ogs_pfcp_urr_remove(urr);

    return true;
}
