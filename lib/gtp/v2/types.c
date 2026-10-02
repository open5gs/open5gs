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

#include "ogs-gtp.h"

/* 8.13 Protocol Configuration Options (PCO)
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008 */

/* 8.15 Bearer Quality of Service (Bearer QoS) */
int16_t ogs_gtp2_parse_bearer_qos(
    ogs_gtp2_bearer_qos_t *bearer_qos, ogs_tlv_octet_t *octet)
{
    ogs_gtp2_bearer_qos_t *source = NULL;
    int16_t size = 0;

    ogs_assert(bearer_qos);
    ogs_assert(octet);

    /* Validate IE length instead of asserting */
    if (octet->len != GTP2_BEARER_QOS_LEN) {
        ogs_error("Invalid Bearer QoS IE length [%u], expected [%u]",
                octet->len, GTP2_BEARER_QOS_LEN);
        return 0;
    }

    source = (ogs_gtp2_bearer_qos_t *)octet->data;

    memset(bearer_qos, 0, sizeof(ogs_gtp2_bearer_qos_t));

    bearer_qos->pre_emption_capability = source->pre_emption_capability;
    bearer_qos->priority_level = source->priority_level;
    bearer_qos->pre_emption_vulnerability = source->pre_emption_vulnerability;
    size++;

    bearer_qos->qci = source->qci;
    size++;

    /*
     * Ch 8.15 Bearer QoS in TS 29.274 v15.9.0
     *
     * The UL/DL MBR and GBR fields are encoded as kilobits
     * per second (1 kbps = 1000 bps) in binary value.
     */
    bearer_qos->ul_mbr = ogs_buffer_to_uint64(
            (unsigned char *)octet->data + size, 5) * 1000;
    size += 5;
    bearer_qos->dl_mbr = ogs_buffer_to_uint64(
            (unsigned char *)octet->data + size, 5) * 1000;
    size += 5;
    bearer_qos->ul_gbr = ogs_buffer_to_uint64(
            (unsigned char *)octet->data + size, 5) * 1000;
    size += 5;
    bearer_qos->dl_gbr = ogs_buffer_to_uint64(
            (unsigned char *)octet->data + size, 5) * 1000;
    size += 5;

    ogs_assert(size == octet->len);

    return size;
}
int16_t ogs_gtp2_build_bearer_qos(ogs_tlv_octet_t *octet,
        ogs_gtp2_bearer_qos_t *bearer_qos, void *data, int data_len)
{
    ogs_gtp2_bearer_qos_t target;
    int16_t size = 0;

    ogs_assert(bearer_qos);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len >= GTP2_BEARER_QOS_LEN);

    octet->data = data;
    memcpy(&target, bearer_qos, sizeof(ogs_gtp2_bearer_qos_t));

    memcpy((unsigned char *)octet->data + size, &target, 2);
    size += 2;

    /*
     * Ch 8.15 Bearer QoS in TS 29.274 v15.9.0
     *
     * The UL/DL MBR and GBR fields are encoded as kilobits
     * per second (1 kbps = 1000 bps) in binary value.
     */
    ogs_uint64_to_buffer(target.ul_mbr / 1000, 5,
            (unsigned char *)octet->data + size);
    size += 5;
    ogs_uint64_to_buffer(target.dl_mbr / 1000, 5,
            (unsigned char *)octet->data + size);
    size += 5;
    ogs_uint64_to_buffer(target.ul_gbr / 1000, 5,
            (unsigned char *)octet->data + size);
    size += 5;
    ogs_uint64_to_buffer(target.dl_gbr / 1000, 5,
            (unsigned char *)octet->data + size);
    size += 5;

    octet->len = size;

    return octet->len;
}

/* 8.16 Flow Quality of Service (Flow QoS) */
uint64_t ogs_gtp2_qos_to_kbps(uint8_t br, uint8_t extended, uint8_t extended2)
{
    /*
     * Octet 12 : 00000000
     * 00000000 Use the value indicated by the bit rate in octet 4 and 8
     *
     * Octet 12 : 00000001 - 00111101
     * 256Mbps + the binary coded value in 8 bits * 4Mbps
     * giving a range of 260 Mbps to 500 Mbps in 4 Mbps increments.
     *
     * Octet 12 : 00111110 - 10100001
     * 500Mbps + (the binary coded value in 8 bits - 00111101) * 10Mbps
     * giving a range of 510 Mbps to 1500 Mbps in 10 Mbps increments.
     *
     * Octet 12 : 10100010 - 11110110
     * 1500Mbps + (the binary coded value in 8 bits - 10100001) * 100Mbps
     * giving a range of 1600 Mbps to 10 Gbps Mbps in 100 Mbps increaments.
     */
    if (extended2 >= 0b00000001 && extended2 <= 0b00111101) {
        return 256*1000 + extended2 * 4*1000;
    } else if (extended2 >= 0b00111110 && extended2 <= 0b10100001) {
        return 500*1000 + (extended2 - 0b00111101) * 10*1000;
    } else if (extended2 >= 0b10100010 && extended2 <= 0b11110110) {
        return 1500*1000 + (extended2 - 0b10100001) * 100*1000;
    } else if (extended2 > 0b11110110) {
        ogs_error("Protocol Error : extended2[%x]", extended2);
        return 10*1000*1000; /* 10*1000 Mbps */

    /*
     * Octet 8
     * 00000000 Use the value indicated by the bit rate in octet 4
     *
     * Octet 8 : 00000001 - 01001010
     * 8600 kbps + (the binary coded value in 8 bits) * 100 kbps
     * giving a range of 8700 kbps to 16000 kbps in 100 kbps increments.
     *
     * Octet 8 : 01001011 - 10111010
     * 16 Mbps + (the binary coded value in 8 bits - 01001010) * 1 Mbps
     * giving a range of 17 Mbps to 128 Mbps in 1 Mbps increments.
     *
     * Octet 8 : 10111011 - 11111010
     * 128 Mbps + (the binary coded value in 8 bits - 10111010) * 2 Mbps
     * giving a range of 130 Mbps to 256 Mbps in 2 Mbps increments.
     */
    } else if (extended >= 0b00000001 && extended <= 0b01001010) {
        return 8600 + extended * 100;
    } else if (extended >= 0b01001011 && extended <= 0b10111010) {
        return 16*1000 + (extended - 0b01001010) * 1*1000;
    } else if (extended >= 0b10111011 && extended <= 0b11111010) {
        return 128*1000 + (extended - 0b10111010) * 2*1000;
    } else if (extended > 0b11111010) {
        ogs_error("Protocol Error : extended[%x]", extended);
        return 256*1000; /* 256 Mbps */

    /*
     * Octet 4
     *
     * In UE to network direction:
     * 00000000 Subscribed maximum bit rate
     *
     * In network to UE direction:
     * 00000000 Reserved
     *
     * Octet 4 : 00000001 - 00111111
     * giving a range of 1 kbps to 63 kbps in 1 kbps increments.
     *
     * Octet 4 : 01000000 - 01111111
     * 64 kbps + (the binary coded value in 8 bits - 01000000) * 8 kbps
     * giving a range of 64 kbps to 568 kbps in 8 kbps increments.
     *
     * Octet 4 : 10000000 - 11111110
     * 576 kbps + (the binary coded value in 8 bits – 10000000) * 64 kbps
     * giving a range of 576 kbps to 8640 kbps in 64 kbps increments.
     */
    } else if (br == 0xff) {
        return 0; /* 0kbps */
    } else if (br >= 0b00000001 && br <= 0b00111111) {
        return br;
    } else if (br >= 0b01000000 && br <= 0b01111111) {
        return 64 + (br - 0b01000000) * 8;
    } else if (br >= 0b10000000 && br <= 0b11111110) {
        return 576 + (br - 0b10000000) * 64;
    }

    ogs_fatal("invalid param : br[%d], extended[%d], extended2[%d]",
            br, extended, extended2);
    ogs_assert_if_reached();
    return 0;
}

