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
            return (128 + (embr1 - (0xbb - 1)) * 2) * 1000;
        if (embr1 >= 0x4b && embr1 <= 0xba)
            return (16 + (embr1 - (0x4b - 1)) * 1) * 1000;
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
      /* GBR is encoded the same way as MBR: */
      decoded->dec_gbr_kbps_dl = dec_mbr_kbps(
                                      source->data.guaranteed_bit_rate_downlink,
                                      decoded->bit_rate_downlink_extended_present ?
                                          &source->data.extended.guaranteed_bit_rate_downlink : NULL,
                                      decoded->bit_rate_downlink_extended2_present ?
                                         &source->data.extended2.guaranteed_bit_rate_downlink : NULL);
      decoded->dec_gbr_kbps_ul = dec_mbr_kbps(
                                      source->data.guaranteed_bit_rate_uplink,
                                      decoded->bit_rate_uplink_extended_present ?
                                         &source->data.extended.guaranteed_bit_rate_uplink : NULL,
                                      decoded->bit_rate_uplink_extended2_present ?
                                        &source->data.extended2.guaranteed_bit_rate_uplink : NULL);
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
    if (transfer_delay_ms >= 10)
        return transfer_delay_ms / 10;
    return 1; /* 0 is "Reserved" Network->MS */
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
    int mbr_extended_dl, mbr_extended_ul;
    int gbr_extended_dl, gbr_extended_ul;
    int extended_dl, extended_ul;

    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert((size_t)data_len >= sizeof(ogs_gtp1_qos_profile_t));

    octet->data = data;
    target = (ogs_gtp1_qos_profile_t *)octet->data;

    /* First, copy the encoded buffer as it is: */
    memcpy(target, &decoded->qos_profile, sizeof(ogs_gtp1_qos_profile_t));

    /* Avoid setting Traffic Handling to 0=Reserved even if ignored based on
     * Interactive/Background Traffic Class: */
    if (target->data.traffic_handling_priority == 0)
        target->data.traffic_handling_priority = 1;

    /* Then, encode in the target position the decoded-provided fields: */
    if (decoded->data_octet6_to_13_present)
        target->data.transfer_delay = enc_transfer_delay_ms(decoded->dec_transfer_delay);

    mbr_extended_dl = enc_mbr_kbps(decoded->dec_mbr_kbps_dl,
                                   &target->data.max_bit_rate_downlink,
                                   &target->data.extended.max_bit_rate_downlink,
                                   &target->data.extended2.max_bit_rate_downlink);
    mbr_extended_ul = enc_mbr_kbps(decoded->dec_mbr_kbps_ul,
                                   &target->data.max_bit_rate_uplink,
                                   &target->data.extended.max_bit_rate_uplink,
                                   &target->data.extended2.max_bit_rate_uplink);
    /* GBR is encoded the same way as MBR: */
    gbr_extended_dl = enc_mbr_kbps(decoded->dec_gbr_kbps_dl,
                                  &target->data.guaranteed_bit_rate_downlink,
                                  &target->data.extended.guaranteed_bit_rate_downlink,
                                  &target->data.extended2.guaranteed_bit_rate_downlink);
    gbr_extended_ul = enc_mbr_kbps(decoded->dec_gbr_kbps_ul,
                                  &target->data.guaranteed_bit_rate_uplink,
                                  &target->data.extended.guaranteed_bit_rate_uplink,
                                  &target->data.extended2.guaranteed_bit_rate_uplink);
    extended_dl = ogs_max(mbr_extended_dl, gbr_extended_dl);
    extended_ul = ogs_max(mbr_extended_ul, gbr_extended_ul);

    /* Finally, set len based on the required octets to encode the fields: */
    if (extended_ul == 2)
        octet->len = 21;
    else if (extended_dl == 2)
        octet->len = 19;
    else if (extended_ul == 1)
        octet->len = 17;
    else if (extended_dl == 1)
        octet->len = 15;
    else if (decoded->data_octet14_present)
        octet->len = 13;
    else if (decoded->data_octet6_to_13_present)
        octet->len = 12;
    else
        octet->len = 6;
    return octet->len;
}

