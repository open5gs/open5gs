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

#include "ogs-gtp.h"

#include "mme-event.h"
#include "mme-gtp-path.h"
#include "mme-s11-build.h"
#include "mme-sm.h"

static void _gtpv2_c_recv_cb(short when, ogs_socket_t fd, void *data)
{
    int rv;
    ssize_t size;
    mme_event_t *e = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);

    size = ogs_recv(fd, pkbuf->data, pkbuf->len, 0);
    if (size <= 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_recv() failed");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    ogs_pkbuf_trim(pkbuf, size);

    e = mme_event_new(MME_EVT_S11_MESSAGE);
    ogs_assert(e);
    e->pkbuf = pkbuf;
    rv = ogs_queue_push(mme_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_pkbuf_free(e->pkbuf);
        mme_event_free(e);
    }
}

int mme_gtp_open(void)
{
    int rv;
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;
    mme_sgw_t *sgw = NULL;

    ogs_list_for_each(&mme_self()->gtpc_list, node) {
        sock = ogs_gtp_server(node);
        ogs_assert(sock);

        node->poll = ogs_pollset_add(mme_self()->pollset,
                OGS_POLLIN, sock->fd, _gtpv2_c_recv_cb, NULL);
    }
    ogs_list_for_each(&mme_self()->gtpc_list6, node) {
        sock = ogs_gtp_server(node);
        ogs_assert(sock);

        node->poll = ogs_pollset_add(mme_self()->pollset,
                OGS_POLLIN, sock->fd, _gtpv2_c_recv_cb, NULL);
    }

    mme_self()->gtpc_sock = ogs_gtp_local_sock_first(&mme_self()->gtpc_list);
    mme_self()->gtpc_sock6 = ogs_gtp_local_sock_first(&mme_self()->gtpc_list6);
    mme_self()->gtpc_addr = ogs_gtp_local_addr_first(&mme_self()->gtpc_list);
    mme_self()->gtpc_addr6 = ogs_gtp_local_addr_first(&mme_self()->gtpc_list6);

    ogs_assert(mme_self()->gtpc_addr || mme_self()->gtpc_addr6);

    mme_self()->pgw_addr = mme_pgw_addr_find_by_apn(
            &mme_self()->pgw_list, AF_INET, NULL);
    mme_self()->pgw_addr6 = mme_pgw_addr_find_by_apn(
            &mme_self()->pgw_list, AF_INET6, NULL);
    ogs_assert(mme_self()->pgw_addr || mme_self()->pgw_addr6);

    ogs_list_for_each(&mme_self()->sgw_list, sgw) {
        rv = ogs_gtp_connect(
                mme_self()->gtpc_sock, mme_self()->gtpc_sock6, sgw->node);
        ogs_assert(rv == OGS_OK);
    }

    return OGS_OK;
}

void mme_gtp_close(void)
{
    ogs_socknode_remove_all(&mme_self()->gtpc_list);
    ogs_socknode_remove_all(&mme_self()->gtpc_list6);
}

int mme_gtp_send_create_session_request(mme_sess_t *sess)
{
    int rv;
    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;

    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_CREATE_SESSION_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_create_session_request(&pkbuf, h.type, sess);
    ogs_assert(rv == OGS_OK);

    xact = ogs_gtp_xact_local_create(mme_ue->gnode, &h, pkbuf);
    ogs_assert(xact);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}


