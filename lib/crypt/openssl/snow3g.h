/* $OpenBSD: snow3g.h,v 0.10 2015 raphael.catolino@gmail.com$ */
/* ====================================================================
 * Copyright (c) 1998-2002 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http:
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http:
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 */

#ifndef HEADER_SNOW3G_H
#define HEADER_SNOW3G_H

#if 0 /* modified by acetcom */
#include <openssl/opensslconf.h>
#else
#include "core/ogs-core.h"
#endif

#ifdef OPENSSL_NO_SNOW3G
#error SNOW3G is disabled.
#endif

#include <stddef.h>
#include <stdint.h>

#define SNOW_ENCRYPT	1
#define SNOW_DECRYPT	0

#define SNOW_WORD_SIZE 4
#define SNOW_KEY_SIZE 16

#ifdef  __cplusplus
extern "C" {
#endif

struct snow_key_st {
  uint32_t key[4];
  uint32_t iv[4];
};

struct fsm_st {
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
};

typedef struct snow_ctx_st {
  uint32_t lfsr[SNOW_KEY_SIZE];
  struct fsm_st fsm;
} SNOW_CTX;

struct snow_key_st
#if 0
snow_array_to_key(const unsigned char key[16], const unsigned char iv[16]);
#else
snow_array_to_key(const unsigned char *key, const unsigned char *iv);
#endif

void
SNOW_set_key(struct snow_key_st key_iv, SNOW_CTX *ctx);

void
SNOW_gen_keystream(uint32_t *stream, size_t nb_word, SNOW_CTX *ctx);

void
SNOW_init(uint32_t countc, uint8_t bearer, uint8_t direction,
    const char *confidentiality_key, SNOW_CTX *ctx);

void
SNOW(size_t nb_byte, const unsigned char *in, unsigned char *out, SNOW_CTX *ctx);

#ifdef  __cplusplus
}
#endif

#endif /* !HEADER_SNOW_H */
