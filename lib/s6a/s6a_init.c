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

        CHECK_FCT_DO(s6a_fd_init(s6a_hss_config()), 
                _exit(EXIT_FAILURE));

        CHECK_FCT_DO(s6a_app_init(MODE_HSS), _exit(EXIT_FAILURE));

        signal_thread(check_signal);
        s6a_final();

        _exit(EXIT_SUCCESS);
    }

    /* Parent */
    CHECK_FCT_DO(s6a_fd_init(s6a_mme_config()), return -1);

    CHECK_FCT_DO(s6a_app_init(MODE_MME), return -1);

    rv = semaphore_post(semaphore);
    d_assert(rv == CORE_OK, return -1, "semaphore_post() failed");

    return 0;
}

void s6a_final()
{
    s6a_app_final();

    s6a_fd_final();

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
        case SIGUSR1:
        {
            void s6a_cli_test_message();
            s6a_cli_test_message();
            break;
        }
        default:
        {
            d_error("Unknown signal number = %d\n", signum);
            break;
        }
            
    }
    return 0;
}
