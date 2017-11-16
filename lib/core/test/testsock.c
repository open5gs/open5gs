#include "core_debug.h"
#include "core_thread.h"

#include "core_udp.h"
#include "core_tcp.h"

#include "testutil.h"

#define DATASTR "This is a test"
#define STRLEN 8092
#define PORT 7777

static void sock_test1(abts_case *tc, void *data)
{
    sock_id udp;
    status_t rv;

    rv = udp_open(&udp, AF_UNSPEC, 0, PORT, NULL, 0, SOCK_F_BIND);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = udp_open(&udp, AF_UNSPEC,
            "127.0.0.1", PORT, NULL, 0, SOCK_F_BIND);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = udp_open(&udp, AF_UNSPEC, "::1", PORT, NULL, 0, SOCK_F_BIND);
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

    rv = tcp_open(&tcp, AF_UNSPEC, NULL, 0, "::1", PORT, SOCK_F_CONNECT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = sock_read(tcp, str, STRLEN, 0, NULL, NULL);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    rv = sock_delete(tcp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_exit(id, size);
    return NULL;
}

static void sock_test2(abts_case *tc, void *data)
{
    sock_id tcp, tcp2;
    status_t rv;
    ssize_t size;

    rv = tcp_open(&tcp, AF_INET6, NULL, PORT, NULL, 0, SOCK_F_BIND);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test2_thread, NULL, test2_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sock_accept(&tcp2, tcp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = sock_write(tcp2, DATASTR, strlen(DATASTR), 0, NULL, 0);
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
    c_sockaddr_t dst_addr;
    socklen_t addrlen;
    char str[STRLEN];
    ssize_t size;
    int rc;

    rv = udp_open(&udp, AF_INET, NULL, 0, NULL, 0, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_pton("127.0.0.1", PORT, &dst_addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = sock_write(udp, DATASTR, strlen(DATASTR), 0,
            &dst_addr, sockaddr_len(&dst_addr));
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

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
    c_sockaddr_t src_addr;
    socklen_t addrlen;
    char str[STRLEN];
    char buf[CORE_ADDRSTRLEN];

    rv = udp_open(&udp, AF_INET, NULL, PORT, NULL, 0, SOCK_F_BIND);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test3_thread, NULL, test3_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = sock_read(udp, str, STRLEN, 0, &src_addr, &addrlen);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, sizeof(struct sockaddr_in), addrlen);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_NTOP(&src_addr, buf));

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

    rv = udp_open(&udp, AF_UNSPEC,
            NULL, 0, "127.0.0.1", PORT, SOCK_F_CONNECT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = sock_write(udp, DATASTR, strlen(DATASTR), 0, NULL, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    size = sock_read(udp, str, STRLEN, 0, NULL, NULL);
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
    c_sockaddr_t src_addr;
    socklen_t addrlen;
    char str[STRLEN];
    char buf[CORE_ADDRSTRLEN];

    rv = udp_open(&udp, AF_INET, NULL, PORT, NULL, 0, SOCK_F_BIND);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test4_thread, NULL, test4_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = sock_read(udp, str, STRLEN, 0, &src_addr, &addrlen);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, sizeof(struct sockaddr_in), addrlen);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_NTOP(&src_addr, buf));

    size = sock_write(udp, DATASTR, strlen(DATASTR), 0, &src_addr, addrlen);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    thread_join(&rv, test4_thread);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), rv);

    rv = sock_delete(udp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

abts_suite *testsock(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, sock_test1, NULL);
    abts_run_test(suite, sock_test2, NULL);
    abts_run_test(suite, sock_test3, NULL);
    abts_run_test(suite, sock_test4, NULL);

    return suite;
}
