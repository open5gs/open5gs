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

#include "context.h"

#include "event.h"
#include "gtp-path.h"
#include "s5c-build.h"

#define SMF_GTP_HANDLED     1

static void _gtpv2_c_recv_cb(short when, ogs_socket_t fd, void *data)
{
    smf_event_t *e = NULL;
    int rv;
    ssize_t size;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_sockaddr_t from;
    ogs_gtp_node_t *gnode = NULL;

    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);

    size = ogs_recvfrom(fd, pkbuf->data, pkbuf->len, 0, &from);
    if (size <= 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_recvfrom() failed");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    ogs_pkbuf_trim(pkbuf, size);

    e = smf_event_new(SMF_EVT_S5C_MESSAGE);
    gnode = ogs_gtp_node_find_by_addr(&smf_self()->sgw_s5c_list, &from);
    if (!gnode) {
        gnode = ogs_gtp_node_add_by_addr(&smf_self()->sgw_s5c_list, &from);
        ogs_assert(gnode);
        gnode->sock = data;
    }
    ogs_assert(e);
    e->gnode = gnode;
    e->pkbuf = pkbuf;

    rv = ogs_queue_push(smf_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_pkbuf_free(e->pkbuf);
        smf_event_free(e);
    }
}

int smf_gtp_open(void)
{
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    ogs_list_for_each(&smf_self()->gtpc_list, node) {
        sock = ogs_gtp_server(node);
        ogs_assert(sock);
        
        node->poll = ogs_pollset_add(smf_self()->pollset,
                OGS_POLLIN, sock->fd, _gtpv2_c_recv_cb, sock);
    }
    ogs_list_for_each(&smf_self()->gtpc_list6, node) {
        sock = ogs_gtp_server(node);
        ogs_assert(sock);

        node->poll = ogs_pollset_add(smf_self()->pollset,
                OGS_POLLIN, sock->fd, _gtpv2_c_recv_cb, sock);
    }

    smf_self()->gtpc_sock = ogs_socknode_sock_first(&smf_self()->gtpc_list);
    if (smf_self()->gtpc_sock)
        smf_self()->gtpc_addr = &smf_self()->gtpc_sock->local_addr;

    smf_self()->gtpc_sock6 = ogs_socknode_sock_first(&smf_self()->gtpc_list6);
    if (smf_self()->gtpc_sock6)
        smf_self()->gtpc_addr6 = &smf_self()->gtpc_sock6->local_addr;

    ogs_assert(smf_self()->gtpc_addr || smf_self()->gtpc_addr6);

    return OGS_OK;
}

void smf_gtp_close(void)
{
    ogs_socknode_remove_all(&smf_self()->gtpc_list);
    ogs_socknode_remove_all(&smf_self()->gtpc_list6);
}

void smf_gtp_send_create_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact)
{
    int rv;
    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_CREATE_SESSION_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    pkbuf = smf_s5c_build_create_session_response(h.type, sess);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void smf_gtp_send_delete_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact)
{
    int rv;
    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(xact);
    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_DELETE_SESSION_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    pkbuf = smf_s5c_build_delete_session_response(h.type, sess);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}
