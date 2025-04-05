#include "s8-path.h"
#include "gtp-path.h"

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

    /* Create new socknode for GTP-C */
    node = ogs_socknode_new(AF_UNSPEC);
    if (!node) {
        ogs_error("ogs_socknode_new() failed");
        return OGS_ERROR;
    }

    /* Set node addresses */
    node->addr = smf_self()->s8_config.addr;
    node->addr6 = smf_self()->s8_config.addr6;

    /* Create GTP-C socket */
    sock = ogs_gtp_server(node);
    if (!sock) {
        ogs_error("ogs_gtp_server() failed");
        ogs_socknode_free(node);
        return OGS_ERROR;
    }

    node->sock = sock;
    ogs_list_add(&smf_self()->sgw_s8_list, node);

    return OGS_OK;
}

void smf_s8_final(void)
{
    ogs_socknode_t *node = NULL, *next_node = NULL;

    ogs_list_for_each_safe(&smf_self()->sgw_s8_list, next_node, node) {
        ogs_list_remove(&smf_self()->sgw_s8_list, node);
        if (node->sock)
            ogs_sock_destroy(node->sock);
        ogs_socknode_free(node);
    }

    ogs_info("S8 path finalized");
}

void smf_s8_send_create_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact, uint8_t cause)
{
    int rv;
    ogs_gtp2_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE;
    h.teid = sess->sgw_s8_teid;

    pkbuf = smf_s8_build_create_session_response(sess, xact);
    if (!pkbuf) {
        ogs_error("smf_s8_build_create_session_response() failed");
        return;
    }

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_update_tx() failed");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);
}