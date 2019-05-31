#include "gtp/gtp_types.h"
#include "gtp/gtp_node.h"
#include "gtp/gtp_path.h"

#include "sgw_context.h"
#include "sgw_event.h"
#include "sgw_gtp_path.h"

static void _gtpv2_c_recv_cb(short when, ogs_socket_t fd, void *data)
{
    sgw_event_t *e = NULL;
    int rv;
    gtp_header_t *gtp_h = NULL;
    uint32_t teid = 0;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(fd != INVALID_SOCKET);

    rv = gtp_recv(fd, &pkbuf);
    if (rv != OGS_OK)
    {
        ogs_log_message(OGS_LOG_WARN, ogs_socket_errno, "gtp_recv() failed");
        return;
    }

    gtp_h = (gtp_header_t *)pkbuf->data;
    ogs_assert(gtp_h);
    ogs_assert(gtp_h->teid_presence);
    teid = ntohl(gtp_h->teid);

    if (SGW_S5C_TEID(teid))
        e = sgw_event_new(SGW_EVT_S5C_MESSAGE);
    else
        e = sgw_event_new(SGW_EVT_S11_MESSAGE);
    ogs_assert(e);
    e->pkbuf = pkbuf;

    rv = ogs_queue_push(sgw_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_pkbuf_free(e->pkbuf);
        sgw_event_free(e);
    }
}