int16_t ogs_gtp2_parse_flow_qos(
    ogs_gtp2_flow_qos_t *flow_qos, ogs_tlv_octet_t *octet)
{
    ogs_gtp2_flow_qos_t *source = NULL;
    int16_t size = 0;

    ogs_assert(flow_qos);
    ogs_assert(octet);

    if (octet->len != GTP2_FLOW_QOS_LEN) {
        ogs_error("Invalid Flow QoS IE length [%u], expected [%u]",
                octet->len, GTP2_FLOW_QOS_LEN);
        return 0;
    }

    source = (ogs_gtp2_flow_qos_t *)octet->data;

    memset(flow_qos, 0, sizeof(ogs_gtp2_flow_qos_t));

    flow_qos->qci = source->qci;
    size++;

    /*
     * Ch 8.16 Flow QoS in TS 29.274 v15.9.0
     *
     * The UL/DL MBR and GBR fields are encoded as kilobits
     * per second (1 kbps = 1000 bps) in binary value.
     */
    flow_qos->ul_mbr = ogs_buffer_to_uint64(
            (unsigned char *)octet->data + size, 5) * 1000;
    size += 5;
    flow_qos->dl_mbr = ogs_buffer_to_uint64(
            (unsigned char *)octet->data + size, 5) * 1000;
    size += 5;
    flow_qos->ul_gbr = ogs_buffer_to_uint64(
            (unsigned char *)octet->data + size, 5) * 1000;
    size += 5;
    flow_qos->dl_gbr = ogs_buffer_to_uint64(
            (unsigned char *)octet->data + size, 5) * 1000;
    size += 5;

    ogs_assert(size == octet->len);

    return size;
}
int16_t ogs_gtp2_build_flow_qos(ogs_tlv_octet_t *octet,
        ogs_gtp2_flow_qos_t *flow_qos, void *data, int data_len)
{
    ogs_gtp2_flow_qos_t target;
    int16_t size = 0;

    ogs_assert(flow_qos);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len >= GTP2_FLOW_QOS_LEN);

    octet->data = data;
    memcpy(&target, flow_qos, sizeof(ogs_gtp2_flow_qos_t));

    memcpy((unsigned char *)octet->data + size, &target, 2);
    size += 1;

    /*
     * Ch 8.16 Flow QoS in TS 29.274 v15.9.0
     *
     * The UL/DL MBR and GBR fields are encoded as kilobits
     * per second (1 kbps = 1000 bps) in binary value.
     */
    ogs_uint64_to_buffer(target.ul_mbr / 1000, 5,
            (unsigned char *)octet->data + size);
    size += 5;
    ogs_uint64_to_buffer(target.dl_mbr / 1000, 5,
            (unsigned char *)octet->data + size);
    size += 5;
    ogs_uint64_to_buffer(target.ul_gbr / 1000, 5,
            (unsigned char *)octet->data + size);
    size += 5;
    ogs_uint64_to_buffer(target.dl_gbr / 1000, 5,
            (unsigned char *)octet->data + size);
    size += 5;

    octet->len = size;

    return octet->len;
}

/* 8.19 EPS Bearer Level Traffic Flow Template (Bearer TFT)
 * See subclause 10.5.6.12 in 3GPP TS 24.008 [13]. */
