/*
 * Copyright 2002-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*
 * Copyright (C) 2019-2020 by Sukchan Lee <acetcom@gmail.com>
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

/*-
 * HMAC-SHA-224/256/384/512 implementation
 * Last update: 06/15/2005
 * Issue date:  06/15/2005
 *
 * Copyright (C) 2005 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "ogs-crypt.h"

/* HMAC-SHA-224 functions */

void ogs_hmac_sha224_init(ogs_hmac_sha224_ctx *ctx, const uint8_t *key,
                      uint32_t key_size)
{
    uint32_t fill;
    uint32_t num;

    uint8_t key_temp[OGS_SHA224_DIGEST_SIZE];
    int i;

    if (key_size == OGS_SHA224_BLOCK_SIZE) {
        memcpy(key_temp, key, sizeof(key_temp));
        num = OGS_SHA224_BLOCK_SIZE;
    } else {
        if (key_size > OGS_SHA224_BLOCK_SIZE){
            num = OGS_SHA224_DIGEST_SIZE;
            ogs_sha224(key, key_size, key_temp);
        } else { /* key_size > OGS_SHA224_BLOCK_SIZE */
            memcpy(key_temp, key, sizeof(key_temp));
            num = key_size;
        }
        fill = OGS_SHA224_BLOCK_SIZE - num;

        memset(ctx->block_ipad + num, 0x36, fill);
        memset(ctx->block_opad + num, 0x5c, fill);
    }

    for (i = 0; i < num; i++) {
        ctx->block_ipad[i] = key_temp[i] ^ 0x36;
        ctx->block_opad[i] = key_temp[i] ^ 0x5c;
    }

    ogs_sha224_init(&ctx->ctx_inside);
    ogs_sha224_update(&ctx->ctx_inside, ctx->block_ipad, OGS_SHA224_BLOCK_SIZE);

    ogs_sha224_init(&ctx->ctx_outside);
    ogs_sha224_update(&ctx->ctx_outside, ctx->block_opad,
                  OGS_SHA224_BLOCK_SIZE);

    /* for hmac_reinit */
    memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
           sizeof(ogs_sha224_ctx));
    memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
           sizeof(ogs_sha224_ctx));
}

void ogs_hmac_sha224_reinit(ogs_hmac_sha224_ctx *ctx)
{
    memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
           sizeof(ogs_sha224_ctx));
    memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
           sizeof(ogs_sha224_ctx));
}

void ogs_hmac_sha224_update(ogs_hmac_sha224_ctx *ctx, const uint8_t *message,
                        uint32_t message_len)
{
    ogs_sha224_update(&ctx->ctx_inside, message, message_len);
}

void ogs_hmac_sha224_final(ogs_hmac_sha224_ctx *ctx, uint8_t *mac,
                       uint32_t mac_size)
{
    uint8_t digest_inside[OGS_SHA224_DIGEST_SIZE];
    uint8_t mac_temp[OGS_SHA224_DIGEST_SIZE];

    ogs_sha224_final(&ctx->ctx_inside, digest_inside);
    ogs_sha224_update(&ctx->ctx_outside, digest_inside, OGS_SHA224_DIGEST_SIZE);
    ogs_sha224_final(&ctx->ctx_outside, mac_temp);
    memcpy(mac, mac_temp, mac_size);
}

void ogs_hmac_sha224(const uint8_t *key, uint32_t key_size,
          const uint8_t *message, uint32_t message_len,
          uint8_t *mac, uint32_t mac_size)
{
    ogs_hmac_sha224_ctx ctx;

    ogs_hmac_sha224_init(&ctx, key, key_size);
    ogs_hmac_sha224_update(&ctx, message, message_len);
    ogs_hmac_sha224_final(&ctx, mac, mac_size);
}

/* HMAC-SHA-256 functions */

