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

#include "context.h"

#if HAVE_NETINET_IP_H
#include <netinet/ip.h>
#endif

#if HAVE_NETINET_IP6_H
#include <netinet/ip6.h>
#endif

#if HAVE_NETINET_IP_ICMP_H
#include <netinet/ip_icmp.h>
#endif

#if HAVE_NETINET_ICMP6_H
#include <netinet/icmp6.h>
#endif

#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#if HAVE_NET_IF_DL_H
#include <net/if_dl.h>
#endif

#if HAVE_IFADDRS_H
#include <ifaddrs.h>
#endif

#include "arp-nd.h"
#include "event.h"
#include "gtp-path.h"
#include "pfcp-path.h"
#include "rule-match.h"

#define UPF_GTP_HANDLED     1

const uint8_t proxy_mac_addr[] = { 0x0e, 0x00, 0x00, 0x00, 0x00, 0x01 };

static ogs_pkbuf_pool_t *packet_pool = NULL;

static void upf_gtp_handle_multicast(ogs_pkbuf_t *recvbuf);

static uint16_t _get_eth_type(uint8_t *data, uint len) {
    if (len > ETHER_HDR_LEN) {
        struct ether_header *hdr = (struct ether_header*)data;
        return htobe16(hdr->ether_type);
    }
    return 0;
}

static void _gtpv1_tun_recv_common_cb(
        short when, ogs_socket_t fd, bool has_eth, void *data)
{
    ogs_pkbuf_t *recvbuf = NULL;

    upf_sess_t *sess = NULL;
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_pdr_t *fallback_pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_user_plane_report_t report;

    recvbuf = ogs_tun_read(fd, packet_pool);
    if (!recvbuf) {
        ogs_warn("ogs_tun_read() failed");
        return;
    }

    if (has_eth) {
        ogs_pkbuf_t *replybuf = NULL;
        uint16_t eth_type = _get_eth_type(recvbuf->data, recvbuf->len);
        uint8_t size;

        if (eth_type == ETHERTYPE_ARP) {
            if (is_arp_req(recvbuf->data, recvbuf->len)) {
                replybuf = ogs_pkbuf_alloc(packet_pool, OGS_MAX_PKT_LEN);
                ogs_assert(replybuf);
                ogs_pkbuf_reserve(replybuf, OGS_TUN_MAX_HEADROOM);
                ogs_pkbuf_put(replybuf, OGS_MAX_PKT_LEN-OGS_TUN_MAX_HEADROOM);
                size = arp_reply(replybuf->data, recvbuf->data, recvbuf->len,
                    proxy_mac_addr);
                ogs_pkbuf_trim(replybuf, size);
                ogs_info("[SEND] reply to ARP request: %u", size);
            } else {
                goto cleanup;
            }
        } else if (eth_type == ETHERTYPE_IPV6 &&
                    is_nd_req(recvbuf->data, recvbuf->len)) {
            replybuf = ogs_pkbuf_alloc(packet_pool, OGS_MAX_PKT_LEN);
            ogs_assert(replybuf);
            ogs_pkbuf_reserve(replybuf, OGS_TUN_MAX_HEADROOM);
            ogs_pkbuf_put(replybuf, OGS_MAX_PKT_LEN-OGS_TUN_MAX_HEADROOM);
            size = nd_reply(replybuf->data, recvbuf->data, recvbuf->len,
                proxy_mac_addr);
            ogs_pkbuf_trim(replybuf, size);
            ogs_info("[SEND] reply to ND solicit: %u", size);
        }
        if (replybuf) {
            if (ogs_tun_write(fd, replybuf) != OGS_OK)
                ogs_warn("ogs_tun_write() for reply failed");
            goto cleanup;
        }
        if (eth_type != ETHERTYPE_IP && eth_type != ETHERTYPE_IPV6) {
            ogs_error("[DROP] Invalid eth_type [%x]]", eth_type);
            ogs_log_hexdump(OGS_LOG_ERROR, recvbuf->data, recvbuf->len);
            goto cleanup;
        }
        ogs_pkbuf_pull(recvbuf, ETHER_HDR_LEN);
    }

    sess = upf_sess_find_by_ue_ip_address(recvbuf);
    if (!sess)
        goto cleanup;

    ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
        far = pdr->far;
        ogs_assert(far);

        /* Check if PDR is Downlink */
        if (pdr->src_if != OGS_PFCP_INTERFACE_CORE)
            continue;

        /* Save the Fallback PDR : Lowest precedence downlink PDR */
        fallback_pdr = pdr;

        /* Check if FAR is Downlink */
        if (far->dst_if != OGS_PFCP_INTERFACE_ACCESS)
            continue;

        /* Check if Outer header creation */
        if (far->outer_header_creation.ip4 == 0 &&
            far->outer_header_creation.ip6 == 0 &&
            far->outer_header_creation.udp4 == 0 &&
            far->outer_header_creation.udp6 == 0 &&
            far->outer_header_creation.gtpu4 == 0 &&
            far->outer_header_creation.gtpu6 == 0)
            continue;

        /* Check if Rule List in PDR */
        if (ogs_list_first(&pdr->rule_list) &&
            ogs_pfcp_pdr_rule_find_by_packet(pdr, recvbuf) == NULL)
            continue;

        break;
    }

    if (!pdr)
        pdr = fallback_pdr;

    if (!pdr) {
        if (ogs_app()->parameter.multicast) {
            upf_gtp_handle_multicast(recvbuf);
        }
        goto cleanup;
    }

    ogs_assert(true == ogs_pfcp_up_handle_pdr(pdr, recvbuf, &report));

    if (report.type.downlink_data_report) {
        ogs_assert(pdr->sess);
        sess = UPF_SESS(pdr->sess);
        ogs_assert(sess);

        report.downlink_data.pdr_id = pdr->id;
        if (pdr->qer && pdr->qer->qfi)
            report.downlink_data.qfi = pdr->qer->qfi; /* for 5GC */

        ogs_assert(OGS_OK ==
            upf_pfcp_send_session_report_request(sess, &report));
    }

