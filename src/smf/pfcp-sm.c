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

#include "sbi-path.h"
#include "pfcp-path.h"

#include "n4-handler.h"

static void pfcp_restoration(ogs_pfcp_node_t *node);
static void reselect_upf(ogs_pfcp_node_t *node);
static void node_timeout(ogs_pfcp_xact_t *xact, void *data);

void smf_pfcp_state_initial(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_pfcp_node_t *node = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    node = e->pfcp_node;
    ogs_assert(node);

    node->t_no_heartbeat = ogs_timer_add(ogs_app()->timer_mgr,
            smf_timer_pfcp_no_heartbeat, node);
    ogs_assert(node->t_no_heartbeat);

    OGS_FSM_TRAN(s, &smf_pfcp_state_will_associate);
}

void smf_pfcp_state_final(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_pfcp_node_t *node = NULL;
    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    node = e->pfcp_node;
    ogs_assert(node);

    ogs_timer_delete(node->t_no_heartbeat);
}

void smf_pfcp_state_will_associate(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_pfcp_node_t *node = NULL;
    ogs_pfcp_xact_t *xact = NULL;
    ogs_pfcp_message_t *message = NULL;

    smf_sess_t *sess;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    node = e->pfcp_node;
    ogs_assert(node);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        if (node->t_association) {
            ogs_timer_start(node->t_association,
                    ogs_local_conf()->time.message.pfcp.association_interval);

            ogs_pfcp_cp_send_association_setup_request(node, node_timeout);
        }
        break;

    case OGS_FSM_EXIT_SIG:
        if (node->t_association) {
            ogs_timer_stop(node->t_association);
        }
        break;

    case SMF_EVT_N4_TIMER:
        switch(e->h.timer_id) {
        case SMF_TIMER_PFCP_ASSOCIATION:
            node = e->pfcp_node;
            ogs_assert(node);

            ogs_warn("Retry association with peer failed %s",
                    ogs_sockaddr_to_string_static(node->addr_list));

            ogs_assert(node->t_association);
            ogs_timer_start(node->t_association,
                ogs_local_conf()->time.message.pfcp.association_interval);

            ogs_pfcp_cp_send_association_setup_request(node, node_timeout);
            break;
        case SMF_TIMER_PFCP_NO_ESTABLISHMENT_RESPONSE:
            sess = smf_sess_find_by_id(e->sess_id);
            if (!sess) {
                ogs_warn("Session has already been removed");
                break;
            }
            ogs_fsm_dispatch(&sess->sm, e);
            break;
        case SMF_TIMER_PFCP_NO_DELETION_RESPONSE:
            sess = smf_sess_find_by_id(e->sess_id);
            if (!sess) {
                ogs_warn("Session has already been removed");
                break;
            }
            SMF_SESS_CLEAR(sess);
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    smf_timer_get_name(e->h.timer_id), e->h.timer_id);
            break;
        }
        break;
    case SMF_EVT_N4_MESSAGE:
        message = e->pfcp_message;
        ogs_assert(message);
        xact = ogs_pfcp_xact_find_by_id(e->pfcp_xact_id);
        ogs_assert(xact);

        switch (message->h.type) {
        case OGS_PFCP_HEARTBEAT_REQUEST_TYPE:
            ogs_expect(true ==
                ogs_pfcp_handle_heartbeat_request(node, xact,
                    &message->pfcp_heartbeat_request));
            break;
        case OGS_PFCP_HEARTBEAT_RESPONSE_TYPE:
            ogs_expect(true ==
                ogs_pfcp_handle_heartbeat_response(node, xact,
                    &message->pfcp_heartbeat_response));
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
            ogs_pfcp_cp_handle_association_setup_request(node, xact,
                    &message->pfcp_association_setup_request);
            OGS_FSM_TRAN(s, smf_pfcp_state_associated);
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
            ogs_pfcp_cp_handle_association_setup_response(node, xact,
                    &message->pfcp_association_setup_response);
            OGS_FSM_TRAN(s, smf_pfcp_state_associated);
            break;
        default:
            ogs_warn("cannot handle PFCP message type[%d]",
                    message->h.type);
            break;
        }
        break;
    default:
        ogs_error("Unknown event %s", smf_event_get_name(e));
        break;
    }
}

