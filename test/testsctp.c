#define TRACE_MODULE _testsctp

#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_network.h"

#include "mme_context.h"
#include "s1ap_path.h"

#if 1 /* ADDR */
#include <arpa/inet.h>
#endif

status_t tests1ap_enb_connect(sock_id *new)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    struct in_addr mme_s1ap_addr;

    mme_context_t *mme = mme_self();
    mme_s1ap_addr.s_addr = mme->s1ap_addr;

    if (!mme) return CORE_ERROR;

    rv = sctp_client(new, AF_UNSPEC, SOCK_STREAM,
            inet_ntoa(mme_s1ap_addr), mme->s1ap_port);
    if (rv != CORE_OK) return CORE_ERROR;

    return CORE_OK;
}

status_t tests1ap_enb_close(sock_id id)
{
    return sock_delete(id);
}

status_t tests1ap_enb_send(sock_id id, pkbuf_t *sendbuf)
{
    int size = core_sctp_sendmsg(id,
            sendbuf->payload, sendbuf->len, NULL, 18, 0);
    if (size < 0 || size != sendbuf->len)
    {
        return CORE_ERROR;
    }

    pkbuf_free(sendbuf);
    return CORE_OK;
}

int tests1ap_enb_read(sock_id id, pkbuf_t *recvbuf)
{
    c_uint32_t ppid;
    int size;

    do
    {
        size = core_sctp_recvmsg(id,
            recvbuf->payload, MAX_SDU_LEN, NULL, &ppid, NULL);
    } while(size <= 0);
    recvbuf->len = size;

    return size;
}
