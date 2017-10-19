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

static int s1ap_usrsctp_recv_cb(struct socket *sock,
        union sctp_sockstore addr, void *data, size_t datalen,
        struct sctp_rcvinfo rcv, int flags, void *ulp_info);

static void debug_printf(const char *format, ...);

status_t s1ap_open(void)
{
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
		d_error("usrsctp_setsockopt SCTP_RECVRCVINFO");
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
			d_error("usrsctp_setsockopt SCTP_EVENT");
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

    d_trace(1, "s1_enb_listen() %s:%d\n", 
        INET_NTOP(&mme_self()->s1ap_addr, buf), mme_self()->s1ap_port);

    return CORE_OK;
}

status_t s1ap_close()
{
    d_assert(mme_self(), return CORE_ERROR, "Null param");
    d_assert(mme_self()->s1ap_sock != NULL, return CORE_ERROR,
            "S1-ENB path already opened");

    usrsctp_close((struct socket *)mme_self()->s1ap_sock);
    while(usrsctp_finish() != 0) core_sleep(time_from_msec(50));

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

static int s1ap_usrsctp_recv_cb(struct socket *sock,
    union sctp_sockstore addr, void *data, size_t datalen,
    struct sctp_rcvinfo rcv, int flags, void *ulp_info)
{
#if 0
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
#else
	char namebuf[INET6_ADDRSTRLEN];
	const char *name;
	uint16_t port;

	if (data) {
		if (flags & MSG_NOTIFICATION) {
			printf("Notification of length %d received.\n", (int)datalen);
		} else {
			switch (addr.sa.sa_family) {
#ifdef INET
			case AF_INET:
				name = inet_ntop(AF_INET, &addr.sin.sin_addr, namebuf, INET_ADDRSTRLEN);
				port = ntohs(addr.sin.sin_port);
				break;
#endif
#ifdef INET6
			case AF_INET6:
				name = inet_ntop(AF_INET6, &addr.sin6.sin6_addr, namebuf, INET6_ADDRSTRLEN),
				port = ntohs(addr.sin6.sin6_port);
				break;
#endif
			case AF_CONN:
#ifdef _WIN32
				_snprintf(namebuf, INET6_ADDRSTRLEN, "%p", addr.sconn.sconn_addr);
#else
				snprintf(namebuf, INET6_ADDRSTRLEN, "%p", addr.sconn.sconn_addr);
#endif
				name = namebuf;
				port = ntohs(addr.sconn.sconn_port);
				break;
			default:
				name = NULL;
				port = 0;
				break;
			}
			printf("Msg of length %d received from %s:%u on stream %u with SSN %u and TSN %u, PPID %u, context %u.\n",
			       (int)datalen,
			       name,
			       port,
			       rcv.rcv_sid,
			       rcv.rcv_ssn,
			       rcv.rcv_tsn,
			       ntohl(rcv.rcv_ppid),
			       rcv.rcv_context);
		}
		free(data);
	}
	return (1);
#endif
}

static void debug_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}
