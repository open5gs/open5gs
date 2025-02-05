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

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#include "ogs-core.h"

int ogs_ascii_to_hex(char *in, int in_len, void *out, int out_len)
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

    return j;
}

void *ogs_hex_to_ascii(const void *in, int in_len, void *out, int out_len)
{
    char *p, *last;
    int i = 0, l, off = 0;

    p = out;
    last = p + out_len;
    p[0] = 0;

    l = (in_len - off) > out_len ? out_len : in_len - off;
    for (i = 0; i < l; i++) {
        p = ogs_slprintf(p, last, "%02x", ((char*)in)[off+i] & 0xff);
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

char ogs_to_hex(char ch)
{
    static char hex[] = "0123456789ABCDEF";
    return hex[ch & 15];
}

char *ogs_uint24_to_0string(ogs_uint24_t x)
{
    return ogs_msprintf("%06x", x.v);
}

char *ogs_uint28_to_0string(uint32_t x)
{
    return ogs_msprintf("%07x", x);
}

char *ogs_uint32_to_0string(uint32_t x)
{
    return ogs_msprintf("%08x", x);
}

char *ogs_uint36_to_0string(uint64_t x)
{
    return ogs_msprintf("%09llx", (long long)x);
}

char *ogs_uint64_to_0string(uint64_t x)
{
    return ogs_msprintf("%016llx", (long long)x);
}

char *ogs_uint64_to_string(uint64_t x)
{
    char *str, *p, *dup;

    str = ogs_uint64_to_0string(x);
    if (!str) {
        ogs_error("ogs_uint64_to_0string[%lld] failed", (long long)x);
        return NULL;
    }

    p = ogs_left_trimcharacter(str, '0');
    if (!p) {
        ogs_error("ogs_left_trimcharacter[%s] failld", str);
        return NULL;
    }

    dup = ogs_strdup(p);
    ogs_free(str);

    return dup;
}

ogs_uint24_t ogs_uint24_from_string(char *str, int base)
{
    ogs_uint24_t x;

    ogs_assert(str);

    x.v = ogs_uint64_from_string(str, base);
    return x;
}

uint64_t ogs_uint64_from_string(char *str, int base)
{
    uint64_t x;

    ogs_assert(str);

    if (strlen(str) == 0)
        return 0;

    errno = 0;
    x = strtoll(str, NULL, base);

    if ((errno == ERANGE && (x == LONG_MAX || x == LONG_MIN)) ||
            (errno != 0 && x == 0)) {
        ogs_log_message(OGS_LOG_FATAL, ogs_errno, "strtoll()) failed [%lld]",
                (long long)x);
        ogs_assert_if_reached();
    }

    return x;
}

double *ogs_alloc_double(double value)
{
    double *mem = (double *)ogs_calloc(1, sizeof(double));
    if (!mem) {
        ogs_error("No memory");
        return NULL;
    }

    *mem = value;

    return mem;
}
