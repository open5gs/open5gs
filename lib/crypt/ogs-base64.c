/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "core-config-private.h"

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#include "ogs-crypt.h"

/* aaaack but it's fast and const should make it shared text page. */
static const unsigned char pr2six[256] =
{
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

static bool ogs_base64_scan(
        const char *in, size_t *out_nprbytes, size_t *out_decoded_size)
{
    const unsigned char *p = NULL;
    size_t nprbytes = 0, padding = 0;
    size_t decoded_size = 0;

    if (!in)
        return false;

    p = (const unsigned char *)in;
    while (*p) {
        if (pr2six[*p] <= 63) {
            if (padding)
                return false;
            nprbytes++;
        } else if (*p == '=') {
            padding++;
            if (padding > 2)
                return false;
        } else {
            return false;
        }
        p++;
    }

    if (!nprbytes)
        return false;

    /* A single remaining base64 character cannot produce valid output. */
    if (nprbytes % 4 == 1)
        return false;

    /* If padding exists, the encoded length including padding must be aligned. */
    if (padding && ((nprbytes + padding) % 4))
        return false;

    decoded_size = ((nprbytes + 3) / 4) * 3;
    decoded_size -= (4 - nprbytes) & 3;

    if (!decoded_size)
        return false;

    if (out_nprbytes)
        *out_nprbytes = nprbytes;
    if (out_decoded_size)
        *out_decoded_size = decoded_size;

    return true;
}

int ogs_base64_decoded_size(const char *in)
{
    size_t decoded_size = 0;

    if (!ogs_base64_scan(in, NULL, &decoded_size))
        return OGS_ERROR;

    return (int)decoded_size;
}

int ogs_base64_decode_to_buffer(
        uint8_t *out, size_t out_size, const char *in)
{
    const unsigned char *bufin = NULL;
    unsigned char *bufout = NULL;
    size_t nprbytes = 0, decoded_size = 0;

    if (!out || !out_size)
        return OGS_ERROR;

    if (!ogs_base64_scan(in, &nprbytes, &decoded_size))
        return OGS_ERROR;

    if (out_size < decoded_size)
        return OGS_ERROR;

    bufout = out;
    bufin = (const unsigned char *)in;

    while (nprbytes > 4) {
        *(bufout++) =
            (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
        *(bufout++) =
            (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
        *(bufout++) =
            (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
        bufin += 4;
        nprbytes -= 4;
    }

    if (nprbytes > 1) {
        *(bufout++) =
            (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    }
    if (nprbytes > 2) {
        *(bufout++) =
            (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    }
    if (nprbytes > 3) {
        *(bufout++) =
            (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    }

    return (int)decoded_size;
}

int ogs_base64_decode_to_string(char *out, size_t out_size, const char *in)
{
    int len;

    if (!out || out_size <= 1)
        return OGS_ERROR;

    len = ogs_base64_decode_to_buffer((uint8_t *)out, out_size - 1, in);
    if (len <= 0)
        return OGS_ERROR;

    out[len] = '\0';
    return len;
}

static const char basis_64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int ogs_base64_encoded_size(size_t in_size)
{
    size_t encoded_size;

    if (in_size > (SIZE_MAX - 1) / 4 * 3)
        return OGS_ERROR;

    encoded_size = ((in_size + 2) / 3 * 4) + 1;
    return (int)encoded_size;
}

int ogs_base64_encode_from_buffer(
        char *out, size_t out_size, const uint8_t *in, size_t in_size)
{
    size_t i;
    int needed;
    char *p = NULL;

    if (!out || !out_size || (!in && in_size))
        return OGS_ERROR;

    needed = ogs_base64_encoded_size(in_size);
    if (needed <= 0 || out_size < (size_t)needed)
        return OGS_ERROR;

    p = out;
    for (i = 0; i + 2 < in_size; i += 3) {
        *p++ = basis_64[(in[i] >> 2) & 0x3F];
        *p++ = basis_64[((in[i] & 0x3) << 4) |
                        ((int) (in[i + 1] & 0xF0) >> 4)];
        *p++ = basis_64[((in[i + 1] & 0xF) << 2) |
                        ((int) (in[i + 2] & 0xC0) >> 6)];
        *p++ = basis_64[in[i + 2] & 0x3F];
    }

    if (i < in_size) {
        *p++ = basis_64[(in[i] >> 2) & 0x3F];
        if (i == (in_size - 1)) {
            *p++ = basis_64[((in[i] & 0x3) << 4)];
            *p++ = '=';
        } else {
            *p++ = basis_64[((in[i] & 0x3) << 4) |
                            ((int) (in[i + 1] & 0xF0) >> 4)];
            *p++ = basis_64[((in[i + 1] & 0xF) << 2)];
        }
        *p++ = '=';
    }

    *p++ = '\0';
    return (int)(p - out);
}

int ogs_base64_encode_from_string(
        char *out, size_t out_size, const char *in)
{
    if (!in)
        return OGS_ERROR;

    return ogs_base64_encode_from_buffer(
            out, out_size, (const uint8_t *)in, strlen(in));
}

/*
 * PEM parsing approach informed by aws-c-io (Apache-2.0).
 * https://github.com/awslabs/aws-c-io/blob/main/source/pem.c
 */
int ogs_fbase64_decode(const char *header,
        const uint8_t *data, size_t data_size, ogs_datum_t *result)
{
    static const char begin[] = "-----BEGIN ";
    static const char end[] = "-----END ";
    static const char dashes[] = "-----";
    const size_t begin_len = sizeof(begin) - 1;
    const size_t end_len = sizeof(end) - 1;
    const size_t dashes_len = sizeof(dashes) - 1;
    const uint8_t *cursor, *limit, *body = NULL, *body_end = NULL;
    size_t header_len, body_len;
    char *encoded, *out;
    uint8_t *decoded;
    int decoded_size, size, rv = OGS_ERROR;

    ogs_assert(header);
    ogs_assert(data);
    ogs_assert(data_size);
    ogs_assert(result);

    memset(result, 0, sizeof(*result));
    if (data_size > INT_MAX) {
        ogs_error("PEM input is too large");
        return OGS_ERROR;
    }

    header_len = strlen(header);
    cursor = data;
    limit = data + data_size;
    while (cursor < limit) {
        const uint8_t *line = cursor, *line_end;
        size_t line_len;

        while (cursor < limit && *cursor != '\r' && *cursor != '\n')
            cursor++;
        line_end = cursor;
        while (cursor < limit && (*cursor == '\r' || *cursor == '\n'))
            cursor++;

        while (line < line_end && (*line == ' ' || *line == '\t'))
            line++;

        if (body) {
            /* Accept an END label that differs from the BEGIN label. */
            if ((size_t)(line_end - line) >= end_len &&
                    memcmp(line, end, end_len) == 0) {
                body_end = line;
                break;
            }
            continue;
        }

        while (line_end > line && (line_end[-1] == ' ' || line_end[-1] == '\t'))
            line_end--;
        line_len = line_end - line;
        if (line_len >= begin_len + dashes_len &&
                line_len - begin_len - dashes_len == header_len &&
                memcmp(line, begin, begin_len) == 0 &&
                memcmp(line + begin_len, header, header_len) == 0 &&
                memcmp(line_end - dashes_len, dashes, dashes_len) == 0)
            body = cursor;
    }

    if (!body) {
        ogs_error("Could not find PEM BEGIN line for [%s]", header);
        return OGS_ERROR;
    }

    if (!body_end) {
        ogs_error("Could not find PEM END line for [%s]", header);
        return OGS_ERROR;
    }

    body_len = body_end - body;
    if (memchr(body, '\0', body_len)) {
        ogs_error("NUL byte in PEM body for [%s]", header);
        return OGS_ERROR;
    }

    encoded = ogs_malloc(body_len + 1);
    if (!encoded) {
        ogs_error("Could not allocate PEM body for [%s]", header);
        return OGS_ERROR;
    }

    out = encoded;
    for (; body < body_end; body++) {
        if (*body == ' ' || *body == '\t' || *body == '\r' || *body == '\n')
            continue;
        *out++ = *body;
    }
    *out = '\0';

    decoded_size = ogs_base64_decoded_size(encoded);
    if (decoded_size <= 0) {
        ogs_error("Invalid Base64 data in PEM block [%s]", header);
        goto cleanup;
    }

    decoded = ogs_malloc(decoded_size);
    if (!decoded) {
        ogs_error("Could not allocate decoded PEM data for [%s]", header);
        goto cleanup;
    }

    size = ogs_base64_decode_to_buffer(decoded, decoded_size, encoded);
    if (size != decoded_size) {
        ogs_error("Could not decode PEM block [%s]", header);
        ogs_free(decoded);
        goto cleanup;
    }

    result->data = decoded;
    result->size = size;
    rv = OGS_OK;

cleanup:
    ogs_free(encoded);
    return rv;
}

int ogs_pem_decode_curve25519_key(const char *filename, uint8_t *key)
{
    int rv;

    uint8_t buf[OGS_HUGE_LEN];
    size_t bytes_read;
    ogs_datum_t result;

#define CURVE25519_HEADER_LEN 16
    const char *_header = "302e0201 00300506 032b656e 04220420";
    uint8_t header[CURVE25519_HEADER_LEN];

    ogs_assert(filename);
    ogs_assert(key);

    rv = ogs_file_read_full(filename, buf, OGS_HUGE_LEN, &bytes_read);
    if (rv != OGS_OK) {
        ogs_error("ogs_file_read_full[%s] failed", filename);
        return OGS_ERROR;
    }

    rv = ogs_fbase64_decode("PRIVATE KEY", buf, bytes_read, &result);
    if (rv != OGS_OK) {
        ogs_error("ogs_fbase64_decode[%s] failed", filename);
        ogs_log_hexdump(OGS_LOG_ERROR, result.data, result.size);
        return OGS_ERROR;
    }

    rv = OGS_OK;

    if (result.size != 48) {
        ogs_error("Invalid size [%d]", (int)result.size);
        rv = OGS_ERROR;
        goto cleanup;
    }

    if (memcmp(ogs_hex_from_string(_header, header, sizeof(header)),
                result.data, CURVE25519_HEADER_LEN) != 0) {
        ogs_error("Invalid header [%d]", (int)result.size);
        ogs_log_hexdump(OGS_LOG_FATAL, result.data, result.size);
        rv = OGS_ERROR;
        goto cleanup;
    }

    memcpy(key, result.data+CURVE25519_HEADER_LEN, OGS_ECCKEY_LEN);

cleanup:
    if (result.data)
        ogs_free(result.data);

    return rv;
}

int ogs_pem_decode_secp256r1_key(const char *filename, uint8_t *key)
{
    int rv;

    uint8_t buf[OGS_HUGE_LEN];
    size_t bytes_read;
    ogs_datum_t result;

#define SECP256R1_HEADER_LEN 7
    const char *_header = "30770201 010420";
    uint8_t header[SECP256R1_HEADER_LEN];

    ogs_assert(filename);
    ogs_assert(key);

    rv = ogs_file_read_full(filename, buf, OGS_HUGE_LEN, &bytes_read);
    if (rv != OGS_OK) {
        ogs_error("ogs_file_read_full[%s] failed", filename);
        return OGS_ERROR;
    }

    rv = ogs_fbase64_decode("EC PRIVATE KEY", buf, bytes_read, &result);
    if (rv != OGS_OK) {
        ogs_error("ogs_fbase64_decode[%s] failed", filename);
        ogs_log_hexdump(OGS_LOG_ERROR, result.data, result.size);
        return OGS_ERROR;
    }

    rv = OGS_OK;

    if (result.size != 121) {
        ogs_error("Invalid size [%d]", (int)result.size);
        rv = OGS_ERROR;
        goto cleanup;
    }

    if (memcmp(ogs_hex_from_string(_header, header, sizeof(header)),
                result.data, SECP256R1_HEADER_LEN) != 0) {
        ogs_error("Invalid header [%d]", (int)result.size);
        ogs_log_hexdump(OGS_LOG_FATAL, result.data, result.size);
        rv = OGS_ERROR;
        goto cleanup;
    }

    memcpy(key, result.data+SECP256R1_HEADER_LEN, OGS_ECCKEY_LEN);

cleanup:
    if (result.data)
        ogs_free(result.data);

    return rv;
}
