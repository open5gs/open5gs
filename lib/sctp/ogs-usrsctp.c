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

#include <stdarg.h>

#include "ogs-sctp.h"

static void ogs_debug_printf(const char *format, ...);

void ogs_sctp_init(uint16_t port)
{
    usrsctp_init(port, NULL, ogs_debug_printf);
#ifdef SCTP_DEBUG
    usrsctp_sysctl_set_sctp_debug_on(SCTP_DEBUG_ALL);
#endif
    usrsctp_sysctl_set_sctp_blackhole(2);
    usrsctp_sysctl_set_sctp_enable_sack_immediately(1);
}

void ogs_sctp_final(void)
{
    while (usrsctp_finish() != 0) {
        ogs_warn("try to finsih SCTP");
        ogs_msleep(1000);
    }
}

ogs_sock_t *ogs_sctp_socket(int family, int type)
{
    struct socket *socket = NULL;
    const int on = 1;
    struct sctp_event event;
    uint16_t event_types[] = {
        SCTP_ASSOC_CHANGE,
        SCTP_PEER_ADDR_CHANGE,
        SCTP_REMOTE_ERROR,
        SCTP_SHUTDOWN_EVENT,
        SCTP_ADAPTATION_INDICATION,
        SCTP_PARTIAL_DELIVERY_EVENT
    };
    int i;

    if (!(socket = usrsctp_socket(family, type, IPPROTO_SCTP,
                    NULL, NULL, 0, NULL))) {
        ogs_error("ogs_sctp_socket() failed");
        return NULL;
    }

    if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_RECVRCVINFO,
                &on, sizeof(int)) < 0) {
        ogs_error("usrsctp_setsockopt SCTP_RECVRCVINFO failed");
        return NULL;
    }

    memset(&event, 0, sizeof(event));
    event.se_assoc_id = SCTP_FUTURE_ASSOC;
    event.se_on = 1;
    for (i = 0; i < (int)(sizeof(event_types)/sizeof(uint16_t)); i++) {
        event.se_type = event_types[i];
        if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_EVENT,
                    &event, sizeof(struct sctp_event)) < 0) {
            ogs_error("usrsctp_setsockopt SCTP_EVENT failed");
            return NULL;
        }
    }

    return (ogs_sock_t *)socket;
}

void ogs_sctp_destroy(ogs_sock_t *sock)
{
    ogs_assert(sock);
    usrsctp_close((struct socket *)sock);
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

            if (ogs_sctp_bind(new, addr) == OGS_OK) {
                ogs_trace("sctp_server [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sctp_destroy(new);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_error("sctp_server %s failed",
                ogs_sockaddr_to_string_static(sa_list));
        return NULL;
    }

    ogs_assert(new);

    rv = ogs_sctp_listen(new);
    ogs_assert(rv == OGS_OK);

    return new;
}

ogs_sock_t *ogs_sctp_client(
        int type,
        ogs_sockaddr_t *sa_list, ogs_sockaddr_t *local_sa_list,
        ogs_sockopt_t *socket_option)
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

            if (ogs_sctp_connect(new, addr) == OGS_OK) {
                ogs_trace("sctp_client [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
                break;
            }

            ogs_sctp_destroy(new);
        }

        addr = addr->next;
    }

    if (addr == NULL) {
        ogs_error("sctp_client %s failed",
                ogs_sockaddr_to_string_static(sa_list));
        return NULL;
    }

    return new;
}

int ogs_sctp_bind(ogs_sock_t *sock, ogs_sockaddr_t *sa_list)
{
    struct socket *socket = (struct socket *)sock;
    socklen_t addrlen;

    ogs_assert(socket);
    ogs_assert(sa_list);

    addrlen = ogs_sockaddr_len(sa_list);
    ogs_assert(addrlen);

    if (usrsctp_bind(socket, &sa_list->sa, addrlen) != 0) {
        ogs_error("sctp_bind() %s failed",
                ogs_sockaddr_to_string_static(sa_list));

        return OGS_ERROR;
    }

    ogs_debug("sctp_bind() %s", ogs_sockaddr_to_string_static(sa_list));

    return OGS_OK;
}

