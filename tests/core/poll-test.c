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

#include "ogs-core.h"
#include "core/abts.h"

#define DATASTR "This is a test"
#define STRLEN 8092
#define PORT 7777
#define NUM 100

#ifndef AI_PASSIVE
#define AI_PASSIVE 1
#endif

#if 0 /* FIXME : Not working in WIN32, i585 */
static int test1_called = 0;

static void test1_handler(short when, ogs_socket_t fd, void *data)
{
    abts_case *tc = data;
    char str[STRLEN];
    ssize_t size;

    ABTS_INT_EQUAL(tc, OGS_POLLIN, when);

    size = ogs_recv(fd, str, STRLEN, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    test1_called++;
}

static void test1_func(abts_case *tc, void *data)
{
    int rv;
    int i;
    ssize_t size;
    ogs_poll_t *poll[NUM];
    ogs_sockaddr_t *addr;
    ogs_socknode_t *server[NUM], *client[NUM];
    ogs_pollset_t *pollset = ogs_pollset_create(512);
    ABTS_PTR_NOTNULL(tc, pollset);

    for (i = 0; i < NUM; i++) {
        rv = ogs_getaddrinfo(&addr, AF_INET, "127.0.0.1", PORT+i, AI_PASSIVE);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
        server[i] = ogs_socknode_new(addr);
        ABTS_PTR_NOTNULL(tc, server[i]);
        server[i]->sock = ogs_udp_server(server[i]->addr, NULL);
        ABTS_PTR_NOTNULL(tc, server[i]->sock);
        rv = ogs_getaddrinfo(&addr, AF_INET, "127.0.0.1", PORT+i, AI_PASSIVE);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
        client[i] = ogs_socknode_new(addr);
        ABTS_PTR_NOTNULL(tc, client[i]);
        client[i]->sock = ogs_udp_client(client[i]->addr, NULL);
        ABTS_PTR_NOTNULL(tc, client[i]->sock);
    }

    for (i = 0; i < NUM; i+=2) {
        poll[i] = ogs_pollset_add(pollset, OGS_POLLIN,
                server[i]->sock->fd, test1_handler, tc);
        ABTS_PTR_NOTNULL(tc, poll);

        size = ogs_send(client[i]->sock->fd, DATASTR, strlen(DATASTR), 0);
        ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    }

    test1_called = 0;
    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, NUM/2, test1_called);

    for (i = 1; i < NUM; i+=2) {
        poll[i] = ogs_pollset_add(pollset, OGS_POLLIN,
                server[i]->sock->fd, test1_handler, tc);
        ABTS_PTR_NOTNULL(tc, poll);
    }

    for (i = 0; i < NUM; i++) {
        size = ogs_send(client[i]->sock->fd, DATASTR, strlen(DATASTR), 0);
        ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    }

    test1_called = 0;
    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, NUM, test1_called);

    for (i = 0; i < NUM; i+=2) {
        ogs_pollset_remove(poll[i]);
    }

    for (i = 1; i < NUM; i+=2) {
        size = ogs_send(client[i]->sock->fd, DATASTR, strlen(DATASTR), 0);
        ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    }

    test1_called = 0;
    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, NUM/2, test1_called);

    for (i = 1; i < NUM; i+=2) {
        ogs_pollset_remove(poll[i]);
    }

    rv = ogs_pollset_poll(pollset, ogs_time_from_msec(100));
    ABTS_INT_EQUAL(tc, OGS_TIMEUP, rv);

    for (i = 0; i < NUM; i++) {
        ogs_socknode_free(client[i]);
        ogs_socknode_free(server[i]);
    }

    ogs_pollset_destroy(pollset);
}
#endif

static ogs_sock_t *test2_client;
static int test2_okay = 1;

static void test2_handler(short when, ogs_socket_t fd, void *data)
{
    abts_case *tc = data;
    const char *test = "test string";
    int len;

    ABTS_INT_EQUAL(tc, OGS_POLLOUT, when);

    len = ogs_send(fd, test, (int)strlen(test) + 1, 0);

    if (len > 0) {
        ogs_sock_destroy(test2_client);
    }

    test2_okay = 0;
}

