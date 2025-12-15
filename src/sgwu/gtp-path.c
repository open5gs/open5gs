/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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
    char buf1[OGS_ADDRSTRLEN];
    char buf2[OGS_ADDRSTRLEN];

    sgwu_sess_t *sess = NULL;

    ogs_pkbuf_t *pkbuf = NULL;
    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t from;

    ogs_gtp2_header_t *gtp_h = NULL;
    ogs_gtp2_header_desc_t header_desc;
    ogs_pfcp_user_plane_report_t report;

    ogs_assert(fd != INVALID_SOCKET);
    sock = data;
    ogs_assert(sock);

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

        ogs_debug("[RECV] Echo Request from [%s]", OGS_ADDR(&from, buf1));
        echo_rsp = ogs_gtp2_handle_echo_req(pkbuf);
        ogs_expect(echo_rsp);
        if (echo_rsp) {
            ssize_t sent;

            /* Echo reply */
            ogs_debug("[SEND] Echo Response to [%s]", OGS_ADDR(&from, buf1));

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

    ogs_trace("[RECV] GPU-U Type [%d] from [%s] : TEID[0x%x]",
            header_desc.type, OGS_ADDR(&from, buf1), header_desc.teid);

    /* Remove GTP header and send packets to peer NF */
    ogs_assert(ogs_pkbuf_pull(pkbuf, len));

    if (header_desc.type == OGS_GTPU_MSGTYPE_END_MARKER) {
        ogs_pfcp_object_t *pfcp_object = NULL;
        ogs_pfcp_pdr_t *pdr = NULL;
        ogs_gtp2_header_desc_t sendhdr;
        ogs_pkbuf_t *sendbuf = NULL;

        pfcp_object = ogs_pfcp_object_find_by_teid(header_desc.teid);
        if (!pfcp_object) {
            /*
             * Refer to the following 5G standard
             *
             * TS23.527 Restoration procedures
             * 4.3 UPF Restoration Procedures
             * 4.3.2 Restoration Procedure for PSA UPF Restart
             *
             * The UPF shall not send GTP-U Error indication message
             * for a configurable period after an UPF restart
             * when the UPF receives a G-PDU not matching any PDRs.
             */
            if (ogs_time_ntp32_now() >
                   (ogs_pfcp_self()->local_recovery +
                    ogs_time_sec(ogs_local_conf()->time.message.pfcp.
                        association_interval))) {
                ogs_error("[%s] Send Error Indication [TEID:0x%x] to [%s]",
                        OGS_ADDR(&sock->local_addr, buf1),
                        header_desc.teid,
                        OGS_ADDR(&from, buf2));
                ogs_gtp1_send_error_indication(
                        sock, header_desc.teid, 0, &from);
            }
            goto cleanup;
        }

        switch(pfcp_object->type) {
        case OGS_PFCP_OBJ_PDR_TYPE:
            pdr = (ogs_pfcp_pdr_t *)pfcp_object;
            ogs_assert(pdr);
            break;
        default:
            ogs_fatal("Unknown type [%d]", pfcp_object->type);
            ogs_assert_if_reached();
        }

        ogs_assert(pdr);

        sendbuf = ogs_pkbuf_copy(pkbuf);
        ogs_assert(sendbuf);

        /* Forward packet */
        memset(&sendhdr, 0, sizeof(sendhdr));
        sendhdr.type = header_desc.type;

        ogs_pfcp_send_g_pdu(pdr, &sendhdr, sendbuf);

    } else if (header_desc.type == OGS_GTPU_MSGTYPE_ERR_IND) {
        ogs_pfcp_far_t *far = NULL;

        far = ogs_pfcp_far_find_by_gtpu_error_indication(pkbuf);
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
    } else if (header_desc.type == OGS_GTPU_MSGTYPE_GPDU) {
        struct ip *ip_h = NULL;
        ogs_pfcp_object_t *pfcp_object = NULL;
        ogs_pfcp_sess_t *pfcp_sess = NULL;
        ogs_pfcp_pdr_t *pdr = NULL;

        ip_h = (struct ip *)pkbuf->data;
        ogs_assert(ip_h);

        pfcp_object = ogs_pfcp_object_find_by_teid(header_desc.teid);
        if (!pfcp_object) {
            /*
             * Refer to the following 5G standard
             *
             * TS23.527 Restoration procedures
             * 4.3 UPF Restoration Procedures
             * 4.3.2 Restoration Procedure for PSA UPF Restart
             *
             * The UPF shall not send GTP-U Error indication message
             * for a configurable period after an UPF restart
             * when the UPF receives a G-PDU not matching any PDRs.
             */
            if (ogs_time_ntp32_now() >
                   (ogs_pfcp_self()->local_recovery +
                    ogs_time_sec(ogs_local_conf()->time.message.pfcp.
                        association_interval))) {
                ogs_error("[%s] Send Error Indication [TEID:0x%x] to [%s]",
                        OGS_ADDR(&sock->local_addr, buf1),
                        header_desc.teid,
                        OGS_ADDR(&from, buf2));
                ogs_gtp1_send_error_indication(
                        sock, header_desc.teid, 0, &from);
            }
            goto cleanup;
        }

        switch(pfcp_object->type) {
        case OGS_PFCP_OBJ_PDR_TYPE:
            pdr = (ogs_pfcp_pdr_t *)pfcp_object;
            ogs_assert(pdr);
            break;
        case OGS_PFCP_OBJ_SESS_TYPE:
            /* SGWU does not use SESS TYPE */
            ogs_assert_if_reached();
            pfcp_sess = (ogs_pfcp_sess_t *)pfcp_object;
            ogs_assert(pfcp_sess);

            ogs_list_for_each(&pfcp_sess->pdr_list, pdr) {
                /* Check if TEID */
                if (header_desc.teid != pdr->f_teid.teid)
                    continue;

                /* Check if QFI */
                if (pdr->qfi && pdr->qfi != header_desc.qos_flow_identifier)
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
        ogs_assert(true == ogs_pfcp_up_handle_pdr(
                    pdr, header_desc.type, &header_desc, pkbuf, &report));

        if (report.type.downlink_data_report) {
            ogs_assert(pdr->sess);
            ogs_assert(pdr->sess->obj.type == OGS_PFCP_OBJ_SESS_TYPE);
            sess = SGWU_SESS(pdr->sess);
            ogs_assert(sess);

            report.downlink_data.pdr_id = pdr->id;
            report.downlink_data.qfi =
                header_desc.qos_flow_identifier; /* for 5GC */

            ogs_assert(OGS_OK ==
                sgwu_pfcp_send_session_report_request(sess, &report));
        }

        /*
         * The ogs_pfcp_up_handle_pdr() function
         * buffers or frees the Packet Buffer(pkbuf) memory.
         */
        return;
    } else {
        ogs_error("[DROP] Invalid GTPU Type [%d]", header_desc.type);
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

#if OGS_USE_TALLOC == 1
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
