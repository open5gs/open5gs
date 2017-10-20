#define TRACE_MODULE _s1ap_usrsctp

#include "core_debug.h"
#include "core_thread.h"

#include "mme_event.h"

#include "s1ap_path.h"

#if HAVE_USRSCTP_H
#ifndef INET
#define INET            1
#endif
#ifndef INET6
#define INET6           1
#endif
#include <usrsctp.h>
#endif

#define LOCAL_UDP_PORT  9899

static void handle_notification(union sctp_notification *notif, size_t n);
static int s1ap_usrsctp_recv_cb(struct socket *sock,
        union sctp_sockstore addr, void *data, size_t datalen,
        struct sctp_rcvinfo rcv, int flags, void *ulp_info);

static void debug_printf(const char *format, ...);

int accept_thread_should_stop = 0;
static thread_id accept_thread;
static void *THREAD_FUNC accept_main(thread_id id, void *data);

status_t s1ap_open(void)
{
    status_t rv;
    char buf[INET_ADDRSTRLEN];

    struct socket *psock = NULL;
    struct sockaddr_in local_addr;
    const int on = 1;
    struct sctp_event event;
    c_uint16_t event_types[] = {
        SCTP_ASSOC_CHANGE,
        SCTP_PEER_ADDR_CHANGE,
        SCTP_REMOTE_ERROR,
        SCTP_SHUTDOWN_EVENT,
        SCTP_ADAPTATION_INDICATION,
        SCTP_PARTIAL_DELIVERY_EVENT
    };
    int i;

    usrsctp_init(LOCAL_UDP_PORT, NULL, debug_printf);
#ifdef SCTP_DEBUG
    usrsctp_sysctl_set_sctp_debug_on(SCTP_DEBUG_ALL);
#endif
    usrsctp_sysctl_set_sctp_blackhole(2);
    usrsctp_sysctl_set_sctp_enable_sack_immediately(1);

    if (!(psock = usrsctp_socket(
            AF_INET, SOCK_STREAM, IPPROTO_SCTP, s1ap_usrsctp_recv_cb,
            NULL, 0, NULL)))
    {
        d_error("usrsctp_socket failed");
        return CORE_ERROR;
    }

    mme_self()->s1ap_sock = (net_sock_t *)psock;

    if (usrsctp_setsockopt(psock, IPPROTO_SCTP, SCTP_RECVRCVINFO,
                &on, sizeof(int)) < 0)
    {
        d_error("usrsctp_setsockopt SCTP_RECVRCVINFO failed");
        return CORE_ERROR;
    }

    memset(&event, 0, sizeof(event));
    event.se_assoc_id = SCTP_FUTURE_ASSOC;
    event.se_on = 1;
    for (i = 0; i < (int)(sizeof(event_types)/sizeof(c_uint16_t)); i++)
    {
        event.se_type = event_types[i];
        if (usrsctp_setsockopt(psock, IPPROTO_SCTP, SCTP_EVENT,
                    &event, sizeof(struct sctp_event)) < 0)
        {
            d_error("usrsctp_setsockopt SCTP_EVENT failed");
            return CORE_ERROR;
        }
    }

    memset((void *)&local_addr, 0, sizeof(struct sockaddr_in));
    local_addr.sin_family = AF_INET;
    local_addr.sin_len = sizeof(struct sockaddr_in);
    local_addr.sin_port = htons(mme_self()->s1ap_port);
    local_addr.sin_addr.s_addr = 0;

    if (usrsctp_bind(psock, (struct sockaddr *)&local_addr,
                sizeof(struct sockaddr_in)) == -1)
    {
        d_error("usrsctp_bind failed(%s:%d)",
            INET_NTOP(&mme_self()->s1ap_addr, buf), mme_self()->s1ap_port);
        return CORE_ERROR;
    }

    if (usrsctp_listen(psock, 1) < 0)
    {
        d_error("usrsctp_listen failed");
        return CORE_ERROR;
    }

    rv = thread_create(&accept_thread, NULL, accept_main, NULL);
    if (rv != CORE_OK) return rv;

    d_trace(1, "s1_enb_listen() %s:%d\n", 
        INET_NTOP(&mme_self()->s1ap_addr, buf), mme_self()->s1ap_port);

    return CORE_OK;
}

