/*
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Last update: 02/02/2007
 * Issue date:  04/30/2005
 *
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
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

#ifndef _CORE_SHA2_H_
#define _CORE_SHA2_H_

#include "core.h"

#define SHA224_DIGEST_SIZE ( 224 / 8)
#define SHA256_DIGEST_SIZE ( 256 / 8)
#define SHA384_DIGEST_SIZE ( 384 / 8)
#define SHA512_DIGEST_SIZE ( 512 / 8)

#define SHA256_BLOCK_SIZE  ( 512 / 8)
#define SHA512_BLOCK_SIZE  (1024 / 8)
#define SHA384_BLOCK_SIZE  SHA512_BLOCK_SIZE
#define SHA224_BLOCK_SIZE  SHA256_BLOCK_SIZE

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
    c_uint32_t tot_len;
    c_uint32_t len;
    c_uint8_t block[2 * SHA256_BLOCK_SIZE];
    c_uint32_t h[8];
} sha256_ctx;

typedef struct {
    c_uint32_t tot_len;
    c_uint32_t len;
    c_uint8_t block[2 * SHA512_BLOCK_SIZE];
    c_uint64_t h[8];
} sha512_ctx;

typedef sha512_ctx sha384_ctx;
typedef sha256_ctx sha224_ctx;

CORE_DECLARE(void) sha224_init(sha224_ctx *ctx);
CORE_DECLARE(void) sha224_update(sha224_ctx *ctx, const c_uint8_t *message,
        c_uint32_t len);
CORE_DECLARE(void) sha224_final(sha224_ctx *ctx, c_uint8_t *digest);
CORE_DECLARE(void) sha224(const c_uint8_t *message, c_uint32_t len,
        c_uint8_t *digest);

CORE_DECLARE(void) sha256_init(sha256_ctx * ctx);
CORE_DECLARE(void) sha256_update(sha256_ctx *ctx, const c_uint8_t *message,
                   c_uint32_t len);
CORE_DECLARE(void) sha256_final(sha256_ctx *ctx, c_uint8_t *digest);
CORE_DECLARE(void) sha256(const c_uint8_t *message, c_uint32_t len,
        c_uint8_t *digest);

CORE_DECLARE(void) sha384_init(sha384_ctx *ctx);
CORE_DECLARE(void) sha384_update(sha384_ctx *ctx, const c_uint8_t *message,
                   c_uint32_t len);
CORE_DECLARE(void) sha384_final(sha384_ctx *ctx, c_uint8_t *digest);
CORE_DECLARE(void) sha384(const c_uint8_t *message, c_uint32_t len,
        c_uint8_t *digest);

CORE_DECLARE(void) sha512_init(sha512_ctx *ctx);
CORE_DECLARE(void) sha512_update(sha512_ctx *ctx, const c_uint8_t *message,
                   c_uint32_t len);
CORE_DECLARE(void) sha512_final(sha512_ctx *ctx, c_uint8_t *digest);
CORE_DECLARE(void) sha512(const c_uint8_t *message, c_uint32_t len,
        c_uint8_t *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_CORE_SHA2_H_ */
