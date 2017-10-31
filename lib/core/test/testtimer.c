#include "core_time.h"
#include "core_timer.h"
#include "core_param.h"
#include "testutil.h"

#define TEST_TIMER_NUM          200
#define TEST_TIMER_PRECISION    20 /* 2ms precision */
#define TEST_DURATION           400

c_uint8_t expire_check[TEST_DURATION/TEST_TIMER_PRECISION];

typedef struct _test_timer_eliment
{
    c_uint8_t type;
    c_uint32_t duration;
} test_timer_eliment;


test_timer_eliment timer_eliment[] ={
    {TIMER_TYPE_ONE_SHOT, 500},
    {TIMER_TYPE_ONE_SHOT, 50},
    {TIMER_TYPE_ONE_SHOT, 200},
    {TIMER_TYPE_ONE_SHOT, 90},
    {TIMER_TYPE_ONE_SHOT, 800}
};


void test_expire_func_1(c_uintptr_t data,
        c_uintptr_t param1, c_uintptr_t param2, c_uintptr_t param3,
        c_uintptr_t param4, c_uintptr_t param5, c_uintptr_t param6)
{
    c_uint32_t index = param2;

    expire_check[index] = TRUE;
}

void test_expire_func_2(c_uintptr_t data,
        c_uintptr_t param1, c_uintptr_t param2, c_uintptr_t param3,
        c_uintptr_t param4, c_uintptr_t param5, c_uintptr_t param6)
{
    c_uint32_t index = param2;

    expire_check[index]++;
}

static void test_now(abts_case *tc, void *data)
{
    c_time_t timediff;
    c_time_t current;
    time_t os_now;

    current = time_now();
    time(&os_now);

    timediff = os_now - (current / USEC_PER_SEC);
    /* Even though these are called so close together, there is the chance
    * that the time will be slightly off, so accept anything between -1 and
    * 1 second.
    */
    ABTS_ASSERT(tc, "core_time and OS time do not agree",
    (timediff > -2) && (timediff < 2));
}


