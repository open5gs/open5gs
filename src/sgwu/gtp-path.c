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

#include "gtp-path.h"
#include "pfcp-path.h"

#define SGWU_GTP_HANDLED     1

static ogs_pkbuf_pool_t *packet_pool = NULL;

static void _gtpv1_u_recv_cb(short when, ogs_socket_t fd, void *data)
{
    int len;
    ssize_t size;
    char buf[OGS_ADDRSTRLEN];

    sgwu_sess_t *sess = NULL;

    ogs_pkbuf_t *pkbuf = NULL;
    ogs_sockaddr_t from;

    ogs_gtp_header_t *gtp_h = NULL;
    ogs_pfcp_user_plane_report_t report;

    uint32_t teid;
    uint8_t qfi;

    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(packet_pool, OGS_MAX_PKT_LEN);
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

    /* Remove GTP header and send packets to peer NF */
    len = ogs_gtpu_header_len(pkbuf);
    if (len < 0) {
        ogs_error("[DROP] Cannot decode GTPU packet");
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
            ogs_pfcp_up_handle_error_indication(far, &report);

            if (report.type.error_indication_report) {
                ogs_assert(far->sess);
                sess = SGWU_SESS(far->sess);
                ogs_assert(sess);

                sgwu_pfcp_send_session_report_request(sess, &report);
            }

        } else {
            ogs_error("[DROP] Cannot decode Error-Indication");
            ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        }
    } else if (gtp_h->type == OGS_GTPU_MSGTYPE_GPDU) {
        struct ip *ip_h = NULL;
        ogs_pfcp_pdr_t *pdr = NULL;

        ip_h = (struct ip *)pkbuf->data;
        ogs_assert(ip_h);

        pdr = ogs_pfcp_pdr_find_by_teid_and_qfi(teid, qfi);
        if (pdr) {
            ogs_pfcp_up_handle_pdr(pdr, pkbuf, &report);

            if (report.type.downlink_data_report) {
                ogs_assert(pdr->sess);
                sess = SGWU_SESS(pdr->sess);
                ogs_assert(sess);

                report.downlink_data.pdr_id = pdr->id;
                report.downlink_data.qfi = qfi; /* for 5GC */

                sgwu_pfcp_send_session_report_request(sess, &report);
            }
        }
    } else {
        ogs_error("[DROP] Invalid GTPU Type [%d]", gtp_h->type);
        ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
    }

cleanup:
    ogs_pkbuf_free(pkbuf);
}

int sgwu_gtp_init(void)
{
    ogs_pkbuf_config_t config;
    memset(&config, 0, sizeof config);

    config.cluster_2048_pool = ogs_app()->pool.packet;

    packet_pool = ogs_pkbuf_pool_create(&config);

    return OGS_OK;
}

void sgwu_gtp_final(void)
{
    ogs_pkbuf_pool_destroy(packet_pool);
}

int sgwu_gtp_open(void)
{
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    ogs_list_for_each(&sgwu_self()->gtpu_list, node) {
        sock = ogs_gtp_server(node);
        ogs_assert(sock);

        if (sock->family == AF_INET)
            sgwu_self()->gtpu_sock = sock;
        else if (sock->family == AF_INET6)
            sgwu_self()->gtpu_sock6 = sock;

        node->poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, sock->fd, _gtpv1_u_recv_cb, sock);
    }

    ogs_assert(sgwu_self()->gtpu_sock || sgwu_self()->gtpu_sock6);

    return OGS_OK;
}

void sgwu_gtp_close(void)
{
    ogs_socknode_remove_all(&sgwu_self()->gtpu_list);
}
