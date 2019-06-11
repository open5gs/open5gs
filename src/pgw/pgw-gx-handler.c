#include "pgw-context.h"
#include "fd/gx/gx-message.h"
#include "pgw-gtp-path.h"
#include "pgw-s5c-build.h"
#include "pgw-ipfw.h"

static int bearer_binding(pgw_sess_t *sess, gx_message_t *gx_message);

void pgw_gx_handle_cca_initial_request(
        pgw_sess_t *sess, gx_message_t *gx_message,
        gtp_xact_t *xact, gtp_create_session_request_t *req)
{
    int rv;
    gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    ogs_assert(gx_message);
    ogs_assert(xact);
    ogs_assert(req);

    /* Send Create Session Request with Creating Default Bearer */
    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_CREATE_SESSION_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    rv = pgw_s5c_build_create_session_response(
            &pkbuf, h.type, sess, gx_message, req);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(xact, &h, pkbuf);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    rv = bearer_binding(sess, gx_message);
    ogs_assert(rv == OGS_OK);
}

void pgw_gx_handle_cca_termination_request(
        pgw_sess_t *sess, gx_message_t *gx_message,
        gtp_xact_t *xact, gtp_delete_session_request_t *req)
{
    int rv;
    gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;
    uint32_t sgw_s5c_teid;

    ogs_assert(xact);
    ogs_assert(sess);
    ogs_assert(gx_message);
    ogs_assert(req);

    /* backup sgw_s5c_teid in session context */
    sgw_s5c_teid = sess->sgw_s5c_teid;

    ogs_debug("[PGW] Delete Session Response");
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    /* Remove a pgw session */
    pgw_sess_remove(sess);

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_DELETE_SESSION_RESPONSE_TYPE;
    h.teid = sgw_s5c_teid;

    rv = pgw_s5c_build_delete_session_response(
            &pkbuf, h.type, gx_message, req);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(xact, &h, pkbuf);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);
}

void pgw_gx_handle_re_auth_request(
        pgw_sess_t *sess, gx_message_t *gx_message)
{
    int rv;

    rv = bearer_binding(sess, gx_message);
    ogs_assert(rv == OGS_OK);
}

static int bearer_binding(pgw_sess_t *sess, gx_message_t *gx_message)
{
    int rv;
    int i, j;

    ogs_assert(sess);
    ogs_assert(gx_message);

    for (i = 0; i < gx_message->num_of_pcc_rule; i++)
    {
        gtp_xact_t *xact = NULL;
        gtp_header_t h;
        ogs_pkbuf_t *pkbuf = NULL;
        pgw_bearer_t *bearer = NULL;

        pcc_rule_t *pcc_rule = &gx_message->pcc_rule[i];
        int bearer_created = 0;
        int qos_presence = 0;
        int tft_presence = 0;

        ogs_assert(pcc_rule);
        if (pcc_rule->name == NULL)
        {
            ogs_error("No PCC Rule Name");
            continue;
        }

        if (pcc_rule->type == PCC_RULE_TYPE_INSTALL)
        {
            bearer = pgw_bearer_find_by_qci_arp(sess, 
                        pcc_rule->qos.qci,
                        pcc_rule->qos.arp.priority_level,
                        pcc_rule->qos.arp.pre_emption_capability,
                        pcc_rule->qos.arp.pre_emption_vulnerability);
            if (!bearer)
            {
                bearer = pgw_bearer_add(sess);
                ogs_assert(bearer);

                bearer->name = ogs_strdup(pcc_rule->name);
                ogs_assert(bearer->name);

                memcpy(&bearer->qos, &pcc_rule->qos, sizeof(qos_t));
                ogs_assert(pcc_rule->num_of_flow);

                bearer_created = 1;
            }
            else
            {
                ogs_assert(strcmp(bearer->name, pcc_rule->name) == 0);

                if (pcc_rule->num_of_flow)
                {
                    /* Remove all previous flow */
                    pgw_pf_remove_all(bearer);

                    /* Update Bearer Request will encode updated TFT */
                    tft_presence = 1;
                }

                if ((pcc_rule->qos.mbr.downlink &&
                    bearer->qos.mbr.downlink != pcc_rule->qos.mbr.downlink) ||
                    (pcc_rule->qos.mbr.uplink &&
                     bearer->qos.mbr.uplink != pcc_rule->qos.mbr.uplink) ||
                    (pcc_rule->qos.gbr.downlink &&
                    bearer->qos.gbr.downlink != pcc_rule->qos.gbr.downlink) ||
                    (pcc_rule->qos.gbr.uplink &&
                    bearer->qos.gbr.uplink != pcc_rule->qos.gbr.uplink))
                {
                    /* Update QoS parameter */
                    memcpy(&bearer->qos, &pcc_rule->qos, sizeof(qos_t));

                    /* Update Bearer Request will encode updated QoS parameter */
                    qos_presence = 1;
                }

                if (tft_presence == 0 && qos_presence == 0)
                {
                    ogs_warn("[IGNORE] Update Bearer Request : "
                            "Both QoS and TFT is NULL");
                    continue;
                }
            }

            for (j = 0; j < pcc_rule->num_of_flow; j++)
            {
                flow_t *flow = &pcc_rule->flow[j];
                pgw_rule_t rule;
                pgw_pf_t *pf = NULL;

                ogs_assert(flow);
                ogs_assert(flow->description);

                rv = pgw_compile_packet_filter(&rule, flow->description);
                ogs_assert(rv == OGS_OK);

                pf = pgw_pf_add(bearer, pcc_rule->precedence);
                ogs_assert(pf);

                memcpy(&pf->rule, &rule, sizeof(pgw_rule_t));
                pf->direction = flow->direction;
            }

            memset(&h, 0, sizeof(gtp_header_t));

            if (bearer_created == 1)
            {
                h.type = GTP_CREATE_BEARER_REQUEST_TYPE;
                h.teid = sess->sgw_s5c_teid;

                rv = pgw_s5c_build_create_bearer_request(&pkbuf, h.type, bearer);
                ogs_assert(rv == OGS_OK);
            }
            else
            {
                h.type = GTP_UPDATE_BEARER_REQUEST_TYPE;
                h.teid = sess->sgw_s5c_teid;

                rv = pgw_s5c_build_update_bearer_request(
                        &pkbuf, h.type, bearer, qos_presence, tft_presence);
                ogs_assert(rv == OGS_OK);
            }

            xact = gtp_xact_local_create(sess->gnode, &h, pkbuf);
            ogs_assert(xact);

            rv = gtp_xact_commit(xact);
            ogs_assert(rv == OGS_OK);
        }
        else if (pcc_rule->type == PCC_RULE_TYPE_REMOVE)
        {
            bearer = pgw_bearer_find_by_name(sess, pcc_rule->name);
            ogs_assert(bearer);

            memset(&h, 0, sizeof(gtp_header_t));
            h.type = GTP_DELETE_BEARER_REQUEST_TYPE;
            h.teid = sess->sgw_s5c_teid;

            rv = pgw_s5c_build_delete_bearer_request(&pkbuf, h.type, bearer);
            ogs_assert(rv == OGS_OK);

            xact = gtp_xact_local_create(sess->gnode, &h, pkbuf);
            ogs_assert(xact);

            rv = gtp_xact_commit(xact);
            ogs_assert(rv == OGS_OK);

            return OGS_OK;
        }
        else
            ogs_assert_if_reached();
    }

    return OGS_OK;
}

