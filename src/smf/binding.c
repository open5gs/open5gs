/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "binding.h"
#include "s5c-build.h"
#include "pfcp-path.h"
#include "gtp-path.h"

#include "ipfw/ipfw2.h"

static void gtp_bearer_timeout(ogs_gtp_xact_t *xact, void *data)
{
    smf_bearer_t *bearer = data;
    smf_sess_t *sess = NULL;
    smf_ue_t *smf_ue = NULL;
    uint8_t type = 0;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    type = xact->seq[0].type;

    switch (type) {
    case OGS_GTP2_CREATE_BEARER_REQUEST_TYPE:
        ogs_error("[%s] No Create Bearer Response", smf_ue->imsi_bcd);
        if (!smf_bearer_cycle(bearer)) {
            ogs_warn("[%s] Bearer has already been removed", smf_ue->imsi_bcd);
            break;
        }
        ogs_assert(OGS_OK ==
            smf_epc_pfcp_send_one_bearer_modification_request(
                bearer, NULL, OGS_PFCP_MODIFY_REMOVE,
                OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
                OGS_GTP2_CAUSE_UNDEFINED_VALUE));
        break;
    case OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE:
        ogs_error("[%s] No Update Bearer Response", smf_ue->imsi_bcd);
        break;
    default:
        ogs_error("GTP Timeout : IMSI[%s] Message-Type[%d]",
                smf_ue->imsi_bcd, type);
        break;
    }
}

/*
 * Issue #338
 *
 * <DOWNLINK>
 * RULE : Source <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> Destination <UE_IP> <UE_PORT>
 * TFT : Local <UE_IP> <UE_PORT> REMOTE <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 *
 * <UPLINK>
 * RULE : Source <UE_IP> <UE_PORT> Destination <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 * TFT : Local <UE_IP> <UE_PORT> REMOTE <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 */
static void encode_traffic_flow_template(
        ogs_gtp2_tft_t *tft, smf_bearer_t *bearer, uint8_t tft_operation_code)
{
    int i;
    smf_pf_t *pf = NULL;

    ogs_assert(tft);
    ogs_assert(bearer);

    memset(tft, 0, sizeof(*tft));
    tft->code = tft_operation_code;

    i = 0;
    if (tft_operation_code != OGS_GTP2_TFT_CODE_DELETE_EXISTING_TFT &&
        tft_operation_code != OGS_GTP2_TFT_CODE_NO_TFT_OPERATION) {
        ogs_list_for_each_entry(&bearer->pf_to_add_list, pf, to_add_node) {
            ogs_assert(i < OGS_MAX_NUM_OF_FLOW_IN_GTP);
            tft->pf[i].identifier = pf->identifier - 1;

            /* Deletion of packet filters
             * from existing requires only the identifier */
            if (tft_operation_code !=
                OGS_GTP2_TFT_CODE_DELETE_PACKET_FILTERS_FROM_EXISTING) {

                tft->pf[i].direction = pf->direction;
                tft->pf[i].precedence = pf->precedence - 1;

                ogs_pf_content_from_ipfw_rule(
                        pf->direction, &tft->pf[i].content, &pf->ipfw_rule,
                        ogs_app()->
                        parameter.no_ipv4v6_local_addr_in_packet_filter);
            }

            i++;
        }
    }
    tft->num_of_packet_filter = i;
}

