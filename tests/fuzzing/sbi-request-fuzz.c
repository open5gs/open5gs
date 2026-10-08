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

#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "fuzzing.h"
#include "ogs-sbi.h"
#include "yuarel.h"

#define kMinInputLength 4
#define kMaxInputLength 16384

/* Same cap as the nghttp2 server applies to the query string */
#define MAX_NUM_OF_PARAM_IN_QUERY 16

/*
 * Input: "[<STATUS> ]<METHOD> <URI>[?<QUERY>]\n(<Name>: <Value>\n)*[\n]<BODY>"
 * A leading status code parses the input as a peer NF's response. Headers end
 * at an empty line or the first non-header line. A body without Content-Type
 * is sent as JSON, so the older "<METHOD> <URI>\n<BODY>" seeds still apply.
 */

static int is_header_name_char(int c)
{
    return isalnum(c) || c == '-' || c == '_';
}

/* Returns true if a Content-Type header was set */
static bool parse_headers(char **pos, char *end, ogs_hash_t *headers)
{
    char *p = *pos;
    bool content_type = false;

    while (p < end) {
        char *name = p, *value = NULL, *eol = NULL;
        size_t n = 0;

        while (p + n < end && is_header_name_char((unsigned char)p[n]))
            n++;

        if (n == 0 || p + n >= end || p[n] != ':') {
            if (*p == '\n')
                p++;
            else if (*p == '\r' && p + 1 < end && p[1] == '\n')
                p += 2;
            break;
        }

        eol = memchr(p, '\n', end - p);
        name[n] = '\0';
        value = p + n + 1;
        while (*value == ' ')
            value++;

        if (eol) {
            *eol = '\0';
            p = eol + 1;
        } else {
            p = end;
        }

        /* The server drops headers with an empty value */
        if (*value) {
            ogs_sbi_header_set(headers, name, value);
            if (!ogs_strcasecmp(name, OGS_SBI_CONTENT_TYPE))
                content_type = true;
        }
    }

    *pos = p;
    return content_type;
}

/* Mirrors the :path handling in nghttp2-server.c */
static bool set_request_uri(ogs_sbi_request_t *request, char *uri)
{
    char *saveptr = NULL, *query = NULL;
    struct yuarel_param params[MAX_NUM_OF_PARAM_IN_QUERY + 2];
    bool ok = true;
    int j;

    request->h.uri = ogs_sbi_parse_uri(uri, "?", &saveptr);
    if (request->h.uri == NULL)
        return false;

    memset(params, 0, sizeof(params));
    query = ogs_sbi_parse_uri(NULL, "?", &saveptr);
    if (query && *query)
        yuarel_parse_query(query, '&', params, MAX_NUM_OF_PARAM_IN_QUERY + 1);

    for (j = 0; params[j].key && params[j].val; j++) {
        /* The server rejects the request outright */
        if (j >= MAX_NUM_OF_PARAM_IN_QUERY) {
            ok = false;
            break;
        }
        if (strlen(params[j].key))
            ogs_sbi_header_set(request->http.params,
                    params[j].key, params[j].val);
    }

    if (query)
        ogs_free(query);

    return ok;
}

static void set_body(ogs_sbi_http_message_t *http, const char *body, size_t len)
{
    if (len == 0)
        return;

    /* The HTTP layer always hands open5gs a NUL-terminated body */
    http->content = ogs_malloc(len + 1);
    if (http->content == NULL)
        return;
    memcpy(http->content, body, len);
    http->content[len] = '\0';
    http->content_length = len;
}

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_http_message_t *http = NULL;
    char *text = NULL, *end = NULL, *line = NULL, *pos = NULL;
    char *uri = NULL, *sp = NULL;
    bool is_response = false;
    int status = 0;

    if (Size < kMinInputLength || Size > kMaxInputLength) {
        return 0;
    }

    if (!initialized) {
        initialize();
        ogs_log_install_domain(&__ogs_sbi_domain, "sbi", OGS_LOG_NONE);
        ogs_sbi_message_init(8, 8);
    }

    text = ogs_malloc(Size + 1);
    if (text == NULL) {
        return 0;
    }
    memcpy(text, Data, Size);
    text[Size] = '\0';
    end = text + Size;

    line = text;
    pos = memchr(text, '\n', Size);
    if (pos)
        *pos++ = '\0';
    else
        pos = end;

    while (isdigit((unsigned char)*line) && status < 1000) {
        status = status * 10 + (*line++ - '0');
        is_response = true;
    }
    while (*line == ' ')
        line++;

    sp = strchr(line, ' ');
    if (sp) {
        *sp = '\0';
        uri = sp + 1;
    } else {
        uri = line + strlen(line);
    }

    if (is_response) {
        response = ogs_sbi_response_new();
        if (response == NULL)
            goto out;
        response->status = status;
        response->h.method = ogs_strdup(line);
        response->h.uri = ogs_strdup(uri);
        http = &response->http;
    } else {
        request = ogs_sbi_request_new();
        if (request == NULL)
            goto out;
        request->h.method = ogs_strdup(line);
        if (!set_request_uri(request, uri))
            goto out;
        http = &request->http;
    }

    if (!parse_headers(&pos, end, http->headers) && pos < end)
        ogs_sbi_header_set(http->headers,
                OGS_SBI_CONTENT_TYPE, OGS_SBI_CONTENT_JSON_TYPE);
    set_body(http, pos, end - pos);

    memset(&message, 0, sizeof(message));
    if (request) {
        if (ogs_sbi_parse_request(&message, request) == OGS_OK)
            ogs_sbi_message_free(&message);
    } else {
        if (ogs_sbi_parse_response(&message, response) == OGS_OK)
            ogs_sbi_message_free(&message);
    }

out:
    if (request)
        ogs_sbi_request_free(request);
    if (response)
        ogs_sbi_response_free(response);
    ogs_free(text);

    return 0;
}
