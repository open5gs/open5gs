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

void ogs_sctp_init(uint16_t port)
{
}

void ogs_sctp_final(void)
{
}

ogs_sock_t *ogs_sctp_socket(int family, int type)
{
    ogs_sock_t *new = NULL;
    int rv;

    new = ogs_sock_socket(family, type, IPPROTO_SCTP);
    if (!new) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_sock_socket(faimly:%d type:%d) failed", family, type);
        return NULL;
    }

    rv = subscribe_to_events(new);
    if (rv != OGS_OK) {
        ogs_sock_destroy(new);
        return NULL;
    }

    return new;
}

ogs_sock_t *ogs_sctp_server(
        int type, ogs_sockaddr_t *sa_list, ogs_sockopt_t *socket_option)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];

    ogs_sock_t *new = NULL;
    ogs_sockaddr_t *addr;
    ogs_sockopt_t option;

    ogs_assert(sa_list);

    ogs_sockopt_init(&option);
    if (socket_option)
        memcpy(&option, socket_option, sizeof option);

    addr = sa_list;
    while (addr) {
        new = ogs_sctp_socket(addr->ogs_sa_family, type);
        if (new) {
            rv = ogs_sctp_peer_addr_params(new, &option);
            ogs_assert(rv == OGS_OK);

            rv = ogs_sctp_rto_info(new, &option);
            ogs_assert(rv == OGS_OK);

            rv = ogs_sctp_initmsg(new, &option);
            ogs_assert(rv == OGS_OK);

            if (option.sctp_nodelay == true) {
                rv = ogs_sctp_nodelay(new, true);
                ogs_assert(rv == OGS_OK);
            } else
                ogs_warn("SCTP NO_DELAY Disabled");

            if (option.so_linger.l_onoff == true) {
                rv = ogs_sctp_so_linger(new, option.so_linger.l_linger);
                ogs_assert(rv == OGS_OK);
            }

            rv = ogs_listen_reusable(new->fd, true);
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

    ogs_assert(new);

    rv = ogs_sock_listen(new);
    ogs_assert(rv == OGS_OK);

    return new;
}

ogs_sock_t *ogs_sctp_client(
        int type, ogs_sockaddr_t *sa_list, ogs_sockopt_t *socket_option)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];

    ogs_sock_t *new = NULL;
    ogs_sockaddr_t *addr;
    ogs_sockopt_t option;

    ogs_assert(sa_list);

    ogs_sockopt_init(&option);
    if (socket_option)
        memcpy(&option, socket_option, sizeof option);

    addr = sa_list;
    while (addr) {
        new = ogs_sctp_socket(addr->ogs_sa_family, type);
        if (new) {
            rv = ogs_sctp_peer_addr_params(new, &option);
            ogs_assert(rv == OGS_OK);

            rv = ogs_sctp_rto_info(new, &option);
            ogs_assert(rv == OGS_OK);

            rv = ogs_sctp_initmsg(new, &option);
            ogs_assert(rv == OGS_OK);

            if (option.sctp_nodelay == true) {
                rv = ogs_sctp_nodelay(new, true);
                ogs_assert(rv == OGS_OK);
            } else
                ogs_warn("SCTP NO_DELAY Disabled");

            if (option.so_linger.l_onoff == true) {
                rv = ogs_sctp_so_linger(new, option.so_linger.l_linger);
                ogs_assert(rv == OGS_OK);
            }

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

    ogs_assert(new);

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

    memset(buf, 0, sizeof(buf));
    rc = getsockopt(sd, IPPROTO_SCTP, SCTP_EVENTS, buf, &buf_len);
    ogs_closesocket(sd);
    if (rc < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "getsockopt(SCTP_PEER_ADDR_PARAMS) failed [%d]", rc);
        return rc;
    }

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
static int sctp_setsockopt_event_subscribe_workaround(
        int fd, const struct sctp_event_subscribe *event_subscribe)
{
    const unsigned int compiletime_size = sizeof(*event_subscribe);
    int rc;

    if (determine_sctp_sockopt_event_subscribe_size() < 0) {
        ogs_error("Cannot determine SCTP_EVENTS socket option size");
        return OGS_ERROR;
    }

    if (compiletime_size == sctp_sockopt_event_subscribe_size) {
        /* no kernel workaround needed */
        return setsockopt(fd, IPPROTO_SCTP, SCTP_EVENTS,
                event_subscribe, compiletime_size);
    } else if (compiletime_size < sctp_sockopt_event_subscribe_size) {
        /* we are using an older userspace with a more modern kernel
         * and hence need to pad the data */
        uint8_t buf[256];
        ogs_assert(sctp_sockopt_event_subscribe_size <= sizeof(buf));

        memcpy(buf, event_subscribe, compiletime_size);
        memset(buf + sizeof(*event_subscribe),
                0, sctp_sockopt_event_subscribe_size - compiletime_size);
        return setsockopt(fd, IPPROTO_SCTP, SCTP_EVENTS,
                buf, sctp_sockopt_event_subscribe_size);
    } else /* if (compiletime_size > sctp_sockopt_event_subscribe_size) */ {
        /* we are using a newer userspace with an older kernel and hence
         * need to truncate the data - but only if the caller didn't try
         * to enable any of the events of the truncated portion */
        rc = byte_nonzero((const uint8_t *)event_subscribe,
                sctp_sockopt_event_subscribe_size, compiletime_size);
        if (rc >= 0) {
            ogs_error("Kernel only supports sctp_event_subscribe of %u bytes, "
                "but caller tried to enable more modern event at offset %u",
                sctp_sockopt_event_subscribe_size, rc);
            return OGS_ERROR;
        }

        return setsockopt(fd, IPPROTO_SCTP, SCTP_EVENTS, event_subscribe,
                sctp_sockopt_event_subscribe_size);
    }
}

static int subscribe_to_events(ogs_sock_t *sock)
{
    struct sctp_event_subscribe event_subscribe;

    ogs_assert(sock);

    memset(&event_subscribe, 0, sizeof(event_subscribe));
    event_subscribe.sctp_data_io_event = 1;
    event_subscribe.sctp_association_event = 1;
    event_subscribe.sctp_send_failure_event = 1;
    event_subscribe.sctp_shutdown_event = 1;

#ifdef DISABLE_SCTP_EVENT_WORKAROUND
    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_EVENTS,
                    &event_subscribe, sizeof(event_subscribe)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt(SCTP_EVENTS) failed");
        return OGS_ERROR;
    }
#else
    if (sctp_setsockopt_event_subscribe_workaround(
                sock->fd, &event_subscribe) < 0) {
        ogs_error("sctp_setsockopt_events_linux_workaround() failed");
        return OGS_ERROR;
    }
#endif

    return OGS_OK;
}

