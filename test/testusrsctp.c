#define TRACE_MODULE _testusrsctp

#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"

#include "mme_context.h"
#include "s1ap_path.h"

#ifndef INET
#define INET            1
#endif
#ifndef INET6
#define INET6           1
#endif
#if HAVE_USRSCTP_H
#include <usrsctp.h>
#endif

net_sock_t *tests1ap_enb_connect(void)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    mme_context_t *mme = mme_self();
    struct sockaddr_in remote_addr;
    struct socket *psock = NULL;
    const int on = 1;

    if (!mme) return NULL;

    /* You should not change SOCK_STREAM to SOCK_SEQPACKET at this time.
     * if you wanna to use SOCK_SEQPACKET, you need to update s1ap_sendto() */
    if (!(psock = usrsctp_socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP,
                    NULL, NULL, 0, NULL)))
    {
        d_error("usrsctp_socket error");
        return NULL;
    }

    if (usrsctp_setsockopt(psock, IPPROTO_SCTP, SCTP_RECVRCVINFO,
                &on, sizeof(int)) < 0)
    {
        d_error("usrsctp_setsockopt SCTP_RECVRCVINFO failed");
        return NULL;
    }

    memset((void *)&remote_addr, 0, sizeof(struct sockaddr_in));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_len = sizeof(struct sockaddr_in);
    remote_addr.sin_port = htons(mme_self()->s1ap_port);
    remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (usrsctp_connect(psock, (struct sockaddr *)&remote_addr,
                sizeof(struct sockaddr_in)) == -1)
    {
        d_error("usrsctp_connect error");
        return NULL;
    }

    return (net_sock_t *)psock;
}

status_t tests1ap_enb_close(net_sock_t *sock)
{
    usrsctp_close((struct socket *)sock);
    return CORE_OK;
}

int tests1ap_enb_send(net_sock_t *sock, pkbuf_t *sendbuf)
{
    return s1ap_sendto(sock, sendbuf, inet_addr("127.0.0.1"),
            mme_self()->s1ap_port);
}

int tests1ap_enb_read(net_sock_t *sock, pkbuf_t *recvbuf)
{
    int rc = 0;

	struct socket *psock = (struct socket *)sock;
	struct sockaddr_in addr;
	char name[INET6_ADDRSTRLEN];
	ssize_t n = 0;
	int flags = 0;
	socklen_t from_len;
	socklen_t infolen;
	struct sctp_rcvinfo rcv_info;
	unsigned int infotype = 0;

    while(1)
    {
        n = usrsctp_recvv(psock, recvbuf->payload, MAX_SDU_LEN,
                (struct sockaddr *)&addr, &from_len, (void *)&rcv_info,
                &infolen, &infotype, &flags);
        if (n > 0) {
            if (flags & MSG_NOTIFICATION)
            {
                /* Nothing to do */
            }
            else
            {
                c_uint32_t ppid = ntohl(rcv_info.rcv_ppid);
                if ((flags & MSG_EOR) && ppid == SCTP_S1AP_PPID)
                {
                    if (n > 0)
                    {
                        rc = n;
                        break;
                    }
                }
            }
        }
    }

    return rc;
}
