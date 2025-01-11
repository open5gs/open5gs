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

#include "pfcp-path.h"

static void pfcp_node_fsm_init(ogs_pfcp_node_t *node, bool try_to_assoicate)
{
    sgwc_event_t e;

    ogs_assert(node);

    memset(&e, 0, sizeof(e));
    e.pfcp_node = node;

    if (try_to_assoicate == true) {
        node->t_association = ogs_timer_add(ogs_app()->timer_mgr,
                sgwc_timer_pfcp_association, node);
        ogs_assert(node->t_association);
    }

    ogs_fsm_init(&node->sm, sgwc_pfcp_state_initial, sgwc_pfcp_state_final, &e);
}

static void pfcp_node_fsm_fini(ogs_pfcp_node_t *node)
{
    sgwc_event_t e;

    ogs_assert(node);

    memset(&e, 0, sizeof(e));
    e.pfcp_node = node;

    ogs_fsm_fini(&node->sm, &e);

    if (node->t_association)
        ogs_timer_delete(node->t_association);
}

static void pfcp_recv_cb(short when, ogs_socket_t fd, void *data)
{
    int rv;

    ssize_t size;
    sgwc_event_t *e = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_sockaddr_t from;
    ogs_pfcp_node_t *node = NULL;
    ogs_pfcp_message_t *message = NULL;
    ogs_pfcp_header_t *h = NULL;

    ogs_pfcp_status_e pfcp_status;;
    ogs_pfcp_node_id_t node_id;

    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);

    size = ogs_recvfrom(fd, pkbuf->data, pkbuf->len, 0, &from);
    if (size <= 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_recvfrom() failed");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    ogs_pkbuf_trim(pkbuf, size);

    h = (ogs_pfcp_header_t *)pkbuf->data;
    if (h->version != OGS_PFCP_VERSION) {
        ogs_pfcp_header_t rsp;

        ogs_error("Not supported version[%d]", h->version);

        memset(&rsp, 0, sizeof rsp);
        rsp.flags = (OGS_PFCP_VERSION << 5);
        rsp.type = OGS_PFCP_VERSION_NOT_SUPPORTED_RESPONSE_TYPE;
        rsp.length = htobe16(4);
        rsp.sqn_only = h->sqn_only;
        if (ogs_sendto(fd, &rsp, 8, 0, &from) < 0) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                    "ogs_sendto() failed");
        }
        ogs_pkbuf_free(pkbuf);

        return;
    }

    e = sgwc_event_new(SGWC_EVT_SXA_MESSAGE);
    ogs_assert(e);

    /*
     * Issue #1911
     *
     * Because ogs_pfcp_message_t is over 80kb in size,
     * it can cause stack overflow.
     * To avoid this, the pfcp_message structure uses heap memory.
     */
    if ((message = ogs_pfcp_parse_msg(pkbuf)) == NULL) {
        ogs_error("ogs_pfcp_parse_msg() failed");
        ogs_pkbuf_free(pkbuf);
        sgwc_event_free(e);
        return;
    }

    pfcp_status = ogs_pfcp_extract_node_id(message, &node_id);
    switch (pfcp_status) {
    case OGS_PFCP_STATUS_SUCCESS:
    case OGS_PFCP_STATUS_NODE_ID_NONE:
    case OGS_PFCP_STATUS_NODE_ID_OPTIONAL_ABSENT:
        ogs_debug("ogs_pfcp_extract_node_id() "
                "type [%d] pfcp_status [%d] node_id [%s] from %s",
                message->h.type, pfcp_status,
                pfcp_status == OGS_PFCP_STATUS_SUCCESS ?
                    ogs_pfcp_node_id_to_string_static(&node_id) :
                    "NULL",
                ogs_sockaddr_to_string_static(&from));
        break;

    case OGS_PFCP_ERROR_SEMANTIC_INCORRECT_MESSAGE:
    case OGS_PFCP_ERROR_NODE_ID_NOT_PRESENT:
    case OGS_PFCP_ERROR_NODE_ID_NOT_FOUND:
    case OGS_PFCP_ERROR_UNKNOWN_MESSAGE:
        ogs_error("ogs_pfcp_extract_node_id() failed "
                "type [%d] pfcp_status [%d] from %s",
                message->h.type, pfcp_status,
                ogs_sockaddr_to_string_static(&from));
        goto cleanup;

    default:
        ogs_error("Unexpected pfcp_status "
                "type [%d] pfcp_status [%d] from %s",
                message->h.type, pfcp_status,
                ogs_sockaddr_to_string_static(&from));
        goto cleanup;
    }

    node = ogs_pfcp_node_find(&ogs_pfcp_self()->pfcp_peer_list,
            pfcp_status == OGS_PFCP_STATUS_SUCCESS ? &node_id : NULL, &from);
    if (!node) {
        if (message->h.type == OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE ||
            message->h.type == OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE) {
            ogs_assert(pfcp_status == OGS_PFCP_STATUS_SUCCESS);
            node = ogs_pfcp_node_add(&ogs_pfcp_self()->pfcp_peer_list,
                    &node_id, &from);
            if (!node) {
                ogs_error("No memory: ogs_pfcp_node_add() failed");
                goto cleanup;
            }
            ogs_debug("Added PFCP-Node: addr_list %s",
                    ogs_sockaddr_to_string_static(node->addr_list));

            pfcp_node_fsm_init(node, false);

        } else {
            ogs_error("Cannot find PFCP-Node: type [%d] node_id %s from %s",
                    message->h.type,
                    pfcp_status == OGS_PFCP_STATUS_SUCCESS ?
                        ogs_pfcp_node_id_to_string_static(&node_id) :
                        "NULL",
                    ogs_sockaddr_to_string_static(&from));
            goto cleanup;
        }
    } else {
        ogs_debug("Found PFCP-Node: addr_list %s",
                ogs_sockaddr_to_string_static(node->addr_list));
        ogs_expect(OGS_OK == ogs_pfcp_node_merge(
                    node,
                    pfcp_status == OGS_PFCP_STATUS_SUCCESS ?  &node_id : NULL,
                    &from));
        ogs_debug("Merged PFCP-Node: addr_list %s",
                ogs_sockaddr_to_string_static(node->addr_list));
    }

    e->pfcp_node = node;
    e->pkbuf = pkbuf;
    e->pfcp_message = message;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        goto cleanup;
    }

    return;

