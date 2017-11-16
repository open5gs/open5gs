#include "core_debug.h"
#include "core_thread.h"
#include "core_sctp.h"

#include "testutil.h"

#define DATASTR "This is a test"
#define STRLEN 8092
#define PORT 7777
#define PPID 12345

static void sctp_test1(abts_case *tc, void *data)
{
    sock_id sctp;
    status_t rv;

    rv = sctp_open(&sctp, AF_UNSPEC, SOCK_SEQPACKET,
            0, PORT, NULL, 0, SOCK_F_BIND);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sctp_open(&sctp, AF_UNSPEC, SOCK_STREAM,
            "127.0.0.1", PORT, NULL, 0, SOCK_F_BIND);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sctp_open(&sctp, AF_UNSPEC, SOCK_SEQPACKET,
            "::1", PORT, NULL, 0, SOCK_F_BIND);
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

    rv = sctp_open(&sctp, AF_UNSPEC, SOCK_SEQPACKET,
            NULL, 0, "::1", PORT, SOCK_F_CONNECT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = sctp_read(sctp, str, STRLEN, NULL, NULL, &ppid, NULL);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, PPID, ppid);

    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_exit(id, size);
    return NULL;
}

static void sctp_test2(abts_case *tc, void *data)
{
    sock_id sctp, sctp2;
    status_t rv;
    ssize_t size;

    rv = sctp_open(&sctp, AF_INET6, SOCK_STREAM,
            NULL, PORT, NULL, 0, SOCK_F_BIND);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test2_thread, NULL, test2_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sock_accept(&sctp2, sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = sctp_write(sctp2, DATASTR, strlen(DATASTR),
            NULL, 0, PPID, 0);
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
    struct sockaddr_in dst_addr;
    socklen_t addrlen;
    char str[STRLEN];
    ssize_t size;
    int rc;

    rv = sctp_open(&sctp, AF_INET, SOCK_SEQPACKET, NULL, 0, NULL, 0, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin_port = htons(PORT);
    dst_addr.sin_family = AF_INET;
    rc = inet_pton(AF_INET, "127.0.0.1", &(dst_addr.sin_addr));
    ABTS_INT_EQUAL(tc, 1, rc);

    size = sctp_write(sctp, DATASTR, strlen(DATASTR),
            (struct sockaddr *)&dst_addr, sizeof(struct sockaddr), PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

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
    struct sockaddr_in src_addr;
    socklen_t addrlen;
    char str[STRLEN];
    char buf[INET6_ADDRSTRLEN];
    c_uint32_t ppid;

    rv = sctp_open(&sctp, AF_INET, SOCK_SEQPACKET,
            NULL, PORT, NULL, 0, SOCK_F_BIND);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test3_thread, NULL, test3_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    addrlen = sizeof(src_addr);
    size = sctp_read(sctp, str, STRLEN,
            (struct sockaddr *)&src_addr, &addrlen, &ppid, NULL);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, sizeof(src_addr), addrlen);
    ABTS_INT_EQUAL(tc, PPID, ppid);
    ABTS_STR_EQUAL(tc, "127.0.0.1", SOCK_NTOP(&src_addr, buf));
    
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

    rv = sctp_open(&sctp, AF_UNSPEC, SOCK_STREAM,
            NULL, 0, "::1", PORT, SOCK_F_CONNECT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = sctp_write(sctp, DATASTR, strlen(DATASTR), NULL, 0, PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    size = sctp_read(sctp, str, STRLEN, NULL, NULL, &ppid, NULL);
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
    struct sockaddr_in6 src_addr;
    socklen_t addrlen;
    char str[STRLEN];
    c_uint32_t ppid;
    char buf[INET6_ADDRSTRLEN];

    rv = sctp_open(&sctp, AF_INET6, SOCK_SEQPACKET,
            NULL, PORT, NULL, 0, SOCK_F_BIND);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test4_thread, NULL, test4_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    addrlen = sizeof(src_addr);
    size = sctp_read(sctp, str, STRLEN, (struct sockaddr *)&src_addr, &addrlen, &ppid, NULL);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, sizeof(src_addr), addrlen);
    ABTS_STR_EQUAL(tc, "::1", SOCK_NTOP(&src_addr, buf));
    ABTS_INT_EQUAL(tc, PPID, ppid);

    size = sctp_write(sctp, DATASTR, strlen(DATASTR),
            (struct sockaddr *)&src_addr, addrlen, PPID, 0);
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
