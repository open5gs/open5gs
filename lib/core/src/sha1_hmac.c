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

#include "core_sha1_hmac.h"

void hmac_sha1_init(hmac_sha1_ctx *ctx, c_uint8_t *key,
                      c_uint32_t key_size)
{
    c_uint32_t fill;
    c_uint32_t num;

    c_uint8_t *key_used;
    c_uint8_t key_temp[SHA1_DIGEST_SIZE];
    int i;

    if (key_size == SHA1_BLOCK_SIZE) {
        key_used = key;
        num = SHA1_BLOCK_SIZE;
    } else {
        if (key_size > SHA1_BLOCK_SIZE){
            key_used = key_temp;
            num = SHA1_DIGEST_SIZE;
            sha1(key, key_size, key_used);
        } else { /* key_size > SHA1_BLOCK_SIZE */
            key_used = key;
            num = key_size;
        }
        fill = SHA1_BLOCK_SIZE - num;

        memset(ctx->block_ipad + num, 0x36, fill);
        memset(ctx->block_opad + num, 0x5c, fill);
    }

    for (i = 0; i < num; i++) {
        ctx->block_ipad[i] = key_used[i] ^ 0x36;
        ctx->block_opad[i] = key_used[i] ^ 0x5c;
    }

    sha1_init(&ctx->ctx_inside);
    sha1_update(&ctx->ctx_inside, ctx->block_ipad, SHA1_BLOCK_SIZE);

    sha1_init(&ctx->ctx_outside);
    sha1_update(&ctx->ctx_outside, ctx->block_opad,
                  SHA1_BLOCK_SIZE);

    /* for hmac_reinit */
    memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
           sizeof(sha1_ctx));
    memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
           sizeof(sha1_ctx));
}

void hmac_sha1_reinit(hmac_sha1_ctx *ctx)
{
    memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
           sizeof(sha1_ctx));
    memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
           sizeof(sha1_ctx));
}

void hmac_sha1_update(hmac_sha1_ctx *ctx, c_uint8_t *message,
                        c_uint32_t message_len)
{
    sha1_update(&ctx->ctx_inside, message, message_len);
}

void hmac_sha1_final(hmac_sha1_ctx *ctx, c_uint8_t *mac,
                       c_uint32_t mac_size)
{
    c_uint8_t digest_inside[SHA1_DIGEST_SIZE];
    c_uint8_t mac_temp[SHA1_DIGEST_SIZE];

    sha1_final(&ctx->ctx_inside, digest_inside);
    sha1_update(&ctx->ctx_outside, digest_inside, SHA1_DIGEST_SIZE);
    sha1_final(&ctx->ctx_outside, mac_temp);
    memcpy(mac, mac_temp, mac_size);
}

void hmac_sha1(c_uint8_t *key, c_uint32_t key_size,
          c_uint8_t *message, c_uint32_t message_len,
          c_uint8_t *mac, c_uint32_t mac_size)
{
    hmac_sha1_ctx ctx;

    hmac_sha1_init(&ctx, key, key_size);
    hmac_sha1_update(&ctx, message, message_len);
    hmac_sha1_final(&ctx, mac, mac_size);
}
