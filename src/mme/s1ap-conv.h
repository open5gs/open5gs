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

#ifndef S1AP_CONV_H
#define S1AP_CONV_H

#include "asn1c/s1ap-message.h"

#ifdef __cplusplus
extern "C" {
#endif

void s1ap_uint8_to_OCTET_STRING(
        uint8_t uint8, OCTET_STRING_t *octet_string);
void s1ap_uint16_to_OCTET_STRING(
        uint16_t uint16, OCTET_STRING_t *octet_string);
void s1ap_uint32_to_OCTET_STRING(
        uint32_t uint32, OCTET_STRING_t *octet_string);

void s1ap_buffer_to_OCTET_STRING(
        void *buf, int size, S1AP_TBCD_STRING_t *tbcd_string);

void s1ap_uint32_to_ENB_ID(
        S1AP_ENB_ID_PR present, uint32_t enb_id, S1AP_ENB_ID_t *eNB_ID);
void s1ap_ENB_ID_to_uint32(
        S1AP_ENB_ID_t *eNB_ID, uint32_t *uint32);

int s1ap_BIT_STRING_to_ip(
        BIT_STRING_t *bit_string, ip_t *ip);
int s1ap_ip_to_BIT_STRING(
        ip_t *ip, BIT_STRING_t *bit_string);

int s1ap_copy_ie(
        const asn_TYPE_descriptor_t *td, void *src, void *dst);

#ifdef __cplusplus
}
#endif

#endif /* S1AP_CONV_H */

