#include "core_debug.h"
#include "core_thread.h"

#include "core_network.h"

#include "testutil.h"

#define DATASTR "This is a test"
#define STRLEN 8092
#define PORT 7777
#define PORT2 7778

static void sock_test1(abts_case *tc, void *data)
{
    sock_id udp;
    status_t rv;

    rv = udp_server(&udp, AF_UNSPEC, NULL, PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = udp_server(&udp, AF_UNSPEC, "127.0.0.1", PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = udp_server(&udp, AF_UNSPEC, "::1", PORT);
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
    char str[STRLEN];
    ssize_t size;

    rv = tcp_client(&tcp, AF_UNSPEC, "::1", PORT);
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
    ssize_t size;

    rv = tcp_server(&tcp, AF_INET6, NULL, PORT);
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

    rv = udp_socket(&udp, AF_INET);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_getaddrinfo(&sa, AF_UNSPEC, "127.0.0.1", PORT, 0);
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
    c_sockaddr_t sa;
    socklen_t addrlen;
    char str[STRLEN];
    char buf[CORE_ADDRSTRLEN];

    rv = udp_server(&udp, AF_INET, NULL, PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test3_thread, NULL, test3_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_recvfrom(udp, str, STRLEN, 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, sizeof(struct sockaddr_in), sa.sa_len);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_NTOP(&sa, buf));

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
    char str[STRLEN];
    ssize_t size;

    rv = udp_client(&udp, AF_INET, "127.0.0.1", PORT);
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
    c_sockaddr_t sa;
    socklen_t addrlen;
    char str[STRLEN];
    char buf[CORE_ADDRSTRLEN];

    rv = udp_server(&udp, AF_INET, NULL, PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test4_thread, NULL, test4_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_recvfrom(udp, str, STRLEN, 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, sizeof(struct sockaddr_in), sa.sa_len);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_NTOP(&sa, buf));

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
    char str[STRLEN];
    ssize_t size;

    rv = udp_server(&udp, AF_INET6, NULL, PORT2);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = udp_connect(udp, "::1", PORT);
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
    c_sockaddr_t sa;
    socklen_t addrlen;
    char str[STRLEN];
    char buf[CORE_ADDRSTRLEN];

    rv = udp_server(&udp, AF_INET6, NULL, PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = udp_connect(udp, "::1", PORT2);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test5_thread, NULL, test5_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_recvfrom(udp, str, STRLEN, 0, &sa);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, sizeof(struct sockaddr_in6), sa.sa_len);
    ABTS_STR_EQUAL(tc, "::1", CORE_NTOP(&sa, buf));

    size = core_send(udp, DATASTR, strlen(DATASTR), 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    thread_join(&rv, test5_thread);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), rv);

    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static void sock_test6(abts_case *tc, void *data)
{
    c_sockaddr_t *sa;
    status_t rv;

    rv = core_getaddrinfo(&sa, AF_UNSPEC, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_freeaddrinfo(sa);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static void sock_test7(abts_case *tc, void *data)
{
    c_sockaddr_t sa;
    char buf[CORE_ADDRSTRLEN];
    status_t rv;

    rv = core_inet_pton(AF_INET, "127.0.0.1", &sa);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_NTOP(&sa, buf));

    rv = core_inet_pton(AF_INET6, "::1", &sa);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, "::1", CORE_NTOP(&sa, buf));
}


abts_suite *testsock(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

#if 0
    abts_run_test(suite, sock_test1, NULL);
#endif
    abts_run_test(suite, sock_test2, NULL);
#if 0
    abts_run_test(suite, sock_test3, NULL);
    abts_run_test(suite, sock_test4, NULL);
    abts_run_test(suite, sock_test5, NULL);
    abts_run_test(suite, sock_test6, NULL);
    abts_run_test(suite, sock_test7, NULL);
#endif

    return suite;
}
