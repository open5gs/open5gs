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

typedef struct ogs_ipfw_rule_s {
    uint8_t proto;

    uint8_t ipv4_local;
    uint8_t ipv4_remote;
    uint8_t ipv6_local;
    uint8_t ipv6_remote;

    struct {
        struct {
            uint32_t addr[4];
            uint32_t mask[4];
        } local;
        struct {
            uint32_t addr[4];
            uint32_t mask[4];
        } remote;
    } ip;
    struct {
        struct {
            uint16_t low;
            uint16_t high;
        } local;
        struct {
            uint16_t low;
            uint16_t high;
        } remote;
    } port;

    uint16_t tos_traffic_class;
    uint32_t security_parameter_index;
    uint32_t flow_label; /* 24bit */
    uint32_t sdf_filter_id;
} ogs_ipfw_rule_t;

int ogs_ipfw_compile_rule(ogs_ipfw_rule_t *ipfw_rule, char *description);

#ifdef __cplusplus
}
#endif

#endif /* OGS_IPFW_H */
