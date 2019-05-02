/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-sctp.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_sock_domain

static int subscribe_to_events(ogs_sock_t *sock);
static int set_paddrparams(ogs_sock_t *sock, uint32_t spp_hbinterval);
static int set_rtoinfo(ogs_sock_t *sock,
        uint32_t srto_initial, uint32_t srto_min, uint32_t srto_max);
static int set_initmsg(ogs_sock_t *sock,
        uint32_t sinit_num_ostreams, uint32_t sinit_max_instreams,
        uint32_t sinit_max_attempts, uint32_t sinit_max_init_timeo);

static int sctp_num_ostreams = -1;

void ogs_sctp_set_num_ostreams(int sctp_streams)
{
    sctp_num_ostreams = sctp_streams;
}

ogs_sock_t *ogs_sctp_socket(int family, int type)
{
    ogs_sock_t *new = NULL;
    int rv;

    new = ogs_sock_socket(family, type, IPPROTO_SCTP);
    ogs_assert(new);

    rv = subscribe_to_events(new);
    ogs_assert(rv == OGS_OK);

    /* heartbit interval : 5 secs */
    rv = set_paddrparams(new, 5000);
    ogs_assert(rv == OGS_OK);

    /*
     * RTO info
     * 
     * initial : 3 secs
     * min : 1 sec
     * max : 5 secs
     */
    rv = set_rtoinfo(new, 3000, 1000, 5000);
    ogs_assert(rv == OGS_OK);

    /*
     * INITMSG
     * 
     * max number of input streams : 65535
     * max attemtps : 4
     * max initial timeout : 8 secs
     */
    rv = set_initmsg(new, sctp_num_ostreams, 65535, 4, 8000);
    ogs_assert(rv == OGS_OK);

    return new;
}

ogs_sock_t *ogs_sctp_server(int type, ogs_sockaddr_t *sa_list)
{
    int rv;
    ogs_sock_t *new;
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    addr = sa_list;
    while (addr) {
        new = ogs_sctp_socket(addr->c_sa_family, type);
        if (new) {
            rv = ogs_listen_reusable(new->fd);
            ogs_assert(rv == OGS_OK);

            if (ogs_sock_bind(new, addr) == OGS_OK) {
                ogs_debug("sctp_server() [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sock_destroy(new);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "sctp_server() [%s]:%d failed",
                OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));
        return NULL;
    }

    rv = ogs_sock_listen(new);
    ogs_assert(rv == OGS_OK);

    return new;
}

ogs_sock_t *ogs_sctp_client(int type, ogs_sockaddr_t *sa_list)
{
    ogs_sock_t *new = NULL;
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    addr = sa_list;
    while (addr) {
        new = ogs_sctp_socket(addr->c_sa_family, type);
        if (new) {
            if (ogs_sock_connect(new, addr) == OGS_OK) {
                ogs_debug("sctp_client() [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sock_destroy(new);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "sctp_client() [%s]:%d failed",
                OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));
        return NULL;
    }

    return new;
}

int ogs_sctp_connect(ogs_sock_t *sock, ogs_sockaddr_t *sa_list)
{
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(sock);

    addr = sa_list;
    while (addr) {
        if (ogs_sock_connect(sock, addr) == OGS_OK) {
            ogs_debug("sctp_connect() [%s]:%d",
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
            break;
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "sctp_connect() [%s]:%d failed",
                OGS_ADDR(sa_list, buf), OGS_PORT(sa_list));
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_sctp_sendmsg(ogs_sock_t *sock, const void *msg, size_t len,
        ogs_sockaddr_t *to, uint32_t ppid, uint16_t stream_no)
{
    int size;
    socklen_t addrlen = 0;

    ogs_assert(sock);

    if (to)
        addrlen = ogs_sockaddr_len(to);
    
    size = sctp_sendmsg(sock->fd, msg, len,
            to ? &to->sa : NULL, addrlen,
            htonl(ppid),
            0,  /* flags */
            stream_no,
            0,  /* timetolive */
            0); /* context */
    if (size < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "sctp_sendmsg(len:%d) failed", (int)len);
    }

    return size;
}

int ogs_sctp_recvmsg(ogs_sock_t *sock, void *msg, size_t len,
        ogs_sockaddr_t *from, ogs_sctp_info_t *sinfo, int *msg_flags)
{
    int size;
    socklen_t addrlen = sizeof(struct sockaddr_storage);

    int flags = 0;
    struct sctp_sndrcvinfo sndrcvinfo;

    ogs_assert(sock);

    size = sctp_recvmsg(sock->fd, msg, len,
                from ? &from->sa : NULL,  from ? &addrlen : NULL,
                &sndrcvinfo, &flags);
    if (size < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "sctp_recvmsg(%d) failed", size);
        return size;
    }

    if (msg_flags) {
        *msg_flags = flags;
    }

    if (sinfo) {
        sinfo->ppid = ntohl(sndrcvinfo.sinfo_ppid);
        sinfo->stream_no = sndrcvinfo.sinfo_stream;
    }

    return size;
}

int ogs_sctp_recvdata(ogs_sock_t *sock, void *msg, size_t len,
        ogs_sockaddr_t *from, ogs_sctp_info_t *sinfo)
{
    int size;
    int flags = 0;

    do {
        size = ogs_sctp_recvmsg(sock, msg, len, from, sinfo, &flags);
        if (size < 0) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                    "ogs_sctp_recvdata(%d)", size);
            return size;
        }

        if (flags & MSG_NOTIFICATION) {
            /* Nothing */
        }
        else if (flags & MSG_EOR) {
            break;
        }
        else {
            ogs_assert_if_reached();
        }
    } while(1);

    return size;
}

static int subscribe_to_events(ogs_sock_t *sock)
{
    struct sctp_event_subscribe event;

    ogs_assert(sock);

    memset(&event, 0, sizeof(event));
    event.sctp_data_io_event = 1;
    event.sctp_association_event = 1;
    event.sctp_send_failure_event = 1;
    event.sctp_shutdown_event = 1;

    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_EVENTS,
                            &event, sizeof(event)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "Unable to subscribe to SCTP events");
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int set_paddrparams(ogs_sock_t *sock, uint32_t spp_hbinterval)
{
    struct sctp_paddrparams heartbeat;
    socklen_t socklen;

    ogs_assert(sock);

    memset(&heartbeat, 0, sizeof(heartbeat));
    socklen = sizeof(heartbeat);
    if (getsockopt(sock->fd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                            &heartbeat, &socklen) != 0 ) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "getsockopt for SCTP_PEER_ADDR failed");
        return OGS_ERROR;
    }

    ogs_trace("Old spp _flags = 0x%x hbinter = %d pathmax = %d",
            heartbeat.spp_flags,
            heartbeat.spp_hbinterval,
            heartbeat.spp_pathmaxrxt);

    heartbeat.spp_hbinterval = spp_hbinterval;

    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                            &heartbeat, sizeof( heartbeat)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt for SCTP_PEER_ADDR_PARAMS failed");
        return OGS_ERROR;
    }

    ogs_trace("New spp _flags = 0x%x hbinter = %d pathmax = %d",
            heartbeat.spp_flags,
            heartbeat.spp_hbinterval,
            heartbeat.spp_pathmaxrxt);

    return OGS_OK;
}