static int sctp_sockopt_paddrparams_size = 0;

static int determine_sctp_sockopt_paddrparams_size(void)
{
    uint8_t buf[256];
    socklen_t buf_len = sizeof(buf);
    int sd, rc;

    /* only do this once */
    if (sctp_sockopt_paddrparams_size != 0)
        return 0;

    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (sd < 0)
        return sd;

    memset(buf, 0, sizeof(buf));
    rc = getsockopt(sd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS, buf, &buf_len);
    ogs_closesocket(sd);
    if (rc < 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "getsockopt(SCTP_PEER_ADDR_PARAMS) failed [%d]", rc);
        return rc;
    }

    sctp_sockopt_paddrparams_size = buf_len;

    ogs_debug("sizes of 'struct sctp_paddrparams': "
            "compile-time %zu, kernel: %u",
            sizeof(struct sctp_paddrparams),
            sctp_sockopt_paddrparams_size);
    return 0;
}

static int sctp_setsockopt_paddrparams_workaround(
        int fd, const struct sctp_paddrparams *paddrparams)
{
    const unsigned int compiletime_size = sizeof(*paddrparams);
    int rc;

    if (determine_sctp_sockopt_paddrparams_size() < 0) {
        ogs_error("Cannot determine SCTP_PEER_ADDR_PARAMS socket option size");
        return OGS_ERROR;
    }

    if (compiletime_size == sctp_sockopt_paddrparams_size) {
        /* no kernel workaround needed */
        return setsockopt(fd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                paddrparams, compiletime_size);
    } else if (compiletime_size < sctp_sockopt_paddrparams_size) {
        /* we are using an older userspace with a more modern kernel
         * and hence need to pad the data */
        uint8_t buf[256];
        ogs_assert(sctp_sockopt_paddrparams_size <= sizeof(buf));

        memcpy(buf, paddrparams, compiletime_size);
        memset(buf + sizeof(*paddrparams),
                0, sctp_sockopt_paddrparams_size - compiletime_size);
        return setsockopt(fd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                buf, sctp_sockopt_paddrparams_size);
    } else /* if (compiletime_size > sctp_sockopt_paddrparams_size) */ {
        /* we are using a newer userspace with an older kernel and hence
         * need to truncate the data - but only if the caller didn't try
         * to enable any of the events of the truncated portion */
        rc = byte_nonzero((const uint8_t *)paddrparams,
                sctp_sockopt_paddrparams_size, compiletime_size);
        if (rc >= 0) {
            ogs_error("Kernel only supports sctp_paddrparams of %u bytes, "
                "but caller tried to enable more modern event at offset %u",
                sctp_sockopt_paddrparams_size, rc);
            return OGS_ERROR;
        }

        return setsockopt(fd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS, paddrparams,
                sctp_sockopt_paddrparams_size);
    }
}

int ogs_sctp_peer_addr_params(ogs_sock_t *sock, ogs_sockopt_t *option)
{
    struct sctp_paddrparams paddrparams;
    socklen_t socklen;

    ogs_assert(sock);
    ogs_assert(option);

    memset(&paddrparams, 0, sizeof(paddrparams));
    socklen = sizeof(paddrparams);
    if (getsockopt(sock->fd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                            &paddrparams, &socklen) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "getsockopt(SCTP_PEER_ADDR) failed");
        return OGS_ERROR;
    }

