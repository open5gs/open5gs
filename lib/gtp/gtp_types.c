/*
 * Copyright (c) 2017, NextEPC Group
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define TRACE_MODULE _gtptypes

#include "core_debug.h"
#include "core_lib.h"
#include "core_network.h"

#include "gtp_types.h"
#include "gtp_message.h"

/* 8.13 Protocol Configuration Options (PCO) 
 * 10.5.6.3 Protocol configuration options in 3GPP TS 24.008 */

/* 8.15 Bearer Quality of Service (Bearer QoS) */
c_int16_t gtp_parse_bearer_qos(
    gtp_bearer_qos_t *bearer_qos, tlv_octet_t *octet)
{
    gtp_bearer_qos_t *source = (gtp_bearer_qos_t *)octet->data;
    c_int16_t size = 0;

    d_assert(bearer_qos, return -1, "Null param");
    d_assert(octet, return -1, "Null param");
    d_assert(octet->len == GTP_BEARER_QOS_LEN, return -1, "Null param");

    memset(bearer_qos, 0, sizeof(gtp_bearer_qos_t));

    bearer_qos->pre_emption_capability = source->pre_emption_capability;
    bearer_qos->priority_level = source->priority_level;
    bearer_qos->pre_emption_vulnerability = source->pre_emption_vulnerability;
    size++;

    bearer_qos->qci = source->qci;
    size++;

    bearer_qos->ul_mbr = core_buffer_to_uint64(octet->data + size, 5);
    size += 5;
    bearer_qos->dl_mbr = core_buffer_to_uint64(octet->data + size, 5);
    size += 5;
    bearer_qos->ul_gbr = core_buffer_to_uint64(octet->data + size, 5);
    size += 5;
    bearer_qos->dl_gbr = core_buffer_to_uint64(octet->data + size, 5);
    size += 5;

    d_assert(size == octet->len, return -1, "decode error");
    
    return size;
}
c_int16_t gtp_build_bearer_qos(
    tlv_octet_t *octet, gtp_bearer_qos_t *bearer_qos, void *data, int data_len)
{
    gtp_bearer_qos_t target;
    c_int16_t size = 0;

    d_assert(bearer_qos, return -1, "Null param");
    d_assert(octet, return -1, "Null param");
    d_assert(data, return -1, "Null param");
    d_assert(data_len >= GTP_BEARER_QOS_LEN, return -1, "Null param");

    octet->data = data;
    memcpy(&target, bearer_qos, sizeof(gtp_bearer_qos_t));

    memcpy(octet->data + size, &target, 2);
    size += 2;

    core_uint64_to_buffer(target.ul_mbr, 5, octet->data + size);
    size += 5;
    core_uint64_to_buffer(target.dl_mbr, 5, octet->data + size);
    size += 5;
    core_uint64_to_buffer(target.ul_gbr, 5, octet->data + size);
    size += 5;
    core_uint64_to_buffer(target.dl_gbr, 5, octet->data + size);
    size += 5;

    octet->len = size;

    return octet->len;
}

/* 8.19 EPS Bearer Level Traffic Flow Template (Bearer TFT) 
 * See subclause 10.5.6.12 in 3GPP TS 24.008 [13]. */
