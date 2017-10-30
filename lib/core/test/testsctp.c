#define TRACE_MODULE _testsctp
#include "core_debug.h"
#include "core_net.h"
#include "testutil.h"

#define TEST_SERVER_PORT    5121
#define TEST_BUFFER_SIZE    1024

#define TEST_MAX_NUM     4

static char buffer[TEST_BUFFER_SIZE];
static int sctp_stream_server_started = 0;
static int sctp_seq_server_started = 0;

pthread_t streamserver_tid, seqserver_tid;
net_sock_t *streamserver_sock,*seqserver_sock;

static void *sctp_stream_session_main(void *param)
{
    int rc;

    net_sock_t *net_sock = (net_sock_t *)param;
    while (1)
    {
        rc = net_read(net_sock, buffer, TEST_BUFFER_SIZE, 1);
        if (rc > 0)
        {
            if (!strncmp(buffer, "QUIT",4))
            {
                break;
            }
            else
            {
                /* Send received data */
                rc = net_send(net_sock, buffer, rc);
            }
        }
        else if (rc == 0)
        {
            /* Timeout */
        }
        else
        {
            if (rc != -2 && net_sock->sndrcv_errno != EAGAIN)
                break;
        }
    }

    net_close(net_sock);

    return NULL;
}

static void start_stream_sctp_session(net_sock_t *net_sock)
{
    pthread_t tid;

    pthread_create(&tid, NULL, sctp_stream_session_main, (void *)net_sock);
    pthread_detach(tid);
    return;
}

static void *sctp_stream_server_main(void *param)
{
    int rc;
    net_sock_t *new_sock;

    rc = net_listen(&streamserver_sock, 
            SOCK_STREAM, IPPROTO_SCTP, TEST_SERVER_PORT);
    if (rc != 0)
    {
        d_error("net_sctp_listen Error(rc = %d)\n",rc);
        return NULL;
    }

    sctp_stream_server_started = 1;

    while (1)
    {
        rc = net_accept(&new_sock, streamserver_sock, 1);
        if (rc >0)
        {
            /* New connection arrived. Start session */
            start_stream_sctp_session(new_sock);
        }
        else if (rc == 0)
        {
            /* Timeout */
        }
        else
        {
            /* Error occured */
            break;
        }
    }

    return NULL;
}

static void start_stream_sctp_server()
{
    pthread_create(&streamserver_tid, NULL, sctp_stream_server_main, NULL);
    while (sctp_stream_server_started == 0)
    {
        sleep(1);
    }
    return;
}

static void stop_stream_sctp_server()
{
    net_close(streamserver_sock);
    pthread_join(streamserver_tid, NULL);
}

static void *sctp_seq_server_main(void *param)
{
    int rc;

    rc = net_listen(&seqserver_sock, 
            SOCK_SEQPACKET, IPPROTO_SCTP, TEST_SERVER_PORT);
    if (rc != 0)
    {
        d_error("net_sctp Error(rc = %d)\n",rc);
        return NULL;
    }

    sctp_seq_server_started = 1;

    while (1)
    {
        d_trace(1,"Wait for data....\n");
        rc = net_read(seqserver_sock, buffer, TEST_BUFFER_SIZE, 2);
        if (rc >0)
        {
            d_trace(1,"RECV %d bytes\n", rc);
            if (!strncmp(buffer, "QUIT",4))
            {
                break;
            }
            else
            {
                /* Send received data */
                rc = net_send(seqserver_sock, buffer, rc);
                d_trace(1,"SEND %d bytes\n", rc);
                if (rc == -1)
                {
                    printf("error = %d\n", seqserver_sock->sndrcv_errno);
                }
            }
        }
        else if (rc == 0)
        {
            /* Timeout */
        }
        else
        {
            /* Error occured */
            if (rc != -2 && seqserver_sock->sndrcv_errno != EAGAIN)
                break;
        }
    }

    return NULL;
}

static void start_seq_sctp_server()
{
    pthread_create(&seqserver_tid, NULL, sctp_seq_server_main, NULL);
    while (sctp_seq_server_started == 0)
    {
        sleep(1);
    }
    return;
}

static void stop_seq_sctp_server()
{
    net_close(seqserver_sock);
    pthread_join(seqserver_tid, NULL);
}

