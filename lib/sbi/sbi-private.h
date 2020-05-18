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

#ifndef OGS_SBI_PRIVATE_H
#define OGS_SBI_PRIVATE_H

#include "ogs-core.h"
#include "yuarel.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct http_message_s {
    ogs_hash_t *params;
    ogs_hash_t *headers;

    char *content;
    size_t content_length;
} http_message_t;

typedef struct ogs_sbi_request_s {
    ogs_sbi_header_t h;
    http_message_t http;

    /* Used in microhttpd */
    bool suspended;
    ogs_poll_t *poll;
} ogs_sbi_request_t;

typedef struct ogs_sbi_response_s {
    ogs_sbi_header_t h;
    http_message_t http;

    int status;
} ogs_sbi_response_t;

#ifdef __cplusplus
}
#endif

#endif /* OGS_SBI_PRIVATE_H */
