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

#include "pgw-sm.h"
#include "pgw-context.h"
#include "pgw-event.h"
#include "pgw-gtp-path.h"
#include "pgw-s5c-handler.h"
#include "pgw-gx-handler.h"
#include "pgw-fd-path.h"

void pgw_state_initial(ogs_fsm_t *s, pgw_event_t *e)
{
    pgw_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &pgw_state_operational);
}

void pgw_state_final(ogs_fsm_t *s, pgw_event_t *e)
{
    pgw_sm_debug(e);

    ogs_assert(s);
}

void pgw_state_operational(ogs_fsm_t *s, pgw_event_t *e)
{
    int rv;
    ogs_pkbuf_t *recvbuf = NULL;
    ogs_gtp_xact_t *xact = NULL;
    ogs_gtp_message_t gtp_message;
    pgw_sess_t *sess = NULL;
    ogs_diam_gx_message_t *gx_message = NULL;
    ogs_gtp_node_t *gnode = NULL;

    pgw_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        rv = pgw_gtp_open();
        if (rv != OGS_OK)
            ogs_fatal("Can't establish PGW path");
        break;
    case OGS_FSM_EXIT_SIG:
        pgw_gtp_close();
        break;
    case PGW_EVT_S5C_MESSAGE:
        ogs_assert(e);
        recvbuf = e->pkbuf;
        ogs_assert(recvbuf);

        if (ogs_gtp_parse_msg(&gtp_message, recvbuf) != OGS_OK) {
            ogs_error("ogs_gtp_parse_msg() failed");
            ogs_pkbuf_free(recvbuf);
            break;
        }

        /*
         * 5.5.2 in spec 29.274
         *
         * If a peer's TEID is not available, the TEID field still shall be
         * present in the header and its value shall be set to "0" in the
         * following messages:
         *
         * - Create Session Request message on S2a/S2b/S5/S8
         *
         * - Create Session Request message on S4/S11, if for a given UE,
         *   the SGSN/MME has not yet obtained the Control TEID of the SGW.
         *
         * - If a node receives a message and the TEID-C in the GTPv2 header of
         *   the received message is not known, it shall respond with
         *   "Context not found" Cause in the corresponding response message
         *   to the sender, the TEID used in the GTPv2-C header in the response
         *   message shall be then set to zero.
         *
         * - If a node receives a request message containing protocol error,
         *   e.g. Mandatory IE missing, which requires the receiver to reject
         *   the message as specified in clause 7.7, it shall reject
         *   the request message. For the response message, the node should
         *   look up the remote peer's TEID and accordingly set the GTPv2-C
         *   header TEID and the message cause code. As an implementation
         *   option, the node may not look up the remote peer's TEID and
         *   set the GTPv2-C header TEID to zero in the response message.
         *   However in this case, the cause code shall not be set to
         *   "Context not found".
         */
        if (gtp_message.h.teid_presence && gtp_message.h.teid != 0) {
            /* Cause is not "Context not found" */
            sess = pgw_sess_find_by_teid(gtp_message.h.teid);
        }

        if (sess) {
            gnode = sess->gnode;
            ogs_assert(gnode);
        } else {
            gnode = e->gnode;
            ogs_assert(gnode);
        }

        rv = ogs_gtp_xact_receive(gnode, &gtp_message.h, &xact);
        if (rv != OGS_OK) {
            ogs_pkbuf_free(recvbuf);
            break;
        }

        switch(gtp_message.h.type) {
        case OGS_GTP_ECHO_REQUEST_TYPE:
            pgw_s5c_handle_echo_request(xact, &gtp_message.echo_request);
            break;
        case OGS_GTP_ECHO_RESPONSE_TYPE:
            pgw_s5c_handle_echo_response(xact, &gtp_message.echo_response);
            break;
        case OGS_GTP_CREATE_SESSION_REQUEST_TYPE:
            if (gtp_message.h.teid == 0) {
                ogs_expect(!sess);
                sess = pgw_sess_add_by_message(&gtp_message);
                if (sess)
                    OGS_SETUP_GTP_NODE(sess, gnode);
            }
            pgw_s5c_handle_create_session_request(
                sess, xact, &gtp_message.create_session_request);
            break;
        case OGS_GTP_DELETE_SESSION_REQUEST_TYPE:
            pgw_s5c_handle_delete_session_request(
                sess, xact, &gtp_message.delete_session_request);
            break;
        case OGS_GTP_CREATE_BEARER_RESPONSE_TYPE:
            pgw_s5c_handle_create_bearer_response(
                sess, xact, &gtp_message.create_bearer_response);
            break;
        case OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE:
            pgw_s5c_handle_update_bearer_response(
                sess, xact, &gtp_message.update_bearer_response);
            break;
        case OGS_GTP_DELETE_BEARER_RESPONSE_TYPE:
            pgw_s5c_handle_delete_bearer_response(
                sess, xact, &gtp_message.delete_bearer_response);
            break;
        case OGS_GTP_BEARER_RESOURCE_COMMAND_TYPE:
            pgw_s5c_handle_bearer_resource_command(
                sess, xact, &gtp_message.bearer_resource_command);
            break;
        default:
            ogs_warn("Not implmeneted(type:%d)", gtp_message.h.type);
            break;
        }
        ogs_pkbuf_free(recvbuf);
        break;

    case PGW_EVT_GX_MESSAGE:
        ogs_assert(e);

        recvbuf = e->pkbuf;
        ogs_assert(recvbuf);
        gx_message = (ogs_diam_gx_message_t *)recvbuf->data;
        ogs_assert(gx_message);

        sess = e->sess;
        ogs_assert(sess);

        switch(gx_message->cmd_code) {
        case OGS_DIAM_GX_CMD_CODE_CREDIT_CONTROL:
            xact = e->xact;
            ogs_assert(xact);

            switch(gx_message->cc_request_type) {
            case OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST:
                pgw_gx_handle_cca_initial_request(
                        sess, gx_message, xact);
                break;
            case OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST:
                pgw_gx_handle_cca_termination_request(
                        sess, gx_message, xact);
                break;
            default:
                ogs_error("Not implemented(%d)",
                        gx_message->cc_request_type);
                break;
            }
            break;
        case OGS_DIAM_GX_CMD_RE_AUTH:
            pgw_gx_handle_re_auth_request(sess, gx_message);
            break;
        default:
            ogs_error("Invalid type(%d)", gx_message->cmd_code);
            break;
        }

        ogs_diam_gx_message_free(gx_message);
        ogs_pkbuf_free(recvbuf);
        break;
    default:
        ogs_error("No handler for event %s", pgw_event_get_name(e));
        break;
    }
}