/* 7.7.28 MM Context */
/* TODO: UMTS support, see Figure 41 and Figure 42. */
int ogs_gtp1_build_mm_context(ogs_gtp1_tlv_mm_context_t *octet,
    const ogs_gtp1_mm_context_decoded_t *decoded, uint8_t *data, int data_len)
{
    uint8_t *ptr = data;
    unsigned int i;
    uint16_t *len_ptr;
    uint16_t val16;

    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert((size_t)data_len >= 1);

    octet->data = data;

#define CHECK_SPACE_ERR(bytes) \
    if ((ptr - data) + (bytes) > data_len) \
        return OGS_ERROR
    CHECK_SPACE_ERR(1);
    *ptr++ = (decoded->gupii & 0x01) << 7 |
             (decoded->ugipai & 0x01) << 6 |
             (decoded->used_gprs_protection_algo & 0x07) << 3 |
             (decoded->ksi & 0x07);

    CHECK_SPACE_ERR(1);
    *ptr++ = (decoded->sec_mode & 0x03) << 6 |
             (decoded->num_vectors & 0x07) << 3 |
             (decoded->sec_mode == OGS_GTP1_SEC_MODE_USED_CIPHER_VALUE_UMTS_KEY_AND_QUINTUPLETS ?
                decoded->used_cipher & 0x07 : 0x07);

    CHECK_SPACE_ERR(sizeof(decoded->ck));
    memcpy(ptr, &decoded->ck[0], sizeof(decoded->ck));
    ptr += sizeof(decoded->ck);

    CHECK_SPACE_ERR(sizeof(decoded->ik));
    memcpy(ptr, &decoded->ik[0], sizeof(decoded->ik));
    ptr += sizeof(decoded->ik);

    /* Quintuplet Length */
    CHECK_SPACE_ERR(2);
    len_ptr = (uint16_t *)ptr; /* will be filled later */
    ptr += 2;

    for (i = 0; i < decoded->num_vectors; i++) {
        CHECK_SPACE_ERR(sizeof(decoded->auth_quintuplets[0]));

        memcpy(ptr, &decoded->auth_quintuplets[i].rand, sizeof(decoded->auth_quintuplets[i].rand));
        ptr += sizeof(decoded->auth_quintuplets[i].rand);

        *ptr++ = decoded->auth_quintuplets[i].xres_len;
        memcpy(ptr, &decoded->auth_quintuplets[i].xres[0], decoded->auth_quintuplets[i].xres_len);
        ptr += decoded->auth_quintuplets[i].xres_len;

        memcpy(ptr, &decoded->auth_quintuplets[i].ck, sizeof(decoded->auth_quintuplets[i].ck));
        ptr += sizeof(decoded->auth_quintuplets[i].ck);
        memcpy(ptr, &decoded->auth_quintuplets[i].ik, sizeof(decoded->auth_quintuplets[i].ik));
        ptr += sizeof(decoded->auth_quintuplets[i].ik);

        *ptr++ = decoded->auth_quintuplets[i].autn_len;
        memcpy(ptr, &decoded->auth_quintuplets[i].autn[0], decoded->auth_quintuplets[i].autn_len);
        ptr += decoded->auth_quintuplets[i].autn_len;
    }

    *len_ptr = htobe16(ptr - (((uint8_t *)len_ptr) + 2));

    CHECK_SPACE_ERR(sizeof(decoded->drx_param));
    memcpy(ptr, &decoded->drx_param, sizeof(decoded->drx_param));
    ptr += sizeof(decoded->drx_param);

    if (decoded->ms_network_capability_len != 0) {
        /* MS Network Capability Length */
        CHECK_SPACE_ERR(1 + decoded->ms_network_capability_len);
        *ptr++ = decoded->ms_network_capability_len;
        memcpy(ptr, &decoded->ms_network_capability, decoded->ms_network_capability_len);
        ptr += decoded->ms_network_capability_len;
    } else {
        CHECK_SPACE_ERR(1);
        *ptr++ = 0;
    }

    if (decoded->imeisv_len != 0) {
        /* Container Len */
        CHECK_SPACE_ERR(2);
        val16 = htobe16(2 + decoded->imeisv_len);
        memcpy(ptr, &val16, 2);
        ptr += 2;

        /* Container (Mobile Identity IMEISV), TS 29.060 Table 47A */
        CHECK_SPACE_ERR(2 + decoded->imeisv_len);
        *ptr++ = 0x23;
        *ptr++ = decoded->imeisv_len; /* Length of mobile identity contents */
        memcpy(ptr, &decoded->imeisv[0], decoded->imeisv_len);
        ptr += decoded->imeisv_len;
    } else {
        /* Container Len */
        CHECK_SPACE_ERR(2);
        *ptr++ = 0;
        *ptr++ = 0;
    }

    if (decoded->nrsrna) {
        CHECK_SPACE_ERR(2);
        *ptr++ = 1;
        *ptr++ = 0x01;
    }

    octet->len = (ptr - data);
    return OGS_OK;
#undef CHECK_SPACE_ERR
}

