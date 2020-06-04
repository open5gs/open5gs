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

void testgmm_recv(test_ue_t *test_ue, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_nas_5gs_message_t message;

    ogs_assert(test_ue);
    ogs_assert(pkbuf);

    rv = ogs_nas_5gmm_decode(&message, pkbuf);
    ogs_assert(rv == OGS_OK);

    switch (message.gmm.h.message_type) {
    case OGS_NAS_5GS_AUTHENTICATION_REQUEST:
        testgmm_handle_authentication_request(test_ue,
                &message.gmm.authentication_request);
        break;
    case OGS_NAS_5GS_SECURITY_MODE_COMMAND:
        break;
    default:
        ogs_error("Unknown message[%d]", message.gmm.h.message_type);
        break;
    }

    ogs_pkbuf_free(pkbuf);
}

void testgsm_recv(test_ue_t *test_ue, ogs_pkbuf_t *pkbuf)
{
    ogs_assert(test_ue);
    ogs_assert(pkbuf);

    ogs_pkbuf_free(pkbuf);
}