static void _gtpv1_u_recv_cb(short when, ogs_socket_t fd, void *data)
{
    char buf[OGS_ADDRSTRLEN];
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_sockaddr_t from;
    gtp_header_t *gtp_h = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *tunnel = NULL;
    uint32_t teid;
    int i;

    ogs_assert(fd != INVALID_SOCKET);

    rv = gtp_recvfrom(fd, &pkbuf, &from);
    if (rv != OGS_OK)
    {
        ogs_log_message(OGS_LOG_WARN, ogs_socket_errno, "gtp_recv() failed");
        return;
    }

    gtp_h = (gtp_header_t *)pkbuf->data;
    if (gtp_h->type == GTPU_MSGTYPE_ECHO_REQ)
    {
        ogs_pkbuf_t *echo_rsp;

        ogs_debug("[SGW] RECV Echo Request from [%s]",
                OGS_ADDR(&from, buf));
        echo_rsp = gtp_handle_echo_req(pkbuf);
        if (echo_rsp)
        {
            ssize_t sent;

            /* Echo reply */
            ogs_debug("[SGW] SEND Echo Response to [%s]",
                    OGS_ADDR(&from, buf));

            sent = ogs_sendto(fd, echo_rsp->data, echo_rsp->len, 0, &from);
            if (sent < 0 || sent != echo_rsp->len)
            {
                ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                        "ogs_sendto() failed");
            }
            ogs_pkbuf_free(echo_rsp);
        }
    }
    else if (gtp_h->type == GTPU_MSGTYPE_GPDU || 
                gtp_h->type == GTPU_MSGTYPE_END_MARKER)
    {
        teid = ntohl(gtp_h->teid);
        if (gtp_h->type == GTPU_MSGTYPE_GPDU)
            ogs_debug("[SGW] RECV GPU-U from [%s] : TEID[0x%x]",
                    OGS_ADDR(&from, buf), teid);
        else if (gtp_h->type == GTPU_MSGTYPE_END_MARKER)
            ogs_debug("[SGW] RECV End Marker from [%s] : TEID[0x%x]",
                    OGS_ADDR(&from, buf), teid);

        tunnel = sgw_tunnel_find_by_teid(teid);
        if (!tunnel)
        {
            if (gtp_h->type == GTPU_MSGTYPE_GPDU)
                ogs_warn("[SGW] RECV GPU-U from [%s] : No TEID[0x%x]",
                        OGS_ADDR(&from, buf), teid);
            else if (gtp_h->type == GTPU_MSGTYPE_END_MARKER)
                ogs_warn("[SGW] RECV End Marker from [%s] : No TEID[0x%x]",
                        OGS_ADDR(&from, buf), teid);
            ogs_pkbuf_free(pkbuf);
            return;
        }
        bearer = tunnel->bearer;
        ogs_assert(bearer);

        /* Convert TEID */
        if (tunnel->interface_type == GTP_F_TEID_S1_U_SGW_GTP_U)
        {
            sgw_tunnel_t *s5u_tunnel = NULL;

            s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
            ogs_assert(s5u_tunnel);
            ogs_assert(s5u_tunnel->gnode);
            ogs_assert(s5u_tunnel->gnode->sock);
            ogs_debug("[SGW] SEND GPU-U to PGW[%s]: TEID[0x%x]",
                OGS_ADDR(&s5u_tunnel->gnode->conn, buf),
                s5u_tunnel->remote_teid);

            gtp_h->teid = htonl(s5u_tunnel->remote_teid);
            gtp_sendto(s5u_tunnel->gnode, pkbuf);
        }
        else if (tunnel->interface_type ==
                    GTP_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING ||
                tunnel->interface_type ==
                    GTP_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING)
        {
            sgw_tunnel_t *indirect_tunnel = NULL;

            indirect_tunnel = sgw_tunnel_find_by_interface_type(bearer,
                    tunnel->interface_type);
            ogs_assert(indirect_tunnel);
            ogs_assert(indirect_tunnel->gnode);
            ogs_assert(indirect_tunnel->gnode->sock);
            ogs_debug("[SGW] SEND GPU-U to Indirect Tunnel[%s]: TEID[0x%x]",
                OGS_ADDR(&indirect_tunnel->gnode->conn, buf),
                indirect_tunnel->remote_teid);

            gtp_h->teid = htonl(indirect_tunnel->remote_teid);
            gtp_sendto(indirect_tunnel->gnode, pkbuf);
        }
        else if (tunnel->interface_type == GTP_F_TEID_S5_S8_SGW_GTP_U)
        {
            sgw_tunnel_t *s1u_tunnel = NULL;

            s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
            ogs_assert(s1u_tunnel);

            if (s1u_tunnel->remote_teid)
            {
                ogs_assert(s1u_tunnel->gnode);
                ogs_assert(s1u_tunnel->gnode->sock);
                ogs_debug("[SGW] SEND GPU-U to ENB[%s]: TEID[0x%x]",
                    OGS_ADDR(&s1u_tunnel->gnode->conn, buf),
                    s1u_tunnel->remote_teid);

                /* If there is buffered packet, send it first */
                for (i = 0; i < bearer->num_buffered_pkt; i++)
                {
                    gtp_header_t *gtp_h = NULL;

                    gtp_h = (gtp_header_t *)bearer->buffered_pkts[i]->data;
                    gtp_h->teid = htonl(s1u_tunnel->remote_teid);

                    gtp_sendto(s1u_tunnel->gnode, bearer->buffered_pkts[i]);
                    ogs_pkbuf_free(bearer->buffered_pkts[i]);
                }
                bearer->num_buffered_pkt = 0;

                gtp_h->teid = htonl(s1u_tunnel->remote_teid);
                gtp_sendto(s1u_tunnel->gnode, pkbuf);
            }
            else
            {
                /* S1U path is deactivated.
                 * Send downlink_data_notification to MME.
                 *
                 */
                sgw_ue_t *sgw_ue = NULL;

                ogs_assert(bearer->sess);
                ogs_assert(bearer->sess->sgw_ue);

                sgw_ue = bearer->sess->sgw_ue;

                ogs_debug("[SGW] S1U PATH deactivated : STATE[0x%x]",
                        SGW_GET_UE_STATE(sgw_ue));
                if ((SGW_GET_UE_STATE(sgw_ue) & SGW_S1U_INACTIVE))
                {
                    ogs_debug("    SGW-S1U Inactive");
                    if (!(SGW_GET_UE_STATE(sgw_ue) & SGW_DL_NOTI_SENT))
                    {
                        sgw_event_t *e;

                        ogs_debug("    EVENT DL Data Notification");
                        e = sgw_event_new(SGW_EVT_LO_DLDATA_NOTI);
                        ogs_assert(e);
                        e->bearer = bearer;
                        rv = ogs_queue_push(sgw_self()->queue, e);
                        if (rv != OGS_OK) {
                            ogs_error("ogs_queue_push() failed:%d", (int)rv);
                            sgw_event_free(e);
                        }

                        SGW_SET_UE_STATE(sgw_ue, SGW_DL_NOTI_SENT);
                    }

                    /* Buffer the packet */
                    if (bearer->num_buffered_pkt < MAX_NUM_BUFFER_PKT)
                    {
                        bearer->buffered_pkts[bearer->num_buffered_pkt++] = 
                            pkbuf;
                        return;
                    }
                }
                else
                {
                    /* UE is S1U_ACTIVE state but there is no s1u teid */
                    ogs_debug("[SGW] UE is ACITVE but there is no matched "
                            "ENB_S1U_TEID[%d]", teid);

                    /* Just drop it */
                }
            }
        }
    }

    ogs_pkbuf_free(pkbuf);
    return;
}

