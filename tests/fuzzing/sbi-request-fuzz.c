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

#define kMinInputLength 4
#define kMaxInputLength 16384

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    const uint8_t *nl = NULL;
    size_t line_len = 0, body_len = 0;
    char *line = NULL, *sp = NULL;

    if (Size < kMinInputLength || Size > kMaxInputLength) {
        return 0;
    }

    if (!initialized) {
        initialize();
        ogs_log_install_domain(&__ogs_sbi_domain, "sbi", OGS_LOG_NONE);
        ogs_sbi_message_init(8, 8);
    }

    /* Split "<METHOD> <URI>\n<BODY>" */
    nl = (const uint8_t *)memchr(Data, '\n', Size);
    line_len = nl ? (size_t)(nl - Data) : Size;
    body_len = nl ? Size - line_len - 1 : 0;

    line = ogs_malloc(line_len + 1);
    if (line == NULL) {
        return 0;
    }
    memcpy(line, Data, line_len);
    line[line_len] = '\0';

    request = ogs_sbi_request_new();
    if (request == NULL) {
        ogs_free(line);
        return 0;
    }

    sp = strchr(line, ' ');
    if (sp) {
        *sp = '\0';
        request->h.method = ogs_strdup(line);
        request->h.uri = ogs_strdup(sp + 1);
    } else {
        request->h.method = ogs_strdup(line);
        request->h.uri = ogs_strdup("");
    }

    /* The HTTP layer always hands open5gs a NUL-terminated body */
    if (body_len) {
        request->http.content = ogs_malloc(body_len + 1);
        if (request->http.content) {
            memcpy(request->http.content, nl + 1, body_len);
            request->http.content[body_len] = '\0';
            request->http.content_length = body_len;
        }
    }

    memset(&message, 0, sizeof(message));
    if (ogs_sbi_parse_request(&message, request) == OGS_OK) {
        ogs_sbi_message_free(&message);
    }

    ogs_sbi_request_free(request);
    ogs_free(line);

    return 0;
}