cleanup:
    ogs_pkbuf_free(recvbuf);
}

static void _gtpv1_tun_recv_cb(short when, ogs_socket_t fd, void *data)
{
    _gtpv1_tun_recv_common_cb(when, fd, false, data);
}

static void _gtpv1_tun_recv_eth_cb(short when, ogs_socket_t fd, void *data)
{
    _gtpv1_tun_recv_common_cb(when, fd, true, data);
}

static void _gtpv1_u_recv_cb(short when, ogs_socket_t fd, void *data)
{
    int len;
    ssize_t size;
    char buf[OGS_ADDRSTRLEN];

    upf_sess_t *sess = NULL;

    ogs_pkbuf_t *pkbuf = NULL;
    ogs_sockaddr_t from;

    ogs_gtp_header_t *gtp_h = NULL;
    ogs_pfcp_user_plane_report_t report;

    uint32_t teid;
    uint8_t qfi;

    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(packet_pool, OGS_MAX_PKT_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_TUN_MAX_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_PKT_LEN-OGS_TUN_MAX_HEADROOM);

    size = ogs_recvfrom(fd, pkbuf->data, pkbuf->len, 0, &from);
    if (size <= 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_recv() failed");
        goto cleanup;
    }

    ogs_pkbuf_trim(pkbuf, size);

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);

    gtp_h = (ogs_gtp_header_t *)pkbuf->data;
    if (gtp_h->version != OGS_GTP_VERSION_1) {
        ogs_error("[DROP] Invalid GTPU version [%d]", gtp_h->version);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        goto cleanup;
    }

    if (gtp_h->type == OGS_GTPU_MSGTYPE_ECHO_REQ) {
        ogs_pkbuf_t *echo_rsp;

        ogs_debug("[RECV] Echo Request from [%s]", OGS_ADDR(&from, buf));
        echo_rsp = ogs_gtp_handle_echo_req(pkbuf);
        ogs_expect(echo_rsp);
        if (echo_rsp) {
            ssize_t sent;

            /* Echo reply */
            ogs_debug("[SEND] Echo Response to [%s]", OGS_ADDR(&from, buf));

            sent = ogs_sendto(fd, echo_rsp->data, echo_rsp->len, 0, &from);
            if (sent < 0 || sent != echo_rsp->len) {
                ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                        "ogs_sendto() failed");
            }
            ogs_pkbuf_free(echo_rsp);
        }
        goto cleanup;
    }

    teid = be32toh(gtp_h->teid);

    ogs_debug("[RECV] GPU-U Type [%d] from [%s] : TEID[0x%x]",
            gtp_h->type, OGS_ADDR(&from, buf), teid);

    qfi = 0;
    if (gtp_h->flags & OGS_GTPU_FLAGS_E) {
        /*
         * TS29.281
         * 5.2.1 General format of the GTP-U Extension Header
         * Figure 5.2.1-3: Definition of Extension Header Type
         *
         * Note 4 : For a GTP-PDU with several Extension Headers, the PDU
         *          Session Container should be the first Extension Header
         */
        ogs_gtp_extension_header_t *extension_header =
            (ogs_gtp_extension_header_t *)(pkbuf->data + OGS_GTPV1U_HEADER_LEN);
        ogs_assert(extension_header);
        if (extension_header->type ==
                OGS_GTP_EXTENSION_HEADER_TYPE_PDU_SESSION_CONTAINER) {
            if (extension_header->pdu_type ==
                OGS_GTP_EXTENSION_HEADER_PDU_TYPE_UL_PDU_SESSION_INFORMATION) {
                    ogs_debug("   QFI [0x%x]",
                            extension_header->qos_flow_identifier);
                    qfi = extension_header->qos_flow_identifier;
            }
        }
    }

    /* Remove GTP header and send packets to TUN interface */
    len = ogs_gtpu_header_len(pkbuf);
    if (len < 0) {
        ogs_error("[DROP] Cannot decode GTPU packet");
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        goto cleanup;
    }
    if (gtp_h->type != OGS_GTPU_MSGTYPE_END_MARKER &&
        pkbuf->len <= len) {
        ogs_error("[DROP] Small GTPU packet(type:%d len:%d)", gtp_h->type, len);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        goto cleanup;
    }
    ogs_assert(ogs_pkbuf_pull(pkbuf, len));

    if (gtp_h->type == OGS_GTPU_MSGTYPE_END_MARKER) {
        /* Nothing */

    } else if (gtp_h->type == OGS_GTPU_MSGTYPE_ERR_IND) {
        ogs_pfcp_far_t *far = NULL;

        far = ogs_pfcp_far_find_by_error_indication(pkbuf);
        if (far) {
            ogs_assert(true ==
                ogs_pfcp_up_handle_error_indication(far, &report));

            if (report.type.error_indication_report) {
                ogs_assert(far->sess);
                sess = UPF_SESS(far->sess);
                ogs_assert(sess);

                ogs_assert(OGS_OK ==
                    upf_pfcp_send_session_report_request(sess, &report));
            }

        } else {
            ogs_error("[DROP] Cannot find FAR by Error-Indication");
            ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        }

    } else if (gtp_h->type == OGS_GTPU_MSGTYPE_GPDU) {
        uint16_t eth_type = 0;
        struct ip *ip_h = NULL;
        uint32_t *src_addr = NULL;
        ogs_pfcp_object_t *pfcp_object = NULL;
        ogs_pfcp_sess_t *pfcp_sess = NULL;
        ogs_pfcp_pdr_t *pdr = NULL;
        ogs_pfcp_far_t *far = NULL;

        ogs_pfcp_subnet_t *subnet = NULL;
        ogs_pfcp_dev_t *dev = NULL;

        ip_h = (struct ip *)pkbuf->data;
        ogs_assert(ip_h);

        pfcp_object = ogs_pfcp_object_find_by_teid(teid);
        if (!pfcp_object) {
            /* TODO : Send Error Indication */
            goto cleanup;
        }

        switch(pfcp_object->type) {
        case OGS_PFCP_OBJ_PDR_TYPE:
            pdr = (ogs_pfcp_pdr_t *)pfcp_object;
            ogs_assert(pdr);
            break;
        case OGS_PFCP_OBJ_SESS_TYPE:
            pfcp_sess = (ogs_pfcp_sess_t *)pfcp_object;
            ogs_assert(pfcp_sess);

            ogs_list_for_each(&pfcp_sess->pdr_list, pdr) {

                /* Check if Source Interface */
                if (pdr->src_if != OGS_PFCP_INTERFACE_ACCESS &&
                    pdr->src_if != OGS_PFCP_INTERFACE_CP_FUNCTION)
                    continue;

                /* Check if TEID */
                if (teid != pdr->f_teid.teid)
                    continue;

                /* Check if QFI */
                if (qfi && pdr->qfi != qfi)
                    continue;

                /* Check if Rule List in PDR */
                if (ogs_list_first(&pdr->rule_list) &&
                    ogs_pfcp_pdr_rule_find_by_packet(pdr, pkbuf) == NULL)
                    continue;

                break;
            }

            if (!pdr) {
                /* TODO : Send Error Indication */
                goto cleanup;
            }

            break;
        default:
            ogs_fatal("Unknown type [%d]", pfcp_object->type);
            ogs_assert_if_reached();
        }

        ogs_assert(pdr);
        ogs_assert(pdr->sess);
        ogs_assert(pdr->sess->obj.type == OGS_PFCP_OBJ_SESS_TYPE);

        sess = UPF_SESS(pdr->sess);
        ogs_assert(sess);

        far = pdr->far;
        ogs_assert(far);

        if (ip_h->ip_v == 4 && sess->ipv4) {
            src_addr = &ip_h->ip_src.s_addr;
            ogs_assert(src_addr);

            /*
             * From Issue #1354
             *
             * Do not check Indirect Tunnel
             *    pdr->dst_if = OGS_PFCP_INTERFACE_ACCESS;
             *    far->dst_if = OGS_PFCP_INTERFACE_ACCESS;
             */
            if (far->dst_if != OGS_PFCP_INTERFACE_ACCESS) {

                if (src_addr[0] == sess->ipv4->addr[0]) {
                    /* Source IP address should be matched in uplink */
                } else {
                    ogs_error("[DROP] Source IP-%d Spoofing APN:%s SrcIf:%d DstIf:%d TEID:0x%x",
                                ip_h->ip_v, pdr->dnn, pdr->src_if, far->dst_if, teid);
                    ogs_error("       SRC:%08X, UE:%08X",
                        be32toh(src_addr[0]), be32toh(sess->ipv4->addr[0]));
                    ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);

                    goto cleanup;
                }
            }

            subnet = sess->ipv4->subnet;
            eth_type = ETHERTYPE_IP;

        } else if (ip_h->ip_v == 6 && sess->ipv6) {
            struct ip6_hdr *ip6_h = (struct ip6_hdr *)pkbuf->data;
            ogs_assert(ip6_h);
            src_addr = (uint32_t *)ip6_h->ip6_src.s6_addr;
            ogs_assert(src_addr);

            /*
             * From Issue #1354
             *
             * Do not check Router Advertisement
             *    pdr->src_if = OGS_PFCP_INTERFACE_CP_FUNCTION;
             *    far->dst_if = OGS_PFCP_INTERFACE_ACCESS;
             *
             * Do not check Indirect Tunnel
             *    pdr->dst_if = OGS_PFCP_INTERFACE_ACCESS;
             *    far->dst_if = OGS_PFCP_INTERFACE_ACCESS;
             */
            if (far->dst_if != OGS_PFCP_INTERFACE_ACCESS) {

                if (IN6_IS_ADDR_LINKLOCAL((struct in6_addr *)src_addr) &&
                    src_addr[2] == sess->ipv6->addr[2] &&
                    src_addr[3] == sess->ipv6->addr[3]) {
                    /*
                     * if Link-local address,
                     * Interface Identifier should be matched
                     */
                } else if (src_addr[0] == sess->ipv6->addr[0] &&
                            src_addr[1] == sess->ipv6->addr[1]) {
                    /*
                     * If Global address
                     * 64 bit prefix should be matched
                     */
                } else {
                    ogs_error("[DROP] Source IP-%d Spoofing APN:%s SrcIf:%d DstIf:%d TEID:0x%x",
                                ip_h->ip_v, pdr->dnn, pdr->src_if, far->dst_if, teid);
                    ogs_error("SRC:%08x %08x %08x %08x",
                            be32toh(src_addr[0]), be32toh(src_addr[1]),
                            be32toh(src_addr[2]), be32toh(src_addr[3]));
                    ogs_error("UE:%08x %08x %08x %08x",
                            be32toh(sess->ipv6->addr[0]),
                            be32toh(sess->ipv6->addr[1]),
                            be32toh(sess->ipv6->addr[2]),
                            be32toh(sess->ipv6->addr[3]));
                    ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);

                    goto cleanup;
                }
            }

            subnet = sess->ipv6->subnet;
            eth_type = ETHERTYPE_IPV6;

        } else {
            ogs_error("Invalid packet [IP version:%d, Packet Length:%d]",
                    ip_h->ip_v, pkbuf->len);
            ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
            goto cleanup;
        }

        if (far->dst_if == OGS_PFCP_INTERFACE_CORE) {

            if (!subnet) {
#if 0 /* It's redundant log message */
                ogs_error("[DROP] Cannot find subnet V:%d, IPv4:%p, IPv6:%p",
                        ip_h->ip_v, sess->ipv4, sess->ipv6);
                ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
#endif
                goto cleanup;
            }

            dev = subnet->dev;
            ogs_assert(dev);

            if (dev->is_tap) {
                ogs_assert(eth_type);
                eth_type = htobe16(eth_type);
                ogs_pkbuf_push(pkbuf, sizeof(eth_type));
                memcpy(pkbuf->data, &eth_type, sizeof(eth_type));
                ogs_pkbuf_push(pkbuf, ETHER_ADDR_LEN);
                memcpy(pkbuf->data, proxy_mac_addr, ETHER_ADDR_LEN);
                ogs_pkbuf_push(pkbuf, ETHER_ADDR_LEN);
                memcpy(pkbuf->data, dev->mac_addr, ETHER_ADDR_LEN);
            }

            /* TODO: if destined to another UE, hairpin back out. */
            if (ogs_tun_write(dev->fd, pkbuf) != OGS_OK)
                ogs_warn("ogs_tun_write() failed");

        } else if (far->dst_if == OGS_PFCP_INTERFACE_ACCESS) {
            ogs_assert(true == ogs_pfcp_up_handle_pdr(pdr, pkbuf, &report));

            if (report.type.downlink_data_report) {
                ogs_error("Indirect Data Fowarding Buffered");

                report.downlink_data.pdr_id = pdr->id;
                if (pdr->qer && pdr->qer->qfi)
                    report.downlink_data.qfi = pdr->qer->qfi; /* for 5GC */

                ogs_assert(OGS_OK ==
                    upf_pfcp_send_session_report_request(sess, &report));
            }

        } else if (far->dst_if == OGS_PFCP_INTERFACE_CP_FUNCTION) {

            if (!far->gnode) {
                ogs_error("No Outer Header Creation in FAR");
                goto cleanup;
            }

            if ((far->apply_action & OGS_PFCP_APPLY_ACTION_FORW) == 0) {
                ogs_error("Not supported Apply Action [0x%x]",
                            far->apply_action);
                goto cleanup;
            }

            ogs_assert(true == ogs_pfcp_up_handle_pdr(pdr, pkbuf, &report));

            ogs_assert(report.type.downlink_data_report == 0);

        } else {
            ogs_fatal("Not implemented : FAR-DST_IF[%d]", far->dst_if);
            ogs_assert_if_reached();
        }
    } else {
        ogs_error("[DROP] Invalid GTPU Type [%d]", gtp_h->type);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
    }

