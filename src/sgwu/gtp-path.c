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

    /* Remove GTP header and send packets to peer NF */
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
                sess = SGWU_SESS(far->sess);
                ogs_assert(sess);

                ogs_assert(OGS_OK ==
                    sgwu_pfcp_send_session_report_request(sess, &report));
            }

        } else {
            ogs_error("[DROP] Cannot find FAR by Error-Indication");
            ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
        }
    } else if (gtp_h->type == OGS_GTPU_MSGTYPE_GPDU) {
        struct ip *ip_h = NULL;
        ogs_pfcp_object_t *pfcp_object = NULL;
        ogs_pfcp_sess_t *pfcp_sess = NULL;
        ogs_pfcp_pdr_t *pdr = NULL;

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
        ogs_assert(true == ogs_pfcp_up_handle_pdr(pdr, pkbuf, &report));

        if (report.type.downlink_data_report) {
            ogs_assert(pdr->sess);
            ogs_assert(pdr->sess->obj.type == OGS_PFCP_OBJ_SESS_TYPE);
            sess = SGWU_SESS(pdr->sess);
            ogs_assert(sess);

            report.downlink_data.pdr_id = pdr->id;
            report.downlink_data.qfi = qfi; /* for 5GC */

            ogs_assert(OGS_OK ==
                sgwu_pfcp_send_session_report_request(sess, &report));
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

void sgwu_gtp_final(void)
{
    ogs_pkbuf_pool_destroy(packet_pool);
}

int sgwu_gtp_open(void)
{
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

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

    return OGS_OK;
}

void sgwu_gtp_close(void)
{
    ogs_socknode_remove_all(&ogs_gtp_self()->gtpu_list);
}