status_t s1ap_close()
{
    struct socket *psock = (struct socket *)mme_self()->s1ap_sock;
    d_assert(mme_self(), return CORE_ERROR, "Null param");
    d_assert(mme_self()->s1ap_sock != NULL, return CORE_ERROR,
            "S1-ENB path already opened");

    accept_thread_should_stop = 1;
    usrsctp_close(psock);
#if 0 /* FIXME : how to release usrsctp_accept() blocking */
    while(usrsctp_finish() != 0)
    {
        d_error("try to finsih SCTP\n");
        core_sleep(time_from_msec(1000));
    }

    thread_delete(accept_thread);
#endif

    return CORE_OK;
}

status_t s1ap_sctp_close(net_sock_t *sock)
{
    usrsctp_close((struct socket *)sock);
    return CORE_OK;
}

status_t s1ap_sendto(net_sock_t *s, pkbuf_t *pkbuf,
        c_uint32_t addr, c_uint16_t port)
{
    char buf[INET_ADDRSTRLEN];
    ssize_t sent;
    struct socket *psock = (struct socket *)s;
    struct sctp_sndinfo sndinfo;

    d_assert(s, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    memset((void *)&sndinfo, 0, sizeof(struct sctp_sndinfo));
    sndinfo.snd_ppid = htonl(SCTP_S1AP_PPID);
    sent = usrsctp_sendv(psock, pkbuf->payload, pkbuf->len, 
            NULL, 0,  /* Only SOCK_STREAM is supported at this time */
            (void *)&sndinfo, (socklen_t)sizeof(struct sctp_sndinfo),
            SCTP_SENDV_SNDINFO, 0);

    d_trace(10,"Sent %d->%d bytes to [%s:%d]\n", 
            pkbuf->len, sent, INET_NTOP(&addr, buf), port);
    d_trace_hex(10, pkbuf->payload, pkbuf->len);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("sent : %d, pkbuf->len : %d\n", sent, pkbuf->len);
        return CORE_ERROR;
    }
    pkbuf_free(pkbuf);

    return CORE_OK;
}

static void *THREAD_FUNC accept_main(thread_id id, void *data)
{
    event_t e;

    struct socket *sock = NULL;
    struct sockaddr_in remote_addr;
    socklen_t addr_len;

    while (!accept_thread_should_stop)
    {
        if ((sock = usrsctp_accept((struct socket *)mme_self()->s1ap_sock,
                    (struct sockaddr *)&remote_addr, &addr_len)) == NULL)
        {
            d_error("usrsctp_accept failed");
            continue;
        }

        event_set(&e, MME_EVT_S1AP_LO_ACCEPT);
        event_set_param1(&e, (c_uintptr_t)sock);
        mme_event_send(&e);
    }

    return NULL;
}

static int s1ap_usrsctp_recv_cb(struct socket *sock,
    union sctp_sockstore addr, void *data, size_t datalen,
    struct sctp_rcvinfo rcv, int flags, void *ulp_info)
{
    if (data)
    {
        if (flags & MSG_NOTIFICATION)
        {
            handle_notification((union sctp_notification *)data, datalen);
        }
        else
        {
            c_uint32_t ppid = ntohl(rcv.rcv_ppid);
            if ((flags & MSG_EOR) && ppid == SCTP_S1AP_PPID)
            {
                event_t e;
                pkbuf_t *pkbuf;

                pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
                d_assert(pkbuf, return 1, );

                pkbuf->len = datalen;
                memcpy(pkbuf->payload, data, pkbuf->len);

                event_set(&e, MME_EVT_S1AP_MESSAGE);
                event_set_param1(&e, (c_uintptr_t)sock);
                event_set_param2(&e, (c_uintptr_t)pkbuf);
                mme_event_send(&e);
            }
            else
            {
                d_warn("Unknwon PPID(%d) for data length(%ld)\n",
                        ppid, datalen);
            }
        }
        free(data);
    }
    return (1);
}


