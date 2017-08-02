#include "logger.h"

#include "core_general.h"
#include "core_debug.h"
#include "core_param.h"
#include "core_file.h"
#include "core_signal.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <signal.h>

#define MAX_LOG_FILE_SIZE (6*16*1024) /* 96 KB */
#define MAX_BACKUP_LOG_FILE_NUM 19 /* 96 KB x 19 */
#define FILE_CHECK_CYCLE 15

static char g_buffer[1024];
static char g_path[MAX_FILENAME_SIZE] = "wemania.log";
static file_t *g_file = NULL;

static int request_stop = 0;

status_t log_file_backup()
{
    status_t rv;
    char fn[MAX_FILENAME_SIZE], nfn[MAX_FILENAME_SIZE];
    file_t *tmpf = NULL;
    int i, j;

    file_close(g_file);

    for (i = 0; ; i++)
    {
        sprintf(fn, "%s.%d", g_path, i);

        rv = file_open(&tmpf, fn, FILE_READ, 0);
        if (rv != CORE_OK)
            break;

        file_close(tmpf);
    }

    for (j = i - 1; j >= 0; j--)
    {
        sprintf(fn, "%s.%d", g_path, j);
        sprintf(nfn, "%s.%d", g_path, j + 1);

        if (j + 1 > MAX_BACKUP_LOG_FILE_NUM)
        {
            rv = file_remove(fn);
            if (rv != CORE_OK)
            {
                fprintf(stderr, "Can't remove '%s'\n", fn);
                return CORE_ERROR;
            }
        }
        else
        {
            rv = file_rename(fn, nfn);
            if (rv != CORE_OK)
            {
                fprintf(stderr, "Can't rename '%s' to '%s'\n", fn, nfn);
                return CORE_ERROR;
            }
        }
    }

    sprintf(fn, "%s", g_path);
    sprintf(nfn, "%s.0", g_path);

    rv = file_rename(fn, nfn);
    if (rv != CORE_OK)
    {
        fprintf(stderr, "Can't rename '%s' to '%s'\n", fn, nfn);
        return CORE_ERROR;
    }

    rv = file_open(&g_file, g_path,
            FILE_CREATE | FILE_WRITE| FILE_APPEND,
            FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    if (rv != CORE_OK)
    {
        fprintf(stderr, "Cannot open log file '%s'", g_path);
        return CORE_ERROR;
    }

    return CORE_OK;
}

int logger_start(const char *path)
{
    status_t rv;
    int ret, count = 0;
    size_t nbytes;
    ssize_t r;
    file_info_t finfo;
    int us;
    fd_set readfd;
    struct timeval timer_val;
    struct sockaddr_un svaddr;

    /* If path is given, use it */
    if (path)
        strcpy(g_path, path);

    us = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (us < 0)
    {
        fprintf(stderr, "socket() failed. (%d:%s)\n",
                errno, strerror(errno));
        return -1;
    }

    svaddr.sun_family = AF_UNIX;
    strcpy(svaddr.sun_path, D_LOGD_IPC_PATH);

    ret = bind(us, (struct sockaddr *)&svaddr, sizeof(svaddr));
    if (ret != 0)
    {
        if (errno == EADDRINUSE)
        {
            ret = unlink(D_LOGD_IPC_PATH);
            if (ret != 0)
            {
                fprintf(stderr, "unlink() failed. (%d:%s)\n",
                        errno, strerror(errno));
                return -1;
            }
            ret = bind(us, (struct sockaddr *)&svaddr, sizeof(svaddr));
            if (ret != 0)
            {
                fprintf(stderr, "bind() failed 2. (%d:%s)\n",
                        errno, strerror(errno));
                return -1;
            }
        }
        else
        {
            fprintf(stderr, "bind() failed. (%d:%s)\n",
                    errno, strerror(errno));
            return -1;
        }
    }

    rv = file_open(&g_file, g_path,
            FILE_CREATE | FILE_WRITE| FILE_APPEND,
            FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    if (rv != CORE_OK)
    {
        fprintf(stderr, "Cannot open log file '%s'\n", g_path);
        close(us);
        return -1;
    }


    while (!request_stop)
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
            fprintf(stderr, "select() error(%d: %s)",
                    errno, strerror(errno));
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
                fprintf(stderr, "read() failed. (%d:%s)\n",
                        errno, strerror(errno));
                continue;
            }

            if (r == 0)
                continue;

            nbytes = r;
            rv = file_write(g_file, g_buffer, &nbytes);
            if (rv != CORE_OK || r != nbytes)
            {
                fprintf(stderr, "Cannot write %ld bytes to log file (%ld written)\n",
                        (long)r, (long)nbytes);
            }

            if (count % FILE_CHECK_CYCLE == 0)
            {
                file_info_get(&finfo, FILE_INFO_SIZE, g_file);
                if (finfo.size > MAX_LOG_FILE_SIZE)
                {
                    log_file_backup();
                }
            }

            count++;
        }
    }

    file_close(g_file);

    close(us);

    return 0;
}

void logger_stop()
{
    request_stop = 1;
}
