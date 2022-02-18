/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#include "ogs-gtp.h"

/* 7.7.51 User Location Information (ULI) */
int16_t ogs_gtp1_parse_uli(ogs_gtp1_uli_t *uli, ogs_tlv_octet_t *octet)
{
    ogs_gtp1_uli_t *source = (ogs_gtp1_uli_t *)octet->data;
    int16_t size = 0;

    ogs_assert(uli);
    ogs_assert(octet);

    memset(uli, 0, sizeof(ogs_gtp1_uli_t));

    uli->geo_loc_type = source->geo_loc_type;
    size++;

    switch (uli->geo_loc_type) {
    case OGS_GTP1_GEO_LOC_TYPE_CGI:
        ogs_assert(size + sizeof(uli->cgi) <= octet->len);
        memcpy(&uli->cgi,
                (unsigned char *)octet->data + size, sizeof(uli->cgi));
        uli->cgi.lac = be16toh(uli->cgi.lac);
        uli->cgi.ci = be16toh(uli->cgi.ci);
        size += sizeof(uli->cgi);
        break;
    case OGS_GTP1_GEO_LOC_TYPE_SAI:
        ogs_assert(size + sizeof(uli->sai) <= octet->len);
        memcpy(&uli->sai,
                (unsigned char *)octet->data + size, sizeof(uli->sai));
        uli->sai.lac = be16toh(uli->sai.lac);
        uli->sai.sac = be16toh(uli->sai.sac);
        size += sizeof(uli->sai);
        break;
    case OGS_GTP1_GEO_LOC_TYPE_RAI:
        ogs_assert(size + sizeof(uli->rai) <= octet->len);
        memcpy(&uli->rai,
                (unsigned char *)octet->data + size, sizeof(uli->rai));
        uli->rai.lac = be16toh(uli->rai.lac);
        uli->rai.rac = be16toh(uli->rai.rac); //FIXME: Only the first octet (10) contains the RAC and the second octet (11) is coded as "11111111"
        size += sizeof(uli->rai);
        break;
    default:
        ogs_warn("Unknown Geographic Location Type 0x%x in User Location Information IE",
                 uli->geo_loc_type);
        return 0;
    }

    ogs_assert(size == octet->len);
    return size;
}
int16_t ogs_gtp1_build_uli(
        ogs_tlv_octet_t *octet, ogs_gtp1_uli_t *uli, void *data, int data_len)
{
    ogs_gtp1_uli_t target;
    int16_t size = 0;

    ogs_assert(uli);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len);

    octet->data = data;
    memcpy(&target, uli, sizeof(ogs_gtp1_uli_t));

    ogs_assert(size + sizeof(target.geo_loc_type) <= data_len);
    memcpy((unsigned char *)octet->data + size,
            &target.geo_loc_type, sizeof(target.geo_loc_type));
    size += sizeof(target.geo_loc_type);

    if (target.geo_loc_type == OGS_GTP1_GEO_LOC_TYPE_CGI) {
        ogs_assert(size + sizeof(target.cgi) <= data_len);
        target.cgi.lac = htobe16(target.cgi.lac);
        target.cgi.ci = htobe16(target.cgi.ci);
        memcpy((unsigned char *)octet->data + size,
                &target.cgi, sizeof(target.cgi));
        size += sizeof(target.cgi);
    }
    if (target.geo_loc_type == OGS_GTP1_GEO_LOC_TYPE_SAI) {
        ogs_assert(size + sizeof(target.sai) <= data_len);
        target.sai.lac = htobe16(target.sai.lac);
        target.sai.sac = htobe16(target.sai.sac);
        memcpy((unsigned char *)octet->data + size,
                &target.sai, sizeof(target.sai));
        size += sizeof(target.sai);
    }
    if (target.geo_loc_type == OGS_GTP1_GEO_LOC_TYPE_RAI) {
        ogs_assert(size + sizeof(target.rai) <= data_len);
        target.rai.lac = htobe16(target.rai.lac);
        target.rai.rac = htobe16(target.rai.rac); //FIXME: Only the first octet (10) contains the RAC and the second octet (11) is coded as "11111111"
        memcpy((unsigned char *)octet->data + size,
                &target.rai, sizeof(target.rai));
        size += sizeof(target.rai);
    }

    octet->len = size;

    return octet->len;
}

