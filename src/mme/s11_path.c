#define TRACE_MODULE _mme_s11_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "event.h"
#include "s11_path.h"

static int _mme_s11_recv_cb(net_sock_t *net_sock, void *data)
{
    event_t e;
    int rc;
    pkbuf_t *pkbuf = NULL;
    d_assert(net_sock, return -1, "Null param");

    pkbuf = gtp_read(net_sock);
    if (pkbuf == NULL)
    {
        if (net_sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(1, "S11_PDU is received from SGW\n");
    d_trace_hex(1, pkbuf->payload, pkbuf->len);

    event_set(&e, EVT_MSG_MME_S11);
    event_set_param1(&e, (c_uintptr_t)net_sock);
    event_set_param2(&e, (c_uintptr_t)pkbuf);

    rc = event_send(mme_self()->queue_id, &e);
    if (rc <= 0)
        return rc;

    return 0;
}

status_t mme_s11_open()
{
    return gtp_open(&mme_self()->s11_sock, _mme_s11_recv_cb, NULL,
            mme_self()->sgw_remote_addr, mme_self()->s11_local_port);
}

status_t mme_s11_close()
{
    return gtp_close(mme_self()->s11_sock);
}

status_t mme_s11_send_to_sgw(pkbuf_t *pkbuf)
{
    return  gtp_send(mme_self()->s11_sock, pkbuf, 
            mme_self()->sgw_remote_addr, mme_self()->s11_remote_port);
}