int sgw_gtp_open()
{
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    ogs_list_for_each(&sgw_self()->gtpc_list, node)
    {
        ogs_socknode_set_poll(node, sgw_self()->pollset,
                OGS_POLLIN, _gtpv2_c_recv_cb, NULL);

        sock = gtp_server(node);
        ogs_assert(sock);
    }
    ogs_list_for_each(&sgw_self()->gtpc_list6, node)
    {
        ogs_socknode_set_poll(node, sgw_self()->pollset,
                OGS_POLLIN, _gtpv2_c_recv_cb, NULL);

        sock = gtp_server(node);
        ogs_assert(sock);
    }

    sgw_self()->gtpc_sock = gtp_local_sock_first(&sgw_self()->gtpc_list);
    sgw_self()->gtpc_sock6 = gtp_local_sock_first(&sgw_self()->gtpc_list6);
    sgw_self()->gtpc_addr = gtp_local_addr_first(&sgw_self()->gtpc_list);
    sgw_self()->gtpc_addr6 = gtp_local_addr_first(&sgw_self()->gtpc_list6);

    ogs_assert(sgw_self()->gtpc_addr || sgw_self()->gtpc_addr6);

    ogs_list_for_each(&sgw_self()->gtpu_list, node)
    {
        ogs_socknode_set_poll(node, sgw_self()->pollset,
                OGS_POLLIN, _gtpv1_u_recv_cb, NULL);

        sock = gtp_server(node);
        ogs_assert(sock);
    }
    ogs_list_for_each(&sgw_self()->gtpu_list6, node)
    {
        ogs_socknode_set_poll(node, sgw_self()->pollset,
                OGS_POLLIN, _gtpv1_u_recv_cb, NULL);

        sock = gtp_server(node);
        ogs_assert(sock);
    }

    sgw_self()->gtpu_sock = gtp_local_sock_first(&sgw_self()->gtpu_list);
    sgw_self()->gtpu_sock6 = gtp_local_sock_first(&sgw_self()->gtpu_list6);
    sgw_self()->gtpu_addr = gtp_local_addr_first(&sgw_self()->gtpu_list);
    sgw_self()->gtpu_addr6 = gtp_local_addr_first(&sgw_self()->gtpu_list6);

    ogs_assert(sgw_self()->gtpu_addr || sgw_self()->gtpu_addr6);

    return OGS_OK;
}

void sgw_gtp_close()
{
    ogs_socknode_remove_all(&sgw_self()->gtpc_list);
    ogs_socknode_remove_all(&sgw_self()->gtpc_list6);
    ogs_socknode_remove_all(&sgw_self()->gtpu_list);
    ogs_socknode_remove_all(&sgw_self()->gtpu_list6);
}

int sgw_gtp_send_end_marker(sgw_tunnel_t *s1u_tunnel)
{
    char buf[OGS_ADDRSTRLEN];
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    gtp_header_t *h = NULL;

    ogs_assert(s1u_tunnel);
    ogs_assert(s1u_tunnel->gnode);
    ogs_assert(s1u_tunnel->gnode->sock);

    ogs_debug("[SGW] SEND End Marker to ENB[%s]: TEID[0x%x]",
        OGS_ADDR(&s1u_tunnel->gnode->conn, buf),
        s1u_tunnel->remote_teid);

    pkbuf = ogs_pkbuf_alloc(NULL,
            100 /* enough for END_MARKER; use smaller buffer */);
    ogs_pkbuf_put(pkbuf, 100);
    h = (gtp_header_t *)pkbuf->data;

    memset(h, 0, GTPV1U_HEADER_LEN);

    /*
     * Flags
     * 0x20 - Version : GTP release 99 version (1)
     * 0x10 - Protocol Type : GTP (1)
     */
    h->flags = 0x30;
    h->type = GTPU_MSGTYPE_END_MARKER;
    h->teid =  htonl(s1u_tunnel->remote_teid);
    
    rv = gtp_sendto(s1u_tunnel->gnode, pkbuf);
    ogs_assert(rv == OGS_OK);
    ogs_pkbuf_free(pkbuf);

    return rv;
}
