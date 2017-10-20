#define TRACE_MODULE _testsctp

#include "core_debug.h"
#include "core_pkbuf.h"

#include "mme_context.h"
#include "s1ap_path.h"

net_sock_t *tests1ap_enb_connect(void)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    mme_context_t *mme = mme_self();
    net_sock_t *sock = NULL;

    if (!mme) return NULL;

    rv = net_open_ext(&sock, mme->s1ap_addr, 
            INET_NTOP(&mme->s1ap_addr, buf), 0, mme->s1ap_port, 
            SOCK_SEQPACKET, IPPROTO_SCTP, SCTP_S1AP_PPID, 0);
    if (rv != CORE_OK) return NULL;

    return sock;
}

status_t tests1ap_enb_close(net_sock_t *sock)
{
    return net_close(sock);
}

int tests1ap_enb_send(net_sock_t *sock, pkbuf_t *sendbuf)
{
    return s1ap_sendto(sock, sendbuf, mme_self()->s1ap_addr,
            mme_self()->s1ap_port);
}

int tests1ap_enb_read(net_sock_t *sock, pkbuf_t *recvbuf)
{
    int rc = 0;

    while(1)
    {
        rc = net_read(sock, recvbuf->payload, recvbuf->len, 0);
        if (rc == -2) 
        {
            continue;
        }
        else if (rc <= 0)
        {
            if (sock->sndrcv_errno == EAGAIN)
            {
                continue;
            }
            break;
        }
        else
        {
            break;
        }
    }

    return rc;
}
