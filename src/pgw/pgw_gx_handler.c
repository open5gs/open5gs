#define TRACE_MODULE _pgw_gx_handler

#include "core_debug.h"

#include "pgw_context.h"
#include "fd/gx/gx_message.h"
#include "pgw_gtp_path.h"
#include "pgw_s5c_build.h"
#include "pgw_ipfw.h"

static status_t bearer_binding(pgw_sess_t *sess, gx_message_t *gx_message);

void pgw_gx_handle_cca_initial_request(
        pgw_sess_t *sess, gx_message_t *gx_message,
        gtp_xact_t *xact, gtp_create_session_request_t *req)
{
    status_t rv;
    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;

    d_assert(sess, return, "Null param");
    d_assert(gx_message, return, "Null param");
    d_assert(xact, return, "Null param");
    d_assert(req, return, "Null param");

    /* Send Create Session Request with Creating Default Bearer */
    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_CREATE_SESSION_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    rv = pgw_s5c_build_create_session_response(
            &pkbuf, h.type, sess, gx_message, req);
    d_assert(rv == CORE_OK, return, "S11 build error");

    rv = gtp_xact_update_tx(xact, &h, pkbuf);
    d_assert(rv == CORE_OK, return, "gtp_xact_update_tx error");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    rv = bearer_binding(sess, gx_message);
    d_assert(rv == CORE_OK, return,);
}

void pgw_gx_handle_cca_termination_request(
        pgw_sess_t *sess, gx_message_t *gx_message,
        gtp_xact_t *xact, gtp_delete_session_request_t *req)
{
    status_t rv;
    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;
    c_uint32_t sgw_s5c_teid;

    d_assert(xact, return, "Null param");
    d_assert(sess, return, "Null param");
    d_assert(gx_message, return, "Null param");
    d_assert(req, return, "Null param");

    /* backup sgw_s5c_teid in session context */
    sgw_s5c_teid = sess->sgw_s5c_teid;

    /* Remove a pgw session */
    pgw_sess_remove(sess);

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_DELETE_SESSION_RESPONSE_TYPE;
    h.teid = sgw_s5c_teid;

    rv = pgw_s5c_build_delete_session_response(
            &pkbuf, h.type, sess, gx_message, req);
    d_assert(rv == CORE_OK, return, "S11 build error");

    rv = gtp_xact_update_tx(xact, &h, pkbuf);
    d_assert(rv == CORE_OK, return, "gtp_xact_update_tx error");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void pgw_gx_handle_re_auth_request(
        pgw_sess_t *sess, gx_message_t *gx_message)
{
    status_t rv;

    rv = bearer_binding(sess, gx_message);
    d_assert(rv == CORE_OK, return,);
}

static status_t bearer_binding(pgw_sess_t *sess, gx_message_t *gx_message)
{
    status_t rv;
    int i, j;

    d_assert(sess, return CORE_ERROR,);
    d_assert(gx_message, return CORE_ERROR,);

    for (i = 0; i < gx_message->num_of_pcc_rule; i++)
    {
        gtp_xact_t *xact = NULL;
        gtp_header_t h;
        pkbuf_t *pkbuf = NULL;

        pcc_rule_t *pcc_rule = &gx_message->pcc_rule[i];
        pgw_bearer_t *bearer = pgw_bearer_find_by_qci_arp(sess, 
                    pcc_rule->qos.qci,
                    pcc_rule->qos.arp.priority_level,
                    pcc_rule->qos.arp.pre_emption_capability,
                    pcc_rule->qos.arp.pre_emption_vulnerability);
        if (bearer)
        {
            d_error("CHECK WEBUI : PCC Rule Modification is NOT implemented");
            d_error("Please remove Flow in PCC Rule");
            return CORE_ERROR;
        }

        bearer = pgw_bearer_add(sess);
        d_assert(bearer, return CORE_ERROR, "Null param");

        bearer->name = core_strdup(pcc_rule->name);
        d_assert(bearer->name, return CORE_ERROR,);

        memcpy(&bearer->qos, &pcc_rule->qos, sizeof(qos_t));

        for (j = 0; j < pcc_rule->num_of_flow; j++)
        {
            flow_t *flow = &pcc_rule->flow[j];
            pgw_rule_t rule;
            pgw_pf_t *pf = NULL;

            d_assert(flow, return CORE_ERROR, "Null param");
            d_assert(flow->description, return CORE_ERROR, "Null param");

            rv = pgw_compile_packet_filter(&rule, flow->description);
            d_assert(rv == CORE_OK, return CORE_ERROR, "Failed to compile packet filter");

            pf = pgw_pf_add(bearer, pcc_rule->precedence);
            d_assert(pf, return CORE_ERROR, "Null param");

            memcpy(&pf->rule, &rule, sizeof(pgw_rule_t));
            pf->direction = flow->direction;
        }

        memset(&h, 0, sizeof(gtp_header_t));
        h.type = GTP_CREATE_BEARER_REQUEST_TYPE;
        h.teid = sess->sgw_s5c_teid;

        rv = pgw_s5c_build_create_bearer_request(&pkbuf, h.type, bearer);
        d_assert(rv == CORE_OK, return CORE_ERROR, "S11 build error");

        xact = gtp_xact_local_create(sess->gnode, &h, pkbuf);
        d_assert(xact, return CORE_ERROR, "Null param");

        rv = gtp_xact_commit(xact);
        d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");
    }

    return CORE_OK;
}

