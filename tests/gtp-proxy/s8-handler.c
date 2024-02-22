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

#include "s8-handler.h"
#include "gtp-path.h"

void test_s8_handle_create_session_response(
        ogs_gtp_xact_t *xact, test_sess_t *sess,
        ogs_gtp2_create_session_response_t *rsp)
{
    int rv, i;
    uint8_t cause_value;

    test_bearer_t *bearer = NULL;
    ogs_gtp2_f_teid_t *smf_s5_c_teid = NULL;
    ogs_gtp2_f_teid_t *smf_s5_u_teid = NULL;
    ogs_paa_t paa;

    ogs_assert(xact);
    ogs_assert(sess);
    ogs_assert(rsp);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    if (rsp->cause.presence) {
        ogs_gtp2_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
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
    if (rsp->pdn_address_allocation.presence == 0) {
        ogs_error("No PDN Address Allocation");
        return;
    }

    ogs_expect(
        rsp->pdn_address_allocation.data && rsp->pdn_address_allocation.len);

    for (i = 0; i < OGS_BEARER_PER_UE; i++) {
        if (rsp->bearer_contexts_created[i].presence == 0) {
            break;
        }
        if (rsp->bearer_contexts_created[i].eps_bearer_id.presence == 0) {
            ogs_error("No EBI");
            break;
        }
        // XXX It should be s5_s8_u_pgw_f_teid but for libgtp parses it into s5_s8_u_sgw_f_teid
        if (rsp->bearer_contexts_created[i].s5_s8_u_sgw_f_teid.presence == 0) {
            ogs_error("No S5/S8 GTP-U TEID");
            break;
        }

        bearer = test_bearer_find_by_sess_ebi(
                    sess, rsp->bearer_contexts_created[i].eps_bearer_id.u8);
        ogs_assert(bearer);

        smf_s5_u_teid = rsp->bearer_contexts_created[i].s5_s8_u_sgw_f_teid.data;
        ogs_assert(smf_s5_u_teid);

        bearer->sgw_s1u_teid = be32toh(smf_s5_u_teid->teid);
        ogs_assert(OGS_OK ==
                ogs_gtp2_f_teid_to_ip(smf_s5_u_teid, &bearer->sgw_s1u_ip));

    }

    smf_s5_c_teid = rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.data;
    ogs_assert(smf_s5_c_teid);

    sess->smf_s2b_c_teid = be32toh(smf_s5_c_teid->teid);

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

void test_s8_handle_delete_session_response(
        ogs_gtp_xact_t *xact, test_sess_t *sess,
        ogs_gtp2_delete_session_response_t *rsp)
{
    int rv;
    uint8_t cause_value;

    ogs_assert(xact);
    ogs_assert(sess);
    ogs_assert(rsp);

    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    if (rsp->cause.presence) {
        ogs_gtp2_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);
    }

    test_sess_remove(sess);
}
