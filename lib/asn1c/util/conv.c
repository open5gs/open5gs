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

#include "conv.h"

void ogs_asn_uint8_to_OCTET_STRING(
        uint8_t uint8, OCTET_STRING_t *octet_string)
{
    ogs_assert(octet_string);

    octet_string->size = 1;
    octet_string->buf = CALLOC(octet_string->size, sizeof(uint8_t));

    octet_string->buf[0] = uint8;
}

void ogs_asn_uint16_to_OCTET_STRING(
        uint16_t uint16, OCTET_STRING_t *octet_string)
{
    ogs_assert(octet_string);

    octet_string->size = 2;
    octet_string->buf = CALLOC(octet_string->size, sizeof(uint8_t));

    octet_string->buf[0] = uint16 >> 8;
    octet_string->buf[1] = uint16;
}

void ogs_asn_uint24_to_OCTET_STRING(
        ogs_uint24_t uint24, OCTET_STRING_t *octet_string)
{
    ogs_assert(octet_string);

    octet_string->size = 3;
    octet_string->buf = CALLOC(octet_string->size, sizeof(uint8_t));

    octet_string->buf[0] = uint24.v >> 16;
    octet_string->buf[1] = uint24.v >> 8;
    octet_string->buf[2] = uint24.v;
}

void ogs_asn_uint32_to_OCTET_STRING(
        uint32_t uint32, OCTET_STRING_t *octet_string)
{
    ogs_assert(octet_string);

    octet_string->size = 4;
    octet_string->buf = CALLOC(octet_string->size, sizeof(uint8_t));

    octet_string->buf[0] = uint32 >> 24;
    octet_string->buf[1] = uint32 >> 16;
    octet_string->buf[2] = uint32 >> 8;
    octet_string->buf[3] = uint32;
}

void ogs_asn_OCTET_STRING_to_uint8(
        OCTET_STRING_t *octet_string, uint8_t *uint8)
{
    ogs_assert(octet_string);
    ogs_assert(uint8);

    *uint8 = octet_string->buf[0];
}
void ogs_asn_OCTET_STRING_to_uint16(
        OCTET_STRING_t *octet_string, uint16_t *uint16)
{
    ogs_assert(octet_string);
    ogs_assert(uint16);

    *uint16 = (octet_string->buf[0] << 8) + octet_string->buf[1];
}
void ogs_asn_OCTET_STRING_to_uint24(
        OCTET_STRING_t *octet_string, ogs_uint24_t *uint24)
{
    ogs_assert(octet_string);
    ogs_assert(uint24);

    memcpy(uint24, octet_string->buf, sizeof(ogs_uint24_t));
    *uint24 = ogs_be24toh(*uint24);
}
void ogs_asn_OCTET_STRING_to_uint32(
        OCTET_STRING_t *octet_string, uint32_t *uint32)
{
    ogs_assert(octet_string);
    ogs_assert(uint32);

    *uint32 = (octet_string->buf[0] << 24) + (octet_string->buf[1] << 16) +
                (octet_string->buf[2] << 8) + octet_string->buf[3];
}

void ogs_asn_buffer_to_OCTET_STRING(
        void *buf, int size, OCTET_STRING_t *octet_string)
{
    octet_string->size = size;
    octet_string->buf = CALLOC(octet_string->size, sizeof(uint8_t));

    memcpy(octet_string->buf, buf, size);
}

void ogs_asn_buffer_to_BIT_STRING(
        void *buf, int size, int unused, BIT_STRING_t *bit_string)
{
    bit_string->size = size;
    bit_string->buf = CALLOC(bit_string->size, sizeof(uint8_t));
    bit_string->bits_unused = unused;

    memcpy(bit_string->buf, buf, size);
}

void ogs_asn_uint32_to_BIT_STRING(
        uint32_t uint32, uint8_t bitsize, BIT_STRING_t *bit_string)
{
    char tmp[32];
    uint64_t uint64;
    ogs_assert(bit_string);

    uint64 = uint32;
    ogs_uint64_to_buffer(
            uint64 << ((32 - bitsize) % 8), (bitsize + 7) / 8, tmp);
    ogs_asn_buffer_to_BIT_STRING(
            tmp, (bitsize + 7) / 8, (32 - bitsize) % 8, bit_string);
}

void ogs_asn_BIT_STRING_to_uint32(BIT_STRING_t *bit_string, uint32_t *uint32)
{
    ogs_assert(bit_string);
    ogs_assert(uint32);

    *uint32 = ogs_buffer_to_uint64(bit_string->buf, bit_string->size)
                    >> bit_string->bits_unused;
}

