#include "core_debug.h"
#include "core_param.h"
#include "core_file.h"
#include "core_thread.h"

#include <syslog.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

int g_trace_mask = 1;

int g_msg_to = D_MSG_TO_STDOUT;

int g_console_connected = 0;
int g_syslog_connected = 0;
int g_socket_connected = 0;

int g_log_level_console = D_LOG_LEVEL_FULL;
int g_log_level_stdout = D_LOG_LEVEL_FULL;
int g_log_level_syslog = D_LOG_LEVEL_FULL;
int g_log_level_socket = D_LOG_LEVEL_FULL;

static int g_console_fd = -1;
static int g_socket_fd = -1;
static struct sockaddr_un g_socket_addr;

static thread_id socket_thread = 0;
static void *THREAD_FUNC socket_main(thread_id id, void *data);
static int socket_handler(const char *path);

void d_msg_register_syslog(const char *name)
{
    d_assert(name, return, );

    openlog(name, 0, LOG_DAEMON);
    g_syslog_connected = 1;
}

void d_msg_deregister_syslog()
{
    g_syslog_connected = 0;
    closelog();
}

void d_msg_register_socket(const char *name, const char *log_file)
{
    status_t rv;

    d_assert(name, return, );

    g_socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    d_assert(g_socket_fd >= 0, return,
            "socket() failed. (%d:%s)\n", errno, strerror(errno));

    g_socket_addr.sun_family = AF_UNIX;
    strcpy(g_socket_addr.sun_path, name);

    rv = thread_create(&socket_thread, NULL, socket_main, (void*)log_file);
    d_assert(rv == CORE_OK, return, "socket thread creation failed");

    g_socket_connected = 1;
    d_msg_to(D_MSG_TO_SOCKET, 1);
}

void d_msg_deregister_socket()
{
    d_msg_to(D_MSG_TO_SOCKET, 0);
    g_socket_connected = 0;

    thread_delete(socket_thread);

    close(g_socket_fd);
    g_socket_fd = -1;
}

static void *THREAD_FUNC socket_main(thread_id id, void *data)
{
    int ret;
    char *path = data;

    ret = socket_handler(path);
    if (ret != 0)
    {
        d_error("Failed to initialize logger.");
        d_error("Check file permission for `%s`", path); 
    }

    return NULL;
}

