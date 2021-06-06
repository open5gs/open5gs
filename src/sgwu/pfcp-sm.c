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

#include "pfcp-path.h"
#include "sxa-handler.h"

static void node_timeout(ogs_pfcp_xact_t *xact, void *data);

void sgwu_pfcp_state_initial(ogs_fsm_t *s, sgwu_event_t *e)
{
    int rv;
    ogs_pfcp_node_t *node = NULL;

    ogs_assert(s);
    ogs_assert(e);

    sgwu_sm_debug(e);

    node = e->pfcp_node;
    ogs_assert(node);

    rv = ogs_pfcp_connect(
            ogs_pfcp_self()->pfcp_sock, ogs_pfcp_self()->pfcp_sock6, node);
    ogs_assert(rv == OGS_OK);

    node->t_no_heartbeat = ogs_timer_add(ogs_app()->timer_mgr,
            sgwu_timer_no_heartbeat, node);
    ogs_assert(node->t_no_heartbeat);

    OGS_FSM_TRAN(s, &sgwu_pfcp_state_will_associate);
}

void sgwu_pfcp_state_final(ogs_fsm_t *s, sgwu_event_t *e)
{
    ogs_pfcp_node_t *node = NULL;
    ogs_assert(s);
    ogs_assert(e);

    sgwu_sm_debug(e);

    node = e->pfcp_node;
    ogs_assert(node);

    ogs_timer_delete(node->t_no_heartbeat);
}

void sgwu_pfcp_state_will_associate(ogs_fsm_t *s, sgwu_event_t *e)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_pfcp_node_t *node = NULL;
    ogs_pfcp_xact_t *xact = NULL;
    ogs_pfcp_message_t *message = NULL;
    ogs_sockaddr_t *addr = NULL;
    ogs_assert(s);
    ogs_assert(e);

    sgwu_sm_debug(e);

    node = e->pfcp_node;
    ogs_assert(node);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        if (node->t_association) {
            ogs_timer_start(node->t_association,
                ogs_app()->time.message.pfcp.association_interval);

            ogs_pfcp_up_send_association_setup_request(node, node_timeout);
        }
        break;

    case OGS_FSM_EXIT_SIG:
        if (node->t_association) {
            ogs_timer_stop(node->t_association);
        }
        break;

    case SGWU_EVT_SXA_TIMER:
        switch(e->timer_id) {
        case SGWU_TIMER_ASSOCIATION:
            addr = node->sa_list;
            ogs_assert(addr);

            ogs_warn("Retry to association with peer [%s]:%d failed",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));

            ogs_assert(node->t_association);
            ogs_timer_start(node->t_association,
                ogs_app()->time.message.pfcp.association_interval);

            ogs_pfcp_up_send_association_setup_request(node, node_timeout);
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    sgwu_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    case SGWU_EVT_SXA_MESSAGE:
        message = e->pfcp_message;
        ogs_assert(message);
        xact = e->pfcp_xact;
        ogs_assert(xact);

        switch (message->h.type) {
        case OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
            ogs_pfcp_up_handle_association_setup_request(node, xact,
                    &message->pfcp_association_setup_request);
            OGS_FSM_TRAN(s, sgwu_pfcp_state_associated);
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
            ogs_pfcp_up_handle_association_setup_response(node, xact,
                    &message->pfcp_association_setup_response);
            OGS_FSM_TRAN(s, sgwu_pfcp_state_associated);
            break;
        default:
            ogs_warn("cannot handle PFCP message type[%d]",
                    message->h.type);
            break;
        }
        break;
    default:
        ogs_error("Unknown event %s", sgwu_event_get_name(e));
        break;
    }
}

