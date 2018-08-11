#include "core_time.h"
#include "testutil.h"

#define STR_SIZE 100

/* The time value is used throughout the tests, so just make this a global.
 * Also, we need a single value that we can test for the positive tests, so
 * I chose the number below, it corresponds to:
 *           2002-08-14 12:05:36.186711 -25200 [257 Sat].
 * Which happens to be when I wrote the new tests.
 */
static c_time_t now = C_INT64_C(1032030336186711);

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

static void test_gmtstr(abts_case *tc, void *data)
{
    status_t rv;
    time_exp_t xt;
    char str[STR_SIZE+1];

    rv = time_exp_gmt(&xt, now);
    if (rv == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "time_exp_gmt");
    }
    ABTS_TRUE(tc, rv == CORE_OK);
    sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d.%06d %+05d [%d %s]%s",
                 xt.tm_year + 1900,
                 xt.tm_mon,
                 xt.tm_mday,
                 xt.tm_hour,
                 xt.tm_min,
                 xt.tm_sec,
                 xt.tm_usec,
                 xt.tm_gmtoff,
                 xt.tm_yday + 1,
                 day_snames[xt.tm_wday],
                 (xt.tm_isdst ? " DST" : ""));
    ABTS_STR_EQUAL(tc, "2002-08-14 19:05:36.186711 +0000 [257 Sat]", str);
}

static void test_exp_lt(abts_case *tc, void *data)
{
    status_t rv;
    time_exp_t xt;
    time_t posix_secs = (time_t)time_sec(now);
    struct tm *posix_exp = localtime(&posix_secs);

    rv = time_exp_lt(&xt, now);
    if (rv == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "time_exp_lt");
    }
    ABTS_TRUE(tc, rv == CORE_OK);

#define CHK_FIELD(f) \
    ABTS_ASSERT(tc, "Mismatch in " #f, posix_exp->f == xt.f)

    CHK_FIELD(tm_sec);
    CHK_FIELD(tm_min);
    CHK_FIELD(tm_hour);
    CHK_FIELD(tm_mday);
    CHK_FIELD(tm_mon);
    CHK_FIELD(tm_year);
    CHK_FIELD(tm_wday);
    CHK_FIELD(tm_yday);
    CHK_FIELD(tm_isdst);
#undef CHK_FIELD
}

static void test_exp_get_gmt(abts_case *tc, void *data)
{
    status_t rv;
    time_exp_t xt;
    c_time_t imp;
    c_int64_t hr_off_64;

    rv = time_exp_gmt(&xt, now);
    ABTS_TRUE(tc, rv == CORE_OK);
    rv = time_exp_get(&imp, &xt);
    if (rv == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "time_exp_get");
    }
    ABTS_TRUE(tc, rv == CORE_OK);
    hr_off_64 = (c_int64_t) xt.tm_gmtoff * USEC_PER_SEC;
    ABTS_TRUE(tc, now + hr_off_64 == imp);
}

static void test_exp_get_lt(abts_case *tc, void *data)
{
    status_t rv;
    time_exp_t xt;
    c_time_t imp;
    c_int64_t hr_off_64;

    rv = time_exp_lt(&xt, now);
    ABTS_TRUE(tc, rv == CORE_OK);
    rv = time_exp_get(&imp, &xt);
    if (rv == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "time_exp_get");
    }
    ABTS_TRUE(tc, rv == CORE_OK);
    hr_off_64 = (c_int64_t) xt.tm_gmtoff * USEC_PER_SEC;
    ABTS_TRUE(tc, now + hr_off_64 == imp);
}

static void test_imp_gmt(abts_case *tc, void *data)
{
    status_t rv;
    time_exp_t xt;
    c_time_t imp;

    rv = time_exp_gmt(&xt, now);
    ABTS_TRUE(tc, rv == CORE_OK);
    rv = time_exp_gmt_get(&imp, &xt);
    if (rv == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "time_exp_gmt_get");
    }
    ABTS_TRUE(tc, rv == CORE_OK);
    ABTS_TRUE(tc, now == imp);
}

static void test_rfcstr(abts_case *tc, void *data)
{
    status_t rv;
    char str[STR_SIZE];

    rv = rfc822_date(str, now);
    if (rv == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "rfc822_date");
    }
    ABTS_TRUE(tc, rv == CORE_OK);
    ABTS_STR_EQUAL(tc, "Sat, 14 Sep 2002 19:05:36 GMT", str);
}