int16_t ogs_gtp2_parse_tft(ogs_gtp2_tft_t *tft, ogs_tlv_octet_t *octet)
{
    int16_t size = 0;
    int i, j, len = 0;

    ogs_assert(tft);
    ogs_assert(octet);

    memset(tft, 0, sizeof(ogs_gtp2_tft_t));

    if (size + (int)sizeof(tft->flags) > octet->len) {
        ogs_error("TFT: size[%d]+flags[%d] > IE Length[%d]",
                size, (int)sizeof(tft->flags), octet->len);
        return size;
    }
    memcpy(&tft->flags, (unsigned char *)octet->data+size, sizeof(tft->flags));
    size++;

    if (tft->code == OGS_GTP2_TFT_CODE_IGNORE_THIS_IE) {
        ogs_error("Invalid TFT Code(Spare)");
        return size;
    }

    if (tft->code == OGS_GTP2_TFT_CODE_NO_TFT_OPERATION ||
        tft->code == OGS_GTP2_TFT_CODE_DELETE_EXISTING_TFT)
        return size;

    for (i = 0; i < tft->num_of_packet_filter &&
                i < OGS_MAX_NUM_OF_FLOW_IN_GTP ; i++) {
        if (size + (int)sizeof(tft->pf[i].flags) > octet->len) {
            ogs_error("TFT: size[%d]+pf[%d].flags[%d] > IE Length[%d]",
                    size, i, (int)sizeof(tft->pf[i].flags), octet->len);
            return size;
        }
        memcpy(&tft->pf[i].flags, (unsigned char *)octet->data+size,
                sizeof(tft->pf[i].flags));
        size += sizeof(tft->pf[i].flags);

        if (tft->code == OGS_GTP2_TFT_CODE_DELETE_PACKET_FILTERS_FROM_EXISTING)
            continue;

        if (size + (int)sizeof(tft->pf[i].precedence) > octet->len) {
            ogs_error("TFT: size[%d]+pf[%d].precedence[%d] > IE Length[%d]",
                    size, i, (int)sizeof(tft->pf[i].precedence), octet->len);
            return size;
        }
        memcpy(&tft->pf[i].precedence, (unsigned char *)octet->data+size,
                sizeof(tft->pf[i].precedence));
        size += sizeof(tft->pf[i].precedence);

        if (size + (int)sizeof(tft->pf[i].content.length) > octet->len) {
            ogs_error("TFT: size[%d]+pf[%d].content.length[%d] > IE Length[%d]",
                    size, i, (int)sizeof(tft->pf[i].content.length),
                    octet->len);
            return size;
        }
        memcpy(&tft->pf[i].content.length, (unsigned char *)octet->data+size,
                sizeof(tft->pf[i].content.length));
        size += sizeof(tft->pf[i].content.length);

        /*
         * Critical validation:
         * content.length must not exceed remaining IE length.
         * This prevents out-of-bounds reads/crash for malformed TFT/TAD.
         */
        if ((int)tft->pf[i].content.length > (octet->len - size)) {
            ogs_error("TFT: pf[%d].content.length[%u] > remaining[%d] "
                    "(size[%d], IE[%d])", i, tft->pf[i].content.length,
                    octet->len - size, size, octet->len);
            return size;
        }

        j = 0; len = 0;
        while(len < tft->pf[i].content.length) {
            int comp_max = (int)(sizeof(tft->pf[i].content.component) /
                                 sizeof(tft->pf[i].content.component[0]));
            if (j >= comp_max) {
                ogs_error("TFT: pf[%d] too many components (j[%d] >= max[%d])",
                        i, j, comp_max);
                return size;
            }
            if (size + len + (int)sizeof(tft->pf[i].content.component[j].type) >
                octet->len) {
                ogs_error("TFT: size[%d]+len[%d]+pf[%d].component[%d]."
                        "type[%d] > IE Length[%d]",
                        size, len, i, j,
                        (int)sizeof(tft->pf[i].content.component[j].type),
                        octet->len);
                return size;
            }
            memcpy(&tft->pf[i].content.component[j].type,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].type));
            len += sizeof(tft->pf[i].content.component[j].type);
            switch(tft->pf[i].content.component[j].type) {
            case OGS_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE:
                if (size + len +
                    (int)sizeof(tft->pf[i].content.component[j].proto) >
                    octet->len) {
                    ogs_error("TFT: size[%d]+len[%d]+pf[%d].component[%d]."
                            "proto[%d] > IE Length[%d]", size, len, i, j,
                            (int)sizeof(tft->pf[i].content.component[j].proto),
                            octet->len);
                    return size;
                }
                memcpy(&tft->pf[i].content.component[j].proto,
                        (unsigned char *)octet->data+size+len,
                        sizeof(tft->pf[i].content.component[j].proto));
                len += sizeof(tft->pf[i].content.component[j].proto);
                break;
            case OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE:
            case OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE:
                if (size + len +
                    (int)sizeof(tft->pf[i].content.component[j].ipv4.addr) >
                    octet->len) {
                    ogs_error("TFT: size[%d]+len[%d]+pf[%d].component[%d]."
                            "ipv4.addr[%d] > IE Length[%d]",
                            size, len, i, j,
                            (int)sizeof(tft->pf[i].content.component[j].
                                ipv4.addr),
                            octet->len);
                    return size;
                }
                memcpy(&tft->pf[i].content.component[j].ipv4.addr,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv4.addr));
                len += sizeof(tft->pf[i].content.component[j].ipv4.addr);

                if (size + len +
                    (int)sizeof(tft->pf[i].content.component[j].ipv4.mask) >
                    octet->len) {
                    ogs_error("TFT: size[%d]+len[%d]+pf[%d].component[%d]."
                            "ipv4.mask[%d] > IE Length[%d]",
                            size, len, i, j,
                            (int)sizeof(tft->pf[i].content.component[j].
                                ipv4.mask),
                            octet->len);
                    return size;
                }
                memcpy(&tft->pf[i].content.component[j].ipv4.mask,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv4.mask));
                len += sizeof(tft->pf[i].content.component[j].ipv4.mask);
                break;
            case OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE:
            case OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE:
                if (size + len + (int)sizeof(tft->pf[i].content.component[j].
                        ipv6.addr) >
                    octet->len) {
                    ogs_error("TFT: size[%d]+len[%d]+pf[%d].component[%d]."
                            "ipv6.addr[%d] > IE Length[%d]",
                            size, len, i, j,
                            (int)sizeof(tft->pf[i].content.component[j].
                                ipv6.addr),
                            octet->len);
                    return size;
                }
                memcpy(&tft->pf[i].content.component[j].ipv6.addr,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv6.addr));
                len += sizeof(tft->pf[i].content.component[j].ipv6.addr);

                if (size + len + (int)sizeof(tft->pf[i].content.component[j].
                        ipv6.prefixlen) >
                    octet->len) {
                    ogs_error("TFT: size[%d]+len[%d]+pf[%d].component[%d]."
                            "ipv6.prefixlen[%d] > IE Length[%d]",
                            size, len, i, j,
                            (int)sizeof(tft->pf[i].content.component[j].
                                ipv6.prefixlen),
                            octet->len);
                    return size;
                }
                memcpy(&tft->pf[i].content.component[j].ipv6.prefixlen,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv6.prefixlen));
                len += sizeof(tft->pf[i].content.component[j].ipv6.prefixlen);
                break;
            case OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_TYPE:
            case OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_TYPE:
                if (size + len + (int)sizeof(tft->pf[i].content.component[j].
                        ipv6_mask.addr) >
                    octet->len) {
                    ogs_error("TFT: size[%d]+len[%d]+pf[%d].component[%d]."
                            "ipv6_mask.addr[%d] > IE Length[%d]",
                            size, len, i, j,
                            (int)sizeof(tft->pf[i].content.component[j].
                                ipv6_mask.addr),
                            octet->len);
                    return size;
                }
                memcpy(&tft->pf[i].content.component[j].ipv6_mask.addr,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv6_mask.addr));
                len += sizeof(tft->pf[i].content.component[j].ipv6_mask.addr);

                if (size + len + (int)sizeof(tft->pf[i].content.component[j].
                        ipv6_mask.mask) >
                    octet->len) {
                    ogs_error("TFT: size[%d]+len[%d]+pf[%d].component[%d]."
                            "ipv6_mask.mask[%d] > IE Length[%d]",
                            size, len, i, j,
                            (int)sizeof(tft->pf[i].content.component[j].
                                ipv6_mask.mask),
                            octet->len);
                    return size;
                }
                memcpy(&tft->pf[i].content.component[j].ipv6_mask.mask,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv6_mask.mask));
                len += sizeof(tft->pf[i].content.component[j].ipv6_mask.mask);
                break;
            case OGS_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE:
            case OGS_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE:
                if (size + len + (int)sizeof(tft->pf[i].content.component[j].
                        port.low) >
                    octet->len) {
                    ogs_error("TFT: size[%d]+len[%d]+pf[%d].component[%d]."
                            "port.low[%d] > IE Length[%d]",
                            size, len, i, j,
                            (int)sizeof(tft->pf[i].content.component[j].
                                port.low),
                            octet->len);
                    return size;
                }
                memcpy(&tft->pf[i].content.component[j].port.low,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].port.low));
                tft->pf[i].content.component[j].port.low =
                    be16toh(tft->pf[i].content.component[j].port.low);
                len += sizeof(tft->pf[i].content.component[j].port.low);
                break;
            case OGS_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE:
            case OGS_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE:
                if (size + len + (int)sizeof(tft->pf[i].content.component[j].
                        port.low) >
                    octet->len) {
                    ogs_error("TFT: size[%d]+len[%d]+pf[%d].component[%d]."
                            "port.low[%d] > IE Length[%d]",
                            size, len, i, j,
                            (int)sizeof(tft->pf[i].content.component[j].
                                port.low),
                            octet->len);
                    return size;
                }
                memcpy(&tft->pf[i].content.component[j].port.low,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].port.low));
                tft->pf[i].content.component[j].port.low =
                    be16toh(tft->pf[i].content.component[j].port.low);
                len += sizeof(tft->pf[i].content.component[j].port.low);

                if (size + len + (int)sizeof(tft->pf[i].content.component[j].
                        port.high) >
                    octet->len) {
                    ogs_error("TFT: size[%d]+len[%d]+pf[%d].component[%d]."
                            "port.high[%d] > IE Length[%d]",
                            size, len, i, j,
                            (int)sizeof(tft->pf[i].content.component[j].
                                port.high),
                            octet->len);
                    return size;
                }
                memcpy(&tft->pf[i].content.component[j].port.high,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].port.high));
                tft->pf[i].content.component[j].port.high =
                    be16toh(tft->pf[i].content.component[j].port.high);
                len += sizeof(tft->pf[i].content.component[j].port.high);
                break;
            default:
                ogs_error("Unknown Packet Filter Type(%d)",
                        tft->pf[i].content.component[j].type);
                return -1;
            }
            j++;
        }
        tft->pf[i].content.num_of_component = j;
        size += len;
    }

    if (size != octet->len)
        ogs_error("Mismatch IE Length[%d] != Decoded[%d]", octet->len, size);

    return size;
}
int16_t ogs_gtp2_build_tft(
    ogs_tlv_octet_t *octet, ogs_gtp2_tft_t *tft, void *data, int data_len)
{
    ogs_gtp2_tft_t target;
    uint16_t size = 0;
    int i, j;

    ogs_assert(tft);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len >= OGS_GTP2_MAX_TRAFFIC_FLOW_TEMPLATE);

    ogs_assert(tft->code != OGS_GTP2_TFT_CODE_IGNORE_THIS_IE);

    octet->data = data;
    memcpy(&target, tft, sizeof(ogs_gtp2_tft_t));

    ogs_assert(size + sizeof(target.flags) <= data_len);
    memcpy((unsigned char *)octet->data + size, &target.flags,
            sizeof(target.flags));
    size += sizeof(target.flags);

    if (tft->code == OGS_GTP2_TFT_CODE_NO_TFT_OPERATION ||
        tft->code == OGS_GTP2_TFT_CODE_DELETE_EXISTING_TFT)
        return size;

    for (i = 0; i < target.num_of_packet_filter &&
                i < OGS_MAX_NUM_OF_FLOW_IN_GTP; i++) {
        ogs_assert(size + sizeof(target.pf[i].flags) <= data_len);
        memcpy((unsigned char *)octet->data + size, &target.pf[i].flags,
                sizeof(target.pf[i].flags));
        size += sizeof(target.pf[i].flags);

        if (tft->code == OGS_GTP2_TFT_CODE_DELETE_PACKET_FILTERS_FROM_EXISTING)
            continue;

        ogs_assert(size + sizeof(target.pf[i].precedence) <= data_len);
        memcpy((unsigned char *)octet->data + size, &target.pf[i].precedence,
                sizeof(target.pf[i].precedence));
        size += sizeof(target.pf[i].precedence);

        ogs_assert(size + sizeof(target.pf[i].content.length) <= data_len);
        memcpy((unsigned char *)octet->data + size,
                &target.pf[i].content.length,
                sizeof(target.pf[i].content.length));
        size += sizeof(target.pf[i].content.length);

        for (j = 0; j < target.pf[i].content.num_of_component; j++) {
            ogs_assert(size +
                sizeof(target.pf[i].content.component[j].type) <= data_len);
            memcpy((unsigned char *)octet->data + size,
                    &target.pf[i].content.component[j].type,
                    sizeof(target.pf[i].content.component[j].type));
            size += sizeof(target.pf[i].content.component[j].type);
            switch(target.pf[i].content.component[j].type) {
            case OGS_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE:
                ogs_assert(size + sizeof(
                        target.pf[i].content.component[j].proto) <= data_len);
                memcpy((unsigned char *)octet->data + size,
                        &target.pf[i].content.component[j].proto,
                        sizeof(target.pf[i].content.component[j].proto));
                size += sizeof(target.pf[i].content.component[j].proto);
                break;
            case OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE:
            case OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE:
                ogs_assert(size +
                    sizeof(target.pf[i].content.component[j].ipv4.addr)
                        <= data_len);
                memcpy((unsigned char *)octet->data + size,
                    &target.pf[i].content.component[j].ipv4.addr,
                    sizeof(target.pf[i].content.component[j].ipv4.addr));
                size += sizeof(target.pf[i].content.component[j].ipv4.addr);

                ogs_assert(size +
                    sizeof(target.pf[i].content.component[j].ipv4.mask)
                        <= data_len);
                memcpy((unsigned char *)octet->data + size,
                        &target.pf[i].content.component[j].ipv4.mask,
                        sizeof(target.pf[i].content.component[j].ipv4.mask));
                size += sizeof(target.pf[i].content.component[j].ipv4.mask);
                break;
            case OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE:
            case OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE:
                ogs_assert(size +
                    sizeof(target.pf[i].content.component[j].ipv6.addr)
                        <= data_len);
                memcpy((unsigned char *)octet->data + size,
                        &target.pf[i].content.component[j].ipv6.addr,
                        sizeof(target.pf[i].content.component[j].ipv6.addr));
                size += sizeof(target.pf[i].content.component[j].ipv6.addr);

                ogs_assert(size +
                    sizeof(target.pf[i].content.component[j].ipv6.prefixlen)
                        <= data_len);
                memcpy((unsigned char *)octet->data + size,
                    &target.pf[i].content.component[j].ipv6.prefixlen,
                    sizeof(target.pf[i].content.component[j].ipv6.prefixlen));
                size += sizeof(
                        target.pf[i].content.component[j].ipv6.prefixlen);
                break;
            case OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_TYPE:
            case OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_TYPE:
                ogs_assert(size +
                    sizeof(target.pf[i].content.component[j].ipv6_mask.addr)
                        <= data_len);
                memcpy((unsigned char *)octet->data + size,
                        &target.pf[i].content.component[j].ipv6_mask.addr,
                        sizeof(
                        target.pf[i].content.component[j].ipv6_mask.addr));
                size += sizeof(
                        target.pf[i].content.component[j].ipv6_mask.addr);

                ogs_assert(size +
                    sizeof(target.pf[i].content.component[j].ipv6_mask.mask)
                        <= data_len);
                memcpy((unsigned char *)octet->data + size,
                        &target.pf[i].content.component[j].ipv6_mask.mask,
                        sizeof(
                        target.pf[i].content.component[j].ipv6_mask.mask));
                size += sizeof(
                        target.pf[i].content.component[j].ipv6_mask.mask);
                break;
            case OGS_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE:
            case OGS_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE:
                ogs_assert(size +
                    sizeof(target.pf[i].content.component[j].port.low)
                        <= data_len);
                target.pf[i].content.component[j].port.low =
                    htobe16(target.pf[i].content.component[j].port.low);
                memcpy((unsigned char *)octet->data + size,
                    &target.pf[i].content.component[j].port.low,
                    sizeof(target.pf[i].content.component[j].port.low));
                size += sizeof(target.pf[i].content.component[j].port.low);
                break;
            case OGS_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE:
            case OGS_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE:
                ogs_assert(size +
                    sizeof(target.pf[i].content.component[j].port.low)
                        <= data_len);
                target.pf[i].content.component[j].port.low =
                    htobe16(target.pf[i].content.component[j].port.low);
                memcpy((unsigned char *)octet->data + size,
                        &target.pf[i].content.component[j].port.low,
                        sizeof(target.pf[i].content.component[j].port.low));
                size += sizeof(target.pf[i].content.component[j].port.low);

                ogs_assert(size +
                    sizeof(target.pf[i].content.component[j].port.high)
                        <= data_len);
                target.pf[i].content.component[j].port.high =
                    htobe16(target.pf[i].content.component[j].port.high);
                memcpy((unsigned char *)octet->data + size,
                    &target.pf[i].content.component[j].port.high,
                    sizeof(target.pf[i].content.component[j].port.high));
                size += sizeof(target.pf[i].content.component[j].port.high);
                break;
            default:
                ogs_error("Unknown Packet Filter Type(%d)",
                        target.pf[i].content.component[j].type);
                return -1;
            }
        }
    }

    octet->len = size;

    return octet->len;
}


