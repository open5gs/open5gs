/*
 * Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
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

#include "snow-3g.h"
#include "openssl/snow3g.h"

/* Multiply in GF(2^64) modulo x^64 + x^4 + x^3 + x + 1. */
static uint64_t snow_3g_gf_mul(uint64_t value, uint64_t factor)
{
    uint64_t product = 0;
    unsigned int bit;

    for (bit = 0; bit < 64; bit++) {
        uint64_t carry = value >> 63;

        product ^= value & (UINT64_C(0) - (factor & 1));
        value = (value << 1) ^ (UINT64_C(0x1b) & (UINT64_C(0) - carry));
        factor >>= 1;
    }

    return product;
}

void snow_3g_f9(uint8_t *key, uint32_t count, uint32_t fresh, uint32_t dir,
        uint8_t *data, uint64_t length, uint8_t *out)
{
    struct snow_key_st key_iv;
    SNOW_CTX ctx;
    uint32_t stream[5];
    uint64_t p, q, eval = 0;
    uint64_t remaining = length;
    unsigned int i;

    /* TS 35.215, SAGE Document 1, section 4.4. */
    for (i = 0; i < 4; i++)
        key_iv.key[3 - i] = ogs_buffer_to_uint64(key + 4 * i, 4);

    dir &= 1;
    key_iv.iv[3] = count;
    key_iv.iv[2] = fresh;
    key_iv.iv[1] = count ^ (dir << 31);
    key_iv.iv[0] = fresh ^ (dir << 15);

    SNOW_set_key(key_iv, &ctx);
    SNOW_gen_keystream(stream, OGS_ARRAY_SIZE(stream), &ctx);

    /* Section 4.5. Evaluate the message polynomial at P. */
    p = ((uint64_t)stream[0] << 32) | stream[1];
    q = ((uint64_t)stream[2] << 32) | stream[3];

    while (remaining >= 64) {
        eval = snow_3g_gf_mul(eval ^ ogs_buffer_to_uint64(data, 8), p);
        data += 8;
        remaining -= 64;
    }

    if (remaining) {
        int bytes = (remaining + 7) / 8;
        uint64_t block = ogs_buffer_to_uint64(data, bytes);

        /* Left-align the last block and clear bits outside the message. */
        block <<= (8 - bytes) * 8;
        block &= UINT64_MAX << (64 - remaining);
        eval = snow_3g_gf_mul(eval ^ block, p);
    }

    eval = snow_3g_gf_mul(eval ^ length, q);
    ogs_uint64_to_buffer((uint32_t)(eval >> 32) ^ stream[4], 4, out);
}
