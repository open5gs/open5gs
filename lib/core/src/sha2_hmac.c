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

#include <string.h>

#include "core_sha2_hmac.h"

/* HMAC-SHA-224 functions */

void hmac_sha224_init(hmac_sha224_ctx *ctx, c_uint8_t *key,
                      c_uint32_t key_size)
{
    c_uint32_t fill;
    c_uint32_t num;

    c_uint8_t *key_used;
    c_uint8_t key_temp[SHA224_DIGEST_SIZE];
    int i;

    if (key_size == SHA224_BLOCK_SIZE) {
        key_used = key;
        num = SHA224_BLOCK_SIZE;
    } else {
        if (key_size > SHA224_BLOCK_SIZE){
            key_used = key_temp;
            num = SHA224_DIGEST_SIZE;
            sha224(key, key_size, key_used);
        } else { /* key_size > SHA224_BLOCK_SIZE */
            key_used = key;
            num = key_size;
        }
        fill = SHA224_BLOCK_SIZE - num;

        memset(ctx->block_ipad + num, 0x36, fill);
        memset(ctx->block_opad + num, 0x5c, fill);
    }

    for (i = 0; i < num; i++) {
        ctx->block_ipad[i] = key_used[i] ^ 0x36;
        ctx->block_opad[i] = key_used[i] ^ 0x5c;
    }

    sha224_init(&ctx->ctx_inside);
    sha224_update(&ctx->ctx_inside, ctx->block_ipad, SHA224_BLOCK_SIZE);

    sha224_init(&ctx->ctx_outside);
    sha224_update(&ctx->ctx_outside, ctx->block_opad,
                  SHA224_BLOCK_SIZE);

    /* for hmac_reinit */
    memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
           sizeof(sha224_ctx));
    memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
           sizeof(sha224_ctx));
}

void hmac_sha224_reinit(hmac_sha224_ctx *ctx)
{
    memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
           sizeof(sha224_ctx));
    memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
           sizeof(sha224_ctx));
}

void hmac_sha224_update(hmac_sha224_ctx *ctx, c_uint8_t *message,
                        c_uint32_t message_len)
{
    sha224_update(&ctx->ctx_inside, message, message_len);
}

void hmac_sha224_final(hmac_sha224_ctx *ctx, c_uint8_t *mac,
                       c_uint32_t mac_size)
{
    c_uint8_t digest_inside[SHA224_DIGEST_SIZE];
    c_uint8_t mac_temp[SHA224_DIGEST_SIZE];

    sha224_final(&ctx->ctx_inside, digest_inside);
    sha224_update(&ctx->ctx_outside, digest_inside, SHA224_DIGEST_SIZE);
    sha224_final(&ctx->ctx_outside, mac_temp);
    memcpy(mac, mac_temp, mac_size);
}

void hmac_sha224(c_uint8_t *key, c_uint32_t key_size,
          c_uint8_t *message, c_uint32_t message_len,
          c_uint8_t *mac, c_uint32_t mac_size)
{
    hmac_sha224_ctx ctx;

    hmac_sha224_init(&ctx, key, key_size);
    hmac_sha224_update(&ctx, message, message_len);
    hmac_sha224_final(&ctx, mac, mac_size);
}

/* HMAC-SHA-256 functions */

void hmac_sha256_init(hmac_sha256_ctx *ctx, c_uint8_t *key,
                      c_uint32_t key_size)
{
    c_uint32_t fill;
    c_uint32_t num;

    c_uint8_t *key_used;
    c_uint8_t key_temp[SHA256_DIGEST_SIZE];
    int i;

    if (key_size == SHA256_BLOCK_SIZE) {
        key_used = key;
        num = SHA256_BLOCK_SIZE;
    } else {
        if (key_size > SHA256_BLOCK_SIZE){
            key_used = key_temp;
            num = SHA256_DIGEST_SIZE;
            sha256(key, key_size, key_used);
        } else { /* key_size > SHA256_BLOCK_SIZE */
            key_used = key;
            num = key_size;
        }
        fill = SHA256_BLOCK_SIZE - num;

        memset(ctx->block_ipad + num, 0x36, fill);
        memset(ctx->block_opad + num, 0x5c, fill);
    }

    for (i = 0; i < num; i++) {
        ctx->block_ipad[i] = key_used[i] ^ 0x36;
        ctx->block_opad[i] = key_used[i] ^ 0x5c;
    }

    sha256_init(&ctx->ctx_inside);
    sha256_update(&ctx->ctx_inside, ctx->block_ipad, SHA256_BLOCK_SIZE);

    sha256_init(&ctx->ctx_outside);
    sha256_update(&ctx->ctx_outside, ctx->block_opad,
                  SHA256_BLOCK_SIZE);

    /* for hmac_reinit */
    memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
           sizeof(sha256_ctx));
    memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
           sizeof(sha256_ctx));
}