int mme_gtp_send_modify_bearer_request(
        mme_bearer_t *bearer, int uli_presence)
{
    int rv;

    ogs_gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_MODIFY_BEARER_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_modify_bearer_request(
            &pkbuf, h.type, bearer, uli_presence);
    ogs_assert(rv == OGS_OK);

    xact = ogs_gtp_xact_local_create(mme_ue->gnode, &h, pkbuf);
    ogs_assert(xact);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int mme_gtp_send_delete_session_request(mme_sess_t *sess)
{
    int rv;
    ogs_pkbuf_t *s11buf = NULL;
    ogs_gtp_header_t h;
    ogs_gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_DELETE_SESSION_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_delete_session_request(&s11buf, h.type, sess);
    ogs_assert(rv == OGS_OK);

    xact = ogs_gtp_xact_local_create(mme_ue->gnode, &h, s11buf);
    ogs_assert(xact);

    OGS_GTP_XACT_STORE_SESSION(xact, sess);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int mme_gtp_send_delete_all_sessions(mme_ue_t *mme_ue)
{
    int rv;
    mme_sess_t *sess = NULL, *next_sess = NULL;

    ogs_assert(mme_ue);

    if (SESSION_CONTEXT_WILL_DELETED(mme_ue)) {
        ogs_warn("The MME has already sent a Delete-Session-Request to the SGW"
                " for all sessions.");
        return OGS_OK;
    }

    mme_ue->session_context_will_deleted = 1;

    sess = mme_sess_first(mme_ue);
    while (sess != NULL) {
        next_sess = mme_sess_next(sess);

        if (MME_HAVE_SGW_S1U_PATH(sess)) {
            mme_bearer_t *bearer = mme_default_bearer_in_sess(sess);
            ogs_assert(bearer);

            if (bearer &&
                OGS_FSM_CHECK(&bearer->sm, esm_state_pdn_will_disconnect)) {
                ogs_warn("PDN will disconnect[EBI:%d]", bearer->ebi);
            } else {
                rv = mme_gtp_send_delete_session_request(sess);
                ogs_assert(rv == OGS_OK);
            }
        } else {
            mme_sess_remove(sess);
        }

        sess = next_sess;
    }

    return OGS_OK;
}

int mme_gtp_send_create_bearer_response(mme_bearer_t *bearer)
{
    int rv;

    ogs_gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);
    xact = bearer->xact;
    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_CREATE_BEARER_RESPONSE_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_create_bearer_response(&pkbuf, h.type, bearer);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    ogs_assert(xact);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int mme_gtp_send_update_bearer_response(mme_bearer_t *bearer)
{
    int rv;

    ogs_gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);
    xact = bearer->xact;
    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_update_bearer_response(&pkbuf, h.type, bearer);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    ogs_assert(xact);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int mme_gtp_send_delete_bearer_response(mme_bearer_t *bearer)
{
    int rv;

    ogs_gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(mme_ue);
    xact = bearer->xact;
    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_DELETE_BEARER_RESPONSE_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_delete_bearer_response(&pkbuf, h.type, bearer);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    ogs_assert(xact);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int mme_gtp_send_release_access_bearers_request(mme_ue_t *mme_ue)
{
    int rv;
    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_xact_t *xact = NULL;

    ogs_assert(mme_ue);

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_release_access_bearers_request(&pkbuf, h.type);
    ogs_assert(rv == OGS_OK);

    xact = ogs_gtp_xact_local_create(mme_ue->gnode, &h, pkbuf);
    ogs_assert(xact);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int mme_gtp_send_create_indirect_data_forwarding_tunnel_request(
        mme_ue_t *mme_ue)
{
    int rv;
    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_xact_t *xact = NULL;

    ogs_assert(mme_ue);

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_create_indirect_data_forwarding_tunnel_request(
            &pkbuf, h.type, mme_ue);
    ogs_assert(rv == OGS_OK);

    xact = ogs_gtp_xact_local_create(mme_ue->gnode, &h, pkbuf);
    ogs_assert(xact);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}

int mme_gtp_send_delete_indirect_data_forwarding_tunnel_request(
        mme_ue_t *mme_ue)
{
    int rv;
    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_xact_t *xact = NULL;

    ogs_assert(mme_ue);

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_TLV_MAX_HEADROOM);
    ogs_pkbuf_reserve(pkbuf, OGS_TLV_MAX_HEADROOM);

    xact = ogs_gtp_xact_local_create(mme_ue->gnode, &h, pkbuf);
    ogs_assert(xact);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    return OGS_OK;
}
