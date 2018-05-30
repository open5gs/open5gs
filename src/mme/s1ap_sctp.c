#define TRACE_MODULE _s1ap_sctp

#include "core_debug.h"
#include "core_thread.h"

#if HAVE_NETINET_SCTP_H
#include <netinet/sctp.h>
#endif

#include "mme_event.h"

#include "s1ap_path.h"

static int s1ap_accept_handler(sock_id sock, void *data);

status_t s1ap_init(int sctp_streams, c_uint16_t port)
{
    sctp_set_num_ostreams(sctp_streams);
    return CORE_OK;
}

status_t s1ap_final()
{
    return CORE_OK;
}

status_t s1ap_server(sock_node_t *snode, int type)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    d_assert(snode, return CORE_ERROR,);

    rv = sctp_server(&snode->sock, type, snode->list);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = sock_register(snode->sock, s1ap_accept_handler, NULL);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    d_trace(1, "s1ap_server() [%s]:%d\n",
            CORE_ADDR(snode->list, buf), CORE_PORT(snode->list));

    return CORE_OK;
}

status_t s1ap_delete(sock_id sock)
{
    d_assert(sock, return CORE_ERROR,);
    return sock_delete(sock);
}

status_t s1ap_send(sock_id sock, pkbuf_t *pkbuf,
        c_sockaddr_t *addr, c_uint16_t stream_no)
{
    int sent;

    d_assert(sock, return CORE_ERROR,);
    d_assert(pkbuf, return CORE_ERROR,);

    sent = core_sctp_sendmsg(sock, pkbuf->payload, pkbuf->len,
            addr, SCTP_S1AP_PPID, stream_no);
    d_trace(50, "[S1AP] SEND[%d] : ", sent);
    d_trace_hex(50, pkbuf->payload, pkbuf->len);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("core_sctp_sendmsg error (%d:%s)", errno, strerror(errno));
        return CORE_ERROR;
    }
    pkbuf_free(pkbuf);

    return CORE_OK;
}

status_t s1ap_recv(sock_id id, pkbuf_t *pkbuf)
{
    int size;

    size = core_sctp_recvdata(id, pkbuf->payload, MAX_SDU_LEN, NULL, NULL);
    if (size <= 0)
    {
        d_error("s1ap_recv() failed");
        return CORE_ERROR;
    }

    pkbuf->len = size;
    return CORE_OK;;
}

static int s1ap_accept_handler(sock_id id, void *data)
{
    char buf[CORE_ADDRSTRLEN];
    status_t rv;
    sock_id new;

    d_assert(id, return -1, "Null param");

    rv = sock_accept(&new, id);
    if (rv == CORE_OK)
    {
        c_sockaddr_t *addr = NULL;
        event_t e;

        addr = core_calloc(1, sizeof(c_sockaddr_t));
        d_assert(addr, return -1,);
        memcpy(addr, sock_remote_addr(new), sizeof(c_sockaddr_t));

        d_trace(1, "eNB-S1 accepted[%s]:%d in s1_path module\n", 
            CORE_ADDR(addr, buf), CORE_PORT(addr));

        event_set(&e, MME_EVT_S1AP_LO_ACCEPT);
        event_set_param1(&e, (c_uintptr_t)new);
        event_set_param2(&e, (c_uintptr_t)addr);
        event_set_param3(&e, (c_uintptr_t)0);
        if (mme_event_send(&e) != CORE_OK)
        {
            CORE_FREE(addr);
        }

        return 0;
    }
    else
    {
        d_error("sock accept failed(%d:%s)", errno, strerror(errno));

        return -1;
    }
}

