#include "ogs-sctp.h"

#include "mme_event.h"

#include "s1ap_path.h"

#if HAVE_NETDB_H
#include <netdb.h>
#endif

#if HAVE_USRSCTP
#ifndef INET
#define INET            1
#endif
#ifndef INET6
#define INET6           1
#endif
#include <usrsctp.h>
#endif

static ogs_sock_t *s1ap_usrsctp_socket(int family, int type,
    int (*receive_cb)(struct socket *sock, union sctp_sockstore addr,
        void *data, size_t datalen, struct sctp_rcvinfo, int flags,
        void *ulp_info));
static int s1ap_usrsctp_bind(ogs_sock_t *sock, ogs_sockaddr_t *sa);
static int s1ap_usrsctp_connect(ogs_sock_t *sock, ogs_sockaddr_t *sa);
static int s1ap_usrsctp_listen(ogs_sock_t *sock);

static int s1ap_usrsctp_recv_handler(struct socket *sock,
        union sctp_sockstore addr, void *data, size_t datalen,
        struct sctp_rcvinfo rcv, int flags, void *ulp_info);

static ogs_sockaddr_t *usrsctp_remote_addr(union sctp_sockstore *store);
static void debug_printf(const char *format, ...);

static int sctp_num_ostreams = -1;

int s1ap_init(int sctp_streams, uint16_t port)
{
    usrsctp_init(port, NULL, debug_printf);
#ifdef SCTP_DEBUG
    usrsctp_sysctl_set_sctp_debug_on(SCTP_DEBUG_ALL);
#endif
    usrsctp_sysctl_set_sctp_blackhole(2);
    usrsctp_sysctl_set_sctp_enable_sack_immediately(1);
    sctp_num_ostreams = sctp_streams;

    return OGS_OK;
}

int s1ap_final()
{
    while(usrsctp_finish() != 0)
    {
        ogs_error("try to finsih SCTP");
        ogs_msleep(1000);
    }
    return OGS_OK;
}

void s1ap_server(ogs_socknode_t *snode, int type)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(snode);

    snode->sock = ogs_sctp_server(type, snode->addr);
    ogs_assert(snode->sock);

    ogs_info("s1ap_server() [%s]:%d",
            OGS_ADDR(snode->addr, buf), OGS_PORT(snode->addr));
}

void s1ap_closesocket(ogs_sock_t *sock)
{
    ogs_assert(sock);
    usrsctp_close((struct socket *)sock);
}

void s1ap_delete(ogs_socknode_t *snode)
{
    ogs_assert(snode);
    s1ap_closesocket(snode->sock);
    snode->sock = NULL;
}

