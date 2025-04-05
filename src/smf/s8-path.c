/* smf/s8-path.c */
#include "s8-path.h"

static int smf_s8_listen(void);

int smf_s8_init(void)
{
    int rv;

    ogs_list_init(&smf_self()->s8_config.authorized_peers);

    rv = smf_s8_listen();
    if (rv != OGS_OK) return rv;

    ogs_info("Roaming S8 path initialized");
    return OGS_OK;
}

static int smf_s8_listen(void)
{
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    /* Create GTP-C socket for S8 interface */
    node = ogs_gtp_server(smf_self()->s8_config.addr,
                         smf_self()->s8_config.addr6,
                         ogs_config()->gtpc.port);
    if (node == NULL) return OGS_ERROR;

    /* Setup GTP-C server */
    sock = node->sock;
    if (sock) {
        sock->data = node;
        if (ogs_sock_register(sock, smf_self()->gtpc_ev_base,
            OGS_POLLIN, smf_s8_recv_cb, sock) != OGS_OK) {
            ogs_error("S8 ogs_sock_register() failed");
            return OGS_ERROR;
        }
    }

    /* Store node in context */
    ogs_list_add(&smf_self()->gtpc_list, node);

    return OGS_OK;
}

void smf_s8_final(void)
{
    smf_s8_peer_t *peer = NULL, *next_peer = NULL;

    /* Remove all peers */
    ogs_list_for_each_safe(&smf_self()->s8_config.authorized_peers,
                          next_peer, peer) {
        ogs_list_remove(&peer->node);
        ogs_free(peer);
    }

    ogs_info("S8 path finalized");
}

void smf_s8_send_create_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact, uint8_t cause)
{
    int rv;
    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    ogs_assert(xact);

    /* Build Create Session Response */
    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE;
    h.teid = sess->sgw_s8_teid;

    pkbuf = smf_s8_build_create_session_response(h.type, sess, cause);
    if (!pkbuf) {
        ogs_error("S8 Cannot build Create Session Response");
        return;
    }

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("S8 ogs_gtp_xact_update_tx() failed");
        return;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);
}