void smf_pfcp_state_associated(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_pfcp_node_t *node = NULL;
    ogs_pfcp_xact_t *xact = NULL;
    ogs_pfcp_message_t *message = NULL;

    smf_sess_t *sess = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    node = e->pfcp_node;
    ogs_assert(node);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        ogs_info("PFCP associated %s",
                ogs_sockaddr_to_string_static(node->addr_list));
        ogs_timer_start(node->t_no_heartbeat,
                ogs_local_conf()->time.message.pfcp.no_heartbeat_duration);
        ogs_assert(OGS_OK ==
            ogs_pfcp_send_heartbeat_request(node, node_timeout));

        if (node->restoration_required == true) {
            pfcp_restoration(node);
            node->restoration_required = false;
            ogs_error("PFCP restoration");
        }

        smf_metrics_inst_global_inc(SMF_METR_GLOB_GAUGE_PFCP_PEERS_ACTIVE);
        break;
    case OGS_FSM_EXIT_SIG:
        ogs_info("PFCP de-associated %s",
            ogs_sockaddr_to_string_static(node->addr_list));
        ogs_timer_stop(node->t_no_heartbeat);

        smf_metrics_inst_global_dec(SMF_METR_GLOB_GAUGE_PFCP_PEERS_ACTIVE);
        break;
    case SMF_EVT_N4_MESSAGE:
        message = e->pfcp_message;
        ogs_assert(message);
        xact = ogs_pfcp_xact_find_by_id(e->pfcp_xact_id);
        ogs_assert(xact);

        if (message->h.seid_presence && message->h.seid != 0) {
               sess = smf_sess_find_by_seid(message->h.seid);
        } else if (xact->local_seid) { /* rx no SEID or SEID=0 */
            /* 3GPP TS 29.244 7.2.2.4.2: we receive SEID=0 under some
             * conditions, such as cause "Session context not found". In those
             * cases, we still want to identify the local session which
             * originated the message, so try harder by using the SEID we
             * locally stored in xact when sending the original request: */
            sess = smf_sess_find_by_seid(xact->local_seid);
        }
        if (sess)
            e->sess_id = sess->id;

        switch (message->h.type) {
        case OGS_PFCP_HEARTBEAT_REQUEST_TYPE:
            ogs_expect(true ==
                ogs_pfcp_handle_heartbeat_request(node, xact,
                    &message->pfcp_heartbeat_request));
            if (node->restoration_required == true) {
                if (node->t_association) {
        /*
         * node->t_association that the PFCP entity attempts an association.
         *
         * In this case, even if Remote PFCP entity is restarted,
         * PFCP restoration must be performed after PFCP association.
         *
         * Otherwise, Session related PFCP cannot be initiated
         * because the peer PFCP entity is in a de-associated state.
         */
                    OGS_FSM_TRAN(s, smf_pfcp_state_will_associate);
                } else {

        /*
         * If the peer PFCP entity is performing the association,
         * Restoration can be performed immediately.
         */
                    pfcp_restoration(node);
                    node->restoration_required = false;
                    ogs_error("PFCP restoration");
                }
            }
            break;
        case OGS_PFCP_HEARTBEAT_RESPONSE_TYPE:
            ogs_expect(true ==
                ogs_pfcp_handle_heartbeat_response(node, xact,
                    &message->pfcp_heartbeat_response));
            if (node->restoration_required == true) {
        /*
         * node->t_association that the PFCP entity attempts an association.
         *
         * In this case, even if Remote PFCP entity is restarted,
         * PFCP restoration must be performed after PFCP association.
         *
         * Otherwise, Session related PFCP cannot be initiated
         * because the peer PFCP entity is in a de-associated state.
         */
                if (node->t_association) {
                    OGS_FSM_TRAN(s, smf_pfcp_state_will_associate);
                } else {
        /*
         * If the peer PFCP entity is performing the association,
         * Restoration can be performed immediately.
         */
                    pfcp_restoration(node);
                    node->restoration_required = false;
                    ogs_error("PFCP restoration");
                }
            }
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
            ogs_warn("PFCP[REQ] has already been associated %s",
                    ogs_sockaddr_to_string_static(node->addr_list));
            ogs_pfcp_cp_handle_association_setup_request(node, xact,
                    &message->pfcp_association_setup_request);
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
            ogs_warn("PFCP[RSP] has already been associated %s",
                    ogs_sockaddr_to_string_static(node->addr_list));
            ogs_pfcp_cp_handle_association_setup_response(node, xact,
                    &message->pfcp_association_setup_response);
            break;
        case OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
            if (!message->h.seid_presence) ogs_error("No SEID");

            if (!sess) {
                ogs_gtp_xact_t *gtp_xact =
                    ogs_gtp_xact_find_by_id(xact->assoc_xact_id);
                ogs_error("No Session");
                if (!gtp_xact) {
                    ogs_error("No associated GTP transaction");
                    break;
                }
                if (gtp_xact->gtp_version == 1)
                    ogs_gtp1_send_error_message(gtp_xact, 0,
                        OGS_GTP1_CREATE_PDP_CONTEXT_RESPONSE_TYPE,
                        OGS_GTP1_CAUSE_CONTEXT_NOT_FOUND);
                else
                    ogs_gtp2_send_error_message(gtp_xact, 0,
                        OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE,
                        OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND);
                break;
            }
            ogs_fsm_dispatch(&sess->sm, e);
            break;

        case OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE:
            if (!message->h.seid_presence) ogs_error("No SEID");

            if (xact->epc)
                smf_epc_n4_handle_session_modification_response(
                    sess, xact, e->gtp2_message,
                    &message->pfcp_session_modification_response);
            else
                smf_5gc_n4_handle_session_modification_response(
                    sess, xact, &message->pfcp_session_modification_response);
            break;

        case OGS_PFCP_SESSION_DELETION_RESPONSE_TYPE:
            if (!message->h.seid_presence) ogs_error("No SEID");

            if (!sess) {
                ogs_gtp_xact_t *gtp_xact =
                    ogs_gtp_xact_find_by_id(xact->assoc_xact_id);
                ogs_error("No Session");
                if (!gtp_xact) {
                    ogs_error("No associated GTP transaction");
                    break;
                }
                if (gtp_xact->gtp_version == 1)
                    ogs_gtp1_send_error_message(gtp_xact, 0,
                        OGS_GTP1_DELETE_PDP_CONTEXT_RESPONSE_TYPE,
                        OGS_GTP1_CAUSE_CONTEXT_NOT_FOUND);
                else
                    ogs_gtp2_send_error_message(gtp_xact, 0,
                        OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE,
                        OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND);
                break;
            }
            ogs_fsm_dispatch(&sess->sm, e);
            break;

        case OGS_PFCP_SESSION_REPORT_REQUEST_TYPE:
            if (!message->h.seid_presence) ogs_error("No SEID");

            if (!sess) {
                    ogs_error("No Session");
                    ogs_pfcp_send_error_message(xact, 0,
                        OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE,
                        OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND, 0);
                    break;
            }
            ogs_fsm_dispatch(&sess->sm, e);
            break;

        default:
            ogs_error("Not implemented PFCP message type[%d]",
                    message->h.type);
            break;
        }

        break;
    case SMF_EVT_N4_TIMER:
        switch(e->h.timer_id) {
        case SMF_TIMER_PFCP_NO_HEARTBEAT:
            node = e->pfcp_node;
            ogs_assert(node);

            ogs_assert(OGS_OK ==
                ogs_pfcp_send_heartbeat_request(node, node_timeout));
            break;
        case SMF_TIMER_PFCP_NO_ESTABLISHMENT_RESPONSE:
            sess = smf_sess_find_by_id(e->sess_id);
            if (!sess) {
                ogs_warn("Session has already been removed");
                break;
            }
            ogs_fsm_dispatch(&sess->sm, e);
            break;
        case SMF_TIMER_PFCP_NO_DELETION_RESPONSE:
            sess = smf_sess_find_by_id(e->sess_id);
            if (!sess) {
                ogs_warn("Session has already been removed");
                break;
            }
            SMF_SESS_CLEAR(sess);
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    smf_timer_get_name(e->h.timer_id), e->h.timer_id);
            break;
        }
        break;
    case SMF_EVT_N4_NO_HEARTBEAT:
        ogs_warn("No Heartbeat from UPF %s",
                ogs_sockaddr_to_string_static(node->addr_list));

        /*
         * reselect_upf() should not be executed on node_timeout
         * because the timer cannot be deleted in the timer expiration function.
         *
         * Note that reselct_upf contains SMF_SESS_CLEAR.
         */
        node = e->pfcp_node;
        ogs_assert(node);
        reselect_upf(node);

        OGS_FSM_TRAN(s, smf_pfcp_state_will_associate);
        break;
    default:
        ogs_error("Unknown event %s", smf_event_get_name(e));
        break;
    }
}

