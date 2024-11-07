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
    ogs_assert(octet->len == GTP2_BEARER_QOS_LEN);

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
    ogs_assert(octet->len == GTP2_FLOW_QOS_LEN);

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

    ogs_assert(size+sizeof(tft->flags) <= octet->len);
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
        ogs_assert(size+sizeof(tft->pf[i].flags) <= octet->len);
        memcpy(&tft->pf[i].flags, (unsigned char *)octet->data+size,
                sizeof(tft->pf[i].flags));
        size += sizeof(tft->pf[i].flags);

        if (tft->code == OGS_GTP2_TFT_CODE_DELETE_PACKET_FILTERS_FROM_EXISTING)
            continue;

        ogs_assert(size+sizeof(tft->pf[i].precedence) <= octet->len);
        memcpy(&tft->pf[i].precedence, (unsigned char *)octet->data+size,
                sizeof(tft->pf[i].precedence));
        size += sizeof(tft->pf[i].precedence);

        ogs_assert(size+sizeof(tft->pf[i].content.length) <= octet->len);
        memcpy(&tft->pf[i].content.length, (unsigned char *)octet->data+size,
                sizeof(tft->pf[i].content.length));
        size += sizeof(tft->pf[i].content.length);

        j = 0; len = 0;
        while(len < tft->pf[i].content.length) {
            ogs_assert(size+len+sizeof(tft->pf[i].content.component[j].type) <=
                        octet->len);
            memcpy(&tft->pf[i].content.component[j].type,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].type));
            len += sizeof(tft->pf[i].content.component[j].type);
            switch(tft->pf[i].content.component[j].type) {
            case OGS_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE:
                ogs_assert(size+len+
                        sizeof(tft->pf[i].content.component[j].proto) <=
                        octet->len);
                memcpy(&tft->pf[i].content.component[j].proto,
                        (unsigned char *)octet->data+size+len,
                        sizeof(tft->pf[i].content.component[j].proto));
                len += sizeof(tft->pf[i].content.component[j].proto);
                break;
            case OGS_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE:
            case OGS_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE:
                ogs_assert(size+len+
                        sizeof(tft->pf[i].content.component[j].ipv4.addr) <=
                        octet->len);
                memcpy(&tft->pf[i].content.component[j].ipv4.addr,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv4.addr));
                len += sizeof(tft->pf[i].content.component[j].ipv4.addr);

                ogs_assert(size+len+
                    sizeof(tft->pf[i].content.component[j].ipv4.mask) <=
                    octet->len);
                memcpy(&tft->pf[i].content.component[j].ipv4.mask,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv4.mask));
                len += sizeof(tft->pf[i].content.component[j].ipv4.mask);
                break;
            case OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE:
            case OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE:
                ogs_assert(size+len+
                    sizeof(tft->pf[i].content.component[j].ipv6.addr) <=
                    octet->len);
                memcpy(&tft->pf[i].content.component[j].ipv6.addr,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv6.addr));
                len += sizeof(tft->pf[i].content.component[j].ipv6.addr);

                ogs_assert(size+len+
                    sizeof(tft->pf[i].content.component[j].ipv6.prefixlen) <=
                        octet->len);
                memcpy(&tft->pf[i].content.component[j].ipv6.prefixlen,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv6.prefixlen));
                len += sizeof(tft->pf[i].content.component[j].ipv6.prefixlen);
                break;
            case OGS_PACKET_FILTER_IPV6_LOCAL_ADDRESS_TYPE:
            case OGS_PACKET_FILTER_IPV6_REMOTE_ADDRESS_TYPE:
                ogs_assert(size+len+
                    sizeof(tft->pf[i].content.component[j].ipv6_mask.addr) <=
                        octet->len);
                memcpy(&tft->pf[i].content.component[j].ipv6_mask.addr,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv6_mask.addr));
                len += sizeof(tft->pf[i].content.component[j].ipv6_mask.addr);

                ogs_assert(size+len+
                    sizeof(tft->pf[i].content.component[j].ipv6_mask.mask) <=
                        octet->len);
                memcpy(&tft->pf[i].content.component[j].ipv6_mask.mask,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].ipv6_mask.mask));
                len += sizeof(tft->pf[i].content.component[j].ipv6_mask.mask);
                break;
            case OGS_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE:
            case OGS_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE:
                ogs_assert(size+len+
                    sizeof(tft->pf[i].content.component[j].port.low) <=
                        octet->len);
                memcpy(&tft->pf[i].content.component[j].port.low,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].port.low));
                tft->pf[i].content.component[j].port.low =
                    be16toh(tft->pf[i].content.component[j].port.low);
                len += sizeof(tft->pf[i].content.component[j].port.low);
                break;
            case OGS_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE:
            case OGS_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE:
                ogs_assert(size+len+
                    sizeof(tft->pf[i].content.component[j].port.low) <=
                        octet->len);
                memcpy(&tft->pf[i].content.component[j].port.low,
                    (unsigned char *)octet->data+size+len,
                    sizeof(tft->pf[i].content.component[j].port.low));
                tft->pf[i].content.component[j].port.low =
                    be16toh(tft->pf[i].content.component[j].port.low);
                len += sizeof(tft->pf[i].content.component[j].port.low);

                ogs_assert(size+len+
                    sizeof(tft->pf[i].content.component[j].port.high) <=
                        octet->len);
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