static void test2_func(abts_case *tc, void *data)
{
    int rv;
    ogs_poll_t *poll;
    ogs_sock_t *server, *client, *accept;
    ogs_sockaddr_t *addr;
    ogs_pollset_t *pollset = ogs_pollset_create(512);
    ABTS_PTR_NOTNULL(tc, pollset);

    rv = ogs_getaddrinfo(&addr, AF_INET, "127.0.0.1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    server = ogs_tcp_server(addr, NULL);
    ABTS_PTR_NOTNULL(tc, server);
    rv = ogs_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_getaddrinfo(&addr, AF_INET, "127.0.0.1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    test2_client = ogs_tcp_client(addr, NULL);
    ABTS_PTR_NOTNULL(tc, test2_client);
    rv = ogs_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    accept = ogs_sock_accept(server);
    ABTS_PTR_NOTNULL(tc, accept);

    poll = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept->fd, test2_handler, tc);
    ABTS_PTR_NOTNULL(tc, poll);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 0, test2_okay);

    ogs_pollset_remove(poll);

    ogs_sock_destroy(accept);
    ogs_sock_destroy(server);

    ogs_pollset_destroy(pollset);
}

static ogs_socket_t test3_fd[2];
static int test3_okay = 1;

static void test3_handler(short when, ogs_socket_t fd, void *data)
{
    abts_case *tc = data;
    const char *test = "test string";
    int len;

    ABTS_INT_EQUAL(tc, OGS_POLLOUT, when);

    len = ogs_send(fd, test, (int)strlen(test) + 1, 0);

    if (len > 0) {
        ogs_closesocket(test3_fd[0]);
    }
    test3_okay = 0;
}

static void test3_func(abts_case *tc, void *data)
{
    int rv;
    ogs_poll_t *poll = NULL;
    ogs_pollset_t *pollset = ogs_pollset_create(512);
    ABTS_PTR_NOTNULL(tc, pollset);

    rv = ogs_socketpair(AF_SOCKPAIR, SOCK_STREAM, 0, test3_fd);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    poll = ogs_pollset_add(pollset, OGS_POLLOUT,
            test3_fd[1], test3_handler, tc);
    ABTS_PTR_NOTNULL(tc, poll);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_pollset_remove(poll);

    ABTS_INT_EQUAL(tc, 0, test3_okay);

    ogs_closesocket(test3_fd[1]);

    ogs_pollset_destroy(pollset);
}