void smf_pfcp_state_exception(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", smf_event_get_name(e));
        break;
    }
}

static void pfcp_restoration(ogs_pfcp_node_t *node)
{
    smf_ue_t *smf_ue = NULL;

    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];

    ogs_list_for_each(&smf_self()->smf_ue_list, smf_ue) {
        smf_sess_t *sess = NULL;
        ogs_assert(smf_ue);

        ogs_list_for_each(&smf_ue->sess_list, sess) {
            ogs_assert(sess);

            if (node == sess->pfcp_node) {
                if (sess->epc) {
                    ogs_info("UE IMSI[%s] APN[%s] IPv4[%s] IPv6[%s]",
                        smf_ue->imsi_bcd, sess->session.name,
                        sess->ipv4 ?
                            OGS_INET_NTOP(&sess->ipv4->addr, buf1) : "",
                        sess->ipv6 ?
                            OGS_INET6_NTOP(&sess->ipv6->addr, buf2) : "");
                    ogs_assert(OGS_OK ==
                        smf_epc_pfcp_send_session_establishment_request(
                            sess, OGS_INVALID_POOL_ID,
                            OGS_PFCP_CREATE_RESTORATION_INDICATION));
                } else {
                    ogs_info("UE SUPI[%s] DNN[%s] IPv4[%s] IPv6[%s]",
                        smf_ue->supi, sess->session.name,
                        sess->ipv4 ?
                            OGS_INET_NTOP(&sess->ipv4->addr, buf1) : "",
                        sess->ipv6 ?
                            OGS_INET6_NTOP(&sess->ipv6->addr, buf2) : "");
                    ogs_assert(OGS_OK ==
                            smf_5gc_pfcp_send_session_establishment_request(
                                sess, OGS_PFCP_CREATE_RESTORATION_INDICATION));
                }
            }
        }
    }
}

