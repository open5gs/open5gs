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

#include "event.h"
#include "gtp-path.h"
#include "pfcp-path.h"
#include "s5c-build.h"
#include "gn-build.h"

static bool check_if_router_solicit(ogs_pkbuf_t *pkbuf);
static void send_router_advertisement(smf_sess_t *sess, uint8_t *ip6_dst);

static void bearer_timeout(ogs_gtp_xact_t *xact, void *data);

static void _gtpv1v2_c_recv_cb(short when, ogs_socket_t fd, void *data)
{
    smf_event_t *e = NULL;
    int rv;
    ssize_t size;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_sockaddr_t from;
    ogs_gtp_node_t *gnode = NULL;
    uint8_t gtp_ver;
    char frombuf[OGS_ADDRSTRLEN];

    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);

    size = ogs_recvfrom(fd, pkbuf->data, pkbuf->len, 0, &from);
    if (size <= 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_recvfrom() failed");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    ogs_pkbuf_trim(pkbuf, size);

    gnode = ogs_gtp_node_find_by_addr(&smf_self()->sgw_s5c_list, &from);
    if (!gnode) {
        gnode = ogs_gtp_node_add_by_addr(&smf_self()->sgw_s5c_list, &from);
        if (!gnode) {
            ogs_error("Failed to create new gnode(%s:%u), mempool full, ignoring msg!",
                      OGS_ADDR(&from, frombuf), OGS_PORT(&from));
            ogs_pkbuf_free(pkbuf);
            return;
        }
        gnode->sock = data;
        smf_gtp_node_new(gnode);
        smf_metrics_inst_global_inc(SMF_METR_GLOB_GAUGE_GTP_PEERS_ACTIVE);
    }

    gtp_ver = ((ogs_gtp2_header_t *)pkbuf->data)->version;
    switch (gtp_ver) {
    case 1:
        e = smf_event_new(SMF_EVT_GN_MESSAGE);
        break;
    case 2:
        e = smf_event_new(SMF_EVT_S5C_MESSAGE);
        break;
    default:
        ogs_warn("Rx unexpected GTP version %u", gtp_ver);
        ogs_pkbuf_free(pkbuf);
        return;
    }
    ogs_assert(e);
    e->gnode = gnode->data_ptr; /* smf_gtp_node_t */
    e->pkbuf = pkbuf;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_pkbuf_free(e->pkbuf);
        ogs_event_free(e);
    }
}

static void _gtpv1_u_recv_cb(short when, ogs_socket_t fd, void *data)
{
    int len;
    ssize_t size;
    char buf[OGS_ADDRSTRLEN];

    ogs_pkbuf_t *pkbuf = NULL;
    ogs_sockaddr_t from;

    ogs_gtp2_header_t *gtp_h = NULL;
    ogs_gtp2_header_desc_t header_desc;

    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_PKT_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, OGS_MAX_PKT_LEN);

    size = ogs_recvfrom(fd, pkbuf->data, pkbuf->len, 0, &from);
    if (size <= 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_recv() failed");
        goto cleanup;
    }

    ogs_pkbuf_trim(pkbuf, size);

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);

    gtp_h = (ogs_gtp2_header_t *)pkbuf->data;
    if (gtp_h->version != OGS_GTP2_VERSION_1) {
        ogs_error("[DROP] Invalid GTPU version [%d]", gtp_h->version);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        goto cleanup;
    }

    len = ogs_gtpu_parse_header(&header_desc, pkbuf);
    if (len < 0) {
        ogs_error("[DROP] Cannot decode GTPU packet");
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        goto cleanup;
    }
    if (header_desc.type == OGS_GTPU_MSGTYPE_ECHO_REQ) {
        ogs_pkbuf_t *echo_rsp;

        ogs_debug("[RECV] Echo Request from [%s]", OGS_ADDR(&from, buf));
        echo_rsp = ogs_gtp2_handle_echo_req(pkbuf);
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
    if (header_desc.type != OGS_GTPU_MSGTYPE_END_MARKER &&
        pkbuf->len <= len) {
        ogs_error("[DROP] Small GTPU packet(type:%d len:%d)",
                header_desc.type, len);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        goto cleanup;
    }

    ogs_debug("[RECV] GPU-U Type [%d] from [%s] : TEID[0x%x]",
            header_desc.type, OGS_ADDR(&from, buf), header_desc.teid);

    /* Remove GTP header and send packets to TUN interface */
    ogs_assert(ogs_pkbuf_pull(pkbuf, len));

    if (header_desc.type == OGS_GTPU_MSGTYPE_GPDU) {
        smf_sess_t *sess = NULL;
        ogs_pfcp_far_t *far = NULL;

        far = ogs_pfcp_far_find_by_teid(header_desc.teid);
        if (!far) {
            ogs_error("No FAR for TEID [%d]", header_desc.teid);
            goto cleanup;
        }

        if (far->dst_if != OGS_PFCP_INTERFACE_CP_FUNCTION) {
            ogs_error("Invalid Destination Interface [%d]", far->dst_if);
            goto cleanup;
        }

        if (header_desc.qos_flow_identifier) {
            ogs_error("QFI[%d] Found", header_desc.qos_flow_identifier);
            goto cleanup;
        }

        ogs_assert(far->sess);
        sess = SMF_SESS(far->sess);
        ogs_assert(sess);

        if (sess->ipv6 && check_if_router_solicit(pkbuf) == true) {
            struct ip6_hdr *ip6_h = (struct ip6_hdr *)pkbuf->data;
            ogs_assert(ip6_h);
            send_router_advertisement(sess, ip6_h->ip6_src.s6_addr);
        }
    } else {
        ogs_error("[DROP] Invalid GTPU Type [%d]", header_desc.type);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
    }

cleanup:
    ogs_pkbuf_free(pkbuf);
}

