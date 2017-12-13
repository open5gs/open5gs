#include "core_debug.h"
#include "core_thread.h"
#include "core_pkbuf.h"

#include "core_network.h"

#include "testutil.h"

#define DATASTR "This is a test"
#define STRLEN 8092
#define PORT 7777
#define PORT2 7778

#ifndef AI_PASSIVE
#define AI_PASSIVE 1
#endif

static void sock_test1(abts_case *tc, void *data)
{
    sock_id udp;
    c_sockaddr_t *addr;
    status_t rv;

    rv = core_getaddrinfo(&addr, AF_UNSPEC, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = udp_server(&udp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_getaddrinfo(&addr, AF_UNSPEC, "127.0.0.1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = udp_server(&udp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_getaddrinfo(&addr, AF_UNSPEC, "::1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = udp_server(&udp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static thread_id test2_thread;
static void *THREAD_FUNC test2_main(thread_id id, void *data)
{
    abts_case *tc = data;
    status_t rv;
    sock_id tcp;
    c_sockaddr_t *addr;
    char str[STRLEN];
    ssize_t size;

    rv = core_getaddrinfo(&addr, AF_UNSPEC, "::1", PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tcp_client(&tcp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_recv(tcp, str, STRLEN, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    rv = sock_delete(tcp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_exit(id, size);
    return NULL;
}

static void sock_test2(abts_case *tc, void *data)
{
    status_t rv;
    sock_id tcp, tcp2;
    c_sockaddr_t *addr;
    ssize_t size;

    rv = core_getaddrinfo(&addr, AF_INET6, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = tcp_server(&tcp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test2_thread, NULL, test2_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sock_accept(&tcp2, tcp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_send(tcp2, DATASTR, strlen(DATASTR), 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    thread_join(&rv, test2_thread);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), rv);

    rv = sock_delete(tcp2);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sock_delete(tcp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static thread_id test3_thread;
static void *THREAD_FUNC test3_main(thread_id id, void *data)
{
    abts_case *tc = data;
    status_t rv;
    sock_id udp;
    c_sockaddr_t *sa;
    char str[STRLEN];
    ssize_t size;
    int rc;
    char buf[CORE_ADDRSTRLEN];

    rv = udp_socket(&udp, AF_INET);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_getaddrinfo(&sa, AF_INET, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sendto(udp, DATASTR, strlen(DATASTR), 0, sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    rv = core_freeaddrinfo(sa);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_exit(id, size);
    return NULL;
}

static void sock_test3(abts_case *tc, void *data)
{
    sock_id udp;
    status_t rv;
    ssize_t size;
    c_sockaddr_t sa, *addr;
    socklen_t addrlen;
    char str[STRLEN];
    char buf[CORE_ADDRSTRLEN];

    rv = core_getaddrinfo(&addr, AF_INET, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = udp_server(&udp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test3_thread, NULL, test3_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_recvfrom(udp, str, STRLEN, 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_ADDR(&sa, buf));

    thread_join(&rv, test3_thread);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), rv);

    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static thread_id test4_thread;
static void *THREAD_FUNC test4_main(thread_id id, void *data)
{
    abts_case *tc = data;
    status_t rv;
    sock_id udp;
    c_sockaddr_t *addr;
    char str[STRLEN];
    ssize_t size;

    rv = core_getaddrinfo(&addr, AF_INET, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = udp_client(&udp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_send(udp, DATASTR, strlen(DATASTR), 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    size = core_recv(udp, str, STRLEN, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_exit(id, size);
    return NULL;
}

static void sock_test4(abts_case *tc, void *data)
{
    sock_id udp;
    status_t rv;
    ssize_t size;
    c_sockaddr_t sa, *addr;
    socklen_t addrlen;
    char str[STRLEN];
    char buf[CORE_ADDRSTRLEN];

    rv = core_getaddrinfo(&addr, AF_INET, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = udp_server(&udp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test4_thread, NULL, test4_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_recvfrom(udp, str, STRLEN, 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_ADDR(&sa, buf));

    size = core_sendto(udp, DATASTR, strlen(DATASTR), 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    thread_join(&rv, test4_thread);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), rv);

    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static thread_id test5_thread;
static void *THREAD_FUNC test5_main(thread_id id, void *data)
{
    abts_case *tc = data;
    status_t rv;
    sock_id udp;
    c_sockaddr_t *addr;
    char str[STRLEN];
    ssize_t size;

    rv = core_getaddrinfo(&addr, AF_INET6, NULL, PORT2, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = udp_server(&udp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_getaddrinfo(&addr, AF_INET6, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = udp_connect(udp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_send(udp, DATASTR, strlen(DATASTR), 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    size = core_recv(udp, str, STRLEN, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_exit(id, size);
    return NULL;
}

static void sock_test5(abts_case *tc, void *data)
{
    sock_id udp;
    status_t rv;
    ssize_t size;
    c_sockaddr_t sa, *addr;
    socklen_t addrlen;
    char str[STRLEN];
    char buf[CORE_ADDRSTRLEN];

    rv = core_getaddrinfo(&addr, AF_INET6, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = udp_server(&udp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_getaddrinfo(&addr, AF_INET6, NULL, PORT2, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = udp_connect(udp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test5_thread, NULL, test5_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_recvfrom(udp, str, STRLEN, 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "::1", CORE_ADDR(&sa, buf));

    size = core_send(udp, DATASTR, strlen(DATASTR), 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    thread_join(&rv, test5_thread);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), rv);

    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static void sock_test6(abts_case *tc, void *data)
{
    c_sockaddr_t addr, *paddr, *dst;
    char buf[CORE_ADDRSTRLEN];
    status_t rv;

    rv = core_inet_pton(AF_INET, "127.0.0.1", &addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_ADDR(&addr, buf));

    rv = core_inet_pton(AF_INET6, "::1", &addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, "::1", CORE_ADDR(&addr, buf));

    paddr = NULL;
    rv = core_addaddrinfo(&paddr, AF_UNSPEC, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_sortaddrinfo(&paddr, AF_INET6);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, "::1", CORE_ADDR(paddr, buf));
    ABTS_PTR_NOTNULL(tc, paddr->next);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_ADDR(paddr->next, buf));

    rv = core_copyaddrinfo(&dst, paddr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, "::1", CORE_ADDR(dst, buf));
    ABTS_PTR_NOTNULL(tc, paddr->next);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_ADDR(dst->next, buf));
    rv = core_freeaddrinfo(dst);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_sortaddrinfo(&paddr, AF_INET);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_ADDR(paddr, buf));
    ABTS_PTR_NOTNULL(tc, paddr->next);
    ABTS_STR_EQUAL(tc, "::1", CORE_ADDR(paddr->next, buf));

    rv = core_filteraddrinfo(&paddr, AF_INET);
    ABTS_PTR_NOTNULL(tc, paddr);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_ADDR(paddr, buf));

    rv = core_copyaddrinfo(&dst, paddr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_ADDR(dst, buf));
    rv = core_addaddrinfo(&dst, AF_UNSPEC, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_ADDR(dst, buf));

    rv = core_freeaddrinfo(dst);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_filteraddrinfo(&paddr, AF_INET6);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_PTR_NULL(tc, paddr);

    rv = core_copyaddrinfo(&dst, paddr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_PTR_NULL(tc, dst);

    paddr = NULL;
    rv = core_addaddrinfo(&paddr, AF_INET, "127.0.0.1", PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_addaddrinfo(&paddr, AF_INET, "127.0.0.2", PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_addaddrinfo(&paddr, AF_INET, "127.0.0.3", PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_filteraddrinfo(&paddr, AF_INET6);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static void sock_test7(abts_case *tc, void *data)
{
    status_t rv;
    sock_node_t *node;
    c_sockaddr_t *addr;
    list_t list, list6;

    list_init(&list);
    list_init(&list6);

    rv = core_getaddrinfo(&addr, AF_UNSPEC, "localhost", PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sock_add_node(&list, &node, addr, AF_INET);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    core_freeaddrinfo(addr);

    sock_remove_all_nodes(&list);

    rv = sock_probe_node(&list, &list6, NULL, PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    sock_remove_all_nodes(&list);
    sock_remove_all_nodes(&list6);
}

static void sock_test8(abts_case *tc, void *data)
{
    status_t rv;
    ipsubnet_t ipsub;

    rv = core_ipsubnet(&ipsub, "127.0.0.1", "8");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_ipsubnet(&ipsub, "fe80::1", "64");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_ipsubnet(&ipsub, "172.16.0.1", "16");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_ipsubnet(&ipsub, "cafe::1", "64");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_ipsubnet(&ipsub, "172.16.0.1", NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_ipsubnet(&ipsub, "cafe::1", NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

abts_suite *testsock(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, sock_test1, NULL);
    abts_run_test(suite, sock_test2, NULL);
    abts_run_test(suite, sock_test3, NULL);
    abts_run_test(suite, sock_test4, NULL);
    abts_run_test(suite, sock_test5, NULL);
    abts_run_test(suite, sock_test6, NULL);
    abts_run_test(suite, sock_test7, NULL);
    abts_run_test(suite, sock_test8, NULL);

    return suite;
}
