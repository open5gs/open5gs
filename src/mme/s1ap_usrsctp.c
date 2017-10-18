#define TRACE_MODULE _s1ap_usrsctp

#include "core_debug.h"
#include "core_thread.h"

#include "mme_event.h"

#include "s1ap_path.h"

#if HAVE_USRSCTP_H
#undef SCTP_DEBUG
#include <usrsctp.h>
#endif

#define LOCAL_UDP_PORT 8277

static int s1ap_usrsctp_thread_should_stop = 0;
static thread_id s1ap_usrsctp_thread;

struct socket *s1ap_usrsctp_connect(c_uint32_t addr);

static void *THREAD_FUNC s1ap_usrsctp_accept_cb(thread_id id, void *data);
static int s1ap_usrsctp_recv_cb(struct socket *sock,
        union sctp_sockstore addr, void *data, size_t datalen,
        struct sctp_rcvinfo rcv, int flags, void *ulp_info);

static void debug_printf(const char *format, ...);

status_t s1ap_open(void)
{
    char buf[INET_ADDRSTRLEN];

    struct socket *psock = NULL;
    struct sockaddr_in local_addr;
	struct sctp_setadaptation ind = {0};

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

    memset((void *)&local_addr, 0, sizeof(struct sockaddr_in));
    local_addr.sin_family = AF_INET;
    local_addr.sin_len = sizeof(struct sockaddr_in);
    local_addr.sin_port = htons(mme_self()->s1ap_port);
#if 1 /* FIXME : At this time, I'll just test using loopback */
    mme_self()->s1ap_addr = inet_addr("127.0.0.1");
#endif
    local_addr.sin_addr.s_addr = mme_self()->s1ap_addr;

    if (usrsctp_bind(psock, (struct sockaddr *)&local_addr,
                sizeof(struct sockaddr_in)) == -1)
    {
        d_error("usrsctp_bind failed(%s:%d)",
            INET_NTOP(&mme_self()->s1ap_addr, buf), mme_self()->s1ap_port);
        return CORE_ERROR;
    }

	if (usrsctp_setsockopt(psock, IPPROTO_SCTP, SCTP_ADAPTATION_LAYER,
        (const void*)&ind, (socklen_t)sizeof(struct sctp_setadaptation)) < 0)
    {
		perror("setsockopt");
	}

    if (usrsctp_listen(psock, 1) < 0)
    {
        d_error("usrsctp_listen failed");
        return CORE_ERROR;
    }

#if 0
    if (thread_create(&s1ap_usrsctp_thread,
                NULL, s1ap_usrsctp_accept_cb, NULL) != CORE_OK)
    {
        d_error("accept thread creation failed");
        return CORE_ERROR;
    }
#endif

    d_trace(1, "s1_enb_listen() %s:%d\n", 
        INET_NTOP(&mme_self()->s1ap_addr, buf), mme_self()->s1ap_port);

    return CORE_OK;
}

status_t s1ap_close()
{
    struct socket *dummy;

    d_assert(mme_self(), return CORE_ERROR, "Null param");
    d_assert(mme_self()->s1ap_sock != NULL, return CORE_ERROR,
            "S1-ENB path already opened");

    s1ap_usrsctp_thread_should_stop = 1;
    dummy = s1ap_usrsctp_connect(mme_self()->s1ap_addr);
    thread_delete(s1ap_usrsctp_thread);
    usrsctp_close(dummy);
    usrsctp_close((struct socket *)mme_self()->s1ap_sock);
    core_sleep(time_from_msec(100));
    d_assert(usrsctp_finish() == 0, , "failed to execute 'usrsctp_finish()'");

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
    struct sockaddr_in remote_addr;
	struct sctp_sndinfo sndinfo;

    d_assert(s, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    memset((void *)&remote_addr, 0, sizeof(struct sockaddr_in));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_len = sizeof(struct sockaddr_in);
    remote_addr.sin_port = htons(port);
    remote_addr.sin_addr.s_addr = addr;

    memset((void *)&sndinfo, 0, sizeof(struct sctp_sndinfo));
	sndinfo.snd_ppid = SCTP_S1AP_PPID;
    sent = usrsctp_sendv(psock, pkbuf->payload, pkbuf->len, 
            (struct sockaddr *)&remote_addr, 1,
            (void *)&sndinfo, (socklen_t)sizeof(struct sctp_sndinfo),
            SCTP_SENDV_SNDINFO, 0);

    d_trace(10,"Sent %d->%d bytes to [%s:%d]\n", 
            pkbuf->len, sent, INET_NTOP(&addr, buf), port);
    d_trace_hex(10, pkbuf->payload, pkbuf->len);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("net_send error (%d:%s)", 
                s->sndrcv_errno, strerror(s->sndrcv_errno));
        return CORE_ERROR;
    }
    pkbuf_free(pkbuf);

    return CORE_OK;
}

