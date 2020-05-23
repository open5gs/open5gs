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
    octet_string->size = 1;
    octet_string->buf = CALLOC(octet_string->size, sizeof(uint8_t));

    octet_string->buf[0] = uint8;
}

void ogs_asn_uint16_to_OCTET_STRING(
        uint16_t uint16, OCTET_STRING_t *octet_string)
{
    octet_string->size = 2;
    octet_string->buf = CALLOC(octet_string->size, sizeof(uint8_t));

    octet_string->buf[0] = uint16 >> 8;
    octet_string->buf[1] = uint16;
}

void ogs_asn_uint32_to_OCTET_STRING(
        uint32_t uint32, OCTET_STRING_t *octet_string)
{
    octet_string->size = 4;
    octet_string->buf = CALLOC(octet_string->size, sizeof(uint8_t));

    octet_string->buf[0] = uint32 >> 24;
    octet_string->buf[1] = uint32 >> 16;
    octet_string->buf[2] = uint32 >> 8;
    octet_string->buf[3] = uint32;
}

int ogs_asn_BIT_STRING_to_ip(BIT_STRING_t *bit_string, ogs_ip_t *ip)
{
    char buf[OGS_ADDRSTRLEN], buf2[OGS_ADDRSTRLEN];

    ogs_assert(bit_string);
    ogs_assert(ip);

    if (bit_string->size == OGS_IPV4V6_LEN) {
        ip->ipv4 = 1;
        ip->ipv6 = 1;
        memcpy(&ip->both.addr, bit_string->buf, OGS_IPV4_LEN);
        memcpy(&ip->both.addr6, bit_string->buf+OGS_IPV4_LEN, OGS_IPV6_LEN);
        ogs_debug("    IPv4[%s] IPv6[%s]",
            INET_NTOP(&ip->both.addr, buf), INET6_NTOP(&ip->both.addr6, buf2));
    } else if (bit_string->size == OGS_IPV4_LEN) {
        ip->ipv4 = 1;
        memcpy(&ip->addr, bit_string->buf, OGS_IPV4_LEN);
        ogs_debug("    IPv4[%s]", INET_NTOP(&ip->addr, buf));
    } else if (bit_string->size == OGS_IPV6_LEN) {
        ip->ipv6 = 1;
        memcpy(&ip->addr6, bit_string->buf, OGS_IPV6_LEN);
        ogs_debug("    IPv6[%s]", INET_NTOP(&ip->addr6, buf));
    } else
        ogs_assert_if_reached();

    ip->len =  bit_string->size;

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
        memcpy(bit_string->buf, &ip->both.addr, OGS_IPV4_LEN);
        memcpy(bit_string->buf+OGS_IPV4_LEN, &ip->both.addr6, OGS_IPV6_LEN);
        ogs_debug("    IPv4[%s] IPv6[%s]",
            INET_NTOP(&ip->both.addr, buf), INET6_NTOP(&ip->both.addr6, buf2));
    } else if (ip->ipv4) {
        bit_string->size = OGS_IPV4_LEN;
        bit_string->buf = CALLOC(bit_string->size, sizeof(uint8_t));
        memcpy(bit_string->buf, &ip->addr, OGS_IPV4_LEN);
        ogs_debug("    IPv4[%s]", INET_NTOP(&ip->addr, buf));
    } else if (ip->ipv6) {
        bit_string->size = OGS_IPV6_LEN;
        bit_string->buf = CALLOC(bit_string->size, sizeof(uint8_t));
        memcpy(bit_string->buf, &ip->addr6, OGS_IPV6_LEN);
        ogs_debug("    IPv6[%s]", INET_NTOP(&ip->addr6, buf));
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
