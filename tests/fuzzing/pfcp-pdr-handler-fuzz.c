/*
 * Copyright (C) 2026 by Open5GS Contributors
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

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "fuzzing.h"
#include "ogs-app.h"
#include "ogs-pfcp.h"

#define kMinInputLength 5
#define kMaxInputLength 2048

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    ogs_pkbuf_t *pkbuf;
    ogs_pfcp_message_t *message;
    ogs_pfcp_sess_t sess;
    ogs_pfcp_sereq_flags_t sereq_flags;
    uint8_t cause_value = 0;
    uint8_t offending_ie_value = 0;

    if (size < kMinInputLength || size > kMaxInputLength)
        return 0;

    if (!initialized) {
        initialize();
        ogs_app()->pool.nf = 64;
        ogs_app()->pool.sess = 64;
        ogs_pfcp_context_init();
    }

    pkbuf = ogs_pkbuf_alloc(NULL, size);
    if (!pkbuf)
        return 0;

    ogs_pkbuf_put_data(pkbuf, data, size);

    message = ogs_pfcp_parse_msg(pkbuf);
    if (!message) {
        ogs_pkbuf_free(pkbuf);
        return 0;
    }

    if (message->h.type == OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE) {
        memset(&sess, 0, sizeof(sess));
        ogs_pfcp_pool_init(&sess);
        memset(&sereq_flags, 0, sizeof(sereq_flags));

        ogs_pfcp_handle_create_pdr(
            &sess,
            &message->pfcp_session_establishment_request.create_pdr[0],
            &sereq_flags,
            &cause_value,
            &offending_ie_value);
    }

    ogs_pkbuf_free(pkbuf);
    return 0;
}
