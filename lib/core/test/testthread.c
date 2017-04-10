#include "core_mutex.h"
#include "core_thread.h"
#include "testutil.h"

static mutex_id lock;
static int x = 0;

static status_t exit_ret_val = 123;

static thread_id t1;
static thread_id t2;
static thread_id t3;
static thread_id t4;

static void *THREAD_FUNC thread_func1(thread_id id, void *data)
{
    int i;

    for (i = 0; i < 10000; i++)
    {
        mutex_lock(lock);
        x++;
        mutex_unlock(lock);
    }
    thread_exit(id, exit_ret_val);
    return NULL;
}

static void init_thread(abts_case *tc, void *data)
{
    status_t rv;

    rv = mutex_create(&lock, MUTEX_DEFAULT);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static void create_threads(abts_case *tc, void *data)
{
    status_t rv;

    rv = thread_create(&t1, NULL, thread_func1, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = thread_create(&t2, NULL, thread_func1, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = thread_create(&t3, NULL, thread_func1, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    rv = thread_create(&t4, NULL, thread_func1, NULL);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static void join_threads(abts_case *tc, void *data)
{
    status_t s;

    thread_join(&s, t1);
    ABTS_INT_EQUAL(tc, exit_ret_val, s);
    thread_join(&s, t2);
    ABTS_INT_EQUAL(tc, exit_ret_val, s);
    thread_join(&s, t3);
    ABTS_INT_EQUAL(tc, exit_ret_val, s);
    thread_join(&s, t4);
    ABTS_INT_EQUAL(tc, exit_ret_val, s);
}

static void check_locks(abts_case *tc, void *data)
{
    ABTS_INT_EQUAL(tc, 40000, x);
}

static void final_thread(abts_case *tc, void *data)
{
    status_t rv;

    rv = mutex_delete(lock);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

abts_suite *testthread(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, init_thread, NULL);
    abts_run_test(suite, create_threads, NULL);
    abts_run_test(suite, join_threads, NULL);
    abts_run_test(suite, check_locks, NULL);
    abts_run_test(suite, final_thread, NULL);

    return suite;
}