static uint32_t dec_mbr_kbps(uint8_t mbr_byte, const uint8_t *extended_mbr_byte, const uint8_t *extended2_mbr_byte)
{
    uint8_t mbr, embr1, embr2;
    mbr = mbr_byte;
    embr1 = extended_mbr_byte ? *extended_mbr_byte : 0;
    embr2 = extended2_mbr_byte ? *extended2_mbr_byte : 0;

    if (mbr == 0)
        return 0;
    if (mbr == 0xff)
        return UINT32_MAX;
    if (mbr == 0xfe) { /* Check extended field */
        if (extended_mbr_byte == NULL || embr1 == 0)
            return 8640;
        if (embr1 == 0xfa) { /* Check extended2 field */
            if (extended2_mbr_byte == NULL || embr2 == 0)
                return 256 * 1000;
            if (embr2 == 0xf6)
                return 10 * 1000 * 1000; /* TODO: check "extended quality of service" IE */
            if (embr2 >= 0xa2 && embr2 <= 0xf6)
                return (1500 + (embr2 - (0xa2 - 1)) * 100) * 1000;
            if (embr2 >= 0x3e && embr2 <= 0xa1)
                return (500 + (embr2 - (0x3e - 1)) * 10) * 1000;
            return (256 + embr2 * 4) * 1000;
        }
        if (embr1 >= 0xbb && embr1 <= 0xfa)
            return (128 + (embr1 - (0xbb - 1)) * 1000) * 1000;
        if (embr1 >= 0x4b && embr1 <= 0xba)
            return (16 + (embr1 - (0x4b - 1)) * 100) * 1000;
        return 8600 + embr1 * 100;
    }
    if (mbr & 0x80) {
        mbr &= ~0x80;
        return 576 + mbr_byte * 64;
    }
    if (mbr & 0x40) {
        mbr &= ~0x40;
        return 64 + mbr * 8;
    }
    return mbr;
}

static uint16_t dec_transfer_delay_ms(uint8_t transfer_delay_byte)
{
    transfer_delay_byte &= 0x3f; /* 6 bits */
    if (transfer_delay_byte <= 0x0f)
        return transfer_delay_byte;
    if (transfer_delay_byte <= 0x1f)
        return 200 + (transfer_delay_byte - 0x10) * 50;
    return 1000 + (transfer_delay_byte - 0x20) * 100;
}

/* TS 29.060 7.7.34 Quality of Service (QoS) Profile */
/* TS 24.008 10.5.6.5 Quality of service */
int16_t ogs_gtp1_parse_qos_profile(
    ogs_gtp1_qos_profile_decoded_t *decoded, const ogs_tlv_octet_t *octet)
{
    ogs_gtp1_qos_profile_t *source = (ogs_gtp1_qos_profile_t *)octet->data;

    ogs_assert(decoded);
    ogs_assert(octet);

    memset(decoded, 0, sizeof(ogs_gtp1_qos_profile_decoded_t));
    switch (octet->len) {
    case 21: /* ARP + octet 3 + octet 3-22 */
        decoded->bit_rate_uplink_extended2_present = true;
        OGS_GNUC_FALLTHROUGH;
    case 19: /* ARP + octet 3 + octet 3-20 */
        decoded->bit_rate_downlink_extended2_present = true;
        OGS_GNUC_FALLTHROUGH;
    case 17: /* ARP + octet 3 + octet 3-18 */
        decoded->bit_rate_uplink_extended_present = true;
        OGS_GNUC_FALLTHROUGH;
    case 15: /* ARP + octet 3 + octet 3-16 */
        decoded->bit_rate_downlink_extended_present = true;
        OGS_GNUC_FALLTHROUGH;
    case 13: /* ARP + octet 3-14 */
        decoded->data_octet14_present = true;
        OGS_GNUC_FALLTHROUGH;
    case 12: /* ARP + octet 3-13 */
        decoded->data_octet6_to_13_present = true;
        OGS_GNUC_FALLTHROUGH;
    case 4: /* ARP + octet 3 + octet 4 + octet 5 */
        break;
    default:
        ogs_warn("Qos Profile wrong length %u", octet->len);
        return -1;
    }
    memcpy(&decoded->qos_profile, source, octet->len);

    /* Calculate resulting MBRs in kbps: */
    if (decoded->data_octet6_to_13_present) {
        decoded->dec_transfer_delay = dec_transfer_delay_ms(source->data.transfer_delay);
        decoded->dec_mbr_kbps_dl = dec_mbr_kbps(
                                        source->data.max_bit_rate_downlink,
                                        decoded->bit_rate_downlink_extended_present ?
                                            &source->data.extended.max_bit_rate_downlink : NULL,
                                        decoded->bit_rate_downlink_extended2_present ?
                                           &source->data.extended2.max_bit_rate_downlink : NULL);
        decoded->dec_mbr_kbps_ul = dec_mbr_kbps(
                                        source->data.max_bit_rate_uplink,
                                        decoded->bit_rate_uplink_extended_present ?
                                           &source->data.extended.max_bit_rate_uplink : NULL,
                                        decoded->bit_rate_uplink_extended2_present ?
                                          &source->data.extended2.max_bit_rate_uplink : NULL);
    }

    return octet->len;
}

static uint8_t enc_transfer_delay_ms(uint16_t transfer_delay_ms)
{
    if (transfer_delay_ms >= 4000)
        return 0x3e;
    if (transfer_delay_ms >= 1000) {
        transfer_delay_ms -= 1000;
        return 0x20 + (transfer_delay_ms / 100);
    }
    if (transfer_delay_ms >= 200) {
        transfer_delay_ms -= 200;
        return 0x10 + (transfer_delay_ms / 50);
    }
    if (transfer_delay_ms > 150)
        transfer_delay_ms = 150;
    return transfer_delay_ms / 10;
}

