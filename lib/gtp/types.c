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

int __ogs_gtp_domain;

/* 8.13 Protocol Configuration Options (PCO) 
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008 */

/* 8.15 Bearer Quality of Service (Bearer QoS) */
int16_t ogs_gtp_parse_bearer_qos(
    ogs_gtp_bearer_qos_t *bearer_qos, ogs_tlv_octet_t *octet)
{
    ogs_gtp_bearer_qos_t *source = (ogs_gtp_bearer_qos_t *)octet->data;
    int16_t size = 0;

    ogs_assert(bearer_qos);
    ogs_assert(octet);
    ogs_assert(octet->len == GTP_BEARER_QOS_LEN);

    memset(bearer_qos, 0, sizeof(ogs_gtp_bearer_qos_t));

    bearer_qos->pre_emption_capability = source->pre_emption_capability;
    bearer_qos->priority_level = source->priority_level;
    bearer_qos->pre_emption_vulnerability = source->pre_emption_vulnerability;
    size++;

    bearer_qos->qci = source->qci;
    size++;

    bearer_qos->ul_mbr = ogs_buffer_to_uint64(octet->data + size, 5);
    size += 5;
    bearer_qos->dl_mbr = ogs_buffer_to_uint64(octet->data + size, 5);
    size += 5;
    bearer_qos->ul_gbr = ogs_buffer_to_uint64(octet->data + size, 5);
    size += 5;
    bearer_qos->dl_gbr = ogs_buffer_to_uint64(octet->data + size, 5);
    size += 5;

    ogs_assert(size == octet->len);
    
    return size;
}
int16_t ogs_gtp_build_bearer_qos(ogs_tlv_octet_t *octet,
        ogs_gtp_bearer_qos_t *bearer_qos, void *data, int data_len)
{
    ogs_gtp_bearer_qos_t target;
    int16_t size = 0;

    ogs_assert(bearer_qos);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len >= GTP_BEARER_QOS_LEN);

    octet->data = data;
    memcpy(&target, bearer_qos, sizeof(ogs_gtp_bearer_qos_t));

    memcpy(octet->data + size, &target, 2);
    size += 2;

    ogs_uint64_to_buffer(target.ul_mbr, 5, octet->data + size);
    size += 5;
    ogs_uint64_to_buffer(target.dl_mbr, 5, octet->data + size);
    size += 5;
    ogs_uint64_to_buffer(target.ul_gbr, 5, octet->data + size);
    size += 5;
    ogs_uint64_to_buffer(target.dl_gbr, 5, octet->data + size);
    size += 5;

    octet->len = size;

    return octet->len;
}

/* 8.19 EPS Bearer Level Traffic Flow Template (Bearer TFT) 
 * See subclause 10.5.6.12 in 3GPP TS 24.008 [13]. */
