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

#include "sgw-context.h"
#include "sgw-event.h"
#include "sgw-sm.h"
#include "sgw-gtp-path.h"
#include "sgw-s11-handler.h"
#include "sgw-s5c-handler.h"

static void sgw_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp_echo_request_t *req)
{
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[SGW] Receiving Echo Request");
    /* FIXME : Before implementing recovery counter correctly,
     *         I'll re-use the recovery value in request message */
    ogs_gtp_send_echo_response(xact, req->recovery.u8, 0);
}

static void sgw_handle_echo_response(
        ogs_gtp_xact_t *s11_xact, ogs_gtp_echo_response_t *rsp)
{
    /* Not Implemented */
}


void sgw_state_initial(ogs_fsm_t *s, sgw_event_t *e)
{
    sgw_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &sgw_state_operational);
}

void sgw_state_final(ogs_fsm_t *s, sgw_event_t *e)
{
    sgw_sm_debug(e);

    ogs_assert(s);
}

void sgw_state_operational(ogs_fsm_t *s, sgw_event_t *e)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_xact_t *xact = NULL;
    ogs_gtp_message_t message;
    sgw_ue_t *sgw_ue = NULL;
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    ogs_gtp_node_t *gnode = NULL;

    sgw_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        rv = sgw_gtp_open();
        if (rv != OGS_OK) {
            ogs_error("Can't establish SGW path");
            break;
        }
        break;
    case OGS_FSM_EXIT_SIG:
        sgw_gtp_close();
        break;
    case SGW_EVT_S11_MESSAGE:
        ogs_assert(e);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        if (ogs_gtp_parse_msg(&message, pkbuf) != OGS_OK) {
            ogs_error("ogs_gtp_parse_msg() failed");
            ogs_pkbuf_free(pkbuf);
            break;
        }

        if (message.h.teid_presence && message.h.teid != 0) {
            /* Cause is not "Context not found" */
            sgw_ue = sgw_ue_find_by_teid(message.h.teid);
        }

        if (sgw_ue) {
            gnode = sgw_ue->gnode;
            ogs_assert(gnode);
        } else {
            gnode = e->gnode;
            ogs_assert(gnode);
        }

        rv = ogs_gtp_xact_receive(gnode, &message.h, &xact);
        if (rv != OGS_OK) {
            ogs_pkbuf_free(pkbuf);
            break;
        }

        switch(message.h.type) {
        case OGS_GTP_ECHO_REQUEST_TYPE:
            sgw_handle_echo_request(xact, &message.echo_request);
            break;
        case OGS_GTP_ECHO_RESPONSE_TYPE:
            sgw_handle_echo_response(xact, &message.echo_response);
            break;
        case OGS_GTP_CREATE_SESSION_REQUEST_TYPE:
            if (message.h.teid == 0) {
                ogs_expect(!sgw_ue);
                sgw_ue = sgw_ue_add_by_message(&message);
                if (sgw_ue)
                    OGS_SETUP_GTP_NODE(sgw_ue, gnode);
            }
            sgw_s11_handle_create_session_request(xact, sgw_ue,
                    &message);
            break;
        case OGS_GTP_MODIFY_BEARER_REQUEST_TYPE:
            sgw_s11_handle_modify_bearer_request(xact, sgw_ue,
                    &message.modify_bearer_request);
            break;
        case OGS_GTP_DELETE_SESSION_REQUEST_TYPE:
            sgw_s11_handle_delete_session_request(xact, sgw_ue,
                    &message);
            break;
        case OGS_GTP_CREATE_BEARER_RESPONSE_TYPE:
            sgw_s11_handle_create_bearer_response(xact, sgw_ue,
                    &message);
            break;
        case OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE:
            sgw_s11_handle_update_bearer_response(xact, sgw_ue,
                    &message);
            break;
        case OGS_GTP_DELETE_BEARER_RESPONSE_TYPE:
            sgw_s11_handle_delete_bearer_response(xact, sgw_ue,
                    &message);
            break;
        case OGS_GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE:
            sgw_s11_handle_release_access_bearers_request(xact, sgw_ue,
                &message.release_access_bearers_request);
            break;
        case OGS_GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE:
            sgw_s11_handle_downlink_data_notification_ack(xact, sgw_ue,
                &message.downlink_data_notification_acknowledge);
            break;
        case OGS_GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
            sgw_s11_handle_create_indirect_data_forwarding_tunnel_request(
                xact, sgw_ue,
                &message.create_indirect_data_forwarding_tunnel_request);
            break;
        case OGS_GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
            sgw_s11_handle_delete_indirect_data_forwarding_tunnel_request(
                xact, sgw_ue);
            break;
        case OGS_GTP_BEARER_RESOURCE_COMMAND_TYPE:
            sgw_s11_handle_bearer_resource_command(xact, sgw_ue, &message);
            break;
        default:
            ogs_warn("Not implemented(type:%d)", message.h.type);
            break;
        }
        ogs_pkbuf_free(pkbuf);
        break;

    case SGW_EVT_S5C_MESSAGE:
        ogs_assert(e);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        if (ogs_gtp_parse_msg(&message, pkbuf) != OGS_OK) {
            ogs_error("ogs_gtp_parse_msg() failed");
            ogs_pkbuf_free(pkbuf);
            break;
        }

        if (message.h.teid_presence && message.h.teid != 0) {
            sess = sgw_sess_find_by_teid(message.h.teid);
        }

        if (sess) {
            gnode = sess->gnode;
            ogs_assert(gnode);
        } else {
            gnode = e->gnode;
            ogs_assert(gnode);
        }

        rv = ogs_gtp_xact_receive(gnode, &message.h, &xact);
        if (rv != OGS_OK) {
            ogs_pkbuf_free(pkbuf);
            break;
        }

        switch(message.h.type) {
        case OGS_GTP_ECHO_REQUEST_TYPE:
            sgw_handle_echo_request(xact, &message.echo_request);
            break;
        case OGS_GTP_ECHO_RESPONSE_TYPE:
            sgw_handle_echo_response(xact, &message.echo_response);
            break;
        case OGS_GTP_CREATE_SESSION_RESPONSE_TYPE:
            sgw_s5c_handle_create_session_response(xact, sess,
                    &message);
            break;
        case OGS_GTP_DELETE_SESSION_RESPONSE_TYPE:
            sgw_s5c_handle_delete_session_response(xact, sess,
                    &message);
            break;
        case OGS_GTP_CREATE_BEARER_REQUEST_TYPE:
            sgw_s5c_handle_create_bearer_request(xact, sess,
                    &message);
            break;
        case OGS_GTP_UPDATE_BEARER_REQUEST_TYPE:
            sgw_s5c_handle_update_bearer_request(xact, sess,
                    &message);
            break;
        case OGS_GTP_DELETE_BEARER_REQUEST_TYPE:
            sgw_s5c_handle_delete_bearer_request(xact, sess,
                    &message);
            break;
        case OGS_GTP_BEARER_RESOURCE_FAILURE_INDICATION_TYPE:
            sgw_s5c_handle_bearer_resource_failure_indication(xact, sess,
                    &message);
            break;
        default:
            ogs_warn("Not implmeneted(type:%d)", message.h.type);
            break;
        }
        ogs_pkbuf_free(pkbuf);
        break;
    case SGW_EVT_LO_DLDATA_NOTI:
        ogs_assert(e);

        bearer = e->bearer;
        ogs_assert(bearer);

        sgw_s11_handle_lo_dldata_notification(bearer);

        break;
    default:
        ogs_error("No handler for event %s", sgw_event_get_name(e));
        break;
    }
}
