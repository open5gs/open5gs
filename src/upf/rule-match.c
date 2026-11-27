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

#define _DEFAULT_SOURCE 1
#define _BSD_SOURCE     1

#include "rule-match.h"

#if HAVE_NETINET_IP_H
#include <netinet/ip.h>
#endif

#if HAVE_NETINET_IP6_H
#include <netinet/ip6.h>
#endif

upf_sess_t *upf_sess_find_by_ue_ip_address(ogs_pkbuf_t *pkbuf)
{
    upf_sess_t *sess = NULL;

    char buf[OGS_ADDRSTRLEN];

    struct ip *ip_h = NULL;
    struct ip6_hdr *ip6_h = NULL;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);
    ogs_assert(pkbuf->data);

    ip_h = (struct ip *)pkbuf->data;
    if (ip_h->ip_v == 4) {
        ip_h = (struct ip *)pkbuf->data;
        sess = upf_sess_find_by_ipv4(ip_h->ip_dst.s_addr);
    } else if (ip_h->ip_v == 6) {
        ip6_h = (struct ip6_hdr *)pkbuf->data;
        sess = upf_sess_find_by_ipv6((uint32_t *)ip6_h->ip6_dst.s6_addr);
    } else {
        ogs_error("Invalid packet [IP version:%d, Packet Length:%d]",
                ip_h->ip_v, pkbuf->len);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
    }

    if (sess) {
        if (ip_h && sess->ipv4)
            ogs_trace("PAA IPv4:%s", OGS_INET_NTOP(&sess->ipv4->addr, buf));
        if (ip6_h && sess->ipv6)
            ogs_trace("PAA IPv6:%s", OGS_INET6_NTOP(&sess->ipv6->addr, buf));
    }

    return sess;
}
