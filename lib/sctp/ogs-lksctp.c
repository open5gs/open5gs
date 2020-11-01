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
static int set_paddrparams(ogs_sock_t *sock, ogs_sockopt_t *option);
static int set_rtoinfo(ogs_sock_t *sock, ogs_sockopt_t *option);
static int set_initmsg(ogs_sock_t *sock, ogs_sockopt_t *option);
static int set_nodelay(ogs_sock_t *sock, int on);

void ogs_sctp_init(uint16_t port)
{
}

void ogs_sctp_final(void)
{
}

ogs_sock_t *ogs_sctp_socket(int family, int type, ogs_socknode_t *node)
{
    ogs_sock_t *new = NULL;
    int rv;
    ogs_sockopt_t option = {
        .sctp.heartbit_interval = 5000,     /* 5 seconds */
        .sctp.rto_initial = 3000,           /* 3 seconds */
        .sctp.rto_min = 1000,               /* 1 seconds */
        .sctp.rto_max = 5000,               /* 5 seconds */
        .sctp.max_num_of_ostreams = DEFAULT_SCTP_MAX_NUM_OF_OSTREAMS,
        .sctp.max_num_of_istreams = 65535,
        .sctp.max_attempts = 4,
        .sctp.max_initial_timeout = 8000,   /* 8 seconds */
    };

    ogs_sctp_set_option(&option, node);

    new = ogs_sock_socket(family, type, IPPROTO_SCTP);
    ogs_assert(new);

    rv = subscribe_to_events(new);
    ogs_assert(rv == OGS_OK);

    rv = set_paddrparams(new, &option);
    ogs_assert(rv == OGS_OK);

    rv = set_rtoinfo(new, &option);
    ogs_assert(rv == OGS_OK);

    rv = set_initmsg(new, &option);
    ogs_assert(rv == OGS_OK);

    if (node) {
        if (node->option.nodelay) {
            rv = set_nodelay(new, node->option.nodelay);
            ogs_assert(rv == OGS_OK);
        }
    }

    return new;
}

ogs_sock_t *ogs_sctp_server(int type, ogs_socknode_t *node)
{
    int rv;
    ogs_sock_t *new;
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(node);
    ogs_assert(node->addr);

    addr = node->addr;
    while (addr) {
        new = ogs_sctp_socket(addr->ogs_sa_family, type, node);
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
                OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));
        return NULL;
    }

    rv = ogs_sock_listen(new);
    ogs_assert(rv == OGS_OK);

    node->sock = new;

    return new;
}

ogs_sock_t *ogs_sctp_client(int type, ogs_socknode_t *node)
{
    ogs_sock_t *new = NULL;
    ogs_sockaddr_t *addr;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(node);
    ogs_assert(node->addr);

    addr = node->addr;
    while (addr) {
        new = ogs_sctp_socket(addr->ogs_sa_family, type, node);
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
                OGS_ADDR(node->addr, buf), OGS_PORT(node->addr));
        return NULL;
    }

    node->sock = new;

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
    socklen_t addrlen = 0;

    ogs_assert(sock);

    if (to)
        addrlen = ogs_sockaddr_len(to);
    
    return sctp_sendmsg(sock->fd, msg, len,
            to ? &to->sa : NULL, addrlen,
            htobe32(ppid),
            0,  /* flags */
            stream_no,
            0,  /* timetolive */
            0); /* context */
}

int ogs_sctp_recvmsg(ogs_sock_t *sock, void *msg, size_t len,
        ogs_sockaddr_t *from, ogs_sctp_info_t *sinfo, int *msg_flags)
{
    int size;
    socklen_t addrlen = sizeof(struct sockaddr_storage);
    ogs_sockaddr_t addr;

    int flags = 0;
    struct sctp_sndrcvinfo sndrcvinfo;

    ogs_assert(sock);

    memset(&sndrcvinfo, 0, sizeof sndrcvinfo);
    memset(&addr, 0, sizeof addr);
    size = sctp_recvmsg(sock->fd, msg, len, &addr.sa, &addrlen,
                &sndrcvinfo, &flags);
    if (size < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "sctp_recvmsg(%d) failed", size);
        return size;
    }

    if (from) {
        memcpy(from, &addr, sizeof(ogs_sockaddr_t));
    }

    if (msg_flags) {
        *msg_flags = flags;
    }

    if (sinfo) {
        sinfo->ppid = be32toh(sndrcvinfo.sinfo_ppid);
        sinfo->stream_no = sndrcvinfo.sinfo_stream;
    }

    return size;
}

