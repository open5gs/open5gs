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

#include "smf-sm.h"
#include "context.h"
#include "event.h"
#include "gtp-path.h"
#include "fd-path.h"
#include "pfcp-path.h"
#include "s5c-handler.h"
#include "gx-handler.h"

void smf_state_initial(ogs_fsm_t *s, smf_event_t *e)
{
    smf_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &smf_state_operational);
}

void smf_state_final(ogs_fsm_t *s, smf_event_t *e)
{
    smf_sm_debug(e);

    ogs_assert(s);
}

void smf_state_operational(ogs_fsm_t *s, smf_event_t *e)
{
    int rv;
    ogs_pkbuf_t *recvbuf = NULL;
    smf_sess_t *sess = NULL;

    ogs_gtp_node_t *gnode = NULL;
    ogs_gtp_xact_t *gtp_xact = NULL;
    ogs_gtp_message_t gtp_message;

    ogs_diam_gx_message_t *gx_message = NULL;

    ogs_pfcp_node_t *pfcp_node = NULL;
    ogs_pfcp_xact_t *pfcp_xact = NULL;
    ogs_pfcp_message_t pfcp_message;

    smf_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        rv = smf_gtp_open();
        if (rv != OGS_OK) {
            ogs_fatal("Can't establish S11-GTP path");
        }

        rv = smf_pfcp_open();
        if (rv != OGS_OK) {
            ogs_fatal("Can't establish N4-PFCP path");
        }

        ogs_list_for_each(&ogs_pfcp_self()->n4_list, pfcp_node) {
            smf_event_t e;
            e.pfcp_node = pfcp_node;

            ogs_fsm_create(&pfcp_node->sm,
                    smf_pfcp_state_initial, smf_pfcp_state_final);
            ogs_fsm_init(&pfcp_node->sm, &e);
        }
        break;
    case OGS_FSM_EXIT_SIG:
        ogs_list_for_each(&ogs_pfcp_self()->n4_list, pfcp_node) {
            smf_event_t e;
            e.pfcp_node = pfcp_node;

            ogs_fsm_fini(&pfcp_node->sm, &e);
            ogs_fsm_delete(&pfcp_node->sm);
        }

        smf_gtp_close();
        smf_pfcp_close();
        break;
    case SMF_EVT_S5C_MESSAGE:
        ogs_assert(e);
        recvbuf = e->pkbuf;
        ogs_assert(recvbuf);

        if (ogs_gtp_parse_msg(&gtp_message, recvbuf) != OGS_OK) {
            ogs_error("ogs_gtp_parse_msg() failed");
            ogs_pkbuf_free(recvbuf);
            break;
        }

        if (gtp_message.h.teid != 0) {
            sess = smf_sess_find_by_teid(gtp_message.h.teid);
        }

        if (sess) {
            gnode = sess->gnode;
            ogs_assert(gnode);
        } else {
            gnode = e->gnode;
            ogs_assert(gnode);
        }

        rv = ogs_gtp_xact_receive(gnode, &gtp_message.h, &gtp_xact);
        if (rv != OGS_OK) {
            ogs_pkbuf_free(recvbuf);
            break;
        }

        switch(gtp_message.h.type) {
        case OGS_GTP_ECHO_REQUEST_TYPE:
            smf_s5c_handle_echo_request(gtp_xact, &gtp_message.echo_request);
            break;
        case OGS_GTP_ECHO_RESPONSE_TYPE:
            smf_s5c_handle_echo_response(gtp_xact, &gtp_message.echo_response);
            break;
        case OGS_GTP_CREATE_SESSION_REQUEST_TYPE:
            if (gtp_message.h.teid == 0) {
                ogs_expect(!sess);
                sess = smf_sess_add_by_message(&gtp_message);
                if (sess)
                    OGS_SETUP_GTP_NODE(sess, gnode);
            }
            smf_s5c_handle_create_session_request(
                sess, gtp_xact, &gtp_message.create_session_request);
            break;
        case OGS_GTP_DELETE_SESSION_REQUEST_TYPE:
            smf_s5c_handle_delete_session_request(
                sess, gtp_xact, &gtp_message.delete_session_request);
            break;
        case OGS_GTP_CREATE_BEARER_RESPONSE_TYPE:
            smf_s5c_handle_create_bearer_response(
                sess, gtp_xact, &gtp_message.create_bearer_response);
            break;
        case OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE:
            smf_s5c_handle_update_bearer_response(
                sess, gtp_xact, &gtp_message.update_bearer_response);
            break;
        case OGS_GTP_DELETE_BEARER_RESPONSE_TYPE:
            smf_s5c_handle_delete_bearer_response(
                sess, gtp_xact, &gtp_message.delete_bearer_response);
            break;
        case OGS_GTP_BEARER_RESOURCE_COMMAND_TYPE:
            smf_s5c_handle_bearer_resource_command(
                sess, gtp_xact, &gtp_message.bearer_resource_command);
            break;
        default:
            ogs_warn("Not implmeneted(type:%d)", gtp_message.h.type);
            break;
        }
        ogs_pkbuf_free(recvbuf);
        break;

    case SMF_EVT_GX_MESSAGE:
        ogs_assert(e);

        recvbuf = e->pkbuf;
        ogs_assert(recvbuf);
        gx_message = (ogs_diam_gx_message_t *)recvbuf->data;
        ogs_assert(gx_message);

        sess = e->sess;
        ogs_assert(sess);

        switch(gx_message->cmd_code) {
        case OGS_DIAM_GX_CMD_CODE_CREDIT_CONTROL:
            gtp_xact = e->gtp_xact;
            ogs_assert(gtp_xact);

            switch(gx_message->cc_request_type) {
            case OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST:
                smf_gx_handle_cca_initial_request(
                        sess, gx_message, gtp_xact);
                break;
            case OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST:
                smf_gx_handle_cca_termination_request(
                        sess, gx_message, gtp_xact);
                break;
            default:
                ogs_error("Not implemented(%d)",
                        gx_message->cc_request_type);
                break;
            }

            break;
        case OGS_DIAM_GX_CMD_RE_AUTH:
            smf_gx_handle_re_auth_request(sess, gx_message);
            break;
        default:
            ogs_error("Invalid type(%d)", gx_message->cmd_code);
            break;
        }

        ogs_diam_gx_message_free(gx_message);
        ogs_pkbuf_free(recvbuf);
        break;
    case SMF_EVT_N4_MESSAGE:
        ogs_assert(e);
        recvbuf = e->pkbuf;
        ogs_assert(recvbuf);
        pfcp_node = e->pfcp_node;
        ogs_assert(pfcp_node);

        if (ogs_pfcp_parse_msg(&pfcp_message, recvbuf) != OGS_OK) {
            ogs_error("ogs_pfcp_parse_msg() failed");
            ogs_pkbuf_free(recvbuf);
            break;
        }

        rv = ogs_pfcp_xact_receive(pfcp_node, &pfcp_message.h, &pfcp_xact);
        if (rv != OGS_OK) {
            ogs_pkbuf_free(recvbuf);
            break;
        }

        e->pfcp_message = &pfcp_message;
        e->pfcp_xact = pfcp_xact;
        ogs_fsm_dispatch(&pfcp_node->sm, e);
        if (OGS_FSM_CHECK(&pfcp_node->sm, smf_pfcp_state_exception)) {
            ogs_error("PFCP state machine exception");
            break;
        }

        ogs_pkbuf_free(recvbuf);
        break;
    case SMF_EVT_N4_TIMER:
    case SMF_EVT_N4_NO_HEARTBEAT:
        pfcp_node = e->pfcp_node;
        ogs_assert(pfcp_node);
        ogs_assert(OGS_FSM_STATE(&pfcp_node->sm));

        ogs_fsm_dispatch(&pfcp_node->sm, e);
        break;
    default:
        ogs_error("No handler for event %s", smf_event_get_name(e));
        break;
    }
}
