#define TRACE_MODULE _testusrsctp

#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"
#include "core_network.h"

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

status_t tests1ap_enb_connect(sock_id *new)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    mme_context_t *mme = mme_self();
    struct sockaddr_in remote_addr;
    struct socket *psock = NULL;
    const int on = 1;

    if (!mme) return CORE_ERROR;

    /* You should not change SOCK_STREAM to SOCK_SEQPACKET at this time.
     * if you wanna to use SOCK_SEQPACKET, you need to update s1ap_sendto() */
    if (!(psock = usrsctp_socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP,
                    NULL, NULL, 0, NULL)))
    {
        d_error("usrsctp_socket error");
        return CORE_ERROR;
    }

    if (usrsctp_setsockopt(psock, IPPROTO_SCTP, SCTP_RECVRCVINFO,
                &on, sizeof(int)) < 0)
    {
        d_error("usrsctp_setsockopt SCTP_RECVRCVINFO failed");
        return CORE_ERROR;
    }

    memset((void *)&remote_addr, 0, sizeof(struct sockaddr_in));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_len = sizeof(struct sockaddr_in);
    remote_addr.sin_port = htons(mme_self()->s1ap_port);
    remote_addr.sin_addr.s_addr = mme_self()->s1ap_addr;
    if (usrsctp_connect(psock, (struct sockaddr *)&remote_addr,
                sizeof(struct sockaddr_in)) == -1)
    {
        d_error("usrsctp_connect error");
        return CORE_ERROR;
    }

    *new = (sock_id)psock;

    return CORE_OK;
}

status_t tests1ap_enb_close(sock_id id)
{
    usrsctp_close((struct socket *)id);
    return CORE_OK;
}

status_t tests1ap_enb_send(sock_id id, pkbuf_t *sendbuf)
{
    return s1ap_sendto((net_sock_t *)id, sendbuf, mme_self()->s1ap_addr,
            mme_self()->s1ap_port);
}

int tests1ap_enb_read(sock_id id, pkbuf_t *recvbuf)
{
    int rc = 0;

	struct socket *psock = (struct socket *)id;
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
