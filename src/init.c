/**
 * @file init.c
 */

/* Core library */
#define TRACE_MODULE _cellwire_init
#include "core_general.h"
#include "core_debug.h"
#include "core_thread.h"
#include "core_net.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "logger.h"

#include "cellwire.h"

#include "s6a_app.h"

static pid_t child_pid;
static thread_id net_thread;

static int check_signal(int signum);
void *THREAD_FUNC net_main(void *data);

status_t cellwire_initialize(char *config_path, char *log_path)
{
    status_t rv;
    int ret;
    semaphore_id semaphore;

    core_initialize();

    if (config_path)
    {
        /* TODO */
    }

    if (log_path)
        logger_start(log_path);

    rv = semaphore_create(&semaphore, 0);
    d_assert(rv == CORE_OK, return -1, "semaphore_create() failed");

    child_pid = fork();
    d_assert(child_pid >= 0, _exit(EXIT_FAILURE), "fork() failed");

    if (child_pid == 0)
    {
        /* Child */
        rv = semaphore_wait(semaphore);
        d_assert(rv == CORE_OK, _exit(EXIT_FAILURE), "semaphore_wait() failed");

        rv = semaphore_delete(semaphore);
        d_assert(rv == CORE_OK, _exit(EXIT_FAILURE), "semaphore_delete() failed");

        ret = hss_init();
        if (ret != 0) _exit(EXIT_FAILURE);

        signal_thread(check_signal);
        hss_final();

        _exit(EXIT_SUCCESS);
    }

    /* Parent */
    rv = mme_initialize();
    if (rv != CORE_OK) return rv;

    rv = semaphore_post(semaphore);
    d_assert(rv == CORE_OK, return -1, "semaphore_post() failed");

    rv = thread_create(&net_thread, NULL, net_main, NULL);
    if (rv != CORE_OK) return rv;

    return CORE_OK;
}

void cellwire_terminate(void)
{
    thread_delete(net_thread);

    mme_terminate();

    core_kill(child_pid, SIGTERM);

    core_terminate();
}

static int check_signal(int signum)
{
    switch (signum)
    {
        case SIGTERM:
        case SIGINT:
        {
            d_info("%s received", 
                    signum == SIGTERM ? "SIGTERM" : "SIGINT");

            return 1;
        }
        default:
        {
            d_error("Unknown signal number = %d\n", signum);
            break;
        }
            
    }
    return 0;
}

void *THREAD_FUNC net_main(void *data)
{
    while (!thread_should_stop())
    {
        net_fds_read_run(50); 
    }

    return NULL;
}

