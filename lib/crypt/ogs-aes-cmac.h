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
