#include "core_atomic.h"
#include "core_thread.h"
#include "core_mutex.h"
#include "testutil.h"

static void test_set32(abts_case *tc, void *data)
{
    c_uint32_t y32;
    atomic_set32(&y32, 2);
    ABTS_INT_EQUAL(tc, 2, y32);
}

static void test_read32(abts_case *tc, void *data)
{
    c_uint32_t y32;
    atomic_set32(&y32, 2);
    ABTS_INT_EQUAL(tc, 2, atomic_read32(&y32));
}

static void test_dec32(abts_case *tc, void *data)
{
    c_uint32_t y32;
    int rv;

    atomic_set32(&y32, 2);

    rv = atomic_dec32(&y32);
    ABTS_INT_EQUAL(tc, 1, y32);
    ABTS_ASSERT(tc, "atomic_dec returned zero when it shouldn't", rv != 0);

    rv = atomic_dec32(&y32);
    ABTS_INT_EQUAL(tc, 0, y32);
    ABTS_ASSERT(tc, "atomic_dec didn't returned zero when it should", rv == 0);
}

static void test_xchg32(abts_case *tc, void *data)
{
    c_uint32_t oldval;
    c_uint32_t y32;

    atomic_set32(&y32, 100);
    oldval = atomic_xchg32(&y32, 50);

    ABTS_INT_EQUAL(tc, 100, oldval);
    ABTS_INT_EQUAL(tc, 50, y32);
}

static void test_xchgptr(abts_case *tc, void *data)
{
    int a;
    void *ref = "little piggy";
    void *target_ptr = ref;
    void *old_ptr;

    old_ptr = atomic_xchgptr(&target_ptr, &a);
    ABTS_PTR_EQUAL(tc, ref, old_ptr);
    ABTS_PTR_EQUAL(tc, &a, (void *) target_ptr);
}

static void test_cas_equal(abts_case *tc, void *data)
{
    c_uint32_t casval = 0;
    c_uint32_t oldval;

    oldval = atomic_cas32(&casval, 12, 0);
    ABTS_INT_EQUAL(tc, 0, oldval);
    ABTS_INT_EQUAL(tc, 12, casval);
}

static void test_cas_equal_nonnull(abts_case *tc, void *data)
{
    c_uint32_t casval = 12;
    c_uint32_t oldval;

    oldval = atomic_cas32(&casval, 23, 12);
    ABTS_INT_EQUAL(tc, 12, oldval);
    ABTS_INT_EQUAL(tc, 23, casval);
}

static void test_cas_notequal(abts_case *tc, void *data)
{
    c_uint32_t casval = 12;
    c_uint32_t oldval;

    oldval = atomic_cas32(&casval, 23, 2);
    ABTS_INT_EQUAL(tc, 12, oldval);
    ABTS_INT_EQUAL(tc, 12, casval);
}

static void test_casptr_equal(abts_case *tc, void *data)
{
    int a;
    void *target_ptr = NULL;
    void *old_ptr;

    old_ptr = atomic_casptr(&target_ptr, &a, NULL);
    ABTS_PTR_EQUAL(tc, NULL, old_ptr);
    ABTS_PTR_EQUAL(tc, &a, (void *) target_ptr);
}

static void test_casptr_equal_nonnull(abts_case *tc, void *data)
{
    int a, b;
    void *target_ptr = &a;
    void *old_ptr;

    old_ptr = atomic_casptr(&target_ptr, &b, &a);
    ABTS_PTR_EQUAL(tc, &a, old_ptr);
    ABTS_PTR_EQUAL(tc, &b, (void *) target_ptr);
}

static void test_casptr_notequal(abts_case *tc, void *data)
{
    int a, b;
    void *target_ptr = &a;
    void *old_ptr;

    old_ptr = atomic_casptr(&target_ptr, &a, &b);
    ABTS_PTR_EQUAL(tc, &a, old_ptr);
    ABTS_PTR_EQUAL(tc, &a, (void *) target_ptr);
}

