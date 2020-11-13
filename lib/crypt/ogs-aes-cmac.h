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

#ifndef OGS_AES_CMAC_H
#define OGS_AES_CMAC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Caculate CMAC value
 *
 * @param cmac
 * @param key
 * @param msg
 * @param len
 *
 * @return OGS_OK
 *         OGS_ERROR
 */
int ogs_aes_cmac_calculate(uint8_t *cmac, const uint8_t *key,
        const uint8_t *msg, const uint32_t len);

/**
 * Verify CMAC value
 *
 * @param cmac
 * @param key
 * @param msg
 * @param len
 *
 * @return OGS_OK
 *         OGS_ERROR
 *         OGS_ERR_INVALID_CMAC
 */
#define OGS_ERR_INVALID_CMAC -2
int ogs_aes_cmac_verify(uint8_t *cmac, const uint8_t *key,
        const uint8_t *msg, const uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* OGS_AES_CMAC_H */