void sgwu_pfcp_state_associated(ogs_fsm_t *s, sgwu_event_t *e)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_pfcp_node_t *node = NULL;
    ogs_pfcp_xact_t *xact = NULL;
    ogs_pfcp_message_t *message = NULL;

    ogs_sockaddr_t *addr = NULL;
    sgwu_sess_t *sess = NULL;

    ogs_assert(s);
    ogs_assert(e);

    sgwu_sm_debug(e);

    node = e->pfcp_node;
    ogs_assert(node);
    addr = node->sa_list;
    ogs_assert(addr);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        ogs_info("PFCP associated");
        ogs_timer_start(node->t_no_heartbeat,
                ogs_app()->time.message.pfcp.no_heartbeat_duration);
        break;
    case OGS_FSM_EXIT_SIG:
        ogs_info("PFCP de-associated");
        ogs_timer_stop(node->t_no_heartbeat);
        break;
    case SGWU_EVT_SXA_MESSAGE:
        message = e->pfcp_message;
        ogs_assert(message);
        xact = e->pfcp_xact;
        ogs_assert(xact);

        if (message->h.seid_presence && message->h.seid != 0)
            sess = sgwu_sess_find_by_up_seid(message->h.seid);

        switch (message->h.type) {
        case OGS_PFCP_HEARTBEAT_REQUEST_TYPE:
            ogs_assert(true ==
                ogs_pfcp_handle_heartbeat_request(node, xact,
                    &message->pfcp_heartbeat_request));
            break;
        case OGS_PFCP_HEARTBEAT_RESPONSE_TYPE:
            ogs_assert(true ==
                ogs_pfcp_handle_heartbeat_response(node, xact,
                    &message->pfcp_heartbeat_response));
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
            ogs_warn("PFCP[REQ] has already been associated");
            ogs_pfcp_up_handle_association_setup_request(node, xact,
                    &message->pfcp_association_setup_request);
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
            ogs_warn("PFCP[RSP] has already been associated");
            ogs_pfcp_up_handle_association_setup_response(node, xact,
                    &message->pfcp_association_setup_response);
            break;
        case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
            if (message->h.seid_presence && message->h.seid == 0) {
                ogs_expect(!sess);
                sess = sgwu_sess_add_by_message(message);
                if (sess)
                    OGS_SETUP_PFCP_NODE(sess, node);
            }
            sgwu_sxa_handle_session_establishment_request(
                sess, xact, &message->pfcp_session_establishment_request);
            break;
        case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
            sgwu_sxa_handle_session_modification_request(
                sess, xact, &message->pfcp_session_modification_request);
            break;
        case OGS_PFCP_SESSION_DELETION_REQUEST_TYPE:
            sgwu_sxa_handle_session_deletion_request(
                sess, xact, &message->pfcp_session_deletion_request);
            break;
        case OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE:
            sgwu_sxa_handle_session_report_response(
                sess, xact, &message->pfcp_session_report_response);
            break;
        default:
            ogs_error("Not implemented PFCP message type[%d]",
                    message->h.type);
            break;
        }

        break;
    case SGWU_EVT_SXA_TIMER:
        switch(e->timer_id) {
        case SGWU_TIMER_NO_HEARTBEAT:
            node = e->pfcp_node;
            ogs_assert(node);

            ogs_assert(OGS_OK ==
                ogs_pfcp_send_heartbeat_request(node, node_timeout));
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    sgwu_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    case SGWU_EVT_SXA_NO_HEARTBEAT:
        ogs_warn("No Heartbeat from SGW-C [%s]:%d",
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
        OGS_FSM_TRAN(s, sgwu_pfcp_state_will_associate);
        break;
    default:
        ogs_error("Unknown event %s", sgwu_event_get_name(e));
        break;
    }
}

void sgwu_pfcp_state_exception(ogs_fsm_t *s, sgwu_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    sgwu_sm_debug(e);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", sgwu_event_get_name(e));
        break;
    }
}

static void node_timeout(ogs_pfcp_xact_t *xact, void *data)
{
    int rv;

    sgwu_event_t *e = NULL;
    uint8_t type;

    ogs_assert(xact);
    type = xact->seq[0].type;

    switch (type) {
    case OGS_PFCP_HEARTBEAT_REQUEST_TYPE:
        ogs_assert(data);

        e = sgwu_event_new(SGWU_EVT_SXA_NO_HEARTBEAT);
        e->pfcp_node = data;

        rv = ogs_queue_push(ogs_app()->queue, e);
        if (rv != OGS_OK) {
            ogs_warn("ogs_queue_push() failed:%d", (int)rv);
            sgwu_event_free(e);
        }
        break;
    case OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
        break;
    default:
        ogs_error("Not implemented [type:%d]", type);
        break;
    }
}
