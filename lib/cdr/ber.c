/*
 * Copyright (C) 2026 by DNL
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

#include "ber.h"

#include <string.h>

void ogs_ber_init(ogs_ber_t *ber, uint8_t *buf, size_t size)
{
    ber->buf = buf;
    ber->size = size;
    ber->pos = size;
    ber->error = 0;
}

size_t ogs_ber_len(const ogs_ber_t *ber)
{
    return ber->size - ber->pos;
}

int ogs_ber_finish(ogs_ber_t *ber)
{
    size_t len = ogs_ber_len(ber);

    if (ber->error)
        return -1;
    if (ber->pos > 0)
        memmove(ber->buf, ber->buf + ber->pos, len);
    return (int)len;
}

void ogs_ber_prepend(ogs_ber_t *ber, const uint8_t *data, size_t len)
{
    if (ber->error || ber->pos < len) {
        ber->error = 1;
        return;
    }
    ber->pos -= len;
    memcpy(ber->buf + ber->pos, data, len);
}

static void prepend_byte(ogs_ber_t *ber, uint8_t b)
{
    ogs_ber_prepend(ber, &b, 1);
}

static void prepend_length(ogs_ber_t *ber, size_t len)
{
    if (len < 0x80) {
        prepend_byte(ber, (uint8_t)len);
    } else {
        int n = 0;
        while (len) {
            prepend_byte(ber, (uint8_t)(len & 0xff));
            len >>= 8;
            n++;
        }
        prepend_byte(ber, (uint8_t)(0x80 | n));
    }
}

static void prepend_tag(ogs_ber_t *ber,
        uint8_t cls, int constructed, uint32_t number)
{
    uint8_t first = cls | (constructed ? OGS_BER_CONSTRUCTED : 0);

    if (number < 31) {
        prepend_byte(ber, first | (uint8_t)number);
    } else {
        /* high-tag-number form, base-128, last septet first */
        prepend_byte(ber, (uint8_t)(number & 0x7f));
        number >>= 7;
        while (number) {
            prepend_byte(ber, (uint8_t)(0x80 | (number & 0x7f)));
            number >>= 7;
        }
        prepend_byte(ber, first | 0x1f);
    }
}

void ogs_ber_prepend_header(ogs_ber_t *ber,
        uint8_t cls, int constructed, uint32_t tag_number, size_t len)
{
    prepend_length(ber, len);
    prepend_tag(ber, cls, constructed, tag_number);
}

void ogs_ber_ctx_octets(ogs_ber_t *ber,
        uint32_t tag, const uint8_t *data, size_t len)
{
    ogs_ber_prepend(ber, data, len);
    ogs_ber_prepend_header(ber, OGS_BER_CLASS_CONTEXT, 0, tag, len);
}

void ogs_ber_ctx_uint(ogs_ber_t *ber, uint32_t tag, uint64_t value)
{
    uint8_t tmp[9];
    int n = 0;

    do {
        tmp[n++] = (uint8_t)(value & 0xff);
        value >>= 8;
    } while (value);

    /* keep sign bit clear: INTEGER is signed in BER */
    if (tmp[n - 1] & 0x80)
        tmp[n++] = 0x00;

    /* tmp[] holds LSB first; write out reversed */
    {
        uint8_t out[9];
        int i;
        for (i = 0; i < n; i++)
            out[i] = tmp[n - 1 - i];
        ogs_ber_ctx_octets(ber, tag, out, n);
    }
}

void ogs_ber_ctx_bool(ogs_ber_t *ber, uint32_t tag, int value)
{
    uint8_t b = value ? 0xff : 0x00;
    ogs_ber_ctx_octets(ber, tag, &b, 1);
}

void ogs_ber_ctx_null(ogs_ber_t *ber, uint32_t tag)
{
    ogs_ber_ctx_octets(ber, tag, NULL, 0);
}

void ogs_ber_ctx_single_bit(ogs_ber_t *ber, uint32_t tag, unsigned int bit)
{
    /* BIT STRING: leading unused-bits octet, bit 0 = MSB of first octet */
    uint8_t v[1 + 8] = { 0 };
    unsigned int nbytes = bit / 8 + 1;

    v[0] = 7 - (bit % 8);               /* unused bits in last octet */
    v[1 + bit / 8] = (uint8_t)(0x80 >> (bit % 8));
    ogs_ber_ctx_octets(ber, tag, v, 1 + nbytes);
}

size_t ogs_ber_mark(const ogs_ber_t *ber)
{
    return ber->pos;
}

void ogs_ber_wrap(ogs_ber_t *ber, uint8_t cls, uint32_t tag, size_t mark)
{
    size_t len = mark - ber->pos;
    ogs_ber_prepend_header(ber, cls, 1, tag, len);
}
