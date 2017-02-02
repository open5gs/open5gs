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

#ifndef _CORE_HMAC_SHA1_H
#define _CORE_HMAC_SHA1_H

#include "core_sha1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    sha1_ctx ctx_inside;
    sha1_ctx ctx_outside;

    /* for hmac_reinit */
    sha1_ctx ctx_inside_reinit;
    sha1_ctx ctx_outside_reinit;

    c_uint8_t block_ipad[SHA1_BLOCK_SIZE];
    c_uint8_t block_opad[SHA1_BLOCK_SIZE];
} hmac_sha1_ctx;

CORE_DECLARE(void) hmac_sha1_init(hmac_sha1_ctx *ctx, c_uint8_t *key,
                      c_uint32_t key_size);
CORE_DECLARE(void) hmac_sha1_reinit(hmac_sha1_ctx *ctx);
CORE_DECLARE(void) hmac_sha1_update(hmac_sha1_ctx *ctx, c_uint8_t *message,
                        c_uint32_t message_len);
CORE_DECLARE(void) hmac_sha1_final(hmac_sha1_ctx *ctx, c_uint8_t *mac,
                       c_uint32_t mac_size);
CORE_DECLARE(void) hmac_sha1(c_uint8_t *key, c_uint32_t key_size,
                 c_uint8_t *message, c_uint32_t message_len,
                 c_uint8_t *mac, c_uint32_t mac_size);

#ifdef __cplusplus
}
#endif

#endif /* ! _CORE_HMAC_SHA1_H */

