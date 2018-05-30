#define TRACE_MODULE _sctp

#include "core_debug.h"
#include "core_arch_network.h"

#if HAVE_NETINET_SCTP_H
#include <netinet/sctp.h>
#endif

static status_t subscribe_to_events(sock_id id);
static status_t set_paddrparams(sock_id id, c_uint32_t spp_hbinterval);
static status_t set_rtoinfo(sock_id id,
        c_uint32_t srto_initial, c_uint32_t srto_min, c_uint32_t srto_max);
static status_t set_initmsg(sock_id id,
        c_uint32_t sinit_num_ostreams, c_uint32_t sinit_max_instreams,
        c_uint32_t sinit_max_attempts, c_uint32_t sinit_max_init_timeo);

static int sctp_num_ostreams = -1;

void sctp_set_num_ostreams(int sctp_streams)
{
    sctp_num_ostreams = sctp_streams;
}

status_t sctp_socket(sock_id *new, int family, int type)
{
    status_t rv;

    rv = sock_socket(new, family, type, IPPROTO_SCTP);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = subscribe_to_events(*new);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    /* heartbit interval : 5 secs */
    rv = set_paddrparams(*new, 5000);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    /*
     * RTO info
     * 
     * initial : 3 secs
     * min : 1 sec
     * max : 5 secs
     */
    rv = set_rtoinfo(*new, 3000, 1000, 5000);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    /*
     * INITMSG
     * 
     * max number of input streams : 65535
     * max attemtps : 4
     * max initial timeout : 8 secs
     */
    rv = set_initmsg(*new, sctp_num_ostreams, 65535, 4, 8000);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t sctp_server(sock_id *new, int type, c_sockaddr_t *sa_list)
{
    status_t rv;
    c_sockaddr_t *addr;
    char buf[CORE_ADDRSTRLEN];

    addr = sa_list;
    while(addr)
    {
        rv = sctp_socket(new, addr->c_sa_family, type);
        if (rv == CORE_OK)
        {
            d_assert(sock_setsockopt(*new, SOCK_O_REUSEADDR, 1) == CORE_OK,
                    return CORE_ERROR,
                    "setsockopt [%s]:%d failed(%d:%s)",
                    CORE_ADDR(addr, buf), CORE_PORT(addr),
                    errno, strerror(errno));

            if (sock_bind(*new, addr) == CORE_OK)
            {
                d_trace(1, "sctp_server() [%s]:%d\n",
                        CORE_ADDR(addr, buf), CORE_PORT(addr));
                break;
            }

            rv = sock_delete(*new);
            d_assert(rv == CORE_OK, return CORE_ERROR,);
        }

        addr = addr->next;
    }

    if (addr == NULL)
    {
        d_error("sctp_server() [%s]:%d failed(%d:%s)",
                CORE_ADDR(sa_list, buf), CORE_PORT(sa_list),
                errno, strerror(errno));
        return CORE_ERROR;
    }

    rv = sock_listen(*new);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t sctp_client(sock_id *new, int type, c_sockaddr_t *sa_list)
{
    status_t rv;
    c_sockaddr_t *addr;
    char buf[CORE_ADDRSTRLEN];

    addr = sa_list;
    while(addr)
    {
        rv = sctp_socket(new, addr->c_sa_family, type);
        if (rv == CORE_OK)
        {
            if (sock_connect(*new, addr) == CORE_OK)
            {
                d_trace(1, "sctp_client() [%s]:%d\n",
                        CORE_ADDR(addr, buf), CORE_PORT(addr));
                break;
            }

            rv = sock_delete(*new);
            d_assert(rv == CORE_OK, return CORE_ERROR,);
        }

        addr = addr->next;
    }

    if (addr == NULL)
    {
        d_error("sctp_client() [%s]:%d failed(%d:%s)",
                CORE_ADDR(sa_list, buf), CORE_PORT(sa_list),
                errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t sctp_connect(sock_id id, c_sockaddr_t *sa_list)
{
    c_sockaddr_t *addr;
    char buf[CORE_ADDRSTRLEN];

    d_assert(id, return CORE_ERROR,);

    addr = sa_list;
    while(addr)
    {
        if (sock_connect(id, addr) == CORE_OK)
        {
            d_trace(1, "sctp_connect() [%s]:%d\n",
                    CORE_ADDR(addr, buf), CORE_PORT(addr));
            break;
        }

        addr = addr->next;
    }

    if (addr == NULL)
    {
        d_error("sctp_connect() [%s]:%d failed(%d:%s)",
                CORE_ADDR(sa_list, buf), CORE_PORT(sa_list),
                errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

int core_sctp_sendmsg(sock_id id, const void *msg, size_t len,
        c_sockaddr_t *to, c_uint32_t ppid, c_uint16_t stream_no)
{
    sock_t *sock = (sock_t *)id;
    int size;
    socklen_t addrlen = 0;

    d_assert(id, return -1, );

    if (to)
        addrlen = sockaddr_len(to);
    
    size = sctp_sendmsg(sock->fd, msg, len,
            to ? &to->sa : NULL, addrlen,
            htonl(ppid),
            0,  /* flags */
            stream_no,
            0,  /* timetolive */
            0); /* context */
    if (size < 0)
    {
        d_error("sctp_sendmsg(len:%ld) failed(%d:%s)",
                len, errno, strerror(errno));
    }

    return size;
}

int core_sctp_recvmsg(sock_id id, void *msg, size_t len,
        c_sockaddr_t *from, sctp_info_t *sinfo, int *msg_flags)
{
    sock_t *sock = (sock_t *)id;
    int size;
    socklen_t addrlen = sizeof(struct sockaddr_storage);

    int flags = 0;
    struct sctp_sndrcvinfo sndrcvinfo;

    d_assert(id, return -1,);

    size = sctp_recvmsg(sock->fd, msg, len,
                from ? &from->sa : NULL,  from ? &addrlen : NULL,
                &sndrcvinfo, &flags);
    if (size < 0)
    {
        d_error("sctp_recvmsg(%d) failed(%d:%s)",
                size, errno, strerror(errno));
        return size;
    }

    if (msg_flags)
    {
        *msg_flags = flags;
    }

    if (sinfo)
    {
        sinfo->ppid = ntohl(sndrcvinfo.sinfo_ppid);
        sinfo->stream_no = sndrcvinfo.sinfo_stream;
    }

    return size;
}

int core_sctp_recvdata(sock_id id, void *msg, size_t len,
        c_sockaddr_t *from, sctp_info_t *sinfo)
{
    int size;
    int flags = 0;

    do
    {
        size = core_sctp_recvmsg(id, msg, len, from, sinfo, &flags);
        if (size < 0)
        {
            d_error("core_sctp_recvdata(%d) failed(%d:%s)",
                    size, errno, strerror(errno));
            return size;
        }

        if (flags & MSG_NOTIFICATION)
        {
            /* Nothing */
        }
        else if (flags & MSG_EOR)
        {
            break;
        }
        else
        {
            d_assert(0, return -1,);
        }

    } while(1);

    return size;
}

static status_t subscribe_to_events(sock_id id)
{
    sock_t *sock = (sock_t *)id;
    struct sctp_event_subscribe event;

    d_assert(id, return CORE_ERROR,);

    memset(&event, 0, sizeof(event));
    event.sctp_data_io_event = 1;
    event.sctp_association_event = 1;
    event.sctp_send_failure_event = 1;
    event.sctp_shutdown_event = 1;

    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_EVENTS,
                            &event, sizeof( event)) != 0 ) 
    {
        d_error("Unable to subscribe to SCTP events: (%d:%s)",
                errno, strerror( errno ));
        return CORE_ERROR;
    }

    return CORE_OK;
}

static status_t set_paddrparams(sock_id id, c_uint32_t spp_hbinterval)
{
    sock_t *sock = (sock_t *)id;
    struct sctp_paddrparams heartbeat;
    socklen_t socklen;

    d_assert(id, return CORE_ERROR,);

    memset(&heartbeat, 0, sizeof(heartbeat));
    socklen = sizeof(heartbeat);
    if (getsockopt(sock->fd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                            &heartbeat, &socklen) != 0 ) 
    {
        d_error("getsockopt for SCTP_PEER_ADDR failed(%d:%s)",
                errno, strerror(errno));
        return CORE_ERROR;
    }

    d_trace(3,"Old spp _flags = 0x%x hbinter = %d pathmax = %d\n",
            heartbeat.spp_flags,
            heartbeat.spp_hbinterval,
            heartbeat.spp_pathmaxrxt);

    heartbeat.spp_hbinterval = spp_hbinterval;

    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                            &heartbeat, sizeof( heartbeat)) != 0 ) 
    {
        d_error("setsockopt for SCTP_PEER_ADDR_PARAMS failed(%d:%s)",
                errno, strerror(errno));
        return CORE_ERROR;
    }

    d_trace(3,"New spp _flags = 0x%x hbinter = %d pathmax = %d\n",
            heartbeat.spp_flags,
            heartbeat.spp_hbinterval,
            heartbeat.spp_pathmaxrxt);

    return CORE_OK;
}

static status_t set_rtoinfo(sock_id id,
        c_uint32_t srto_initial, c_uint32_t srto_min, c_uint32_t srto_max)
{
    sock_t *sock = (sock_t *)id;
    struct sctp_rtoinfo rtoinfo;
    socklen_t socklen;

    d_assert(id, return CORE_ERROR,);

    memset(&rtoinfo, 0, sizeof(rtoinfo));
    socklen = sizeof(rtoinfo);
    if (getsockopt(sock->fd, IPPROTO_SCTP, SCTP_RTOINFO,
                            &rtoinfo, &socklen) != 0 ) 
    {
        d_error("getsockopt for SCTP_RTOINFO failed(%d:%s)",
                errno, strerror( errno ));
        return CORE_ERROR;
    }

    d_trace(3,"Old RTO (initial:%d max:%d min:%d)\n",
            rtoinfo.srto_initial,
            rtoinfo.srto_max,
            rtoinfo.srto_min);

    rtoinfo.srto_initial = srto_initial;
    rtoinfo.srto_min = srto_min;
    rtoinfo.srto_max = srto_max;

    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_RTOINFO,
                            &rtoinfo, sizeof(rtoinfo)) != 0 ) 
    {
        d_error("setsockopt for SCTP_RTOINFO failed(%d:%s)",
                errno, strerror( errno ));
        return CORE_ERROR;
    }
    d_trace(3,"New RTO (initial:%d max:%d min:%d)\n",
            rtoinfo.srto_initial,
            rtoinfo.srto_max,
            rtoinfo.srto_min);

    return CORE_OK;
}

static status_t set_initmsg(sock_id id,
        c_uint32_t sinit_num_ostreams, c_uint32_t sinit_max_instreams,
        c_uint32_t sinit_max_attempts, c_uint32_t sinit_max_init_timeo)
{
    sock_t *sock = (sock_t *)id;
    struct sctp_initmsg initmsg;
    socklen_t socklen;

    d_assert(id, return CORE_ERROR,);
    d_assert(sinit_num_ostreams > 1, return CORE_ERROR,
            "Invalid SCTP number of output streams = %d\n", sctp_num_ostreams);

    memset(&initmsg, 0, sizeof(initmsg));
    socklen = sizeof(initmsg);
    if (getsockopt(sock->fd, IPPROTO_SCTP, SCTP_INITMSG,
                            &initmsg, &socklen) != 0 ) 
    {
        d_error("getsockopt for SCTP_INITMSG failed(%d:%s)",
                errno, strerror( errno ));
        return CORE_ERROR;
    }

    d_trace(3,"Old INITMSG (numout:%d maxin:%d maxattempt:%d maxinit_to:%d)\n",
                initmsg.sinit_num_ostreams,
                initmsg.sinit_max_instreams,
                initmsg.sinit_max_attempts,
                initmsg.sinit_max_init_timeo);

    initmsg.sinit_num_ostreams = sinit_num_ostreams;
    initmsg.sinit_max_instreams = sinit_max_instreams;
    initmsg.sinit_max_attempts = sinit_max_attempts;
    initmsg.sinit_max_init_timeo = sinit_max_init_timeo;

    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_INITMSG,
                            &initmsg, sizeof(initmsg)) != 0 ) 
    {
        d_error("setsockopt for SCTP_INITMSG failed(%d:%s)",
                errno, strerror( errno ));
        return CORE_ERROR;
    }

    d_trace(3,"New INITMSG (numout:%d maxin:%d maxattempt:%d maxinit_to:%d)\n",
                initmsg.sinit_num_ostreams,
                initmsg.sinit_max_instreams,
                initmsg.sinit_max_attempts,
                initmsg.sinit_max_init_timeo);

    return CORE_OK;
}
