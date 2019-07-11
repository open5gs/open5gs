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

#include "ogs-crypt.h"
#include "base/types.h"

#include "hss-auc.h"
#include "milenage.h"

#define FC_VALUE 0x10

void hss_auc_kasme(const uint8_t *ck, const uint8_t *ik, 
        const uint8_t plmn_id[3], const uint8_t *sqn,  const uint8_t *ak,
        uint8_t *kasme)
{
    uint8_t s[14];
    uint8_t k[32];
    int i;

    memcpy(&k[0], ck, 16);
    memcpy(&k[16], ik, 16);

    s[0] = FC_VALUE;
    memcpy(&s[1], plmn_id, 3);
    s[4] = 0x00;
    s[5] = 0x03;

    for (i = 0; i < 6; i++)
        s[6+i] = sqn[i] ^ ak[i];
    s[12] = 0x00;
    s[13] = 0x06;

    ogs_hmac_sha256(k, 32, s, 14, kasme, 32);
}

void hss_auc_sqn(
    const uint8_t *opc, const uint8_t *k, const uint8_t *auts,
    uint8_t *sqn_ms, uint8_t *mac_s)
{
    int i;
    uint8_t ak[HSS_AK_LEN];
    uint8_t amf[2] = { 0, 0 };
    const uint8_t *rand = auts;
    const uint8_t *conc_sqn_ms = auts + RAND_LEN;

    milenage_f2345(opc, k, rand, NULL, NULL, NULL, NULL, ak);
    for (i = 0; i < HSS_SQN_LEN; i++)
        sqn_ms[i] = ak[i] ^ conc_sqn_ms[i];
    milenage_f1(opc, k, auts, sqn_ms, amf, NULL, mac_s);
}
