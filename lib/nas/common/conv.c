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

void ogs_nas_imeisv_to_bcd(
    ogs_nas_mobile_identity_imeisv_t *imeisv, uint8_t imeisv_len, char *bcd)
{
    int bcd_len;

    bcd[0] = '0' + imeisv->digit1;
    bcd[1] = '0' + imeisv->digit2;
    bcd[2] = '0' + imeisv->digit3;
    bcd[3] = '0' + imeisv->digit4;
    bcd[4] = '0' + imeisv->digit5;
    bcd[5] = '0' + imeisv->digit6;
    bcd[6] = '0' + imeisv->digit7;
    bcd[7] = '0' + imeisv->digit8;
    bcd[8] = '0' + imeisv->digit9;
    bcd[9] = '0' + imeisv->digit10;
    bcd[10] = '0' + imeisv->digit11;
    bcd[11] = '0' + imeisv->digit12;
    bcd[12] = '0' + imeisv->digit13;
    bcd[13] = '0' + imeisv->digit14;
    bcd[14] = '0' + imeisv->digit15;
    bcd[15] = '0' + imeisv->digit16;
    bcd[16] = '0' + imeisv->digit17;

    bcd_len = imeisv_len * 2 - 1;
    if (!imeisv->odd_even) { /* if bcd length is even */
        if (imeisv->digit17 != 0xf) {
            ogs_warn("Spec warning : bcd[%d] = 0x%x, 0x%x",
                    bcd_len-1, imeisv->digit17, bcd[bcd_len-1]);
        }
        (bcd_len)--;
    }

    bcd[bcd_len] = 0;
}

void *ogs_nas_imeisv_bcd_to_buffer(const char *in, uint8_t *out, int *out_len)
{

    /*
     * TS38.413
     * 9.3.1.54 Masked IMEISV
     *
     * The first to fourth bits correspond to the first digit of the IMEISV,
     * the fifth to eighth bits correspond to the second digit of the IMEISV,
     * and so on
     *
     * IMEISV buffer uses different ordering.
     * So, I'll other function instead of ogs_bcd_to_buffer();
     *
     */

    return ogs_bcd_to_buffer_reverse_order(in, out, out_len);
}

int ogs_nas_gprs_timer_from_sec(
        ogs_nas_gprs_timer_t *gprs_timer, ogs_time_t sec)
{
    ogs_time_t timer_value = sec;

    ogs_assert(gprs_timer);
    memset(gprs_timer, 0, sizeof(*gprs_timer));

    if (timer_value <= 63) {
        if (timer_value%2 != 0) {
            ogs_error("Not multiples of 2 seconds");
            return OGS_ERROR;
        }
        gprs_timer->value = timer_value / 2;
    } else {
        if (timer_value%60 != 0) {
            ogs_error("Not multiples of 1 minute");
            return OGS_ERROR;
        }
        timer_value /= 60; /* multiples of 1 minute */
        if (timer_value <= 31) {
            gprs_timer->unit = OGS_NAS_GPRS_TIMER_UNIT_MULTIPLES_OF_1_MM;
            gprs_timer->value = timer_value;
        } else {
            if (timer_value%6 != 0) {
                ogs_error("Not multiples of decihours(= 6 minutes)");
                return OGS_ERROR;
            }
            timer_value /= 6; /* multiples of decihours = 6 minutes */
            if (timer_value <= 31) {
                gprs_timer->unit = OGS_NAS_GPRS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
                gprs_timer->value = timer_value;
            } else {
                ogs_error("Overflow!");
                return OGS_ERROR;
            }
        }
    }

    return OGS_OK;
}

int ogs_nas_gprs_timer_3_from_sec(
        ogs_nas_gprs_timer_t *gprs_timer, ogs_time_t sec)
{
    ogs_time_t timer_value = sec;

    ogs_assert(gprs_timer);
    memset(gprs_timer, 0, sizeof(*gprs_timer));

    if (timer_value <= 63) {
        if (timer_value%2 != 0) {
            ogs_error("Not multiples of 2 seconds");
            return OGS_ERROR;
        }
        gprs_timer->unit = OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_2_SS;
        gprs_timer->value = timer_value / 2;
    } else {
        if (timer_value%60 != 0) {
            ogs_error("Not multiples of 1 minute");
            return OGS_ERROR;
        }
        timer_value /= 60; /* multiples of 1 minute */
        if (timer_value <= 31) {
            gprs_timer->unit = OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_1_MM;
            gprs_timer->value = timer_value;
        } else {
            if (timer_value%10 != 0) {
                ogs_error("Not multiples of decihours(= 10 minutes)");
                return OGS_ERROR;
            }
            timer_value /= 10; /* multiples of decihours = 10 mintues */
            if (timer_value <= 31) {
                gprs_timer->unit = OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_10_MM;
                gprs_timer->value = timer_value;
            } else {
                if (timer_value%6 != 0) {
                    ogs_error("Not multiples of 1 hour");
                    return OGS_ERROR;
                }
                timer_value /= 6; /* multiples of 1 hour */
                if (timer_value <= 31) {
                    gprs_timer->unit =
                        OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_1_HH;
                    gprs_timer->value = timer_value;
                } else {
                    if (timer_value%10 != 0) {
                        ogs_error("Not multiples of 10 hours");
                        return OGS_ERROR;
                    }
                    timer_value /= 10; /* multiples of 10 hours */
                    if (timer_value <= 31) {
                        gprs_timer->unit =
                            OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_10_HH;
                        gprs_timer->value = timer_value;
                    } else {
                        if (timer_value%32 != 0) {
                            ogs_error("Not multiples of 10 hours");
                            return OGS_ERROR;
                        }
                        timer_value /= 32; /* multiples of 320 hours */
                        if (timer_value <= 31) {
                            gprs_timer->unit =
                                OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_320_HH;
                            gprs_timer->value = timer_value;
                        } else {
                            ogs_error("Overflow!");
                            return OGS_ERROR;
                        }
                    }
                }
            }
        }
    }

    return OGS_OK;
}
