/**
 * @file main.c
 */

#include "ogs-core.h"
#include "base/context.h"

/* Server */
#include "app/application.h"
#include "app-init.h"

static char *version = "NextEPC daemon v" PACKAGE_VERSION;

static void show_version()
{
    printf("%s", version);
}

static void show_help(const char *name)
{
    printf("%s", version);

    printf("\n"
           "Usage: %s [arguments]\n"
           "\n"
           "Arguments:\n"
           "   -v                   Show version\n"
           "   -h                   Show help\n"
           "   -D                   Start as daemon\n"
           "   -f                   Set configuration file name\n"
           "   -l log_file          Log file path to be logged to\n"
           "   -p pid_file          PID file path\n"
           "   -d core:gtp:event    Enable debugging\n"
           "   -t sock:mem:         Enable trace\n"
           "\n", name);
}

static int check_signal(int signum)
{
    switch (signum)
    {
        case SIGTERM:
        case SIGINT:
        {
            ogs_info("%s received", 
                    signum == SIGTERM ? "SIGTERM" : "SIGINT");

            return 1;
        }
        case SIGHUP:
        {
            ogs_info("SIGHUP received");
            app_logger_restart();
            break;
        }
        default:
        {
            ogs_error("Signal-%d received (%s)",
                    signum, ogs_signal_description_get(signum));
            break;
        }
            
    }
    return 0;
}

void terminate()
{
    app_terminate();
    base_finalize();
    ogs_core_finalize();
}

int main(int argc, char *argv[])
{
    /**************************************************************************
     * Starting up process.
     *
     * Keep the order of starting-up
     */
    int rv;
    int i;
    app_param_t param;
    const char *debug_mask = NULL;
    const char *trace_mask = NULL;

    memset(&param, 0, sizeof(param));
    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-v"))
        {
            show_version();
            return EXIT_SUCCESS;
        }
        if (!strcmp(argv[i], "-h"))
        {
            show_help(argv[0]);
            return EXIT_SUCCESS;
        }
        if (!strcmp(argv[i], "-D"))
        {
            pid_t pid;
            pid = fork();

            ogs_assert(pid >= 0);

            if (pid != 0)
            {
                /* Parent */
                return EXIT_SUCCESS;
            }
            /* Child */

            setsid();
            umask(027);
            continue;
        }
        if (!strcmp(argv[i], "-f"))
        {
            param.config_path = argv[++i];
            continue;
        }
        if (!strcmp(argv[i], "-l"))
        {
            param.log_path = argv[++i];
            continue;
        }
        if (!strcmp(argv[i], "-p"))
        {
            param.pid_path = argv[++i];
            continue;
        }
        if (!strcmp(argv[i], "-d"))
        {
            param.log_level = OGS_LOG_DEBUG;
            param.log_domain = argv[++i];
            continue;
        }
        if (!strcmp(argv[i], "-t"))
        {
            param.log_level = OGS_LOG_TRACE;
            param.log_domain = argv[++i];
            continue;
        }
        if (argv[i][0] == '-') {
            show_help(argv[0]);
            return EXIT_FAILURE;
        }
    }

    atexit(terminate);
    ogs_core_initialize();
    ogs_setup_signal_thread();
    base_initialize();

    ogs_info("NextEPC daemon start");
    ogs_log_print(OGS_LOG_INFO, "\n");

    rv = app_initialize(&param);
    if (rv != OGS_OK)
    {
        if (rv == OGS_RETRY)
            return EXIT_SUCCESS;

        ogs_fatal("NextEPC initialization failed. Aborted");
        return EXIT_FAILURE;
    }

    ogs_log_print(OGS_LOG_INFO, "\n\n%s\n\n", version);
    ogs_signal_thread(check_signal);

    ogs_info("NextEPC daemon terminating...");

    return EXIT_SUCCESS;
}
