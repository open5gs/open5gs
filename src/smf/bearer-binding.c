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

#include "bearer-binding.h"
#include "s5c-build.h"
#include "pfcp-path.h"

#include "ipfw/ipfw2.h"

static void bearer_timeout(ogs_gtp_xact_t *xact, void *data)
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
    case OGS_GTP_CREATE_BEARER_REQUEST_TYPE:
        ogs_error("[%s] No Create Bearer Response", smf_ue->imsi_bcd);
        smf_epc_pfcp_send_bearer_modification_request(
                bearer, OGS_PFCP_MODIFY_REMOVE);
        break;
    case OGS_GTP_UPDATE_BEARER_REQUEST_TYPE:
        ogs_error("[%s] No Update Bearer Response", smf_ue->imsi_bcd);
        break;
    case OGS_GTP_DELETE_BEARER_REQUEST_TYPE:
        ogs_error("[%s] No Delete Bearer Response", smf_ue->imsi_bcd);
        smf_epc_pfcp_send_bearer_modification_request(
                bearer, OGS_PFCP_MODIFY_REMOVE);
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
        ogs_gtp_tft_t *tft, smf_bearer_t *bearer)
{
    int i, j, len;
    smf_pf_t *pf = NULL;

    ogs_assert(tft);
    ogs_assert(bearer);

    memset(tft, 0, sizeof(*tft));
    tft->code = OGS_GTP_TFT_CODE_CREATE_NEW_TFT;

    i = 0;
    pf = smf_pf_first(bearer);
    while (pf) {
        tft->pf[i].direction = pf->direction;
        tft->pf[i].identifier = pf->identifier - 1;
        tft->pf[i].precedence = i+1;

        j = 0, len = 0;
        if (pf->ipfw_rule.proto) {
            tft->pf[i].component[j].type =
                GTP_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE;
            tft->pf[i].component[j].proto = pf->ipfw_rule.proto;
            j++; len += 2;
        }

        if (pf->ipfw_rule.ipv4_src) {
            if (pf->direction == OGS_FLOW_DOWNLINK_ONLY)
                tft->pf[i].component[j].type =
                    GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
            else
                tft->pf[i].component[j].type =
                    GTP_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
            tft->pf[i].component[j].ipv4.addr = pf->ipfw_rule.ip.src.addr[0];
            tft->pf[i].component[j].ipv4.mask = pf->ipfw_rule.ip.src.mask[0];
            j++; len += 9;
        }

        if (pf->ipfw_rule.ipv4_dst) {
            if (pf->direction == OGS_FLOW_DOWNLINK_ONLY)
                tft->pf[i].component[j].type =
                    GTP_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
            else
                tft->pf[i].component[j].type =
                    GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;

            tft->pf[i].component[j].ipv4.addr = pf->ipfw_rule.ip.dst.addr[0];
            tft->pf[i].component[j].ipv4.mask = pf->ipfw_rule.ip.dst.mask[0];
            j++; len += 9;
        }

        if (pf->ipfw_rule.ipv6_src) {
            if (pf->direction == OGS_FLOW_DOWNLINK_ONLY)
                tft->pf[i].component[j].type =
                    GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE;
            else
                tft->pf[i].component[j].type =
                    GTP_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE;
            memcpy(tft->pf[i].component[j].ipv6.addr,
                    pf->ipfw_rule.ip.src.addr,
                    sizeof pf->ipfw_rule.ip.src.addr);
            tft->pf[i].component[j].ipv6.prefixlen =
                contigmask((uint8_t *)pf->ipfw_rule.ip.src.mask, 128);
            j++; len += 18;
        }

        if (pf->ipfw_rule.ipv6_dst) {
            if (pf->direction == OGS_FLOW_DOWNLINK_ONLY)
                tft->pf[i].component[j].type =
                    GTP_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE;
            else
                tft->pf[i].component[j].type =
                    GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE;
            memcpy(tft->pf[i].component[j].ipv6.addr,
                    pf->ipfw_rule.ip.dst.addr,
                    sizeof pf->ipfw_rule.ip.dst.addr);
            tft->pf[i].component[j].ipv6.prefixlen =
                contigmask((uint8_t *)pf->ipfw_rule.ip.dst.mask, 128);
            j++; len += 18;
        }

        if (pf->ipfw_rule.port.src.low) {
            if (pf->ipfw_rule.port.src.low == pf->ipfw_rule.port.src.high) {
                if (pf->direction == OGS_FLOW_DOWNLINK_ONLY)
                    tft->pf[i].component[j].type =
                        GTP_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE;
                else
                    tft->pf[i].component[j].type =
                        GTP_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE;
                tft->pf[i].component[j].port.low = pf->ipfw_rule.port.src.low;
                j++; len += 3;
            } else {
                if (pf->direction == OGS_FLOW_DOWNLINK_ONLY)
                    tft->pf[i].component[j].type =
                        GTP_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE;
                else
                    tft->pf[i].component[j].type =
                        GTP_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE;
                tft->pf[i].component[j].port.low = pf->ipfw_rule.port.src.low;
                tft->pf[i].component[j].port.high =
                    pf->ipfw_rule.port.src.high;
                j++; len += 5;
            }
        }

        if (pf->ipfw_rule.port.dst.low) {
            if (pf->ipfw_rule.port.dst.low == pf->ipfw_rule.port.dst.high) {
                if (pf->direction == OGS_FLOW_DOWNLINK_ONLY)
                    tft->pf[i].component[j].type =
                        GTP_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE;
                else
                    tft->pf[i].component[j].type =
                        GTP_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE;
                tft->pf[i].component[j].port.low =
                    pf->ipfw_rule.port.dst.low;
                j++; len += 3;
            } else {
                if (pf->direction == OGS_FLOW_DOWNLINK_ONLY)
                    tft->pf[i].component[j].type =
                        GTP_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE;
                else
                    tft->pf[i].component[j].type =
                        GTP_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE;
                tft->pf[i].component[j].port.low =
                    pf->ipfw_rule.port.dst.low;
                tft->pf[i].component[j].port.high =
                    pf->ipfw_rule.port.dst.high;
                j++; len += 5;
            }
        }

        tft->pf[i].num_of_component = j;
        tft->pf[i].length = len;
        i++;

        pf = smf_pf_next(pf);
    }
    tft->num_of_packet_filter = i;
}