c_int16_t gtp_build_tft(
    tlv_octet_t *octet, gtp_tft_t *tft, void *data, int data_len)
{
    gtp_tft_t target;
    c_uint16_t size = 0;
    int i, j;

    d_assert(tft, return -1, "Null param");
    d_assert(octet, return -1, "Null param");
    d_assert(data, return -1, "Null param");
    d_assert(data_len >= GTP_MAX_TRAFFIC_FLOW_TEMPLATE,
            return -1, "Null param");

    octet->data = data;
    memcpy(&target, tft, sizeof(gtp_tft_t));

    d_assert(size + sizeof(target.flags) <= data_len, 
            return -1, "encode error");
    memcpy(octet->data + size, &target.flags, sizeof(target.flags));
    size += sizeof(target.flags);

    for (i = 0; i < target.num_of_packet_filter; i++)
    {
        d_assert(size + sizeof(target.pf[i].flags) <= data_len, 
                return -1, "encode error");
        memcpy(octet->data + size, &target.pf[i].flags,
                sizeof(target.pf[i].flags));
        size += sizeof(target.pf[i].flags);

        d_assert(size + sizeof(target.pf[i].precedence) <= data_len, 
                return -1, "encode error");
        memcpy(octet->data + size, &target.pf[i].precedence,
                sizeof(target.pf[i].precedence));
        size += sizeof(target.pf[i].precedence);

        d_assert(size + sizeof(target.pf[i].length) <= data_len, 
                return -1, "encode error");
        memcpy(octet->data + size, &target.pf[i].length,
                sizeof(target.pf[i].length));
        size += sizeof(target.pf[i].length);

        for (j = 0; j < target.pf[i].num_of_component; j++)
        {
            d_assert(size +
                sizeof(target.pf[i].component[j].type) <= data_len, 
                return -1, "encode error");
            memcpy(octet->data + size, &target.pf[i].component[j].type,
                sizeof(target.pf[i].component[j].type));
            size += sizeof(target.pf[i].component[j].type);
            switch(target.pf[i].component[j].type)
            {
                case GTP_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE:
                {
                    d_assert(size +
                        sizeof(target.pf[i].component[j].proto) <= data_len, 
                        return -1, "encode error");
                    memcpy(octet->data + size, &target.pf[i].component[j].proto,
                        sizeof(target.pf[i].component[j].proto));
                    size += sizeof(target.pf[i].component[j].proto);
                    break;
                }
                case GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE:
                case GTP_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE:
                    d_assert(size +
                        sizeof(target.pf[i].component[j].ipv4.addr)
                            <= data_len, 
                        return -1, "encode error");
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv4.addr,
                        sizeof(target.pf[i].component[j].ipv4.addr));
                    size += sizeof(target.pf[i].component[j].ipv4.addr);

                    d_assert(size +
                        sizeof(target.pf[i].component[j].ipv4.mask)
                            <= data_len, 
                        return -1, "encode error");
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv4.mask,
                        sizeof(target.pf[i].component[j].ipv4.mask));
                    size += sizeof(target.pf[i].component[j].ipv4.mask);
                    break;
                case GTP_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE:
                case GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE:
                    d_assert(size +
                        sizeof(target.pf[i].component[j].ipv6.addr)
                            <= data_len, 
                        return -1, "encode error");
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv6.addr,
                        sizeof(target.pf[i].component[j].ipv6.addr));
                    size += sizeof(target.pf[i].component[j].ipv6.addr);

                    d_assert(size +
                        sizeof(target.pf[i].component[j].ipv6.prefixlen)
                            <= data_len, 
                        return -1, "encode error");
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv6.prefixlen,
                        sizeof(target.pf[i].component[j].ipv6.prefixlen));
                    size += sizeof(target.pf[i].component[j].ipv6.prefixlen);
                    break;
                case GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_TYPE:
                    d_assert(size +
                        sizeof(target.pf[i].component[j].ipv6_mask.addr)
                            <= data_len, 
                        return -1, "encode error");
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv6_mask.addr,
                        sizeof(target.pf[i].component[j].ipv6_mask.addr));
                    size += sizeof(target.pf[i].component[j].ipv6_mask.addr);

                    d_assert(size +
                        sizeof(target.pf[i].component[j].ipv6_mask.mask)
                            <= data_len, 
                        return -1, "encode error");
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].ipv6_mask.mask,
                        sizeof(target.pf[i].component[j].ipv6_mask.mask));
                    size += sizeof(target.pf[i].component[j].ipv6_mask.mask);
                    break;
                case GTP_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE:
                case GTP_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE:
                    d_assert(size +
                        sizeof(target.pf[i].component[j].port.low)
                            <= data_len, 
                        return -1, "encode error");
                    target.pf[i].component[j].port.low = 
                        htons(target.pf[i].component[j].port.low);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].port.low,
                        sizeof(target.pf[i].component[j].port.low));
                    size += sizeof(target.pf[i].component[j].port.low);
                    break;
                    
                case GTP_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE:
                case GTP_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE:
                    d_assert(size +
                        sizeof(target.pf[i].component[j].port.low)
                            <= data_len, 
                        return -1, "encode error");
                    target.pf[i].component[j].port.low = 
                        htons(target.pf[i].component[j].port.low);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].port.low,
                        sizeof(target.pf[i].component[j].port.low));
                    size += sizeof(target.pf[i].component[j].port.low);

                    d_assert(size +
                        sizeof(target.pf[i].component[j].port.high)
                            <= data_len, 
                        return -1, "encode error");
                    target.pf[i].component[j].port.high = 
                        htons(target.pf[i].component[j].port.high);
                    memcpy(octet->data + size,
                        &target.pf[i].component[j].port.high,
                        sizeof(target.pf[i].component[j].port.high));
                    size += sizeof(target.pf[i].component[j].port.high);
                    break;
                default:
                    d_error("Unknown Packet Filter Type(%d)", 
                            target.pf[i].component[j].type);
                    return -1;
            }
        }
    }


    octet->len = size;

    return octet->len;
}