/* 8.21 User Location Information (ULI) */
int16_t ogs_gtp2_parse_uli(ogs_gtp2_uli_t *uli, ogs_tlv_octet_t *octet)
{
    ogs_gtp2_uli_t *source = NULL;
    int16_t size = 0;

    ogs_assert(uli);
    ogs_assert(octet);

    source = (ogs_gtp2_uli_t *)octet->data;

    memset(uli, 0, sizeof(ogs_gtp2_uli_t));

    if (octet->len < 1) {
        ogs_error("ULI IE too short [%u]", octet->len);
        return 0;
    }

    uli->flags = source->flags;
    size++;

    if (uli->flags.cgi) {
        if (size + sizeof(uli->cgi) > octet->len) {
            ogs_error("size[%d]+sizeof(uli->cgi)[%d] > IE Length[%d]",
                    size, (int)sizeof(uli->cgi), octet->len);
            return 0;
        }
        memcpy(&uli->cgi,
                (unsigned char *)octet->data + size, sizeof(uli->cgi));
        uli->cgi.lac = be16toh(uli->cgi.lac);
        uli->cgi.ci = be16toh(uli->cgi.ci);
        size += sizeof(uli->cgi);
    }
    if (uli->flags.sai) {
        if (size + sizeof(uli->sai) > octet->len) {
            ogs_error("size[%d]+sizeof(uli->sai)[%d] > IE Length[%d]",
                    size, (int)sizeof(uli->sai), octet->len);
            return 0;
        }
        memcpy(&uli->sai,
                (unsigned char *)octet->data + size, sizeof(uli->sai));
        uli->sai.lac = be16toh(uli->sai.lac);
        uli->sai.sac = be16toh(uli->sai.sac);
        size += sizeof(uli->sai);
    }
    if (uli->flags.rai) {
        if (size + sizeof(uli->rai) > octet->len) {
            ogs_error("size[%d]+sizeof(uli->lai)[%d] > IE Length[%d]",
                    size, (int)sizeof(uli->lai), octet->len);
            return 0;
        }
        memcpy(&uli->rai,
                (unsigned char *)octet->data + size, sizeof(uli->rai));
        uli->rai.lac = be16toh(uli->rai.lac);
        uli->rai.rac = be16toh(uli->rai.rac);
        size += sizeof(uli->rai);
    }
    if (uli->flags.tai) {
        if (size + sizeof(uli->tai) > octet->len) {
            ogs_error("size[%d]+sizeof(uli->tai)[%d] > IE Length[%d]",
                    size, (int)sizeof(uli->tai), octet->len);
            return 0;
        }
        memcpy(&uli->tai,
                (unsigned char *)octet->data + size, sizeof(uli->tai));
        uli->tai.tac = be16toh(uli->tai.tac);
        size += sizeof(uli->tai);
    }
    if (uli->flags.e_cgi) {
        if (size + sizeof(uli->e_cgi) > octet->len) {
            ogs_error("size[%d]+sizeof(uli->e_cgi)[%d] > IE Length[%d]",
                    size, (int)sizeof(uli->e_cgi), octet->len);
            return 0;
        }
        memcpy(&uli->e_cgi,
                (unsigned char *)octet->data + size, sizeof(uli->e_cgi));
        uli->e_cgi.cell_id = be32toh(uli->e_cgi.cell_id);
        size += sizeof(uli->e_cgi);
    }
    if (uli->flags.lai) {
        if (size + sizeof(uli->lai) > octet->len) {
            ogs_error("size[%d]+sizeof(uli->lai)[%d] > IE Length[%d]",
                    size, (int)sizeof(uli->lai), octet->len);
            return 0;
        }
        memcpy(&uli->lai,
                (unsigned char *)octet->data + size, sizeof(uli->lai));
        uli->lai.lac = be16toh(uli->lai.lac);
        size += sizeof(uli->lai);
    }
    if (uli->flags.enodeb_id) {
        if (size + sizeof(uli->enodeb_id) > octet->len) {
            ogs_error("size[%d]+sizeof(uli->enodeb_id)[%d] > IE Length[%d]",
                    size, (int)sizeof(uli->enodeb_id), octet->len);
            return 0;
        }
        memcpy(&uli->enodeb_id,
                (unsigned char *)octet->data + size, sizeof(uli->enodeb_id));
        uli->enodeb_id.enodeb_id = be16toh(uli->enodeb_id.enodeb_id);
        size += sizeof(uli->enodeb_id);
    }
    if (uli->flags.ext_enodeb_id) {  /* TODO */
        ogs_error("Extended Macro eNodeB ID in ULI not implemented! see 3GPP TS 29.274 8.21.8");
    }

    if (size != octet->len)
        ogs_error("Mismatch IE Length[%d] != Decoded[%d]", octet->len, size);

    return size;
}
int16_t ogs_gtp2_build_uli(
        ogs_tlv_octet_t *octet, ogs_gtp2_uli_t *uli, void *data, int data_len)
{
    ogs_gtp2_uli_t target;
    int16_t size = 0;

    ogs_assert(uli);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len);

    octet->data = data;
    memcpy(&target, uli, sizeof(ogs_gtp2_uli_t));

    ogs_assert(size + sizeof(target.flags) <= data_len);
    memcpy((unsigned char *)octet->data + size,
            &target.flags, sizeof(target.flags));
    size += sizeof(target.flags);

    if (target.flags.cgi) {
        ogs_assert(size + sizeof(target.cgi) <= data_len);
        target.cgi.lac = htobe16(target.cgi.lac);
        target.cgi.ci = htobe16(target.cgi.ci);
        memcpy((unsigned char *)octet->data + size,
                &target.cgi, sizeof(target.cgi));
        size += sizeof(target.cgi);
    }
    if (target.flags.sai) {
        ogs_assert(size + sizeof(target.sai) <= data_len);
        target.sai.lac = htobe16(target.sai.lac);
        target.sai.sac = htobe16(target.sai.sac);
        memcpy((unsigned char *)octet->data + size,
                &target.sai, sizeof(target.sai));
        size += sizeof(target.sai);
    }
    if (target.flags.rai) {
        ogs_assert(size + sizeof(target.rai) <= data_len);
        target.rai.lac = htobe16(target.rai.lac);
        target.rai.rac = htobe16(target.rai.rac);
        memcpy((unsigned char *)octet->data + size,
                &target.rai, sizeof(target.rai));
        size += sizeof(target.rai);
    }
    if (target.flags.tai) {
        ogs_assert(size + sizeof(target.tai) <= data_len);
        target.tai.tac = htobe16(target.tai.tac);
        memcpy((unsigned char *)octet->data + size,
                &target.tai, sizeof(target.tai));
        size += sizeof(target.tai);
    }
    if (target.flags.e_cgi) {
        ogs_assert(size + sizeof(target.e_cgi) <= data_len);
        target.e_cgi.cell_id = htobe32(target.e_cgi.cell_id);
        memcpy((unsigned char *)octet->data + size,
                &target.e_cgi, sizeof(target.e_cgi));
        size += sizeof(target.e_cgi);
    }
    if (target.flags.lai) {
        ogs_assert(size + sizeof(target.lai) <= data_len);
        target.lai.lac = htobe16(target.lai.lac);
        memcpy((unsigned char *)octet->data + size,
                &target.lai, sizeof(target.lai));
        size += sizeof(target.lai);
    }
    if (target.flags.enodeb_id) {
        ogs_assert(size + sizeof(target.enodeb_id) <= data_len);
        target.enodeb_id.enodeb_id = htobe16(target.enodeb_id.enodeb_id);
        memcpy((unsigned char *)octet->data + size,
                &target.enodeb_id, sizeof(target.enodeb_id));
        size += sizeof(target.enodeb_id);
    }
    if (uli->flags.ext_enodeb_id) { /* TODO */
        ogs_error("Extended Macro eNodeB ID in ULI not implemented! see 3GPP TS 29.274 8.21.8");
    }

    octet->len = size;

    return octet->len;
}