/* basic timer Test */
static void timer_test_1(abts_case *tc, void *data)
{

    int n = 0;
    tm_block_id id_array[100];
    tm_block_id id;
    tm_service_t tm_service;

    memset((char*)id_array, 0x00, sizeof(tm_service));
    memset(expire_check, 0x00, sizeof(expire_check));

    /* init tm_service */
    tm_service_init(&tm_service);

    for(n = 0; n < sizeof(timer_eliment) / sizeof(test_timer_eliment); n++)
    {
        id_array[n] = tm_create(&tm_service, TIMER_TYPE_ONE_SHOT,
                timer_eliment[n].duration, test_expire_func_1);

        tm_set_param1(id_array[n], (c_uintptr_t)id_array[n]);
        tm_set_param2(id_array[n], n);
    }


    for(n = 0; n < sizeof(timer_eliment) / sizeof(test_timer_eliment); n++)
    {
        tm_start(id_array[n]);
    }

    id = (tm_block_id)list_first(&tm_service.idle_list);
    ABTS_INT_EQUAL(tc, id, 0);

    id = (tm_block_id)list_first(&tm_service.active_list);
    ABTS_INT_NEQUAL(tc, id, 0);
    ABTS_INT_EQUAL(tc, id, id_array[1]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[3]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[2]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[0]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[4]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, 0);

    core_sleep(70000);
    tm_execute_tm_service(&tm_service, 0);

    id = (tm_block_id)list_first(&tm_service.idle_list);
    ABTS_INT_EQUAL(tc, id, id_array[1]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, 0);

    id = (tm_block_id)list_first(&tm_service.active_list);
    ABTS_INT_NEQUAL(tc, id, 0);
    ABTS_INT_EQUAL(tc, id, id_array[3]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[2]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[0]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[4]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, 0);

    ABTS_INT_EQUAL(tc, expire_check[0], 0);
    ABTS_INT_EQUAL(tc, expire_check[1], 1);
    ABTS_INT_EQUAL(tc, expire_check[2], 0);
    ABTS_INT_EQUAL(tc, expire_check[3], 0);
    ABTS_INT_EQUAL(tc, expire_check[4], 0);


    core_sleep(40000);
    tm_execute_tm_service(&tm_service, 0);

    id = (tm_block_id)list_first(&tm_service.idle_list);
    ABTS_INT_EQUAL(tc, id, id_array[1]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[3]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, 0);

    id = (tm_block_id)list_first(&tm_service.active_list);
    ABTS_INT_NEQUAL(tc, id, 0);
    ABTS_INT_EQUAL(tc, id, id_array[2]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[0]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[4]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, 0);

    ABTS_INT_EQUAL(tc, expire_check[0], 0);
    ABTS_INT_EQUAL(tc, expire_check[1], 1);
    ABTS_INT_EQUAL(tc, expire_check[2], 0);
    ABTS_INT_EQUAL(tc, expire_check[3], 1);
    ABTS_INT_EQUAL(tc, expire_check[4], 0);


    core_sleep(140000);
    tm_execute_tm_service(&tm_service, 0);

    id = (tm_block_id)list_first(&tm_service.idle_list);
    ABTS_INT_EQUAL(tc, id, id_array[1]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[3]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[2]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, 0);

    id = (tm_block_id)list_first(&tm_service.active_list);
    ABTS_INT_NEQUAL(tc, id, 0);
    ABTS_INT_EQUAL(tc, id, id_array[0]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[4]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, 0);

    ABTS_INT_EQUAL(tc, expire_check[0], 0);
    ABTS_INT_EQUAL(tc, expire_check[1], 1);
    ABTS_INT_EQUAL(tc, expire_check[2], 1);
    ABTS_INT_EQUAL(tc, expire_check[3], 1);
    ABTS_INT_EQUAL(tc, expire_check[4], 0);


    core_sleep(300000);
    tm_execute_tm_service(&tm_service, 0);

    id = (tm_block_id)list_first(&tm_service.idle_list);
    ABTS_INT_EQUAL(tc, id, id_array[1]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[3]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[2]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[0]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, 0);

    id = (tm_block_id)list_first(&tm_service.active_list);
    ABTS_INT_NEQUAL(tc, id, 0);
    ABTS_INT_EQUAL(tc, id, id_array[4]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, 0);

    ABTS_INT_EQUAL(tc, expire_check[0], 1);
    ABTS_INT_EQUAL(tc, expire_check[1], 1);
    ABTS_INT_EQUAL(tc, expire_check[2], 1);
    ABTS_INT_EQUAL(tc, expire_check[3], 1);
    ABTS_INT_EQUAL(tc, expire_check[4], 0);


    core_sleep(300000);
    tm_execute_tm_service(&tm_service, 0);

    id = (tm_block_id)list_first(&tm_service.idle_list);
    ABTS_INT_EQUAL(tc, id, id_array[1]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[3]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[2]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[0]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, id_array[4]); if(tc->failed) return;
    id = (tm_block_id)list_next(id);
    ABTS_INT_EQUAL(tc, id, 0);

    id = (tm_block_id)list_first(&tm_service.active_list);
    ABTS_INT_EQUAL(tc, id, 0);

    ABTS_INT_EQUAL(tc, expire_check[0], 1);
    ABTS_INT_EQUAL(tc, expire_check[1], 1);
    ABTS_INT_EQUAL(tc, expire_check[2], 1);
    ABTS_INT_EQUAL(tc, expire_check[3], 1);
    ABTS_INT_EQUAL(tc, expire_check[4], 1);

    for( n = 0; n < sizeof(timer_eliment) / sizeof(test_timer_eliment); n++)
    {
        tm_delete(id_array[n]);
    }

    ABTS_INT_EQUAL(tc, tm_pool_avail(), MAX_NUM_OF_TIMER);

    return;
}