int ogs_asn_BIT_STRING_to_ip(BIT_STRING_t *bit_string, ogs_ip_t *ip)
{
    char buf[OGS_ADDRSTRLEN], buf2[OGS_ADDRSTRLEN];

    ogs_assert(bit_string);
    ogs_assert(ip);

    memset(ip, 0, sizeof(*ip));

    if (bit_string->size == OGS_IPV4V6_LEN) {
        ip->ipv4 = 1;
        ip->ipv6 = 1;
        memcpy(&ip->addr, bit_string->buf, OGS_IPV4_LEN);
        memcpy(&ip->addr6, bit_string->buf+OGS_IPV4_LEN, OGS_IPV6_LEN);
        ogs_debug("    IPv4[%s] IPv6[%s]",
            OGS_INET_NTOP(&ip->addr, buf),
            OGS_INET6_NTOP(&ip->addr6, buf2));
    } else if (bit_string->size == OGS_IPV4_LEN) {
        ip->ipv4 = 1;
        memcpy(&ip->addr, bit_string->buf, OGS_IPV4_LEN);
        ogs_debug("    IPv4[%s]", OGS_INET_NTOP(&ip->addr, buf));
    } else if (bit_string->size == OGS_IPV6_LEN) {
        ip->ipv6 = 1;
        memcpy(&ip->addr6, bit_string->buf, OGS_IPV6_LEN);
        ogs_debug("    IPv6[%s]", OGS_INET_NTOP(&ip->addr6, buf));
    } else
        ogs_assert_if_reached();

    ip->len = bit_string->size;

    return OGS_OK;
}
int ogs_asn_ip_to_BIT_STRING(ogs_ip_t *ip, BIT_STRING_t *bit_string)
{
    char buf[OGS_ADDRSTRLEN], buf2[OGS_ADDRSTRLEN];

    ogs_assert(ip);
    ogs_assert(bit_string);

    if (ip->ipv4 && ip->ipv6) {
        bit_string->size = OGS_IPV4V6_LEN;
        bit_string->buf = CALLOC(bit_string->size, sizeof(uint8_t));
        memcpy(bit_string->buf, &ip->addr, OGS_IPV4_LEN);
        memcpy(bit_string->buf+OGS_IPV4_LEN, &ip->addr6, OGS_IPV6_LEN);
        ogs_debug("    IPv4[%s] IPv6[%s]",
            OGS_INET_NTOP(&ip->addr, buf),
            OGS_INET6_NTOP(&ip->addr6, buf2));
    } else if (ip->ipv4) {
        bit_string->size = OGS_IPV4_LEN;
        bit_string->buf = CALLOC(bit_string->size, sizeof(uint8_t));
        memcpy(bit_string->buf, &ip->addr, OGS_IPV4_LEN);
        ogs_debug("    IPv4[%s]", OGS_INET_NTOP(&ip->addr, buf));
    } else if (ip->ipv6) {
        bit_string->size = OGS_IPV6_LEN;
        bit_string->buf = CALLOC(bit_string->size, sizeof(uint8_t));
        memcpy(bit_string->buf, &ip->addr6, OGS_IPV6_LEN);
        ogs_debug("    IPv6[%s]", OGS_INET_NTOP(&ip->addr6, buf));
    } else
        ogs_assert_if_reached();

    return OGS_OK;
}

int ogs_asn_copy_ie(const asn_TYPE_descriptor_t *td, void *src, void *dst)
{
    asn_enc_rval_t enc_ret = {0};
    asn_dec_rval_t dec_ret = {0};
    uint8_t buffer[OGS_MAX_SDU_LEN];

    ogs_assert(td);
    ogs_assert(src);
    ogs_assert(dst);

    enc_ret = aper_encode_to_buffer(td, NULL, src, buffer, OGS_MAX_SDU_LEN);
    if (enc_ret.encoded < 0) {
        ogs_error("aper_encode_to_buffer() failed[%d]", (int)enc_ret.encoded);
        return OGS_ERROR;
    }

    dec_ret = aper_decode(NULL, td, (void **)&dst,
            buffer, ((enc_ret.encoded + 7) / 8), 0, 0);

    if (dec_ret.code != RC_OK) {
        ogs_error("aper_decode() failed[%d]", dec_ret.code);
        return OGS_ERROR;
    }

    return OGS_OK;
}