static void test_add32(abts_case *tc, void *data)
{
    c_uint32_t oldval;
    c_uint32_t y32;

    atomic_set32(&y32, 23);
    oldval = atomic_add32(&y32, 4);
    ABTS_INT_EQUAL(tc, 23, oldval);
    ABTS_INT_EQUAL(tc, 27, y32);
}

static void test_add32_neg(abts_case *tc, void *data)
{
    c_uint32_t oldval;
    c_uint32_t y32;

    atomic_set32(&y32, 23);
    oldval = atomic_add32(&y32, -10);
    ABTS_INT_EQUAL(tc, 23, oldval);
    ABTS_INT_EQUAL(tc, 13, y32);
}

static void test_inc32(abts_case *tc, void *data)
{
    c_uint32_t oldval;
    c_uint32_t y32;

    atomic_set32(&y32, 23);
    oldval = atomic_inc32(&y32);
    ABTS_INT_EQUAL(tc, 23, oldval);
    ABTS_INT_EQUAL(tc, 24, y32);
}

static void test_set_add_inc_sub(abts_case *tc, void *data)
{
    c_uint32_t y32;

    atomic_set32(&y32, 0);
    atomic_add32(&y32, 20);
    atomic_inc32(&y32);
    atomic_sub32(&y32, 10);

    ABTS_INT_EQUAL(tc, 11, y32);
}

static void test_wrap_zero(abts_case *tc, void *data)
{
    c_uint32_t y32;
    c_uint32_t rv;
    c_uint32_t minus1 = (c_uint32_t)-1;
    char str[256];;

    atomic_set32(&y32, 0);
    rv = atomic_dec32(&y32);

    ABTS_ASSERT(tc, "atomic_dec32 on zero returned zero.", rv != 0);
    sprintf(str, "zero wrap failed: 0 - 1 = %d", y32);
    ABTS_ASSERT(tc, str, y32 == minus1);
}

static void test_inc_neg1(abts_case *tc, void *data)
{
    c_uint32_t y32 = (c_uint32_t)-1;
    c_uint32_t minus1 = (c_uint32_t)-1;
    c_uint32_t rv;
    char str[256];

    rv = atomic_inc32(&y32);

    ABTS_ASSERT(tc, "atomic_inc32 didn't return the old value.", rv == minus1);
    sprintf(str, "zero wrap failed: -1 + 1 = %d", y32);
    ABTS_ASSERT(tc, str, y32 == 0);
}

void *THREAD_FUNC thread_func_mutex(thread_id id, void *data);
void *THREAD_FUNC thread_func_atomic(thread_id id, void *data);

mutex_id thread_lock;
volatile c_uint32_t mutex_locks = 0;
volatile c_uint32_t atomic_ops = 0;
status_t exit_ret_val = 123; /* just some made up number to check on later */

#define NUM_THREADS 40
#define NUM_ITERATIONS 20000

void *THREAD_FUNC thread_func_mutex(thread_id id, void *data)
{
    int i;

    for (i = 0; i < NUM_ITERATIONS; i++) {
        mutex_lock(thread_lock);
        mutex_locks++;
        mutex_unlock(thread_lock);
    }
    thread_exit(id, exit_ret_val);
    return NULL;
}

void *THREAD_FUNC thread_func_atomic(thread_id id, void *data)
{
    int i;

    for (i = 0; i < NUM_ITERATIONS ; i++) {
        atomic_inc32(&atomic_ops);
        atomic_add32(&atomic_ops, 2);
        atomic_dec32(&atomic_ops);
        atomic_dec32(&atomic_ops);
    }
    thread_exit(id, exit_ret_val);
    return NULL;
}

