#include "core_msgq.h"
#include "core_thread.h"
#include "core_portable.h"
#include "testutil.h"

static char msg[16][24] = {
    {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18},
    {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28},
    {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38},
    {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48},
    {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58},
    {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68},
    {0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78},
    {0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88},
    {0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98},
    {0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8},
    {0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8},
    {0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8},
    {0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8},
    {0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8},
    {0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8},
    {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}
};

static char rmsg[16][24];

static int msglen[16] = {3, 4, 5, 6, 7, 8, 1, 2, 11, 12, 13, 14, 15, 16, 17, 18};

static msgq_id md;

static void msgq_test1(abts_case *tc, void *data)
{
    int i, n;

    msgq_init();

    /* Basic test */
    md = msgq_create(5, 8, 0);
    ABTS_INT_NEQUAL(tc, 0, md);

    n = msgq_send(md, msg[0], msglen[0]);
    ABTS_INT_EQUAL(tc, msglen[0], n);

    n = msgq_recv(md, rmsg[0], 8);
    ABTS_INT_EQUAL(tc, msglen[0], n);

    n = memcmp(msg[0], rmsg[0], msglen[0]);
    ABTS_INT_EQUAL(tc, 0, n);

    msgq_delete(md);

    /* Test with send() and recv() function up to queue size */
    md = msgq_create(5, 8, 0);
    ABTS_INT_NEQUAL(tc, 0, md);

    for (i = 0; i < 5; i++)
    {
        n = msgq_send(md, msg[i], msglen[i]);
        ABTS_INT_EQUAL(tc, msglen[i], n);
    }

    for (i = 0; i < 5; i++)
    {
        n = msgq_recv(md, rmsg[i], 8);
        ABTS_INT_EQUAL(tc, msglen[i], n);

        n = memcmp(msg[i], rmsg[i], msglen[i]);
        ABTS_INT_EQUAL(tc, 0, n);
    }

    msgq_delete(md);

    /* Test with send() and timedrecv() function up to queue size */
    md = msgq_create(5, 8, 0);
    ABTS_INT_NEQUAL(tc, 0, md);

    for (i = 0; i < 5; i++)
    {
        n = msgq_send(md, msg[i], msglen[i]);
        ABTS_INT_EQUAL(tc, msglen[i], n);
    }

    for (i = 0; i < 5; i++)
    {
        n = msgq_timedrecv(md, rmsg[i], 8, 0);
        ABTS_INT_EQUAL(tc, msglen[i], n);

        n = memcmp(msg[i], rmsg[i], msglen[i]);
        ABTS_INT_EQUAL(tc, 0, n);
    }

    msgq_delete(md);
}

static void msgq_test2(abts_case *tc, void *data)
{
    int i, n;

    msgq_init();

    md = msgq_create(5, 8, MSGQ_O_NONBLOCK);
    ABTS_INT_NEQUAL(tc, 0, md);

    /* fill up the queue */
    for (i = 0; i < 5; i++)
    {
        n = msgq_send(md, msg[i], msglen[i]);
        ABTS_INT_EQUAL(tc, msglen[i], n);
    }

    /* Now, there is no room to send.
     * Confirm that send() returns CORE_EGAIN */
    n = msgq_send(md, msg[0], msglen[0]);
    ABTS_INT_EQUAL(tc, CORE_EAGAIN, n);

    /* empty queue */
    for (i = 0; i < 5; i++)
    {
        n = msgq_recv(md, rmsg[i], 8);
        ABTS_INT_EQUAL(tc, msglen[i], n);

        n = memcmp(msg[i], rmsg[i], msglen[i]);
        ABTS_INT_EQUAL(tc, 0, n);
    }

    /* Now, there is no sent buffer to be read.
     * Confirm that recv() return CORE_EGAIN */
    n = msgq_recv(md, rmsg[i], 8);
    ABTS_INT_EQUAL(tc, CORE_EAGAIN, n);

    msgq_delete(md);
}

static void msgq_test3(abts_case *tc, void *data)
{
    int i, j, n;

    msgq_init();

    md = msgq_create(16, 24, MSGQ_O_BLOCK);
    ABTS_INT_NEQUAL(tc, 0, md);

    /* Repeat 10 times */
    for (j = 0; j < 10; j++)
    {
        /* Cycle repeatedly by queue depth */
        for (i = 0; i < 16; i++)
        {
            n = msgq_send(md, msg[i], 24);
            ABTS_INT_EQUAL(tc, 24, n);

            n = msgq_recv(md, rmsg[i], 24);
            ABTS_INT_EQUAL(tc, 24, n);

            n = memcmp(msg[i], rmsg[i], 24);
            ABTS_INT_EQUAL(tc, 0, n);
        }
    }

    msgq_delete(md);
}

#define TEST_QUEUE_SIZE 128
#define TEST_EVT_SIZE (sizeof(test_event_t))

typedef struct {
    int a;
    int b;
    char c[30];
} test_event_t;

static thread_id thr_producer;
static thread_id thr_consumer;
static int max = 1000000;
static int exit_ret_val = 123;