int16_t ogs_gtp2_parse_node_identifier(
    ogs_gtp2_node_identifier_t *node_identifier, ogs_tlv_octet_t *octet)
{
    int16_t size = 0;

    ogs_assert(node_identifier);
    ogs_assert(octet);

    memset(node_identifier, 0, sizeof(ogs_gtp2_node_identifier_t));

    if (size + sizeof(node_identifier->name_len) > octet->len) {
        ogs_error("Invalid TLV length [%d != %d]", size, octet->len);
        ogs_log_hexdump(OGS_LOG_ERROR, octet->data, octet->len);
        return size;
    }
    memcpy(&node_identifier->name_len,
            (unsigned char *)octet->data + size,
            sizeof(node_identifier->name_len));
    size += sizeof(node_identifier->name_len);

    if (size + node_identifier->name_len > octet->len) {
        ogs_error("Invalid TLV length [%d != %d]", size, octet->len);
        ogs_log_hexdump(OGS_LOG_ERROR, octet->data, octet->len);
        return size;
    }
    node_identifier->name = (char *)octet->data + size;
    size += node_identifier->name_len;

    if (size + sizeof(node_identifier->realm_len) > octet->len) {
        ogs_error("Invalid TLV length [%d != %d]", size, octet->len);
        ogs_log_hexdump(OGS_LOG_ERROR, octet->data, octet->len);
        return size;
    }
    memcpy(&node_identifier->realm_len,
            (unsigned char *)octet->data + size,
            sizeof(node_identifier->realm_len));
    size += sizeof(node_identifier->realm_len);

    if (size + node_identifier->realm_len > octet->len) {
        ogs_error("Invalid TLV length [%d != %d]", size, octet->len);
        ogs_log_hexdump(OGS_LOG_ERROR, octet->data, octet->len);
        return size;
    }
    node_identifier->realm = (char *)octet->data + size;
    size += node_identifier->realm_len;

    if (size != octet->len) {
        ogs_error("Invalid TLV length [%d != %d]", size, octet->len);
        ogs_log_hexdump(OGS_LOG_ERROR, octet->data, octet->len);
    }

    return size;
}
int16_t ogs_gtp2_build_node_identifier(ogs_tlv_octet_t *octet,
    ogs_gtp2_node_identifier_t *node_identifier, void *data, int data_len)
{
    int16_t size = 0;

    ogs_assert(node_identifier);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len);

    octet->data = data;

    ogs_assert(size + sizeof(node_identifier->name_len) <= data_len);
    memcpy((unsigned char *)octet->data + size,
            &node_identifier->name_len,
            sizeof(node_identifier->name_len));
    size += sizeof(node_identifier->name_len);

    ogs_assert(size + node_identifier->name_len <= data_len);
    memcpy((unsigned char *)octet->data + size,
            node_identifier->name, node_identifier->name_len);
    size += node_identifier->name_len;

    ogs_assert(size + sizeof(node_identifier->realm_len) <= data_len);
    memcpy((unsigned char *)octet->data + size,
            &node_identifier->realm_len,
            sizeof(node_identifier->realm_len));
    size += sizeof(node_identifier->realm_len);

    ogs_assert(size + node_identifier->realm_len <= data_len);
    memcpy((unsigned char *)octet->data + size,
            node_identifier->realm, node_identifier->realm_len);
    size += node_identifier->realm_len;

    octet->len = size;

    return octet->len;
}

/*
 * Bounded reader and writer used by the variable-length IEs below
 * (S10 mobility management IEs, TS 29.274 clauses 8.38 to 8.51).
 *
 * Parse functions return the decoded size, or 0 on error.
 * Build functions return the encoded size, or 0 on error.
 */
typedef struct ie_reader_s {
    uint8_t *data;
    int len;
    int pos;
} ie_reader_t;

static bool ie_read_end(ie_reader_t *r)
{
    return r->pos >= r->len;
}

static bool ie_read_bytes(ie_reader_t *r, void *dst, int n)
{
    if (n < 0 || r->pos + n > r->len)
        return false;
    if (n)
        memcpy(dst, r->data + r->pos, n);
    r->pos += n;
    return true;
}

static bool ie_read_ptr(ie_reader_t *r, uint8_t **dst, int n)
{
    if (n < 0 || r->pos + n > r->len)
        return false;
    *dst = n ? r->data + r->pos : NULL;
    r->pos += n;
    return true;
}

static bool ie_read_u8(ie_reader_t *r, uint8_t *v)
{
    return ie_read_bytes(r, v, 1);
}

static bool ie_read_u16(ie_reader_t *r, uint16_t *v)
{
    uint8_t b[2];
    if (!ie_read_bytes(r, b, 2))
        return false;
    *v = (b[0] << 8) | b[1];
    return true;
}

static bool ie_read_u24(ie_reader_t *r, uint32_t *v)
{
    uint8_t b[3];
    if (!ie_read_bytes(r, b, 3))
        return false;
    *v = (b[0] << 16) | (b[1] << 8) | b[2];
    return true;
}