int16_t ogs_gtp_build_tft(
    ogs_tlv_octet_t *octet, ogs_gtp_tft_t *tft, void *data, int data_len)
{
    ogs_gtp_tft_t target;
    uint16_t size = 0;
    int i, j;

    ogs_assert(tft);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len >= OGS_GTP_MAX_TRAFFIC_FLOW_TEMPLATE);

    octet->data = data;
    memcpy(&target, tft, sizeof(ogs_gtp_tft_t));

    ogs_assert(size + sizeof(target.flags) <= data_len);
    memcpy(octet->data + size, &target.flags, sizeof(target.flags));
    size += sizeof(target.flags);

    for (i = 0; i < target.num_of_packet_filter; i++)
    {
        ogs_assert(size + sizeof(target.pf[i].flags) <= data_len);
        memcpy(octet->data + size, &target.pf[i].flags,
                sizeof(target.pf[i].flags));
        size += sizeof(target.pf[i].flags);

        ogs_assert(size + sizeof(target.pf[i].precedence) <= data_len);
        memcpy(octet->data + size, &target.pf[i].precedence,
                sizeof(target.pf[i].precedence));
        size += sizeof(target.pf[i].precedence);

        ogs_assert(size + sizeof(target.pf[i].length) <= data_len);
        memcpy(octet->data + size, &target.pf[i].length,
                sizeof(target.pf[i].length));
        size += sizeof(target.pf[i].length);

        for (j = 0; j < target.pf[i].num_of_component; j++)
        {
            ogs_assert(size +
                sizeof(target.pf[i].component[j].type) <= data_len);
            memcpy(octet->data + size, &target.pf[i].component[j].type,
                sizeof(target.pf[i].component[j].type));
            size += sizeof(target.pf[i].component[j].type);
            switch(target.pf[i].component[j].type)
            {
                case GTP_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE:
                {
                    ogs_assert(size +
                        sizeof(target.pf[i].component[j].proto) <= data_len);
                    memcpy(octet->data + size, &target.pf[i].component[j].proto,
                        sizeof(target.pf[i].component[j].proto));
                    size += sizeof(target.pf[i].component[j].proto);
                    break;
                }
                case GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE:
                case GTP_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE:
                    ogs_assert(size +
                        sizeof(target.pf[i].component[j].ipv4.addr)
                            <= data_len);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv4.addr,
                        sizeof(target.pf[i].component[j].ipv4.addr));
                    size += sizeof(target.pf[i].component[j].ipv4.addr);

                    ogs_assert(size +
                        sizeof(target.pf[i].component[j].ipv4.mask)
                            <= data_len);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv4.mask,
                        sizeof(target.pf[i].component[j].ipv4.mask));
                    size += sizeof(target.pf[i].component[j].ipv4.mask);
                    break;
                case GTP_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE:
                case GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE:
                    ogs_assert(size +
                        sizeof(target.pf[i].component[j].ipv6.addr)
                            <= data_len);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv6.addr,
                        sizeof(target.pf[i].component[j].ipv6.addr));
                    size += sizeof(target.pf[i].component[j].ipv6.addr);

                    ogs_assert(size +
                        sizeof(target.pf[i].component[j].ipv6.prefixlen)
                            <= data_len);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv6.prefixlen,
                        sizeof(target.pf[i].component[j].ipv6.prefixlen));
                    size += sizeof(target.pf[i].component[j].ipv6.prefixlen);
                    break;
                case GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_TYPE:
                    ogs_assert(size +
                        sizeof(target.pf[i].component[j].ipv6_mask.addr)
                            <= data_len);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv6_mask.addr,
                        sizeof(target.pf[i].component[j].ipv6_mask.addr));
                    size += sizeof(target.pf[i].component[j].ipv6_mask.addr);

                    ogs_assert(size +
                        sizeof(target.pf[i].component[j].ipv6_mask.mask)
                            <= data_len);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv6_mask.mask,
                        sizeof(target.pf[i].component[j].ipv6_mask.mask));
                    size += sizeof(target.pf[i].component[j].ipv6_mask.mask);
                    break;
                case GTP_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE:
                case GTP_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE:
                    ogs_assert(size +
                        sizeof(target.pf[i].component[j].port.low)
                            <= data_len);
                    target.pf[i].component[j].port.low = 
                        htons(target.pf[i].component[j].port.low);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].port.low,
                        sizeof(target.pf[i].component[j].port.low));
                    size += sizeof(target.pf[i].component[j].port.low);
                    break;
                    
                case GTP_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE:
                case GTP_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE:
                    ogs_assert(size +
                        sizeof(target.pf[i].component[j].port.low)
                            <= data_len);
                    target.pf[i].component[j].port.low = 
                        htons(target.pf[i].component[j].port.low);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].port.low,
                        sizeof(target.pf[i].component[j].port.low));
                    size += sizeof(target.pf[i].component[j].port.low);

                    ogs_assert(size +
                        sizeof(target.pf[i].component[j].port.high)
                            <= data_len);
                    target.pf[i].component[j].port.high = 
                        htons(target.pf[i].component[j].port.high);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].port.high,
                        sizeof(target.pf[i].component[j].port.high));
                    size += sizeof(target.pf[i].component[j].port.high);
                    break;
                default:
                    ogs_error("Unknown Packet Filter Type(%d)", 
                            target.pf[i].component[j].type);
                    return -1;
            }
        }
    }


    octet->len = size;

    return octet->len;
}