/* is any of the bytes from offset .. u8_size in 'u8' non-zero? return offset
 * or -1 if all zero */
static int byte_nonzero(
        const uint8_t *u8, unsigned int offset, unsigned int u8_size)
{
    int j;

    for (j = offset; j < u8_size; j++) {
        if (u8[j] != 0)
            return j;
    }

    return OGS_ERROR;
}

static int sctp_sockopt_event_subscribe_size = 0;

static int determine_sctp_sockopt_event_subscribe_size(void)
{
    uint8_t buf[256];
    socklen_t buf_len = sizeof(buf);
    int sd, rc;

    /* only do this once */
    if (sctp_sockopt_event_subscribe_size != 0)
        return 0;

    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (sd < 0)
        return sd;

    rc = getsockopt(sd, IPPROTO_SCTP, SCTP_EVENTS, buf, &buf_len);
    ogs_closesocket(sd);
    if (rc < 0)
        return rc;

    sctp_sockopt_event_subscribe_size = buf_len;

    ogs_debug("sizes of 'struct sctp_event_subscribe': "
            "compile-time %zu, kernel: %u",
            sizeof(struct sctp_event_subscribe),
            sctp_sockopt_event_subscribe_size);
    return 0;
}

/*
 * The workaround is stolen from libosmo-netif.
 * - http://osmocom.org/projects/libosmo-netif/repository/revisions/master/entry/src/stream.c
 *
 * Attempt to work around Linux kernel ABI breakage
 *
 * The Linux kernel ABI for the SCTP_EVENTS socket option has been broken
 * repeatedly.
 *  - until commit 35ea82d611da59f8bea44a37996b3b11bb1d3fd7 ( kernel < 4.11),
 *    the size is 10 bytes
 *  - in 4.11 it is 11 bytes
 *  - in 4.12 .. 5.4 it is 13 bytes
 *  - in kernels >= 5.5 it is 14 bytes
 *
 * This wouldn't be a problem if the kernel didn't have a "stupid" assumption
 * that the structure size passed by userspace will match 1:1 the length
 * of the structure at kernel compile time. In an ideal world, it would just
 * use the known first bytes and assume the remainder is all zero.
 * But as it doesn't do that, let's try to work around this */
static int sctp_setsockopt_events_linux_workaround(
        int fd, const struct sctp_event_subscribe *event)
{
    const unsigned int compiletime_size = sizeof(*event);
    int rc;

    if (determine_sctp_sockopt_event_subscribe_size() < 0) {
        ogs_error("Cannot determine SCTP_EVENTS socket option size");
        return OGS_ERROR;
    }

    if (compiletime_size == sctp_sockopt_event_subscribe_size) {
        /* no kernel workaround needed */
        return setsockopt(fd, IPPROTO_SCTP, SCTP_EVENTS,
                event, compiletime_size);
    } else if (compiletime_size < sctp_sockopt_event_subscribe_size) {
        /* we are using an older userspace with a more modern kernel
         * and hence need to pad the data */
        uint8_t buf[256];
        ogs_assert(sctp_sockopt_event_subscribe_size <= sizeof(buf));

        memcpy(buf, event, compiletime_size);
        memset(buf + sizeof(*event),
                0, sctp_sockopt_event_subscribe_size - compiletime_size);
        return setsockopt(fd, IPPROTO_SCTP, SCTP_EVENTS,
                buf, sctp_sockopt_event_subscribe_size);
    } else /* if (compiletime_size > sctp_sockopt_event_subscribe_size) */ {
        /* we are using a newer userspace with an older kernel and hence
         * need to truncate the data - but only if the caller didn't try
         * to enable any of the events of the truncated portion */
        rc = byte_nonzero((const uint8_t *)event,
                sctp_sockopt_event_subscribe_size, compiletime_size);
        if (rc >= 0) {
            ogs_error("Kernel only supports sctp_event_subscribe of %u bytes, "
                "but caller tried to enable more modern event at offset %u",
                sctp_sockopt_event_subscribe_size, rc);
            return OGS_ERROR;
        }

        return setsockopt(fd, IPPROTO_SCTP, SCTP_EVENTS, event,
                sctp_sockopt_event_subscribe_size);
    }
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

#ifdef DISABLE_SCTP_EVENT_WORKAROUND
    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_EVENTS,
                            &event, sizeof(event)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "Unable to subscribe to SCTP events");
        return OGS_ERROR;
    }
