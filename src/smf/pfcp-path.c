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

#include "sbi-path.h"
#include "pfcp-path.h"
#include "n4-build.h"

static void pfcp_node_fsm_init(ogs_pfcp_node_t *node, bool try_to_assoicate)
{
    smf_event_t e;

    ogs_assert(node);

    memset(&e, 0, sizeof(e));
    e.pfcp_node = node;

    if (try_to_assoicate == true) {
        node->t_association = ogs_timer_add(ogs_app()->timer_mgr,
                smf_timer_pfcp_association, node);
        ogs_assert(node->t_association);
    }

    ogs_fsm_create(&node->sm, smf_pfcp_state_initial, smf_pfcp_state_final);
    ogs_fsm_init(&node->sm, &e);
}

static void pfcp_node_fsm_fini(ogs_pfcp_node_t *node)
{
    smf_event_t e;

    ogs_assert(node);

    memset(&e, 0, sizeof(e));
    e.pfcp_node = node;

    ogs_fsm_fini(&node->sm, &e);
    ogs_fsm_delete(&node->sm);

    if (node->t_association)
        ogs_timer_delete(node->t_association);
}

static void pfcp_recv_cb(short when, ogs_socket_t fd, void *data)
{
    int rv;

    ssize_t size;
    smf_event_t *e = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_sockaddr_t from;
    ogs_pfcp_node_t *node = NULL;
    ogs_pfcp_header_t *h = NULL;

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
    if (h->version > OGS_PFCP_VERSION) {
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

    e = smf_event_new(SMF_EVT_N4_MESSAGE);
    ogs_assert(e);

    node = ogs_pfcp_node_find(&ogs_pfcp_self()->peer_list, &from);
    if (!node) {
        node = ogs_pfcp_node_add(&ogs_pfcp_self()->peer_list, &from);
        ogs_assert(node);

        node->sock = data;
        pfcp_node_fsm_init(node, false);
    }
    e->pfcp_node = node;
    e->pkbuf = pkbuf;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        ogs_pkbuf_free(e->pkbuf);
        smf_event_free(e);
    }
}

int smf_pfcp_open(void)
{
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;
    ogs_pfcp_node_t *pfcp_node = NULL;

    /* PFCP Server */
    ogs_list_for_each(&ogs_pfcp_self()->pfcp_list, node) {
        sock = ogs_pfcp_server(node);
        ogs_assert(sock);
        
        node->poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, sock->fd, pfcp_recv_cb, sock);
    }
    ogs_list_for_each(&ogs_pfcp_self()->pfcp_list6, node) {
        sock = ogs_pfcp_server(node);
        ogs_assert(sock);

        node->poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, sock->fd, pfcp_recv_cb, sock);
    }

    ogs_pfcp_self()->pfcp_sock =
        ogs_socknode_sock_first(&ogs_pfcp_self()->pfcp_list);
    if (ogs_pfcp_self()->pfcp_sock)
        ogs_pfcp_self()->pfcp_addr = &ogs_pfcp_self()->pfcp_sock->local_addr;

    ogs_pfcp_self()->pfcp_sock6 =
        ogs_socknode_sock_first(&ogs_pfcp_self()->pfcp_list6);
    if (ogs_pfcp_self()->pfcp_sock6)
        ogs_pfcp_self()->pfcp_addr6 = &ogs_pfcp_self()->pfcp_sock6->local_addr;

    ogs_assert(ogs_pfcp_self()->pfcp_addr || ogs_pfcp_self()->pfcp_addr6);

    ogs_list_for_each(&ogs_pfcp_self()->peer_list, pfcp_node)
        pfcp_node_fsm_init(pfcp_node, true);

    return OGS_OK;
}

void smf_pfcp_close(void)
{
    ogs_pfcp_node_t *pfcp_node = NULL;

    ogs_list_for_each(&ogs_pfcp_self()->peer_list, pfcp_node)
        pfcp_node_fsm_fini(pfcp_node);

    ogs_socknode_remove_all(&ogs_pfcp_self()->pfcp_list);
    ogs_socknode_remove_all(&ogs_pfcp_self()->pfcp_list6);
}

