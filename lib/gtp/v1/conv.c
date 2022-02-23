/*
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

int ogs_gtp1_gsn_addr_to_sockaddr(const ogs_gtp1_gsn_addr_t *gsnaddr,
        uint16_t gsnaddr_len, uint16_t port, ogs_sockaddr_t **list)
{
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

    ogs_assert(gsnaddr);
    ogs_assert(list);

    switch (gsnaddr_len) {
    case OGS_GTP_GSN_ADDRESS_IPV4_LEN:
        addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
        ogs_expect_or_return_val(addr, OGS_ERROR);
        addr->ogs_sa_family = AF_INET;
        addr->ogs_sin_port = port;
        addr->sin.sin_addr.s_addr = gsnaddr->addr;
        *list = addr;
        break;
    case OGS_GTP_GSN_ADDRESS_IPV6_LEN:
        addr6 = ogs_calloc(1, sizeof(ogs_sockaddr_t));
        ogs_expect_or_return_val(addr6, OGS_ERROR);
        addr6->ogs_sa_family = AF_INET6;
        addr6->ogs_sin_port = port;
        memcpy(addr6->sin6.sin6_addr.s6_addr, gsnaddr->addr6, OGS_IPV6_LEN);
        *list = addr6;
        break;
    default:
        ogs_error("No IPv4 or IPv6");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp1_sockaddr_to_gsn_addr(const ogs_sockaddr_t *addr,
        const ogs_sockaddr_t *addr6, ogs_gtp1_gsn_addr_t *gsnaddr, int *len)
{
    ogs_assert(gsnaddr);

    if (addr && addr6) {
        ogs_error("GSN Address: Both IPv4 and IPv6 not supported");
        return OGS_ERROR;
    } else if (addr) {
        gsnaddr->addr = addr->sin.sin_addr.s_addr;
        *len = OGS_GTP_GSN_ADDRESS_IPV4_LEN;
    } else if (addr6) {
        memcpy(gsnaddr->addr6, addr6->sin6.sin6_addr.s6_addr, OGS_IPV6_LEN);
        *len = OGS_GTP_GSN_ADDRESS_IPV6_LEN;
    } else {
        ogs_error("No IPv4 or IPv6");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp1_gsn_addr_to_ip(const ogs_gtp1_gsn_addr_t *gsnaddr, uint16_t gsnaddr_len,
        ogs_ip_t *ip)
{
    ogs_assert(ip);
    ogs_assert(gsnaddr);

    memset(ip, 0, sizeof(ogs_ip_t));

    if (gsnaddr_len == OGS_GTP_GSN_ADDRESS_IPV4_LEN) {
        ip->ipv4 = 1;
        ip->ipv6 = 0;
        ip->addr = gsnaddr->addr;
    } else if (gsnaddr_len == OGS_GTP_GSN_ADDRESS_IPV6_LEN) {
        ip->ipv4 = 0;
        ip->ipv6 = 1;
        memcpy(ip->addr6, gsnaddr->addr6, OGS_IPV6_LEN);
    } else {
        ogs_error("No IPv4 or IPv6");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp1_eua_to_ip(const ogs_eua_t *eua, uint16_t eua_len, ogs_ip_t *ip,
        uint8_t *pdu_session_type)
{
    ogs_assert(eua);
    ogs_assert(ip);

    memset(ip, 0, sizeof *ip);

    switch (eua->organization) {
    case OGS_PDP_EUA_ORG_IETF:
        break;
    case OGS_PDP_EUA_ORG_ETSI:
    default:
        ogs_error("Unsupported EUA organization %u", eua->organization);
        return OGS_ERROR;
    }

    eua_len -= 2;
    switch (eua->type) {
    case OGS_PDP_EUA_IETF_IPV4:
        if (eua_len == OGS_IPV4_LEN) {
            ip->addr = eua->addr;
        } else if (eua_len != 0) {
            ogs_error("Wrong IPv4 EUA length %u", eua_len);
            return OGS_ERROR;
        }
        ip->ipv4 = 1;
        ip->ipv6 = 0;
        *pdu_session_type = OGS_PDU_SESSION_TYPE_IPV4;
        break;
    case OGS_PDP_EUA_IETF_IPV6:
        if (eua_len == OGS_IPV6_LEN) {
            memcpy(ip->addr6, eua->addr6, OGS_IPV6_LEN);
        } else if (eua_len != 0) {
            ogs_error("Wrong IPv6 EUA length %u", eua_len);
            return OGS_ERROR;
        }
        ip->ipv4 = 0;
        ip->ipv6 = 1;
        *pdu_session_type = OGS_PDU_SESSION_TYPE_IPV6;
        break;
    case OGS_PDP_EUA_IETF_IPV4V6:
        if (eua_len == OGS_IPV4_LEN) {
            ip->addr = eua->addr;
        } else if (eua_len == OGS_IPV6_LEN) {
            memcpy(ip->addr6, eua->addr6, OGS_IPV6_LEN);
        } else if (eua_len == OGS_IPV4_LEN + OGS_IPV6_LEN) {
            ip->addr = eua->both.addr;
            memcpy(ip->addr6, eua->both.addr6, OGS_IPV6_LEN);
        } else if (eua_len != 0) {
            ogs_error("Wrong IPv4v6 EUA length %u", eua_len);
            return OGS_ERROR;
        }
        ip->ipv4 = 1;
        ip->ipv6 = 1;
        *pdu_session_type = OGS_PDU_SESSION_TYPE_IPV4V6;
        break;
    default:
        ogs_error("No IPv4 or IPv6");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_gtp1_ip_to_eua(uint8_t pdu_session_type, const ogs_ip_t *ip,
        ogs_eua_t *eua, uint8_t *eua_len)
{
    ogs_assert(eua);
    ogs_assert(ip);
    ogs_assert(eua_len);

    memset(eua, 0, sizeof *eua);

    eua->spare = 0xf; /* TS 29.060 Figure 35 */
    eua->organization = OGS_PDP_EUA_ORG_IETF;

    switch (pdu_session_type)
    {
    case OGS_PDU_SESSION_TYPE_IPV4:
        if (!ip->ipv4) {
            ogs_error("EUA type IPv4 but no IPv4 address available");
            return OGS_ERROR;
        }
        eua->addr = ip->addr;
        *eua_len = 2 + OGS_IPV4_LEN;
        eua->type = OGS_PDP_EUA_IETF_IPV4;
        break;
    case OGS_PDU_SESSION_TYPE_IPV6:
        if (!ip->ipv6) {
            ogs_error("EUA type IPv4 but no IPv6 address available");
            return OGS_ERROR;
        }
        memcpy(eua->addr6, ip->addr6, OGS_IPV6_LEN);
        *eua_len = 2 + OGS_IPV6_LEN;
        eua->type = OGS_PDP_EUA_IETF_IPV6;
        break;
    case OGS_PDU_SESSION_TYPE_IPV4V6:
        if (ip->ipv4 && ip->ipv6) {
            eua->both.addr = ip->addr;
            memcpy(eua->both.addr6, ip->addr6, OGS_IPV6_LEN);
            *eua_len = 2 + OGS_IPV4_LEN + OGS_IPV6_LEN;
        } else if (ip->ipv4) {
            eua->addr = ip->addr;
            *eua_len = 2 + OGS_IPV4_LEN;
        } else if (ip->ipv6) {
            memcpy(eua->addr6, ip->addr6, OGS_IPV6_LEN);
            *eua_len = 2 + OGS_IPV6_LEN;
        } else {
            ogs_error("EUA type IPv4 but no IPv4 nor IPv6 address available");
            return OGS_ERROR;
        }
        eua->type = OGS_PDP_EUA_IETF_IPV4V6;
        break;
    default:
        ogs_error("Unexpected session type");
        return OGS_ERROR;
    }
    return OGS_OK;
}

