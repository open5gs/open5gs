#define TRACE_MODULE _sgw_path
#include "core_debug.h"
#include "core_pkbuf.h"

#include "types.h"
#include "gtp_types.h"
#include "gtp_node.h"
#include "gtp_path.h"

#include "sgw_context.h"
#include "sgw_event.h"
#include "sgw_gtp_path.h"

static int _gtpv2_c_recv_cb(sock_id sock, void *data)
{
    event_t e;
    status_t rv;
    gtp_header_t *gtp_h = NULL;
    c_uint32_t teid = 0;
    pkbuf_t *pkbuf = NULL;

    d_assert(sock, return -1, "Null param");

    rv = gtp_recv(sock, &pkbuf);
    if (rv != CORE_OK)
    {
        if (errno == EAGAIN)
            return 0;

        return -1;
    }

    gtp_h = (gtp_header_t *)pkbuf->payload;
    d_assert(gtp_h, return -1, "Null param");
    d_assert(gtp_h->teid_presence, return -1,);
    teid = ntohl(gtp_h->teid);

    if (SGW_S5C_TEID(teid))
    {
        d_trace(10, "S5C PDU received from PGW\n");
        event_set(&e, SGW_EVT_S5C_MESSAGE);
    }
    else
    {
        d_trace(10, "S11 PDU received from MME\n");
        event_set(&e, SGW_EVT_S11_MESSAGE);
    }
    event_set_param1(&e, (c_uintptr_t)pkbuf);

    d_trace_hex(10, pkbuf->payload, pkbuf->len);

    rv = sgw_event_send(&e);
    if (rv != CORE_OK)
    {
        d_error("sgw_event_send error");
        pkbuf_free(pkbuf);
        return -1;
    }

    return 0;
}

