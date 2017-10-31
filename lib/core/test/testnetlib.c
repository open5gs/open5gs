#define TRACE_MODULE _testnetlib
#include "core_debug.h"
#include "core_net.h"
#include "testutil.h"

#define TEST_SERVER_PORT    5121
#define TEST_BUFFER_SIZE    1024

#define TEST_MAX_NUM     4

#define DISABLE_FTPTEST  1
#define DISABLE_LINKTEST 1

static char buffer[TEST_BUFFER_SIZE];
static int tcp_server_started = 0;
static int udp_server_started = 0;

pthread_t tserver_tid,userver_tid;
net_sock_t *tserver_sock,*userver_sock;

static void *tcp_session_main(void *param)
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
            printf("error = %d\n", net_sock->sndrcv_errno);
            break;
        }
    }

    net_close(net_sock);

    return NULL;
}

static void start_tcp_session(net_sock_t *net_sock)
{
    pthread_t tid;

    pthread_create(&tid, NULL, tcp_session_main, (void *)net_sock);
    pthread_detach(tid);
    return;
}

static void *tcp_server_main(void *param)
{
    int rc;
    net_sock_t *new_sock;

    rc = net_listen(&tserver_sock, SOCK_STREAM, IPPROTO_TCP, TEST_SERVER_PORT);
    if (rc != 0)
    {
        d_error("net_tcp_listen Error(rc = %d)\n",rc);
        return NULL;
    }

    tcp_server_started = 1;

    while (1)
    {
        rc = net_accept(&new_sock, tserver_sock, 1);
        if (rc >0)
        {
            /* New connection arrived. Start session */
            start_tcp_session(new_sock);
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

static void start_tcp_server()
{
    pthread_create(&tserver_tid, NULL, tcp_server_main, NULL);
    while (tcp_server_started == 0)
    {
        sleep(1);
    }
    sleep(1);
    return;
}

static void stop_tcp_server()
{
    net_close(tserver_sock);
    pthread_join(tserver_tid, NULL);
}

static void *udp_server_main(void *param)
{
    int rc;

    rc = net_listen(&userver_sock, 
            SOCK_DGRAM, IPPROTO_UDP, TEST_SERVER_PORT);
    if (rc != 0)
    {
        d_error("net_udp Error(rc = %d)\n",rc);
        return NULL;
    }

    udp_server_started = 1;

    while (1)
    {
        d_trace(1,"Wait for data....\n");
        rc = net_read(userver_sock, buffer, TEST_BUFFER_SIZE, 2);
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
                rc = net_send(userver_sock, buffer, rc);
                d_trace(1,"SEND %d bytes\n", rc);
                if (rc == -1)
                {
                    printf("error = %d\n", userver_sock->sndrcv_errno);
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
            break;
        }
    }

    return NULL;
}

static void start_udp_server()
{
    pthread_create(&userver_tid, NULL, udp_server_main, NULL);
    while (udp_server_started == 0)
    {
        sleep(1);
    }
    return;
}

static void stop_udp_server()
{
    net_close(userver_sock);
    pthread_join(userver_tid, NULL);
}

static void netlib1(abts_case *tc, void *data)
{
    int rc = 0;
    net_sock_t *net_sock;
    char inputbuf[TEST_MAX_NUM][20];
    char outputbuf[TEST_MAX_NUM][20];
    int i;

    /* Start TCP server */
    start_tcp_server();

    rc = net_open(&net_sock, "127.0.0.1", 0,
            TEST_SERVER_PORT, SOCK_STREAM, IPPROTO_TCP);
    ABTS_INT_EQUAL(tc, 0, rc);
    for (i=0; i< TEST_MAX_NUM; i++)
    {
        sprintf(inputbuf[i],"asdf%d",i);
        rc = net_send(net_sock, inputbuf[i], strlen(inputbuf[i])+1);
        ABTS_INT_EQUAL(tc, strlen(inputbuf[i])+1, rc);
    }

    for (i=0; i< TEST_MAX_NUM; i++)
    {
        memset(outputbuf[i], 0, sizeof(outputbuf[i]));
        rc = 0;
        while (1)
        {
            int n;
            n = net_read(net_sock, outputbuf[i], 6, 1);
            rc += n;
            if (n == 0 || n == 6)
                break;
        }
        ABTS_INT_EQUAL(tc, 6, rc);
        ABTS_INT_EQUAL(tc, 6, strlen(outputbuf[i])+1);
        ABTS_STR_EQUAL(tc, inputbuf[i], outputbuf[i]);
    }
    /* Send QUIT */
    rc = net_send(net_sock, "QUIT", 4);
    ABTS_INT_EQUAL(tc, 4, rc);

    /* Close */
    rc = net_close(net_sock);
    ABTS_INT_EQUAL(tc, 0, rc);

    /* Stop TCP server */
    stop_tcp_server();
}

static void netlib2(abts_case *tc, void *data)
{
    int rc = 0;
    net_sock_t *net_sock[TEST_MAX_NUM];
    char inputbuf[TEST_MAX_NUM][20];
    char outputbuf[TEST_MAX_NUM][20];
    int i;

    /* Start TCP server */
    start_tcp_server();

    /* Connect to invalid port */
    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_FATAL);
    for (i =0 ; i<TEST_MAX_NUM; i++)
    {
        net_sock[i] = NULL;
        rc = net_open(&net_sock[i], "127.0.0.1", 0,TEST_SERVER_PORT + 1,
                SOCK_STREAM, IPPROTO_TCP);
        ABTS_INT_EQUAL(tc, -1, rc);
        ABTS_PTR_NULL(tc, net_sock[i]);
    }
    d_log_set_level(D_MSG_TO_STDOUT, D_LOG_LEVEL_FULL);

    /* Connect to valid port */
    for (i =0 ; i<TEST_MAX_NUM; i++)
    {
        net_sock[i] = NULL;
        rc = net_open(&net_sock[i], "127.0.0.1", 0,TEST_SERVER_PORT,
                SOCK_STREAM, IPPROTO_TCP);
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
            n = net_read(net_sock[i], outputbuf[i], 6, 1);
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

    /* Stop TCP server */
    stop_tcp_server();
}

static void netlib3(abts_case *tc, void *data)
{
    int rc = 0;
    net_sock_t *net_sock[TEST_MAX_NUM];
    char inputbuf[TEST_MAX_NUM][20];
    char outputbuf[TEST_MAX_NUM][20];
    int i;

    /* Connect to invalid port.
     * In UDP cases, net_open should be success always
     */
    for (i =0 ; i<TEST_MAX_NUM; i++)
    {
        net_sock[i] = NULL;
        rc = net_open(&net_sock[i], "127.0.0.1", 0,TEST_SERVER_PORT + 1,
                SOCK_DGRAM, IPPROTO_UDP);
        ABTS_INT_EQUAL(tc, 0, rc);
        ABTS_PTR_NOTNULL(tc, net_sock[i]);
    }

    for (i = 0 ; i< TEST_MAX_NUM; i++)
    {
        rc = net_close(net_sock[i]);
        ABTS_INT_EQUAL(tc, 0, rc);
    }

    /* Start UDP Server */
    start_udp_server();

    for (i =0 ; i<TEST_MAX_NUM; i++)
    {
        net_sock[i] = NULL;
        rc = net_open(&net_sock[i], "127.0.0.1", 0,TEST_SERVER_PORT,
                SOCK_DGRAM, IPPROTO_UDP);
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
            n = net_read(net_sock[i], outputbuf[i], 6, 1);
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

    stop_udp_server();
}

static void netlib4(abts_case *tc, void *data)
{
    int rc;
    net_ftp_t *ftp_session = NULL;
    char *homedir = NULL;
    int remote_size = 0;
    int local_size = 0;
    char host[] = "127.0.0.1:21";
    int i;

#if DISABLE_FTPTEST
    return;
#endif
    rc = net_ftp_open(host,
            "susia",
            "ich3lie",
            0,&ftp_session);
    ABTS_INT_EQUAL(tc, 0, rc);
    ABTS_PTR_NOTNULL(tc, ftp_session);

    homedir = getenv("HOME");
    if (homedir)
    {
        char filename[100];
        struct stat statbuff;
        sprintf(filename,"%s/.bashrc",homedir);

        if (stat(filename, &statbuff) == 0)
        {
            remote_size= statbuff.st_size;
        }
    }
    rc = net_ftp_get(ftp_session,".bashrc",NULL);
    ABTS_INT_EQUAL(tc, 0, rc);
    {
        struct stat statbuff;

        rc = stat(".bashrc", &statbuff);
        ABTS_INT_EQUAL(tc, 0, rc);
        local_size= statbuff.st_size;
        ABTS_INT_EQUAL(tc, local_size, remote_size);
    }

    for (i=0; i < 20; i++)
    {
        rc = net_ftp_get(ftp_session,".bashrc","bashrc");
        ABTS_INT_EQUAL(tc, 0, rc);
        {
            struct stat statbuff;
            rc = stat("bashrc", &statbuff);
            ABTS_INT_EQUAL(tc, 0, rc);
            local_size= statbuff.st_size;
            ABTS_INT_EQUAL(tc, local_size, remote_size);
        }
        rc = net_ftp_put(ftp_session,"bashrc", NULL);
        ABTS_INT_EQUAL(tc, 0, rc);

        unlink("bashrc");
    }

    rc = net_ftp_quit(ftp_session);
    ABTS_INT_EQUAL(tc, 0, rc);

    rc = net_ftp_close(ftp_session);
    ABTS_INT_EQUAL(tc, 0, rc);
}

#if LINUX == 1
static void filter_updu(char *buf, int len)
{
    unsigned short proto = 0x88B6;
    struct ethhdr *eth_hdr = NULL;

    eth_hdr = (struct ethhdr *)buf;
    if (ntohs(eth_hdr->h_proto) == proto)
    {
        d_print_hex(buf, len);
    }
}

static void netlib5(abts_case *tc, void *data)
{
    net_link_t *net_link = NULL;
    int promisc = 1;
    int rc;
    int max_count = 10;
    char buf[1024];

#if DISABLE_LINKTEST
    return;
#endif
    rc = net_link_open(&net_link, "eth0", ETH_P_ALL);
    ABTS_INT_EQUAL(tc, 0, rc);
    ABTS_PTR_NOTNULL(tc, net_link);
    rc = net_link_promisc(net_link, promisc);
    ABTS_INT_EQUAL(tc, 0, rc);
#if 0
    d_print("HW addr of %s : ",net_link->ifname);
    d_print_hex(net_link->hwaddr.sa_data, 6);
#endif
    while (max_count-- > 0)
    {
        rc = net_link_read(net_link, buf, 1024, 1);
        ABTS_TRUE(tc, rc > 0);
        filter_updu(buf, rc);
    }
    rc = net_link_close(net_link);
    ABTS_INT_EQUAL(tc, 0, rc);
}

static int make_test_updu(char *src_addr, char *dst_addr, char *buf, int len)
{
    unsigned short proto = 0x88B6;
    struct ethhdr *eth_hdr = NULL;
    int rc = sizeof(struct ethhdr);
    char *ptr = buf;

    eth_hdr = (struct ethhdr *)buf;
    memcpy(eth_hdr->h_source, src_addr, 6);
    memcpy(eth_hdr->h_dest, dst_addr, 6);
    eth_hdr->h_proto = htons(proto);
    /* Fill the data */
    rc += sprintf(ptr+rc,"Hellow World");
    d_print_hex(buf, rc);
    return rc;
}

static void netlib6(abts_case *tc, void *data)
{
    net_link_t *net_link = NULL;
    int promisc = 1;
    int rc;
    int max_count = 1;
    char buf[1024];
#if 0
    char dst_addr[6] = {'\x00','\x00','\x00','\x11','\x22','\x33'};
#else
    char dst_addr[6] = {'\xff','\xff','\xff','\xff','\xff','\xff'};
#endif

#if DISABLE_LINKTEST
    return;
#endif

    rc = net_link_open(&net_link, "eth0", ETH_P_ALL);
    ABTS_INT_EQUAL(tc, 0, rc);
    ABTS_PTR_NOTNULL(tc, net_link);
    rc = net_link_promisc(net_link, promisc);
    ABTS_INT_EQUAL(tc, 0, rc);
    while (max_count-- > 0)
    {
        rc = make_test_updu(net_link->hwaddr.sa_data, dst_addr,buf,1024);

        rc = net_link_write(net_link, buf, rc);
        ABTS_TRUE(tc, rc > 0);
    }
    rc = net_link_close(net_link);
    ABTS_INT_EQUAL(tc, 0, rc);
}
#endif

abts_suite *testnetlib(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, netlib1, NULL);
    /*
     * OpenSUSE OBS
     * - Ubuntu 17.04 i586 failed
     * - Jinyoung Fixed
     *
[  542s] testnetlib          :  Line 262: expected <0>, but saw <-1>
[  542s] [10/30 07:48:38.730] ERRR: connect error(111:Connection refused)(proto:6 remote:127.0.0.1 dport:5121 lport:0) (net_lib.c:353)
[  542s] [10/30 07:48:38.730] ERRR: connect error(111:Connection refused)(proto:6 remote:127.0.0.1 dport:5121 lport:0) (net_lib.c:353)
[  542s] [10/30 07:48:38.730] ERRR: connect error(111:Connection refused)(proto:6 remote:127.0.0.1 dport:5121 lport:0) (net_lib.c:353)
[  542s] [10/30 07:48:38.730] ERRR: connect error(111:Connection refused)(proto:6 remote:127.0.0.1 dport:5121 lport:0) (net_lib.c:353)
[  542s] [10/30 07:48:38.730] ASSERT: !(net_sock && buffer). Invalid params
[  542s]  (net_lib.c:590)
[  542s] [10/30 07:48:38.730] ASSERT: !(net_sock). net_sock is NULL
[  542s]  (net_lib.c:408)
[  542s] [10/30 07:48:38.730] ASSERT: !(net_sock). net_sock is NULL
    */
    abts_run_test(suite, netlib2, NULL);
    abts_run_test(suite, netlib3, NULL);
    abts_run_test(suite, netlib4, NULL);
#if LINUX == 1
    abts_run_test(suite, netlib5, NULL);
    abts_run_test(suite, netlib6, NULL);
#endif

    return suite;
}