static void reselect_upf(ogs_pfcp_node_t *node)
{
    int r;
    smf_ue_t *smf_ue = NULL;
    ogs_pfcp_node_t *iter = NULL;

    ogs_assert(node);

    if (node->restoration_required == true) {
        ogs_error("UPF has already been restarted");
        return;
    }

    ogs_list_for_each(&ogs_pfcp_self()->pfcp_peer_list, iter) {
        if (iter == node)
            continue;
        if (OGS_FSM_CHECK(&iter->sm, smf_pfcp_state_associated))
            break;
    }

    if (iter == NULL) {
        ogs_error("No UPF available");
        return;
    }

    ogs_list_for_each(&smf_self()->smf_ue_list, smf_ue) {
        smf_sess_t *sess = NULL, *next_sess = NULL;

        ogs_list_for_each_safe(&smf_ue->sess_list, next_sess, sess) {

            if (node == sess->pfcp_node) {
                if (sess->epc) {
                    ogs_error("[%s:%s] EPC restoration is not implemented",
                            smf_ue->imsi_bcd, sess->session.name);
                } else {
                    if (PCF_SM_POLICY_ASSOCIATED(sess)) {
                        smf_npcf_smpolicycontrol_param_t param;

                        ogs_info("[%s:%d] SMF-initiated Deletion",
                                smf_ue->supi, sess->psi);
                        ogs_assert(sess->sm_context_ref);
                        memset(&param, 0, sizeof(param));
                        r = smf_sbi_discover_and_send(
                                OGS_SBI_SERVICE_TYPE_NPCF_SMPOLICYCONTROL, NULL,
                                smf_npcf_smpolicycontrol_build_delete,
                                sess, NULL,
                                OGS_PFCP_DELETE_TRIGGER_SMF_INITIATED,
                                &param);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                    } else {
                        ogs_error("[%s:%d] No PolicyAssociationId. "
                                "Forcibly remove SESSION",
                                smf_ue->supi, sess->psi);
                        SMF_SESS_CLEAR(sess);
                    }
                }
            }
        }
    }
}

static void node_timeout(ogs_pfcp_xact_t *xact, void *data)
{
    int rv;

    smf_event_t *e = NULL;
    uint8_t type;

    ogs_assert(xact);
    type = xact->seq[0].type;

    switch (type) {
    case OGS_PFCP_HEARTBEAT_REQUEST_TYPE:
        ogs_assert(data);

        e = smf_event_new(SMF_EVT_N4_NO_HEARTBEAT);
        e->pfcp_node = data;

        rv = ogs_queue_push(ogs_app()->queue, e);
        if (rv != OGS_OK) {
            ogs_error("ogs_queue_push() failed:%d", (int)rv);
            ogs_event_free(e);
        }
        break;
    case OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
        break;
    default:
        ogs_error("Not implemented [type:%d]", type);
        break;
    }
}
