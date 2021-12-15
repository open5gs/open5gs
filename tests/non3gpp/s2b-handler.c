/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "s2b-handler.h"
#include "gtp-path.h"

void test_s2b_handle_create_session_response(
        ogs_gtp_xact_t *xact, test_sess_t *sess,
        ogs_gtp_create_session_response_t *rsp)
{
    int rv;
    uint8_t cause_value;

    test_bearer_t *bearer = NULL;
    ogs_gtp_f_teid_t *smf_s2b_c_teid = NULL;
    ogs_gtp_f_teid_t *smf_s2b_u_teid = NULL;
    ogs_paa_t paa;

    ogs_assert(xact);
    ogs_assert(sess);
    ogs_assert(rsp);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        } else {
            ogs_error("GTP Failed [CAUSE:%d]", cause_value);
            return;
        }
    } else {
        ogs_error("No Cause");
        return;
    }

    if (rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.presence == 0) {
        ogs_error("No S5/S8 TEID");
        return;
    }
    if (rsp->bearer_contexts_created.presence == 0) {
        ogs_error("No Bearer Context");
        return;
    }
    if (rsp->bearer_contexts_created.eps_bearer_id.presence == 0) {
        ogs_error("No EBI");
        return;
    }
    if (rsp->bearer_contexts_created.s12_rnc_f_teid.presence == 0) {
        ogs_error("No S1U TEID");
        return;
    }
    if (rsp->pdn_address_allocation.presence == 0) {
        ogs_error("No PDN Address Allocation");
        return;
    }

    ogs_expect(
        rsp->pdn_address_allocation.data && rsp->pdn_address_allocation.len);

    bearer = test_bearer_find_by_sess_ebi(
                sess, rsp->bearer_contexts_created.eps_bearer_id.u8);
    ogs_assert(bearer);

    smf_s2b_c_teid = rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.data;
    ogs_assert(smf_s2b_c_teid);

    sess->smf_s2b_c_teid = be32toh(smf_s2b_c_teid->teid);

    smf_s2b_u_teid = rsp->bearer_contexts_created.s12_rnc_f_teid.data;
    ogs_assert(smf_s2b_u_teid);

    bearer->sgw_s1u_teid = be32toh(smf_s2b_u_teid->teid);
    ogs_assert(OGS_OK ==
            ogs_gtp_f_teid_to_ip(smf_s2b_u_teid, &bearer->sgw_s1u_ip));

    memcpy(&paa,
            rsp->pdn_address_allocation.data, rsp->pdn_address_allocation.len);

    switch (paa.session_type) {
    case OGS_PDU_SESSION_TYPE_IPV4:
        sess->ue_ip.ipv4 = 1;
        sess->ue_ip.addr = paa.addr;
        break;
    case OGS_PDU_SESSION_TYPE_IPV6:
        sess->ue_ip.ipv6 = 1;
        memset(sess->ue_ip.addr6, 0, 8);
        memcpy(sess->ue_ip.addr6+8, paa.addr6, 8);
        break;
    case OGS_PDU_SESSION_TYPE_IPV4V6:
        sess->ue_ip.ipv4 = 1;
        sess->ue_ip.addr = paa.both.addr;
        sess->ue_ip.ipv6 = 1;
        memset(sess->ue_ip.addr6, 0, 8);
        memcpy(sess->ue_ip.addr6+8, paa.both.addr6, 8);
        break;
    default:
        ogs_fatal("Invalid PDU Address Type [%d]", paa.session_type);
        ogs_assert_if_reached();
    }
}

void test_s2b_handle_delete_session_response(
        ogs_gtp_xact_t *xact, test_sess_t *sess,
        ogs_gtp_delete_session_response_t *rsp)
{
    int rv;
    uint8_t cause_value;

    ogs_assert(xact);
    ogs_assert(sess);
    ogs_assert(rsp);

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        ogs_assert(cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED);
    }

    test_sess_remove(sess);
}

void test_s2b_handle_create_bearer_request(
        ogs_gtp_xact_t *xact, test_sess_t *sess,
        ogs_gtp_create_bearer_request_t *req)
{
    int rv;
    test_bearer_t *linked_bearer = NULL;
    test_bearer_t *bearer = NULL;
    ogs_gtp_f_teid_t *smf_s2b_u_teid = NULL;

    ogs_assert(xact);
    ogs_assert(sess);
    ogs_assert(req);

    if (req->linked_eps_bearer_id.presence == 0) {
        ogs_error("No Linked EBI");
        return;
    }
    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        return;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        return;
    }
    if (req->bearer_contexts.s4_u_sgsn_f_teid.presence == 0) {
        ogs_error("No GTP TEID");
        return;
    }
    if (req->bearer_contexts.bearer_level_qos.presence == 0) {
        ogs_error("No QoS");
        return;
    }
    if (req->bearer_contexts.tft.presence == 0) {
        ogs_error("No TFT");
        return;
    }

    linked_bearer =
        test_bearer_find_by_sess_ebi(sess, req->linked_eps_bearer_id.u8);
    ogs_assert(linked_bearer);
    ogs_assert(linked_bearer->sess == sess);

    bearer = test_bearer_add(sess,
            linked_bearer->ebi + ogs_list_count(&sess->bearer_list));
    ogs_assert(bearer);

    /* Receive Data Plane(UL) : SMF-S2B-U */
    smf_s2b_u_teid = req->bearer_contexts.s4_u_sgsn_f_teid.data;
    ogs_assert(smf_s2b_u_teid);
    bearer->sgw_s1u_teid = be32toh(smf_s2b_u_teid->teid);
    rv = ogs_gtp_f_teid_to_ip(smf_s2b_u_teid, &bearer->sgw_s1u_ip);
    ogs_assert(rv == OGS_OK);

    ogs_assert(OGS_OK == test_s2b_send_create_bearer_response(bearer, xact));
}

void test_s2b_handle_delete_bearer_request(
        ogs_gtp_xact_t *xact, test_sess_t *sess,
        ogs_gtp_delete_bearer_request_t *req)
{
    test_bearer_t *bearer = NULL;
    ogs_gtp_cause_t *cause = NULL;

    ogs_assert(xact);
    ogs_assert(sess);
    ogs_assert(req);

    ogs_assert(req->cause.presence);
    cause = req->cause.data;
    ogs_assert(cause);

    ogs_assert(cause->value ==
            OGS_GTP_CAUSE_ACCESS_CHANGED_FROM_NON_3GPP_TO_3GPP);

    ogs_assert(req->linked_eps_bearer_id.presence);
    bearer = test_bearer_find_by_sess_ebi(sess, req->linked_eps_bearer_id.u8);
    ogs_assert(bearer);

    ogs_assert(OGS_OK == test_s2b_send_delete_bearer_response(bearer, xact));

    test_sess_remove(sess);
}