#if !defined(__FreeBSD__)
    ogs_debug("OLD spp_flags = 0x%x hbinter = %d pathmax = %d, sackdelay = %d",
            paddrparams.spp_flags,
            paddrparams.spp_hbinterval,
            paddrparams.spp_pathmaxrxt,
            paddrparams.spp_sackdelay);
#else
    ogs_debug("OLD spp_flags = 0x%x hbinter = %d pathmax = %d",
            paddrparams.spp_flags,
            paddrparams.spp_hbinterval,
            paddrparams.spp_pathmaxrxt);
#endif

    paddrparams.spp_hbinterval = option->sctp.spp_hbinterval;
#if !defined(__FreeBSD__)
    paddrparams.spp_sackdelay = option->sctp.spp_sackdelay;
#endif

#ifdef DISABLE_SCTP_EVENT_WORKAROUND
    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                            &paddrparams, sizeof(paddrparams)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt(SCTP_PEER_ADDR_PARAMS) failed");
        return OGS_ERROR;
    }
#else
    if (sctp_setsockopt_paddrparams_workaround(sock->fd, &paddrparams) < 0) {
        ogs_error("sctp_setsockopt_paddrparams_workaround() failed");
        return OGS_ERROR;
    }
#endif

#if !defined(__FreeBSD__)
    ogs_debug("NEW spp_flags = 0x%x hbinter = %d pathmax = %d, sackdelay = %d",
            paddrparams.spp_flags,
            paddrparams.spp_hbinterval,
            paddrparams.spp_pathmaxrxt,
            paddrparams.spp_sackdelay);
#else
    ogs_debug("NEW spp_flags = 0x%x hbinter = %d pathmax = %d",
            paddrparams.spp_flags,
            paddrparams.spp_hbinterval,
            paddrparams.spp_pathmaxrxt);
#endif

    return OGS_OK;
}

int ogs_sctp_rto_info(ogs_sock_t *sock, ogs_sockopt_t *option)
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

    ogs_debug("OLD RTO (initial:%d max:%d min:%d)",
            rtoinfo.srto_initial,
            rtoinfo.srto_max,
            rtoinfo.srto_min);

    rtoinfo.srto_initial = option->sctp.srto_initial;
    rtoinfo.srto_min = option->sctp.srto_min;
    rtoinfo.srto_max = option->sctp.srto_max;

    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_RTOINFO,
                            &rtoinfo, sizeof(rtoinfo)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt for SCTP_RTOINFO failed");
        return OGS_ERROR;
    }

    ogs_debug("New RTO (initial:%d max:%d min:%d)",
            rtoinfo.srto_initial,
            rtoinfo.srto_max,
            rtoinfo.srto_min);

    return OGS_OK;
}

int ogs_sctp_initmsg(ogs_sock_t *sock, ogs_sockopt_t *option)
{
    struct sctp_initmsg initmsg;
    socklen_t socklen;

    ogs_assert(sock);
    ogs_assert(option);
    ogs_assert(option->sctp.sinit_num_ostreams > 1);

    memset(&initmsg, 0, sizeof(initmsg));
    socklen = sizeof(initmsg);
    if (getsockopt(sock->fd, IPPROTO_SCTP, SCTP_INITMSG,
                            &initmsg, &socklen) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "getsockopt for SCTP_INITMSG failed");
        return OGS_ERROR;
    }

    ogs_debug("Old INITMSG (numout:%d maxin:%d maxattempt:%d maxinit_to:%d)",
                initmsg.sinit_num_ostreams,
                initmsg.sinit_max_instreams,
                initmsg.sinit_max_attempts,
                initmsg.sinit_max_init_timeo);

    initmsg.sinit_num_ostreams = option->sctp.sinit_num_ostreams;
    initmsg.sinit_max_instreams = option->sctp.sinit_max_instreams;
    initmsg.sinit_max_attempts = option->sctp.sinit_max_attempts;
    initmsg.sinit_max_init_timeo = option->sctp.sinit_max_init_timeo;

    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_INITMSG,
                            &initmsg, sizeof(initmsg)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt for SCTP_INITMSG failed");
        return OGS_ERROR;
    }

    ogs_debug("New INITMSG (numout:%d maxin:%d maxattempt:%d maxinit_to:%d)",
                initmsg.sinit_num_ostreams,
                initmsg.sinit_max_instreams,
                initmsg.sinit_max_attempts,
                initmsg.sinit_max_init_timeo);

    return OGS_OK;
}

int ogs_sctp_nodelay(ogs_sock_t *sock, int on)
{
    ogs_assert(sock);

    ogs_debug("Turn on SCTP_NODELAY");
    if (setsockopt(sock->fd, IPPROTO_SCTP, SCTP_NODELAY,
                &on, sizeof(on)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt(IPPROTO_SCTP, SCTP_NODELAY) failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_sctp_so_linger(ogs_sock_t *sock, int l_linger)
{
    ogs_assert(sock);
    return ogs_so_linger(sock->fd, l_linger);
}
