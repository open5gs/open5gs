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

#include "test-common.h"

void testesm_handle_activate_default_eps_bearer_context_request(
        test_ue_t *test_ue, ogs_nas_eps_message_t *message)
{
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    ogs_nas_eps_activate_default_eps_bearer_context_request_t
        *activate_default_eps_bearer_context_request = NULL;

    ogs_nas_pdn_address_t *pdn_address = NULL;
    ogs_nas_access_point_name_t *access_point_name = NULL;

    ogs_assert(message);

    activate_default_eps_bearer_context_request =
            &message->esm.activate_default_eps_bearer_context_request;

    pdn_address = &activate_default_eps_bearer_context_request->pdn_address;
    access_point_name =
        &activate_default_eps_bearer_context_request->access_point_name;

    sess = test_sess_find_by_apn(test_ue, access_point_name->apn);
    ogs_assert(sess);
    bearer = test_bearer_find_by_sess_ebi(
            sess, message->esm.h.eps_bearer_identity);
    if (!bearer)
        bearer = test_bearer_add(sess, message->esm.h.eps_bearer_identity);
    ogs_assert(bearer);

    switch (pdn_address->pdn_type) {
    case OGS_PDU_SESSION_TYPE_IPV4:
        sess->ue_ip.ipv4 = 1;
        sess->ue_ip.addr = pdn_address->addr;
        break;
    case OGS_PDU_SESSION_TYPE_IPV6:
        sess->ue_ip.ipv6 = 1;
        memcpy(sess->ue_ip.addr6, "\xca\xfe\x00\x00\x00\x00\x00\x00", 8);
        memcpy(sess->ue_ip.addr6+8, pdn_address->addr6, 8);
        break;
    case OGS_PDU_SESSION_TYPE_IPV4V6:
        sess->ue_ip.ipv4 = 1;
        sess->ue_ip.addr = pdn_address->both.addr;
        sess->ue_ip.ipv6 = 1;
        memcpy(sess->ue_ip.addr6, "\xca\xfe\x00\x00\x00\x00\x00\x00", 8);
        memcpy(sess->ue_ip.addr6+8, pdn_address->both.addr6, 8);
        break;
    default:
        ogs_fatal("Invalid PDU Address Type [%d]", pdn_address->pdn_type);
        ogs_assert_if_reached();
    }
}

void testesm_handle_activate_dedicated_eps_bearer_context_request(
        test_ue_t *test_ue, ogs_nas_eps_message_t *message)
{
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    ogs_nas_eps_activate_dedicated_eps_bearer_context_request_t
        *activate_dedicated_eps_bearer_context_request = NULL;

    ogs_nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity = NULL;

    ogs_assert(message);

    activate_dedicated_eps_bearer_context_request =
            &message->esm.activate_dedicated_eps_bearer_context_request;
    linked_eps_bearer_identity =
    &activate_dedicated_eps_bearer_context_request->linked_eps_bearer_identity;

    bearer = test_bearer_find_by_ue_ebi(
            test_ue, linked_eps_bearer_identity->eps_bearer_identity);
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    bearer = test_bearer_find_by_sess_ebi(
            sess, message->esm.h.eps_bearer_identity);
    if (!bearer)
        bearer = test_bearer_add(sess, message->esm.h.eps_bearer_identity);
    ogs_assert(bearer);
}

void testesm_handle_deactivate_eps_bearer_context_request(
        test_ue_t *test_ue, ogs_nas_eps_message_t *message)
{
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;
    uint8_t ebi;

    ogs_assert(message);

    ebi = message->esm.h.eps_bearer_identity;
    ogs_assert(ebi);

    bearer = test_bearer_find_by_ue_ebi(test_ue, ebi);
    ogs_assert(bearer);
}
