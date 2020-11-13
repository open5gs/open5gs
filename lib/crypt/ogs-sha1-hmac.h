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

#if !defined(OGS_CRYPT_INSIDE) && !defined(OGS_CRYPT_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SHA1_HMAC_H
#define OGS_SHA1_HMAC_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    ogs_sha1_ctx ctx_inside;
    ogs_sha1_ctx ctx_outside;

    /* for hmac_reinit */
    ogs_sha1_ctx ctx_inside_reinit;
    ogs_sha1_ctx ctx_outside_reinit;

    uint8_t block_ipad[OGS_SHA1_BLOCK_SIZE];
    uint8_t block_opad[OGS_SHA1_BLOCK_SIZE];
} ogs_hmac_sha1_ctx;

void ogs_hmac_sha1_init(ogs_hmac_sha1_ctx *ctx, const uint8_t *key,
                      uint32_t key_size);
void ogs_hmac_sha1_reinit(ogs_hmac_sha1_ctx *ctx);
void ogs_hmac_sha1_update(ogs_hmac_sha1_ctx *ctx, const uint8_t *message,
                        uint32_t message_len);
void ogs_hmac_sha1_final(ogs_hmac_sha1_ctx *ctx, uint8_t *mac,
                       uint32_t mac_size);
void ogs_hmac_sha1(const uint8_t *key, uint32_t key_size,
                 const uint8_t *message, uint32_t message_len,
                 uint8_t *mac, uint32_t mac_size);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SHA1_HMAC_H */