/* 8.21 User Location Information (ULI) */
c_int16_t gtp_parse_uli(gtp_uli_t *uli, tlv_octet_t *octet)
{
    gtp_uli_t *source = (gtp_uli_t *)octet->data;
    c_int16_t size = 0;

    d_assert(uli, return -1, "Null param");
    d_assert(octet, return -1, "Null param");

    memset(uli, 0, sizeof(gtp_uli_t));

    uli->flags = source->flags;
    size++;

    if (uli->flags.cgi)
    {
        d_assert(size + sizeof(uli->cgi) <= octet->len, 
                return -1, "decode error");
        memcpy(&uli->cgi, octet->data + size, sizeof(uli->cgi));
        uli->cgi.lac = ntohs(uli->cgi.lac);
        uli->cgi.ci = ntohs(uli->cgi.ci);
        size += sizeof(uli->cgi);
    }
    if (uli->flags.sai)
    {
        d_assert(size + sizeof(uli->sai) <= octet->len, 
                return -1, "decode error");
        memcpy(&uli->sai, octet->data + size, sizeof(uli->sai));
        uli->sai.lac = ntohs(uli->sai.lac);
        uli->sai.sac = ntohs(uli->sai.sac);
        size += sizeof(uli->sai);
    }
    if (uli->flags.rai)
    {
        d_assert(size + sizeof(uli->rai) <= octet->len, 
                return -1, "decode error");
        memcpy(&uli->rai, octet->data + size, sizeof(uli->rai));
        uli->rai.lac = ntohs(uli->rai.lac);
        uli->rai.rac = ntohs(uli->rai.rac);
        size += sizeof(uli->rai);
    }
    if (uli->flags.tai)
    {
        d_assert(size + sizeof(uli->tai) <= octet->len, 
                return -1, "decode error");
        memcpy(&uli->tai, octet->data + size, sizeof(uli->tai));
        uli->tai.tac = ntohs(uli->tai.tac);
        size += sizeof(uli->tai);
    }
    if (uli->flags.e_cgi)
    {
        d_assert(size + sizeof(uli->e_cgi) <= octet->len, 
                return -1, "decode error");
        memcpy(&uli->e_cgi, octet->data + size, sizeof(uli->e_cgi));
        uli->e_cgi.cell_id = ntohl(uli->e_cgi.cell_id);
        size += sizeof(uli->e_cgi);
    }
    if (uli->flags.lai)
    {
        d_assert(size + sizeof(uli->lai) <= octet->len, 
                return -1, "decode error");
        memcpy(&uli->lai, octet->data + size, sizeof(uli->lai));
        uli->lai.lac = ntohs(uli->lai.lac);
        size += sizeof(uli->lai);
    }

    d_assert(size == octet->len, return -1, "decode error");
    
    return size;
}
c_int16_t gtp_build_uli(
        tlv_octet_t *octet, gtp_uli_t *uli, void *data, int data_len)
{
    gtp_uli_t target;
    c_int16_t size = 0;

    d_assert(uli, return -1, "Null param");
    d_assert(octet, return -1, "Null param");
    d_assert(data, return -1, "Null param");
    d_assert(data_len, return -1, "Null param");

    octet->data = data;
    memcpy(&target, uli, sizeof(gtp_uli_t));

    d_assert(size + sizeof(target.flags) <= data_len, 
            return -1, "encode error");
    memcpy(octet->data + size, &target.flags, sizeof(target.flags));
    size += sizeof(target.flags);

    if (target.flags.cgi)
    {
        d_assert(size + sizeof(target.cgi) <= data_len, 
                return -1, "encode error");
        target.cgi.lac = htons(target.cgi.lac);
        target.cgi.ci = htons(target.cgi.ci);
        memcpy(octet->data + size, &target.cgi, sizeof(target.cgi));
        size += sizeof(target.cgi);
    }
    if (target.flags.sai)
    {
        d_assert(size + sizeof(target.sai) <= data_len, 
                return -1, "encode error");
        target.sai.lac = htons(target.sai.lac);
        target.sai.sac = htons(target.sai.sac);
        memcpy(octet->data + size, &target.sai, sizeof(target.sai));
        size += sizeof(target.sai);
    }
    if (target.flags.rai)
    {
        d_assert(size + sizeof(target.rai) <= data_len, 
                return -1, "encode error");
        target.rai.lac = htons(target.rai.lac);
        target.rai.rac = htons(target.rai.rac);
        memcpy(octet->data + size, &target.rai, sizeof(target.rai));
        size += sizeof(target.rai);
    }
    if (target.flags.tai)
    {
        d_assert(size + sizeof(target.tai) <= data_len, 
                return -1, "encode error");
        target.tai.tac = htons(target.tai.tac);
        memcpy(octet->data + size, &target.tai, sizeof(target.tai));
        size += sizeof(target.tai);
    }
    if (target.flags.e_cgi)
    {
        d_assert(size + sizeof(target.e_cgi) <= data_len, 
                return -1, "encode error");
        target.e_cgi.cell_id = htonl(target.e_cgi.cell_id);
        memcpy(octet->data + size, &target.e_cgi, sizeof(target.e_cgi));
        size += sizeof(target.e_cgi);
    }
    if (target.flags.lai)
    {
        d_assert(size + sizeof(target.lai) <= data_len, 
                return -1, "encode error");
        target.lai.lac = htons(target.lai.lac);
        memcpy(octet->data + size, &target.lai, sizeof(target.lai));
        size += sizeof(target.lai);
    }

    octet->len = size;

    return octet->len;
}