cleanup:
    ogs_pkbuf_free(pkbuf);
    ogs_pfcp_message_free(message);
    sgwc_event_free(e);
}

int sgwc_pfcp_open(void)
{
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    /* PFCP Server */
    ogs_list_for_each(&ogs_pfcp_self()->pfcp_list, node) {
        sock = ogs_pfcp_server(node);
        if (!sock) return OGS_ERROR;

        node->poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, sock->fd, pfcp_recv_cb, sock);
        ogs_assert(node->poll);
    }
    ogs_list_for_each(&ogs_pfcp_self()->pfcp_list6, node) {
        sock = ogs_pfcp_server(node);
        if (!sock) return OGS_ERROR;

        node->poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, sock->fd, pfcp_recv_cb, sock);
        ogs_assert(node->poll);
    }

    OGS_SETUP_PFCP_SERVER;

    return OGS_OK;
}

void sgwc_pfcp_close(void)
{
    ogs_pfcp_node_t *pfcp_node = NULL;

    ogs_list_for_each(&ogs_pfcp_self()->pfcp_peer_list, pfcp_node)
        pfcp_node_fsm_fini(pfcp_node);

    ogs_freeaddrinfo(ogs_pfcp_self()->pfcp_advertise);
    ogs_freeaddrinfo(ogs_pfcp_self()->pfcp_advertise6);

    ogs_socknode_remove_all(&ogs_pfcp_self()->pfcp_list);
    ogs_socknode_remove_all(&ogs_pfcp_self()->pfcp_list6);
}

static void sess_timeout(ogs_pfcp_xact_t *xact, void *data)
{
    sgwc_sess_t *sess = NULL;
    ogs_pool_id_t sess_id = OGS_INVALID_POOL_ID;
    uint8_t type;

    ogs_assert(xact);
    type = xact->seq[0].type;

    ogs_assert(data);
    sess_id = OGS_POINTER_TO_UINT(data);
    ogs_assert(sess_id >= OGS_MIN_POOL_ID && sess_id <= OGS_MAX_POOL_ID);

    sess = sgwc_sess_find_by_id(sess_id);
    if (!sess) {
        ogs_error("Session has already been removed [%d]", type);
        return;
    }

    switch (type) {
    case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
        ogs_error("No PFCP session establishment response");
        break;
    case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
        ogs_error("No PFCP session modification response");
        break;
    case OGS_PFCP_SESSION_DELETION_REQUEST_TYPE:
        ogs_error("No PFCP session deletion response");
        break;
    default:
        ogs_error("Not implemented [type:%d]", type);
        break;
    }
}

