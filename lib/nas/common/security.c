/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-nas-common.h"

void ogs_nas_mac_calculate(uint8_t algorithm_identity,
        uint8_t *knas_int, uint32_t count, uint8_t bearer, 
        uint8_t direction, ogs_pkbuf_t *pkbuf, uint8_t *mac)
{
    uint8_t *ivec = NULL;;
    uint8_t cmac[16];
    uint32_t mac32;

    ogs_assert(knas_int);
    ogs_assert(bearer <= 0x1f);
    ogs_assert(direction == 0 || direction == 1);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);
    ogs_assert(mac);

    switch (algorithm_identity) {
    case OGS_NAS_SECURITY_ALGORITHMS_128_EIA1:
        snow_3g_f9(knas_int, count, (bearer << 27), direction, 
                pkbuf->data, (pkbuf->len << 3), mac);
        break;
    case OGS_NAS_SECURITY_ALGORITHMS_128_EIA2:
        count = htonl(count);

        ogs_pkbuf_push(pkbuf, 8);

        ivec = pkbuf->data;
        memset(ivec, 0, 8);
        memcpy(ivec + 0, &count, sizeof(count));
        ivec[4] = (bearer << 3) | (direction << 2);

        ogs_aes_cmac_calculate(cmac, knas_int, pkbuf->data, pkbuf->len);
        memcpy(mac, cmac, 4);

        ogs_pkbuf_pull(pkbuf, 8);

        break;
    case OGS_NAS_SECURITY_ALGORITHMS_128_EIA3:
        zuc_eia3(knas_int, count, bearer, direction, 
                (pkbuf->len << 3), pkbuf->data, &mac32);
        mac32 = ntohl(mac32);
        memcpy(mac, &mac32, sizeof(uint32_t));
        break;
    case OGS_NAS_SECURITY_ALGORITHMS_EIA0:
        ogs_error("Invalid identity : NAS_SECURITY_ALGORITHMS_EIA0");
        break;
    default:
        ogs_assert_if_reached();
        break;
    }
}

void ogs_nas_encrypt(uint8_t algorithm_identity,
        uint8_t *knas_enc, uint32_t count, uint8_t bearer, 
        uint8_t direction, ogs_pkbuf_t *pkbuf)
{
    uint8_t ivec[16];
    SNOW_CTX ctx;

    ogs_assert(knas_enc);
    ogs_assert(bearer <= 0x1f);
    ogs_assert(direction == 0 || direction == 1);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);

    switch (algorithm_identity) {
    case OGS_NAS_SECURITY_ALGORITHMS_128_EEA1:
#if 0 /* Issue #2581 : snow_3g_f8 have memory problem */
        snow_3g_f8(knas_enc, count, bearer, direction, 
                pkbuf->data, (pkbuf->len << 3));
#else
        SNOW_init(count, bearer, direction, (const char *)knas_enc, &ctx);
        SNOW(pkbuf->len, pkbuf->data, pkbuf->data, &ctx);
#endif
        break;
    case OGS_NAS_SECURITY_ALGORITHMS_128_EEA2:
        count = htonl(count);

        memset(ivec, 0, 16);
        memcpy(ivec + 0, &count, sizeof(count));
        ivec[4] = (bearer << 3) | (direction << 2);
        ogs_aes_ctr128_encrypt(knas_enc, ivec, 
                pkbuf->data, pkbuf->len, pkbuf->data);
        break;
    case OGS_NAS_SECURITY_ALGORITHMS_128_EEA3:
        zuc_eea3(knas_enc, count, bearer, direction, 
                (pkbuf->len << 3), pkbuf->data, pkbuf->data);
        break;
    case OGS_NAS_SECURITY_ALGORITHMS_EEA0:
        ogs_error("Invalid identity : NAS_SECURITY_ALGORITHMS_EEA0");
        break;
    default:
        ogs_assert_if_reached();
        break;
    }
}
