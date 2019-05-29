#include "ogs-sctp.h"

#include "mme_event.h"

#include "s1ap_path.h"

static void accept_handler(short when, ogs_socket_t fd, void *data);

int s1ap_init(int sctp_streams, uint16_t port)
{
    ogs_sctp_set_num_ostreams(sctp_streams);
    return OGS_OK;
}

int s1ap_final()
{
    return OGS_OK;
}

void s1ap_server(ogs_socknode_t *snode, int type)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(snode);

    snode->sock = ogs_sctp_server(type, snode->addr);
    ogs_assert(snode->sock);

    snode->poll = ogs_pollset_add(mme_self()->pollset,
            OGS_POLLIN, snode->sock->fd, accept_handler, snode->sock);
    ogs_assert(snode->poll);

    ogs_info("s1ap_server() [%s]:%d",
            OGS_ADDR(snode->addr, buf), OGS_PORT(snode->addr));
}

void s1ap_closesocket(ogs_sock_t *sock)
{
    ogs_assert(sock);
    ogs_sock_destroy(sock);
}

void s1ap_delete(ogs_socknode_t *snode)
{
    ogs_assert(snode);

    ogs_pollset_remove(snode->poll);
    s1ap_closesocket(snode->sock);
}

int s1ap_send(ogs_sock_t *sock, ogs_pkbuf_t *pkbuf,
        ogs_sockaddr_t *addr, uint16_t stream_no)
{
    int sent;

    ogs_assert(sock);
    ogs_assert(pkbuf);

    sent = ogs_sctp_sendmsg(sock, pkbuf->data, pkbuf->len,
            addr, SCTP_S1AP_PPID, stream_no);
    if (sent < 0 || sent != pkbuf->len)
    {
        ogs_error("ogs_sctp_sendmsg error (%d:%s)", errno, strerror(errno));
        return OGS_ERROR;
    }
    ogs_pkbuf_free(pkbuf);

    return OGS_OK;
}

int s1ap_recv(ogs_sock_t *sock, ogs_pkbuf_t *pkbuf)
{
    int size;

    ogs_assert(sock);
    ogs_assert(pkbuf);

    size = ogs_sctp_recvdata(sock, pkbuf->data, MAX_SDU_LEN, NULL, NULL);
    if (size <= 0)
    {
        ogs_error("s1ap_recv() failed");
        return OGS_ERROR;
    }

    ogs_pkbuf_trim(pkbuf, size);
    return OGS_OK;;
}

static void accept_handler(short when, ogs_socket_t fd, void *data)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *sock = data;
    ogs_sock_t *new = NULL;

    ogs_assert(sock);
    ogs_assert(fd != INVALID_SOCKET);

    new = ogs_sock_accept(sock);
    if (new)
    {
        int rv;
        ogs_sockaddr_t *addr = NULL;
        mme_event_t *e = NULL;

        addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
        ogs_assert(addr);
        memcpy(addr, &new->remote_addr, sizeof(ogs_sockaddr_t));

        ogs_info("eNB-S1 accepted[%s]:%d in s1_path module", 
            OGS_ADDR(addr, buf), OGS_PORT(addr));

        e = mme_event_new(MME_EVT_S1AP_LO_ACCEPT);
        ogs_assert(e);
        e->enb_sock = new;
        e->enb_addr = addr;
        rv = ogs_queue_push(mme_self()->queue, e);
        if (rv != OGS_OK) {
            ogs_warn("ogs_queue_push() failed:%d", (int)rv);
            ogs_free(e->enb_addr);
            mme_event_free(e);
        }
    }
    else
    {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "accept() failed");
    }
}