static void test_atomics_threaded(abts_case *tc, void *data)
{
    thread_id t1[NUM_THREADS];
    thread_id t2[NUM_THREADS];
    status_t rv;
    int i;

#ifdef HAVE_PTHREAD_SETCONCURRENCY
    pthread_setconcurrency(8);
#endif

    rv = mutex_create(&thread_lock, MUTEX_DEFAULT);
    ABTS_ASSERT(tc, "Could not create lock", rv == CORE_OK) ;

    for (i = 0; i < NUM_THREADS; i++) {
        status_t r1, r2;
        r1 = thread_create(&t1[i], NULL, thread_func_mutex, NULL);
        r2 = thread_create(&t2[i], NULL, thread_func_atomic, NULL);
        ABTS_ASSERT(tc, "Failed creating threads", !r1 && !r2);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        status_t s1, s2;
        thread_join(&s1, t1[i]);
        thread_join(&s2, t2[i]);

        ABTS_ASSERT(tc, "Invalid return value from thread_join",
                    s1 == exit_ret_val && s2 == exit_ret_val);
    }

    ABTS_INT_EQUAL(tc, NUM_THREADS * NUM_ITERATIONS, mutex_locks);
    ABTS_INT_EQUAL(tc, NUM_THREADS * NUM_ITERATIONS,
                   atomic_read32(&atomic_ops));

    rv = mutex_delete(thread_lock);
    ABTS_ASSERT(tc, "Failed creating threads", rv == CORE_OK);
}

#undef NUM_THREADS
#define NUM_THREADS 7

typedef struct tbox_t tbox_t;

struct tbox_t {
    abts_case *tc;
    c_uint32_t *mem;
    c_uint32_t preval;
    c_uint32_t postval;
    c_uint32_t loop;
    void (*func)(tbox_t *box);
};

static CORE_INLINE void busyloop_read32(tbox_t *tbox)
{
    c_uint32_t val;

    do {
        val = atomic_read32(tbox->mem);

        if (val != tbox->preval)
            thread_yield();
        else
            break;
    } while (1);
}

static void busyloop_set32(tbox_t *tbox)
{
    do {
        busyloop_read32(tbox);
        atomic_set32(tbox->mem, tbox->postval);
    } while (--tbox->loop);
}

static void busyloop_add32(tbox_t *tbox)
{
    c_uint32_t val;

    do {
        busyloop_read32(tbox);
        val = atomic_add32(tbox->mem, tbox->postval);
        mutex_lock(thread_lock);
        ABTS_INT_EQUAL(tbox->tc, val, tbox->preval);
        mutex_unlock(thread_lock);
    } while (--tbox->loop);
}

static void busyloop_sub32(tbox_t *tbox)
{
    do {
        busyloop_read32(tbox);
        atomic_sub32(tbox->mem, tbox->postval);
    } while (--tbox->loop);
}

static void busyloop_inc32(tbox_t *tbox)
{
    c_uint32_t val;

    do {
        busyloop_read32(tbox);
        val = atomic_inc32(tbox->mem);
        mutex_lock(thread_lock);
        ABTS_INT_EQUAL(tbox->tc, val, tbox->preval);
        mutex_unlock(thread_lock);
    } while (--tbox->loop);
}

static void busyloop_dec32(tbox_t *tbox)
{
    c_uint32_t val;

    do {
        busyloop_read32(tbox);
        val = atomic_dec32(tbox->mem);
        mutex_lock(thread_lock);
        ABTS_INT_NEQUAL(tbox->tc, 0, val);
        mutex_unlock(thread_lock);
    } while (--tbox->loop);
}

static void busyloop_cas32(tbox_t *tbox)
{
    c_uint32_t val;

    do {
        do {
            val = atomic_cas32(tbox->mem, tbox->postval, tbox->preval);

            if (val != tbox->preval)
                thread_yield();
            else
                break;
        } while (1);
    } while (--tbox->loop);
}

static void busyloop_xchg32(tbox_t *tbox)
{
    c_uint32_t val;

    do {
        busyloop_read32(tbox);
        val = atomic_xchg32(tbox->mem, tbox->postval);
        mutex_lock(thread_lock);
        ABTS_INT_EQUAL(tbox->tc, val, tbox->preval);
        mutex_unlock(thread_lock);
    } while (--tbox->loop);
}