void ogs_hmac_sha256_init(ogs_hmac_sha256_ctx *ctx, const uint8_t *key,
                      uint32_t key_size)
{
    uint32_t fill;
    uint32_t num;

    uint8_t key_temp[OGS_SHA256_DIGEST_SIZE];
    int i;

    if (key_size == OGS_SHA256_BLOCK_SIZE) {
        memcpy(key_temp, key, sizeof(key_temp));
        num = OGS_SHA256_BLOCK_SIZE;
    } else {
        if (key_size > OGS_SHA256_BLOCK_SIZE){
            num = OGS_SHA256_DIGEST_SIZE;
            ogs_sha256(key, key_size, key_temp);
        } else { /* key_size > OGS_SHA256_BLOCK_SIZE */
            memcpy(key_temp, key, sizeof(key_temp));
            num = key_size;
        }
        fill = OGS_SHA256_BLOCK_SIZE - num;

        memset(ctx->block_ipad + num, 0x36, fill);
        memset(ctx->block_opad + num, 0x5c, fill);
    }

    for (i = 0; i < num; i++) {
        ctx->block_ipad[i] = key_temp[i] ^ 0x36;
        ctx->block_opad[i] = key_temp[i] ^ 0x5c;
    }

    ogs_sha256_init(&ctx->ctx_inside);
    ogs_sha256_update(&ctx->ctx_inside, ctx->block_ipad, OGS_SHA256_BLOCK_SIZE);

    ogs_sha256_init(&ctx->ctx_outside);
    ogs_sha256_update(&ctx->ctx_outside, ctx->block_opad,
                  OGS_SHA256_BLOCK_SIZE);

    /* for hmac_reinit */
    memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
           sizeof(ogs_sha256_ctx));
    memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
           sizeof(ogs_sha256_ctx));
}

void ogs_hmac_sha256_reinit(ogs_hmac_sha256_ctx *ctx)
{
    memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
           sizeof(ogs_sha256_ctx));
    memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
           sizeof(ogs_sha256_ctx));
}

void ogs_hmac_sha256_update(ogs_hmac_sha256_ctx *ctx, const uint8_t *message,
                        uint32_t message_len)
{
    ogs_sha256_update(&ctx->ctx_inside, message, message_len);
}

void ogs_hmac_sha256_final(ogs_hmac_sha256_ctx *ctx, uint8_t *mac,
                       uint32_t mac_size)
{
    uint8_t digest_inside[OGS_SHA256_DIGEST_SIZE];
    uint8_t mac_temp[OGS_SHA256_DIGEST_SIZE];

    ogs_sha256_final(&ctx->ctx_inside, digest_inside);
    ogs_sha256_update(&ctx->ctx_outside, digest_inside, OGS_SHA256_DIGEST_SIZE);
    ogs_sha256_final(&ctx->ctx_outside, mac_temp);
    memcpy(mac, mac_temp, mac_size);
}

void ogs_hmac_sha256(const uint8_t *key, uint32_t key_size,
          const uint8_t *message, uint32_t message_len,
          uint8_t *mac, uint32_t mac_size)
{
    ogs_hmac_sha256_ctx ctx;

    ogs_hmac_sha256_init(&ctx, key, key_size);
    ogs_hmac_sha256_update(&ctx, message, message_len);
    ogs_hmac_sha256_final(&ctx, mac, mac_size);
}

/* HMAC-SHA-384 functions */

void ogs_hmac_sha384_init(ogs_hmac_sha384_ctx *ctx, const uint8_t *key,
                      uint32_t key_size)
{
    uint32_t fill;
    uint32_t num;

    uint8_t key_temp[OGS_SHA384_DIGEST_SIZE];
    int i;

    if (key_size == OGS_SHA384_BLOCK_SIZE) {
        memcpy(key_temp, key, sizeof(key_temp));
        num = OGS_SHA384_BLOCK_SIZE;
    } else {
        if (key_size > OGS_SHA384_BLOCK_SIZE){
            num = OGS_SHA384_DIGEST_SIZE;
            ogs_sha384(key, key_size, key_temp);
        } else { /* key_size > OGS_SHA384_BLOCK_SIZE */
            memcpy(key_temp, key, sizeof(key_temp));
            num = key_size;
        }
        fill = OGS_SHA384_BLOCK_SIZE - num;

        memset(ctx->block_ipad + num, 0x36, fill);
        memset(ctx->block_opad + num, 0x5c, fill);
    }

    for (i = 0; i < num; i++) {
        ctx->block_ipad[i] = key_temp[i] ^ 0x36;
        ctx->block_opad[i] = key_temp[i] ^ 0x5c;
    }

    ogs_sha384_init(&ctx->ctx_inside);
    ogs_sha384_update(&ctx->ctx_inside, ctx->block_ipad, OGS_SHA384_BLOCK_SIZE);

    ogs_sha384_init(&ctx->ctx_outside);
    ogs_sha384_update(&ctx->ctx_outside, ctx->block_opad,
                  OGS_SHA384_BLOCK_SIZE);

    /* for hmac_reinit */
    memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
           sizeof(ogs_sha384_ctx));
    memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
           sizeof(ogs_sha384_ctx));
}

void ogs_hmac_sha384_reinit(ogs_hmac_sha384_ctx *ctx)
{
    memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
           sizeof(ogs_sha384_ctx));
    memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
           sizeof(ogs_sha384_ctx));
}