static bool ie_read_u32(ie_reader_t *r, uint32_t *v)
{
    uint8_t b[4];
    if (!ie_read_bytes(r, b, 4))
        return false;
    *v = ((uint32_t)b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
    return true;
}

/* Read a one-octet length followed by at most 'max' octets */
static bool ie_read_lv(ie_reader_t *r, uint8_t *len, uint8_t *dst, int max)
{
    if (!ie_read_u8(r, len))
        return false;
    if (*len > max)
        return false;
    return ie_read_bytes(r, dst, *len);
}

typedef struct ie_writer_s {
    uint8_t *data;
    int len;
    int pos;
} ie_writer_t;

static bool ie_write_bytes(ie_writer_t *w, const void *src, int n)
{
    if (n < 0 || w->pos + n > w->len)
        return false;
    if (n) {
        ogs_assert(src);
        memcpy(w->data + w->pos, src, n);
    }
    w->pos += n;
    return true;
}

static bool ie_write_u8(ie_writer_t *w, uint8_t v)
{
    return ie_write_bytes(w, &v, 1);
}

static bool ie_write_u16(ie_writer_t *w, uint16_t v)
{
    uint8_t b[2] = { v >> 8, v };
    return ie_write_bytes(w, b, 2);
}

static bool ie_write_u24(ie_writer_t *w, uint32_t v)
{
    uint8_t b[3] = { v >> 16, v >> 8, v };
    return ie_write_bytes(w, b, 3);
}

static bool ie_write_u32(ie_writer_t *w, uint32_t v)
{
    uint8_t b[4] = { v >> 24, v >> 16, v >> 8, v };
    return ie_write_bytes(w, b, 4);
}

static bool ie_write_lv(ie_writer_t *w, uint8_t len, const void *src, int max)
{
    if (len > max)
        return false;
    return ie_write_u8(w, len) && ie_write_bytes(w, src, len);
}

/* 8.38 MM Context, Figure 8.38-5: EPS Security Context and Quadruplets */
int16_t ogs_gtp2_parse_mm_context(
    ogs_gtp2_mm_context_t *mm_context, ogs_tlv_octet_t *octet)
{
    ie_reader_t r;
    uint8_t o5, o6, o7, s, len;
    bool uambri, sambri, osci;
    int i;

    ogs_assert(mm_context);
    ogs_assert(octet);

    memset(mm_context, 0, sizeof(*mm_context));
    r.data = octet->data;
    r.len = octet->len;
    r.pos = 0;

    if (!ie_read_u8(&r, &o5) || !ie_read_u8(&r, &o6) || !ie_read_u8(&r, &o7))
        goto truncated;

    mm_context->security_mode = o5 >> 5;
    if (mm_context->security_mode !=
            OGS_GTP2_MM_CONTEXT_SECURITY_MODE_EPS_SECURITY_CONTEXT_AND_QUADRUPLETS) {
        ogs_error("Unsupported MM Context security mode [%d]",
                mm_context->security_mode);
        return 0;
    }
    mm_context->nh_presence = (o5 >> 4) & 1;
    mm_context->drx_parameter_presence = (o5 >> 3) & 1;
    mm_context->ksi_asme = o5 & 0x07;

    mm_context->num_of_quintuplets = o6 >> 5;
    mm_context->num_of_quadruplets = (o6 >> 2) & 0x07;
    uambri = (o6 >> 1) & 1;
    osci = o6 & 1;

    sambri = o7 >> 7;
    mm_context->nas_integrity_algorithm = (o7 >> 4) & 0x07;
    mm_context->nas_cipher_algorithm = o7 & 0x0f;

    if (mm_context->num_of_quadruplets > OGS_GTP2_MAX_AUTH_VECTORS ||
        mm_context->num_of_quintuplets > OGS_GTP2_MAX_AUTH_VECTORS) {
        ogs_error("Too many authentication vectors [quadruplets:%d "
                "quintuplets:%d]", mm_context->num_of_quadruplets,
                mm_context->num_of_quintuplets);
        return 0;
    }

    if (!ie_read_u24(&r, &mm_context->nas_downlink_count) ||
        !ie_read_u24(&r, &mm_context->nas_uplink_count) ||
        !ie_read_bytes(&r, mm_context->kasme, OGS_GTP2_KASME_LEN))
        goto truncated;

    for (i = 0; i < mm_context->num_of_quadruplets; i++) {
        ogs_gtp2_auth_quadruplet_t *v = &mm_context->quadruplet[i];
        if (!ie_read_bytes(&r, v->rand, OGS_GTP2_RAND_LEN) ||
            !ie_read_lv(&r, &v->xres_len, v->xres, OGS_GTP2_MAX_XRES_LEN) ||
            !ie_read_lv(&r, &v->autn_len, v->autn, OGS_GTP2_AUTN_LEN) ||
            !ie_read_bytes(&r, v->kasme, OGS_GTP2_KASME_LEN))
            goto truncated;
    }
    for (i = 0; i < mm_context->num_of_quintuplets; i++) {
        ogs_gtp2_auth_quintuplet_t *v = &mm_context->quintuplet[i];
        if (!ie_read_bytes(&r, v->rand, OGS_GTP2_RAND_LEN) ||
            !ie_read_lv(&r, &v->xres_len, v->xres, OGS_GTP2_MAX_XRES_LEN) ||
            !ie_read_bytes(&r, v->ck, OGS_GTP2_CK_LEN) ||
            !ie_read_bytes(&r, v->ik, OGS_GTP2_IK_LEN) ||
            !ie_read_lv(&r, &v->autn_len, v->autn, OGS_GTP2_AUTN_LEN))
            goto truncated;
    }

    if (mm_context->drx_parameter_presence) {
        if (!ie_read_bytes(&r, mm_context->drx_parameter, 2))
            goto truncated;
    }
    if (mm_context->nh_presence) {
        if (!ie_read_bytes(&r, mm_context->nh, OGS_GTP2_NH_LEN) ||
            !ie_read_u8(&r, &mm_context->ncc))
            goto truncated;
        mm_context->ncc &= 0x07;
    }
    if (sambri) {
        uint32_t ul, dl;
        mm_context->subscribed_ue_ambr_presence = true;
        if (!ie_read_u32(&r, &ul) || !ie_read_u32(&r, &dl))
            goto truncated;
        mm_context->subscribed_ue_ambr.uplink = ul;
        mm_context->subscribed_ue_ambr.downlink = dl;
    }
    if (uambri) {
        uint32_t ul, dl;
        mm_context->used_ue_ambr_presence = true;
        if (!ie_read_u32(&r, &ul) || !ie_read_u32(&r, &dl))
            goto truncated;
        mm_context->used_ue_ambr.uplink = ul;
        mm_context->used_ue_ambr.downlink = dl;
    }

    if (!ie_read_lv(&r, &mm_context->ue_network_capability_len,
                mm_context->ue_network_capability,
                OGS_GTP2_MAX_UE_NETWORK_CAPABILITY_LEN) ||
        !ie_read_lv(&r, &mm_context->ms_network_capability_len,
                mm_context->ms_network_capability,
                OGS_GTP2_MAX_MS_NETWORK_CAPABILITY_LEN) ||
        !ie_read_lv(&r, &mm_context->mei_len,
                mm_context->mei, OGS_GTP2_MAX_MEI_LEN))
        goto truncated;

    /* The fields below may be absent when sent by an older release */
    if (ie_read_end(&r)) {
        if (osci) {
            ogs_error("OSCI is set but no Old EPS Security Context");
            return 0;
        }
        goto done;
    }
    if (!ie_read_u8(&r, &mm_context->access_restriction_data.octet))
        goto truncated;
    mm_context->access_restriction_data_presence = true;

    if (osci) {
        mm_context->old_security_context_presence = true;
        if (!ie_read_u8(&r, &s))
            goto truncated;
        mm_context->old_nh_presence = s >> 7;
        mm_context->rlos = (s >> 6) & 1;
        mm_context->old_ksi_asme = (s >> 3) & 0x07;
        mm_context->old_ncc = s & 0x07;
        if (!ie_read_bytes(&r, mm_context->old_kasme, OGS_GTP2_KASME_LEN))
            goto truncated;
        if (mm_context->old_nh_presence) {
            if (!ie_read_bytes(&r, mm_context->old_nh, OGS_GTP2_NH_LEN))
                goto truncated;
        }
    }

    if (ie_read_end(&r)) goto done;
    if (!ie_read_lv(&r, &mm_context->voice_domain_preference_len,
                mm_context->voice_domain_preference,
                OGS_GTP2_MAX_VOICE_DOMAIN_PREFERENCE_LEN))
        goto truncated;

    if (ie_read_end(&r)) goto done;
    if (!ie_read_u16(&r, &mm_context->ue_radio_capability_for_paging_len) ||
        !ie_read_ptr(&r, &mm_context->ue_radio_capability_for_paging,
                mm_context->ue_radio_capability_for_paging_len))
        goto truncated;

    if (ie_read_end(&r)) goto done;
    if (!ie_read_u8(&r, &mm_context->extended_access_restriction_data_len))
        goto truncated;
    if (mm_context->extended_access_restriction_data_len) {
        uint8_t *skip = NULL;
        if (!ie_read_u8(&r, &mm_context->extended_access_restriction_data) ||
            !ie_read_ptr(&r, &skip,
                mm_context->extended_access_restriction_data_len - 1))
            goto truncated;
    }

    if (ie_read_end(&r)) goto done;
    if (!ie_read_lv(&r, &mm_context->ue_additional_security_capability_len,
                mm_context->ue_additional_security_capability,
                OGS_GTP2_MAX_UE_ADDITIONAL_SECURITY_CAPABILITY_LEN))
        goto truncated;

    if (ie_read_end(&r)) goto done;
    if (!ie_read_lv(&r, &mm_context->ue_nr_security_capability_len,
                mm_context->ue_nr_security_capability,
                OGS_GTP2_MAX_UE_NR_SECURITY_CAPABILITY_LEN))
        goto truncated;

    if (ie_read_end(&r)) goto done;
    if (!ie_read_u16(&r, &mm_context->apn_rate_control_statuses_len) ||
        !ie_read_ptr(&r, &mm_context->apn_rate_control_statuses,
                mm_context->apn_rate_control_statuses_len))
        goto truncated;

    if (ie_read_end(&r)) goto done;
    if (!ie_read_lv(&r, &mm_context->core_network_restrictions_len,
                mm_context->core_network_restrictions,
                OGS_GTP2_CORE_NETWORK_RESTRICTIONS_LEN))
        goto truncated;

    if (ie_read_end(&r)) goto done;
    if (!ie_read_u8(&r, &len) ||
        !ie_read_ptr(&r, &mm_context->ue_radio_capability_id, len))
        goto truncated;
    mm_context->ue_radio_capability_id_len = len;

    if (ie_read_end(&r)) goto done;
    if (!ie_read_u8(&r, &len))
        goto truncated;
    mm_context->octet_a_presence = true;
    mm_context->tridi = (len >> 2) & 1;
    mm_context->ensct = len & 0x03;

    /* Octets after 'a' are present only if explicitly specified: ignored */

done:
    return octet->len;

truncated:
    ogs_error("Invalid MM Context [len:%d pos:%d]", r.len, r.pos);
    ogs_log_hexdump(OGS_LOG_ERROR, octet->data, octet->len);
    return 0;
}

int16_t ogs_gtp2_build_mm_context(ogs_tlv_octet_t *octet,
    ogs_gtp2_mm_context_t *mm_context, void *data, int data_len)
{
    ie_writer_t w;
    int i;

    ogs_assert(mm_context);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len);

    w.data = data;
    w.len = data_len;
    w.pos = 0;

    if (mm_context->num_of_quadruplets > OGS_GTP2_MAX_AUTH_VECTORS ||
        mm_context->num_of_quintuplets > OGS_GTP2_MAX_AUTH_VECTORS) {
        ogs_error("Too many authentication vectors");
        return 0;
    }

    if (!ie_write_u8(&w,
            (OGS_GTP2_MM_CONTEXT_SECURITY_MODE_EPS_SECURITY_CONTEXT_AND_QUADRUPLETS << 5) |
            ((mm_context->nh_presence ? 1 : 0) << 4) |
            ((mm_context->drx_parameter_presence ? 1 : 0) << 3) |
            (mm_context->ksi_asme & 0x07)) ||
        !ie_write_u8(&w,
            (mm_context->num_of_quintuplets << 5) |
            (mm_context->num_of_quadruplets << 2) |
            ((mm_context->used_ue_ambr_presence ? 1 : 0) << 1) |
            (mm_context->old_security_context_presence ? 1 : 0)) ||
        !ie_write_u8(&w,
            ((mm_context->subscribed_ue_ambr_presence ? 1 : 0) << 7) |
            ((mm_context->nas_integrity_algorithm & 0x07) << 4) |
            (mm_context->nas_cipher_algorithm & 0x0f)) ||
        !ie_write_u24(&w, mm_context->nas_downlink_count & 0xffffff) ||
        !ie_write_u24(&w, mm_context->nas_uplink_count & 0xffffff) ||
        !ie_write_bytes(&w, mm_context->kasme, OGS_GTP2_KASME_LEN))
        goto overflow;

    for (i = 0; i < mm_context->num_of_quadruplets; i++) {
        ogs_gtp2_auth_quadruplet_t *v = &mm_context->quadruplet[i];
        if (!ie_write_bytes(&w, v->rand, OGS_GTP2_RAND_LEN) ||
            !ie_write_lv(&w, v->xres_len, v->xres, OGS_GTP2_MAX_XRES_LEN) ||
            !ie_write_lv(&w, v->autn_len, v->autn, OGS_GTP2_AUTN_LEN) ||
            !ie_write_bytes(&w, v->kasme, OGS_GTP2_KASME_LEN))
            goto overflow;
    }
    for (i = 0; i < mm_context->num_of_quintuplets; i++) {
        ogs_gtp2_auth_quintuplet_t *v = &mm_context->quintuplet[i];
        if (!ie_write_bytes(&w, v->rand, OGS_GTP2_RAND_LEN) ||
            !ie_write_lv(&w, v->xres_len, v->xres, OGS_GTP2_MAX_XRES_LEN) ||
            !ie_write_bytes(&w, v->ck, OGS_GTP2_CK_LEN) ||
            !ie_write_bytes(&w, v->ik, OGS_GTP2_IK_LEN) ||
            !ie_write_lv(&w, v->autn_len, v->autn, OGS_GTP2_AUTN_LEN))
            goto overflow;
    }

    if (mm_context->drx_parameter_presence) {
        if (!ie_write_bytes(&w, mm_context->drx_parameter, 2))
            goto overflow;
    }
    if (mm_context->nh_presence) {
        if (!ie_write_bytes(&w, mm_context->nh, OGS_GTP2_NH_LEN) ||
            !ie_write_u8(&w, mm_context->ncc & 0x07))
            goto overflow;
    }
    if (mm_context->subscribed_ue_ambr_presence) {
        if (!ie_write_u32(&w, mm_context->subscribed_ue_ambr.uplink) ||
            !ie_write_u32(&w, mm_context->subscribed_ue_ambr.downlink))
            goto overflow;
    }
    if (mm_context->used_ue_ambr_presence) {
        if (!ie_write_u32(&w, mm_context->used_ue_ambr.uplink) ||
            !ie_write_u32(&w, mm_context->used_ue_ambr.downlink))
            goto overflow;
    }

    if (!ie_write_lv(&w, mm_context->ue_network_capability_len,
                mm_context->ue_network_capability,
                OGS_GTP2_MAX_UE_NETWORK_CAPABILITY_LEN) ||
        !ie_write_lv(&w, mm_context->ms_network_capability_len,
                mm_context->ms_network_capability,
                OGS_GTP2_MAX_MS_NETWORK_CAPABILITY_LEN) ||
        !ie_write_lv(&w, mm_context->mei_len,
                mm_context->mei, OGS_GTP2_MAX_MEI_LEN) ||
        !ie_write_u8(&w, mm_context->access_restriction_data.octet))
        goto overflow;

    if (mm_context->old_security_context_presence) {
        if (!ie_write_u8(&w,
                ((mm_context->old_nh_presence ? 1 : 0) << 7) |
                ((mm_context->rlos ? 1 : 0) << 6) |
                ((mm_context->old_ksi_asme & 0x07) << 3) |
                (mm_context->old_ncc & 0x07)) ||
            !ie_write_bytes(&w, mm_context->old_kasme, OGS_GTP2_KASME_LEN))
            goto overflow;
        if (mm_context->old_nh_presence) {
            if (!ie_write_bytes(&w, mm_context->old_nh, OGS_GTP2_NH_LEN))
                goto overflow;
        }
    }

    if (!ie_write_lv(&w, mm_context->voice_domain_preference_len,
                mm_context->voice_domain_preference,
                OGS_GTP2_MAX_VOICE_DOMAIN_PREFERENCE_LEN) ||
        !ie_write_u16(&w, mm_context->ue_radio_capability_for_paging_len) ||
        !ie_write_bytes(&w, mm_context->ue_radio_capability_for_paging,
                mm_context->ue_radio_capability_for_paging_len))
        goto overflow;

    if (mm_context->extended_access_restriction_data_len) {
        if (!ie_write_u8(&w, 1) ||
            !ie_write_u8(&w, mm_context->extended_access_restriction_data))
            goto overflow;
    } else {
        if (!ie_write_u8(&w, 0))
            goto overflow;
    }

    if (!ie_write_lv(&w, mm_context->ue_additional_security_capability_len,
                mm_context->ue_additional_security_capability,
                OGS_GTP2_MAX_UE_ADDITIONAL_SECURITY_CAPABILITY_LEN) ||
        !ie_write_lv(&w, mm_context->ue_nr_security_capability_len,
                mm_context->ue_nr_security_capability,
                OGS_GTP2_MAX_UE_NR_SECURITY_CAPABILITY_LEN) ||
        !ie_write_u16(&w, mm_context->apn_rate_control_statuses_len) ||
        !ie_write_bytes(&w, mm_context->apn_rate_control_statuses,
                mm_context->apn_rate_control_statuses_len) ||
        !ie_write_lv(&w, mm_context->core_network_restrictions_len,
                mm_context->core_network_restrictions,
                OGS_GTP2_CORE_NETWORK_RESTRICTIONS_LEN) ||
        !ie_write_u8(&w, mm_context->ue_radio_capability_id_len) ||
        !ie_write_bytes(&w, mm_context->ue_radio_capability_id,
                mm_context->ue_radio_capability_id_len) ||
        !ie_write_u8(&w,
                ((mm_context->tridi ? 1 : 0) << 2) |
                (mm_context->ensct & 0x03)))
        goto overflow;

    octet->data = data;
    octet->len = w.pos;
    return octet->len;

overflow:
    ogs_error("MM Context does not fit or has an invalid field "
            "[data_len:%d pos:%d]", data_len, w.pos);
    return 0;
}

