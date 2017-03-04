#define TRACE_MODULE _s6a_init

#include "core_debug.h"
#include "core_lib.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "s6a_app.h"

static pid_t s6a_hss_pid;

static int check_signal(int signum);

int s6a_init()
{
    status_t rv;
    int ret;
    semaphore_id semaphore;

    rv = semaphore_create(&semaphore, 0);
    d_assert(rv == CORE_OK, return -1, "semaphore_create() failed");

    s6a_hss_pid = fork();
    d_assert(s6a_hss_pid >= 0, _exit(EXIT_FAILURE), "fork() failed");

    if (s6a_hss_pid == 0)
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
    ret = mme_init();
    if (ret != 0) return -1;

    rv = semaphore_post(semaphore);
    d_assert(rv == CORE_OK, return -1, "semaphore_post() failed");

    return 0;
}

void s6a_final()
{
    mme_final();

    core_kill(s6a_hss_pid, SIGTERM);
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