int smf_gtp_open(void)
{
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    ogs_list_for_each(&ogs_gtp_self()->gtpc_list, node) {
        sock = ogs_gtp_server(node);
        if (!sock) return OGS_ERROR;

        node->poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, sock->fd, _gtpv1v2_c_recv_cb, sock);
        ogs_assert(node->poll);
    }
    ogs_list_for_each(&ogs_gtp_self()->gtpc_list6, node) {
        sock = ogs_gtp_server(node);
        if (!sock) return OGS_ERROR;

        node->poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, sock->fd, _gtpv1v2_c_recv_cb, sock);
        ogs_assert(node->poll);
    }

    OGS_SETUP_GTPC_SERVER;
    /* If we only use 5G, we don't need GTP-C, so there is no check routine. */
    if (!ogs_gtp_self()->gtpc_sock  && !ogs_gtp_self()->gtpc_sock6)
        ogs_warn("No GTP-C configuration");

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

    /* Fetch link-local address for router advertisement */
    if (ogs_gtp_self()->link_local_addr)
        ogs_freeaddrinfo(ogs_gtp_self()->link_local_addr);
    if (ogs_gtp_self()->gtpu_addr6)
        ogs_gtp_self()->link_local_addr =
            ogs_link_local_addr_by_sa(ogs_gtp_self()->gtpu_addr6);

    return OGS_OK;
}

void smf_gtp_close(void)
{
    if (ogs_gtp_self()->link_local_addr)
        ogs_freeaddrinfo(ogs_gtp_self()->link_local_addr);

    ogs_socknode_remove_all(&ogs_gtp_self()->gtpc_list);
    ogs_socknode_remove_all(&ogs_gtp_self()->gtpc_list6);

    ogs_socknode_remove_all(&ogs_gtp_self()->gtpu_list);
}