/* 3GPP TS 23.401 Table E.3 */
int ogs_gtp1_qos_profile_to_qci(const ogs_gtp1_qos_profile_decoded_t *decoded,
        uint8_t *qci)
{
    ogs_assert(decoded);
    ogs_assert(qci);

    if (!decoded->data_octet6_to_13_present) {
        /* traffic class not present, take QCI 9 as default */
        *qci = 9;
        return OGS_OK;
    }

    switch (decoded->qos_profile.data.traffic_class) {
    case OGS_GTP1_QOS_TRAFFIC_CLASS_CONVERSATIONAL:
        if (decoded->qos_profile.data.source_statistics_descriptor == OGS_GTP1_QOS_SRC_STATS_DESC_SPEECH)
            *qci = 1;
        else if (decoded->dec_transfer_delay >= 150)
            *qci = 2;
        else
            *qci = 3;
        break;
    case OGS_GTP1_QOS_TRAFFIC_CLASS_STREAMING:
        *qci = 4;
        break;
    case OGS_GTP1_QOS_TRAFFIC_CLASS_INTERACTIVE:
        switch (decoded->qos_profile.data.traffic_handling_priority) {
        case 1:
            *qci = decoded->qos_profile.data.signalling_indication ? 5 : 6;
            break;
        case 2:
            *qci = 7;
            break;
        case 3:
            *qci = 8;
            break;
        default:
            *qci = 9;
        }
        break;
    case OGS_GTP1_QOS_TRAFFIC_CLASS_SUBSCRIBED:
    case OGS_GTP1_QOS_TRAFFIC_CLASS_BACKGROUND:
    default:
        *qci = 9;
    }
    return OGS_OK;
}
