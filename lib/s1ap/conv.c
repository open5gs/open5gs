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

#include "ogs-s1ap.h"

void ogs_s1ap_buffer_to_OCTET_STRING(
        void *buf, int size, S1AP_TBCD_STRING_t *tbcd_string)
{
    tbcd_string->size = size;
    tbcd_string->buf = CALLOC(tbcd_string->size, sizeof(uint8_t));

    memcpy(tbcd_string->buf, buf, size);
}

void ogs_s1ap_uint32_to_ENB_ID(
    S1AP_ENB_ID_PR present, uint32_t enb_id, S1AP_ENB_ID_t *eNB_ID)
{
    ogs_assert(eNB_ID);

    eNB_ID->present = present;
    if (present == S1AP_ENB_ID_PR_macroENB_ID) {
        BIT_STRING_t *bit_string = &eNB_ID->choice.macroENB_ID;
        ogs_assert(bit_string);

        bit_string->size = 3;
        bit_string->buf = CALLOC(bit_string->size, sizeof(uint8_t));

        bit_string->buf[0] = enb_id >> 12;
        bit_string->buf[1] = enb_id >> 4;
        bit_string->buf[2] = (enb_id & 0xf) << 4;

        bit_string->bits_unused = 4;
    } else if (present == S1AP_ENB_ID_PR_homeENB_ID) {
        BIT_STRING_t *bit_string = &eNB_ID->choice.homeENB_ID;
        ogs_assert(bit_string);

        bit_string->size = 4;
        bit_string->buf = CALLOC(bit_string->size, sizeof(uint8_t));

        bit_string->buf[0] = enb_id >> 20;
        bit_string->buf[1] = enb_id >> 12;
        bit_string->buf[2] = enb_id >> 4;
        bit_string->buf[3] = (enb_id & 0xf) << 4;

        bit_string->bits_unused = 4;
    } else if (present == S1AP_ENB_ID_PR_short_macroENB_ID ||
               present == S1AP_ENB_ID_PR_long_macroENB_ID) {
        BIT_STRING_t *bits = present == S1AP_ENB_ID_PR_short_macroENB_ID ?
            &eNB_ID->choice.short_macroENB_ID : &eNB_ID->choice.long_macroENB_ID;
        unsigned int unused = present == S1AP_ENB_ID_PR_short_macroENB_ID ? 6 : 3;
        uint32_t aligned = enb_id << unused;
        bits->size = 3;
        bits->buf = CALLOC(bits->size, sizeof(uint8_t));
        ogs_assert(bits->buf);
        bits->bits_unused = unused;
        bits->buf[0] = aligned >> 16;
        bits->buf[1] = aligned >> 8;
        bits->buf[2] = aligned;
    } else {
        ogs_assert_if_reached();
    }

}

int ogs_s1ap_ENB_ID_to_uint32(S1AP_ENB_ID_t *eNB_ID, uint32_t *uint32)
{
    ogs_assert(uint32);
    ogs_assert(eNB_ID);

    if (eNB_ID->present == S1AP_ENB_ID_PR_homeENB_ID) {
        uint8_t *buf = eNB_ID->choice.homeENB_ID.buf;
        if (!buf || eNB_ID->choice.homeENB_ID.size != 4 ||
                eNB_ID->choice.homeENB_ID.bits_unused != 4)
            return OGS_ERROR;
        *uint32 = (buf[0] << 20) + (buf[1] << 12) + (buf[2] << 4) +
            ((buf[3] & 0xf0) >> 4);

    } else if (eNB_ID->present == S1AP_ENB_ID_PR_macroENB_ID) {
        uint8_t *buf = eNB_ID->choice.macroENB_ID.buf;
        if (!buf || eNB_ID->choice.macroENB_ID.size != 3 ||
                eNB_ID->choice.macroENB_ID.bits_unused != 4)
            return OGS_ERROR;
        *uint32 = (buf[0] << 12) + (buf[1] << 4) + ((buf[2] & 0xf0) >> 4);
    } else if (eNB_ID->present == S1AP_ENB_ID_PR_short_macroENB_ID ||
               eNB_ID->present == S1AP_ENB_ID_PR_long_macroENB_ID) {
        BIT_STRING_t *bits = eNB_ID->present == S1AP_ENB_ID_PR_short_macroENB_ID ?
            &eNB_ID->choice.short_macroENB_ID : &eNB_ID->choice.long_macroENB_ID;
        unsigned int unused = eNB_ID->present == S1AP_ENB_ID_PR_short_macroENB_ID ? 6 : 3;
        if (!bits->buf || bits->size != 3 || bits->bits_unused != unused)
            return OGS_ERROR;
        *uint32 = ((uint32_t)bits->buf[0] << 16 |
                (uint32_t)bits->buf[1] << 8 | bits->buf[2]) >> unused;
    } else {
        return OGS_ERROR;
    }

    return OGS_OK;
}
