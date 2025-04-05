#include "context.h"
#include "s8-path.h"
#include "s8-build.h"
#include "ogs-gtp.h"
#include "ogs-pkbuf.h"
#include "ogs-socknode.h"

int smf_s8_init(void)
{
    int rv;

    rv = smf_s8_listen();
    if (rv != OGS_OK) {
        ogs_error("Failed to initialize S8 interface");
        return rv;
    }

    return OGS_OK;
}

void smf_s8_final(void)
{
    ogs_socknode_remove_all(&smf_self()->s8_list);
}

static int smf_s8_listen(void)
{
    int rv;
    ogs_socknode_t *node = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(smf_self()->s8_addr);

    addr = smf_self()->s8_addr;
    node = ogs_socknode_new(addr);
    if (!node) {
        ogs_error("Failed to create socket node");
        return OGS_ERROR;
    }

    node->option = OGS_SOCKOPT_REUSEADDR;
    rv = ogs_gtp_server(node);
    if (rv != OGS_OK) {
        ogs_error("Failed to start S8 server");
        ogs_socknode_free(node);
        return rv;
    }

    ogs_list_add(&smf_self()->s8_list, node);

    return OGS_OK;
}

void smf_s8_send_create_session_response(
    smf_sess_t *sess, ogs_gtp_xact_t *xact, uint8_t cause)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int rv;

    ogs_assert(sess);
    ogs_assert(xact);

    pkbuf = smf_s8_build_create_session_response(sess, xact);
    if (!pkbuf) {
        ogs_error("Failed to build Create Session Response");
        return;
    }

    rv = ogs_gtp_xact_update_tx(xact, &pkbuf->gtp2_message->h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("Failed to update transaction");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("Failed to commit transaction");
        return;
    }
}

void smf_s8_send_delete_session_response(
    smf_sess_t *sess, ogs_gtp_xact_t *xact, uint8_t cause)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int rv;

    ogs_assert(sess);
    ogs_assert(xact);

    pkbuf = smf_s8_build_delete_session_response(sess, xact);
    if (!pkbuf) {
        ogs_error("Failed to build Delete Session Response");
        return;
    }

    rv = ogs_gtp_xact_update_tx(xact, &pkbuf->gtp2_message->h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("Failed to update transaction");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("Failed to commit transaction");
        return;
    }
}

void smf_s8_send_modify_bearer_response(
    smf_sess_t *sess, ogs_gtp_xact_t *xact, uint8_t cause)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int rv;

    ogs_assert(sess);
    ogs_assert(xact);

    pkbuf = smf_s8_build_modify_bearer_response(sess, xact);
    if (!pkbuf) {
        ogs_error("Failed to build Modify Bearer Response");
        return;
    }

    rv = ogs_gtp_xact_update_tx(xact, &pkbuf->gtp2_message->h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("Failed to update transaction");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("Failed to commit transaction");
        return;
    }
}