ogs_sock_t *ogs_sctp_server(int type, ogs_sockaddr_t *sa_list)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;

    addr = sa_list;
    while(addr)
    {
        sock = s1ap_usrsctp_socket(
                addr->ogs_sa_family, type, s1ap_usrsctp_recv_handler);
        if (sock)
        {
            if (s1ap_usrsctp_bind(sock, addr) == OGS_OK)
            {
                ogs_trace("sctp_server [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            s1ap_closesocket(sock);
        }

        addr = addr->next;
    }

    if (addr == NULL)
    {
        ogs_error("sctp_server [%s]:%d failed",
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
        return NULL;
    }

    rv = s1ap_usrsctp_listen(sock);
    ogs_assert(rv == OGS_OK);

    return sock;
}

ogs_sock_t *ogs_sctp_client(int type, ogs_sockaddr_t *sa_list)
{
    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;
    char buf[OGS_ADDRSTRLEN];

    addr = sa_list;
    while(addr)
    {
        sock = s1ap_usrsctp_socket(addr->ogs_sa_family, type, NULL);
        if (sock)
        {
            if (s1ap_usrsctp_connect(sock, addr) == OGS_OK)
            {
                ogs_trace("sctp_client [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            s1ap_closesocket(sock);
        }

        addr = addr->next;
    }

    if (addr == NULL)
    {
        ogs_error("sctp_client [%s]:%d failed", 
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
        return NULL;
    }

    return sock;
}

int s1ap_send(ogs_sock_t *sock, ogs_pkbuf_t *pkbuf,
        ogs_sockaddr_t *addr, uint16_t stream_no)
{
    ssize_t sent;
    struct socket *socket = (struct socket *)sock;
    struct sctp_sndinfo sndinfo;

    ogs_assert(socket);
    ogs_assert(pkbuf);

    memset((void *)&sndinfo, 0, sizeof(struct sctp_sndinfo));
    sndinfo.snd_ppid = htonl(SCTP_S1AP_PPID);
    sndinfo.snd_sid = stream_no;
    sent = usrsctp_sendv(socket, pkbuf->data, pkbuf->len, 
            addr ? &addr->sa : NULL, addr ? 1 : 0,
            (void *)&sndinfo, (socklen_t)sizeof(struct sctp_sndinfo),
            SCTP_SENDV_SNDINFO, 0);

    if (sent < 0 || sent != pkbuf->len)
    {
        ogs_error("sent : %d, pkbuf->len : %d", (int)sent, pkbuf->len);
        return OGS_ERROR;
    }
    ogs_pkbuf_free(pkbuf);

    return OGS_OK;
}

int s1ap_recv(ogs_sock_t *sock, ogs_pkbuf_t *pkbuf)
{
	struct socket *socket = (struct socket *)sock;
    ogs_sockaddr_t addr;
	ssize_t n = 0;
	int flags = 0;
	socklen_t from_len;
	socklen_t infolen;
	struct sctp_rcvinfo rcv_info;
	unsigned int infotype = 0;

    while(1)
    {
        n = usrsctp_recvv(socket, pkbuf->data, MAX_SDU_LEN,
                &addr.sa, &from_len, (void *)&rcv_info,
                &infolen, &infotype, &flags);
        if (n > 0)
        {
#undef MSG_NOTIFICATION
#define MSG_NOTIFICATION 0x2000
            if (flags & MSG_NOTIFICATION)
            {
                /* Nothing to do */
            }
            else if (flags & MSG_EOR)
            {
                break;
            }
        }
    }

    ogs_pkbuf_trim(pkbuf, n);
    return OGS_OK;
}

static ogs_sock_t *s1ap_usrsctp_socket(int family, int type,
    int (*receive_cb)(struct socket *sock, union sctp_sockstore store,
        void *data, size_t datalen, struct sctp_rcvinfo, int flags,
        void *ulp_info))
{
    struct socket *socket = NULL;
    const int on = 1;
    struct sctp_event event;
    uint16_t event_types[] = {
        SCTP_ASSOC_CHANGE,
        SCTP_PEER_ADDR_CHANGE,
        SCTP_REMOTE_ERROR,
        SCTP_SHUTDOWN_EVENT,
        SCTP_ADAPTATION_INDICATION,
        SCTP_PARTIAL_DELIVERY_EVENT
    };
    struct sctp_initmsg initmsg;
    socklen_t socklen;
    int i;

    if (!(socket = usrsctp_socket(family, type, IPPROTO_SCTP,
            receive_cb, NULL, 0, NULL)))
    {
        ogs_error("usrsctp_socket failed");
        return NULL;
    }

    if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_RECVRCVINFO,
                &on, sizeof(int)) < 0)
    {
        ogs_error("usrsctp_setsockopt SCTP_RECVRCVINFO failed");
        return NULL;
    }

    memset(&event, 0, sizeof(event));
    event.se_assoc_id = SCTP_FUTURE_ASSOC;
    event.se_on = 1;
    for (i = 0; i < (int)(sizeof(event_types)/sizeof(uint16_t)); i++)
    {
        event.se_type = event_types[i];
        if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_EVENT,
                    &event, sizeof(struct sctp_event)) < 0)
        {
            ogs_error("usrsctp_setsockopt SCTP_EVENT failed");
            return NULL;
        }
    }

    memset(&initmsg, 0, sizeof(struct sctp_initmsg));
    socklen = sizeof(struct sctp_initmsg);
    if (usrsctp_getsockopt(socket, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, &socklen) != 0) 
    {
        ogs_error("getsockopt for SCTP_INITMSG failed(%d:%s)",
                errno, strerror( errno ));
        return NULL;
    }

    ogs_debug("Old INITMSG (numout:%d maxin:%d maxattempt:%d maxinit_to:%d)",
                initmsg.sinit_num_ostreams,
                initmsg.sinit_max_instreams,
                initmsg.sinit_max_attempts,
                initmsg.sinit_max_init_timeo);

    /*
     * INITMSG
     * 
     * max number of input streams : 65535
     * max attemtps : 4
     * max initial timeout : 8 secs
     */
    ogs_assert(sctp_num_ostreams > 1);

    initmsg.sinit_num_ostreams = sctp_num_ostreams;
    initmsg.sinit_max_instreams = 65535;
    initmsg.sinit_max_attempts = 4;
    initmsg.sinit_max_init_timeo = 8000;

    if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_INITMSG,
                            &initmsg, sizeof(initmsg)) != 0) 
    {
        ogs_error("setsockopt for SCTP_INITMSG failed(%d:%s)",
                errno, strerror( errno ));
        return NULL;
    }

    ogs_debug("New INITMSG (numout:%d maxin:%d maxattempt:%d maxinit_to:%d)",
                initmsg.sinit_num_ostreams,
                initmsg.sinit_max_instreams,
                initmsg.sinit_max_attempts,
                initmsg.sinit_max_init_timeo);

    return (ogs_sock_t *)socket;;
}