void ogs_hmac_sha384_update(ogs_hmac_sha384_ctx *ctx, const uint8_t *message,
                        uint32_t message_len)
{
    ogs_sha384_update(&ctx->ctx_inside, message, message_len);
}

void ogs_hmac_sha384_final(ogs_hmac_sha384_ctx *ctx, uint8_t *mac,
                       uint32_t mac_size)
{
    uint8_t digest_inside[OGS_SHA384_DIGEST_SIZE];
    uint8_t mac_temp[OGS_SHA384_DIGEST_SIZE];

    ogs_sha384_final(&ctx->ctx_inside, digest_inside);
    ogs_sha384_update(&ctx->ctx_outside, digest_inside, OGS_SHA384_DIGEST_SIZE);
    ogs_sha384_final(&ctx->ctx_outside, mac_temp);
    memcpy(mac, mac_temp, mac_size);
}

void ogs_hmac_sha384(const uint8_t *key, uint32_t key_size,
          const uint8_t *message, uint32_t message_len,
          uint8_t *mac, uint32_t mac_size)
{
    ogs_hmac_sha384_ctx ctx;

    ogs_hmac_sha384_init(&ctx, key, key_size);
    ogs_hmac_sha384_update(&ctx, message, message_len);
    ogs_hmac_sha384_final(&ctx, mac, mac_size);
}

/* HMAC-SHA-512 functions */

void ogs_hmac_sha512_init(ogs_hmac_sha512_ctx *ctx, const uint8_t *key,
                      uint32_t key_size)
{
    uint32_t fill;
    uint32_t num;

    uint8_t key_temp[OGS_SHA512_DIGEST_SIZE];
    int i;

    if (key_size == OGS_SHA512_BLOCK_SIZE) {
        memcpy(key_temp, key, sizeof(key_temp));
        num = OGS_SHA512_BLOCK_SIZE;
    } else {
        if (key_size > OGS_SHA512_BLOCK_SIZE){
            num = OGS_SHA512_DIGEST_SIZE;
            ogs_sha512(key, key_size, key_temp);
        } else { /* key_size > OGS_SHA512_BLOCK_SIZE */
            memcpy(key_temp, key, sizeof(key_temp));
            num = key_size;
        }
        fill = OGS_SHA512_BLOCK_SIZE - num;

        memset(ctx->block_ipad + num, 0x36, fill);
        memset(ctx->block_opad + num, 0x5c, fill);
    }

    for (i = 0; i < num; i++) {
        ctx->block_ipad[i] = key_temp[i] ^ 0x36;
        ctx->block_opad[i] = key_temp[i] ^ 0x5c;
    }

    ogs_sha512_init(&ctx->ctx_inside);
    ogs_sha512_update(&ctx->ctx_inside, ctx->block_ipad, OGS_SHA512_BLOCK_SIZE);

    ogs_sha512_init(&ctx->ctx_outside);
    ogs_sha512_update(&ctx->ctx_outside, ctx->block_opad,
                  OGS_SHA512_BLOCK_SIZE);

    /* for hmac_reinit */
    memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
           sizeof(ogs_sha512_ctx));
    memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
           sizeof(ogs_sha512_ctx));
}

void ogs_hmac_sha512_reinit(ogs_hmac_sha512_ctx *ctx)
{
    memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
           sizeof(ogs_sha512_ctx));
    memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
           sizeof(ogs_sha512_ctx));
}

void ogs_hmac_sha512_update(ogs_hmac_sha512_ctx *ctx, const uint8_t *message,
                        uint32_t message_len)
{
    ogs_sha512_update(&ctx->ctx_inside, message, message_len);
}

void ogs_hmac_sha512_final(ogs_hmac_sha512_ctx *ctx, uint8_t *mac,
                       uint32_t mac_size)
{
    uint8_t digest_inside[OGS_SHA512_DIGEST_SIZE];
    uint8_t mac_temp[OGS_SHA512_DIGEST_SIZE];

    ogs_sha512_final(&ctx->ctx_inside, digest_inside);
    ogs_sha512_update(&ctx->ctx_outside, digest_inside, OGS_SHA512_DIGEST_SIZE);
    ogs_sha512_final(&ctx->ctx_outside, mac_temp);
    memcpy(mac, mac_temp, mac_size);
}

void ogs_hmac_sha512(const uint8_t *key, uint32_t key_size,
          const uint8_t *message, uint32_t message_len,
          uint8_t *mac, uint32_t mac_size)
{
    ogs_hmac_sha512_ctx ctx;

    ogs_hmac_sha512_init(&ctx, key, key_size);
    ogs_hmac_sha512_update(&ctx, message, message_len);
    ogs_hmac_sha512_final(&ctx, mac, mac_size);
}
