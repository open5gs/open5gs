#include "core.h"
#include "core_time.h"

static c_int32_t get_offset(struct tm *tm)
{
#if defined(HAVE_STRUCT_TM_TM_GMTOFF)
    return tm->tm_gmtoff;
#elif defined(HAVE_STRUCT_TM___TM_GMTOFF)
    return tm->__tm_gmtoff;
#else
#error cannot support tm->tm_gmtoff
#endif
}

status_t time_ansi_put(c_time_t *result, time_t input)
{
    *result = (c_time_t)input * USEC_PER_SEC;
    return CORE_OK;
}

/* NB NB NB NB This returns GMT!!!!!!!!!! */
c_time_t time_now(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * USEC_PER_SEC + tv.tv_usec;
}

void core_sleep(c_time_t t)
{
#ifdef OS2
    DosSleep(t/1000);
#elif defined(BEOS)
    snooze(t);
#elif defined(NETWARE)
    delay(t/1000);
#else
    struct timeval tv;
    tv.tv_usec = t % USEC_PER_SEC;
    tv.tv_sec = t / USEC_PER_SEC;
    select(0, NULL, NULL, NULL, &tv);
#endif
}

static void explode_time(time_exp_t *xt, c_time_t t,
                         c_int32_t offset, int use_localtime)
{
    struct tm tm;
    time_t tt = (t / USEC_PER_SEC) + offset;
    xt->tm_usec = t % USEC_PER_SEC;

#if defined (_POSIX_THREAD_SAFE_FUNCTIONS)
    if (use_localtime)
        localtime_r(&tt, &tm);
    else
        gmtime_r(&tt, &tm);
#else
    if (use_localtime)
        tm = *localtime(&tt);
    else
        tm = *gmtime(&tt);
#endif

    xt->tm_sec  = tm.tm_sec;
    xt->tm_min  = tm.tm_min;
    xt->tm_hour = tm.tm_hour;
    xt->tm_mday = tm.tm_mday;
    xt->tm_mon  = tm.tm_mon;
    xt->tm_year = tm.tm_year;
    xt->tm_wday = tm.tm_wday;
    xt->tm_yday = tm.tm_yday;
    xt->tm_isdst = tm.tm_isdst;
    xt->tm_gmtoff = get_offset(&tm);
}

status_t time_exp_tz(time_exp_t *result,
        c_time_t input, c_int32_t offs)
{
    explode_time(result, input, offs, 0);
    result->tm_gmtoff = offs;
    return CORE_OK;
}

status_t time_exp_gmt(time_exp_t *result, c_time_t input)
{
    return time_exp_tz(result, input, 0);
}

status_t time_exp_lt(time_exp_t *result, c_time_t input)
{
#if defined(__EMX__)
    /* EMX gcc (OS/2) has a timezone global we can use */
    return time_exp_tz(result, input, -timezone);
#else
    explode_time(result, input, 0, 1);
    return CORE_OK;
#endif /* __EMX__ */
}

status_t time_exp_get(c_time_t *t, time_exp_t *xt)
{
    time_t year = xt->tm_year;
    time_t days;
    static const int dayoffset[12] =
    {306, 337, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275};

    /* shift new year to 1st March in order to make leap year calc easy */

    if (xt->tm_mon < 2)
        year--;

    /* Find number of days since 1st March 1900 (in the Gregorian calendar). */

    days = year * 365 + year / 4 - year / 100 + (year / 100 + 3) / 4;
    days += dayoffset[xt->tm_mon] + xt->tm_mday - 1;
    days -= 25508;              /* 1 jan 1970 is 25508 days since 1 mar 1900 */
    days = ((days * 24 + xt->tm_hour) * 60 + xt->tm_min) * 60 + xt->tm_sec;

    if (days < 0) {
        return CORE_EBADDATE;
    }
    *t = days * USEC_PER_SEC + xt->tm_usec;
    return CORE_OK;
}

status_t time_exp_gmt_get(c_time_t *t, time_exp_t *xt)
{
    status_t status = time_exp_get(t, xt);
    if (status == CORE_OK)
        *t -= (time_t) xt->tm_gmtoff * USEC_PER_SEC;
    return status;
}

