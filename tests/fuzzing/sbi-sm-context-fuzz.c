/*
 * Copyright (C) 2019-2026 by Arthur SC Chan <arthur.chan@adalogics.com>
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
#include <string.h>

#include "fuzzing.h"
#include "ogs-sbi.h"

#define kMinInputLength 2
#define kMaxInputLength 16384

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    ogs_sbi_message_t message;
    ogs_sbi_response_t *response = NULL;
    int rv;

    if (Size < kMinInputLength || Size > kMaxInputLength) {
        return 1;
    }

    if (!initialized) {
        initialize();
        ogs_log_install_domain(&__ogs_sbi_domain, "sbi", OGS_LOG_NONE);
        ogs_sbi_message_init(8, 8);
    }

    response = ogs_sbi_response_new();
    if (response == NULL) {
        return 0;
    }

    /* Route to the SM-context create deserializer */
    response->h.method = ogs_strdup(OGS_SBI_HTTP_METHOD_POST);
    response->h.uri = ogs_strdup("/nsmf-pdusession/v1/sm-contexts");
    response->status = 0;

    /* The HTTP layer hands open5gs a NUL-terminated body */
    response->http.content = ogs_malloc(Size + 1);
    if (response->http.content) {
        memcpy(response->http.content, Data, Size);
        response->http.content[Size] = '\0';
        response->http.content_length = Size;
    }

    ogs_sbi_header_set(response->http.headers,
            OGS_SBI_CONTENT_TYPE, OGS_SBI_CONTENT_JSON_TYPE);

    rv = ogs_sbi_parse_response(&message, response);

    ogs_sbi_response_free(response);
    if (rv == OGS_OK) {
        ogs_sbi_message_free(&message);
    }

    return 0;
}