cleanup:
    ogs_pkbuf_free(pkbuf);
}

int upf_gtp_init(void)
{
    ogs_pkbuf_config_t config;
    memset(&config, 0, sizeof config);

    config.cluster_2048_pool = ogs_app()->pool.packet;

#if OGS_USE_TALLOC
    /* allocate a talloc pool for GTP to ensure it doesn't have to go back
     * to the libc malloc all the time */
    packet_pool = talloc_pool(__ogs_talloc_core, 1000*1024);
    ogs_assert(packet_pool);
#else
    packet_pool = ogs_pkbuf_pool_create(&config);
#endif

    return OGS_OK;
}

void upf_gtp_final(void)
{
    ogs_pkbuf_pool_destroy(packet_pool);
}

static void _get_dev_mac_addr(char *ifname, uint8_t *mac_addr)
{
#ifdef SIOCGIFHWADDR
    int fd = socket(PF_INET, SOCK_DGRAM, 0);
    ogs_assert(fd);
    struct ifreq req;
    memset(&req, 0, sizeof(req));
    strncpy(req.ifr_name, ifname, IF_NAMESIZE-1);
    ogs_assert(ioctl(fd, SIOCGIFHWADDR, &req) == 0);
    memcpy(mac_addr, req.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
#else
    struct ifaddrs *ifap;
    ogs_assert(getifaddrs(&ifap) == 0);
    struct ifaddrs *p;
    for (p = ifap; p; p = p->ifa_next) {
        if (strncmp(ifname, p->ifa_name, IF_NAMESIZE-1) == 0) {
            struct sockaddr_dl* sdp = (struct sockaddr_dl*) p->ifa_addr;
            memcpy(mac_addr, sdp->sdl_data + sdp->sdl_nlen, ETHER_ADDR_LEN);
            freeifaddrs(ifap);
            return;
        }
    }
    ogs_assert(0); /* interface not found. */
#endif
}

int upf_gtp_open(void)
{
    ogs_pfcp_dev_t *dev = NULL;
    ogs_pfcp_subnet_t *subnet = NULL;
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;
    int rc;

    ogs_list_for_each(&ogs_gtp_self()->gtpu_list, node) {
        sock = ogs_gtp_server(node);
        if (!sock) return OGS_ERROR;

        if (sock->family == AF_INET)
            ogs_gtp_self()->gtpu_sock = sock;
        else if (sock->family == AF_INET6)
            ogs_gtp_self()->gtpu_sock6 = sock;

        node->poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, sock->fd, _gtpv1_u_recv_cb, sock);
        ogs_assert(node->poll);
    }

    OGS_SETUP_GTPU_SERVER;

    /* NOTE : tun device can be created via following command.
     *
     * $ sudo ip tuntap add name ogstun mode tun
     *
     * Also, before running upf, assign the one IP from IP pool of UE
     * to ogstun. The IP should not be assigned to UE
     *
     * $ sudo ifconfig ogstun 45.45.0.1/16 up
     *
     */

    /* Open Tun interface */
    ogs_list_for_each(&ogs_pfcp_self()->dev_list, dev) {
        dev->is_tap = strstr(dev->ifname, "tap");
        dev->fd = ogs_tun_open(dev->ifname, OGS_MAX_IFNAME_LEN, dev->is_tap);
        if (dev->fd == INVALID_SOCKET) {
            ogs_error("tun_open(dev:%s) failed", dev->ifname);
            return OGS_ERROR;
        }

        if (dev->is_tap) {
            _get_dev_mac_addr(dev->ifname, dev->mac_addr);
            dev->poll = ogs_pollset_add(ogs_app()->pollset,
                    OGS_POLLIN, dev->fd, _gtpv1_tun_recv_eth_cb, NULL);
            ogs_assert(dev->poll);
        } else {
            dev->poll = ogs_pollset_add(ogs_app()->pollset,
                    OGS_POLLIN, dev->fd, _gtpv1_tun_recv_cb, NULL);
            ogs_assert(dev->poll);
        }

        ogs_assert(dev->poll);
    }

    /*
     * On Linux, it is possible to create a persistent tun/tap
     * interface which will continue to exist even if open5gs quit,
     * although this is normally not required.
     * It can be useful to set up a tun/tap interface owned
     * by a non-root user, so open5gs can be started without
     * needing any root privileges at all.
     */

    /* Set P-to-P IP address with Netmask
     * Note that Linux will skip this configuration */
    ogs_list_for_each(&ogs_pfcp_self()->subnet_list, subnet) {
        ogs_assert(subnet->dev);
        rc = ogs_tun_set_ip(subnet->dev->ifname, &subnet->gw, &subnet->sub);
        if (rc != OGS_OK) {
            ogs_error("ogs_tun_set_ip(dev:%s) failed", subnet->dev->ifname);
            return OGS_ERROR;
        }
    }

    return OGS_OK;
}