void smf_bearer_binding(smf_sess_t *sess)
{
    int rv;
    int i, j;

    ogs_assert(sess);

    for (i = 0; i < sess->policy.num_of_pcc_rule; i++) {
        ogs_gtp_xact_t *xact = NULL;
        ogs_gtp2_header_t h;
        ogs_pkbuf_t *pkbuf = NULL;
        smf_bearer_t *bearer = NULL;
        ogs_pcc_rule_t *pcc_rule = &sess->policy.pcc_rule[i];

        ogs_assert(pcc_rule);
        if (pcc_rule->name == NULL) {
            ogs_error("No PCC Rule Name");
            continue;
        }

        if (pcc_rule->type == OGS_PCC_RULE_TYPE_INSTALL) {
            bool bearer_created = false;
            bool qos_presence = false;

            bearer = smf_bearer_find_by_pcc_rule_name(sess, pcc_rule->name);
            if (!bearer) {
                ogs_pfcp_pdr_t *dl_pdr = NULL, *ul_pdr = NULL;

                if (pcc_rule->num_of_flow == 0) {
                    /* TFT is mandatory in
                     * activate dedicated EPS bearer context request */
                    ogs_error("No flow in PCC Rule");
                    continue;
                }

                if (ogs_list_count(&sess->bearer_list) >=
                        OGS_MAX_NUM_OF_BEARER) {
                    ogs_error("Bearer Overflow[%d]",
                            ogs_list_count(&sess->bearer_list));
                    continue;
                }

                bearer = smf_bearer_add(sess);
                ogs_assert(bearer);

                dl_pdr = bearer->dl_pdr;
                ogs_assert(dl_pdr);
                ul_pdr = bearer->ul_pdr;
                ogs_assert(ul_pdr);

                /* Precedence is set to the order in which it was created */
                dl_pdr->precedence = dl_pdr->id;
                ul_pdr->precedence = ul_pdr->id;

                ogs_assert(sess->pfcp_node);
                if (sess->pfcp_node->up_function_features.ftup) {

           /* TS 129 244 V16.5.0 8.2.3
            *
            * At least one of the V4 and V6 flags shall be set to "1",
            * and both may be set to "1" for both scenarios:
            *
            * - when the CP function is providing F-TEID, i.e.
            *   both IPv4 address field and IPv6 address field may be present;
            *   or
            * - when the UP function is requested to allocate the F-TEID,
            *   i.e. when CHOOSE bit is set to "1",
            *   and the IPv4 address and IPv6 address fields are not present.
            */

                    ul_pdr->f_teid.ipv4 = 1;
                    ul_pdr->f_teid.ipv6 = 1;
                    ul_pdr->f_teid.ch = 1;
                    ul_pdr->f_teid_len = 1;
                } else {
                    ogs_gtpu_resource_t *resource = NULL;
                    resource = ogs_pfcp_find_gtpu_resource(
                            &sess->pfcp_node->gtpu_resource_list,
                            sess->session.name, OGS_PFCP_INTERFACE_ACCESS);
                    if (resource) {
                        ogs_user_plane_ip_resource_info_to_sockaddr(
                                &resource->info,
                                &bearer->pgw_s5u_addr, &bearer->pgw_s5u_addr6);
                        if (resource->info.teidri)
                            bearer->pgw_s5u_teid = OGS_PFCP_GTPU_INDEX_TO_TEID(
                                    ul_pdr->index, resource->info.teidri,
                                    resource->info.teid_range);
                        else
                            bearer->pgw_s5u_teid = ul_pdr->index;
                    } else {
                        if (sess->pfcp_node->addr.ogs_sa_family == AF_INET)
                            ogs_assert(OGS_OK ==
                                ogs_copyaddrinfo(&bearer->pgw_s5u_addr,
                                    &sess->pfcp_node->addr));
                        else if (sess->pfcp_node->addr.ogs_sa_family ==
                                AF_INET6)
                            ogs_assert(OGS_OK ==
                                ogs_copyaddrinfo(&bearer->pgw_s5u_addr6,
                                    &sess->pfcp_node->addr));
                        else
                            ogs_assert_if_reached();

                        bearer->pgw_s5u_teid = ul_pdr->index;
                    }

                    ogs_assert(OGS_OK ==
                        ogs_pfcp_sockaddr_to_f_teid(
                            bearer->pgw_s5u_addr, bearer->pgw_s5u_addr6,
                            &ul_pdr->f_teid, &ul_pdr->f_teid_len));
                    ul_pdr->f_teid.teid = bearer->pgw_s5u_teid;
                }

                bearer->pcc_rule.name = ogs_strdup(pcc_rule->name);
                ogs_assert(bearer->pcc_rule.name);

                memcpy(&bearer->qos, &pcc_rule->qos, sizeof(ogs_qos_t));

                bearer_created = true;

            } else {
                ogs_assert(strcmp(bearer->pcc_rule.name, pcc_rule->name) == 0);

                if ((pcc_rule->qos.mbr.downlink &&
                    bearer->qos.mbr.downlink != pcc_rule->qos.mbr.downlink) ||
                    (pcc_rule->qos.mbr.uplink &&
                     bearer->qos.mbr.uplink != pcc_rule->qos.mbr.uplink) ||
                    (pcc_rule->qos.gbr.downlink &&
                    bearer->qos.gbr.downlink != pcc_rule->qos.gbr.downlink) ||
                    (pcc_rule->qos.gbr.uplink &&
                    bearer->qos.gbr.uplink != pcc_rule->qos.gbr.uplink)) {
                    /* Update QoS parameter */
                    memcpy(&bearer->qos, &pcc_rule->qos, sizeof(ogs_qos_t));

                    /* Update Bearer Request encodes updated QoS parameter */
                    qos_presence = true;
                }
            }

        /*
         * We only use the method of adding a flow to an existing tft.
         *
         * EPC: OGS_GTP2_TFT_CODE_ADD_PACKET_FILTERS_TO_EXISTING_TFT
         * 5GC: OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS
         */
            ogs_list_init(&bearer->pf_to_add_list);

            for (j = 0; j < pcc_rule->num_of_flow; j++) {
                smf_pf_t *pf = NULL;
                ogs_flow_t *flow = &pcc_rule->flow[j];

                ogs_expect_or_return(flow);
                ogs_expect_or_return(flow->description);

                /*
                 * To add a flow to an existing tft.
                 * duplicated flows are not added
                 */
                if (smf_pf_find_by_flow(
                    bearer, flow->direction, flow->description) != NULL) {
                    continue;
                }

                /*
                 * To add a flow to an existing tft.
                 *
                 * Only new flows are added to the PF list.
                 * Then, in the PF list, there are all flows
                 * from the beginning to the present.
                 */
                pf = smf_pf_add(bearer);
                if (!pf) {
                    ogs_error("Overflow: PacketFilter in Bearer");
                    break;
                }

                pf->direction = flow->direction;
                pf->flow_description = ogs_strdup(flow->description);
                ogs_assert(pf->flow_description);

                rv = ogs_ipfw_compile_rule(
                        &pf->ipfw_rule, pf->flow_description);
/*
 * Refer to lib/ipfw/ogs-ipfw.h
 * Issue #338
 *
 * <DOWNLINK>
 * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * -->
 * RULE : Source <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> Destination <UE_IP> <UE_PORT>
 *
 * <UPLINK>
 * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * -->
 * RULE : Source <UE_IP> <UE_PORT> Destination <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 */
                if (flow->direction == OGS_FLOW_UPLINK_ONLY)
                    ogs_ipfw_rule_swap(&pf->ipfw_rule);

                if (rv != OGS_OK) {
                    ogs_error("Invalid Flow-Description[%s]",
                            pf->flow_description);
                    smf_pf_remove(pf);
                    break;
                }

                /*
                 * To add a flow to an existing tft.
                 *
                 * 'pf_to_add_list' now has the added flow.
                 */
                ogs_list_add(&bearer->pf_to_add_list, &pf->to_add_node);
            }

            if (bearer_created == false &&
                qos_presence == false &&
                ogs_list_count(&bearer->pf_to_add_list) == 0) {
                ogs_warn("No need to send 'Update Bearer Request'");
                ogs_warn("bearer_created:%d, qos_presence:%d, rule_count:%d",
                    bearer_created, qos_presence,
                    ogs_list_count(&bearer->pf_to_add_list));
                continue;
            }

            if (bearer_created == true) {

                smf_bearer_tft_update(bearer);
                smf_bearer_qos_update(bearer);

                ogs_assert(OGS_OK ==
                    smf_epc_pfcp_send_one_bearer_modification_request(
                        bearer, NULL, OGS_PFCP_MODIFY_CREATE,
                        OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
                        OGS_GTP2_CAUSE_UNDEFINED_VALUE));
            } else {
                ogs_gtp2_tft_t tft;

                memset(&tft, 0, sizeof tft);
                if (ogs_list_count(&bearer->pf_to_add_list) > 0) {
                    encode_traffic_flow_template(
                        &tft, bearer,
                        OGS_GTP2_TFT_CODE_ADD_PACKET_FILTERS_TO_EXISTING_TFT);
                }

                memset(&h, 0, sizeof(ogs_gtp2_header_t));
                h.type = OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE;
                h.teid = sess->sgw_s5c_teid;

                pkbuf = smf_s5c_build_update_bearer_request(
                        h.type, bearer,
                        OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
                        (ogs_list_count(&bearer->pf_to_add_list) > 0) ?
                            &tft : NULL, qos_presence);
                ogs_expect_or_return(pkbuf);

                xact = ogs_gtp_xact_local_create(
                        sess->gnode, &h, pkbuf, gtp_bearer_timeout, bearer);
                ogs_expect_or_return(xact);
                xact->local_teid = sess->smf_n4_teid;

                if (ogs_list_count(&bearer->pf_to_add_list) > 0)
                    xact->update_flags |= OGS_GTP_MODIFY_TFT_UPDATE;
                if (qos_presence == true)
                    xact->update_flags |= OGS_GTP_MODIFY_QOS_UPDATE;

                rv = ogs_gtp_xact_commit(xact);
                ogs_expect(rv == OGS_OK);
            }

        } else if (pcc_rule->type == OGS_PCC_RULE_TYPE_REMOVE) {
            bearer = smf_bearer_find_by_pcc_rule_name(sess, pcc_rule->name);

            if (!bearer) {
                ogs_warn("No need to send 'Delete Bearer Request'");
                ogs_warn("  - Bearer[Name:%s] has already been removed.",
                        pcc_rule->name);
                continue;
            }

            /*
             * TS23.214
             * 6.3.1.7 Procedures with modification of bearer
             * p50
             * 2.  ...
             * For "PGW/MME initiated bearer deactivation procedure",
             * PGW-C shall indicate PGW-U to stop counting and stop
             * forwarding downlink packets for the affected bearer(s).
             */
            ogs_assert(OGS_OK ==
                smf_epc_pfcp_send_one_bearer_modification_request(
                    bearer, NULL,
                    OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_DEACTIVATE,
                    OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
                    OGS_GTP2_CAUSE_UNDEFINED_VALUE));
        } else {
            ogs_error("Invalid Type[%d]", pcc_rule->type);
        }
    }
}

