#include "core_debug.h"
#include "core_thread.h"
#include "core_network.h"

#include "testutil.h"

#define DATASTR "This is a test"
#define STRLEN 8092
#define PORT 7777
#define PORT2 7778
#define PPID 12345

#ifndef AI_PASSIVE
#define AI_PASSIVE 1
#endif

static void sctp_test1(abts_case *tc, void *data)
{
    sock_id sctp;
    c_sockaddr_t *addr;
    status_t rv;

    rv = sctp_socket(&sctp, AF_INET6, SOCK_SEQPACKET);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_getaddrinfo(&addr, AF_INET, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = sctp_server(&sctp, SOCK_STREAM, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    
    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_getaddrinfo(&addr, AF_UNSPEC, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = sctp_server(&sctp, SOCK_SEQPACKET, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
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
    sctp_info_t sinfo;
    c_sockaddr_t *addr;
    c_sockaddr_t from;

    rv = core_getaddrinfo(&addr, AF_UNSPEC, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = sctp_client(&sctp, SOCK_SEQPACKET, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_recvdata(sctp, str, STRLEN, &from, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);

    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_exit(id, size);
    return NULL;
}

static void sctp_test2(abts_case *tc, void *data)
{
    status_t rv;
    sock_id sctp, sctp2;
    ssize_t size;
    c_sockaddr_t *addr;

    rv = core_getaddrinfo(&addr, AF_INET6, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = sctp_server(&sctp, SOCK_STREAM, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test2_thread, NULL, test2_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = sock_accept(&sctp2, sctp);
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
    c_sockaddr_t *addr;
    c_sockaddr_t *to;
    char str[STRLEN];
    ssize_t size;
    int rc;

    rv = sctp_socket(&sctp, AF_INET, SOCK_SEQPACKET);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_getaddrinfo(&to, AF_INET, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR), to, PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    rv = core_freeaddrinfo(to);
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
    c_sockaddr_t from, *addr;
    char str[STRLEN];
    char buf[CORE_ADDRSTRLEN];
    sctp_info_t sinfo;

    rv = core_getaddrinfo(&addr, AF_INET, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = sctp_server(&sctp, SOCK_SEQPACKET, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test3_thread, NULL, test3_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_recvdata(sctp, str, STRLEN, &from, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);
    
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
    c_sockaddr_t *addr;
    char str[STRLEN];
    ssize_t size;
    sctp_info_t sinfo;

    rv = core_getaddrinfo(&addr, AF_UNSPEC, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = sctp_client(&sctp, SOCK_STREAM, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR), NULL, PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    size = core_sctp_recvdata(sctp, str, STRLEN, NULL, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);

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
    c_sockaddr_t from, *addr;
    char str[STRLEN];
    sctp_info_t sinfo;
    char buf[CORE_ADDRSTRLEN];

    rv = core_getaddrinfo(&addr, AF_INET6, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = sctp_server(&sctp, SOCK_SEQPACKET, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test4_thread, NULL, test4_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_recvdata(sctp, str, STRLEN, &from, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "::1", CORE_ADDR(&from, buf));
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);

    size = core_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR), &from, PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    thread_join(&rv, test4_thread);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), rv);

    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static thread_id test5_thread;
static void *THREAD_FUNC test5_main(thread_id id, void *data)
{
    abts_case *tc = data;
    status_t rv;
    sock_id sctp;
    char str[STRLEN];
    c_sockaddr_t from, *remote_addr, *addr;
    sctp_info_t sinfo;
    ssize_t size;
    char buf[CORE_ADDRSTRLEN];

    rv = core_getaddrinfo(&addr, AF_INET6, NULL, PORT2, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = sctp_server(&sctp, SOCK_SEQPACKET, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = core_getaddrinfo(&addr, AF_INET6, NULL, PORT, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = sctp_connect(sctp, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    remote_addr = sock_remote_addr(sctp);
    ABTS_STR_EQUAL(tc, "::1", CORE_ADDR(remote_addr, buf));

    size = core_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR),
            remote_addr, PPID, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    size = core_sctp_recvdata(sctp, str, STRLEN, &from, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "::1", CORE_ADDR(&from, buf));
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);

    rv = sock_delete(sctp);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_exit(id, size);
    return NULL;
}

static void sctp_test5(abts_case *tc, void *data)
{
    sock_id sctp;
    status_t rv;
    ssize_t size;
    c_sockaddr_t from, *addr;
    socklen_t addrlen;
    char str[STRLEN];
    sctp_info_t sinfo;
    char buf[CORE_ADDRSTRLEN];

    rv = core_getaddrinfo(&addr, AF_INET6, NULL, PORT, AI_PASSIVE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = sctp_server(&sctp, SOCK_SEQPACKET, addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = core_freeaddrinfo(addr);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&test5_thread, NULL, test5_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    size = core_sctp_recvdata(sctp, str, STRLEN, &from, &sinfo);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);
    ABTS_STR_EQUAL(tc, "::1", CORE_ADDR(&from, buf));
    ABTS_INT_EQUAL(tc, PPID, sinfo.ppid);

    size = core_sctp_sendmsg(sctp, DATASTR, strlen(DATASTR), &from,
            sinfo.ppid, 0);
    ABTS_INT_EQUAL(tc, strlen(DATASTR), size);

    thread_join(&rv, test5_thread);
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
    abts_run_test(suite, sctp_test5, NULL);

    return suite;
}
