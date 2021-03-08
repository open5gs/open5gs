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
        if (!smf_bearer_cycle(bearer)) {
            ogs_warn("[%s] Bearer has already been removed", smf_ue->imsi_bcd);
            break;
        }
        smf_epc_pfcp_send_bearer_modification_request(
                bearer, OGS_PFCP_MODIFY_REMOVE);
        break;
    case OGS_GTP_UPDATE_BEARER_REQUEST_TYPE:
        ogs_error("[%s] No Update Bearer Response", smf_ue->imsi_bcd);
        break;
    case OGS_GTP_DELETE_BEARER_REQUEST_TYPE:
        ogs_error("[%s] No Delete Bearer Response", smf_ue->imsi_bcd);
        if (!smf_bearer_cycle(bearer)) {
            ogs_warn("[%s] Bearer has already been removed", smf_ue->imsi_bcd);
            break;
        }
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
    int i;
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

        ogs_pf_content_from_ipfw_rule(
                pf->direction, &tft->pf[i].content, &pf->ipfw_rule);

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
                        pcc_rule->qos.index,
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

                bearer->pcc_rule.name = ogs_strdup(pcc_rule->name);
                ogs_assert(bearer->pcc_rule.name);

                memcpy(&bearer->qos, &pcc_rule->qos, sizeof(ogs_qos_t));

                bearer_created = 1;

            } else {
                ogs_assert(strcmp(bearer->pcc_rule.name, pcc_rule->name) == 0);

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

                pf = smf_pf_add(bearer);
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
            bearer = smf_bearer_find_by_pcc_rule_name(sess, pcc_rule->name);

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

void smf_qos_flow_binding(smf_sess_t *sess, ogs_sbi_stream_t *stream)
{
    int rv;
    int i, j;

    ogs_assert(sess);

    for (i = 0; i < sess->num_of_pcc_rule; i++) {
#if 0
        ogs_gtp_header_t h;
        ogs_pkbuf_t *pkbuf = NULL;
#endif
        smf_bearer_t *qos_flow = NULL;

        ogs_pcc_rule_t *pcc_rule = &sess->pcc_rule[i];
        int qos_flow_created = 0;
        int qos_presence = 0;

        ogs_assert(pcc_rule);
        if (pcc_rule->id == NULL) {
            ogs_error("No PCC Rule Id");
            continue;
        }

        if (pcc_rule->type == OGS_PCC_RULE_TYPE_INSTALL) {
            ogs_pfcp_pdr_t *dl_pdr = NULL, *ul_pdr = NULL;

            qos_flow = smf_bearer_find_by_qci_arp(sess,
                        pcc_rule->qos.index,
                        pcc_rule->qos.arp.priority_level,
                        pcc_rule->qos.arp.pre_emption_capability,
                        pcc_rule->qos.arp.pre_emption_vulnerability);
            if (!qos_flow) {
                if (pcc_rule->num_of_flow == 0) {
                    /* TFT is mandatory in
                     * activate dedicated EPS bearer context request */
                    ogs_error("No flow in PCC Rule");
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
                    ul_pdr->f_teid.ch = 1;
                    ul_pdr->f_teid.chid = 1;
                    ul_pdr->f_teid.choose_id = OGS_PFCP_DEFAULT_CHOOSE_ID;
                    ul_pdr->f_teid_len = 2;
                } else {
                    ogs_assert(sess->upf_n3_addr || sess->upf_n3_addr6);
                    ogs_pfcp_sockaddr_to_f_teid(
                            sess->upf_n3_addr, sess->upf_n3_addr6,
                            &ul_pdr->f_teid, &ul_pdr->f_teid_len);
                    ul_pdr->f_teid.teid = sess->upf_n3_teid;
                }

                qos_flow->pcc_rule.id = ogs_strdup(pcc_rule->id);
                ogs_assert(qos_flow->pcc_rule.id);

                memcpy(&qos_flow->qos, &pcc_rule->qos, sizeof(ogs_qos_t));

                qos_flow_created = 1;

            } else {
                ogs_assert(strcmp(qos_flow->pcc_rule.id, pcc_rule->id) == 0);

                if (pcc_rule->num_of_flow) {
                    /* We'll use always 'Create new TFT'.
                     * Therefore, all previous flows are removed
                     * and replaced by the new flow */
                    smf_pf_remove_all(qos_flow);
                }

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
                    qos_presence = 1;
                }

                if (pcc_rule->num_of_flow == 0 && qos_presence == 0) {
                    ogs_warn("No need to send 'Session Modification Request'");
                    ogs_warn("  - Both QoS and TFT are same as before");
                    continue;
                }
            }

            dl_pdr = qos_flow->dl_pdr;
            ogs_assert(dl_pdr);
            ul_pdr = qos_flow->ul_pdr;
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

                pf = smf_pf_add(qos_flow);
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

            if (qos_flow_created == 1) {
                /* Setup QER */
                if (qos_flow->qos.mbr.downlink || qos_flow->qos.mbr.uplink ||
                    qos_flow->qos.gbr.downlink || qos_flow->qos.gbr.uplink) {
                    ogs_pfcp_qer_t *qer = NULL;

                    /* Only 1 QER is used per qos_flow */
                    qer = qos_flow->qer;
                    if (!qer) {
                        qer = ogs_pfcp_qer_add(&sess->pfcp);
                        ogs_assert(qer);
                        qos_flow->qer = qer;
                    }

                    ogs_pfcp_pdr_associate_qer(qos_flow->dl_pdr, qer);
                    ogs_pfcp_pdr_associate_qer(qos_flow->ul_pdr, qer);

                    qer->mbr.uplink = qos_flow->qos.mbr.uplink;
                    qer->mbr.downlink = qos_flow->qos.mbr.downlink;
                    qer->gbr.uplink = qos_flow->qos.gbr.uplink;
                    qer->gbr.downlink = qos_flow->qos.gbr.downlink;
                }

                smf_5gc_pfcp_send_qos_flow_modification_request(
                        qos_flow, NULL, OGS_PFCP_MODIFY_CREATE);

            } else {
                ogs_fatal("Update Qos Flow Not Implemented");
                ogs_assert_if_reached();
#if 0
                ogs_gtp_tft_t tft;

                memset(&tft, 0, sizeof tft);
                if (pcc_rule->num_of_flow)
                    encode_traffic_flow_template(&tft, qos_flow);

                memset(&h, 0, sizeof(ogs_gtp_header_t));
                h.type = OGS_GTP_UPDATE_BEARER_REQUEST_TYPE;
                h.teid = sess->sgw_s5c_teid;

                pkbuf = smf_s5c_build_update_qos_flow_request(
                        h.type, qos_flow,
                        OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
                        pcc_rule->num_of_flow ? &tft : NULL, qos_presence);
                ogs_expect_or_return(pkbuf);

                xact = ogs_gtp_xact_local_create(
                        sess->gnode, &h, pkbuf, qos_flow_timeout, qos_flow);
                ogs_expect_or_return(xact);

                if (pcc_rule->num_of_flow)
                    xact->update_flags |= OGS_GTP_MODIFY_TFT_UPDATE;
                if (qos_presence)
                    xact->update_flags |= OGS_GTP_MODIFY_QOS_UPDATE;

                rv = ogs_gtp_xact_commit(xact);
                ogs_expect(rv == OGS_OK);
#endif
            }
        } else if (pcc_rule->type == OGS_PCC_RULE_TYPE_REMOVE) {
            ogs_fatal("Remove Type Not Implemented");
            ogs_assert_if_reached();
#if 0
            qos_flow = smf_qos_flow_find_by_name(sess, pcc_rule->name);

            if (!qos_flow) {
                ogs_warn("No need to send 'Delete Bearer Request'");
                ogs_warn("  - Bearer[Name:%s] has already been removed.",
                        pcc_rule->name);
                continue;
            }

            memset(&h, 0, sizeof(ogs_gtp_header_t));
            h.type = OGS_GTP_DELETE_BEARER_REQUEST_TYPE;
            h.teid = sess->sgw_s5c_teid;

            pkbuf = smf_s5c_build_delete_qos_flow_request(h.type, qos_flow,
                    OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED);
            ogs_expect_or_return(pkbuf);

            xact = ogs_gtp_xact_local_create(
                    sess->gnode, &h, pkbuf, qos_flow_timeout, qos_flow);
            ogs_expect_or_return(xact);

            rv = ogs_gtp_xact_commit(xact);
            ogs_expect(rv == OGS_OK);
#endif
        } else {
            ogs_error("Invalid Type[%d]", pcc_rule->type);
            ogs_assert_if_reached();
        }
    }
}