/* 8.21 User Location Information (ULI) */
int16_t ogs_gtp_parse_uli(ogs_gtp_uli_t *uli, ogs_tlv_octet_t *octet)
{
    ogs_gtp_uli_t *source = (ogs_gtp_uli_t *)octet->data;
    int16_t size = 0;

    ogs_assert(uli);
    ogs_assert(octet);

    memset(uli, 0, sizeof(ogs_gtp_uli_t));

    uli->flags = source->flags;
    size++;

    if (uli->flags.cgi) {
        ogs_assert(size + sizeof(uli->cgi) <= octet->len);
        memcpy(&uli->cgi, octet->data + size, sizeof(uli->cgi));
        uli->cgi.lac = ntohs(uli->cgi.lac);
        uli->cgi.ci = ntohs(uli->cgi.ci);
        size += sizeof(uli->cgi);
    }
    if (uli->flags.sai) {
        ogs_assert(size + sizeof(uli->sai) <= octet->len);
        memcpy(&uli->sai, octet->data + size, sizeof(uli->sai));
        uli->sai.lac = ntohs(uli->sai.lac);
        uli->sai.sac = ntohs(uli->sai.sac);
        size += sizeof(uli->sai);
    }
    if (uli->flags.rai) {
        ogs_assert(size + sizeof(uli->rai) <= octet->len);
        memcpy(&uli->rai, octet->data + size, sizeof(uli->rai));
        uli->rai.lac = ntohs(uli->rai.lac);
        uli->rai.rac = ntohs(uli->rai.rac);
        size += sizeof(uli->rai);
    }
    if (uli->flags.tai) {
        ogs_assert(size + sizeof(uli->tai) <= octet->len);
        memcpy(&uli->tai, octet->data + size, sizeof(uli->tai));
        uli->tai.tac = ntohs(uli->tai.tac);
        size += sizeof(uli->tai);
    }
    if (uli->flags.e_cgi) {
        ogs_assert(size + sizeof(uli->e_cgi) <= octet->len);
        memcpy(&uli->e_cgi, octet->data + size, sizeof(uli->e_cgi));
        uli->e_cgi.cell_id = ntohl(uli->e_cgi.cell_id);
        size += sizeof(uli->e_cgi);
    }
    if (uli->flags.lai) {
        ogs_assert(size + sizeof(uli->lai) <= octet->len);
        memcpy(&uli->lai, octet->data + size, sizeof(uli->lai));
        uli->lai.lac = ntohs(uli->lai.lac);
        size += sizeof(uli->lai);
    }

    ogs_assert(size == octet->len);
    
    return size;
}
int16_t ogs_gtp_build_uli(
        ogs_tlv_octet_t *octet, ogs_gtp_uli_t *uli, void *data, int data_len)
{
    ogs_gtp_uli_t target;
    int16_t size = 0;

    ogs_assert(uli);
    ogs_assert(octet);
    ogs_assert(data);
    ogs_assert(data_len);

    octet->data = data;
    memcpy(&target, uli, sizeof(ogs_gtp_uli_t));

    ogs_assert(size + sizeof(target.flags) <= data_len);
    memcpy(octet->data + size, &target.flags, sizeof(target.flags));
    size += sizeof(target.flags);

    if (target.flags.cgi) {
        ogs_assert(size + sizeof(target.cgi) <= data_len);
        target.cgi.lac = htons(target.cgi.lac);
        target.cgi.ci = htons(target.cgi.ci);
        memcpy(octet->data + size, &target.cgi, sizeof(target.cgi));
        size += sizeof(target.cgi);
    }
    if (target.flags.sai) {
        ogs_assert(size + sizeof(target.sai) <= data_len);
        target.sai.lac = htons(target.sai.lac);
        target.sai.sac = htons(target.sai.sac);
        memcpy(octet->data + size, &target.sai, sizeof(target.sai));
        size += sizeof(target.sai);
    }
    if (target.flags.rai) {
        ogs_assert(size + sizeof(target.rai) <= data_len); 
        target.rai.lac = htons(target.rai.lac);
        target.rai.rac = htons(target.rai.rac);
        memcpy(octet->data + size, &target.rai, sizeof(target.rai));
        size += sizeof(target.rai);
    }
    if (target.flags.tai) {
        ogs_assert(size + sizeof(target.tai) <= data_len);
        target.tai.tac = htons(target.tai.tac);
        memcpy(octet->data + size, &target.tai, sizeof(target.tai));
        size += sizeof(target.tai);
    }
    if (target.flags.e_cgi) {
        ogs_assert(size + sizeof(target.e_cgi) <= data_len);
        target.e_cgi.cell_id = htonl(target.e_cgi.cell_id);
        memcpy(octet->data + size, &target.e_cgi, sizeof(target.e_cgi));
        size += sizeof(target.e_cgi);
    }
    if (target.flags.lai) {
        ogs_assert(size + sizeof(target.lai) <= data_len);
        target.lai.lac = htons(target.lai.lac);
        memcpy(octet->data + size, &target.lai, sizeof(target.lai));
        size += sizeof(target.lai);
    }

    octet->len = size;

    return octet->len;
}
