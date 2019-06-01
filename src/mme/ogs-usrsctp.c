#include "ogs-sctp.h"

static void ogs_debug_printf(const char *format, ...);

int ogs_sctp_init(uint16_t port)
{
    usrsctp_init(port, NULL, ogs_debug_printf);
#ifdef SCTP_DEBUG
    usrsctp_sysctl_set_sctp_debug_on(SCTP_DEBUG_ALL);
#endif
    usrsctp_sysctl_set_sctp_blackhole(2);
    usrsctp_sysctl_set_sctp_enable_sack_immediately(1);

    return OGS_OK;
}

void ogs_sctp_final()
{
    while (usrsctp_finish() != 0) {
        ogs_warn("try to finsih SCTP");
        ogs_msleep(1000);
    }
}

ogs_sock_t *ogs_sctp_socket(int family, int type, ogs_socknode_t *node)
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

    ogs_sockopt_t option = {
        .sctp.heartbit_interval = 5000,     /* 5 seconds */
        .sctp.rto_initial = 3000,           /* 3 seconds */
        .sctp.rto_min = 1000,               /* 1 seconds */
        .sctp.rto_max = 5000,               /* 5 seconds */
        .sctp.max_num_of_ostreams = DEFAULT_SCTP_MAX_NUM_OF_OSTREAMS,
        .sctp.max_num_of_istreams = 65535,
        .sctp.max_attempts = 4,
        .sctp.max_initial_timeout = 8000    /* 8 seconds */
    };

    ogs_sctp_set_option(&option, node);

    if (!(socket = usrsctp_socket(family, type, IPPROTO_SCTP,
            node ? node->pollin.handler : NULL, NULL, 0, NULL))) {
        ogs_error("ogs_sctp_socket() failed");
        return NULL;
    }

    if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_RECVRCVINFO,
                &on, sizeof(int)) < 0) {
        ogs_error("usrsctp_setsockopt SCTP_RECVRCVINFO failed");
        return NULL;
    }

    memset(&event, 0, sizeof(event));
    event.se_assoc_id = SCTP_FUTURE_ASSOC;
    event.se_on = 1;
    for (i = 0; i < (int)(sizeof(event_types)/sizeof(uint16_t)); i++) {
        event.se_type = event_types[i];
        if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_EVENT,
                    &event, sizeof(struct sctp_event)) < 0) {
            ogs_error("usrsctp_setsockopt SCTP_EVENT failed");
            return NULL;
        }
    }

    memset(&initmsg, 0, sizeof(struct sctp_initmsg));
    socklen = sizeof(struct sctp_initmsg);
    if (usrsctp_getsockopt(socket, IPPROTO_SCTP, SCTP_INITMSG,
                &initmsg, &socklen) != 0) {
        ogs_error("getsockopt for SCTP_INITMSG failed(%d:%s)",
                errno, strerror( errno ));
        return NULL;
    }

    ogs_debug("Old INITMSG (numout:%d maxin:%d maxattempt:%d maxinit_to:%d)",
                initmsg.sinit_num_ostreams,
                initmsg.sinit_max_instreams,
                initmsg.sinit_max_attempts,
                initmsg.sinit_max_init_timeo);

    ogs_assert(option.sctp.max_num_of_ostreams > 1);

    initmsg.sinit_num_ostreams = option.sctp.max_num_of_ostreams;
    initmsg.sinit_max_instreams = option.sctp.max_num_of_istreams;
    initmsg.sinit_max_attempts = option.sctp.max_attempts;
    initmsg.sinit_max_init_timeo = option.sctp.max_initial_timeout;

    if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_INITMSG,
                            &initmsg, sizeof(initmsg)) != 0) {
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

void ogs_sctp_destroy(ogs_sock_t *sock)
{
    ogs_assert(sock);
    usrsctp_close((struct socket *)sock);
}

ogs_sock_t *ogs_sctp_server(int type, ogs_socknode_t *node)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(node);
    ogs_assert(node->addr);

    addr = node->addr;
    while (addr) {
        sock = ogs_sctp_socket(addr->ogs_sa_family, type, node);
        if (sock) {
            if (ogs_sctp_bind(sock, addr) == OGS_OK) {
                ogs_trace("sctp_server [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sctp_destroy(sock);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_error("sctp_server [%s]:%d failed",
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
        return NULL;
    }

    rv = ogs_sctp_listen(sock);
    ogs_assert(rv == OGS_OK);

    node->sock = sock;
    node->cleanup = ogs_sctp_destroy;

    return sock;
}

ogs_sock_t *ogs_sctp_client(int type, ogs_socknode_t *node)
{
    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(node);
    ogs_assert(node->addr);

    addr = node->addr;
    while (addr) {
        sock = ogs_sctp_socket(addr->ogs_sa_family, type, node);
        if (sock) {
            if (ogs_sctp_connect(sock, addr) == OGS_OK) {
                ogs_trace("sctp_client [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sctp_destroy(sock);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_error("sctp_client [%s]:%d failed", 
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
        return NULL;
    }

    node->sock = sock;
    node->cleanup = ogs_sctp_destroy;

    return sock;
}


int ogs_sctp_bind(ogs_sock_t *sock, ogs_sockaddr_t *sa_list)
{
    struct socket *socket = (struct socket *)sock;
    char buf[OGS_ADDRSTRLEN];
    socklen_t addrlen;

    ogs_assert(socket);
    ogs_assert(sa_list);

    addrlen = ogs_sockaddr_len(sa_list);
    ogs_assert(addrlen);

    if (usrsctp_bind(socket, &sa_list->sa, addrlen) != 0) {
        ogs_error("sctp_bind() [%s]:%d failed",
                OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));
        return OGS_ERROR;
    }

    ogs_debug("sctp_bind() [%s]:%d",
            OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));

    return OGS_OK;
}

int ogs_sctp_connect(ogs_sock_t *sock, ogs_sockaddr_t *sa_list)
{
    struct socket *socket = (struct socket *)sock;
    char buf[OGS_ADDRSTRLEN];
    socklen_t addrlen;

    ogs_assert(socket);
    ogs_assert(sa_list);

    addrlen = ogs_sockaddr_len(sa_list);
    ogs_assert(addrlen);

    if (usrsctp_connect(socket, &sa_list->sa, addrlen) != 0) {
        ogs_error("sctp_connect() [%s]:%d",
                OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));
        return OGS_ERROR;
    }

    ogs_debug("sctp_connect() [%s]:%d",
            OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));

    return OGS_OK;
}

int ogs_sctp_listen(ogs_sock_t *sock)
{
    int rc;
    struct socket *socket = (struct socket *)sock;
    ogs_assert(socket);

    rc = usrsctp_listen(socket, 5);
    if (rc < 0) {
        ogs_error("sctp_listen() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

ogs_sock_t *ogs_sctp_accept(ogs_sock_t *sock)
{
    struct socket *socket = (struct socket *)sock;
    struct socket *new = NULL;

    ogs_sockaddr_t addr;
    socklen_t addrlen;

    memset(&addr, 0, sizeof(addr));
    addrlen = sizeof(addr.ss);

    ogs_assert(sock);

    new = usrsctp_accept(socket, &addr.sa, &addrlen);
    if (!new) {
        ogs_error("sctp_accept() failed");
        return NULL;
    }

    return (ogs_sock_t *)new;
}

int ogs_sctp_sendmsg(ogs_sock_t *sock, const void *msg, size_t len,
        ogs_sockaddr_t *to, uint32_t ppid, uint16_t stream_no)
{
    struct socket *socket = (struct socket *)sock;
    struct sctp_sndinfo sndinfo;
    ssize_t sent;

    ogs_assert(socket);

    memset((void *)&sndinfo, 0, sizeof(struct sctp_sndinfo));
    sndinfo.snd_ppid = htonl(ppid);
    sndinfo.snd_sid = stream_no;
    sent = usrsctp_sendv(socket, msg, len,
            to ? &to->sa : NULL, to ? 1 : 0,
            (void *)&sndinfo, (socklen_t)sizeof(struct sctp_sndinfo),
            SCTP_SENDV_SNDINFO, 0);

    if (sent < 0 || sent != len)
    {
        ogs_error("sent : %d, len : %d", (int)sent, (int)len);
        return OGS_ERROR;
    }
    
    return sent;
}

int ogs_sctp_recvmsg(ogs_sock_t *sock, void *msg, size_t len,
        ogs_sockaddr_t *from, ogs_sctp_info_t *sinfo, int *msg_flags)
{
	struct socket *socket = (struct socket *)sock;
    ogs_sockaddr_t addr;
	ssize_t n = 0;
	int flags = 0;
    socklen_t addrlen = sizeof(struct sockaddr_storage);
	socklen_t infolen;
	struct sctp_rcvinfo rcv_info;
	unsigned int infotype = 0;

    ogs_assert(socket);

    memset(&rcv_info, 0, sizeof rcv_info);
    n = usrsctp_recvv(socket, msg, len,
            &addr.sa, &addrlen,
            (void *)&rcv_info,
            &infolen, &infotype, &flags);

    if (n < 0) {
        ogs_error("sctp_recvmsg(%d) failed", (int)n);
        return OGS_ERROR;
    }
    
    if (from) {
        memcpy(from, &addr, sizeof(ogs_sockaddr_t));
    }

    if (msg_flags) {
        *msg_flags = flags;
    }

    if (sinfo) {
        sinfo->ppid = ntohl(rcv_info.rcv_ppid);
        sinfo->stream_no = rcv_info.rcv_sid;
    }

    return n;
}

static void ogs_debug_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}