void hmac_sha256_reinit(hmac_sha256_ctx *ctx)
{
    memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
           sizeof(sha256_ctx));
    memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
           sizeof(sha256_ctx));
}

void hmac_sha256_update(hmac_sha256_ctx *ctx, c_uint8_t *message,
                        c_uint32_t message_len)
{
    sha256_update(&ctx->ctx_inside, message, message_len);
}

void hmac_sha256_final(hmac_sha256_ctx *ctx, c_uint8_t *mac,
                       c_uint32_t mac_size)
{
    c_uint8_t digest_inside[SHA256_DIGEST_SIZE];
    c_uint8_t mac_temp[SHA256_DIGEST_SIZE];

    sha256_final(&ctx->ctx_inside, digest_inside);
    sha256_update(&ctx->ctx_outside, digest_inside, SHA256_DIGEST_SIZE);
    sha256_final(&ctx->ctx_outside, mac_temp);
    memcpy(mac, mac_temp, mac_size);
}

void hmac_sha256(c_uint8_t *key, c_uint32_t key_size,
          c_uint8_t *message, c_uint32_t message_len,
          c_uint8_t *mac, c_uint32_t mac_size)
{
    hmac_sha256_ctx ctx;

    hmac_sha256_init(&ctx, key, key_size);
    hmac_sha256_update(&ctx, message, message_len);
    hmac_sha256_final(&ctx, mac, mac_size);
}

/* HMAC-SHA-384 functions */

void hmac_sha384_init(hmac_sha384_ctx *ctx, c_uint8_t *key,
                      c_uint32_t key_size)
{
    c_uint32_t fill;
    c_uint32_t num;

    c_uint8_t *key_used;
    c_uint8_t key_temp[SHA384_DIGEST_SIZE];
    int i;

    if (key_size == SHA384_BLOCK_SIZE) {
        key_used = key;
        num = SHA384_BLOCK_SIZE;
    } else {
        if (key_size > SHA384_BLOCK_SIZE){
            key_used = key_temp;
            num = SHA384_DIGEST_SIZE;
            sha384(key, key_size, key_used);
        } else { /* key_size > SHA384_BLOCK_SIZE */
            key_used = key;
            num = key_size;
        }
        fill = SHA384_BLOCK_SIZE - num;

        memset(ctx->block_ipad + num, 0x36, fill);
        memset(ctx->block_opad + num, 0x5c, fill);
    }

    for (i = 0; i < num; i++) {
        ctx->block_ipad[i] = key_used[i] ^ 0x36;
        ctx->block_opad[i] = key_used[i] ^ 0x5c;
    }

    sha384_init(&ctx->ctx_inside);
    sha384_update(&ctx->ctx_inside, ctx->block_ipad, SHA384_BLOCK_SIZE);

    sha384_init(&ctx->ctx_outside);
    sha384_update(&ctx->ctx_outside, ctx->block_opad,
                  SHA384_BLOCK_SIZE);

    /* for hmac_reinit */
    memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
           sizeof(sha384_ctx));
    memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
           sizeof(sha384_ctx));
}

void hmac_sha384_reinit(hmac_sha384_ctx *ctx)
{
    memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
           sizeof(sha384_ctx));
    memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
           sizeof(sha384_ctx));
}

void hmac_sha384_update(hmac_sha384_ctx *ctx, c_uint8_t *message,
                        c_uint32_t message_len)
{
    sha384_update(&ctx->ctx_inside, message, message_len);
}