static int s1ap_usrsctp_bind(ogs_sock_t *sock, ogs_sockaddr_t *sa)
{
    struct socket *socket = (struct socket *)sock;
    char buf[OGS_ADDRSTRLEN];
    socklen_t addrlen;

    ogs_assert(socket);
    ogs_assert(sa);

    addrlen = ogs_sockaddr_len(sa);
    ogs_assert(addrlen);

    if (usrsctp_bind(socket, &sa->sa, addrlen) != 0)
    {
        ogs_error("usrsctp_bind [%s]:%d failed",
                OGS_ADDR(sa, buf), OGS_PORT(sa));
        return OGS_ERROR;
    }

    ogs_trace("usrsctp_bind [%s]:%d", OGS_ADDR(sa, buf), OGS_PORT(sa));

    return OGS_OK;
}

static int s1ap_usrsctp_connect(ogs_sock_t *sock, ogs_sockaddr_t *sa)
{
    struct socket *socket = (struct socket *)sock;
    char buf[OGS_ADDRSTRLEN];
    socklen_t addrlen;

    ogs_assert(socket);
    ogs_assert(sa);

    addrlen = ogs_sockaddr_len(sa);
    ogs_assert(addrlen);

    if (usrsctp_connect(socket, &sa->sa, addrlen) != 0)
    {
        ogs_error("usrsctp_connect [%s]:%d", OGS_ADDR(sa, buf), OGS_PORT(sa));
        return OGS_ERROR;
    }

    ogs_trace("usrsctp_connect [%s]:%d", OGS_ADDR(sa, buf), OGS_PORT(sa));

    return OGS_OK;
}

