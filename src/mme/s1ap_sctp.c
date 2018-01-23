#define TRACE_MODULE _s1ap_sctp

#include "core_debug.h"
#include "core_thread.h"

#include "mme_event.h"

#include "s1ap_path.h"

static int s1ap_accept_handler(sock_id sock, void *data);

status_t s1ap_init(c_uint16_t port)
{
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

status_t s1ap_send(sock_id sock, pkbuf_t *pkbuf, c_sockaddr_t *addr)
{
    int sent;

    d_assert(sock, return CORE_ERROR,);
    d_assert(pkbuf, return CORE_ERROR,);

    sent = core_sctp_sendmsg(sock, pkbuf->payload, pkbuf->len,
            addr, SCTP_S1AP_PPID, 0);
    d_trace(50, "[S1AP] Sent %d bytes : ", sent);
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

    size = core_sctp_recvmsg(id, pkbuf->payload, MAX_SDU_LEN, NULL, NULL, NULL);
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
    status_t rv;
    pkbuf_t *pkbuf;
    int size;
    event_t e;
    c_sockaddr_t *addr = NULL;

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

    size = core_sctp_recvmsg(sock, pkbuf->payload, pkbuf->len,
            NULL, NULL, NULL);
    if (size <= 0)
    {
        pkbuf_free(pkbuf);

        if (size == CORE_SCTP_EAGAIN)
        {
            return 0;
        }
        if (size == CORE_SCTP_REMOTE_CLOSED)
        {
            addr = core_calloc(1, sizeof(c_sockaddr_t));
            d_assert(addr, return -1,);
            memcpy(addr, sock_remote_addr(sock), sizeof(c_sockaddr_t));

            event_set(&e, MME_EVT_S1AP_LO_CONNREFUSED);
            event_set_param1(&e, (c_uintptr_t)sock);
            event_set_param2(&e, (c_uintptr_t)addr);
#ifdef NO_FD_LOCK
            sock_delete(sock);
#else
#error do not use lock in socket fd
#endif
            if (mme_event_send(&e) != CORE_OK)
            {
                d_error("Event MME_EVT_S1AP_LO_CONNREFUSED failed");
                CORE_FREE(addr);
            }

            return 0;
        }

        if (errno != EAGAIN)
        {
            d_error("core_sctp_recvmsg(%d) failed(%d:%s)",
                    size, errno, strerror(errno));
        }

        return 0;
    }

    pkbuf->len = size;

    d_trace(50, "[S1AP] Receive : ");
    d_trace_hex(50, pkbuf->payload, pkbuf->len);

    addr = core_calloc(1, sizeof(c_sockaddr_t));
    d_assert(addr, return -1,);
    memcpy(addr, sock_remote_addr(sock), sizeof(c_sockaddr_t));

    event_set(&e, MME_EVT_S1AP_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)sock);
    event_set_param2(&e, (c_uintptr_t)addr);
    event_set_param3(&e, (c_uintptr_t)pkbuf);
    rv = mme_event_send(&e);
    if (rv != CORE_OK)
    {
        pkbuf_free(pkbuf);
        CORE_FREE(addr);
    }
    
    return 0;
}