static void test_sctp1(abts_case *tc, void *data)
{
    int rc = 0;
    net_sock_t *net_sock[TEST_MAX_NUM];
    char inputbuf[TEST_MAX_NUM][25];
    char outputbuf[TEST_MAX_NUM][25];
    int i;

    /* Start SCTP Server */
    start_stream_sctp_server();

    /* Connect to invalid port */
    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_FATAL);
    for (i =0 ; i<TEST_MAX_NUM; i++)
    {
        net_sock[i] = NULL;
        rc = net_open(&net_sock[i], "127.0.0.1", 0,TEST_SERVER_PORT + 1,
                SOCK_STREAM, IPPROTO_SCTP);
        ABTS_INT_EQUAL(tc, -1, rc);
        ABTS_PTR_NULL(tc, net_sock[i]);
    }
    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_FULL);

    for (i =0 ; i<TEST_MAX_NUM; i++)
    {
        net_sock[i] = NULL;
        rc = net_open(&net_sock[i], "127.0.0.1", 0, TEST_SERVER_PORT,
                SOCK_STREAM, IPPROTO_SCTP);
        ABTS_INT_EQUAL(tc, 0, rc);
        ABTS_PTR_NOTNULL(tc, net_sock[i]);
    }

    for (i=0; i< TEST_MAX_NUM; i++)
    {
        sprintf(inputbuf[i],"asdf%d",i);
        memset(outputbuf[i], 0, sizeof(outputbuf[i]));
        rc = net_send(net_sock[i], inputbuf[i], strlen(inputbuf[i])+1);
        ABTS_INT_EQUAL(tc, strlen(inputbuf[i])+1, rc);
        rc = 0;
        while (1)
        {
            int n;
            n = net_read(net_sock[i], outputbuf[i], sizeof(outputbuf[1]), 1);
            if (n < 0 && net_sock[i]->sndrcv_errno == EAGAIN)
                continue;
            rc += n;
            if (n == 0 || n == 6)
                break;
        }
        ABTS_INT_EQUAL(tc, 6, rc);
        ABTS_INT_EQUAL(tc, 6, strlen(outputbuf[i])+1);
        ABTS_STR_EQUAL(tc, inputbuf[i], outputbuf[i]);
    }

    for (i = 0 ; i< TEST_MAX_NUM; i++)
    {
        rc = net_close(net_sock[i]);
        ABTS_INT_EQUAL(tc, 0, rc);
    }

    stop_stream_sctp_server();
}

static void test_sctp2(abts_case *tc, void *data)
{
    int rc = 0;
    net_sock_t *net_sock[TEST_MAX_NUM];
    char inputbuf[TEST_MAX_NUM][25];
    char outputbuf[TEST_MAX_NUM][25];
    int i;

    /* Connect to invalid port.
     * In SCTP cases, net_open should be success always
     */
    for (i =0 ; i<TEST_MAX_NUM; i++)
    {
        net_sock[i] = NULL;
        rc = net_open(&net_sock[i], "127.0.0.1", 0, TEST_SERVER_PORT + 1,
                SOCK_SEQPACKET, IPPROTO_SCTP);
        ABTS_INT_EQUAL(tc, 0, rc);
        ABTS_PTR_NOTNULL(tc, net_sock[i]);
    }

    for (i = 0 ; i< TEST_MAX_NUM; i++)
    {
        rc = net_close(net_sock[i]);
        ABTS_INT_EQUAL(tc, 0, rc);
    }

    /* Start SCTP Server */
    start_seq_sctp_server();

    for (i =0 ; i<TEST_MAX_NUM; i++)
    {
        net_sock[i] = NULL;
        rc = net_open(&net_sock[i], "127.0.0.1", 0, TEST_SERVER_PORT,
                SOCK_SEQPACKET, IPPROTO_SCTP);
        ABTS_INT_EQUAL(tc, 0, rc);
        ABTS_PTR_NOTNULL(tc, net_sock[i]);
    }

    for (i=0; i< TEST_MAX_NUM; i++)
    {
        sprintf(inputbuf[i],"asdf%d",i);
        memset(outputbuf[i], 0, sizeof(outputbuf[i]));
        rc = net_sendto(net_sock[i], inputbuf[i], strlen(inputbuf[i])+1, 
                inet_addr("127.0.0.1"), TEST_SERVER_PORT);
        ABTS_INT_EQUAL(tc, strlen(inputbuf[i])+1, rc);
        rc = 0;
        while (1)
        {
            int n;
            n = net_read(net_sock[i], outputbuf[i], sizeof(outputbuf[i]), 1);
            if (n < 0 && net_sock[i]->sndrcv_errno == EAGAIN)
                continue;
            rc += n;
            if (n == 0 || n == 6)
                break;
        }
        ABTS_INT_EQUAL(tc, 6, rc);
        ABTS_INT_EQUAL(tc, 6, strlen(outputbuf[i])+1);
        ABTS_STR_EQUAL(tc, inputbuf[i], outputbuf[i]);
    }

    for (i = 0 ; i< TEST_MAX_NUM; i++)
    {
        rc = net_close(net_sock[i]);
        ABTS_INT_EQUAL(tc, 0, rc);
    }

    stop_seq_sctp_server();
}

abts_suite *testsctp(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, test_sctp1, NULL);
    abts_run_test(suite, test_sctp2, NULL);

    return suite;
}
