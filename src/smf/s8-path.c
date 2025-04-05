#include "context.h"
#include "s8-path.h"
#include "s8-build.h"

int smf_s8_init(void)
{
    int rv;

    ogs_list_init(&smf_self()->s8_config.authorized_peers);

    rv = smf_s8_listen();
    if (rv != OGS_OK) {
        ogs_error("Failed to initialize S8 interface");
        return rv;
    }

    ogs_info("Roaming S8 path initialized");
    return OGS_OK;
}

static int smf_s8_listen(void)
{
    ogs_socknode_t *node = NULL;
    ogs_sockaddr_t *addr = NULL;
    int rv;

    node = ogs_socknode_new();
    if (!node) {
        ogs_error("Failed to create socket node");
        return OGS_ERROR;
    }

    addr = smf_self()->s8_config.addr;
    if (!addr) {
        ogs_error("No S8 address configured");
        ogs_socknode_free(node);
        return OGS_ERROR;
    }

    node->addr = addr;
    node->option = OGS_SOCKOPT_REUSEADDR;

    rv = ogs_gtp_server(node);
    if (rv != OGS_OK) {
        ogs_error("Failed to start S8 server");
        ogs_socknode_free(node);
        return rv;
    }

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

int smf_s8_send_create_session_response(
    smf_sess_t *sess,
    ogs_gtp_xact_t *xact)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int rv;

    ogs_assert(sess);
    ogs_assert(xact);

    pkbuf = smf_s8_build_create_session_response(sess, xact);
    if (!pkbuf) {
        ogs_error("Failed to build Create Session Response");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_update_tx(xact, &pkbuf);
    if (rv != OGS_OK) {
        ogs_error("Failed to update transaction");
        ogs_pkbuf_free(pkbuf);
        return rv;
    }

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("Failed to commit transaction");
        return rv;
    }

    return OGS_OK;
}