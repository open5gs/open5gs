/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-proto.h"

void ogs_extract_digit_from_string(char *digit, char *string)
{
    bool extracting = false;
    int i = 0;

    ogs_assert(string);
    ogs_assert(digit);

    while (*string && i < OGS_MAX_IMSI_BCD_LEN) {
        if (*string >= '0' && *string <= '9') {
            *digit++ = *string;
            extracting = true;
        } else if (extracting == true) {
            break;
        }
        string++;
        i++;
    }

    *digit = 0;
}

int ogs_paa_to_ip(const ogs_paa_t *paa, ogs_ip_t *ip)
{
    ogs_assert(paa);
    ogs_assert(ip);

    memset(ip, 0, sizeof *ip);

    if (paa->session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
        ip->ipv4 = 1;
        ip->addr = paa->both.addr;
        ip->ipv6 = 1;
        memcpy(ip->addr6, paa->both.addr6, OGS_IPV6_LEN);
    } else if (paa->session_type == OGS_PDU_SESSION_TYPE_IPV4) {
        ip->ipv4 = 1;
        ip->ipv6 = 0;
        ip->addr = paa->addr;
    } else if (paa->session_type == OGS_PDU_SESSION_TYPE_IPV6) {
        ip->ipv4 = 0;
        ip->ipv6 = 1;
        memcpy(ip->addr6, paa->addr6, OGS_IPV6_LEN);
    } else {
        ogs_error("No IPv4 or IPv6");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_ip_to_paa(const ogs_ip_t *ip, ogs_paa_t *paa)
{
    if (ip->ipv4 && ip->ipv6) {
        paa->session_type = OGS_PDU_SESSION_TYPE_IPV4V6;
        paa->both.addr = ip->addr;
        memcpy(paa->both.addr6, ip->addr6, OGS_IPV6_LEN);
    } else if (ip->ipv6) {
        paa->session_type = OGS_PDU_SESSION_TYPE_IPV6;
        memcpy(paa->addr6, ip->addr6, OGS_IPV6_LEN);
    } else if (ip->ipv4) {
        paa->session_type = OGS_PDU_SESSION_TYPE_IPV4;
        paa->addr = ip->addr;
    } else {
        return OGS_ERROR;
    }
    return OGS_OK;
}
