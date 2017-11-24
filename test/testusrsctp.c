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

status_t s1ap_usrsctp_socket(sock_id *new,
    int family, int type,
    int (*receive_cb)(struct socket *sock, union sctp_sockstore addr,
        void *data, size_t datalen, struct sctp_rcvinfo, int flags,
        void *ulp_info));
status_t s1ap_usrsctp_connect(sock_id id, c_sockaddr_t *sa);

status_t tests1ap_enb_connect(sock_id *new)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];
    c_sockaddr_t addr;

    mme_context_t *mme = mme_self();

    if (!mme) return CORE_ERROR;

    rv = s1ap_usrsctp_socket(new, AF_INET, SOCK_STREAM, NULL);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    memset(&addr, 0, sizeof(addr));
    addr.c_sa_family = AF_INET;
    addr.c_sa_port = htons(mme_self()->s1ap_port);
    addr.sin.sin_addr.s_addr = mme_self()->s1ap_addr;

    rv = s1ap_usrsctp_connect(*new, &addr);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t tests1ap_enb_close(sock_id id)
{
    s1ap_sctp_delete(id);
    return CORE_OK;
}

status_t tests1ap_enb_send(sock_id id, pkbuf_t *sendbuf)
{
    return s1ap_send(id, sendbuf, NULL);
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
                rc = n;
                break;
            }
        }
    }

    recvbuf->len = rc;
    return rc;
}