static int socket_handler(const char *path)
{
    status_t rv;
    int ret;
    size_t nbytes;
    ssize_t r;
    int us;
    fd_set readfd;
    struct timeval timer_val;
    struct sockaddr_un svaddr;
    file_t *g_file = NULL;
    char g_buffer[1024];

    us = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (us < 0)
    {
        d_error("socket() failed. (%d:%s)", errno, strerror(errno));
        return -1;
    }
    memcpy(&svaddr, &g_socket_addr, sizeof(struct sockaddr_un));

    ret = bind(us, (struct sockaddr *)&svaddr, sizeof(svaddr));
    if (ret != 0)
    {
        if (errno == EADDRINUSE)
        {
            ret = file_remove(svaddr.sun_path);
            if (ret != 0)
            {
                d_error("unlink(`%s`) failed. (%d:%s)",
                        svaddr.sun_path, errno, strerror(errno));
                return -1;
            }
            ret = bind(us, (struct sockaddr *)&svaddr, sizeof(svaddr));
            if (ret != 0)
            {
                d_error("bind() failed 2. (%d:%s)", errno, strerror(errno));
                return -1;
            }
        }
        else
        {
            d_error("bind() failed. (%d:%s)", errno, strerror(errno));
            return -1;
        }
    }

    rv = file_open(&g_file, path,
            FILE_CREATE | FILE_WRITE| FILE_APPEND,
            FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    if (rv != CORE_OK)
    {
        d_error("Cannot open log file '%s'", path);
        close(us);
        return -1;
    }

    while (!thread_should_stop())
    {
        timer_val.tv_sec = 0;
        timer_val.tv_usec = 50000;
        FD_ZERO(&readfd);
        FD_SET(us, &readfd);

        r = select (us+1, &readfd, NULL, NULL, &timer_val);
        if (r == -1)
        {
            if (errno == EINTR)
                break;
            d_error("select() error(%d: %s)", errno, strerror(errno));
        }

        if (r == 0)
            continue;

        if (FD_ISSET(us, &readfd))
        {
            r = read(us, g_buffer, sizeof(g_buffer));

            if (r < 0)
            {
                if (errno == EINTR)
                    break;
                d_error("read() failed. (%d:%s)", errno, strerror(errno));
                continue;
            }

            if (r == 0)
                continue;

            nbytes = r;
            rv = file_write(g_file, g_buffer, &nbytes);
            if (rv != CORE_OK || r != nbytes)
            {
                d_error("Cannot write %ld bytes to log file (%ld written)",
                        (long)r, (long)nbytes);
            }
        }
    }

    file_close(g_file);

    close(us);

    file_remove(svaddr.sun_path);

    return 0;
}

void d_msg_register_console(int console_fd)
{
    d_assert(console_fd >= 0, return, "param 'console_fd' is invalid");

    g_console_fd = console_fd;
    g_console_connected = 1;
}

void d_msg_deregister_console()
{
    g_console_connected = 0;
    g_console_fd = -1;
}

void d_msg_to(int to, int on_off)
{
    switch (to)
    {
        case D_MSG_TO_CONSOLE:
            g_msg_to = on_off ?
                g_msg_to | D_MSG_TO_CONSOLE :
                g_msg_to & ~D_MSG_TO_CONSOLE;
            break;
        case D_MSG_TO_STDOUT:
            g_msg_to = on_off ?
                g_msg_to | D_MSG_TO_STDOUT :
                g_msg_to & ~D_MSG_TO_STDOUT;
            break;
        case D_MSG_TO_SYSLOG:
            g_msg_to = on_off ?
                g_msg_to | D_MSG_TO_SYSLOG :
                g_msg_to & ~D_MSG_TO_SYSLOG;
            break;
        case D_MSG_TO_SOCKET:
            g_msg_to = on_off ?
                g_msg_to | D_MSG_TO_SOCKET :
                g_msg_to & ~D_MSG_TO_SOCKET;
            break;
        case D_MSG_TO_ALL:
            g_msg_to = on_off ? D_MSG_TO_ALL : 0;
            break;
        default:
            break;
    }
}

int d_msg_get_to()
{
    return g_msg_to;
}

void d_log_set_level(int to, int level)
{
    switch (to)
    {
        case D_MSG_TO_CONSOLE:
            g_log_level_console = level;
            break;
        case D_MSG_TO_STDOUT:
            g_log_level_stdout = level;
            break;
        case D_MSG_TO_SYSLOG:
            g_log_level_syslog = level;
            break;
        case D_MSG_TO_SOCKET:
            g_log_level_socket = level;
            break;
        case D_MSG_TO_ALL:
            g_log_level_console = level;
            g_log_level_stdout = level;
            g_log_level_syslog = level;
            g_log_level_socket = level;
            break;
        default:
            break;
    }
}

int d_log_get_level(int to)
{
    switch (to)
    {
        case D_MSG_TO_CONSOLE:
            return g_log_level_console;
        case D_MSG_TO_STDOUT:
            return g_log_level_stdout;
        case D_MSG_TO_SYSLOG:
            return g_log_level_syslog;
        case D_MSG_TO_SOCKET:
            return g_log_level_socket;
        default:
            break;
    }

    return -1;
}

void d_log_full(int to)
{
    switch (to)
    {
        case D_MSG_TO_CONSOLE:
            g_log_level_console = D_LOG_LEVEL_FULL;
            break;
        case D_MSG_TO_STDOUT:
            g_log_level_stdout = D_LOG_LEVEL_FULL;
            break;
        case D_MSG_TO_SYSLOG:
            g_log_level_syslog = D_LOG_LEVEL_FULL;
            break;
        case D_MSG_TO_SOCKET:
            g_log_level_socket = D_LOG_LEVEL_FULL;
            break;
        case D_MSG_TO_ALL:
            g_log_level_console = D_LOG_LEVEL_FULL;
            g_log_level_stdout = D_LOG_LEVEL_FULL;
            g_log_level_syslog = D_LOG_LEVEL_FULL;
            g_log_level_socket = D_LOG_LEVEL_FULL;
            break;
        default:
            break;
    }
}

void d_log_off(int to)
{
    switch (to)
    {
        case D_MSG_TO_CONSOLE:
            g_log_level_console = D_LOG_LEVEL_NONE;
            break;
        case D_MSG_TO_STDOUT:
            g_log_level_stdout = D_LOG_LEVEL_NONE;
            break;
        case D_MSG_TO_SYSLOG:
            g_log_level_syslog = D_LOG_LEVEL_NONE;
            break;
        case D_MSG_TO_SOCKET:
            g_log_level_socket = D_LOG_LEVEL_NONE;
            break;
        case D_MSG_TO_ALL:
            g_log_level_console = D_LOG_LEVEL_NONE;
            g_log_level_stdout = D_LOG_LEVEL_NONE;
            g_log_level_syslog = D_LOG_LEVEL_NONE;
            g_log_level_socket = D_LOG_LEVEL_NONE;
            break;
        default:
            break;
    }
}

void d_trace_global_on()
{
    g_trace_mask = 1;
}

void d_trace_global_off()
{
    g_trace_mask = 0;
}

void d_trace_level(int *mod_name, int level)
{
    *mod_name = level;
}

void d_trace_off(int *mod_name)
{
    *mod_name = 0;
}

#define TA_NOR          "\033[0m"   /* all off */
#define TA_BOLD         "\033[1m"   /* bold */
#define TA_UNDER        "\033[4m"   /* underscore */
#define TA_BLINK        "\033[5m"   /* blink */
#define TA_REVERSE      "\033[7m"   /* reverse video */
#define TA_CONCEALED    "\033[8m"   /* concealed */

#define TA_FGC_BLACK    "\033[30m"  /* Black */
#define TA_FGC_RED      "\033[31m"  /* Red */
#define TA_FGC_GREEN    "\033[32m"  /* Green */
#define TA_FGC_YELLOW   "\033[33m"  /* Yellow */
#define TA_FGC_BLUE     "\033[34m"  /* Blue */
#define TA_FGC_MAGENTA  "\033[35m"  /* Magenta */
#define TA_FGC_CYAN     "\033[36m"  /* Cyan */
#define TA_FGC_WHITE    "\033[37m"  /* White  */
#define TA_FGC_DEFAULT  "\033[39m"  /* default */

#define TA_BGC_BLACK    "\033[40m"  /* Black */
#define TA_BGC_RED      "\033[41m"  /* Red */
#define TA_BGC_GREEN    "\033[42m"  /* Green */
#define TA_BGC_YELLOW   "\033[43m"  /* Yellow */
#define TA_BGC_BLUE     "\033[44m"  /* Blue */
#define TA_BGC_MAGENTA  "\033[45m"  /* Magenta */
#define TA_BGC_CYAN     "\033[46m"  /* Cyan */
#define TA_BGC_WHITE    "\033[47m"  /* White */
#define TA_BGC_DEFAULT  "\033[49m"  /* default */

#define TIME_FMT_STR    "%02d/%02d %02d:%02d:%02d.%03d"
#define TIME_FMT_STR2   "%02d.%06d"
#define TIME_FMT_STR3   "%04d/%02d/%02d %02d:%02d:%02d.%03d"

int d_msg(int tp, int lv, c_time_t t, char *fn, int ln, char *fmt, ...)
{
    char str[384] = {0}, fstr[HUGE_STRING_LEN] = {0}, *ac_str;
    time_exp_t te;
    size_t n;
    char *lv_str[5] = {"NONE", "FATL", "ERRR", "WARN", "INFO"};


    va_list args;

    va_start(args, fmt);

    if (t)
    {
        time_exp_lt(&te, t);
    }

    switch (tp)
    {
        case D_MSG_TYPE_RAW:
        {
            n = vsprintf(fstr, fmt, args);

            if (g_msg_to & D_MSG_TO_STDOUT)
            {
                printf("%s", fstr);
            }
            if (g_syslog_connected && (g_msg_to & D_MSG_TO_SYSLOG))
            {
                syslog(LOG_DEBUG, "%s", fstr);
            }
            if (g_socket_connected && (g_msg_to & D_MSG_TO_SOCKET))
            {
                sendto(g_socket_fd, fstr, n, 0,
                    (struct sockaddr *)&g_socket_addr, sizeof(g_socket_addr));
            }
            if (g_console_connected && (g_msg_to & D_MSG_TO_CONSOLE))
            {
                if (fstr[n-1] == '\n')
                {
                    fstr[n-1] = '\r'; fstr[n++] = '\n';
                }
                write(g_console_fd, fstr, n);
            }
            break;
        }
        case D_MSG_TYPE_TRACE:
        {
            n = vsprintf(fstr, fmt, args);

            if (g_msg_to & D_MSG_TO_STDOUT)
            {
                printf("%s", fstr);
            }
            if (g_syslog_connected && (g_msg_to & D_MSG_TO_SYSLOG))
            {
                syslog(LOG_DEBUG, "%s", fstr);
            }
            if (g_socket_connected && (g_msg_to & D_MSG_TO_SOCKET))
            {
                sendto(g_socket_fd, fstr, n, 0,
                    (struct sockaddr *)&g_socket_addr, sizeof(g_socket_addr));
            }
            if (g_console_connected && (g_msg_to & D_MSG_TO_CONSOLE))
            {
                if (fstr[n-1] == '\n')
                {
                    fstr[n-1] = '\r'; fstr[n++] = '\n';
                }
                write(g_console_fd, fstr, n);
            }
            break;
        }
        case D_MSG_TYPE_LOG:
        {
            switch(lv)
            {
                case D_LOG_LEVEL_INFO: ac_str = TA_FGC_WHITE; break;
                case D_LOG_LEVEL_WARN: ac_str = TA_FGC_CYAN; break;
                case D_LOG_LEVEL_ERROR: ac_str = TA_FGC_YELLOW; break;
                case D_LOG_LEVEL_FATAL: ac_str = TA_FGC_RED; break;
                default: ac_str = NULL; break;
            }

            vsprintf(str, fmt, args);

            n = sprintf(fstr, "["TIME_FMT_STR"] %s: %s (%s:%d)",
                    te.tm_mon + 1, te.tm_mday, te.tm_hour,
                    te.tm_min, te.tm_sec, te.tm_usec/1000,
                    lv_str[lv], str, fn, ln);

            if ((g_msg_to & D_MSG_TO_STDOUT) &&
                    lv <= g_log_level_stdout)
            {
                printf("%s%s" TA_NOR "\n", ac_str, fstr);
            }
            if (g_syslog_connected && (g_msg_to & D_MSG_TO_SYSLOG) &&
                    lv <= g_log_level_syslog)
            {
                syslog(LOG_INFO, "[%s\n", fstr + 13);
            }
            if (g_socket_connected && (g_msg_to & D_MSG_TO_SOCKET) &&
                    lv <= g_log_level_socket)
            {
                fstr[n++] = '\n';
                sendto(g_socket_fd, fstr, n, 0,
                    (struct sockaddr *)&g_socket_addr, sizeof(g_socket_addr));
            }
            if (g_console_connected && (g_msg_to & D_MSG_TO_CONSOLE) &&
                    lv <= g_log_level_console)
            {
                fstr[n++] = '\r'; /* fstr[n++] = '\n'; FIXME: */
                write(g_console_fd, fstr, n);
            }
            break;
        }
        case D_MSG_TYPE_ASSERT:
        {
            vsprintf(str, fmt, args);

            n = sprintf(fstr, "[" TIME_FMT_STR "] ASSERT: %s (%s:%d)",
                    te.tm_mon + 1, te.tm_mday, te.tm_hour,
                    te.tm_min, te.tm_sec, te.tm_usec/1000, str, fn, ln);

            if (g_msg_to & D_MSG_TO_STDOUT)
            {
                printf(TA_BOLD TA_FGC_RED "%s" TA_NOR "\n", fstr);
            }
            if (g_syslog_connected && (g_msg_to & D_MSG_TO_SYSLOG))
            {
                syslog(LOG_CRIT, "[%s\n", fstr + 13);
            }
            if (g_socket_connected && (g_msg_to & D_MSG_TO_SOCKET))
            {
                fstr[n++] = '\n';
                sendto(g_socket_fd, fstr, n, 0,
                    (struct sockaddr *)&g_socket_addr, sizeof(g_socket_addr));
            }
            if (g_console_connected && (g_msg_to & D_MSG_TO_CONSOLE))
            {
                fstr[n++] = '\r'; /* fstr[n++] = '\n'; FIXME: */
                write(g_console_fd, fstr, n);
            }
            break;
        }
        default:
            break;
    }

    return CORE_OK;
}