static void bearer_timeout(ogs_pfcp_xact_t *xact, void *data)
{
    sgwc_bearer_t *bearer = NULL;
    ogs_pool_id_t bearer_id = OGS_INVALID_POOL_ID;
    uint8_t type;

    ogs_assert(xact);
    type = xact->seq[0].type;

    ogs_assert(data);
    bearer_id = OGS_POINTER_TO_UINT(data);
    ogs_assert(bearer_id >= OGS_MIN_POOL_ID && bearer_id <= OGS_MAX_POOL_ID);

    bearer = sgwc_bearer_find_by_id(bearer_id);
    if (!bearer) {
        ogs_error("Bearer has already been removed [%d]", type);
        return;
    }

    switch (type) {
    case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
        ogs_error("No PFCP session modification response");
        break;
    default:
        ogs_error("Not implemented [type:%d]", type);
        break;
    }
}

int sgwc_pfcp_send_bearer_to_modify_list(
        sgwc_sess_t *sess, ogs_pfcp_xact_t *xact)
{
    int rv;
    ogs_pkbuf_t *sxabuf = NULL;
    ogs_pfcp_header_t h;

    ogs_assert(sess);
    ogs_assert(xact);

    xact->local_seid = sess->sgwc_sxa_seid;

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE;
    h.seid = sess->sgwu_sxa_seid;

    sxabuf = sgwc_sxa_build_bearer_to_modify_list(h.type, sess, xact);
    if (!sxabuf) {
        ogs_error("sgwc_sxa_build_bearer_to_modify_list() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &h, sxabuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int sgwc_pfcp_send_session_establishment_request(
        sgwc_sess_t *sess, ogs_pool_id_t gtp_xact_id, ogs_pkbuf_t *gtpbuf,
        uint64_t flags)
{
    int rv;
    ogs_pkbuf_t *sxabuf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, sess_timeout, OGS_UINT_TO_POINTER(sess->id));
    if (!xact) {
        ogs_error("ogs_pfcp_xact_local_create() failed");
        return OGS_ERROR;
    }

    xact->assoc_xact_id = gtp_xact_id;
    if (gtpbuf) {
        xact->gtpbuf = ogs_pkbuf_copy(gtpbuf);
        if (!xact->gtpbuf) {
            ogs_error("ogs_pkbuf_copy() failed");
            return OGS_ERROR;
        }
    }
    xact->local_seid = sess->sgwc_sxa_seid;
    xact->create_flags = flags;

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE;

/*
 * 7.2.2.4.2 Conditions for Sending SEID=0 in PFCP Header
 *
 * If a peer's SEID is not available, the SEID field shall still be present
 * in the header and its value shall be set to "0" in the following messages:
 *
 * - PFCP Session Establishment Request message on Sxa/Sxb/Sxc/N4;
 *
 * - If a node receives a message for which it has no session, i.e.
 *   if SEID in the PFCP header is not known, it shall respond
 *   with "Session context not found" cause in the corresponding
 *   response message to the sender, the SEID used in the PFCP header
 *   in the response message shall be then set to "0";
 *
 * - If a node receives a request message containing protocol error,
 *   e.g. Mandatory IE missing, which requires the receiver
 *   to reject the message as specified in clause 7.6, it shall reject
 *   the request message. For the response message, the node should look up
 *   the remote peer's SEID and accordingly set SEID in the PFCP header
 *   and the message cause code. As an implementation option,
 *   the node may not look up the remote peer's SEID and
 *   set the PFCP header SEID to "0" in the response message.
 *   However in this case, the cause value shall not be set
 *   to "Session not found".
 *
 * - When the UP function sends PFCP Session Report Request message
 *   over N4 towards another SMF or another PFCP entity in the SMF
 *   as specified in clause 5.22.2 and clause 5.22.3.
 */
    h.seid = sess->sgwu_sxa_seid;

    sxabuf = sgwc_sxa_build_session_establishment_request(h.type, sess);
    if (!sxabuf) {
        ogs_error("sgwc_sxa_build_session_establishment_request() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &h, sxabuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int sgwc_pfcp_send_session_modification_request(
        sgwc_sess_t *sess, ogs_pool_id_t gtp_xact_id,
        ogs_pkbuf_t *gtpbuf, uint64_t flags)
{
    ogs_pfcp_xact_t *xact = NULL;
    sgwc_bearer_t *bearer = NULL;

    ogs_assert(sess);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, sess_timeout, OGS_UINT_TO_POINTER(sess->id));
    if (!xact) {
        ogs_error("ogs_pfcp_xact_local_create() failed");
        return OGS_ERROR;
    }

    xact->assoc_xact_id = gtp_xact_id;
    xact->modify_flags = flags | OGS_PFCP_MODIFY_SESSION;
    if (gtpbuf) {
        xact->gtpbuf = ogs_pkbuf_copy(gtpbuf);
        if (!xact->gtpbuf) {
            ogs_error("ogs_pkbuf_copy() failed");
            return OGS_ERROR;
        }
    }
    xact->local_seid = sess->sgwc_sxa_seid;

    ogs_list_for_each(&sess->bearer_list, bearer)
        ogs_list_add(&xact->bearer_to_modify_list, &bearer->to_modify_node);

    return sgwc_pfcp_send_bearer_to_modify_list(sess, xact);
}

int sgwc_pfcp_send_bearer_modification_request(
        sgwc_bearer_t *bearer, ogs_pool_id_t gtp_xact_id,
        ogs_pkbuf_t *gtpbuf, uint64_t flags)
{
    int rv;
    ogs_pkbuf_t *sxabuf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;
    sgwc_sess_t *sess = NULL;

    ogs_assert(bearer);
    sess = sgwc_sess_find_by_id(bearer->sess_id);
    ogs_assert(sess);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, bearer_timeout, OGS_UINT_TO_POINTER(bearer->id));
    if (!xact) {
        ogs_error("ogs_pfcp_xact_local_create() failed");
        return OGS_ERROR;
    }

    xact->assoc_xact_id = gtp_xact_id;
    xact->modify_flags = flags;
    if (gtpbuf) {
        xact->gtpbuf = ogs_pkbuf_copy(gtpbuf);
        if (!xact->gtpbuf) {
            ogs_error("ogs_pkbuf_copy() failed");
            return OGS_ERROR;
        }
    }
    xact->local_seid = sess->sgwc_sxa_seid;

    ogs_list_add(&xact->bearer_to_modify_list, &bearer->to_modify_node);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE;
    h.seid = sess->sgwu_sxa_seid;

    sxabuf = sgwc_sxa_build_bearer_to_modify_list(h.type, sess, xact);
    if (!sxabuf) {
        ogs_error("sgwc_sxa_build_bearer_to_modify_list() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &h, sxabuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int sgwc_pfcp_send_session_deletion_request(
        sgwc_sess_t *sess, ogs_pool_id_t gtp_xact_id, ogs_pkbuf_t *gtpbuf)
{
    int rv;
    ogs_pkbuf_t *sxabuf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, sess_timeout, OGS_UINT_TO_POINTER(sess->id));
    if (!xact) {
        ogs_error("ogs_pfcp_xact_local_create() failed");
        return OGS_ERROR;
    }

    xact->assoc_xact_id = gtp_xact_id;
    if (gtpbuf) {
        xact->gtpbuf = ogs_pkbuf_copy(gtpbuf);
        if (!xact->gtpbuf) {
            ogs_error("ogs_pkbuf_copy() failed");
            return OGS_ERROR;
        }
    }
    xact->local_seid = sess->sgwc_sxa_seid;

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_DELETION_REQUEST_TYPE;
    h.seid = sess->sgwu_sxa_seid;

    sxabuf = sgwc_sxa_build_session_deletion_request(h.type, sess);
    if (!sxabuf) {
        ogs_error("sgwc_sxa_build_session_deletion_request() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &h, sxabuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int sgwc_pfcp_send_session_report_response(
        ogs_pfcp_xact_t *xact, sgwc_sess_t *sess, uint8_t cause)
{
    int rv;
    ogs_pkbuf_t *sxabuf = NULL;
    ogs_pfcp_header_t h;

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE;
    h.seid = sess->sgwu_sxa_seid;

    sxabuf = ogs_pfcp_build_session_report_response(h.type, cause);
    if (!sxabuf) {
        ogs_error("ogs_pfcp_build_session_report_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_update_tx(xact, &h, sxabuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}
