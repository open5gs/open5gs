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

#include "pgw-context.h"
#include "pgw-gtp-path.h"
#include "pgw-s5c-build.h"
#include "pgw-gx-handler.h"
#include "pgw-ipfw.h"

static int bearer_binding(pgw_sess_t *sess, ogs_diam_gx_message_t *gx_message);

static void timeout(ogs_gtp_xact_t *xact, void *data)
{
    pgw_sess_t *sess = data;
    uint8_t type = 0;

    ogs_assert(sess);

    type = xact->seq[0].type;

    ogs_debug("GTP Timeout : SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x] "
            "Message-Type[%d]", sess->sgw_s5c_teid, sess->pgw_s5c_teid, type);
}

void pgw_gx_handle_cca_initial_request(
        pgw_sess_t *sess, ogs_diam_gx_message_t *gx_message,
        ogs_gtp_xact_t *xact, ogs_gtp_create_session_request_t *req)
{
    int rv;
    ogs_gtp_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    ogs_assert(gx_message);
    ogs_assert(xact);
    ogs_assert(req);

    /* Send Create Session Request with Creating Default Bearer */
    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_CREATE_SESSION_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    rv = pgw_s5c_build_create_session_response(
            &pkbuf, h.type, sess, gx_message, req);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    rv = bearer_binding(sess, gx_message);
    ogs_assert(rv == OGS_OK);
}

void pgw_gx_handle_cca_termination_request(
        pgw_sess_t *sess, ogs_diam_gx_message_t *gx_message,
        ogs_gtp_xact_t *xact, ogs_gtp_delete_session_request_t *req)
{
    int rv;
    ogs_gtp_header_t h;
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

    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_DELETE_SESSION_RESPONSE_TYPE;
    h.teid = sgw_s5c_teid;

    rv = pgw_s5c_build_delete_session_response(
            &pkbuf, h.type, gx_message, req);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);
}

void pgw_gx_handle_re_auth_request(
        pgw_sess_t *sess, ogs_diam_gx_message_t *gx_message)
{
    int rv;

    rv = bearer_binding(sess, gx_message);
    ogs_assert(rv == OGS_OK);
}

static int bearer_binding(pgw_sess_t *sess, ogs_diam_gx_message_t *gx_message)
{
    int rv;
    int i, j;

    ogs_assert(sess);
    ogs_assert(gx_message);

    for (i = 0; i < gx_message->num_of_pcc_rule; i++) {
        ogs_gtp_xact_t *xact = NULL;
        ogs_gtp_header_t h;
        ogs_pkbuf_t *pkbuf = NULL;
        pgw_bearer_t *bearer = NULL;

        ogs_pcc_rule_t *pcc_rule = &gx_message->pcc_rule[i];
        int bearer_created = 0;
        int qos_presence = 0;
        int tft_presence = 0;

        ogs_assert(pcc_rule);
        if (pcc_rule->name == NULL) {
            ogs_error("No PCC Rule Name");
            continue;
        }

        if (pcc_rule->type == OGS_PCC_RULE_TYPE_INSTALL) {
            bearer = pgw_bearer_find_by_qci_arp(sess, 
                        pcc_rule->qos.qci,
                        pcc_rule->qos.arp.priority_level,
                        pcc_rule->qos.arp.pre_emption_capability,
                        pcc_rule->qos.arp.pre_emption_vulnerability);
            if (!bearer) {
                bearer = pgw_bearer_add(sess);
                ogs_assert(bearer);

                bearer->name = ogs_strdup(pcc_rule->name);
                ogs_assert(bearer->name);

                memcpy(&bearer->qos, &pcc_rule->qos, sizeof(ogs_qos_t));
                ogs_assert(pcc_rule->num_of_flow);

                bearer_created = 1;
            } else {
                ogs_assert(strcmp(bearer->name, pcc_rule->name) == 0);

                if (pcc_rule->num_of_flow) {
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
                    bearer->qos.gbr.uplink != pcc_rule->qos.gbr.uplink)) {
                    /* Update QoS parameter */
                    memcpy(&bearer->qos, &pcc_rule->qos, sizeof(ogs_qos_t));

                    /* Update Bearer Request will encode updated QoS parameter */
                    qos_presence = 1;
                }

                if (tft_presence == 0 && qos_presence == 0) {
                    ogs_warn("[IGNORE] Update Bearer Request : "
                            "Both QoS and TFT is NULL");
                    continue;
                }
            }

            for (j = 0; j < pcc_rule->num_of_flow; j++) {
                ogs_flow_t *flow = &pcc_rule->flow[j];
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

            memset(&h, 0, sizeof(ogs_gtp_header_t));

            if (bearer_created == 1) {
                h.type = OGS_GTP_CREATE_BEARER_REQUEST_TYPE;
                h.teid = sess->sgw_s5c_teid;

                rv = pgw_s5c_build_create_bearer_request(&pkbuf, h.type, bearer);
                ogs_assert(rv == OGS_OK);
            } else {
                h.type = OGS_GTP_UPDATE_BEARER_REQUEST_TYPE;
                h.teid = sess->sgw_s5c_teid;

                rv = pgw_s5c_build_update_bearer_request(
                        &pkbuf, h.type, bearer, qos_presence, tft_presence);
                ogs_assert(rv == OGS_OK);
            }

            xact = ogs_gtp_xact_local_create(
                    sess->gnode, &h, pkbuf, timeout, sess);
            ogs_assert(xact);

            rv = ogs_gtp_xact_commit(xact);
            ogs_assert(rv == OGS_OK);
        } else if (pcc_rule->type == OGS_PCC_RULE_TYPE_REMOVE) {
            bearer = pgw_bearer_find_by_name(sess, pcc_rule->name);
            ogs_assert(bearer);

            memset(&h, 0, sizeof(ogs_gtp_header_t));
            h.type = OGS_GTP_DELETE_BEARER_REQUEST_TYPE;
            h.teid = sess->sgw_s5c_teid;

            rv = pgw_s5c_build_delete_bearer_request(&pkbuf, h.type, bearer);
            ogs_assert(rv == OGS_OK);

            xact = ogs_gtp_xact_local_create(
                    sess->gnode, &h, pkbuf, timeout, sess);
            ogs_assert(xact);

            rv = ogs_gtp_xact_commit(xact);
            ogs_assert(rv == OGS_OK);

            return OGS_OK;
        }
        else
            ogs_assert_if_reached();
    }

    return OGS_OK;
}