#define CHECK_EXT1 0xfe
#define CHECK_EXT2 0xfa
static uint32_t enc_mbr_kbps(uint32_t mbr_kbps, uint8_t *mbr_byte, uint8_t *extended_mbr_byte, uint8_t *extended2_mbr_byte)
{
    /* up to EXT2 byte: */
    if (mbr_kbps > 10*1000*1000) {
        *extended2_mbr_byte = 0xf6; /* TODO: need to set the real value somewhere else */
        goto ret_check_ext2;
    }
    if (mbr_kbps >= 1600*1000) {
        mbr_kbps -= 1500*1000;
        *extended2_mbr_byte = 0xa1 + mbr_kbps/(100*1000);
        goto ret_check_ext2;
    }
    if (mbr_kbps >= 510*1000) {
        mbr_kbps -= 500*1000;
        *extended2_mbr_byte = 0x2d + mbr_kbps/(10*1000);
        goto ret_check_ext2;
    }
    if (mbr_kbps >= 260*1000) {
        mbr_kbps -= 256*1000;
        *extended2_mbr_byte = 0x00 + mbr_kbps/(4*1000);
        goto ret_check_ext2;
    }

    /* up to EXT1 byte: */
    if (mbr_kbps >= 130*1000) {
        mbr_kbps -= 128*1000;
        *extended_mbr_byte = 0xba +  mbr_kbps/(2*1000);
        goto ret_check_ext1;
    }
    if (mbr_kbps >= 17*1000) {
        mbr_kbps -= 16*1000;
        *extended_mbr_byte = 0x4a +  mbr_kbps/(1*1000);
        goto ret_check_ext1;
    }
    if (mbr_kbps >= 8700) {
        mbr_kbps -= 8600;
        *extended_mbr_byte = 0x00 +  mbr_kbps/(100);
        goto ret_check_ext1;
    }

    /* Only MBR byte: */
    if (mbr_kbps >= 576) {
        mbr_kbps -= 576;
        *mbr_byte = 0x80 +  mbr_kbps/(64);
        goto ret_mbr;
    }
    if (mbr_kbps >= 64) {
        mbr_kbps -= 64;
        *mbr_byte = 0x40 + mbr_kbps/(8);
        goto ret_mbr;
    }
    if (mbr_kbps > 0) {
        *mbr_byte = mbr_kbps;
        goto ret_mbr;
    }
    /* if (mbr_kpbs == 0) */
    *mbr_byte = 0xff;
    goto ret_mbr;

ret_check_ext2:
    *extended_mbr_byte = CHECK_EXT2;
    *mbr_byte = CHECK_EXT1;
    return 2;
ret_check_ext1:
    *extended2_mbr_byte = 0;
    *mbr_byte = CHECK_EXT1;
    return 1;
ret_mbr:
    *extended2_mbr_byte = 0;
    *extended_mbr_byte = 0;
    return 0;
}

int16_t ogs_gtp1_build_qos_profile(ogs_tlv_octet_t *octet,
    const ogs_gtp1_qos_profile_decoded_t *decoded, void *data, int data_len)
{
    ogs_gtp1_qos_profile_t *target;
    int extended_dl, extended_ul;

    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert((size_t)data_len >= sizeof(ogs_gtp1_qos_profile_t));

    octet->data = data;
    target = (ogs_gtp1_qos_profile_t *)octet->data;

    /* First, encode in the target position the decoded-provided fields: */

    if (decoded->data_octet6_to_13_present)
        target->data.transfer_delay = enc_transfer_delay_ms(decoded->dec_transfer_delay);

    /* TODO: prefill with default values the extended bytes */
    extended_dl = enc_mbr_kbps(decoded->dec_mbr_kbps_dl,
                                &target->data.max_bit_rate_downlink,
                                &target->data.extended.max_bit_rate_downlink,
                                &target->data.extended2.max_bit_rate_downlink);
    extended_ul = enc_mbr_kbps(decoded->dec_mbr_kbps_ul,
                                &target->data.max_bit_rate_uplink,
                                &target->data.extended.max_bit_rate_uplink,
                                &target->data.extended2.max_bit_rate_uplink);

    memcpy(target, &decoded->qos_profile, sizeof(ogs_gtp1_qos_profile_t));
    if (extended_ul == 2)
        octet->len = 23;
    else if (extended_dl == 2)
        octet->len = 21;
    else if (extended_ul == 1)
        octet->len = 19;
    else if (extended_dl == 1)
        octet->len = 17;
    else if (decoded->data_octet14_present)
        octet->len = 15;
    else if (decoded->data_octet6_to_13_present)
        octet->len = 14;
    else
        octet->len = 6;
    return octet->len;
}