void hmac_sha384_final(hmac_sha384_ctx *ctx, c_uint8_t *mac,
                       c_uint32_t mac_size)
{
    c_uint8_t digest_inside[SHA384_DIGEST_SIZE];
    c_uint8_t mac_temp[SHA384_DIGEST_SIZE];

    sha384_final(&ctx->ctx_inside, digest_inside);
    sha384_update(&ctx->ctx_outside, digest_inside, SHA384_DIGEST_SIZE);
    sha384_final(&ctx->ctx_outside, mac_temp);
    memcpy(mac, mac_temp, mac_size);
}

void hmac_sha384(c_uint8_t *key, c_uint32_t key_size,
          c_uint8_t *message, c_uint32_t message_len,
          c_uint8_t *mac, c_uint32_t mac_size)
{
    hmac_sha384_ctx ctx;

    hmac_sha384_init(&ctx, key, key_size);
    hmac_sha384_update(&ctx, message, message_len);
    hmac_sha384_final(&ctx, mac, mac_size);
}

/* HMAC-SHA-512 functions */

void hmac_sha512_init(hmac_sha512_ctx *ctx, c_uint8_t *key,
                      c_uint32_t key_size)
{
    c_uint32_t fill;
    c_uint32_t num;

    c_uint8_t *key_used;
    c_uint8_t key_temp[SHA512_DIGEST_SIZE];
    int i;

    if (key_size == SHA512_BLOCK_SIZE) {
        key_used = key;
        num = SHA512_BLOCK_SIZE;
    } else {
        if (key_size > SHA512_BLOCK_SIZE){
            key_used = key_temp;
            num = SHA512_DIGEST_SIZE;
            sha512(key, key_size, key_used);
        } else { /* key_size > SHA512_BLOCK_SIZE */
            key_used = key;
            num = key_size;
        }
        fill = SHA512_BLOCK_SIZE - num;

        memset(ctx->block_ipad + num, 0x36, fill);
        memset(ctx->block_opad + num, 0x5c, fill);
    }

    for (i = 0; i < num; i++) {
        ctx->block_ipad[i] = key_used[i] ^ 0x36;
        ctx->block_opad[i] = key_used[i] ^ 0x5c;
    }

    sha512_init(&ctx->ctx_inside);
    sha512_update(&ctx->ctx_inside, ctx->block_ipad, SHA512_BLOCK_SIZE);

    sha512_init(&ctx->ctx_outside);
    sha512_update(&ctx->ctx_outside, ctx->block_opad,
                  SHA512_BLOCK_SIZE);

    /* for hmac_reinit */
    memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
           sizeof(sha512_ctx));
    memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
           sizeof(sha512_ctx));
}

void hmac_sha512_reinit(hmac_sha512_ctx *ctx)
{
    memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
           sizeof(sha512_ctx));
    memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
           sizeof(sha512_ctx));
}

void hmac_sha512_update(hmac_sha512_ctx *ctx, c_uint8_t *message,
                        c_uint32_t message_len)
{
    sha512_update(&ctx->ctx_inside, message, message_len);
}

void hmac_sha512_final(hmac_sha512_ctx *ctx, c_uint8_t *mac,
                       c_uint32_t mac_size)
{
    c_uint8_t digest_inside[SHA512_DIGEST_SIZE];
    c_uint8_t mac_temp[SHA512_DIGEST_SIZE];

    sha512_final(&ctx->ctx_inside, digest_inside);
    sha512_update(&ctx->ctx_outside, digest_inside, SHA512_DIGEST_SIZE);
    sha512_final(&ctx->ctx_outside, mac_temp);
    memcpy(mac, mac_temp, mac_size);
}

void hmac_sha512(c_uint8_t *key, c_uint32_t key_size,
          c_uint8_t *message, c_uint32_t message_len,
          c_uint8_t *mac, c_uint32_t mac_size)
{
    hmac_sha512_ctx ctx;

    hmac_sha512_init(&ctx, key, key_size);
    hmac_sha512_update(&ctx, message, message_len);
    hmac_sha512_final(&ctx, mac, mac_size);
}