static void
handle_association_change_event(struct sctp_assoc_change *sac)
{
    unsigned int i, n;

    printf("Association change ");
    switch (sac->sac_state) {
    case SCTP_COMM_UP:
        printf("SCTP_COMM_UP");
        break;
    case SCTP_COMM_LOST:
        printf("SCTP_COMM_LOST");
        break;
    case SCTP_RESTART:
        printf("SCTP_RESTART");
        break;
    case SCTP_SHUTDOWN_COMP:
        printf("SCTP_SHUTDOWN_COMP");
        break;
    case SCTP_CANT_STR_ASSOC:
        printf("SCTP_CANT_STR_ASSOC");
        break;
    default:
        printf("UNKNOWN");
        break;
    }
    printf(", streams (in/out) = (%u/%u)",
           sac->sac_inbound_streams, sac->sac_outbound_streams);
    n = sac->sac_length - sizeof(struct sctp_assoc_change);
    if (((sac->sac_state == SCTP_COMM_UP) ||
         (sac->sac_state == SCTP_RESTART)) && (n > 0)) {
        printf(", supports");
        for (i = 0; i < n; i++) {
            switch (sac->sac_info[i]) {
            case SCTP_ASSOC_SUPPORTS_PR:
                printf(" PR");
                break;
            case SCTP_ASSOC_SUPPORTS_AUTH:
                printf(" AUTH");
                break;
            case SCTP_ASSOC_SUPPORTS_ASCONF:
                printf(" ASCONF");
                break;
            case SCTP_ASSOC_SUPPORTS_MULTIBUF:
                printf(" MULTIBUF");
                break;
            case SCTP_ASSOC_SUPPORTS_RE_CONFIG:
                printf(" RE-CONFIG");
                break;
            default:
                printf(" UNKNOWN(0x%02x)", sac->sac_info[i]);
                break;
            }
        }
    } else if (((sac->sac_state == SCTP_COMM_LOST) ||
                (sac->sac_state == SCTP_CANT_STR_ASSOC)) && (n > 0)) {
        printf(", ABORT =");
        for (i = 0; i < n; i++) {
            printf(" 0x%02x", sac->sac_info[i]);
        }
    }
    printf(".\n");
    if ((sac->sac_state == SCTP_CANT_STR_ASSOC) ||
        (sac->sac_state == SCTP_SHUTDOWN_COMP) ||
        (sac->sac_state == SCTP_COMM_LOST)) {
        exit(0);
    }
    return;
}