static void timer_test_2(abts_case *tc, void *data)
{

    int n = 0;
    tm_block_id id_array[TEST_TIMER_NUM];
    tm_service_t tm_service;
    int duration;
    int tm_num[TEST_DURATION/TEST_TIMER_PRECISION];
    int tm_idx;

    memset((char*)id_array, 0x00, sizeof(tm_service));
    memset(expire_check, 0x00, sizeof(expire_check));
    memset(tm_num, 0x00, sizeof(tm_num));

    /* init tm_service */
    tm_service_init(&tm_service);

    for(n = 0; n < TEST_TIMER_NUM; n++)
    {
        duration = (rand() % (TEST_DURATION/TEST_TIMER_PRECISION))
            * TEST_TIMER_PRECISION;

        tm_idx = duration/TEST_TIMER_PRECISION;
        tm_num[tm_idx]++;
        duration += (TEST_TIMER_PRECISION >> 1);

        id_array[n] = tm_create(&tm_service,
                TIMER_TYPE_ONE_SHOT, duration, test_expire_func_2);
        tm_set_param1(id_array[n], (c_uintptr_t)id_array[n]);
        tm_set_param2(id_array[n], tm_idx);
    }

    for(n = 0; n < TEST_TIMER_NUM; n++)
    {
        tm_start(id_array[n]);
    }

    for(n = 0; n < TEST_DURATION/TEST_TIMER_PRECISION; n++)
    {
        core_sleep(TEST_TIMER_PRECISION * 1000);
        tm_execute_tm_service(&tm_service, 0);
        ABTS_INT_EQUAL(tc, tm_num[n], expire_check[n]);
    }

    for(n = 0; n < TEST_TIMER_NUM; n++)
    {
        tm_delete(id_array[n]);
    }

    ABTS_INT_EQUAL(tc, tm_pool_avail(), MAX_NUM_OF_TIMER);

    return;
}


static void timer_test_3(abts_case *tc, void *data)
{
    c_uint32_t n = 0;
    tm_block_id id_array[TEST_TIMER_NUM];
    tm_service_t tm_service;
    int id_duration[TEST_TIMER_NUM];
    int duration;
    int tm_num[TEST_DURATION/TEST_TIMER_PRECISION];
    int tm_idx, tm_check_id;

    memset((char*)id_array, 0x00, sizeof(tm_service));
    memset(expire_check, 0x00, sizeof(expire_check));
    memset(tm_num, 0x00, sizeof(tm_num));

    /* init tm_service */
    tm_service_init(&tm_service);

    for(n = 0; n < TEST_TIMER_NUM; n++)
    {
        duration = (rand() % (TEST_DURATION/TEST_TIMER_PRECISION))
            * TEST_TIMER_PRECISION;
        tm_idx = duration/TEST_TIMER_PRECISION;
        tm_num[tm_idx]++;
        id_duration[n] = duration;
        duration += (TEST_TIMER_PRECISION >> 1);

        id_array[n] = tm_create(&tm_service,
                TIMER_TYPE_ONE_SHOT, duration, test_expire_func_2);
        tm_set_param1(id_array[n], (c_uintptr_t)id_array[n]);
        tm_set_param2(id_array[n], tm_idx);
    }

    for(n = 0; n < TEST_TIMER_NUM; n++)
    {
        tm_start(id_array[n]);
    }

    for(n = 0; n < TEST_TIMER_NUM / 10; n++)
    {
        tm_idx = n*10 + rand()%10;
        tm_check_id = id_duration[tm_idx]/TEST_TIMER_PRECISION;
        tm_num[tm_check_id]--;
        tm_delete(id_array[tm_idx]);
        id_array[tm_idx] = 0;
    }

    for(n = 0; n < TEST_DURATION/TEST_TIMER_PRECISION; n++)
    {
        core_sleep(TEST_TIMER_PRECISION * 1000);
        tm_execute_tm_service(&tm_service, 0);
        ABTS_INT_EQUAL(tc, tm_num[n], expire_check[n]);
    }

    for(n = 0; n < TEST_TIMER_NUM; n++)
    {
        if(id_array[n] != 0)
        {
            tm_delete(id_array[n]);
        }
    }

    ABTS_INT_EQUAL(tc, tm_pool_avail(), MAX_NUM_OF_TIMER);

    return;
}

abts_suite *testtimer(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

   /*
     * OpenSUSE OBS
     * - Ubuntu 16.10 i586 failed
     * - It is probably VM issue
     * [  661s] testtimer           :  Line 176: expected <1449351760>, but saw <0>
     * [  661s] Line 305: expected <1019>, but saw <1024>
     * [  661s] Line 372: expected <1019>, but saw <1024>
     * [  661s] FAILED 3 of 4
     */
    abts_run_test(suite, test_now, NULL);
    abts_run_test(suite, timer_test_1, NULL);
    abts_run_test(suite, timer_test_2, NULL);
    abts_run_test(suite, timer_test_3, NULL);

    return suite;
}