static void *THREAD_FUNC thread_func_busyloop(thread_id id, void *data)
{
    tbox_t *tbox = data;

    tbox->func(tbox);

    thread_exit(id, 0);

    return NULL;
}

static void test_atomics_busyloop_threaded(abts_case *tc, void *data)
{
    unsigned int i;
    status_t rv;
    c_uint32_t count = 0;
    tbox_t tbox[NUM_THREADS];
    thread_id thread[NUM_THREADS];

    rv = mutex_create(&thread_lock, MUTEX_DEFAULT);
    ABTS_ASSERT(tc, "Could not create lock", rv == CORE_OK);

    /* get ready */
    for (i = 0; i < NUM_THREADS; i++) {
        tbox[i].tc = tc;
        tbox[i].mem = &count;
        tbox[i].loop = 50;
    }

    tbox[0].preval = 98;
    tbox[0].postval = 3891;
    tbox[0].func = busyloop_add32;

    tbox[1].preval = 3989;
    tbox[1].postval = 1010;
    tbox[1].func = busyloop_sub32;

    tbox[2].preval = 2979;
    tbox[2].postval = 0; /* not used */
    tbox[2].func = busyloop_inc32;

    tbox[3].preval = 2980;
    tbox[3].postval = 16384;
    tbox[3].func = busyloop_set32;

    tbox[4].preval = 16384;
    tbox[4].postval = 0; /* not used */
    tbox[4].func = busyloop_dec32;

    tbox[5].preval = 16383;
    tbox[5].postval = 1048576;
    tbox[5].func = busyloop_cas32;

    tbox[6].preval = 1048576;
    tbox[6].postval = 98; /* goto tbox[0] */
    tbox[6].func = busyloop_xchg32;

    /* get set */
    for (i = 0; i < NUM_THREADS; i++) {
        rv = thread_create(&thread[i], NULL, thread_func_busyloop,
                               &tbox[i]);
        ABTS_ASSERT(tc, "Failed creating thread", rv == CORE_OK);
    }

    /* go! */
    atomic_set32(tbox->mem, 98);

    for (i = 0; i < NUM_THREADS; i++) {
        status_t retval;
        rv = thread_join(&retval, thread[i]);
        ABTS_ASSERT(tc, "Thread join failed", rv == CORE_OK);
        ABTS_ASSERT(tc, "Invalid return value from thread_join", retval == 0);
    }

    ABTS_INT_EQUAL(tbox->tc, 98, count);

    rv = mutex_delete(thread_lock);
    ABTS_ASSERT(tc, "Failed creating threads", rv == CORE_OK);
}

abts_suite *testatomic(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_set32, NULL);
    abts_run_test(suite, test_read32, NULL);
    abts_run_test(suite, test_dec32, NULL);
    abts_run_test(suite, test_xchg32, NULL);
    abts_run_test(suite, test_xchgptr, NULL);
    abts_run_test(suite, test_cas_equal, NULL);
    abts_run_test(suite, test_cas_equal_nonnull, NULL);
    abts_run_test(suite, test_cas_notequal, NULL);
    abts_run_test(suite, test_casptr_equal, NULL);
    abts_run_test(suite, test_casptr_equal_nonnull, NULL);
    abts_run_test(suite, test_casptr_notequal, NULL);
    abts_run_test(suite, test_add32, NULL);
    abts_run_test(suite, test_add32_neg, NULL);
    abts_run_test(suite, test_inc32, NULL);
    abts_run_test(suite, test_set_add_inc_sub, NULL);
    abts_run_test(suite, test_wrap_zero, NULL);
    abts_run_test(suite, test_inc_neg1, NULL);

    abts_run_test(suite, test_atomics_threaded, NULL);
    abts_run_test(suite, test_atomics_busyloop_threaded, NULL);

    return suite;
}