void s1ap_recv_handler(short when, ogs_socket_t fd, void *data)
{
    int rv;
    ogs_pkbuf_t *pkbuf;
    int size;
    mme_event_t *e = NULL;
    ogs_sock_t *sock = data;
    ogs_sockaddr_t *addr = NULL;
    ogs_sctp_info_t sinfo;
    int flags = 0;

    ogs_assert(sock);
    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
#if DEPRECATED
    if (pkbuf == NULL)
    {
        char tmp_buf[MAX_SDU_LEN];

        d_fatal("Can't allocate pkbuf");

        /* Read data from socket to exit from select */
        ogs_recv(fd, tmp_buf, MAX_SDU_LEN, 0);

        return;
    }
#endif
    ogs_pkbuf_put(pkbuf, MAX_SDU_LEN);
    size = ogs_sctp_recvmsg(
            sock, pkbuf->data, pkbuf->len, NULL, &sinfo, &flags);
    if (size < 0)
    {
        ogs_error("ogs_sctp_recvmsg(%d) failed(%d:%s)",
                size, errno, strerror(errno));
        return;
    }

    if (flags & MSG_NOTIFICATION)
    {
        union sctp_notification *not =
            (union sctp_notification *)pkbuf->data;

        switch(not->sn_header.sn_type) 
        {
            case SCTP_ASSOC_CHANGE :
            {
                ogs_debug("SCTP_ASSOC_CHANGE:"
                        "[T:%d, F:0x%x, S:%d, I/O:%d/%d]", 
                        not->sn_assoc_change.sac_type,
                        not->sn_assoc_change.sac_flags,
                        not->sn_assoc_change.sac_state,
                        not->sn_assoc_change.sac_inbound_streams,
                        not->sn_assoc_change.sac_outbound_streams);

                if (not->sn_assoc_change.sac_state == SCTP_COMM_UP)
                {
                    ogs_debug("SCTP_COMM_UP");

                    addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
                    ogs_assert(addr);
                    memcpy(addr, &sock->remote_addr, sizeof(ogs_sockaddr_t));

                    e = mme_event_new(MME_EVT_S1AP_LO_SCTP_COMM_UP);
                    ogs_assert(e);
                    e->enb_sock = sock;
                    e->enb_addr = addr;
                    e->inbound_streams = 
                        not->sn_assoc_change.sac_inbound_streams;
                    e->outbound_streams = 
                        not->sn_assoc_change.sac_outbound_streams;
                    rv = ogs_queue_push(mme_self()->queue, e);
                    if (rv != OGS_OK) {
                        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
                        ogs_free(e->enb_addr);
                        mme_event_free(e);
                    }
                }
                else if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP ||
                        not->sn_assoc_change.sac_state == SCTP_COMM_LOST)
                {

                    if (not->sn_assoc_change.sac_state == SCTP_SHUTDOWN_COMP)
                        ogs_debug("SCTP_SHUTDOWN_COMP");
                    if (not->sn_assoc_change.sac_state == SCTP_COMM_LOST)
                        ogs_debug("SCTP_COMM_LOST");

                    addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
                    ogs_assert(addr);
                    memcpy(addr, &sock->remote_addr, sizeof(ogs_sockaddr_t));

                    e = mme_event_new(MME_EVT_S1AP_LO_CONNREFUSED);
                    ogs_assert(e);
                    e->enb_sock = sock;
                    e->enb_addr = addr;
                    rv = ogs_queue_push(mme_self()->queue, e);
                    if (rv != OGS_OK) {
                        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
                        ogs_free(e->enb_addr);
                        mme_event_free(e);
                    }
                }
                break;
            }
            case SCTP_SHUTDOWN_EVENT :
            {
                ogs_debug("SCTP_SHUTDOWN_EVENT:[T:%d, F:0x%x, L:%d]", 
                        not->sn_shutdown_event.sse_type,
                        not->sn_shutdown_event.sse_flags,
                        not->sn_shutdown_event.sse_length);

                addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
                ogs_assert(addr);
                memcpy(addr, &sock->remote_addr, sizeof(ogs_sockaddr_t));

                e = mme_event_new(MME_EVT_S1AP_LO_CONNREFUSED);
                ogs_assert(e);
                e->enb_sock = sock;
                e->enb_addr = addr;
                rv = ogs_queue_push(mme_self()->queue, e);
                if (rv != OGS_OK) {
                    ogs_warn("ogs_queue_push() failed:%d", (int)rv);
                    ogs_free(e->enb_addr);
                    mme_event_free(e);
                }
                break;
            }
            case SCTP_PEER_ADDR_CHANGE:
            {
                ogs_warn("SCTP_PEER_ADDR_CHANGE:[T:%d, F:0x%x, S:%d]", 
                        not->sn_paddr_change.spc_type,
                        not->sn_paddr_change.spc_flags,
                        not->sn_paddr_change.spc_error);
                break;
            }
            case SCTP_REMOTE_ERROR:
            {
                ogs_warn("SCTP_REMOTE_ERROR:[T:%d, F:0x%x, S:%d]", 
                        not->sn_remote_error.sre_type,
                        not->sn_remote_error.sre_flags,
                        not->sn_remote_error.sre_error);
                break;
            }
            case SCTP_SEND_FAILED :
            {
                ogs_error("SCTP_SEND_FAILED:[T:%d, F:0x%x, S:%d]", 
                        not->sn_send_failed.ssf_type,
                        not->sn_send_failed.ssf_flags,
                        not->sn_send_failed.ssf_error);
                break;
            }
            default :
            {
                ogs_error("Discarding event with unknown flags:0x%x type:0x%x",
                        flags, not->sn_header.sn_type);
                break;
            }
        }
    }
    else if (flags & MSG_EOR)
    {
        ogs_pkbuf_trim(pkbuf, size);

        addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
        ogs_assert(addr);
        memcpy(addr, &sock->remote_addr, sizeof(ogs_sockaddr_t));

        e = mme_event_new(MME_EVT_S1AP_MESSAGE);
        ogs_assert(e);
        e->enb_sock = sock;
        e->enb_addr = addr;
        e->pkbuf = pkbuf;
        rv = ogs_queue_push(mme_self()->queue, e);
        if (rv != OGS_OK) {
            ogs_warn("ogs_queue_push() failed:%d", (int)rv);
            ogs_free(e->enb_addr);
            ogs_pkbuf_free(e->pkbuf);
            mme_event_free(e);
        }

        return;
    }
    else
    {
        ogs_assert_if_reached();
    }

    ogs_pkbuf_free(pkbuf);
}