/* 8.46 Complete Request Message */
int16_t ogs_gtp2_parse_complete_request_message(
    ogs_gtp2_complete_request_message_t *message, ogs_tlv_octet_t *octet)
{
    ogs_assert(message);
    ogs_assert(octet);

    memset(message, 0, sizeof(*message));

    if (octet->len < 1) {
        ogs_error("Complete Request Message IE too short [%d]", octet->len);
        return 0;
    }
    message->type = ((uint8_t *)octet->data)[0];
    message->len = octet->len - 1;
    message->data = message->len ? (uint8_t *)octet->data + 1 : NULL;

    return octet->len;
}

int16_t ogs_gtp2_build_complete_request_message(ogs_tlv_octet_t *octet,
    ogs_gtp2_complete_request_message_t *message, void *data, int data_len)
{
    ie_writer_t w = { data, data_len, 0 };

    ogs_assert(message);
    ogs_assert(octet);
    ogs_assert(data);

    if (!ie_write_u8(&w, message->type) ||
        !ie_write_bytes(&w, message->data, message->len)) {
        ogs_error("Complete Request Message does not fit [%d]", data_len);
        return 0;
    }

    octet->data = data;
    octet->len = w.pos;
    return octet->len;
}

/* 8.47 GUTI */
int16_t ogs_gtp2_parse_guti(ogs_gtp2_guti_t *guti, ogs_tlv_octet_t *octet)
{
    ie_reader_t r;
    uint16_t mme_gid;
    uint8_t mme_code;
    uint32_t m_tmsi;

    ogs_assert(guti);
    ogs_assert(octet);

    memset(guti, 0, sizeof(*guti));
    r.data = octet->data;
    r.len = octet->len;
    r.pos = 0;

    if (!ie_read_bytes(&r, &guti->nas_plmn_id, OGS_PLMN_ID_LEN) ||
        !ie_read_u16(&r, &mme_gid) ||
        !ie_read_u8(&r, &mme_code) ||
        !ie_read_u32(&r, &m_tmsi)) {
        ogs_error("GUTI IE too short [%d]", octet->len);
        return 0;
    }
    guti->mme_gid = mme_gid;
    guti->mme_code = mme_code;
    guti->m_tmsi = m_tmsi;

    return octet->len;
}