void smf_bearer_binding(smf_sess_t *sess)
{
    int rv;
    int i, j;

    ogs_assert(sess);

    for (i = 0; i < sess->num_of_pcc_rule; i++) {
        ogs_gtp_xact_t *xact = NULL;
        ogs_gtp_header_t h;
        ogs_pkbuf_t *pkbuf = NULL;
        smf_bearer_t *bearer = NULL;

        ogs_pcc_rule_t *pcc_rule = &sess->pcc_rule[i];
        int bearer_created = 0;
        int qos_presence = 0;

        ogs_assert(pcc_rule);
        if (pcc_rule->name == NULL) {
            ogs_error("No PCC Rule Name");
            continue;
        }

        if (pcc_rule->type == OGS_PCC_RULE_TYPE_INSTALL) {
            ogs_pfcp_pdr_t *dl_pdr = NULL, *ul_pdr = NULL;

            bearer = smf_bearer_find_by_qci_arp(sess,
                        pcc_rule->qos.qci,
                        pcc_rule->qos.arp.priority_level,
                        pcc_rule->qos.arp.pre_emption_capability,
                        pcc_rule->qos.arp.pre_emption_vulnerability);
            if (!bearer) {
                if (pcc_rule->num_of_flow == 0) {
                    /* TFT is mandatory in
                     * activate dedicated EPS bearer context request */
                    ogs_error("No flow in PCC Rule");
                    continue;
                }

                bearer = smf_bearer_add(sess);
                ogs_assert(bearer);

                /* Precedence is set to the order in which it was created */
                ogs_assert(bearer->dl_pdr);
                ogs_assert(bearer->ul_pdr);
                bearer->dl_pdr->precedence = bearer->dl_pdr->id;
                bearer->ul_pdr->precedence = bearer->ul_pdr->id;

                bearer->name = ogs_strdup(pcc_rule->name);
                ogs_assert(bearer->name);

                memcpy(&bearer->qos, &pcc_rule->qos, sizeof(ogs_qos_t));

                bearer_created = 1;

            } else {
                ogs_assert(strcmp(bearer->name, pcc_rule->name) == 0);

                if (pcc_rule->num_of_flow) {
                    /* We'll use always 'Create new TFT'.
                     * Therefore, all previous flows are removed
                     * and replaced by the new flow */
                    smf_pf_remove_all(bearer);
                }

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
                    qos_presence = 1;
                }

                if (pcc_rule->num_of_flow == 0 && qos_presence == 0) {
                    ogs_warn("No need to send 'Update Bearer Request'");
                    ogs_warn("  - Both QoS and TFT are same as before");
                    continue;
                }
            }

            dl_pdr = bearer->dl_pdr;
            ogs_assert(dl_pdr);
            ul_pdr = bearer->ul_pdr;
            ogs_assert(ul_pdr);

            dl_pdr->num_of_flow = 0;
            ul_pdr->num_of_flow = 0;

            for (j = 0; j < pcc_rule->num_of_flow; j++) {
                ogs_flow_t *flow = &pcc_rule->flow[j];
                smf_pf_t *pf = NULL;

                ogs_expect_or_return(flow);
                ogs_expect_or_return(flow->description);

                if (flow->direction == OGS_FLOW_DOWNLINK_ONLY) {
                    dl_pdr->flow_description[dl_pdr->num_of_flow++] =
                        flow->description;
                } else if (flow->direction == OGS_FLOW_UPLINK_ONLY) {
                    ul_pdr->flow_description[ul_pdr->num_of_flow++] =
                        flow->description;
                } else {
                    ogs_error("Flow Bidirectional is not supported[%d]",
                            flow->direction);
                }

                pf = smf_pf_add(bearer, pcc_rule->precedence);
                ogs_assert(pf);

                pf->direction = flow->direction;
                pf->flow_description = ogs_strdup(flow->description);

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
            }

            if (bearer_created == 1) {
                /* Setup QER */
                if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
                    bearer->qos.gbr.downlink || bearer->qos.gbr.uplink) {
                    ogs_pfcp_qer_t *qer = NULL;

                    /* Only 1 QER is used per bearer */
                    qer = bearer->qer;
                    if (!qer) {
                        qer = ogs_pfcp_qer_add(&sess->pfcp);
                        ogs_assert(qer);
                        bearer->qer = qer;
                    }

                    ogs_pfcp_pdr_associate_qer(bearer->dl_pdr, qer);
                    ogs_pfcp_pdr_associate_qer(bearer->ul_pdr, qer);

                    qer->mbr.uplink = bearer->qos.mbr.uplink;
                    qer->mbr.downlink = bearer->qos.mbr.downlink;
                    qer->gbr.uplink = bearer->qos.gbr.uplink;
                    qer->gbr.downlink = bearer->qos.gbr.downlink;
                }

                smf_epc_pfcp_send_bearer_modification_request(
                        bearer, OGS_PFCP_MODIFY_CREATE);

            } else {
                ogs_gtp_tft_t tft;

                memset(&tft, 0, sizeof tft);
                if (pcc_rule->num_of_flow)
                    encode_traffic_flow_template(&tft, bearer);

                memset(&h, 0, sizeof(ogs_gtp_header_t));
                h.type = OGS_GTP_UPDATE_BEARER_REQUEST_TYPE;
                h.teid = sess->sgw_s5c_teid;

                pkbuf = smf_s5c_build_update_bearer_request(
                        h.type, bearer,
                        OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
                        pcc_rule->num_of_flow ? &tft : NULL, qos_presence);
                ogs_expect_or_return(pkbuf);

                xact = ogs_gtp_xact_local_create(
                        sess->gnode, &h, pkbuf, bearer_timeout, bearer);
                ogs_expect_or_return(xact);

                if (pcc_rule->num_of_flow)
                    xact->update_flags |= OGS_GTP_MODIFY_TFT_UPDATE;
                if (qos_presence)
                    xact->update_flags |= OGS_GTP_MODIFY_QOS_UPDATE;

                rv = ogs_gtp_xact_commit(xact);
                ogs_expect(rv == OGS_OK);
            }

        } else if (pcc_rule->type == OGS_PCC_RULE_TYPE_REMOVE) {
            bearer = smf_bearer_find_by_name(sess, pcc_rule->name);

            if (!bearer) {
                ogs_warn("No need to send 'Delete Bearer Request'");
                ogs_warn("  - Bearer[Name:%s] has already been removed.",
                        pcc_rule->name);
                continue;
            }

            memset(&h, 0, sizeof(ogs_gtp_header_t));
            h.type = OGS_GTP_DELETE_BEARER_REQUEST_TYPE;
            h.teid = sess->sgw_s5c_teid;

            pkbuf = smf_s5c_build_delete_bearer_request(h.type, bearer,
                    OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED);
            ogs_expect_or_return(pkbuf);

            xact = ogs_gtp_xact_local_create(
                    sess->gnode, &h, pkbuf, bearer_timeout, bearer);
            ogs_expect_or_return(xact);

            rv = ogs_gtp_xact_commit(xact);
            ogs_expect(rv == OGS_OK);
        } else {
            ogs_error("Invalid Type[%d]", pcc_rule->type);
        }
    }
}

void smf_gtp_send_create_bearer_request(smf_bearer_t *bearer)
{
    int rv;

    smf_sess_t *sess = NULL;
    ogs_gtp_xact_t *xact = NULL;

    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_tft_t tft;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    h.type = OGS_GTP_CREATE_BEARER_REQUEST_TYPE;
    h.teid = sess->sgw_s5c_teid;

    memset(&tft, 0, sizeof tft);
    encode_traffic_flow_template(&tft, bearer);

    pkbuf = smf_s5c_build_create_bearer_request(h.type, bearer, &tft);
    ogs_expect_or_return(pkbuf);

    xact = ogs_gtp_xact_local_create(
            sess->gnode, &h, pkbuf, bearer_timeout, bearer);
    ogs_expect_or_return(xact);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}
