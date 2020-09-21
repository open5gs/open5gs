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

#ifndef OGS_IPFW_H
#define OGS_IPFW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ogs-core.h"

typedef struct ogs_ipfw_rule_s {
    uint8_t proto;

    uint8_t ipv4_src;
    uint8_t ipv4_dst;
    uint8_t ipv6_src;
    uint8_t ipv6_dst;

    struct {
        struct {
            uint32_t addr[4];
            uint32_t mask[4];
        } src;
        struct {
            uint32_t addr[4];
            uint32_t mask[4];
        } dst;
    } ip;
    struct {
        struct {
            uint16_t low;
            uint16_t high;
        } src;
        struct {
            uint16_t low;
            uint16_t high;
        } dst;
    } port;

    uint16_t tos_traffic_class;
    uint32_t security_parameter_index;
    uint32_t flow_label; /* 24bit */
    uint32_t sdf_filter_id;
} ogs_ipfw_rule_t;

int ogs_ipfw_compile_rule(ogs_ipfw_rule_t *ipfw_rule, char *flow_description);
char *ogs_ipfw_encode_flow_description(ogs_ipfw_rule_t *ipfw_rule);

/*
 * Refer to lib/ipfw/ogs-ipfw.h
 * Issue #338
 *
 * <DOWNLINK>
 * RX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * PFCP : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * RULE : Source <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> Destination <UE_IP> <UE_PORT>
 * TFT : Local <UE_IP> <UE_PORT> REMOTE <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 *
 * <UPLINK>
 * RX : permit in from <UE_IP> <UE_PORT> to <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 * GX : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * PFCP : permit out from <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT> to <UE_IP> <UE_PORT>
 * RULE : Source <UE_IP> <UE_PORT> Destination <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 * TFT : Local <UE_IP> <UE_PORT> REMOTE <P-CSCF_RTP_IP> <P-CSCF_RTP_PORT>
 */
ogs_ipfw_rule_t *ogs_ipfw_copy_and_swap(
        ogs_ipfw_rule_t *dst, ogs_ipfw_rule_t *src);
void ogs_ipfw_rule_swap(ogs_ipfw_rule_t *ipfw_rule);

#ifdef __cplusplus
}
#endif

#endif /* OGS_IPFW_H */
