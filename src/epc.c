#define TRACE_MODULE _epc_main

#include "core_debug.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "app.h"

static pid_t hss_pid;

static int check_signal(int signum);

status_t app_initialize(char *config_path, char *log_path)
{
    status_t rv;
    semaphore_id semaphore;

    app_will_initialize(config_path, log_path);

    rv = semaphore_create(&semaphore, 0);
    d_assert(rv == CORE_OK, return -1, "semaphore_create() failed");

    hss_pid = fork();
    d_assert(hss_pid >= 0, _exit(EXIT_FAILURE), "fork() failed");

    if (hss_pid == 0)
    {
        /* Child */
        rv = semaphore_wait(semaphore);
        d_assert(rv == CORE_OK, _exit(EXIT_FAILURE), "semaphore_wait() failed");
        rv = semaphore_delete(semaphore);
        d_assert(rv == CORE_OK, _exit(EXIT_FAILURE), "semaphore_delete() failed");

        rv = hss_initialize();
        if (rv != CORE_OK) _exit(EXIT_FAILURE);

        signal_thread(check_signal);
        hss_terminate();

        _exit(EXIT_SUCCESS);
    }

    /* Parent */
    rv = pgw_initialize();
    if (rv != CORE_OK) return rv;
    rv = sgw_initialize();
    if (rv != CORE_OK) return rv;
    rv = mme_initialize();
    if (rv != CORE_OK) return rv;

    rv = semaphore_post(semaphore);
    d_assert(rv == CORE_OK, return -1, "semaphore_post() failed");

    app_did_initialize(config_path, log_path);

    return CORE_OK;
}

void app_terminate(void)
{
    app_will_terminate();

    mme_terminate();
    sgw_terminate();
    pgw_terminate();

    core_kill(hss_pid, SIGTERM);

    app_did_terminate();
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