/* The format of EUA in PDP Context is not exactly the same for the entire EUA,
 * hence a separate function is required to encode the value part of the address,
 * instead of using regular ogs_gtp1_ip_to_eua(). */
static int enc_pdp_ctx_as_eua(uint8_t pdu_session_type, const ogs_ip_t *ip,
                              uint8_t *data, int data_len)
{
    switch (pdu_session_type)
    {
    case OGS_PDU_SESSION_TYPE_IPV4:
        if (!ip->ipv4) {
            ogs_error("EUA type IPv4 but no IPv4 address available");
            return OGS_ERROR;
        }
        if (data_len < OGS_IPV4_LEN)
            return OGS_ERROR;
        memcpy(data, &ip->addr, OGS_IPV4_LEN);
        return OGS_IPV4_LEN;
    case OGS_PDU_SESSION_TYPE_IPV6:
        if (!ip->ipv6) {
            ogs_error("EUA type IPv4 but no IPv6 address available");
            return OGS_ERROR;
        }
        if (data_len < OGS_IPV6_LEN)
            return OGS_ERROR;
        memcpy(data, ip->addr6, OGS_IPV6_LEN);
        return OGS_IPV6_LEN;
    case OGS_PDU_SESSION_TYPE_IPV4V6:
        if (ip->ipv4 && ip->ipv6) {
            if (data_len < OGS_IPV4_LEN + OGS_IPV6_LEN)
                return OGS_ERROR;
            memcpy(data, &ip->addr, OGS_IPV4_LEN);
            memcpy(data + OGS_IPV4_LEN, ip->addr6, OGS_IPV6_LEN);
            return OGS_IPV4_LEN + OGS_IPV6_LEN;
        } else if (ip->ipv4) {
            if (data_len < OGS_IPV4_LEN)
                return OGS_ERROR;
            memcpy(data, &ip->addr, OGS_IPV4_LEN);
            return OGS_IPV4_LEN;
        } else if (ip->ipv6) {
            if (data_len < OGS_IPV6_LEN)
                return OGS_ERROR;
            memcpy(data, ip->addr6, OGS_IPV6_LEN);
            return OGS_IPV6_LEN;
        } else {
            ogs_error("EUA type IPv4 but no IPv4 nor IPv6 address available");
            return OGS_ERROR;
        }
        break;
    default:
        ogs_error("Unexpected session type");
        return OGS_ERROR;
    }
    return OGS_OK;
}

