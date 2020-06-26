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

#ifndef OGS_ASN_CONV_H
#define OGS_ASN_CONV_H

#include "ogs-core.h"

#include "asn_internal.h"
#include "OCTET_STRING.h"
#include "BIT_STRING.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_ASN_CLEAR_DATA(__dATA) \
    do { \
        ogs_assert((__dATA)); \
        if ((__dATA)->buf) { \
            FREEMEM((__dATA)->buf); \
            (__dATA)->buf = NULL; \
            (__dATA)->size = 0; \
        } \
    } while(0)
#define OGS_ASN_STORE_DATA(__dST, __sRC) \
    do { \
        ogs_assert((__sRC)); \
        ogs_assert((__sRC)->buf); \
        ogs_assert((__dST)); \
        OGS_ASN_CLEAR_DATA(__dST); \
        (__dST)->size = (__sRC)->size; \
        (__dST)->buf = CALLOC((__dST)->size, sizeof(uint8_t)); \
        memcpy((__dST)->buf, (__sRC)->buf, (__dST)->size); \
    } while(0)

void ogs_asn_uint8_to_OCTET_STRING(
        uint8_t uint8, OCTET_STRING_t *octet_string);
void ogs_asn_uint16_to_OCTET_STRING(
        uint16_t uint16, OCTET_STRING_t *octet_string);
void ogs_asn_uint24_to_OCTET_STRING(
        ogs_uint24_t uint24, OCTET_STRING_t *octet_string);
void ogs_asn_uint32_to_OCTET_STRING(
        uint32_t uint32, OCTET_STRING_t *octet_string);

void ogs_asn_OCTET_STRING_to_uint8(
        OCTET_STRING_t *octet_string, uint8_t *uint8);
void ogs_asn_OCTET_STRING_to_uint16(
        OCTET_STRING_t *octet_string, uint16_t *uint16);
void ogs_asn_OCTET_STRING_to_uint24(
        OCTET_STRING_t *octet_string, ogs_uint24_t *uint24);
void ogs_asn_OCTET_STRING_to_uint32(
        OCTET_STRING_t *octet_string, uint32_t *uint32);

void ogs_asn_buffer_to_OCTET_STRING(
        void *buf, int size, OCTET_STRING_t *octet_string);

void ogs_asn_buffer_to_BIT_STRING(
        void *buf, int size, int unused, BIT_STRING_t *bit_string);
void ogs_asn_uint32_to_BIT_STRING(
        uint32_t uint32, uint8_t bitsize, BIT_STRING_t *bit_string);
void ogs_asn_BIT_STRING_to_uint32(BIT_STRING_t *bit_string, uint32_t *uint32);

int ogs_asn_BIT_STRING_to_ip(
        BIT_STRING_t *bit_string, ogs_ip_t *ip);
int ogs_asn_ip_to_BIT_STRING(
        ogs_ip_t *ip, BIT_STRING_t *bit_string);

int ogs_asn_copy_ie(
        const asn_TYPE_descriptor_t *td, void *src, void *dst);

#ifdef __cplusplus
}
#endif

#endif /* OGS_ASN_CONV_H */