static void sess_5gc_timeout(ogs_pfcp_xact_t *xact, void *data)
{
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;
    ogs_sbi_stream_t *stream = NULL;
    uint8_t type;
    char *strerror = NULL;

    ogs_assert(xact);
    ogs_assert(data);

    sess = data;
    ogs_assert(sess);
    stream = xact->assoc_stream;
    ogs_assert(stream);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    type = xact->seq[0].type;
    switch (type) {
    case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
        ogs_error("No PFCP session establishment response");
        break;
    case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
        strerror = ogs_msprintf("[%s:%d] No PFCP session modification response",
                smf_ue->supi, sess->psi);
        ogs_assert(strerror);

        ogs_error("%s", strerror);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT,
                strerror, NULL, NULL, NULL);
        ogs_free(strerror);
        break;
    case OGS_PFCP_SESSION_DELETION_REQUEST_TYPE:
        strerror = ogs_msprintf("[%s:%d] No PFCP session deletion response",
                smf_ue->supi, sess->psi);
        ogs_assert(strerror);

        ogs_error("%s", strerror);
        ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL, strerror, NULL);
        ogs_free(strerror);
        break;
    default:
        ogs_error("Not implemented [type:%d]", type);
        break;
    }
}

static void sess_epc_timeout(ogs_pfcp_xact_t *xact, void *data)
{
    uint8_t type;

    ogs_assert(xact);
    type = xact->seq[0].type;

    switch (type) {
    case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
        ogs_warn("No PFCP session establishment response");
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

void smf_5gc_pfcp_send_session_establishment_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);
    ogs_assert(stream);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = smf_n4_build_session_establishment_request(h.type, sess);
    ogs_expect_or_return(n4buf);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, &h, n4buf, sess_5gc_timeout, sess);
    ogs_expect_or_return(xact);
    xact->assoc_stream = stream;

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void smf_5gc_pfcp_send_session_modification_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, uint64_t flags)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);
    ogs_assert(stream);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = smf_5gc_n4_build_session_modification_request(h.type, sess, flags);
    ogs_expect_or_return(n4buf);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, &h, n4buf, sess_5gc_timeout, sess);
    ogs_expect_or_return(xact);
    xact->assoc_stream = stream;
    xact->modify_flags = flags | OGS_PFCP_MODIFY_SESSION;

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void smf_5gc_pfcp_send_qos_flow_modification_request(smf_bearer_t *qos_flow,
        ogs_sbi_stream_t *stream, uint64_t flags)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;
    smf_sess_t *sess = NULL;

    ogs_assert(qos_flow);
    sess = qos_flow->sess;
    ogs_assert(sess);

    ogs_assert(stream);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = smf_n4_build_qos_flow_modification_request(h.type, qos_flow, flags);
    ogs_expect_or_return(n4buf);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, &h, n4buf, sess_5gc_timeout, qos_flow);
    ogs_expect_or_return(xact);

    xact->assoc_stream = stream;
    xact->modify_flags = flags;

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void smf_5gc_pfcp_send_session_deletion_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, int trigger)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);
    ogs_assert(stream);
    ogs_assert(trigger);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_DELETION_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = smf_n4_build_session_deletion_request(h.type, sess);
    ogs_expect_or_return(n4buf);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, &h, n4buf, sess_5gc_timeout, sess);
    ogs_expect_or_return(xact);
    xact->assoc_stream = stream;
    xact->delete_trigger = trigger;

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void smf_epc_pfcp_send_session_establishment_request(
        smf_sess_t *sess, void *gtp_xact)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = smf_n4_build_session_establishment_request(h.type, sess);
    ogs_expect_or_return(n4buf);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, &h, n4buf, sess_epc_timeout, sess);
    ogs_expect_or_return(xact);

    xact->epc = true; /* EPC PFCP transaction */
    xact->assoc_xact = gtp_xact;

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void smf_epc_pfcp_send_bearer_modification_request(
        smf_bearer_t *bearer, uint64_t flags)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;
    smf_sess_t *sess = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = smf_n4_build_qos_flow_modification_request(h.type, bearer, flags);
    ogs_expect_or_return(n4buf);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, &h, n4buf, sess_epc_timeout, bearer);
    ogs_expect_or_return(xact);

    xact->epc = true; /* EPC PFCP transaction */
    xact->modify_flags = flags;

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void smf_epc_pfcp_send_session_deletion_request(
        smf_sess_t *sess, void *gtp_xact)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_DELETION_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = smf_n4_build_session_deletion_request(h.type, sess);
    ogs_expect_or_return(n4buf);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, &h, n4buf, sess_epc_timeout, sess);
    ogs_expect_or_return(xact);

    xact->epc = true; /* EPC PFCP transaction */
    xact->assoc_xact = gtp_xact;

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}
