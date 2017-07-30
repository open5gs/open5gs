#define TRACE_MODULE _epc_main

#include "core_debug.h"
#include "core_signal.h"
#include "core_semaphore.h"

#include "app.h"

static pid_t hss_pid = 0;

static int check_signal(int signum);

status_t app_initialize(char *config_path, char *log_path)
{
    status_t rv;
    semaphore_id semaphore;

    rv = app_will_initialize(config_path, log_path);
    if (rv != CORE_OK) return rv;

    d_assert(semaphore_create(&semaphore, 0) == CORE_OK, 
            return rv, "semaphore_create() failed");

    hss_pid = fork();
    d_assert(hss_pid >= 0, _exit(EXIT_FAILURE), "fork() failed");

    if (hss_pid == 0)
    {
        /* Child */
        d_assert(semaphore_wait(semaphore) == CORE_OK, 
                _exit(EXIT_FAILURE), "semaphore_wait() failed");
        d_assert(semaphore_delete(semaphore) == CORE_OK, 
                _exit(EXIT_FAILURE), "semaphore_delete() failed");

        rv = hss_initialize();
        if (rv != CORE_OK) _exit(EXIT_FAILURE);

        signal_thread(check_signal);
        hss_terminate();

        _exit(EXIT_SUCCESS);
    }

    /* Parent */
    rv = CORE_OK;
    if (pgw_initialize() != CORE_OK) rv = CORE_ERROR;
    if (sgw_initialize() != CORE_OK) rv = CORE_ERROR;
    if (mme_initialize() != CORE_OK) rv = CORE_ERROR;

    d_assert(semaphore_post(semaphore) == CORE_OK,,
            "semaphore_post() failed");

    if (app_did_initialize(config_path, log_path) != CORE_OK) rv = CORE_ERROR;

    return rv;;
}

void app_terminate(void)
{
    app_will_terminate();

    if (hss_pid)
        core_kill(hss_pid, SIGTERM);

    mme_terminate();
    sgw_terminate();
    pgw_terminate();

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