struct socket *s1ap_usrsctp_connect(c_uint32_t addr)
{
    struct sockaddr_in remote_addr;
    struct socket *psock = NULL;

    if (!(psock = usrsctp_socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP,
                    NULL, NULL, 0, NULL)))
    {
        d_error("usrsctp_socket error");
        return NULL;
    }

    memset((void *)&remote_addr, 0, sizeof(struct sockaddr_in));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_len = sizeof(struct sockaddr_in);
    remote_addr.sin_port = htons(mme_self()->s1ap_port);
    remote_addr.sin_addr.s_addr = addr;
    if (usrsctp_connect(psock, (struct sockaddr *)&remote_addr,
                sizeof(struct sockaddr_in)) == -1)
    {
        d_error("usrsctp_connect error");
        return NULL;
    }

    return psock;
}

static void *THREAD_FUNC s1ap_usrsctp_accept_cb(thread_id id, void *data)
{
    char buf[INET_ADDRSTRLEN];
    struct socket *psock = (struct socket *)mme_self()->s1ap_sock;

    struct sockaddr_in remote_addr;
    struct socket *conn_sock;
    socklen_t addr_len;

    event_t e;
    c_uint32_t addr;
    c_uint16_t port;

    while (!s1ap_usrsctp_thread_should_stop)
    {
        memset(&remote_addr, 0, sizeof(struct sockaddr_in));
        addr_len = sizeof(struct sockaddr_in);

        if ((conn_sock = usrsctp_accept(psock,
                    (struct sockaddr *)&remote_addr, &addr_len)) == NULL)
        {
            d_error("usrsctp_accept failed");
            continue;
        }

        d_trace(1, "eNB-S1 accepted[%s] in s1_path module\n", 
            INET_NTOP(&addr, buf));

        addr = remote_addr.sin_addr.s_addr;
        port = ntohs(remote_addr.sin_port);

        event_set(&e, MME_EVT_S1AP_LO_ACCEPT);
        event_set_param1(&e, (c_uintptr_t)conn_sock);
        event_set_param2(&e, (c_uintptr_t)addr);
        event_set_param3(&e, (c_uintptr_t)port);
        mme_event_send(&e);
    }

    return NULL;
}

static int s1ap_usrsctp_recv_cb(struct socket *sock,
    union sctp_sockstore addr, void *data, size_t datalen,
    struct sctp_rcvinfo rcv, int flags, void *ulp_info)
{
    event_t e;
    pkbuf_t *pkbuf;

    if (data == NULL)
    {
        return 1;
    }

    if (datalen == 0)
    {
        return 1;
    }

    pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    d_assert(pkbuf, return 1, );

    pkbuf->len = datalen;
    memcpy(pkbuf->payload, data, pkbuf->len);

    d_print_hex(pkbuf->payload, pkbuf->len);

    event_set(&e, MME_EVT_S1AP_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)sock);
    event_set_param2(&e, (c_uintptr_t)pkbuf);
    mme_event_send(&e);

    free(data);
    return 1;
}

static void debug_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}