static int _gtpv1_u_recv_cb(sock_id sock, void *data)
{
    status_t rv;
    pkbuf_t *pkbuf = NULL;
    c_sockaddr_t from;
    gtp_header_t *gtp_h = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *tunnel = NULL;
    c_uint32_t teid;
    int i;

    d_assert(sock, return -1, "Null param");

    rv = gtp_recvfrom(sock, &pkbuf, &from);
    if (rv != CORE_OK)
    {
        if (errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(50, "GTP-U PDU received\n");
    d_trace_hex(50, pkbuf->payload, pkbuf->len);

    gtp_h = (gtp_header_t *)pkbuf->payload;
    if (gtp_h->type == GTPU_MSGTYPE_ECHO_REQ)
    {
        pkbuf_t *echo_rsp;

        d_trace(3, "Received echo-req\n");
        echo_rsp = gtp_handle_echo_req(pkbuf);
        if (echo_rsp)
        {
            ssize_t sent;

            /* Echo reply */
            d_trace(3, "Send echo-rsp to peer\n");

            sent = core_sendto(sock,
                    echo_rsp->payload, echo_rsp->len, 0, &from);
            if (sent < 0 || sent != pkbuf->len)
            {
                d_error("core_sendto failed(%d:%s)", errno, strerror(errno));
            }
            pkbuf_free(echo_rsp);
        }
    }
    else if (gtp_h->type == GTPU_MSGTYPE_GPDU || 
                gtp_h->type == GTPU_MSGTYPE_END_MARKER)
    {
        teid = ntohl(gtp_h->teid);
        d_trace(50, "Recv GPDU (teid = 0x%x)\n", teid);

        tunnel = sgw_tunnel_find_by_teid(teid);
        d_assert(tunnel, return -1, "No TEID(0x%x)", teid);
        bearer = tunnel->bearer;
        d_assert(bearer, return -1, "Null param");

        /* Convert TEID */
        if (tunnel->interface_type == 
                GTP_F_TEID_S1_U_SGW_GTP_U ||
            tunnel->interface_type ==
                GTP_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING ||
            tunnel->interface_type ==
                GTP_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING)
        {
            sgw_tunnel_t *s5u_tunnel = NULL;
            d_trace(50, "Recv GPDU (teid = 0x%x) from eNB\n", teid);

            s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
            d_assert(s5u_tunnel, return -1, "Null param");
            gtp_h->teid = htonl(s5u_tunnel->remote_teid);
            gtp_send(s5u_tunnel->gnode, pkbuf);
        }
        else if (tunnel->interface_type == GTP_F_TEID_S5_S8_SGW_GTP_U)
        {
            sgw_tunnel_t *s1u_tunnel = NULL;
            d_trace(50, "Recv GPDU (teid = 0x%x) from PGW\n", teid);

            s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
            d_assert(s1u_tunnel, return -1, "Null param");
            if (s1u_tunnel->remote_teid)
            {
                /* If there is buffered packet, send it first */
                for (i = 0; i < bearer->num_buffered_pkt; i++)
                {
                    gtp_h = (gtp_header_t *)bearer->buffered_pkts[i]->payload;
                    gtp_h->teid =  htonl(s1u_tunnel->remote_teid);

                    gtp_send(s1u_tunnel->gnode, bearer->buffered_pkts[i]);
                    pkbuf_free(bearer->buffered_pkts[i]);
                }
                bearer->num_buffered_pkt = 0;

                gtp_h->teid = htonl(s1u_tunnel->remote_teid);
                gtp_send(s1u_tunnel->gnode, pkbuf);
            }
            else
            {
                /* S1U path is deactivated.
                 * Send downlink_data_notification to MME.
                 *
                 */
                sgw_ue_t *sgw_ue = NULL;

                d_assert(bearer->sess, pkbuf_free(pkbuf); return 0,
                        "Session is NULL");
                d_assert(bearer->sess->sgw_ue, pkbuf_free(pkbuf); return 0,
                        "SGW_UE  is NULL");

                sgw_ue = bearer->sess->sgw_ue;

                if ((SGW_GET_UE_STATE(sgw_ue) & SGW_S1U_INACTIVE))
                {
                    if ( !(SGW_GET_UE_STATE(sgw_ue) & SGW_DL_NOTI_SENT))
                    {
                        event_t e;
                        status_t rv;

                        event_set(&e, SGW_EVT_LO_DLDATA_NOTI);
                        event_set_param1(&e, (c_uintptr_t)bearer->index);
                        rv = sgw_event_send(&e);
                        if (rv != CORE_OK)
                        {
                            d_error("sgw_event_send error");
                            pkbuf_free(pkbuf);
                            return -1;
                        }

                        SGW_SET_UE_STATE(sgw_ue, SGW_DL_NOTI_SENT);
                    }

                    /* Buffer the packet */
                    if (bearer->num_buffered_pkt < MAX_NUM_BUFFER_PKT)
                    {
                        bearer->buffered_pkts[bearer->num_buffered_pkt++] = 
                            pkbuf;
                        return 0;
                    }
                }
                else
                {
                    /* UE is S1U_ACTIVE state but there is no s1u teid */
                    d_warn("UE is ACITVE but there is no matched "
                            "s1u_teid(tedid = 0x%x)",teid);

                    /* Just drop it */
                }
            }
        }
    }

    pkbuf_free(pkbuf);
    return 0;
}

status_t sgw_gtp_open()
{
    status_t rv;

    rv = gtp_server_list(&sgw_self()->gtpc_list, _gtpv2_c_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = gtp_server_list(&sgw_self()->gtpc_list6, _gtpv2_c_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    sgw_self()->gtpc_addr = gtp_local_addr_first(&sgw_self()->gtpc_list);
    sgw_self()->gtpc_addr6 = gtp_local_addr_first(&sgw_self()->gtpc_list6);

    d_assert(sgw_self()->gtpc_addr || sgw_self()->gtpc_addr6,
            return CORE_ERROR, "No GTP Server");

    rv = gtp_server_list(&sgw_self()->gtpu_list, _gtpv1_u_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = gtp_server_list(&sgw_self()->gtpu_list6, _gtpv1_u_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    sgw_self()->gtpu_addr = gtp_local_addr_first(&sgw_self()->gtpu_list);
    sgw_self()->gtpu_addr6 = gtp_local_addr_first(&sgw_self()->gtpu_list6);

    d_assert(sgw_self()->gtpu_addr || sgw_self()->gtpu_addr6,
            return CORE_ERROR, "No GTP Server");

    return CORE_OK;
}

status_t sgw_gtp_close()
{
    sock_delete_list(&sgw_self()->gtpc_list);
    sock_delete_list(&sgw_self()->gtpc_list6);

    sock_delete_list(&sgw_self()->gtpu_list);
    sock_delete_list(&sgw_self()->gtpu_list6);

    return CORE_OK;
}

status_t sgw_gtp_send_end_marker(sgw_bearer_t *bearer)
{
    status_t rv;
    pkbuf_t *pkbuf = NULL;
    gtp_header_t *h = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL;

    d_assert(bearer, return CORE_ERROR,);
    s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
    d_assert(s1u_tunnel, return CORE_ERROR,);

    pkbuf = pkbuf_alloc(0, 100 /* enough for END_MARKER; use smaller buffer */);
    d_assert(pkbuf, return CORE_ERROR,);
    h = (gtp_header_t *)pkbuf->payload;

    memset(h, 0, GTPV1U_HEADER_LEN);

    /*
     * Flags
     * 0x20 - Version : GTP release 99 version (1)
     * 0x10 - Protocol Type : GTP (1)
     */
    h->flags = 0x30;
    h->type = GTPU_MSGTYPE_END_MARKER;
    h->teid =  htonl(s1u_tunnel->remote_teid);
    
    rv = gtp_send(s1u_tunnel->gnode, pkbuf);
    d_assert(rv == CORE_OK, , "gtp send failed");
    pkbuf_free(pkbuf);

    return rv;
}