/* TS 29.060 7.7.29 PDP Context */
int ogs_gtp1_build_pdp_context(ogs_tlv_octet_t *octet,
    const ogs_gtp1_pdp_context_decoded_t *decoded, uint8_t *data, int data_len)
{
    uint8_t *ptr = data;
    uint16_t val16;
    uint32_t val32;
    int rv;
    ogs_tlv_octet_t qos_sub_tlv_unused;

    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert((size_t)data_len >= 1);

    octet->data = data;

#define CHECK_SPACE_ERR(bytes) \
    if ((ptr - data) + (bytes) > data_len) \
        return OGS_ERROR

    CHECK_SPACE_ERR(1);
    *ptr++ = (decoded->ea << 7) | (decoded->vaa << 6) |
             (decoded->asi << 5)| (decoded->order << 4) |
             (decoded->nsapi & 0x0f);

    CHECK_SPACE_ERR(1);
    *ptr++ = (decoded->sapi & 0x0f);

    /* Quality of Service Subscribed */
    CHECK_SPACE_ERR(1 + OGS_GTP1_QOS_PROFILE_MAX_LEN);
    rv = ogs_gtp1_build_qos_profile(&qos_sub_tlv_unused, &decoded->qos_sub,
            ptr, (data + data_len) - (ptr + 1));
    if (rv < 0)
        return rv;
    *ptr = rv;
    ptr += 1 + rv;

    /* Quality of Service Requested */
    CHECK_SPACE_ERR(1 + OGS_GTP1_QOS_PROFILE_MAX_LEN);
    rv = ogs_gtp1_build_qos_profile(&qos_sub_tlv_unused, &decoded->qos_req,
            ptr, (data + data_len) - (ptr + 1));
    if (rv < 0)
        return rv;
    *ptr = rv;
    ptr += 1 + rv;

    /* Quality of Service Negotiated */
    CHECK_SPACE_ERR(1 + OGS_GTP1_QOS_PROFILE_MAX_LEN);
    rv = ogs_gtp1_build_qos_profile(&qos_sub_tlv_unused, &decoded->qos_neg,
            ptr, (data + data_len) - (ptr + 1));
    if (rv < 0)
        return rv;
    *ptr = rv;
    ptr += 1 + rv;

    CHECK_SPACE_ERR(2);
    val16 = htobe16(decoded->snd);
    memcpy(ptr, &val16, 2);
    ptr += 2;

    CHECK_SPACE_ERR(2);
    val16 = htobe16(decoded->snu);
    memcpy(ptr, &val16, 2);
    ptr += 2;

    CHECK_SPACE_ERR(1);
    *ptr++ = decoded->send_npdu_nr;

    CHECK_SPACE_ERR(1);
    *ptr++ = decoded->receive_npdu_nr;

    CHECK_SPACE_ERR(4);
    val32 = htobe32(decoded->ul_teic);
    memcpy(ptr, &val32, 4);
    ptr += 4;

    CHECK_SPACE_ERR(4);
    val32 = htobe32(decoded->ul_teid);
    memcpy(ptr, &val32, 4);
    ptr += 4;

    CHECK_SPACE_ERR(1);
    *ptr++ = decoded->pdp_ctx_id;

    CHECK_SPACE_ERR(1);
    *ptr++ = 0xf0 | decoded->pdp_type_org;
    CHECK_SPACE_ERR(1);
    *ptr++ = ogs_gtp1_pdu_session_type_to_eua_ietf_type(decoded->pdp_type_num[0]);
    /* PDP Address Length filled after PDP Address */
    CHECK_SPACE_ERR(1);
    rv = enc_pdp_ctx_as_eua(decoded->pdp_type_num[0], &decoded->pdp_address[0],
                            ptr + 1, (data + data_len) - (ptr + 1));
    if (rv < 0)
        return rv;
    *ptr = rv;
    ptr += 1 + rv;

    /* GGSN Address for control plane Length */
    CHECK_SPACE_ERR(1);
    *ptr = decoded->ggsn_address_c.ipv6 ? OGS_GTP_GSN_ADDRESS_IPV6_LEN : OGS_GTP_GSN_ADDRESS_IPV4_LEN;
    CHECK_SPACE_ERR(1 + (*ptr));
    memcpy(ptr + 1,
           decoded->ggsn_address_c.ipv6 ?
                (uint8_t *)decoded->ggsn_address_c.addr6 :
                (uint8_t *)&decoded->ggsn_address_c.addr,
                *ptr);
    ptr += 1 + *ptr;

    /* GGSN Address for User Traffic Length */
    CHECK_SPACE_ERR(1);
    *ptr = decoded->ggsn_address_u.ipv6 ? OGS_GTP_GSN_ADDRESS_IPV6_LEN : OGS_GTP_GSN_ADDRESS_IPV4_LEN;
    CHECK_SPACE_ERR(1 + (*ptr));
    memcpy(ptr + 1,
           decoded->ggsn_address_u.ipv6 ?
                (uint8_t *)decoded->ggsn_address_u.addr6 :
                (uint8_t *)&decoded->ggsn_address_u.addr,
                *ptr);
    ptr += 1 + *ptr;

    /* APN */
    rv = strlen(decoded->apn);
    CHECK_SPACE_ERR(1 + rv + 1);
    *ptr = ogs_fqdn_build(
            (char *)ptr + 1, decoded->apn, rv);
    ptr += 1 + *ptr;

    CHECK_SPACE_ERR(2);
    *ptr++ = (decoded->trans_id >> 8) & 0x0f;
    *ptr++ = decoded->trans_id & 0xff;

    if (decoded->ea == OGS_GTP1_PDPCTX_EXT_EUA_YES) {
        CHECK_SPACE_ERR(1);
        *ptr++ = decoded->pdp_type_num[1];
        /* PDP Address Length filled after PDP Address */
        CHECK_SPACE_ERR(1);
        rv = enc_pdp_ctx_as_eua(decoded->pdp_type_num[1], &decoded->pdp_address[1],
                                ptr + 1, (data + data_len) - (ptr + 1));
        if (rv < 0)
            return rv;
        *ptr = rv;
        ptr += 1 + rv;
    }

    octet->len = (ptr - data);
    return OGS_OK;
#undef CHECK_SPACE_ERR
}