int s1ap_recv_handler(sock_id sock, void *data)
{
    pkbuf_t *pkbuf;
    int size;
    event_t e;
    c_sockaddr_t *addr = NULL;
    sctp_info_t sinfo;
    int flags = 0;

    d_assert(sock, return -1, "Null param");

    pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if (pkbuf == NULL)
    {
        char tmp_buf[MAX_SDU_LEN];

        d_fatal("Can't allocate pkbuf");

        /* Read data from socket to exit from select */
        core_recv(sock, tmp_buf, MAX_SDU_LEN, 0);

        return -1;
    }

    size = core_sctp_recvmsg(
            sock, pkbuf->payload, pkbuf->len, NULL, &sinfo, &flags);
    if (size < 0)
    {
        d_error("core_sctp_recvmsg(%d) failed(%d:%s)",
                size, errno, strerror(errno));
        return size;
    }

    if (flags & MSG_NOTIFICATION)
    {
        union sctp_notification *not =
            (union sctp_notification *)pkbuf->payload;

        switch(not->sn_header.sn_type) 
        {
            case SCTP_ASSOC_CHANGE :
                d_trace(3, "SCTP_ASSOC_CHANGE"
                        "(type:0x%x, flags:0x%x, state:0x%x)\n", 
                        not->sn_assoc_change.sac_type,
                        not->sn_assoc_change.sac_flags,
                        not->sn_assoc_change.sac_state);

                if (not->sn_assoc_change.sac_state == SCTP_COMM_UP)
                {
                    d_trace(3, "SCTP_COMM_UP : inbound:%d, outbound = %d\n",
                            not->sn_assoc_change.sac_inbound_streams,
                            not->sn_assoc_change.sac_outbound_streams);

                    addr = core_calloc(1, sizeof(c_sockaddr_t));
                    d_assert(addr, pkbuf_free(pkbuf); return 0,);
                    memcpy(addr, sock_remote_addr(sock), sizeof(c_sockaddr_t));

                    event_set(&e, MME_EVT_S1AP_LO_SCTP_COMM_UP);
                    event_set_param1(&e, (c_uintptr_t)sock);
                    event_set_param2(&e, (c_uintptr_t)addr);
                    event_set_param3(&e, 
                        (c_uintptr_t)not->sn_assoc_change.sac_inbound_streams);
                    event_set_param4(&e, 
                        (c_uintptr_t)not->sn_assoc_change.sac_outbound_streams);

                    if (mme_event_send(&e) != CORE_OK)
                    {
                        d_error("Event MME_EVT_S1AP_LO_CONNREFUSED failed");
                        CORE_FREE(addr);
                    }

                    pkbuf_free(pkbuf);
                    return 0;
                }

                if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP)
                    d_trace(3, "SCTP_SHUTDOWN_COMP\n");
                if (not->sn_assoc_change.sac_state == SCTP_COMM_LOST)
                    d_trace(3, "SCTP_COMM_LOST\n");

                if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP ||
                    not->sn_assoc_change.sac_state == SCTP_COMM_LOST)
                {
                    addr = core_calloc(1, sizeof(c_sockaddr_t));
                    d_assert(addr, pkbuf_free(pkbuf); return 0,);
                    memcpy(addr, sock_remote_addr(sock), sizeof(c_sockaddr_t));

                    event_set(&e, MME_EVT_S1AP_LO_CONNREFUSED);
                    event_set_param1(&e, (c_uintptr_t)sock);
                    event_set_param2(&e, (c_uintptr_t)addr);

                    if (mme_event_send(&e) != CORE_OK)
                    {
                        d_error("Event MME_EVT_S1AP_LO_CONNREFUSED failed");
                        CORE_FREE(addr);
                    }

                    sock_delete(sock);
                    pkbuf_free(pkbuf);
                    return 0;
                }

                break;
            case SCTP_SEND_FAILED :
                d_error("SCTP_SEND_FAILED"
                        "(type:0x%x, flags:0x%x, error:0x%x)\n", 
                        not->sn_send_failed.ssf_type,
                        not->sn_send_failed.ssf_flags,
                        not->sn_send_failed.ssf_error);
                break;
            case SCTP_SHUTDOWN_EVENT :
                d_trace(3, "SCTP_SHUTDOWN_EVENT\n");

                addr = core_calloc(1, sizeof(c_sockaddr_t));
                d_assert(addr, pkbuf_free(pkbuf); return 0,);
                memcpy(addr, sock_remote_addr(sock), sizeof(c_sockaddr_t));

                event_set(&e, MME_EVT_S1AP_LO_CONNREFUSED);
                event_set_param1(&e, (c_uintptr_t)sock);
                event_set_param2(&e, (c_uintptr_t)addr);

                if (mme_event_send(&e) != CORE_OK)
                {
                    d_error("Event MME_EVT_S1AP_LO_CONNREFUSED failed");
                    CORE_FREE(addr);
                }

                sock_delete(sock);
                pkbuf_free(pkbuf);
                return 0;
            default :
                d_error("Discarding event with unknown flags:0x%x type:0x%x",
                        flags, not->sn_header.sn_type);
                break;
        }
    }
    else if (flags & MSG_EOR)
    {
        addr = core_calloc(1, sizeof(c_sockaddr_t));
        d_assert(addr, pkbuf_free(pkbuf); return 0,);
        memcpy(addr, sock_remote_addr(sock), sizeof(c_sockaddr_t));

        event_set(&e, MME_EVT_S1AP_MESSAGE);
        event_set_param1(&e, (c_uintptr_t)sock);
        event_set_param2(&e, (c_uintptr_t)addr);
        event_set_param3(&e, (c_uintptr_t)pkbuf);
        if (mme_event_send(&e) != CORE_OK)
        {
            pkbuf_free(pkbuf);
            CORE_FREE(addr);
        }

        return 0;
    }
    else
    {
        d_assert(0, pkbuf_free(pkbuf); return 0, "Unknown flags : 0x%x", flags);
    }

    pkbuf_free(pkbuf);
    return 0;
}