int16_t ogs_gtp2_build_guti(ogs_tlv_octet_t *octet,
    ogs_gtp2_guti_t *guti, void *data, int data_len)
{
    ie_writer_t w = { data, data_len, 0 };

    ogs_assert(guti);
    ogs_assert(octet);
    ogs_assert(data);

    if (!ie_write_bytes(&w, &guti->nas_plmn_id, OGS_PLMN_ID_LEN) ||
        !ie_write_u16(&w, guti->mme_gid) ||
        !ie_write_u8(&w, guti->mme_code) ||
        !ie_write_u32(&w, guti->m_tmsi)) {
        ogs_error("GUTI does not fit [%d]", data_len);
        return 0;
    }

    octet->data = data;
    octet->len = w.pos;
    return octet->len;
}

/* 8.48 Fully Qualified Container (F-Container) */
int16_t ogs_gtp2_parse_f_container(
    ogs_gtp2_f_container_t *f_container, ogs_tlv_octet_t *octet)
{
    ogs_assert(f_container);
    ogs_assert(octet);

    memset(f_container, 0, sizeof(*f_container));

    if (octet->len < 1) {
        ogs_error("F-Container IE too short [%d]", octet->len);
        return 0;
    }
    f_container->container_type = ((uint8_t *)octet->data)[0] & 0x0f;
    f_container->len = octet->len - 1;
    f_container->data =
        f_container->len ? (uint8_t *)octet->data + 1 : NULL;

    return octet->len;
}

int16_t ogs_gtp2_build_f_container(ogs_tlv_octet_t *octet,
    ogs_gtp2_f_container_t *f_container, void *data, int data_len)
{
    ie_writer_t w = { data, data_len, 0 };

    ogs_assert(f_container);
    ogs_assert(octet);
    ogs_assert(data);

    if (!ie_write_u8(&w, f_container->container_type & 0x0f) ||
        !ie_write_bytes(&w, f_container->data, f_container->len)) {
        ogs_error("F-Container does not fit [%d]", data_len);
        return 0;
    }

    octet->data = data;
    octet->len = w.pos;
    return octet->len;
}

/* 8.49 Fully Qualified Cause (F-Cause) */
int16_t ogs_gtp2_parse_f_cause(
    ogs_gtp2_f_cause_t *f_cause, ogs_tlv_octet_t *octet)
{
    uint8_t *p = NULL;

    ogs_assert(f_cause);
    ogs_assert(octet);

    memset(f_cause, 0, sizeof(*f_cause));
    p = octet->data;

    if (octet->len == 2) {
        f_cause->value_len = 1;
        f_cause->value = p[1];
    } else if (octet->len == 3) {
        f_cause->value_len = 2;
        f_cause->value = (p[1] << 8) | p[2];
    } else {
        ogs_error("Invalid F-Cause IE length [%d]", octet->len);
        return 0;
    }
    f_cause->cause_type = p[0] & 0x0f;

    return octet->len;
}

int16_t ogs_gtp2_build_f_cause(ogs_tlv_octet_t *octet,
    ogs_gtp2_f_cause_t *f_cause, void *data, int data_len)
{
    ie_writer_t w = { data, data_len, 0 };
    bool ok;

    ogs_assert(f_cause);
    ogs_assert(octet);
    ogs_assert(data);

    ok = ie_write_u8(&w, f_cause->cause_type & 0x0f);
    if (f_cause->value_len == 2)
        ok = ok && ie_write_u16(&w, f_cause->value);
    else if (f_cause->value_len <= 1 && f_cause->value <= 0xff)
        ok = ok && ie_write_u8(&w, f_cause->value);
    else
        ok = false;

    if (!ok) {
        ogs_error("Invalid F-Cause [len:%d value:%d]",
                f_cause->value_len, f_cause->value);
        return 0;
    }

    octet->data = data;
    octet->len = w.pos;
    return octet->len;
}

/* 8.51 Target Identification */
int16_t ogs_gtp2_parse_target_identification(
    ogs_gtp2_target_identification_t *target, ogs_tlv_octet_t *octet)
{
    ie_reader_t r;
    uint8_t b[4];

    ogs_assert(target);
    ogs_assert(octet);

    memset(target, 0, sizeof(*target));
    r.data = octet->data;
    r.len = octet->len;
    r.pos = 0;

    if (!ie_read_u8(&r, &target->target_type))
        goto truncated;

    switch (target->target_type) {
    case OGS_GTP2_TARGET_TYPE_MACRO_ENODEB_ID:
    case OGS_GTP2_TARGET_TYPE_EXTENDED_MACRO_ENODEB_ID:
        /* Figure 8.51-2 and Figure 8.51-4 */
        if (!ie_read_bytes(&r, &target->nas_plmn_id, OGS_PLMN_ID_LEN) ||
            !ie_read_bytes(&r, b, 3) ||
            !ie_read_u16(&r, &target->tac))
            goto truncated;
        if (target->target_type == OGS_GTP2_TARGET_TYPE_MACRO_ENODEB_ID) {
            target->enodeb_id = ((b[0] & 0x0f) << 16) | (b[1] << 8) | b[2];
        } else {
            target->smenb = b[0] >> 7;
            if (target->smenb)
                target->enodeb_id =
                    ((b[0] & 0x03) << 16) | (b[1] << 8) | b[2];
            else
                target->enodeb_id =
                    ((b[0] & 0x1f) << 16) | (b[1] << 8) | b[2];
        }
        break;
    case OGS_GTP2_TARGET_TYPE_HOME_ENODEB_ID:
        /* Figure 8.51-3 */
        if (!ie_read_bytes(&r, &target->nas_plmn_id, OGS_PLMN_ID_LEN) ||
            !ie_read_bytes(&r, b, 4) ||
            !ie_read_u16(&r, &target->tac))
            goto truncated;
        target->enodeb_id = ((uint32_t)(b[0] & 0x0f) << 24) |
            (b[1] << 16) | (b[2] << 8) | b[3];
        break;
    default:
        /* Not an E-UTRAN target: only the Target Type is decoded */
        break;
    }

    return octet->len;

truncated:
    ogs_error("Invalid Target Identification [len:%d]", octet->len);
    ogs_log_hexdump(OGS_LOG_ERROR, octet->data, octet->len);
    return 0;
}

int16_t ogs_gtp2_build_target_identification(ogs_tlv_octet_t *octet,
    ogs_gtp2_target_identification_t *target, void *data, int data_len)
{
    ie_writer_t w = { data, data_len, 0 };
    uint32_t id = target ? target->enodeb_id : 0;
    bool ok;

    ogs_assert(target);
    ogs_assert(octet);
    ogs_assert(data);

    ok = ie_write_u8(&w, target->target_type) &&
        ie_write_bytes(&w, &target->nas_plmn_id, OGS_PLMN_ID_LEN);

    switch (target->target_type) {
    case OGS_GTP2_TARGET_TYPE_MACRO_ENODEB_ID:
        ok = ok && id <= 0xfffff && ie_write_u24(&w, id);
        break;
    case OGS_GTP2_TARGET_TYPE_EXTENDED_MACRO_ENODEB_ID:
        if (target->smenb)
            ok = ok && id <= 0x3ffff && ie_write_u24(&w, 0x800000 | id);
        else
            ok = ok && id <= 0x1fffff && ie_write_u24(&w, id);
        break;
    case OGS_GTP2_TARGET_TYPE_HOME_ENODEB_ID:
        ok = ok && id <= 0xfffffff && ie_write_u32(&w, id);
        break;
    default:
        ogs_error("Unsupported Target Type [%d]", target->target_type);
        return 0;
    }
    ok = ok && ie_write_u16(&w, target->tac);

    if (!ok) {
        ogs_error("Invalid Target Identification [type:%d id:0x%x]",
                target->target_type, id);
        return 0;
    }

    octet->data = data;
    octet->len = w.pos;
    return octet->len;
}
