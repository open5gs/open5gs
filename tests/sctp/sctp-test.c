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
#include "core/abts.h"

#define DATASTR "This is a test"
#define STRLEN 8092
#define TEST1_PORT 7711
#define TEST1_PORT2 7712
#define TEST2_PORT 7721
#define TEST3_PORT 7731
#define TEST4_PORT 7741
#define TEST5_PORT 7751
#define TEST5_PORT2 7752
#define PPID 12345

#ifndef AI_PASSIVE
#define AI_PASSIVE 1
#endif

static void test1_func(abts_case *tc, void *data)
{
    ogs_sock_t *sctp;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;
    int rv;

    sctp = ogs_sctp_socket(AF_INET6, SOCK_SEQPACKET, NULL);
    ABTS_PTR_NOTNULL(tc, sctp);
    ogs_sctp_destroy(sctp);

    rv = ogs_getaddrinfo(&addr, AF_INET, NULL, TEST1_PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    sctp = ogs_sctp_server(SOCK_STREAM, node);
    ABTS_PTR_NOTNULL(tc, sctp);
    ogs_socknode_free(node);

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, NULL, TEST1_PORT2, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    sctp = ogs_sctp_server(SOCK_SEQPACKET, node);
    ABTS_PTR_NOTNULL(tc, sctp);
    ogs_socknode_free(node);
}

static ogs_thread_t *test2_thread;
static void test2_main(void *data)
{
    abts_case *tc = data;
    int rv;
    ogs_sock_t *sctp;
    char str[STRLEN];
    ssize_t size;
    uint32_t ppid;
    ogs_sctp_info_t sinfo;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;
    ogs_sockaddr_t from;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, NULL, TEST2_PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    sctp = ogs_sctp_client(SOCK_SEQPACKET, node);
    ABTS_PTR_NOTNULL(tc, sctp);

    size = ogs_sctp_recvdata(sctp, str, STRLEN, &from, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
#if !HAVE_USRSCTP /* FIXME : USRSCTP is not working */
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);
#endif

    ogs_socknode_free(node);
}

static void test2_func(abts_case *tc, void *data)
{
    int rv;
    ogs_sock_t *sctp, *sctp2;
    ssize_t size;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;

    rv = ogs_getaddrinfo(&addr, AF_INET6, NULL, TEST2_PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    sctp = ogs_sctp_server(SOCK_STREAM, node);
    ABTS_PTR_NOTNULL(tc, sctp);

    test2_thread = ogs_thread_create(test2_main, tc);
    ABTS_PTR_NOTNULL(tc, test2_thread);

    sctp2 = ogs_sctp_accept(sctp);
    ABTS_PTR_NOTNULL(tc, sctp2);

    size = ogs_sctp_sendmsg(sctp2, DATASTR, strlen(DATASTR), NULL,  PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    ogs_thread_destroy(test2_thread);

    ogs_sctp_destroy(sctp2);
    ogs_socknode_free(node);
}

static ogs_thread_t *test3_thread;
static void test3_main(void *data)
{
    abts_case *tc = data;
    int rv;
    ogs_sock_t *sctp;
    ogs_sockaddr_t *addr;
    ogs_sockaddr_t *to;
    char str[STRLEN];
    ssize_t size;
    int rc;

    sctp = ogs_sctp_socket(AF_INET, SOCK_SEQPACKET, NULL);
    ABTS_PTR_NOTNULL(tc, sctp);

    rv = ogs_getaddrinfo(&to, AF_INET, NULL, TEST3_PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    size = ogs_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR), to, PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    rv = ogs_freeaddrinfo(to);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_sctp_destroy(sctp);
}

static void test3_func(abts_case *tc, void *data)
{
    ogs_sock_t *sctp;
    int rv;
    ssize_t size;
    ogs_sockaddr_t from, *addr;
    ogs_socknode_t *node;
    char str[STRLEN];
    char buf[OGS_ADDRSTRLEN];
    ogs_sctp_info_t sinfo;

    rv = ogs_getaddrinfo(&addr, AF_INET, NULL, TEST3_PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    sctp = ogs_sctp_server(SOCK_SEQPACKET, node);
    ABTS_PTR_NOTNULL(tc, sctp);

    test3_thread = ogs_thread_create(test3_main, tc);
    ABTS_PTR_NOTNULL(tc, test3_thread);

    size = ogs_sctp_recvdata(sctp, str, STRLEN, &from, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
#if !HAVE_USRSCTP /* FIXME : USRSCTP is not working */
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);
#endif
    
    ogs_thread_destroy(test3_thread);

    ogs_socknode_free(node);
}

static ogs_thread_t *test4_thread;
static void test4_main(void *data)
{
    int rv;
    abts_case *tc = data;
    ogs_sock_t *sctp;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;
    char str[STRLEN];
    ssize_t size;
    ogs_sctp_info_t sinfo;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, NULL, TEST4_PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    sctp = ogs_sctp_client(SOCK_STREAM, node);
    ABTS_PTR_NOTNULL(tc, sctp);

    size = ogs_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR), NULL, PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    size = ogs_sctp_recvdata(sctp, str, STRLEN, NULL, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
#if !HAVE_USRSCTP /* FIXME : USRSCTP is not working */
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);
#endif

    ogs_socknode_free(node);
}

static void test4_func(abts_case *tc, void *data)
{
    int rv;
    ogs_sock_t *sctp;
    ssize_t size;
    ogs_sockaddr_t from, *addr;
    ogs_socknode_t *node;
    char str[STRLEN];
    ogs_sctp_info_t sinfo;
    char buf[OGS_ADDRSTRLEN];

    rv = ogs_getaddrinfo(&addr, AF_INET6, NULL, TEST4_PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    sctp = ogs_sctp_server(SOCK_SEQPACKET, node);
    ABTS_PTR_NOTNULL(tc, sctp);

    test4_thread = ogs_thread_create(test4_main, tc);
    ABTS_PTR_NOTNULL(tc, test4_thread);

    size = ogs_sctp_recvdata(sctp, str, STRLEN, &from, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "::1", OGS_ADDR(&from, buf));
#if !HAVE_USRSCTP /* FIXME : USRSCTP is not working */
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);
#endif

    size = ogs_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR), &from, PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    ogs_thread_destroy(test4_thread);
    ogs_socknode_free(node);
}

static ogs_thread_t *test5_thread;
static void test5_main(void *data)
{
    abts_case *tc = data;
    int rv;
    ogs_sock_t *sctp;
    char str[STRLEN];
    ogs_sockaddr_t from, *remote_addr, *addr;
    ogs_socknode_t *node;
    ogs_sctp_info_t sinfo;
    ssize_t size;
    char buf[OGS_ADDRSTRLEN];

    rv = ogs_getaddrinfo(&addr, AF_INET6, NULL, TEST5_PORT2, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    sctp = ogs_sctp_server(SOCK_SEQPACKET, node);
    ABTS_PTR_NOTNULL(tc, sctp);

    rv = ogs_getaddrinfo(&addr, AF_INET6, NULL, TEST5_PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_sctp_connect(sctp, addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
#if !HAVE_USRSCTP /* FIXME : libusrsctp 0.9.3.0 does not support remote_addr */
    remote_addr = &sctp->remote_addr;
    ABTS_STR_EQUAL(tc, "::1", OGS_ADDR(remote_addr, buf));

    size = ogs_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR),
            remote_addr, PPID, 0);
#else
    size = ogs_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR),
            addr, PPID, 0);
#endif
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    size = ogs_sctp_recvdata(sctp, str, STRLEN, &from, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "::1", OGS_ADDR(&from, buf));
#if !HAVE_USRSCTP /* FIXME : USRSCTP is not working */
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);
#endif

    ogs_socknode_free(node);
}

static void test5_func(abts_case *tc, void *data)
{
    int rv;
    ogs_sock_t *sctp;
    ssize_t size;
    ogs_sockaddr_t from;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;
    socklen_t addrlen;
    char str[STRLEN];
    ogs_sctp_info_t sinfo;
    char buf[OGS_ADDRSTRLEN];

    rv = ogs_getaddrinfo(&addr, AF_INET6, NULL, TEST5_PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    sctp = ogs_sctp_server(SOCK_SEQPACKET, node);
    ABTS_PTR_NOTNULL(tc, sctp);

    test5_thread = ogs_thread_create(test5_main, tc);
    ABTS_PTR_NOTNULL(tc, test5_thread);

    size = ogs_sctp_recvdata(sctp, str, STRLEN, &from, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "::1", OGS_ADDR(&from, buf));
#if !HAVE_USRSCTP /* FIXME : USRSCTP is not working */
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);
#endif

    size = ogs_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR), &from,
            sinfo.ppid, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    ogs_thread_destroy(test5_thread);
    ogs_socknode_free(node);
}

abts_suite *test_sctp(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);
    abts_run_test(suite, test4_func, NULL);
    abts_run_test(suite, test5_func, NULL);

    return suite;
}