int smf_gtp2_send_create_bearer_request(smf_bearer_t *bearer)
{
    int rv;

    smf_sess_t *sess = NULL;
    ogs_gtp_xact_t *xact = NULL;

    ogs_gtp2_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_tft_t tft;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    h.type = OGS_GTP2_CREATE_BEARER_REQUEST_TYPE;
    h.teid = sess->sgw_s5c_teid;

    memset(&tft, 0, sizeof tft);
    encode_traffic_flow_template(
            &tft, bearer, OGS_GTP2_TFT_CODE_CREATE_NEW_TFT);

    pkbuf = smf_s5c_build_create_bearer_request(h.type, bearer, &tft);
    ogs_expect_or_return_val(pkbuf, OGS_ERROR);

    xact = ogs_gtp_xact_local_create(
            sess->gnode, &h, pkbuf, gtp_bearer_timeout, bearer);
    ogs_expect_or_return_val(xact, OGS_ERROR);
    xact->local_teid = sess->smf_n4_teid;

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

void smf_qos_flow_binding(smf_sess_t *sess)
{
    int rv;
    int i, j;

    uint64_t pfcp_flags, check;

    ogs_assert(sess);

    pfcp_flags = OGS_PFCP_MODIFY_NETWORK_REQUESTED;

    ogs_list_init(&sess->qos_flow_to_modify_list);

    for (i = 0; i < sess->policy.num_of_pcc_rule; i++) {
        smf_bearer_t *qos_flow = NULL;
        ogs_pcc_rule_t *pcc_rule = &sess->policy.pcc_rule[i];

        ogs_assert(pcc_rule);
        if (pcc_rule->id == NULL) {
            ogs_error("No PCC Rule Id");
            continue;
        }

        if (pcc_rule->type == OGS_PCC_RULE_TYPE_INSTALL) {
            smf_pf_t *pf = NULL;
            ogs_pfcp_pdr_t *dl_pdr = NULL, *ul_pdr = NULL;

            bool qos_flow_created = false;
            bool qos_presence = false;

            qos_flow = smf_qos_flow_find_by_pcc_rule_id(sess, pcc_rule->id);
            if (!qos_flow) {
                if (pcc_rule->num_of_flow == 0) {
                    /* TFT is mandatory in
                     * activate dedicated EPS bearer context request */
                    ogs_error("No flow in PCC Rule");
                    continue;
                }

                if (ogs_list_count(&sess->bearer_list) >=
                        OGS_MAX_NUM_OF_BEARER) {
                    ogs_error("QosFlow Overflow[%d]",
                            ogs_list_count(&sess->bearer_list));
                    continue;
                }

                qos_flow = smf_qos_flow_add(sess);
                ogs_assert(qos_flow);

                dl_pdr = qos_flow->dl_pdr;
                ogs_assert(dl_pdr);
                ul_pdr = qos_flow->ul_pdr;
                ogs_assert(ul_pdr);

                /* Precedence is derived from PCC Rule Precedence */
                dl_pdr->precedence = pcc_rule->precedence;
                ul_pdr->precedence = pcc_rule->precedence;

                /* Set UPF-N3 TEID & ADDR to the UL PDR */
                ogs_assert(sess->pfcp_node);
                if (sess->pfcp_node->up_function_features.ftup) {

           /* TS 129 244 V16.5.0 8.2.3
            *
            * At least one of the V4 and V6 flags shall be set to "1",
            * and both may be set to "1" for both scenarios:
            *
            * - when the CP function is providing F-TEID, i.e.
            *   both IPv4 address field and IPv6 address field may be present;
            *   or
            * - when the UP function is requested to allocate the F-TEID,
            *   i.e. when CHOOSE bit is set to "1",
            *   and the IPv4 address and IPv6 address fields are not present.
            */

                    ul_pdr->f_teid.ipv4 = 1;
                    ul_pdr->f_teid.ipv6 = 1;
                    ul_pdr->f_teid.ch = 1;
                    ul_pdr->f_teid.chid = 1;
                    ul_pdr->f_teid.choose_id = OGS_PFCP_DEFAULT_CHOOSE_ID;
                    ul_pdr->f_teid_len = 2;
                } else {
                    ogs_assert(OGS_OK ==
                        ogs_pfcp_sockaddr_to_f_teid(
                            sess->upf_n3_addr, sess->upf_n3_addr6,
                            &ul_pdr->f_teid, &ul_pdr->f_teid_len));
                    ul_pdr->f_teid.teid = sess->upf_n3_teid;
                }

                qos_flow->pcc_rule.id = ogs_strdup(pcc_rule->id);
                ogs_assert(qos_flow->pcc_rule.id);

                memcpy(&qos_flow->qos, &pcc_rule->qos, sizeof(ogs_qos_t));

                qos_flow_created = true;

            } else {
                ogs_assert(strcmp(qos_flow->pcc_rule.id, pcc_rule->id) == 0);

                if ((pcc_rule->qos.mbr.downlink &&
                    qos_flow->qos.mbr.downlink != pcc_rule->qos.mbr.downlink) ||
                    (pcc_rule->qos.mbr.uplink &&
                     qos_flow->qos.mbr.uplink != pcc_rule->qos.mbr.uplink) ||
                    (pcc_rule->qos.gbr.downlink &&
                    qos_flow->qos.gbr.downlink != pcc_rule->qos.gbr.downlink) ||
                    (pcc_rule->qos.gbr.uplink &&
                    qos_flow->qos.gbr.uplink != pcc_rule->qos.gbr.uplink)) {
                    /* Update QoS parameter */
                    memcpy(&qos_flow->qos, &pcc_rule->qos, sizeof(ogs_qos_t));

                    /* Update Bearer Request encodes updated QoS parameter */
                    qos_presence = true;
                }
            }

        /*
         * We only use the method of adding a flow to an existing tft.
         *
         * EPC: OGS_GTP2_TFT_CODE_ADD_PACKET_FILTERS_TO_EXISTING_TFT
         * 5GC: OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS
         */
            ogs_list_init(&qos_flow->pf_to_add_list);

            for (j = 0; j < pcc_rule->num_of_flow; j++) {
                ogs_flow_t *flow = &pcc_rule->flow[j];

                ogs_expect_or_return(flow);
                ogs_expect_or_return(flow->description);

                /*
                 * To add a flow to an existing tft.
                 * duplicated flows are not added
                 */
                if (smf_pf_find_by_flow(
                    qos_flow, flow->direction, flow->description) != NULL) {
                    continue;
                }

                /*
                 * To add a flow to an existing tft.
                 *
                 * Only new flows are added to the PF list.
                 * Then, in the PF list, there are all flows
                 * from the beginning to the present.
                 */
                pf = smf_pf_add(qos_flow);
                if (!pf) {
                    ogs_error("Overflow: PacketFilter in Bearer");
                    break;
                }

                pf->direction = flow->direction;
                pf->flow_description = ogs_strdup(flow->description);
                ogs_assert(pf->flow_description);

                rv = ogs_ipfw_compile_rule(
                        &pf->ipfw_rule, pf->flow_description);
/*
 * Refer to lib/ipfw/ogs-ipfw.h
 * Issue #338
 *
 * <DOWNLINK>
 * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * -->
 * RULE : Source <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> Destination <UE_IP> <UE_PORT>
 *
 * <UPLINK>
 * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * -->
 * RULE : Source <UE_IP> <UE_PORT> Destination <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 */
                if (flow->direction == OGS_FLOW_UPLINK_ONLY)
                    ogs_ipfw_rule_swap(&pf->ipfw_rule);

                if (rv != OGS_OK) {
                    ogs_error("Invalid Flow-Description[%s]",
                            pf->flow_description);
                    smf_pf_remove(pf);
                    break;
                }

                /*
                 * To add a flow to an existing tft.
                 *
                 * 'pf_to_add_list' now has the added flow.
                 */
                ogs_list_add(&qos_flow->pf_to_add_list, &pf->to_add_node);
            }

            if (qos_flow_created == false &&
                qos_presence == false &&
                ogs_list_count(&qos_flow->pf_to_add_list) == 0) {
                ogs_warn("No need to send 'Session Modification Request'");
                ogs_warn("qos_flow_created:%d, qos_presence:%d, rule_count:%d",
                    qos_flow_created, qos_presence,
                    ogs_list_count(&qos_flow->pf_to_add_list));
                continue;
            }

            if (qos_flow_created == true) {
                smf_bearer_tft_update(qos_flow);
                smf_bearer_qos_update(qos_flow);

                pfcp_flags |= OGS_PFCP_MODIFY_CREATE;

                ogs_list_add(&sess->qos_flow_to_modify_list,
                                &qos_flow->to_modify_node);
            } else {
                pfcp_flags |= OGS_PFCP_MODIFY_NETWORK_REQUESTED;

                if (ogs_list_count(&qos_flow->pf_to_add_list) > 0) {
                    pfcp_flags |= OGS_PFCP_MODIFY_TFT_ADD;
                    smf_bearer_tft_update(qos_flow);
                }
                if (qos_presence == true) {
                    pfcp_flags |= OGS_PFCP_MODIFY_QOS_MODIFY;
                    smf_bearer_qos_update(qos_flow);
                }

                ogs_list_add(&sess->qos_flow_to_modify_list,
                                &qos_flow->to_modify_node);
            }
        } else if (pcc_rule->type == OGS_PCC_RULE_TYPE_REMOVE) {
            qos_flow = smf_qos_flow_find_by_pcc_rule_id(sess, pcc_rule->id);

            if (!qos_flow) {
                ogs_warn("No need to send 'Session Modification Request'");
                ogs_warn("  - QosFlow[Id:%s] has already been removed.",
                        pcc_rule->id);
                continue;
            }

            pfcp_flags |= OGS_PFCP_MODIFY_REMOVE;

            ogs_list_add(&sess->qos_flow_to_modify_list,
                            &qos_flow->to_modify_node);

        } else {
            ogs_error("Invalid Type[%d]", pcc_rule->type);
            ogs_assert_if_reached();
        }
    }

    check = pfcp_flags & (OGS_PFCP_MODIFY_CREATE|OGS_PFCP_MODIFY_REMOVE);
    if (check != 0 &&
        check != OGS_PFCP_MODIFY_CREATE && check != OGS_PFCP_MODIFY_REMOVE) {
        ogs_fatal("Invalid flags[%d]", (int)pfcp_flags);
        ogs_assert_if_reached();
    }

    if (ogs_list_count(&sess->qos_flow_to_modify_list)) {
        ogs_assert(OGS_OK ==
                smf_5gc_pfcp_send_qos_flow_list_modification_request(
                    sess, NULL, pfcp_flags, 0));
    }
}
