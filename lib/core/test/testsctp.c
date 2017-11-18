#include "core_debug.h"
#include "core_thread.h"
#include "core_network.h"

#include "testutil.h"

#define DATASTR "This is a test"
#define STRLEN 8092
#define PORT 7777
#define PPID 12345

static void sctp_test1(abts_case *tc, void *data)
{
    sock_id sctp;
    status_t rv;

    rv = sctp_socket(&sctp, AF_INET6, SOCK_SEQPACKET);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sctp_server(&sctp, AF_UNSPEC, SOCK_STREAM, "127.0.0.1", PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sctp_server(&sctp, AF_UNSPEC, SOCK_SEQPACKET, "::1", PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static thread_id test2_thread;
static void *THREAD_FUNC test2_main(thread_id id, void *data)
{
    abts_case *tc = data;
    status_t rv;
    sock_id sctp;
    char str[STRLEN];
    ssize_t size;
    c_uint32_t ppid;
    c_sockaddr_t sa;

    rv = sctp_client(&sctp, AF_UNSPEC, SOCK_SEQPACKET, "::1", PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_recvmsg(sctp, str, STRLEN, &sa, &ppid, NULL);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, PPID, ppid);

    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_exit(id, size);
    return NULL;
}

static void sctp_test2(abts_case *tc, void *data)
{
    status_t rv;
    sock_id sctp, sctp2;
    c_sockaddr_t sa;
    ssize_t size;

    rv = sctp_server(&sctp, AF_INET6, SOCK_STREAM, NULL, PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test2_thread, NULL, test2_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sock_accept(&sctp2, &sa, sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_sendmsg(sctp2, DATASTR, strlen(DATASTR), NULL,  PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    thread_join(&rv, test2_thread);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    rv = sock_delete(sctp2);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static thread_id test3_thread;
static void *THREAD_FUNC test3_main(thread_id id, void *data)
{
    abts_case *tc = data;
    status_t rv;
    sock_id sctp;
    c_sockaddr_t *sa;
    char str[STRLEN];
    ssize_t size;
    int rc;

    rv = sctp_socket(&sctp, AF_INET, SOCK_SEQPACKET);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_getaddrinfo(&sa, AF_UNSPEC, "127.0.0.1", PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR), sa, PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    rv = core_freeaddrinfo(sa);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_exit(id, size);
    return NULL;
}

static void sctp_test3(abts_case *tc, void *data)
{
    sock_id sctp;
    status_t rv;
    ssize_t size;
    c_sockaddr_t sa;
    char str[STRLEN];
    char buf[CORE_ADDRSTRLEN];
    c_uint32_t ppid;

    rv = sctp_server(&sctp, AF_INET, SOCK_SEQPACKET, NULL, PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test3_thread, NULL, test3_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_recvmsg(sctp, str, STRLEN, &sa, &ppid, NULL);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, sizeof(struct sockaddr_in), sa.sa_len);
    ABTS_INT_EQUAL(tc, PPID, ppid);
    ABTS_STR_EQUAL(tc, "127.0.0.1", CORE_NTOP(&sa, buf));
    
    thread_join(&rv, test3_thread);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), rv);

    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static thread_id test4_thread;
static void *THREAD_FUNC test4_main(thread_id id, void *data)
{
    abts_case *tc = data;
    status_t rv;
    sock_id sctp;
    char str[STRLEN];
    ssize_t size;
    c_uint32_t ppid;

    rv = sctp_client(&sctp, AF_UNSPEC, SOCK_STREAM, "::1", PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR), NULL, PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    size = core_sctp_recvmsg(sctp, str, STRLEN, NULL, &ppid, NULL);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, PPID, ppid);

    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_exit(id, size);
    return NULL;
}

static void sctp_test4(abts_case *tc, void *data)
{
    sock_id sctp;
    status_t rv;
    ssize_t size;
    c_sockaddr_t sa;
    char str[STRLEN];
    c_uint32_t ppid;
    char buf[CORE_ADDRSTRLEN];

    rv = sctp_server(&sctp, AF_INET6, SOCK_SEQPACKET, NULL, PORT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test4_thread, NULL, test4_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_recvmsg(sctp, str, STRLEN, &sa, &ppid, NULL);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, sizeof(struct sockaddr_in6), sa.sa_len);
    ABTS_STR_EQUAL(tc, "::1", CORE_NTOP(&sa, buf));
    ABTS_INT_EQUAL(tc, PPID, ppid);

    size = core_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR), &sa, PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    thread_join(&rv, test4_thread);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), rv);

    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

abts_suite *testsctp(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, sctp_test1, NULL);
    abts_run_test(suite, sctp_test2, NULL);
    abts_run_test(suite, sctp_test3, NULL);
    abts_run_test(suite, sctp_test4, NULL);

    return suite;
}
