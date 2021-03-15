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

void testgsm_handle_pdu_session_establishment_accept(test_sess_t *sess,
        ogs_nas_5gs_pdu_session_establishment_accept_t
            *pdu_session_establishment_accept)
{
    ogs_nas_pdu_address_t *pdu_address = NULL;

    ogs_assert(sess);
    ogs_assert(pdu_session_establishment_accept);

    pdu_address = &pdu_session_establishment_accept->pdu_address;

    if (pdu_session_establishment_accept->presencemask &
            OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_PDU_ADDRESS_PRESENT) {
        switch (pdu_address->pdn_type) {
        case OGS_PDU_SESSION_TYPE_IPV4:
            sess->ue_ip.ipv4 = 1;
            sess->ue_ip.addr = pdu_address->addr;
            break;
        case OGS_PDU_SESSION_TYPE_IPV6:
            sess->ue_ip.ipv6 = 1;
            memset(sess->ue_ip.addr6, 0, 8);
            memcpy(sess->ue_ip.addr6+8, pdu_address->addr6, 8);
            break;
        case OGS_PDU_SESSION_TYPE_IPV4V6:
            sess->ue_ip.ipv4 = 1;
            sess->ue_ip.addr = pdu_address->both.addr;
            sess->ue_ip.ipv6 = 1;
            memset(sess->ue_ip.addr6, 0, 8);
            memcpy(sess->ue_ip.addr6+8, pdu_address->both.addr6, 8);
            break;
        default:
            ogs_fatal("Invalid PDU Address Type [%d]", pdu_address->pdn_type);
            ogs_assert_if_reached();
        }

    }
}

void testgsm_handle_pdu_session_modification_command(test_sess_t *sess,
        ogs_nas_5gs_pdu_session_modification_command_t
            *pdu_session_modification_command)
{
    ogs_assert(sess);
    ogs_assert(pdu_session_modification_command);
}