const char month_snames[12][4] =
{
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
const char day_snames[7][4] =
{
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

status_t rfc822_date(char *date_str, c_time_t t)
{
    time_exp_t xt;
    const char *s;
    int real_year;

    time_exp_gmt(&xt, t);

    /* example: "Sat, 08 Jan 2000 18:31:41 GMT" */
    /*           12345678901234567890123456789  */

    s = &day_snames[xt.tm_wday][0];
    *date_str++ = *s++;
    *date_str++ = *s++;
    *date_str++ = *s++;
    *date_str++ = ',';
    *date_str++ = ' ';
    *date_str++ = xt.tm_mday / 10 + '0';
    *date_str++ = xt.tm_mday % 10 + '0';
    *date_str++ = ' ';
    s = &month_snames[xt.tm_mon][0];
    *date_str++ = *s++;
    *date_str++ = *s++;
    *date_str++ = *s++;
    *date_str++ = ' ';
    real_year = 1900 + xt.tm_year;
    /* This routine isn't y10k ready. */
    *date_str++ = real_year / 1000 + '0';
    *date_str++ = real_year % 1000 / 100 + '0';
    *date_str++ = real_year % 100 / 10 + '0';
    *date_str++ = real_year % 10 + '0';
    *date_str++ = ' ';
    *date_str++ = xt.tm_hour / 10 + '0';
    *date_str++ = xt.tm_hour % 10 + '0';
    *date_str++ = ':';
    *date_str++ = xt.tm_min / 10 + '0';
    *date_str++ = xt.tm_min % 10 + '0';
    *date_str++ = ':';
    *date_str++ = xt.tm_sec / 10 + '0';
    *date_str++ = xt.tm_sec % 10 + '0';
    *date_str++ = ' ';
    *date_str++ = 'G';
    *date_str++ = 'M';
    *date_str++ = 'T';
    *date_str++ = 0;
    return CORE_OK;
}

status_t core_ctime(char *date_str, c_time_t t)
{
    time_exp_t xt;
    const char *s;
    int real_year;

    /* example: "Wed Jun 30 21:49:08 1993" */
    /*           123456789012345678901234  */

    time_exp_lt(&xt, t);
    s = &day_snames[xt.tm_wday][0];
    *date_str++ = *s++;
    *date_str++ = *s++;
    *date_str++ = *s++;
    *date_str++ = ' ';
    s = &month_snames[xt.tm_mon][0];
    *date_str++ = *s++;
    *date_str++ = *s++;
    *date_str++ = *s++;
    *date_str++ = ' ';
    *date_str++ = xt.tm_mday / 10 + '0';
    *date_str++ = xt.tm_mday % 10 + '0';
    *date_str++ = ' ';
    *date_str++ = xt.tm_hour / 10 + '0';
    *date_str++ = xt.tm_hour % 10 + '0';
    *date_str++ = ':';
    *date_str++ = xt.tm_min / 10 + '0';
    *date_str++ = xt.tm_min % 10 + '0';
    *date_str++ = ':';
    *date_str++ = xt.tm_sec / 10 + '0';
    *date_str++ = xt.tm_sec % 10 + '0';
    *date_str++ = ' ';
    real_year = 1900 + xt.tm_year;
    *date_str++ = real_year / 1000 + '0';
    *date_str++ = real_year % 1000 / 100 + '0';
    *date_str++ = real_year % 100 / 10 + '0';
    *date_str++ = real_year % 10 + '0';
    *date_str++ = 0;

    return CORE_OK;
}

status_t core_strftime(char *s, size_t *retsize, size_t max,
                        const char *format, time_exp_t *xt)
{
    struct tm tm;
    memset(&tm, 0, sizeof tm);
    tm.tm_sec  = xt->tm_sec;
    tm.tm_min  = xt->tm_min;
    tm.tm_hour = xt->tm_hour;
    tm.tm_mday = xt->tm_mday;
    tm.tm_mon  = xt->tm_mon;
    tm.tm_year = xt->tm_year;
    tm.tm_wday = xt->tm_wday;
    tm.tm_yday = xt->tm_yday;
    tm.tm_isdst = xt->tm_isdst;
#if defined(HAVE_STRUCT_TM_TM_GMTOFF)
    tm.tm_gmtoff = xt->tm_gmtoff;
#elif defined(HAVE_STRUCT_TM___TM_GMTOFF)
    tm.__tm_gmtoff = xt->tm_gmtoff;
#endif
    (*retsize) = strftime(s, max, format, &tm);
    return CORE_OK;
}