static ogs_thread_t *test4_thread;
static void test4_main(void *data)
{
    abts_case *tc = data;
    int rv;
    ogs_sock_t *udp;
    ogs_sockaddr_t *sa;
    ssize_t size;

    udp = ogs_sock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ABTS_PTR_NOTNULL(tc, udp);

    rv = ogs_getaddrinfo(&sa, AF_INET, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    size = ogs_sendto(udp->fd, DATASTR, strlen(DATASTR), 0, sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    rv = ogs_freeaddrinfo(sa);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_sock_destroy(udp);
}

static void test4_handler(short when, ogs_socket_t fd, void *data)
{
    abts_case *tc = data;
    ssize_t size;
    char str[STRLEN];
    ogs_sockaddr_t sa;
    char buf[OGS_ADDRSTRLEN];

    ABTS_INT_EQUAL(tc, OGS_POLLIN, when);

    size = ogs_recvfrom(fd, str, STRLEN, 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "127.0.0.1", OGS_ADDR(&sa, buf));
}

static void test4_func(abts_case *tc, void *data)
{
    int rv;
    ogs_sock_t *udp;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;
    ogs_poll_t *poll;
    ogs_pollset_t *pollset = ogs_pollset_create(512);
    ABTS_PTR_NOTNULL(tc, pollset);

    rv = ogs_getaddrinfo(&addr, AF_INET, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    udp = ogs_udp_server(addr, NULL);
    ABTS_PTR_NOTNULL(tc, udp);
    poll = ogs_pollset_add(pollset, OGS_POLLIN, udp->fd, test4_handler, tc);

    test4_thread = ogs_thread_create(test4_main, tc);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_thread_destroy(test4_thread);

    ogs_pollset_remove(poll);

    ogs_sock_destroy(udp);

    rv = ogs_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_pollset_destroy(pollset);
}

static void test5_func(abts_case *tc, void *data)
{
    int rv;
    ogs_pollset_t *pollset = ogs_pollset_create(512);
    ABTS_PTR_NOTNULL(tc, pollset);

    rv = ogs_pollset_poll(pollset, ogs_time_from_msec(100));
    ABTS_INT_EQUAL(tc, OGS_TIMEUP, rv);
    rv = ogs_pollset_notify(pollset);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_pollset_poll(pollset, ogs_time_from_msec(100));
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_pollset_poll(pollset, ogs_time_from_msec(100));
    ABTS_INT_EQUAL(tc, OGS_TIMEUP, rv);
    rv = ogs_pollset_notify(pollset);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_pollset_notify(pollset);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_pollset_notify(pollset);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_pollset_poll(pollset, ogs_time_from_msec(100));
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_pollset_poll(pollset, ogs_time_from_msec(100));
    ABTS_INT_EQUAL(tc, OGS_TIMEUP, rv);

    ogs_pollset_destroy(pollset);
}

static int test6_okay = 1;
static void test6_handler(short when, ogs_socket_t fd, void *data)
{
    abts_case *tc = data;

    ABTS_TRUE(tc, (when & OGS_POLLIN));
    test6_okay = 0;
}

static void test6_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socket_t fd[2];
    ogs_poll_t *poll = NULL;
    ogs_pollset_t *pollset = ogs_pollset_create(512);
    ABTS_PTR_NOTNULL(tc, pollset);

    rv = ogs_socketpair(AF_SOCKPAIR, SOCK_STREAM, 0, fd);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    poll = ogs_pollset_add(pollset, OGS_POLLIN, fd[1], test6_handler, tc);
    ABTS_PTR_NOTNULL(tc, poll);

    ogs_closesocket(fd[0]);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_INT_EQUAL(tc, 0, test6_okay);

    ogs_pollset_remove(poll);

    ogs_closesocket(fd[1]);

    ogs_pollset_destroy(pollset);
}

/*
static void test7_io_handler(ogs_poll_t *poll)
{
    ogs_poll_t *poll = NULL;

    poll = mme_poll_new(pollset, addr, ipv6);
    ogs_assert(poll);

    ogs_poll_queue_push(poll);
}

static void test7_queue_handler(ogs_poll_t *poll)
{
}

static void test7_queue_handler(ogs_poll_t *poll)
{
    mme_poll_data_t *data = _data;
    ogs_assert(data);

    mme_poll_free(poll);
}
*/

static void test7_func(abts_case *tc, void *data)
{
    /*
    int rv;
    ogs_pollset_t *pollset = ogs_pollset_create(512);

    ogs_poll_t *poll = NULL;
    poll = ogs_poll_new(pollset, test7_handler, tc);

    rv = ogs_socketpair(AF_SOCKPAIR, SOCK_STREAM, 0, fd);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_poll_io_register(poll, OGS_POLLIN, fd[0]);
    ogs_poll_io_unregister(poll);

    ogs_closesocket(fd[0]);
    ogs_closesocket(fd[1]);

    ogs_poll_free(poll);

    ogs_poll_t *poll = NULL;

    poll = mme_poll_new(pollset, test7_queue_handler, addr, ipv6);
    ogs_assert(poll);

    ogs_poll_queue_push(poll);
    
    ogs_poll_t *poll = NULL;

    poll = ogs_poll_timer_create(pollset, test7_handler, TIMER_ID);
    ogs_poll_timer_start(poll);

    ogs_pollset_destroy(pollset);
    */
}

static ogs_socknode_t *test8_server;
static ogs_socknode_t *test8_client1, *test8_client2, *test8_client3;
static ogs_sock_t *test8_accept1, *test8_accept2, *test8_accept3;
static int test8_okay = 1;

static void test8_handler(short when, ogs_socket_t fd, void *data)
{
    test8_okay++;
}

static void test8_handler_with_remove(short when, ogs_socket_t fd, void *data)
{
    ogs_poll_t *poll = data;

    ogs_assert(when == OGS_POLLOUT);
    ogs_assert(poll);
    ogs_pollset_remove(poll);

    test8_okay++;
}

static void test8_func(abts_case *tc, void *data)
{
    int rv;
    ogs_poll_t *write1, *write2, *write3;
    ogs_sock_t *server, *client1, *client2, *client3;
    ogs_sock_t *accept1, *accept2, *accept3;
    ogs_sockaddr_t *addr;
    ogs_pollset_t *pollset = ogs_pollset_create(512);
    ABTS_PTR_NOTNULL(tc, pollset);

    rv = ogs_getaddrinfo(&addr, AF_INET, "127.0.0.1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    server = ogs_tcp_server(addr, NULL);
    ABTS_PTR_NOTNULL(tc, server);
    rv = ogs_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_getaddrinfo(&addr, AF_INET, "127.0.0.1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    client1 = ogs_tcp_client(addr, NULL);
    ABTS_PTR_NOTNULL(tc, client1);
    rv = ogs_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    accept1 = ogs_sock_accept(server);
    ABTS_PTR_NOTNULL(tc, accept1);

    rv = ogs_getaddrinfo(&addr, AF_INET, "127.0.0.1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    client2 = ogs_tcp_client(addr, NULL);
    ABTS_PTR_NOTNULL(tc, client2);
    rv = ogs_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    accept2 = ogs_sock_accept(server);
    ABTS_PTR_NOTNULL(tc, accept2);

    rv = ogs_getaddrinfo(&addr, AF_INET, "127.0.0.1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    client3 = ogs_tcp_client(addr, NULL);
    ABTS_PTR_NOTNULL(tc, client3);
    rv = ogs_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    accept3 = ogs_sock_accept(server);
    ABTS_PTR_NOTNULL(tc, accept3);

    write1 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept1->fd, test8_handler, NULL);
    ABTS_PTR_NOTNULL(tc, write1);

    write2 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept2->fd, test8_handler, NULL);
    ABTS_PTR_NOTNULL(tc, write2);

    write3 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept3->fd, test8_handler, NULL);
    ABTS_PTR_NOTNULL(tc, write3);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 4, test8_okay);

    ogs_pollset_remove(write1);
    ogs_pollset_remove(write2);
    ogs_pollset_remove(write3);

    write2 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept2->fd, test8_handler, NULL);
    ABTS_PTR_NOTNULL(tc, write2);

    write3 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept3->fd, test8_handler, NULL);
    ABTS_PTR_NOTNULL(tc, write3);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 6, test8_okay);

    ogs_pollset_remove(write2);
    ogs_pollset_remove(write3);

    write3 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept3->fd, test8_handler, NULL);
    ABTS_PTR_NOTNULL(tc, write3);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 7, test8_okay);

    ogs_pollset_remove(write3);

    write1 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept1->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write1);

    write2 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept2->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write2);

    write3 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept3->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write3);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 10, test8_okay);

    write1 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept1->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write1);

    write2 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept2->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write2);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 12, test8_okay);

    write1 = ogs_pollset_add(pollset, OGS_POLLOUT,
            accept1->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write1);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 13, test8_okay);

    write1 = ogs_pollset_add(pollset, OGS_POLLIN|OGS_POLLOUT,
            accept1->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write1);

    write2 = ogs_pollset_add(pollset, OGS_POLLIN|OGS_POLLOUT,
            accept2->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write2);

    write3 = ogs_pollset_add(pollset, OGS_POLLIN|OGS_POLLOUT,
            accept3->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write3);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 16, test8_okay);

    write2 = ogs_pollset_add(pollset, OGS_POLLIN|OGS_POLLOUT,
            accept2->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write2);

    write3 = ogs_pollset_add(pollset, OGS_POLLIN|OGS_POLLOUT,
            accept3->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write3);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 18, test8_okay);

    write3 = ogs_pollset_add(pollset, OGS_POLLIN|OGS_POLLOUT,
            accept3->fd, test8_handler_with_remove,
            ogs_pollset_self_handler_data());
    ABTS_PTR_NOTNULL(tc, write3);

    rv = ogs_pollset_poll(pollset, OGS_INFINITE_TIME);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ABTS_INT_EQUAL(tc, 19, test8_okay);

    ogs_sock_destroy(client1);
    ogs_sock_destroy(accept1);

    ogs_sock_destroy(client2);
    ogs_sock_destroy(accept2);

    ogs_sock_destroy(client3);
    ogs_sock_destroy(accept3);

    ogs_sock_destroy(server);

    ogs_pollset_destroy(pollset);
}

abts_suite *test_poll(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

#if 0 /* FIXME : Not working in WIN32, i585 */
    abts_run_test(suite, test1_func, NULL);
#endif

    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);
    abts_run_test(suite, test4_func, NULL);
    abts_run_test(suite, test5_func, NULL);
    abts_run_test(suite, test6_func, NULL);
    abts_run_test(suite, test7_func, NULL);
    abts_run_test(suite, test8_func, NULL);

    return suite;
}