static int set_rtoinfo(ogs_sock_t *sock,
        uint32_t srto_initial, uint32_t srto_min, uint32_t srto_max)
{
    struct sctp_rtoinfo rtoinfo;
    socklen_t socklen;

    ogs_assert(sock);

    memset(&rtoinfo, 0, sizeof(rtoinfo));
    socklen = sizeof(rtoinfo);
    if (getsockopt(sock->fd, IPPROTO_SCTP, SCTP_RTOINFO,
                            &rtoinfo, &socklen) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "getsockopt for SCTP_RTOINFO failed");
        return OGS_ERROR;
    }

    ogs_trace("Old RTO (initial:%d max:%d min:%d)",
            rtoinfo.srto_initial,
            rtoinfo.srto_max,
            rtoinfo.srto_min);

    rtoinfo.srto_initial = srto_initial;
    rtoinfo.srto_min = srto_min;
    rtoinfo.srto_max = srto_max;

    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_RTOINFO,
                            &rtoinfo, sizeof(rtoinfo)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt for SCTP_RTOINFO failed");
        return OGS_ERROR;
    }
    ogs_trace("New RTO (initial:%d max:%d min:%d)",
            rtoinfo.srto_initial,
            rtoinfo.srto_max,
            rtoinfo.srto_min);

    return OGS_OK;
}

static int set_initmsg(ogs_sock_t *sock,
        uint32_t sinit_num_ostreams, uint32_t sinit_max_instreams,
        uint32_t sinit_max_attempts, uint32_t sinit_max_init_timeo)
{
    struct sctp_initmsg initmsg;
    socklen_t socklen;

    ogs_assert(sock);
    ogs_assert(sinit_num_ostreams > 1);

    memset(&initmsg, 0, sizeof(initmsg));
    socklen = sizeof(initmsg);
    if (getsockopt(sock->fd, IPPROTO_SCTP, SCTP_INITMSG,
                            &initmsg, &socklen) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "getsockopt for SCTP_INITMSG failed");
        return OGS_ERROR;
    }

    ogs_trace("Old INITMSG (numout:%d maxin:%d maxattempt:%d maxinit_to:%d)",
                initmsg.sinit_num_ostreams,
                initmsg.sinit_max_instreams,
                initmsg.sinit_max_attempts,
                initmsg.sinit_max_init_timeo);

    initmsg.sinit_num_ostreams = sinit_num_ostreams;
    initmsg.sinit_max_instreams = sinit_max_instreams;
    initmsg.sinit_max_attempts = sinit_max_attempts;
    initmsg.sinit_max_init_timeo = sinit_max_init_timeo;

    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_INITMSG,
                            &initmsg, sizeof(initmsg)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt for SCTP_INITMSG failed");
        return OGS_ERROR;
    }

    ogs_trace("New INITMSG (numout:%d maxin:%d maxattempt:%d maxinit_to:%d)",
                initmsg.sinit_num_ostreams,
                initmsg.sinit_max_instreams,
                initmsg.sinit_max_attempts,
                initmsg.sinit_max_init_timeo);

    return OGS_OK;
}