static int s1ap_usrsctp_listen(ogs_sock_t *sock)
{
    int rc;
    struct socket *socket = (struct socket *)sock;
    ogs_assert(socket);

    rc = usrsctp_listen(socket, 5);
    if (rc < 0)
    {
        ogs_error("usrsctp_listen() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int s1ap_usrsctp_recv_handler(struct socket *sock,
    union sctp_sockstore store, void *data, size_t datalen,
    struct sctp_rcvinfo rcv, int flags, void *ulp_info)
{
    if (data)
    {
        int rv;
        mme_event_t *e = NULL;

#undef MSG_NOTIFICATION
#define MSG_NOTIFICATION 0x2000
        if (flags & MSG_NOTIFICATION)
        {
            union sctp_notification *not = (union sctp_notification *)data;
            if (not->sn_header.sn_length == (uint32_t)datalen)
            {
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

                        if (not->sn_assoc_change.sac_state == 
                                SCTP_SHUTDOWN_COMP ||
                            not->sn_assoc_change.sac_state == 
                                SCTP_COMM_LOST)
                        {
                            ogs_sockaddr_t *addr =
                                usrsctp_remote_addr(&store);
                            ogs_assert(addr);

                            if (not->sn_assoc_change.sac_state == 
                                SCTP_SHUTDOWN_COMP)
                                ogs_debug("SCTP_SHUTDOWN_COMP");
                            if (not->sn_assoc_change.sac_state == 
                                SCTP_COMM_LOST)
                                ogs_debug("SCTP_COMM_LOST");

                            e = mme_event_new(MME_EVT_S1AP_LO_CONNREFUSED);
                            ogs_assert(e);
                            e->enb_sock = (ogs_sock_t *)sock;
                            e->enb_addr = addr;
                            rv = ogs_queue_push(mme_self()->queue, e);
                            if (rv != OGS_OK) {
                                ogs_warn("ogs_queue_push() failed:%d", (int)rv);
                                ogs_free(e->enb_addr);
                                mme_event_free(e);
                            } else {
                                ogs_pollset_notify(mme_self()->pollset);
                            }
                        }
                        else if (not->sn_assoc_change.sac_state == SCTP_COMM_UP)
                        {
                            ogs_sockaddr_t *addr =
                                usrsctp_remote_addr(&store);
                            ogs_assert(addr);

                            ogs_debug("SCTP_COMM_UP");

                            e = mme_event_new(MME_EVT_S1AP_LO_SCTP_COMM_UP);
                            ogs_assert(e);
                            e->enb_sock = (ogs_sock_t *)sock;
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
                            } else {
                                ogs_pollset_notify(mme_self()->pollset);
                            }
                        }
                        break;
                    }
                    case SCTP_SHUTDOWN_EVENT :
                    {
                        ogs_sockaddr_t *addr = usrsctp_remote_addr(&store);
                        ogs_assert(addr);

                        ogs_debug("SCTP_SHUTDOWN_EVENT:"
                                "[T:0x%x, F:0x%x, L:0x%x]", 
                                not->sn_shutdown_event.sse_type,
                                not->sn_shutdown_event.sse_flags,
                                not->sn_shutdown_event.sse_length);

                        e = mme_event_new(MME_EVT_S1AP_LO_CONNREFUSED);
                        ogs_assert(e);
                        e->enb_sock = (ogs_sock_t *)sock;
                        e->enb_addr = addr;
                        rv = ogs_queue_push(mme_self()->queue, e);
                        if (rv != OGS_OK) {
                            ogs_warn("ogs_queue_push() failed:%d", (int)rv);
                            ogs_free(e->enb_addr);
                            mme_event_free(e);
                        } else {
                            ogs_pollset_notify(mme_self()->pollset);
                        }
                        break;
                    }
                    case SCTP_PEER_ADDR_CHANGE:
                    {
                        ogs_warn("SCTP_PEER_ADDR_CHANGE:"
                                "[T:%d, F:0x%x, S:%d]", 
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
                                not->sn_send_failed_event.ssfe_type,
                                not->sn_send_failed_event.ssfe_flags,
                                not->sn_send_failed_event.ssfe_error);
                        break;
                    }
                    default :
                        ogs_error("Discarding event with "
                                "unknown flags:0x%x type:0x%x",
                                flags, not->sn_header.sn_type);
                        break;
                }
            }
        }
        else if (flags & MSG_EOR)
        {
            ogs_pkbuf_t *pkbuf;
            ogs_sockaddr_t *addr = NULL;

            pkbuf = ogs_pkbuf_alloc(NULL, MAX_SDU_LEN);
            ogs_pkbuf_put_data(pkbuf, data, datalen);

            addr = usrsctp_remote_addr(&store);
            ogs_assert(addr);

            e = mme_event_new(MME_EVT_S1AP_MESSAGE);
            ogs_assert(e);
            e->enb_sock = (ogs_sock_t *)sock;
            e->enb_addr = addr;
            e->pkbuf = pkbuf;
            rv = ogs_queue_push(mme_self()->queue, e);
            if (rv != OGS_OK) {
                ogs_warn("ogs_queue_push() failed:%d", (int)rv);
                ogs_free(e->enb_addr);
                ogs_pkbuf_free(e->pkbuf);
                mme_event_free(e);
            } else {
                ogs_pollset_notify(mme_self()->pollset);
            }
        }
        else
        {
            ogs_error("Not engough buffer. Need more recv : 0x%x", flags);
        }
        free(data);
    }
    return (1);
}

static ogs_sockaddr_t *usrsctp_remote_addr(union sctp_sockstore *store)
{
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(store);

    addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    ogs_assert(addr);

    addr->ogs_sa_family = store->sin.sin_family;
    switch(addr->ogs_sa_family)
    {
        case AF_INET:
            memcpy(&addr->sin, &store->sin, sizeof(struct sockaddr_in));
            break;
        case AF_INET6:
            memcpy(&addr->sin6, &store->sin6, sizeof(struct sockaddr_in6));
            break;
        default:
            ogs_assert_if_reached();
    }

    return addr;
}

static void debug_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}
