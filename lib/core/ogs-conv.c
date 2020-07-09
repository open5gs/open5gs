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

#include "core-config-private.h"

#if HAVE_CTYPE_H
#include <ctype.h>
#endif

#include "ogs-core.h"

void *ogs_ascii_to_hex(char *in, int in_len, void *out, int out_len)
{
    int i = 0, j = 0, k = 0, hex;
    uint8_t *out_p = out;

    while(i < in_len && j < out_len) {
        if (!isspace(in[i])) {
            hex = isdigit(in[i]) ? in[i] - '0' : 
                islower(in[i]) ? in[i] - 'a' + 10 : in[i] - 'A' + 10;
            if ((k & 0x1) == 0) {
                out_p[j] = (hex << 4);
            } else {
                out_p[j] |= hex;
                j++;
            }
            k++;
        }
        i++;
    }

    return out;
}

void *ogs_hex_to_ascii(void *in, int in_len, void *out, int out_len)
{
    char *p;
    int i = 0, l, off = 0;

    p = out;
    p[0] = 0;

    l = (in_len - off) > out_len ? out_len : in_len - off;
    for (i = 0; i < l; i++) {
        p += sprintf(p, "%02x", ((char*)in)[off+i] & 0xff);
    }

    return out;
}

void *ogs_uint64_to_buffer(uint64_t num, int size, void *buffer)
{
    int i;
    uint8_t *buffer_p = buffer;
    for (i = 0; i < size; i++) 
        buffer_p[i] = (num >> ((size-1-i) * 8)) & 0xff; 

    return buffer;
}

uint64_t ogs_buffer_to_uint64(void *buffer, int size)
{
    uint64_t num = 0;
    uint8_t *buffer_p = buffer;
    int i;

    for (i = 0; i < size; i++) {
        num |= (((uint64_t)buffer_p[i]) << ((size-1-i) * 8));
    }

    return num;
}

void *ogs_bcd_to_buffer(const char *in, void *out, int *out_len)
{
    int i = 0;
    uint8_t *out_p = out;
    int in_len = strlen(in);

    for (i = 0; i < in_len; i++) {
        if (i & 0x01)
            out_p[i>>1] = out_p[i>>1] | (((in[i] - 0x30) << 4) & 0xF0);
        else
            out_p[i>>1] = (in[i] - 0x30) & 0x0F;
    }

    *out_len = (in_len + 1) / 2;
    if (in_len & 0x01) {
        out_p[(*out_len)-1] |= 0xF0;
    }

    return out;
}

void *ogs_bcd_to_buffer_reverse_order(const char *in, void *out, int *out_len)
{
    int i = 0;
    uint8_t *out_p = out;
    int in_len = strlen(in);

    for (i = 0; i < in_len; i++) {
        if (i & 0x01)
            out_p[i>>1] = out_p[i>>1] | ((in[i] - 0x30) & 0x0F);
        else
            out_p[i>>1] = ((in[i] - 0x30) << 4) & 0xF0;
    }

    *out_len = (in_len + 1) / 2;
    if (in_len & 0x01) {
        out_p[(*out_len)-1] |= 0xF0;
    }

    return out;
}

void *ogs_buffer_to_bcd(uint8_t *in, int in_len, void *out)
{
    int i = 0;
    uint8_t *out_p = out;

    for (i = 0; i < in_len-1; i++) {
        out_p[i*2] = 0x30 + (in[i] & 0x0F);
        out_p[i*2+1] = 0x30 + ((in[i] & 0xF0) >> 4);
    }

    if ((in[i] & 0xF0) == 0xF0) {
        out_p[i*2] = 0x30 + (in[i] & 0x0F);
        out_p[i*2+1] = 0;
    } else {
        out_p[i*2] = 0x30 + (in[i] & 0x0F);
        out_p[i*2+1] = 0x30 + ((in[i] & 0xF0) >> 4);
        out_p[i*2+2] = 0;
    }

    return out;
}

char ogs_from_hex(char ch)
{
    return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

char *ogs_uint24_to_string(ogs_uint24_t x)
{
    return ogs_msprintf("%06x", x.v);
}

char *ogs_uint28_to_string(uint32_t x)
{
    return ogs_msprintf("%07x", x);
}

char *ogs_uint32_to_string(uint32_t x)
{
    return ogs_msprintf("%08x", x);
}

char *ogs_uint36_to_string(uint64_t x)
{
    return ogs_msprintf("%09llx", (long long)x);
}

ogs_uint24_t ogs_uint24_from_string(char *str)
{
    ogs_uint24_t x;

    ogs_assert(str);
    ogs_ascii_to_hex(str, strlen(str), &x, 3);
    return ogs_be24toh(x);
}

uint32_t ogs_uint28_from_string(char *str)
{
    uint32_t x;

    ogs_assert(str);

    x = 0;
    ogs_ascii_to_hex(str, strlen(str), &x, 4);

    return be32toh(x) >> 4;
}

uint32_t ogs_uint32_from_string(char *str)
{
    uint32_t x;

    ogs_assert(str);
    ogs_ascii_to_hex(str, strlen(str), &x, 4);
    return be32toh(x);
}

uint64_t ogs_uint36_from_string(char *str)
{
    uint64_t x;

    ogs_assert(str);

    x = 0;
    ogs_ascii_to_hex(str, strlen(str), &x, 5);

    return be64toh(x) >> 28;
}
