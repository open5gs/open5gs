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
int g_network_connected = 0;
int g_file_connected = 0;

int g_log_level_console = D_LOG_LEVEL_FULL;
int g_log_level_stdout = D_LOG_LEVEL_FULL;
int g_log_level_syslog = D_LOG_LEVEL_FULL;
int g_log_level_network = D_LOG_LEVEL_FULL;
int g_log_level_file = D_LOG_LEVEL_FULL;

static int g_console_fd = -1;
static int g_network_fd = -1;
static struct sockaddr_un g_network_addr;

static thread_id network_thread = 0;
static void *THREAD_FUNC network_main(thread_id id, void *data);
static int network_handler(const char *path);

static file_t *g_file = NULL;

status_t d_msg_console_init(int console_fd)
{
    d_assert(console_fd >= 0, return CORE_ERROR,
            "param 'console_fd' is invalid");

    g_console_fd = console_fd;
    g_console_connected = 1;

    return CORE_OK;
}

void d_msg_console_final()
{
    g_console_connected = 0;
    g_console_fd = -1;
}

void d_msg_syslog_init(const char *name)
{
    d_assert(name, return, );

    openlog(name, 0, LOG_DAEMON);
    g_syslog_connected = 1;
}

void d_msg_syslog_final()
{
    g_syslog_connected = 0;
    closelog();
}

status_t d_msg_network_init(const char *name)
{
    d_assert(name, return CORE_ERROR, );

    g_network_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    d_assert(g_network_fd >= 0, return CORE_ERROR,
            "socket() failed. (%d:%s)\n", errno, strerror(errno));

    g_network_addr.sun_family = AF_UNIX;
    strcpy(g_network_addr.sun_path, name);

    return CORE_OK;
}

status_t d_msg_network_start(const char *file)
{
    status_t rv;

    d_assert(file, return CORE_ERROR, );

    rv = thread_create(&network_thread, NULL, network_main, (void*)file);
    d_assert(rv == CORE_OK, return CORE_ERROR,
            "network thread creation failed");

    g_network_connected = 1;
    d_msg_to(D_MSG_TO_NETWORK, 1);

    return CORE_OK;
}

void d_msg_network_stop()
{
    d_msg_to(D_MSG_TO_NETWORK, 0);
    g_network_connected = 0;

    if (network_thread)
        thread_delete(network_thread);
}

void d_msg_network_final()
{
    close(g_network_fd);
    g_network_fd = -1;
}

static void *THREAD_FUNC network_main(thread_id id, void *data)
{
    int ret;
    char *path = data;

    ret = network_handler(path);
    if (ret != 0)
    {
        d_error("Failed to initialize logger.");
        d_error("Check file permission for `%s`", path); 
    }

    return NULL;
}

static int network_handler(const char *path)
{
    status_t rv;
    int ret;
    size_t nbytes;
    ssize_t r;
    int us;
    fd_set readfd;
    struct timeval timer_val;
    struct sockaddr_un svaddr;
    file_t *file = NULL;
    char g_buffer[HUGE_STRING_LEN];

    us = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (us < 0)
    {
        d_error("socket() failed. (%d:%s)", errno, strerror(errno));
        return -1;
    }
    memcpy(&svaddr, &g_network_addr, sizeof(struct sockaddr_un));

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

    rv = file_open(&file, path,
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
            rv = file_write(file, g_buffer, &nbytes);
            if (rv != CORE_OK || r != nbytes)
            {
                d_error("Cannot write %ld bytes to log file (%ld written)",
                        (long)r, (long)nbytes);
            }
        }
    }

    file_close(file);

    close(us);

    file_remove(svaddr.sun_path);

    return 0;
}