static void
handle_peer_address_change_event(struct sctp_paddr_change *spc)
{
    char addr_buf[INET6_ADDRSTRLEN];
    const char *addr;
    struct sockaddr_in *sin;
    struct sockaddr_in6 *sin6;
    struct sockaddr_conn *sconn;

    switch (spc->spc_aaddr.ss_family) {
    case AF_INET:
        sin = (struct sockaddr_in *)&spc->spc_aaddr;
        addr = inet_ntop(AF_INET, &sin->sin_addr, addr_buf, INET_ADDRSTRLEN);
        break;
    case AF_INET6:
        sin6 = (struct sockaddr_in6 *)&spc->spc_aaddr;
        addr = inet_ntop(AF_INET6, &sin6->sin6_addr, addr_buf, INET6_ADDRSTRLEN);
        break;
    case AF_CONN:
        sconn = (struct sockaddr_conn *)&spc->spc_aaddr;
#ifdef _WIN32
        _snprintf(addr_buf, INET6_ADDRSTRLEN, "%p", sconn->sconn_addr);
#else
        snprintf(addr_buf, INET6_ADDRSTRLEN, "%p", sconn->sconn_addr);
#endif
        addr = addr_buf;
        break;
    default:
#ifdef _WIN32
        _snprintf(addr_buf, INET6_ADDRSTRLEN, "Unknown family %d", spc->spc_aaddr.ss_family);
#else
        snprintf(addr_buf, INET6_ADDRSTRLEN, "Unknown family %d", spc->spc_aaddr.ss_family);
#endif
        addr = addr_buf;
        break;
    }
    printf("Peer address %s is now ", addr);
    switch (spc->spc_state) {
    case SCTP_ADDR_AVAILABLE:
        printf("SCTP_ADDR_AVAILABLE");
        break;
    case SCTP_ADDR_UNREACHABLE:
        printf("SCTP_ADDR_UNREACHABLE");
        break;
    case SCTP_ADDR_REMOVED:
        printf("SCTP_ADDR_REMOVED");
        break;
    case SCTP_ADDR_ADDED:
        printf("SCTP_ADDR_ADDED");
        break;
    case SCTP_ADDR_MADE_PRIM:
        printf("SCTP_ADDR_MADE_PRIM");
        break;
    case SCTP_ADDR_CONFIRMED:
        printf("SCTP_ADDR_CONFIRMED");
        break;
    default:
        printf("UNKNOWN");
        break;
    }
    printf(" (error = 0x%08x).\n", spc->spc_error);
    return;
}

static void
handle_send_failed_event(struct sctp_send_failed_event *ssfe)
{
    size_t i, n;

    if (ssfe->ssfe_flags & SCTP_DATA_UNSENT) {
        printf("Unsent ");
    }
    if (ssfe->ssfe_flags & SCTP_DATA_SENT) {
        printf("Sent ");
    }
    if (ssfe->ssfe_flags & ~(SCTP_DATA_SENT | SCTP_DATA_UNSENT)) {
        printf("(flags = %x) ", ssfe->ssfe_flags);
    }
    printf("message with PPID = %u, SID = %u, flags: 0x%04x due to error = 0x%08x",
           ntohl(ssfe->ssfe_info.snd_ppid), ssfe->ssfe_info.snd_sid,
           ssfe->ssfe_info.snd_flags, ssfe->ssfe_error);
    n = ssfe->ssfe_length - sizeof(struct sctp_send_failed_event);
    for (i = 0; i < n; i++) {
        printf(" 0x%02x", ssfe->ssfe_data[i]);
    }
    printf(".\n");
    return;
}

static void
handle_notification(union sctp_notification *notif, size_t n)
{
    if (notif->sn_header.sn_length != (uint32_t)n) {
        return;
    }
    switch (notif->sn_header.sn_type) {
    case SCTP_ASSOC_CHANGE:
        handle_association_change_event(&(notif->sn_assoc_change));
        break;
    case SCTP_PEER_ADDR_CHANGE:
        handle_peer_address_change_event(&(notif->sn_paddr_change));
        break;
    case SCTP_REMOTE_ERROR:
        break;
    case SCTP_SHUTDOWN_EVENT:
        break;
    case SCTP_ADAPTATION_INDICATION:
        break;
    case SCTP_PARTIAL_DELIVERY_EVENT:
        break;
    case SCTP_AUTHENTICATION_EVENT:
        break;
    case SCTP_SENDER_DRY_EVENT:
        break;
    case SCTP_NOTIFICATIONS_STOPPED_EVENT:
        break;
    case SCTP_SEND_FAILED_EVENT:
        handle_send_failed_event(&(notif->sn_send_failed_event));
        break;
    case SCTP_STREAM_RESET_EVENT:
        break;
    case SCTP_ASSOC_RESET_EVENT:
        break;
    case SCTP_STREAM_CHANGE_EVENT:
        break;
    default:
        break;
    }
}

static void debug_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}
