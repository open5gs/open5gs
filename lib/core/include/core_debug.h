#ifndef __CORE_DEBUG_H__
#define __CORE_DEBUG_H__

#include "core.h"
#include "core_time.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef TRACE_MODULE
int TRACE_MODULE;
#endif

extern int g_trace_mask;

#define trace_level_set(__level) (TRACE_MODULE = __level);

#define D_MSG_TYPE_NONE     0
#define D_MSG_TYPE_RAW      1
#define D_MSG_TYPE_TRACE    2
#define D_MSG_TYPE_LOG      3
#define D_MSG_TYPE_ASSERT   4

#define D_MSG_TO_CONSOLE    0x00000001
#define D_MSG_TO_STDOUT     0x00000002
#define D_MSG_TO_SYSLOG     0x00000004
#define D_MSG_TO_NETWORK     0x00000008
#define D_MSG_TO_FILE       0x00000010
#define D_MSG_TO_ALL        (D_MSG_TO_CONSOLE | D_MSG_TO_STDOUT | \
                             D_MSG_TO_SYSLOG | D_MSG_TO_NETWORK | \
                             D_MSG_TO_FILE )

#define D_LOG_LEVEL_NONE    0
#define D_LOG_LEVEL_FATAL   1
#define D_LOG_LEVEL_ERROR   2
#define D_LOG_LEVEL_WARN    3
#define D_LOG_LEVEL_INFO    4
#define D_LOG_LEVEL_FULL    D_LOG_LEVEL_INFO

CORE_DECLARE(int) d_msg(int tp, int lv, c_time_t t, char *fn, int ln,
        char *fmt, ...);

/**
 * Use for printing message. This always print out the message.
 */
#define d_print(fmt, ...) \
    d_msg(D_MSG_TYPE_RAW, 0, 0, NULL, 0, fmt, ## __VA_ARGS__)

#define __MAX_HEX_BUF 128

/**
 * Use for printing binary buffer as printable hex string. This always
 * print out the message.
 */
#define d_print_hex(__buf, __buflen) do { \
    int __i = 0, __l, __off = 0; \
    char __hex[__MAX_HEX_BUF*2+__MAX_HEX_BUF/4+__MAX_HEX_BUF/32+4], *__p; \
    d_print("%d bytes hex:\r\n", __buflen); \
    while (__off < (__buflen)) { \
        __p = __hex; __p[0] = 0; \
        __l = ((__buflen) - __off) > __MAX_HEX_BUF ? \
            __MAX_HEX_BUF : (__buflen) - __off; \
        for (__i = 0; __i < __l; __i++) { \
            __p += sprintf(__p, "%02x", ((char*)(__buf))[__off+__i] & 0xff); \
            if ((__i & 0x1f) == 31) \
                __p += sprintf(__p, "\r\n"); \
            else if ((__i & 0x3) == 3) \
                __p += sprintf(__p, " "); \
        } \
        __off += __l; \
        d_print(__hex); \
    } \
    if (__i & 0x1f) d_print("\r\n"); \
} while (0)

/**
 * Use for trace.
 * Trace message shall be shown only if trace level is set equal or higher
 * than __level.
 */
#define d_trace(__level, fmt, ...) \
    (!g_trace_mask || TRACE_MODULE < __level ? 0 : \
        d_msg(D_MSG_TYPE_TRACE, 0, time_now(), NULL, 0, fmt, ## __VA_ARGS__))

#define d_trace2(__level, fmt, ...) \
    (!g_trace_mask || TRACE_MODULE < __level ? 0 : \
        d_msg(D_MSG_TYPE_RAW, 0, 0, NULL, 0, fmt, ## __VA_ARGS__))

/**
 * Use for trace binary buffer as printable hex string .
 * Trace message shall be shown only if trace level is set equal or higher
 * than __level.
 */
#define d_trace_hex(__level, __buf, __buflen) do { \
    if (g_trace_mask && TRACE_MODULE >= __level) \
        d_print_hex(__buf, __buflen); \
} while (0)

/**
 * Informative log.
 * If log level is larger than 4,
 * this message is logged with informative format.
 */
#define d_info(fmt, ...) \
    d_msg(D_MSG_TYPE_LOG, D_LOG_LEVEL_INFO, time_now(), \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__)

/**
 * Informative log.
 * If log level is larger than 3,
 * this message is logged with warning format.
 */
#define d_warn(fmt, ...) \
    d_msg(D_MSG_TYPE_LOG, D_LOG_LEVEL_WARN, time_now(), \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__)

/**
 * Error log.
 * If log level is larger than 2,
 * this message is logged with error format.
 */
#define d_error(fmt, ...) \
    d_msg(D_MSG_TYPE_LOG, D_LOG_LEVEL_ERROR, time_now(), \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__)

/**
 * Fatal error log.
 * If log level is larger than 1,
 * this message is logged with fatal error format.
 */
#define d_fatal(fmt, ...) \
    d_msg(D_MSG_TYPE_LOG, D_LOG_LEVEL_FATAL, time_now(), \
        __FILE__, __LINE__, fmt, ## __VA_ARGS__)

/**
 * Assertion
 * this message is logged with assertion format.
 */
#define d_assert(cond, expr, fmt, ...) \
    if (!(cond)) { \
        d_msg(D_MSG_TYPE_ASSERT, 0, time_now(), __FILE__, __LINE__, \
            "!("#cond"). "fmt, ## __VA_ARGS__); \
        expr; \
    }

status_t d_msg_console_init(int console_fd);
void d_msg_console_final();
void d_msg_syslog_init(const char *name);
void d_msg_syslog_final();
status_t d_msg_network_init(const char *name);
void d_msg_network_final();
status_t d_msg_network_start(const char *file);
void d_msg_network_stop();
void d_msg_network_final();
status_t d_msg_file_init(const char *file);
void d_msg_file_final();

void d_msg_to(int to, int on_off);

int d_msg_get_to();

/**
 * Turn on log partially.
 * level 4: d_fatal, d_error, d_warn, d_info
 * level 3: d_fatal, d_error, d_warn
 * level 2: d_fatal, d_error
 * level 1: d_fatal
 * level 0: none
 */
void d_log_set_level(int to, int level);

int d_log_get_level(int to);

/**
 * Turn on log fully.
 * All of d_info, d_warn, d_error and d_fatal will be shown.
 * Equivalent to log_level(4).
 */
void d_log_full(int to);

/**
 * Turn off log fully.
 * All log will not be shown.
 * Equivalent to log_level(0).
 */
void d_log_off(int to);

/**
 * Turn on trace mask globally.
 */
void d_trace_global_on();

/**
 * Turn off trace mask globally.
 * Any trace of any module will not be shown although some trace module is
 * turned on.
 */
void d_trace_global_off();

/**
 * Turn on trace of specifed module with level. */
void d_trace_level(int *mod_name, int level);

/**
 * Turn off trace of specifed module.
 * Equivalent to trace_level(0).
 */
void d_trace_off(int *mod_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __CORE_DEBUG_H__ */
