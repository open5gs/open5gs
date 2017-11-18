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
        c_uint32_t sinit_max_attempts, c_uint32_t sinit_max_init_timeo);

status_t sctp_socket(sock_id *new, int family, int type)
{
    status_t rv;

    rv = sock_create(new, family, type, IPPROTO_SCTP);
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
     * max attemtps : 4
     * max initial timeout : 8 secs
     */
    rv = set_initmsg(*new, 4, 8000);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t sctp_server(sock_id *new,
        int family, int type, const char *hostname, c_uint16_t port)
{
    status_t rv;
    c_sockaddr_t *sa;
    sock_t *sock = NULL;
    char buf[CORE_ADDRSTRLEN];

    rv = core_getaddrinfo(&sa, family, hostname, port, AI_PASSIVE);
    d_assert(rv == CORE_OK && sa, return CORE_ERROR,);

    while(sa)
    {
        rv = sctp_socket(new, sa->sa.sa_family, type);
        if (rv != CORE_OK) continue;
        
        sock = (sock_t *)*new;

        d_assert(sock_setsockopt(*new, SOCK_O_REUSEADDR, 1) == CORE_OK,
                return CORE_ERROR,
                "setsockopt(%s:%d) failed(%d:%s)",
                CORE_NTOP(sa, buf), port, errno, strerror(errno));

        if (bind(sock->fd, &sa->sa, sa->sa_len) == 0)
        {
            d_trace(1, "sctp bind %s:%d\n", CORE_NTOP(sa, buf), port);
            break;
        }

        rv = sock_delete(*new);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        sa = sa->next;
    }

    if (sa == NULL)
    {
        d_error("sctp bind(%d:%s:%d) failed(%d:%s)",
                family, hostname, port, errno, strerror(errno));
        return CORE_ERROR;
    }

    rv = core_freeaddrinfo(sa);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = sock_listen(*new);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t sctp_client(sock_id *new,
        int family, int type, const char *hostname, c_uint16_t port)
{
    status_t rv;
    c_sockaddr_t *sa;
    sock_t *sock = NULL;
    char buf[CORE_ADDRSTRLEN];

    rv = core_getaddrinfo(&sa, family, hostname, port, 0);
    d_assert(rv == CORE_OK && sa, return CORE_ERROR,);

    while(sa)
    {
        rv = sctp_socket(new, sa->sa.sa_family, type);
        if (rv != CORE_OK) continue;
        
        sock = (sock_t *)*new;

        if (connect(sock->fd, &sa->sa, sa->sa_len) == 0)
        {
            d_trace(1, "sctp connect %s:%d\n", CORE_NTOP(sa, buf), port);
            break;
        }

        rv = sock_delete(*new);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        sa = sa->next;
    }

    if (sa == NULL)
    {
        d_error("sctp connect(%d:%s:%d) failed(%d:%s)",
                family, hostname, port, errno, strerror(errno));
        return CORE_ERROR;
    }

    rv = core_freeaddrinfo(sa);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

int core_sctp_sendmsg(sock_id id, const void *msg, size_t len,
        c_sockaddr_t *to, c_uint32_t ppid, c_uint16_t stream_no)
{
    sock_t *sock = (sock_t *)id;
    int size;

    d_assert(id, return -1, );
    
    size = sctp_sendmsg(sock->fd, msg, len,
            to ? &to->sa : NULL, to ? to->sa_len : 0,
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
        c_sockaddr_t *from, c_uint32_t *ppid, c_uint16_t *stream_no)
{
    sock_t *sock = (sock_t *)id;
    int size;
    socklen_t addrlen = sizeof(c_sockaddr_t);

    int flags = 0;
    struct sctp_sndrcvinfo sinfo;

    d_assert(id, return -1,);

    do
    {
        size = sctp_recvmsg(sock->fd, msg, len,
                    from ? &from->sa : NULL, from ? &addrlen : NULL,
                    &sinfo, &flags);
        if (size < 0)
        {
            d_error("sctp_recvmsg(len:%ld) failed(%d:%s)",
                    len, errno, strerror(errno));

            return size;
        }
        if (from)
            from->sa_len = addrlen;

        if (!(flags & MSG_NOTIFICATION)) 
            break;

        if (flags & MSG_EOR) 
        {
            union sctp_notification *not = (union sctp_notification *)msg;

            switch( not->sn_header.sn_type ) 
            {
                case SCTP_ASSOC_CHANGE :
                    d_trace(3, "SCTP_ASSOC_CHANGE"
                            "(type:0x%x, flags:0x%x, state:0x%x)\n", 
                            not->sn_assoc_change.sac_type,
                            not->sn_assoc_change.sac_flags,
                            not->sn_assoc_change.sac_state);

                    if (not->sn_assoc_change.sac_state == 
                            SCTP_SHUTDOWN_COMP ||
                        not->sn_assoc_change.sac_state == 
                            SCTP_COMM_LOST)
                    {
                        return CORE_SCTP_REMOTE_CLOSED;
                    }

                    if (not->sn_assoc_change.sac_state == SCTP_COMM_UP)
                        d_trace(3, "SCTP_COMM_UP\n");

                    break;
                case SCTP_SEND_FAILED :
                    d_error("SCTP_SEND_FAILED"
                            "(type:0x%x, flags:0x%x, error:0x%x)\n", 
                            not->sn_send_failed.ssf_type,
                            not->sn_send_failed.ssf_flags,
                            not->sn_send_failed.ssf_error);
                    break;
                case SCTP_SHUTDOWN_EVENT :
                    d_trace(3, "SCTP_SHUTDOWN_EVENT\n");
                    return CORE_SCTP_REMOTE_CLOSED;
                default :
                    d_error("Discarding event with unknown "
                            "flags = 0x%x, type 0x%x", 
                            flags, not->sn_header.sn_type);
                    break;
            }
        }
        else
        {
            d_error("Not engough buffer. Need more recv : 0x%x", flags);
            return CORE_ERROR;
        }
    } while(1);

    if (ppid)
    {
        *ppid = ntohl(sinfo.sinfo_ppid);
    }
    
    if (stream_no)
    {
        *stream_no = sinfo.sinfo_stream;
    }

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
        c_uint32_t sinit_max_attempts, c_uint32_t sinit_max_init_timeo)
{
    sock_t *sock = (sock_t *)id;
    struct sctp_initmsg initmsg;
    socklen_t socklen;

    d_assert(id, return CORE_ERROR,);


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