void upf_gtp_close(void)
{
    ogs_pfcp_dev_t *dev = NULL;

    ogs_socknode_remove_all(&ogs_gtp_self()->gtpu_list);

    ogs_list_for_each(&ogs_pfcp_self()->dev_list, dev) {
        if (dev->poll)
            ogs_pollset_remove(dev->poll);
        ogs_closesocket(dev->fd);
    }
}

static void upf_gtp_handle_multicast(ogs_pkbuf_t *recvbuf)
{
    struct ip *ip_h =  NULL;
    struct ip6_hdr *ip6_h = NULL;
    ogs_pfcp_user_plane_report_t report;

    ip_h = (struct ip *)recvbuf->data;
    if (ip_h->ip_v == 6) {
#if COMPILE_ERROR_IN_MAC_OS_X  /* Compiler error in Mac OS X platform */
        ip6_h = (struct ip6_hdr *)recvbuf->data;
        if (IN6_IS_ADDR_MULTICAST(&ip6_h->ip6_dst))
#else
        struct in6_addr ip6_dst;
        ip6_h = (struct ip6_hdr *)recvbuf->data;
        memcpy(&ip6_dst, &ip6_h->ip6_dst, sizeof(struct in6_addr));
        if (IN6_IS_ADDR_MULTICAST(&ip6_dst))
#endif
        {
            upf_sess_t *sess = NULL;

            /* IPv6 Multicast */
            ogs_list_for_each(&upf_self()->sess_list, sess) {
                if (sess->ipv6) {
                    /* PDN IPv6 is avaiable */
                    ogs_pfcp_pdr_t *pdr = NULL;

                    ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
                        if (pdr->src_if == OGS_PFCP_INTERFACE_CORE) {
                            ogs_assert(true ==
                                ogs_pfcp_up_handle_pdr(pdr, recvbuf, &report));
                            break;
                        }
                    }

                    return;
                }
            }
        }
    }
}
