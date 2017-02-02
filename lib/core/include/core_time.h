#ifndef __CORE_TIME_H__
#define __CORE_TIME_H__

/**
 * @file core_time.h
 * @brief CORE Time Library
 */

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup core_time Time Routines
 * @ingroup CORE
 * @{
 */

/** month names */
CORE_DECLARE_DATA extern const char month_snames[12][4];
/** day names */
CORE_DECLARE_DATA extern const char day_snames[7][4];

/** number of microseconds since 00:00:00 january 1, 1970 UTC */
typedef c_int64_t c_time_t;

/** mechanism to properly type c_time_t literals */
#define TIME_C(val) INT64_C(val)

/** mechanism to properly print c_time_t values */
#define TIME_T_FMT INT64_T_FMT

/** number of microseconds per second */
#define USEC_PER_SEC TIME_C(1000000)

/** @return c_time_t as a second */
#define time_sec(time) ((time) / USEC_PER_SEC)

/** @return c_time_t as a usec */
#define time_usec(time) ((time) % USEC_PER_SEC)

/** @return c_time_t as a msec */
#define time_msec(time) (((time) / 1000) % 1000)

/** @return c_time_t as a msec */
#define time_as_msec(time) ((time) / 1000)

/** @return milliseconds as an c_time_t */
#define time_from_msec(msec) ((c_time_t)(msec) * 1000)

/** @return seconds as an c_time_t */
#define time_from_sec(sec) ((c_time_t)(sec) * USEC_PER_SEC)

/** @return a second and usec combination as an c_time_t */
#define time_make(sec, usec) ((c_time_t)(sec) * USEC_PER_SEC \
                                + (c_time_t)(usec))
/** @see time_exp_t */
typedef struct time_exp_t time_exp_t;

/**
 * a structure similar to ANSI struct tm with the following differences:
 *  - tm_usec isn't an ANSI field
 *  - tm_gmtoff isn't an ANSI field (it's a bsdism)
 */
struct time_exp_t {
    /** microseconds past tm_sec */
    int32_t tm_usec;
    /** (0-61) seconds past tm_min */
    int32_t tm_sec;
    /** (0-59) minutes past tm_hour */
    int32_t tm_min;
    /** (0-23) hours past midnight */
    int32_t tm_hour;
    /** (1-31) day of the month */
    int32_t tm_mday;
    /** (0-11) month of the year */
    int32_t tm_mon;
    /** year since 1900 */
    int32_t tm_year;
    /** (0-6) days since sunday */
    int32_t tm_wday;
    /** (0-365) days since jan 1 */
    int32_t tm_yday;
    /** daylight saving time */
    int32_t tm_isdst;
    /** seconds east of UTC */
    int32_t tm_gmtoff;
};

/**
 * @return the current time
 */
CORE_DECLARE(c_time_t) time_now(void);

/**
 * Sleep for the specified number of micro-seconds.
 * @param t desired amount of time to sleep.
 * @warning May sleep for longer than the specified time.
 */
CORE_DECLARE(void) core_sleep(c_time_t t);

/**
 * convert an ansi time_t to an c_time_t
 * @param result the resulting c_time_t
 * @param input the time_t to convert
 */
CORE_DECLARE(status_t) time_ansi_put(c_time_t *result, time_t input);

/**
 * convert a time to its human readable components using an offset
 * from GMT
 * @param result the exploded time
 * @param input the time to explode
 * @param offs the number of seconds offset to apply
 */
CORE_DECLARE(status_t) time_exp_tz(time_exp_t *result,
        c_time_t input, int32_t offs);

/**
 * convert a time to its human readable components in GMT timezone
 * @param result the exploded time
 * @param input the time to explode
 */
CORE_DECLARE(status_t) time_exp_gmt(time_exp_t *result, c_time_t input);

/**
 * convert a time to its human readable components in local timezone
 * @param result the exploded time
 * @param input the time to explode
 */
CORE_DECLARE(status_t) time_exp_lt(time_exp_t *result, c_time_t input);

/**
 * Convert time value from human readable format to a numeric c_time_t
 * e.g. elapsed usec since epoch
 * @param result the resulting imploded time
 * @param input the input exploded time
 */
CORE_DECLARE(status_t) time_exp_get(c_time_t *result, time_exp_t *input);

/**
 * Convert time value from human readable format to a numeric c_time_t that
 * always represents GMT
 * @param result the resulting imploded time
 * @param input the input exploded time
 */
CORE_DECLARE(status_t) time_exp_gmt_get(c_time_t *result, time_exp_t *input);

/** length of a RFC822 Date */
#define APR_RFC822_DATE_LEN (30)
/**
 * rfc822_date formats dates in the RFC822
 * format in an efficient manner.  It is a fixed length
 * format which requires the indicated amount of storage,
 * including the trailing NUL terminator.
 * @param date_str String to write to.
 * @param t the time to convert
 */
CORE_DECLARE(status_t) rfc822_date(char *date_str, c_time_t t);

/** length of a CTIME date */
#define CTIME_LEN (25)
/**
 * ctime formats dates in the ctime() format
 * in an efficient manner.  it is a fixed length format
 * and requires the indicated amount of storage including
 * the trailing NUL terminator.
 * Unlike ANSI/ISO C ctime(), ctime() does not include
 * a \n at the end of the string.
 * @param date_str String to write to.
 * @param t the time to convert
 */
CORE_DECLARE(status_t) core_ctime(char *date_str, c_time_t t);

/**
 * formats the exploded time according to the format specified
 * @param s string to write to
 * @param retsize The length of the returned string
 * @param max The maximum length of the string
 * @param format The format for the time string
 * @param tm The time to convert
 */
CORE_DECLARE(status_t) core_strftime(char *s, size_t *retsize,
        size_t max, const char *format,
        time_exp_t *tm);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* ! __CORE_TIME_H__ */