status_t d_msg_file_init(const char *file)
{
    status_t rv;

    d_assert(file, return CORE_ERROR, );

    rv = file_open(&g_file, file,
            FILE_CREATE | FILE_WRITE| FILE_APPEND,
            FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    if (rv != CORE_OK)
    {
        d_error("CHECK PERMISSION of Installation Directory...");
        d_error("Cannot create LOG file '%s'", file);
        return CORE_ERROR;
    }

    g_file_connected = 1;
    d_msg_to(D_MSG_TO_FILE, 1);

    return CORE_OK;
}

void d_msg_file_final()
{
    d_msg_to(D_MSG_TO_FILE, 0);
    g_file_connected = 0;

    file_close(g_file);
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
        case D_MSG_TO_NETWORK:
            g_msg_to = on_off ?
                g_msg_to | D_MSG_TO_NETWORK :
                g_msg_to & ~D_MSG_TO_NETWORK;
            break;
        case D_MSG_TO_FILE:
            g_msg_to = on_off ?
                g_msg_to | D_MSG_TO_FILE :
                g_msg_to & ~D_MSG_TO_FILE;
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
        case D_MSG_TO_NETWORK:
            g_log_level_network = level;
            break;
        case D_MSG_TO_FILE:
            g_log_level_file = level;
            break;
        case D_MSG_TO_ALL:
            g_log_level_console = level;
            g_log_level_stdout = level;
            g_log_level_syslog = level;
            g_log_level_network = level;
            g_log_level_file = level;
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
        case D_MSG_TO_NETWORK:
            return g_log_level_network;
        case D_MSG_TO_FILE:
            return g_log_level_file;
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
        case D_MSG_TO_NETWORK:
            g_log_level_network = D_LOG_LEVEL_FULL;
            break;
        case D_MSG_TO_FILE:
            g_log_level_file = D_LOG_LEVEL_FULL;
            break;
        case D_MSG_TO_ALL:
            g_log_level_console = D_LOG_LEVEL_FULL;
            g_log_level_stdout = D_LOG_LEVEL_FULL;
            g_log_level_syslog = D_LOG_LEVEL_FULL;
            g_log_level_network = D_LOG_LEVEL_FULL;
            g_log_level_file = D_LOG_LEVEL_FULL;
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
        case D_MSG_TO_NETWORK:
            g_log_level_network = D_LOG_LEVEL_NONE;
            break;
        case D_MSG_TO_FILE:
            g_log_level_file = D_LOG_LEVEL_NONE;
            break;
        case D_MSG_TO_ALL:
            g_log_level_console = D_LOG_LEVEL_NONE;
            g_log_level_stdout = D_LOG_LEVEL_NONE;
            g_log_level_syslog = D_LOG_LEVEL_NONE;
            g_log_level_network = D_LOG_LEVEL_NONE;
            g_log_level_file = D_LOG_LEVEL_NONE;
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
    char str[HUGE_STRING_LEN+1] = {0}, fstr[HUGE_STRING_LEN+1] = {0}, *ac_str;
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
            if (g_network_connected && (g_msg_to & D_MSG_TO_NETWORK))
            {
                sendto(g_network_fd, fstr, n, 0,
                    (struct sockaddr *)&g_network_addr, sizeof(g_network_addr));
            }
            if (g_file_connected && (g_msg_to & D_MSG_TO_FILE))
            {
                size_t nbytes = n;
                file_write(g_file, fstr, &nbytes);
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
            vsprintf(str, fmt, args);

            n = snprintf(fstr, HUGE_STRING_LEN, "["TIME_FMT_STR"] %s",
                    te.tm_mon + 1, te.tm_mday, te.tm_hour,
                    te.tm_min, te.tm_sec, te.tm_usec/1000, str);

            if (g_msg_to & D_MSG_TO_STDOUT)
            {
                printf("%s", fstr);
            }
            if (g_syslog_connected && (g_msg_to & D_MSG_TO_SYSLOG))
            {
                syslog(LOG_DEBUG, "%s", fstr);
            }
            if (g_network_connected && (g_msg_to & D_MSG_TO_NETWORK))
            {
                sendto(g_network_fd, fstr, n, 0,
                    (struct sockaddr *)&g_network_addr, sizeof(g_network_addr));
            }
            if (g_file_connected && (g_msg_to & D_MSG_TO_FILE))
            {
                size_t nbytes = n;
                file_write(g_file, fstr, &nbytes);
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

            n = snprintf(fstr, HUGE_STRING_LEN,
                    "["TIME_FMT_STR"] %s: %s (%s:%d)",
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
            if (g_network_connected && (g_msg_to & D_MSG_TO_NETWORK) &&
                    lv <= g_log_level_network)
            {
                fstr[n++] = '\n';
                sendto(g_network_fd, fstr, n, 0,
                    (struct sockaddr *)&g_network_addr, sizeof(g_network_addr));
            }
            if (g_file_connected && (g_msg_to & D_MSG_TO_FILE))
            {
                size_t nbytes;

                fstr[n++] = '\n';
                nbytes = n;
                file_write(g_file, fstr, &nbytes);
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

            n = snprintf(fstr, HUGE_STRING_LEN,
                    "[" TIME_FMT_STR "] ASSERT: %s (%s:%d)",
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
            if (g_network_connected && (g_msg_to & D_MSG_TO_NETWORK))
            {
                fstr[n++] = '\n';
                sendto(g_network_fd, fstr, n, 0,
                    (struct sockaddr *)&g_network_addr, sizeof(g_network_addr));
            }
            if (g_file_connected && (g_msg_to & D_MSG_TO_FILE))
            {
                size_t nbytes;

                fstr[n++] = '\n';
                nbytes = n;
                file_write(g_file, fstr, &nbytes);
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
