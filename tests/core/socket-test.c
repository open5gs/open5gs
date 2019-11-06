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
#define PORT2 7778

#ifndef AI_PASSIVE
#define AI_PASSIVE 1
#endif

static void test1_func(abts_case *tc, void *data)
{
    int rv;
    ogs_sock_t *udp;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    udp = ogs_udp_server(node);
    ABTS_PTR_NOTNULL(tc, udp);
    ogs_socknode_free(node);

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, "127.0.0.1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    udp = ogs_udp_server(node);
    ABTS_PTR_NOTNULL(tc, udp);
    ogs_socknode_free(node);

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, "::1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    udp = ogs_udp_server(node);
    ABTS_PTR_NOTNULL(tc, udp);
    ogs_socknode_free(node);
}

static ogs_thread_t *test2_thread;
static void test2_main(void *data)
{
    int rv;
    abts_case *tc = data;
    ogs_sock_t *tcp;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;
    char str[STRLEN];
    ssize_t size;

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, "::1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    tcp = ogs_tcp_client(node);
    ABTS_PTR_NOTNULL(tc, tcp);

    size = ogs_recv(tcp->fd, str, STRLEN, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    ogs_socknode_free(node);
}

static void test2_func(abts_case *tc, void *data)
{
    int rv;
    ogs_sock_t *tcp, *tcp2;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;
    ssize_t size;

    rv = ogs_getaddrinfo(&addr, AF_INET6, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    tcp = ogs_tcp_server(node);
    ABTS_PTR_NOTNULL(tc, tcp);

    test2_thread = ogs_thread_create(test2_main, tc);
    ABTS_PTR_NOTNULL(tc, test2_thread);

    tcp2 = ogs_sock_accept(tcp);
    ABTS_PTR_NOTNULL(tc, tcp2);

    size = ogs_send(tcp2->fd, DATASTR, strlen(DATASTR), 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    ogs_thread_destroy(test2_thread);

    ogs_sock_destroy(tcp2);
    ogs_socknode_free(node);
}

static ogs_thread_t *test3_thread;
static void test3_main(void *data)
{
    abts_case *tc = data;
    int rv;
    ogs_sock_t *udp;
    ogs_sockaddr_t *addr;
    ssize_t size;

    rv = ogs_getaddrinfo(&addr, AF_INET, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    udp = ogs_udp_socket(AF_INET, NULL);
    ABTS_PTR_NOTNULL(tc, udp);

    size = ogs_sendto(udp->fd, DATASTR, strlen(DATASTR), 0, addr);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    ogs_sock_destroy(udp);

    rv = ogs_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
}

static void test3_func(abts_case *tc, void *data)
{
    int rv;
    ogs_sock_t *udp;
    ssize_t size;
    ogs_sockaddr_t sa;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;
    char str[STRLEN];
    char buf[OGS_ADDRSTRLEN];

    rv = ogs_getaddrinfo(&addr, AF_INET, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    udp = ogs_udp_server(node);
    ABTS_PTR_NOTNULL(tc, udp);

    test3_thread = ogs_thread_create(test3_main, tc);
    ABTS_PTR_NOTNULL(tc, test3_thread);

    size = ogs_recvfrom(udp->fd, str, STRLEN, 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "127.0.0.1", OGS_ADDR(&sa, buf));

    ogs_thread_destroy(test3_thread);

    ogs_socknode_free(node);
}

static ogs_thread_t *test4_thread;
static void test4_main(void *data)
{
    int rv;
    abts_case *tc = data;
    ogs_sock_t *udp;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;
    char str[STRLEN];
    ssize_t size;

    rv = ogs_getaddrinfo(&addr, AF_INET, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    udp = ogs_udp_client(node);
    ABTS_PTR_NOTNULL(tc, udp);

    size = ogs_send(udp->fd, DATASTR, strlen(DATASTR), 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    size = ogs_recv(udp->fd, str, STRLEN, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    ogs_socknode_free(node);
}

static void test4_func(abts_case *tc, void *data)
{
    int rv;
    ogs_sock_t *udp;
    ssize_t size;
    ogs_sockaddr_t sa;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;
    char str[STRLEN];
    char buf[OGS_ADDRSTRLEN];

    rv = ogs_getaddrinfo(&addr, AF_INET, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    udp = ogs_udp_server(node);
    ABTS_PTR_NOTNULL(tc, udp);

    test4_thread = ogs_thread_create(test4_main, tc);
    ABTS_PTR_NOTNULL(tc, test4_thread);

    size = ogs_recvfrom(udp->fd, str, STRLEN, 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "127.0.0.1", OGS_ADDR(&sa, buf));

    size = ogs_sendto(udp->fd, DATASTR, strlen(DATASTR), 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    ogs_thread_destroy(test4_thread);

    ogs_socknode_free(node);
}

static ogs_thread_t *test5_thread;
static void test5_main(void *data)
{
    abts_case *tc = data;
    int rv;
    ogs_sock_t *udp;
    ogs_sockaddr_t *addr;
    ogs_socknode_t *node;
    char str[STRLEN];
    ssize_t size;

    rv = ogs_getaddrinfo(&addr, AF_INET6, NULL, PORT2, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    udp = ogs_udp_server(node);
    ABTS_PTR_NOTNULL(tc, udp);

    rv = ogs_getaddrinfo(&addr, AF_INET6, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_udp_connect(udp, addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    size = ogs_send(udp->fd, DATASTR, strlen(DATASTR), 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    size = ogs_recv(udp->fd, str, STRLEN, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    ogs_socknode_free(node);
}

static void test5_func(abts_case *tc, void *data)
{
    ogs_sock_t *udp;
    int rv;
    ssize_t size;
    ogs_sockaddr_t sa, *addr;
    ogs_socknode_t *node;
    char str[STRLEN];
    char buf[OGS_ADDRSTRLEN];

    rv = ogs_getaddrinfo(&addr, AF_INET6, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    node = ogs_socknode_new(addr);
    ABTS_PTR_NOTNULL(tc, node);
    udp = ogs_udp_server(node);
    ABTS_PTR_NOTNULL(tc, udp);

    rv = ogs_getaddrinfo(&addr, AF_INET6, NULL, PORT2, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_udp_connect(udp, addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    rv = ogs_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    test5_thread = ogs_thread_create(test5_main, tc);
    ABTS_PTR_NOTNULL(tc, test5_thread);

    size = ogs_recvfrom(udp->fd, str, STRLEN, 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "::1", OGS_ADDR(&sa, buf));

    size = ogs_send(udp->fd, DATASTR, strlen(DATASTR), 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    ogs_thread_destroy(test5_thread);

    ogs_socknode_free(node);
}

static void test6_func(abts_case *tc, void *data)
{
    ogs_sockaddr_t addr, *paddr, *dst;
    char buf[OGS_ADDRSTRLEN];
    int rv;

    rv = ogs_inet_pton(AF_INET, "127.0.0.1", &addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_STR_EQUAL(tc, "127.0.0.1", OGS_ADDR(&addr, buf));

    rv = ogs_inet_pton(AF_INET6, "::1", &addr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_STR_EQUAL(tc, "::1", OGS_ADDR(&addr, buf));

    paddr = NULL;
    rv = ogs_addaddrinfo(&paddr, AF_UNSPEC, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_sortaddrinfo(&paddr, AF_INET6);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_STR_EQUAL(tc, "::1", OGS_ADDR(paddr, buf));
    ABTS_PTR_NOTNULL(tc, paddr->next);
    ABTS_STR_EQUAL(tc, "127.0.0.1", OGS_ADDR(paddr->next, buf));

    rv = ogs_copyaddrinfo(&dst, paddr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_STR_EQUAL(tc, "::1", OGS_ADDR(dst, buf));
    ABTS_PTR_NOTNULL(tc, paddr->next);
    ABTS_STR_EQUAL(tc, "127.0.0.1", OGS_ADDR(dst->next, buf));
    rv = ogs_freeaddrinfo(dst);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_sortaddrinfo(&paddr, AF_INET);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_STR_EQUAL(tc, "127.0.0.1", OGS_ADDR(paddr, buf));
    ABTS_PTR_NOTNULL(tc, paddr->next);
    ABTS_STR_EQUAL(tc, "::1", OGS_ADDR(paddr->next, buf));

    rv = ogs_filteraddrinfo(&paddr, AF_INET);
    ABTS_PTR_NOTNULL(tc, paddr);
    ABTS_STR_EQUAL(tc, "127.0.0.1", OGS_ADDR(paddr, buf));

    rv = ogs_copyaddrinfo(&dst, paddr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_STR_EQUAL(tc, "127.0.0.1", OGS_ADDR(dst, buf));
    rv = ogs_addaddrinfo(&dst, AF_UNSPEC, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_STR_EQUAL(tc, "127.0.0.1", OGS_ADDR(dst, buf));

    rv = ogs_freeaddrinfo(dst);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_filteraddrinfo(&paddr, AF_INET6);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_TRUE(tc, paddr == NULL);

    rv = ogs_copyaddrinfo(&dst, paddr);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    ABTS_TRUE(tc, dst == NULL);

    paddr = NULL;
    rv = ogs_addaddrinfo(&paddr, AF_INET, "127.0.0.1", PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_addaddrinfo(&paddr, AF_INET, "127.0.0.2", PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_addaddrinfo(&paddr, AF_INET, "127.0.0.3", PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_filteraddrinfo(&paddr, AF_INET6);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
}

static void test7_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *node;
    ogs_sockaddr_t *addr;
    ogs_list_t list, list6;

    ogs_list_init(&list);
    ogs_list_init(&list6);

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, "localhost", PORT, 0);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    node = ogs_socknode_add(&list, AF_INET, addr);
    ABTS_PTR_NOTNULL(tc, node);

    ogs_freeaddrinfo(addr);

    ogs_socknode_remove_all(&list);

    rv = ogs_socknode_probe(&list, &list6, NULL, PORT);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_socknode_remove_all(&list);
    ogs_socknode_remove_all(&list6);
}

static void test8_func(abts_case *tc, void *data)
{
    int rv;
    ogs_ipsubnet_t ipsub;

    rv = ogs_ipsubnet(&ipsub, "127.0.0.1", "8");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_ipsubnet(&ipsub, "fe80::1", "64");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_ipsubnet(&ipsub, "172.16.0.1", "16");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_ipsubnet(&ipsub, "cafe::1", "64");
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_ipsubnet(&ipsub, "172.16.0.1", NULL);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    rv = ogs_ipsubnet(&ipsub, "cafe::1", NULL);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
}

abts_suite *test_socket(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);
    abts_run_test(suite, test4_func, NULL);
    abts_run_test(suite, test5_func, NULL);
    abts_run_test(suite, test6_func, NULL);
    abts_run_test(suite, test7_func, NULL);
    abts_run_test(suite, test8_func, NULL);

    return suite;
}
