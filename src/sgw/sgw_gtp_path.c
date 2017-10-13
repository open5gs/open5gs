#define TRACE_MODULE _sgw_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "types.h"
#include "gtp_types.h"
#include "gtp_path.h"

#include "sgw_context.h"
#include "sgw_event.h"
#include "sgw_gtp_path.h"

static int _gtpc_recv_cb(net_sock_t *sock, void *data)
{
    event_t e;
    status_t rv;
    gtp_header_t *gtp_h = NULL;
    pkbuf_t *pkbuf = NULL;
    c_uint32_t addr = 0;
    c_uint16_t port = 0;
    sgw_mme_t *mme = NULL;

    d_assert(sock, return -1, "Null param");

    pkbuf = gtp_read(sock);
    if (pkbuf == NULL)
    {
        if (sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    gtp_h = (gtp_header_t *)pkbuf->payload;
    d_assert(gtp_h, return -1, "Null param");

    addr = sock->remote.sin_addr.s_addr;
    port = ntohs(sock->remote.sin_port);

    mme = sgw_mme_find(addr, port);
    if (!mme && gtp_h->teid == 0)
    {
        mme = sgw_mme_add();
        d_assert(mme, return -1, "Can't add MME-GTP node");

        mme->addr = addr;
        mme->port = port;
        mme->sock = sock;
    }

    if (mme)
    {
        d_trace(10, "S11 PDU received from MME\n");

        event_set(&e, SGW_EVT_S11_MESSAGE);
        event_set_param1(&e, (c_uintptr_t)mme);
        event_set_param2(&e, (c_uintptr_t)pkbuf);
    
    }
    else
    {
        d_trace(10, "S5C PDU received from PGW\n");

        event_set(&e, SGW_EVT_S5C_MESSAGE);
        event_set_param1(&e, (c_uintptr_t)addr);
        event_set_param2(&e, (c_uintptr_t)port);
        event_set_param3(&e, (c_uintptr_t)pkbuf);
    }

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

static int _gtpu_recv_cb(net_sock_t *sock, void *data)
{
    pkbuf_t *pkbuf = NULL;
    gtp_node_t gnode;
    gtp_header_t *gtp_h = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *tunnel = NULL;
    c_uint32_t teid;
    int i;

    d_assert(sock, return -1, "Null param");

    pkbuf = gtp_read(sock);
    if (pkbuf == NULL)
    {
        if (sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(50, "S1-U PDU received from ENB\n");
    d_trace_hex(50, pkbuf->payload, pkbuf->len);

    gtp_h = (gtp_header_t *)pkbuf->payload;
    if (gtp_h->type == GTPU_MSGTYPE_ECHO_REQ)
    {
        pkbuf_t *echo_rsp;

        d_trace(3, "Received echo-req\n");
        echo_rsp = gtp_handle_echo_req(pkbuf);
        if (echo_rsp)
        {
            /* Echo reply */
            d_trace(3, "Send echo-rsp to peer\n");

            gnode.addr = sock->remote.sin_addr.s_addr;
            gnode.port = ntohs(sock->remote.sin_port);
            gnode.sock = sock;

            gtp_send(&gnode, echo_rsp);
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

        /* Convert TEID */
        gtp_h->teid = htonl(tunnel->remote_teid);
        
        gnode.addr = tunnel->remote_addr;
        gnode.port = GTPV1_U_UDP_PORT;
        gnode.sock = sgw_self()->gtpu_sock;
            
        if (tunnel->interface_type == 
                GTP_F_TEID_S1_U_SGW_GTP_U ||
            tunnel->interface_type ==
                GTP_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING ||
            tunnel->interface_type ==
                GTP_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING)
        {
            d_trace(50, "Recv GPDU (teid = 0x%x) from eNB\n", teid);

            gtp_send(&gnode, pkbuf);
        }
        else if (tunnel->interface_type == GTP_F_TEID_S5_S8_SGW_GTP_U)
        {
            d_trace(50, "Recv GPDU (teid = 0x%x) from PGW\n", teid);

            bearer = tunnel->bearer;
            d_assert(bearer, return -1, "Null param");

            if (tunnel->remote_teid)
            {
                /* If there is buffered packet, send it first */
                for (i = 0; i < bearer->num_buffered_pkt; i++)
                {
                    gtp_h = (gtp_header_t *)bearer->buffered_pkts[i]->payload;
                    gtp_h->teid =  htonl(tunnel->remote_teid);

                    gtp_send(&gnode, bearer->buffered_pkts[i]);
                    pkbuf_free(bearer->buffered_pkts[i]);
                }
                bearer->num_buffered_pkt = 0;

                gtp_send(&gnode, pkbuf);
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


#if 0
static int _gtpv1_s5u_recv_cb(net_sock_t *sock, void *data)
{
    pkbuf_t *pkbuf = NULL;
    gtp_node_t gnode;
    gtp_header_t *gtp_h = NULL;
    sgw_bearer_t *bearer = NULL;
    c_uint32_t teid;

    d_assert(sock, return -1, "Null param");

    pkbuf = gtp_read(sock);
    if (pkbuf == NULL)
    {
        if (sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(50, "S5-U PDU received from PGW\n");
    d_trace_hex(50, pkbuf->payload, pkbuf->len);

    gtp_h = (gtp_header_t *)pkbuf->payload;
    if (gtp_h->type == GTPU_MSGTYPE_ECHO_REQ)
    {
        pkbuf_t *echo_rsp;

        d_trace(3, "Received echo-req");
        echo_rsp = gtp_handle_echo_req(pkbuf);
        if (echo_rsp)
        {
            /* Echo reply */
            d_trace(3, "Send echo-rsp to peer(PGW) ");

            gnode.addr = sock->remote.sin_addr.s_addr;
            gnode.port = GTPV1_U_UDP_PORT;
            gnode.sock = sgw_self()->s5u_sock;

            gtp_send(&gnode, echo_rsp);
            pkbuf_free(echo_rsp);
        }
    }
    else if (gtp_h->type == GTPU_MSGTYPE_GPDU)
    {
        teid = ntohl(gtp_h->teid);

        d_trace(50, "Recv GPDU (teid = 0x%x)",teid);

        bearer = sgw_bearer_find_by_sgw_s5u_teid(teid);
        if (bearer)
        {
            if (bearer->enb_s1u_teid)
            {
                /* Convert Teid and send to enodeB  via s1u */
                gtp_h->teid =  htonl(bearer->enb_s1u_teid);
                
                gnode.addr = bearer->enb_s1u_addr;
                gnode.port = GTPV1_U_UDP_PORT;
                gnode.sock = sgw_self()->s1u_sock;

                /* If there is buffered packet, send it first */
                if (bearer->num_buffered_pkt)
                {
                    int i;
                    for (i = 0; i < bearer->num_buffered_pkt; i++)
                    {
                        gtp_h = 
                            (gtp_header_t *)bearer->buffered_pkts[i]->payload;
                        gtp_h->teid =  htonl(bearer->enb_s1u_teid);

                        gtp_send(&gnode, bearer->buffered_pkts[i]);
                        pkbuf_free(bearer->buffered_pkts[i]);
                    }
                    bearer->num_buffered_pkt = 0;
                }

                gtp_send(&gnode, pkbuf);
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

static int _gtpv1_s1u_recv_cb(net_sock_t *sock, void *data)
{
    pkbuf_t *pkbuf = NULL;
    gtp_node_t gnode;
    gtp_header_t *gtp_h = NULL;
    sgw_tunnel_t *tunnel = NULL;
    c_uint32_t teid;

    d_assert(sock, return -1, "Null param");

    pkbuf = gtp_read(sock);
    if (pkbuf == NULL)
    {
        if (sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(50, "S1-U PDU received from ENB\n");
    d_trace_hex(50, pkbuf->payload, pkbuf->len);

    gtp_h = (gtp_header_t *)pkbuf->payload;
    if (gtp_h->type == GTPU_MSGTYPE_ECHO_REQ)
    {
        pkbuf_t *echo_rsp;

        d_trace(3, "Received echo-req\n");
        echo_rsp = gtp_handle_echo_req(pkbuf);
        if (echo_rsp)
        {
            /* Echo reply */
            d_trace(3, "Send echo-rsp to peer(ENB)\n");

            gnode.addr = sock->remote.sin_addr.s_addr;
            gnode.port = GTPV1_U_UDP_PORT;
            gnode.sock = sgw_self()->s1u_sock;

            gtp_send(&gnode, echo_rsp);
            pkbuf_free(echo_rsp);
        }
    }
    else if (gtp_h->type == GTPU_MSGTYPE_GPDU || 
                gtp_h->type == GTPU_MSGTYPE_END_MARKER)
    {
        teid = ntohl(gtp_h->teid);
        d_trace(50, "Recv GPDU (teid = 0x%x) from ENB\n",teid);

        tunnel = sgw_tunnel_find_by_teid(teid);
        if (tunnel)
        {
            gtp_h->teid =  htonl(tunnel->remote_teid);
            
            gnode.addr = tunnel->remote_addr;
            gnode.port = GTPV1_U_UDP_PORT;
            if (tunnel->interface_type == GTP_F_TEID_S1_U_SGW_GTP_U)
                gnode.sock = sgw_self()->s5u_sock;
            else if (tunnel->interface_type ==
                    GTP_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING)
                gnode.sock = sgw_self()->s1u_sock;
            else if (tunnel->interface_type ==
                    GTP_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING)
                gnode.sock = sgw_self()->s1u_sock;
            else
                d_assert(0, return -1, "Invalid type(%d)",
                        tunnel->interface_type);

            gtp_send(&gnode, pkbuf);
        }
    }

    pkbuf_free(pkbuf);
    return 0;
}
#endif

status_t sgw_gtp_open()
{
    status_t rv;

    rv = gtp_listen(&sgw_self()->gtpc_sock, _gtpc_recv_cb, 
            sgw_self()->gtpc_addr, sgw_self()->gtpc_port, NULL);
    if (rv != CORE_OK)
    {
        d_error("Can't establish GTP-C Path for MME/PGW");
        return rv;
    }

    rv = gtp_listen(&sgw_self()->gtpu_sock, _gtpu_recv_cb, 
            sgw_self()->gtpu_addr, sgw_self()->gtpu_port, NULL);
    if (rv != CORE_OK)
    {
        d_error("Can't establish GTP-U Path for eNB/SGW");
        return rv;
    }

    return CORE_OK;
}

status_t sgw_gtp_close()
{
    status_t rv;

    rv = gtp_close(sgw_self()->gtpc_sock);
    if (rv != CORE_OK)
    {
        d_error("Can't close GTP-C Path for MME/PGW");
        return rv;
    }

    rv = gtp_close(sgw_self()->gtpu_sock);
    if (rv != CORE_OK)
    {
        d_error("Can't close GTP-U Path for eNB/PGW");
        return rv;
    }

    return CORE_OK;
}

status_t sgw_gtp_send_end_marker(sgw_bearer_t *bearer)
{
    status_t rv;
    pkbuf_t *pkbuf = NULL;
    gtp_header_t *h = NULL;
    gtp_node_t gnode;
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
    
    gnode.addr = s1u_tunnel->remote_addr;
    gnode.port = GTPV1_U_UDP_PORT;
    gnode.sock = sgw_self()->gtpu_sock;

    rv = gtp_send(&gnode, pkbuf);
    d_assert(rv == CORE_OK, , "gtp send failed");
    pkbuf_free(pkbuf);

    return rv;
}