static void *THREAD_FUNC producer_main(thread_id id, void *data)
{
    int r;
    int i = 0;
    //time_t i_time = time(NULL);
    unsigned int full = 0;

    while (i++ < max)
    {
        test_event_t t;

        t.a = i;
        t.b = i+2;
        t.c[28] = 'X';
        t.c[29] = 'Y';
        ///printf("P: a = %d b = %d\n",t.a,t.b);
        r = msgq_send(md, (char*)&t, TEST_EVT_SIZE);
        if (r == CORE_EAGAIN)
        {
            full++;
            thread_yield();
            continue;
        }
        else if (r != TEST_EVT_SIZE)
        {
            printf("Producer Error\n");
        }
    }
#if 0
    printf("Total Send:%u Drop = %u , elapsed time: %u\n",
            i,
            full,
            (unsigned int)time(NULL)-(unsigned int)i_time);
#endif
    thread_exit(id, exit_ret_val);

    return NULL;
}

static void *THREAD_FUNC consumer_main(thread_id id, void *data)
{
    abts_case *tc = data;
    int r;
    ///int prev = -1;
    int i = 0;
    ///char str[256];

    while (!thread_should_stop())
    {
        test_event_t t;

#if HAVE_PTHREAD_H == 1
        pthread_testcancel();
#endif
        r = msgq_recv(md, (char*)&t, TEST_EVT_SIZE);
        if (r == CORE_EAGAIN)
        {
            thread_yield();
            continue;
        }
        ABTS_ASSERT(tc, "consumer error", r == TEST_EVT_SIZE);
        ABTS_ASSERT(tc, "consumer error", t.c[28] == 'X' && t.c[29] == 'Y');
        ///sprintf(str, "consumer error - prev:%d, t.a:%d", prev, t.a);
        ///ABTS_ASSERT(tc, str, (prev+1) == t.a);
        ///prev = t.a;
        ///printf("C: a = %d b = %d\n",t.a,t.b);
        i++;
    }

    return NULL;
}

static void *THREAD_FUNC timedconsumer_main(thread_id id, void *data)
{
    abts_case *tc = data;
    int r;
    ///int prev = -1;
    int i = 0;

    while (!thread_should_stop())
    {
        test_event_t t;

#if HAVE_PTHREAD_H == 1
        pthread_testcancel();
#endif
        r = msgq_timedrecv(md, (char*)&t, TEST_EVT_SIZE, 10000);
        if (r == CORE_EAGAIN || r == CORE_TIMEUP)
        {
            thread_yield();
            continue;
        }
        ABTS_ASSERT(tc, "consumer error", r == TEST_EVT_SIZE);
        ABTS_ASSERT(tc, "consumer error", t.c[28] == 'X' && t.c[29] == 'Y');
        ///prev = t.a;
        i++;
    }

    return NULL;
}

static void msgq_test4(abts_case *tc, void *data)
{
    status_t rv;
    int opt = (int)data;
#if HAVE_PTHREAD_H == 1
    os_thread_t *thread;
#endif

    md = msgq_create(TEST_QUEUE_SIZE, TEST_EVT_SIZE, opt);
    ABTS_INT_NEQUAL(tc, 0, md);

    rv = thread_create(&thr_producer, NULL, producer_main, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&thr_consumer, NULL, consumer_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_join(&rv, thr_producer);
    ABTS_INT_EQUAL(tc, exit_ret_val, rv);

#if HAVE_PTHREAD_H == 1
    os_thread_get(&thread, thr_consumer);
    pthread_cancel(*thread);
    thread_join(&rv, thr_consumer);
#else
    thread_delete(thr_consumer);
#endif

    msgq_delete(md);
}

static void msgq_test5(abts_case *tc, void *data)
{
    status_t rv;
    int opt = (int)data;
#if HAVE_PTHREAD_H == 1
    os_thread_t *thread;
#endif

    md = msgq_create(TEST_QUEUE_SIZE, TEST_EVT_SIZE, opt);
    ABTS_INT_NEQUAL(tc, 0, md);

    rv = thread_create(&thr_producer, NULL, producer_main, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = thread_create(&thr_consumer, NULL, timedconsumer_main, tc);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    thread_join(&rv, thr_producer);
    ABTS_INT_EQUAL(tc, exit_ret_val, rv);

#if HAVE_PTHREAD_H == 1
    os_thread_get(&thread, thr_consumer);
    pthread_cancel(*thread);
    thread_join(&rv, thr_consumer);
#else
    thread_delete(thr_consumer);
#endif

    msgq_delete(md);
}

#define STRESS_TEST 1

abts_suite *testmsgq(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

#if 0 /* Deprecated */
    abts_run_test(suite, msgq_test1, NULL);
    abts_run_test(suite, msgq_test2, NULL);
#endif
    abts_run_test(suite, msgq_test3, NULL);

#if STRESS_TEST == 1
    while(1)
    {
#endif
        abts_run_test(suite, msgq_test4, (void *)MSGQ_O_NONBLOCK);
        abts_run_test(suite, msgq_test4, (void *)MSGQ_O_BLOCK);
        abts_run_test(suite, msgq_test5, (void *)MSGQ_O_NONBLOCK);
        abts_run_test(suite, msgq_test5, (void *)MSGQ_O_BLOCK);
#if STRESS_TEST == 1
        printf("Test again = %u\n", (unsigned int)time(NULL));
        sleep(5);
    }
#endif

    return suite;
}