#else
    if (sctp_setsockopt_events_linux_workaround(sock->fd, &event) < 0) {
        ogs_error("couldn't activate SCTP events on FD %u", sock->fd);
        return OGS_ERROR;
    }
#endif

    return OGS_OK;
}

static int set_paddrparams(ogs_sock_t *sock, ogs_sockopt_t *option)
{
    struct sctp_paddrparams heartbeat;
    socklen_t socklen;

    ogs_assert(sock);
    ogs_assert(option);

    memset(&heartbeat, 0, sizeof(heartbeat));
    socklen = sizeof(heartbeat);
    if (getsockopt(sock->fd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                            &heartbeat, &socklen) != 0 ) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "getsockopt for SCTP_PEER_ADDR failed");
        return OGS_ERROR;
    }

    ogs_trace("OLD spp_flags = 0x%x hbinter = %d pathmax = %d",
            heartbeat.spp_flags,
            heartbeat.spp_hbinterval,
            heartbeat.spp_pathmaxrxt);

    heartbeat.spp_hbinterval = option->sctp.heartbit_interval;

    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                            &heartbeat, sizeof( heartbeat)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt for SCTP_PEER_ADDR_PARAMS failed");
        return OGS_ERROR;
    }

    ogs_trace("NEW spp_flags = 0x%x hbinter = %d pathmax = %d",
            heartbeat.spp_flags,
            heartbeat.spp_hbinterval,
            heartbeat.spp_pathmaxrxt);

    return OGS_OK;
}

static int set_rtoinfo(ogs_sock_t *sock, ogs_sockopt_t *option)
{
    struct sctp_rtoinfo rtoinfo;
    socklen_t socklen;

    ogs_assert(sock);
    ogs_assert(option);

    memset(&rtoinfo, 0, sizeof(rtoinfo));
    socklen = sizeof(rtoinfo);
    if (getsockopt(sock->fd, IPPROTO_SCTP, SCTP_RTOINFO,
                            &rtoinfo, &socklen) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "getsockopt for SCTP_RTOINFO failed");
        return OGS_ERROR;
    }

    ogs_trace("OLD RTO (initial:%d max:%d min:%d)",
            rtoinfo.srto_initial,
            rtoinfo.srto_max,
            rtoinfo.srto_min);

    rtoinfo.srto_initial = option->sctp.rto_initial;
    rtoinfo.srto_min = option->sctp.rto_min;
    rtoinfo.srto_max = option->sctp.rto_max;

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

static int set_initmsg(ogs_sock_t *sock, ogs_sockopt_t *option)
{
    struct sctp_initmsg initmsg;
    socklen_t socklen;

    ogs_assert(sock);
    ogs_assert(option);
    ogs_assert(option->sctp.max_num_of_ostreams > 1);

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

    initmsg.sinit_num_ostreams = option->sctp.max_num_of_ostreams;
    initmsg.sinit_max_instreams = option->sctp.max_num_of_istreams;
    initmsg.sinit_max_attempts = option->sctp.max_attempts;
    initmsg.sinit_max_init_timeo = option->sctp.max_initial_timeout;

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

static int set_nodelay(ogs_sock_t *sock, int on)
{
    ogs_assert(sock);

    ogs_trace("Turn on SCTP_NODELAY");
    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_NODELAY,
                &on, sizeof(on)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt for SCTP_NODELAY failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}
