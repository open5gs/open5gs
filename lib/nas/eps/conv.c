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

#include "ogs-nas-eps.h"

void ogs_nas_eps_imsi_to_bcd(
    const ogs_nas_mobile_identity_imsi_t *imsi, uint8_t imsi_len, char *bcd)
{
    int bcd_len;

    ogs_assert(imsi && imsi_len);

    bcd[0] = '0' + imsi->digit1;
    bcd[1] = '0' + imsi->digit2;
    bcd[2] = '0' + imsi->digit3;
    bcd[3] = '0' + imsi->digit4;
    bcd[4] = '0' + imsi->digit5;
    bcd[5] = '0' + imsi->digit6;
    bcd[6] = '0' + imsi->digit7;
    bcd[7] = '0' + imsi->digit8;
    bcd[8] = '0' + imsi->digit9;
    bcd[9] = '0' + imsi->digit10;
    bcd[10] = '0' + imsi->digit11;
    bcd[11] = '0' + imsi->digit12;
    bcd[12] = '0' + imsi->digit13;
    bcd[13] = '0' + imsi->digit14;
    bcd[14] = '0' + imsi->digit15;

    bcd_len = imsi_len * 2 - 1;
    if (!imsi->odd_even) { /* if bcd length is even */
        if (imsi->digit15 != 0xf)
            ogs_warn("Spec warning : bcd[%d] = 0x%x, 0x%x",
                    bcd_len-1, imsi->digit15, bcd[bcd_len-1]);
        (bcd_len)--;
    }

    bcd[bcd_len] = 0;
}

void ogs_nas_imsi_to_buffer(
    const ogs_nas_mobile_identity_imsi_t *imsi, uint8_t imsi_len,
    uint8_t *buf, uint8_t *buf_len)
{
    buf[0] = ((('0' + imsi->digit2) << 4) & 0xf0) |
                (('0' + imsi->digit1) & 0x0f);
    buf[1] = ((('0' + imsi->digit4) << 4) & 0xf0) |
                (('0' + imsi->digit3) & 0x0f);
    buf[2] = ((('0' + imsi->digit6) << 4) & 0xf0) |
                (('0' + imsi->digit5) & 0x0f);
    buf[3] = ((('0' + imsi->digit8) << 4) & 0xf0) |
                (('0' + imsi->digit7) & 0x0f);
    buf[4] = ((('0' + imsi->digit10) << 4) & 0xf0) |
                (('0' + imsi->digit9) & 0x0f);
    buf[5] = ((('0' + imsi->digit12) << 4) & 0xf0) |
                (('0' + imsi->digit11) & 0x0f);
    buf[6] = ((('0' + imsi->digit14) << 4) & 0xf0) |
                (('0' + imsi->digit13) & 0x0f);
    buf[7] = ((('0' + imsi->digit15)) & 0x0f);

    *buf_len = imsi_len;
    if (!imsi->odd_even) { /* if imsi length is even */
        (*buf_len)--;
        if ((buf[*buf_len] & 0xf) != 0xf)
            ogs_warn("Spec warning : buf[%d] = 0x%x", *buf_len, buf[*buf_len]);
    }
}