int ogs_sctp_connect(ogs_sock_t *sock, ogs_sockaddr_t *sa_list)
{
    struct socket *socket = (struct socket *)sock;
    socklen_t addrlen;

    ogs_assert(socket);
    ogs_assert(sa_list);

    addrlen = ogs_sockaddr_len(sa_list);
    ogs_assert(addrlen);

    if (usrsctp_connect(socket, &sa_list->sa, addrlen) != 0) {
        ogs_error("sctp_connect() %s", ogs_sockaddr_to_string_static(sa_list));
        return OGS_ERROR;
    }

    ogs_debug("sctp_connect() %s", ogs_sockaddr_to_string_static(sa_list));

    return OGS_OK;
}

int ogs_sctp_listen(ogs_sock_t *sock)
{
    int rc;
    struct socket *socket = (struct socket *)sock;
    ogs_assert(socket);

    rc = usrsctp_listen(socket, 5);
    if (rc < 0) {
        ogs_error("sctp_listen() failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

ogs_sock_t *ogs_sctp_accept(ogs_sock_t *sock)
{
    struct socket *socket = (struct socket *)sock;
    struct socket *new = NULL;

    ogs_sockaddr_t addr;
    socklen_t addrlen;

    memset(&addr, 0, sizeof(addr));
    addrlen = sizeof(addr.ss);

    ogs_assert(sock);

    new = usrsctp_accept(socket, &addr.sa, &addrlen);
    if (!new) {
        ogs_error("sctp_accept() failed");
        return NULL;
    }

    return (ogs_sock_t *)new;
}

int ogs_sctp_sendmsg(ogs_sock_t *sock, const void *msg, size_t len,
        ogs_sockaddr_t *to, uint32_t ppid, uint16_t stream_no)
{
    struct socket *socket = (struct socket *)sock;
    struct sctp_sndinfo sndinfo;

    ogs_assert(socket);

    memset((void *)&sndinfo, 0, sizeof(struct sctp_sndinfo));
    sndinfo.snd_ppid = htobe32(ppid);
    sndinfo.snd_sid = stream_no;
    return usrsctp_sendv(socket, msg, len,
            to ? &to->sa : NULL, to ? 1 : 0,
            (void *)&sndinfo, (socklen_t)sizeof(struct sctp_sndinfo),
            SCTP_SENDV_SNDINFO, 0);
}

int ogs_sctp_recvmsg(ogs_sock_t *sock, void *msg, size_t len,
        ogs_sockaddr_t *from, ogs_sctp_info_t *sinfo, int *msg_flags)
{
    struct socket *socket = (struct socket *)sock;
    ogs_sockaddr_t addr;
    ssize_t n = 0;
    int flags = 0;
    socklen_t addrlen = sizeof(struct sockaddr_storage);
    socklen_t infolen;
    struct sctp_rcvinfo rcv_info;
    unsigned int infotype = 0;

    ogs_assert(socket);

    memset(&rcv_info, 0, sizeof rcv_info);
    memset(&addr, 0, sizeof addr);
    n = usrsctp_recvv(socket, msg, len,
            &addr.sa, &addrlen,
            (void *)&rcv_info,
            &infolen, &infotype, &flags);

    if (n < 0) {
        ogs_error("sctp_recvmsg(%d) failed", (int)n);
        return OGS_ERROR;
    }
    
    if (from) {
        memcpy(from, &addr, sizeof(ogs_sockaddr_t));
    }

    if (msg_flags) {
        *msg_flags = flags;
    }

    if (sinfo) {
        sinfo->ppid = be32toh(rcv_info.rcv_ppid);
        sinfo->stream_no = rcv_info.rcv_sid;
    }

    return n;
}

ogs_sockaddr_t *ogs_usrsctp_remote_addr(union sctp_sockstore *store)
{
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(store);

    addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    ogs_assert(addr);

    addr->ogs_sa_family = store->sin.sin_family;
    switch(addr->ogs_sa_family) {
    case AF_INET:
        memcpy(&addr->sin, &store->sin, sizeof(struct sockaddr_in));
        break;
    case AF_INET6:
        memcpy(&addr->sin6, &store->sin6, sizeof(struct sockaddr_in6));
        break;
    default:
        ogs_assert_if_reached();
    }

    return addr;
}

static void ogs_debug_printf(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
    vprintf(format, ap);
#pragma GCC diagnostic pop
    va_end(ap);
}

int ogs_sctp_peer_addr_params(ogs_sock_t *sock, ogs_sockopt_t *option)
{
    struct socket *socket = (struct socket *)sock;

    struct sctp_paddrparams paddrparams;
    socklen_t socklen;

    ogs_assert(socket);
    ogs_assert(option);

    memset(&paddrparams, 0, sizeof(paddrparams));
    socklen = sizeof(paddrparams);
    if (usrsctp_getsockopt(socket, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                            &paddrparams, &socklen) != 0) {
        ogs_error("getsockopt for SCTP_PEER_ADDR failed(%d:%s)",
                errno, strerror( errno ));
        return OGS_ERROR;
    }

    ogs_debug("OLD spp_flags = 0x%x hbinter = %d pathmax = %d",
            paddrparams.spp_flags,
            paddrparams.spp_hbinterval,
            paddrparams.spp_pathmaxrxt);

    paddrparams.spp_hbinterval = option->sctp.spp_hbinterval;

    if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_PEER_ADDR_PARAMS,
                            &paddrparams, sizeof(paddrparams)) != 0) {
        ogs_error("setsockopt for SCTP_PEER_ADDR_PARAMS failed(%d:%s)",
                errno, strerror( errno ));
        return OGS_ERROR;
    }

    ogs_debug("NEW spp_flags = 0x%x hbinter = %d pathmax = %d",
            paddrparams.spp_flags,
            paddrparams.spp_hbinterval,
            paddrparams.spp_pathmaxrxt);

    return OGS_OK;
}

int ogs_sctp_rto_info(ogs_sock_t *sock, ogs_sockopt_t *option)
{
    struct socket *socket = (struct socket *)sock;

    struct sctp_rtoinfo rtoinfo;
    socklen_t socklen;

    ogs_assert(sock);
    ogs_assert(option);

    memset(&rtoinfo, 0, sizeof(rtoinfo));
    socklen = sizeof(rtoinfo);
    if (usrsctp_getsockopt(socket, IPPROTO_SCTP, SCTP_RTOINFO,
                            &rtoinfo, &socklen) != 0) {
        ogs_error("getsockopt for SCTP_RTOINFO failed(%d:%s)",
                errno, strerror( errno ));
        return OGS_ERROR;
    }

    ogs_debug("OLD RTO (initial:%d max:%d min:%d)",
            rtoinfo.srto_initial,
            rtoinfo.srto_max,
            rtoinfo.srto_min);

    rtoinfo.srto_initial = option->sctp.srto_initial;
    rtoinfo.srto_min = option->sctp.srto_min;
    rtoinfo.srto_max = option->sctp.srto_max;

    if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_RTOINFO,
                            &rtoinfo, sizeof(rtoinfo)) != 0) {
        ogs_error("setsockopt for SCTP_RTOINFO failed(%d:%s)",
                errno, strerror( errno ));
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
    struct socket *socket = (struct socket *)sock;

    struct sctp_initmsg initmsg;
    socklen_t socklen;

    ogs_assert(socket);
    ogs_assert(option);
    ogs_assert(option->sctp.sinit_num_ostreams > 1);

    memset(&initmsg, 0, sizeof(struct sctp_initmsg));
    socklen = sizeof(struct sctp_initmsg);
    if (usrsctp_getsockopt(socket, IPPROTO_SCTP, SCTP_INITMSG,
                &initmsg, &socklen) != 0) {
        ogs_error("getsockopt for SCTP_INITMSG failed(%d:%s)",
                errno, strerror( errno ));
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

    if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_INITMSG,
                            &initmsg, sizeof(initmsg)) != 0) {
        ogs_error("setsockopt for SCTP_INITMSG failed(%d:%s)",
                errno, strerror( errno ));
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
    struct socket *socket = (struct socket *)sock;

    ogs_assert(socket);

    ogs_debug("Turn on SCTP_NODELAY");
    if (usrsctp_setsockopt(socket, IPPROTO_SCTP, SCTP_NODELAY,
                &on, sizeof(int)) < 0) {
        ogs_error("usrsctp_setsockopt(IPPROTO_SCTP, SCTP_NODELAY) failed");
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_sctp_so_linger(ogs_sock_t *sock, int l_linger)
{
    struct socket *socket = (struct socket *)sock;
    struct linger l;

    ogs_assert(socket);
    ogs_assert(l_linger);

    memset(&l, 0, sizeof(l));
    l.l_onoff = 1;
    l.l_linger = l_linger;
    if (usrsctp_setsockopt(socket, SOL_SOCKET, SO_LINGER,
            (const void *)&l, (socklen_t) sizeof(struct linger)) < 0) {
        ogs_error("Could not set SO_LINGER on SCTP socket");
        return OGS_ERROR;
    }

    return OGS_OK;
}
