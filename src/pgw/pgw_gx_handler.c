#define TRACE_MODULE _pgw_gx_handler

#include "core_debug.h"

#include "pgw_context.h"
#include "gx_message.h"
#include "pgw_gtp_path.h"
#include "pgw_s5c_build.h"

void pgw_gx_handle_create_bearer_request(pgw_sess_t *sess, pcc_rule_t *pcc_rule)
{
    status_t rv;
    gtp_xact_t *xact = NULL;
    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;
    pgw_bearer_t *bearer = NULL;

    d_assert(sess, return, "Null param");
    d_assert(pcc_rule, return, "Null param");

    bearer = pgw_bearer_find_by_qci_arp(sess, 
                pcc_rule->qos.qci,
                pcc_rule->qos.arp.priority_level,
                pcc_rule->qos.arp.pre_emption_capability,
                pcc_rule->qos.arp.pre_emption_vulnerability);
    if (bearer)
    {
        return;
    }

    bearer = pgw_bearer_add(sess);
    d_assert(bearer, return, "Null param");
    memcpy(&bearer->qos, &pcc_rule->qos, sizeof(qos_t));
    memcpy(&bearer->qos, &pcc_rule->qos, sizeof(qos_t));

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_CREATE_BEARER_REQUEST_TYPE;
    h.teid = sess->sgw_s5c_teid;

    rv = pgw_s5c_build_create_bearer_request(&pkbuf, h.type, bearer);
    d_assert(rv == CORE_OK, return, "S11 build error");

    xact = gtp_xact_local_create(sess->sgw, &h, pkbuf);
    d_assert(xact, return, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void pgw_gx_handle_cca_initial_request(
        gtp_xact_t *xact, pgw_sess_t *sess,
        gx_cca_message_t *cca_message, gtp_create_session_request_t *req)
{
    status_t rv;
    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;
    int i = 0;

    d_assert(xact, return, "Null param");
    d_assert(sess, return, "Null param");
    d_assert(cca_message, return, "Null param");
    d_assert(req, return, "Null param");

    PGW_S5C_PATH_IN_SESSION(sess, xact);

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_CREATE_SESSION_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    rv = pgw_s5c_build_create_session_response(
            &pkbuf, h.type, sess, cca_message, req);
    d_assert(rv == CORE_OK, return, "S11 build error");

    rv = gtp_xact_update_tx(xact, &h, pkbuf);
    d_assert(rv == CORE_OK, return, "gtp_xact_update_tx error");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    for (i = 0; i < cca_message->num_of_pcc_rule; i++)
    {
        pgw_gx_handle_create_bearer_request(sess, &cca_message->pcc_rule[i]);
    }
}

void pgw_gx_handle_cca_termination_request(
        gtp_xact_t *xact, pgw_sess_t *sess,
        gx_cca_message_t *cca_message, gtp_delete_session_request_t *req)
{
    status_t rv;
    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;
    c_uint32_t sgw_s5c_teid;

    d_assert(xact, return, "Null param");
    d_assert(sess, return, "Null param");
    d_assert(cca_message, return, "Null param");
    d_assert(req, return, "Null param");

    /* backup sgw_s5c_teid in session context */
    sgw_s5c_teid = sess->sgw_s5c_teid;

    /* Remove a pgw session */
    pgw_sess_remove(sess);

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_DELETE_SESSION_RESPONSE_TYPE;
    h.teid = sgw_s5c_teid;

    rv = pgw_s5c_build_delete_session_response(
            &pkbuf, h.type, sess, cca_message, req);
    d_assert(rv == CORE_OK, return, "S11 build error");

    rv = gtp_xact_update_tx(xact, &h, pkbuf);
    d_assert(rv == CORE_OK, return, "gtp_xact_update_tx error");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}