int smf_gtp1_send_create_pdp_context_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact)
{
    int rv;
    ogs_gtp1_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_gtp1_header_t));
    h.type = OGS_GTP1_CREATE_PDP_CONTEXT_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    pkbuf = smf_gn_build_create_pdp_context_response(h.type, sess);
    if (!pkbuf) {
        ogs_error("smf_gn_build_create_pdp_context_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp1_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp1_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_gtp1_send_delete_pdp_context_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact)
{
    int rv;
    ogs_gtp1_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_gtp1_header_t));
    h.type = OGS_GTP1_DELETE_PDP_CONTEXT_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    pkbuf = smf_gn_build_delete_pdp_context_response(h.type, sess);
    if (!pkbuf) {
        ogs_error("smf_gn_build_delete_pdp_context_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp1_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp1_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

#if 0
int smf_gtp1_send_update_pdp_context_request(
        smf_bearer_t *bearer, uint8_t pti, uint8_t cause_value)
{
    int rv;

    ogs_gtp_xact_t *xact = NULL;
    ogs_gtp1_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    smf_sess_t *sess = NULL;

    ogs_assert(bearer);
    sess = smf_sess_find_by_id(bearer->sess_id);
    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_gtp1_header_t));
    h.type = OGS_GTP1_UPDATE_PDP_CONTEXT_REQUEST_TYPE;
    h.teid = sess->sgw_s5c_teid;

    pkbuf = smf_gn_build_update_pdp_context_request(
                h.type, bearer, pti, cause_value);
    if (!pkbuf) {
        ogs_error("smf_gn_build_update_pdp_context_request() failed");
        return OGS_ERROR;
    }

    xact = ogs_gtp1_xact_local_create(
            sess->gnode, &h, pkbuf, bearer_timeout,
            OGS_UINT_TO_POINTER(bearer->id));
    if (!xact) {
        ogs_error("ogs_gtp1_xact_local_create() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}
#endif

int smf_gtp1_send_update_pdp_context_response(
        smf_bearer_t *bearer, ogs_gtp_xact_t *xact)
{
    int rv;

    ogs_gtp1_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    smf_sess_t *sess = NULL;

    ogs_assert(bearer);
    ogs_assert(xact);
    sess = smf_sess_find_by_id(bearer->sess_id);
    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_gtp1_header_t));
    h.type = OGS_GTP1_UPDATE_PDP_CONTEXT_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    pkbuf = smf_gn_build_update_pdp_context_response(
                h.type, sess, bearer);
    if (!pkbuf) {
        ogs_error("smf_gn_build_update_pdp_context_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp1_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp1_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_gtp2_send_create_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact)
{
    int rv;
    ogs_gtp2_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    pkbuf = smf_s5c_build_create_session_response(h.type, sess);
    if (!pkbuf) {
        ogs_error("smf_s5c_build_create_session_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_gtp2_send_modify_bearer_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact,
        ogs_gtp2_modify_bearer_request_t *req, bool sgw_relocation)
{
    int rv;
    ogs_gtp2_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sess);
    ogs_assert(xact);
    ogs_assert(req);

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    pkbuf = smf_s5c_build_modify_bearer_response(
                h.type, sess, req, sgw_relocation);
    if (!pkbuf) {
        ogs_error("smf_s5c_build_modify_bearer_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_gtp2_send_delete_session_response(
        smf_sess_t *sess, ogs_gtp_xact_t *xact)
{
    int rv;
    ogs_gtp2_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(xact);
    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE;
    h.teid = sess->sgw_s5c_teid;

    pkbuf = smf_s5c_build_delete_session_response(h.type, sess);
    if (!pkbuf) {
        ogs_error("smf_s5c_build_delete_session_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_gtp2_send_delete_bearer_request(
        smf_bearer_t *bearer, uint8_t pti, uint8_t cause_value)
{
    int rv;

    ogs_gtp_xact_t *xact = NULL;
    ogs_gtp2_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;

    smf_sess_t *sess = NULL;

    ogs_assert(bearer);
    sess = smf_sess_find_by_id(bearer->sess_id);
    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_DELETE_BEARER_REQUEST_TYPE;
    h.teid = sess->sgw_s5c_teid;

    pkbuf = smf_s5c_build_delete_bearer_request(
                h.type, bearer, pti, cause_value);
    if (!pkbuf) {
        ogs_error("smf_s5c_build_delete_bearer_request() failed");
        return OGS_ERROR;
    }

    xact = ogs_gtp_xact_local_create(
            sess->gnode, &h, pkbuf, bearer_timeout,
            OGS_UINT_TO_POINTER(bearer->id));
    if (!xact) {
        ogs_error("ogs_gtp_xact_local_create() failed");
        return OGS_ERROR;
    }
    xact->local_teid = sess->smf_n4_teid;

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

static bool check_if_router_solicit(ogs_pkbuf_t *pkbuf)
{
    struct ip *ip_h = NULL;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);
    ogs_assert(pkbuf->data);

    ip_h = (struct ip *)pkbuf->data;
    if (ip_h->ip_v == 6) {
        struct ip6_hdr *ip6_h = (struct ip6_hdr *)pkbuf->data;
        if (ip6_h->ip6_nxt == IPPROTO_ICMPV6) {
            struct icmp6_hdr *icmp_h =
                (struct icmp6_hdr *)(pkbuf->data + sizeof(struct ip6_hdr));
            if (icmp_h->icmp6_type == ND_ROUTER_SOLICIT) {
                ogs_debug("      Router Solict");
                return true;
            }
        }
    }

    return false;
}

static void send_router_advertisement(smf_sess_t *sess, uint8_t *ip6_dst)
{
    int rv;

    ogs_pkbuf_t *pkbuf = NULL;

    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_ue_ip_t *ue_ip = NULL;
    ogs_pfcp_subnet_t *subnet = NULL;
    char ipstr[OGS_ADDRSTRLEN];

    ogs_ipsubnet_t src_ipsub;
    uint16_t plen = 0;
    uint8_t nxt = 0;
    uint8_t *p = NULL;
    struct ip6_hdr *ip6_h =  NULL;
    struct nd_router_advert *advert_h = NULL;
    struct nd_opt_prefix_info *prefix = NULL;

    ogs_assert(sess);
    ue_ip = sess->ipv6;
    ogs_assert(ue_ip);
    subnet = ue_ip->subnet;
    ogs_assert(subnet);

    /* Fetch link-local address for router advertisement */
    if (ogs_gtp_self()->link_local_addr) {
        OGS_ADDR(ogs_gtp_self()->link_local_addr, ipstr);
        rv = ogs_ipsubnet(&src_ipsub, ipstr, NULL);
        if (rv != OGS_OK) {
            ogs_error("ogs_ipsubnet() failed");
            return;
        }
    } else {
        /* For the case of loopback used for GTPU link-local address is not
         * available, hence set the source IP to fe80::1
        */
        memset(src_ipsub.sub, 0, sizeof(src_ipsub.sub));
        src_ipsub.sub[0] = htobe32(0xfe800000);
        src_ipsub.sub[3] = htobe32(0x00000001);
    }

    ogs_debug("      Build Router Advertisement");

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_GTPV1U_5GC_HEADER_LEN+200);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_GTPV1U_5GC_HEADER_LEN);
    ogs_pkbuf_put(pkbuf, 200);
    memset(pkbuf->data, 0, pkbuf->len);

    p = (uint8_t *)pkbuf->data;
    ip6_h = (struct ip6_hdr *)p;
    advert_h = (struct nd_router_advert *)((uint8_t *)ip6_h + sizeof *ip6_h);
    prefix = (struct nd_opt_prefix_info *)
        ((uint8_t*)advert_h + sizeof *advert_h);

    advert_h->nd_ra_type = ND_ROUTER_ADVERT;
    advert_h->nd_ra_code = 0;
    advert_h->nd_ra_curhoplimit = 64;
    advert_h->nd_ra_flags_reserved = 0;
    advert_h->nd_ra_router_lifetime = htobe16(64800);  /* 64800s */
    advert_h->nd_ra_reachable = 0;
    advert_h->nd_ra_retransmit = 0;

    prefix->nd_opt_pi_type = ND_OPT_PREFIX_INFORMATION;
    prefix->nd_opt_pi_len = 4; /* 32bytes */
    prefix->nd_opt_pi_prefix_len = OGS_IPV6_DEFAULT_PREFIX_LEN;
    prefix->nd_opt_pi_flags_reserved =
        ND_OPT_PI_FLAG_ONLINK|ND_OPT_PI_FLAG_AUTO;
    prefix->nd_opt_pi_valid_time = htobe32(0xffffffff); /* Infinite */
    prefix->nd_opt_pi_preferred_time = htobe32(0xffffffff); /* Infinite */
    memcpy(prefix->nd_opt_pi_prefix.s6_addr,
            ue_ip->addr, (OGS_IPV6_DEFAULT_PREFIX_LEN >> 3));

    /* For IPv6 Pseudo-Header */
    plen = sizeof *advert_h + sizeof *prefix;
    nxt = IPPROTO_ICMPV6;

    if (smf_self()->mtu) {
        struct nd_opt_mtu *mtu =
            (struct nd_opt_mtu *)((uint8_t*)prefix + sizeof *prefix);

        mtu->nd_opt_mtu_type = ND_OPT_MTU;
        mtu->nd_opt_mtu_len = 1; /* 8bytes */
        mtu->nd_opt_mtu_mtu = htobe32(smf_self()->mtu);

        plen += sizeof *mtu;
    }

    pkbuf->len = sizeof *ip6_h + plen;

    memcpy(p, src_ipsub.sub, sizeof src_ipsub.sub);
    p += sizeof src_ipsub.sub;
    memcpy(p, ip6_dst, OGS_IPV6_LEN);
    p += OGS_IPV6_LEN;
    p += 2; plen = htobe16(plen); memcpy(p, &plen, 2); p += 2;
    p += 3; *p = nxt; p += 1;

    advert_h->nd_ra_cksum = ogs_in_cksum((uint16_t *)pkbuf->data, pkbuf->len);

    ip6_h->ip6_flow = htobe32(0x60000001);
    ip6_h->ip6_plen = plen;
    ip6_h->ip6_nxt = nxt;  /* ICMPv6 */
    ip6_h->ip6_hlim = 0xff;
    memcpy(ip6_h->ip6_src.s6_addr, src_ipsub.sub, sizeof src_ipsub.sub);
    memcpy(ip6_h->ip6_dst.s6_addr, ip6_dst, OGS_IPV6_LEN);

    ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
        if (pdr->src_if == OGS_PFCP_INTERFACE_CP_FUNCTION && pdr->gnode) {
            ogs_gtp2_header_desc_t header_desc;
            ogs_pkbuf_t *newbuf = NULL;

            memset(&header_desc, 0, sizeof(header_desc));

            header_desc.type = OGS_GTPU_MSGTYPE_GPDU;
            header_desc.teid = pdr->f_teid.teid;

            newbuf = ogs_pkbuf_copy(pkbuf);
            ogs_assert(newbuf);

            ogs_gtp2_send_user_plane(pdr->gnode, &header_desc, newbuf);

            ogs_debug("      Send Router Advertisement");
            break;
        }
    }

    ogs_pkbuf_free(pkbuf);
}

static void bearer_timeout(ogs_gtp_xact_t *xact, void *data)
{
    smf_bearer_t *bearer = NULL;
    ogs_pool_id_t bearer_id = OGS_INVALID_POOL_ID;
    smf_sess_t *sess = NULL;
    smf_ue_t *smf_ue = NULL;
    uint8_t type = 0;

    ogs_assert(xact);
    type = xact->seq[0].type;

    ogs_assert(data);
    bearer_id = OGS_POINTER_TO_UINT(data);
    ogs_assert(bearer_id >= OGS_MIN_POOL_ID && bearer_id <= OGS_MAX_POOL_ID);

    bearer = smf_bearer_find_by_id(bearer_id);
    if (!bearer) {
        ogs_error("Bearer has already been removed [%d]", type);
        return;
    }

    sess = smf_sess_find_by_id(bearer->sess_id);
    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    switch (type) {
    case OGS_GTP2_DELETE_BEARER_REQUEST_TYPE:
        ogs_error("[%s] No Delete Bearer Response", smf_ue->imsi_bcd);
        ogs_assert(OGS_OK ==
            smf_epc_pfcp_send_one_bearer_modification_request(
                bearer, OGS_INVALID_POOL_ID, OGS_PFCP_MODIFY_REMOVE,
                OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
                OGS_GTP2_CAUSE_UNDEFINED_VALUE));
        break;
    default:
        ogs_error("GTP Timeout : IMSI[%s] Message-Type[%d]",
                smf_ue->imsi_bcd, type);
        break;
    }
}