static void test_ctime(abts_case *tc, void *data)
{
    status_t rv;
    char core_str[STR_SIZE];
    char libc_str[STR_SIZE];
    c_time_t now_sec = time_sec(now);
    time_t posix_sec = (time_t) now_sec;

    rv = core_ctime(core_str, now);
    if (rv == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "core_ctime");
    }
    ABTS_TRUE(tc, rv == CORE_OK);
    strcpy(libc_str, ctime(&posix_sec));
    *strchr(libc_str, '\n') = '\0';

    ABTS_STR_EQUAL(tc, libc_str, core_str);
}

static void test_strftime(abts_case *tc, void *data)
{
    status_t rv;
    time_exp_t xt;
    char str[STR_SIZE+1];
    size_t sz;

    rv = time_exp_gmt(&xt, now);
    rv = core_strftime(str, &sz, STR_SIZE, "%R %A %d %B %Y", &xt);
    if (rv == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "core_strftime");
    }
    ABTS_TRUE(tc, rv == CORE_OK);
    ABTS_STR_EQUAL(tc, "19:05 Saturday 14 September 2002", str);
}

static void test_strftimesmall(abts_case *tc, void *data)
{
    status_t rv;
    time_exp_t xt;
    char str[STR_SIZE];
    size_t sz;

    rv = time_exp_gmt(&xt, now);
    rv = core_strftime(str, &sz, STR_SIZE, "%T", &xt);
    if (rv == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "core_strftime");
    }
    ABTS_TRUE(tc, rv == CORE_OK);
    ABTS_STR_EQUAL(tc, "19:05:36", str);
}

static void test_exp_tz(abts_case *tc, void *data)
{
    status_t rv;
    time_exp_t xt;
    c_int32_t hr_off = -5 * 3600; /* 5 hours in seconds */

    rv = time_exp_tz(&xt, now, hr_off);
    if (rv == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "time_exp_tz");
    }
    ABTS_TRUE(tc, rv == CORE_OK);
    ABTS_TRUE(tc, (xt.tm_usec == 186711) &&
                     (xt.tm_sec == 36) &&
                     (xt.tm_min == 5) &&
                     (xt.tm_hour == 14) &&
                     (xt.tm_mday == 14) &&
                     (xt.tm_mon == 8) &&
                     (xt.tm_year == 102) &&
                     (xt.tm_wday == 6) &&
                     (xt.tm_yday == 256));
}

static void test_strftimeoffset(abts_case *tc, void *data)
{
    status_t rv;
    time_exp_t xt;
    char str[STR_SIZE];
    size_t sz;
    c_int32_t hr_off = -5 * 3600; /* 5 hours in seconds */

    time_exp_tz(&xt, now, hr_off);
    rv = core_strftime(str, &sz, STR_SIZE, "%T", &xt);
    if (rv == CORE_ENOTIMPL)
    {
        ABTS_NOT_IMPL(tc, "core_strftime");
    }
    ABTS_TRUE(tc, rv == CORE_OK);
}

/* 0.9.4 and earlier rejected valid dates in 2038 */
static void test_2038(abts_case *tc, void *data)
{
    time_exp_t xt;
    c_time_t t;

    /* 2038-01-19T03:14:07.000000Z */
    xt.tm_year = 138;
    xt.tm_mon = 0;
    xt.tm_mday = 19;
    xt.tm_hour = 3;
    xt.tm_min = 14;
    xt.tm_sec = 7;

    CORE_ASSERT_OK(tc, "explode January 19th, 2038",
                       time_exp_get(&t, &xt));
}

abts_suite *testtime(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_now, NULL);
    abts_run_test(suite, test_gmtstr, NULL);
    abts_run_test(suite, test_exp_lt, NULL);
    abts_run_test(suite, test_exp_get_gmt, NULL);
    abts_run_test(suite, test_exp_get_lt, NULL);
    abts_run_test(suite, test_imp_gmt, NULL);
    abts_run_test(suite, test_rfcstr, NULL);
    abts_run_test(suite, test_ctime, NULL);
    abts_run_test(suite, test_strftime, NULL);
    abts_run_test(suite, test_strftimesmall, NULL);
    abts_run_test(suite, test_exp_tz, NULL);
    abts_run_test(suite, test_strftimeoffset, NULL);
    abts_run_test(suite, test_2038, NULL);

    return suite;
}

