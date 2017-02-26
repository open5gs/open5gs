/**
 * @file main.c
 */

/* Core library */
#include "core.h"
#include "core_general.h"
#define TRACE_MODULE _main_
#include "core_debug.h"
#include "core_cond.h"
#include "core_signal.h"
#include "core_net.h"

#include "logger.h"
#include "symtbl.h"

#include "s6a_lib.h"

/* Server */
#include "cellwire.h"

char *config_path = NULL;
extern char g_compile_time[];

static void show_version()
{
    printf("CellWire daemon v%s - %s\n", 
            PACKAGE_VERSION, g_compile_time);
}

static void show_help()
{
    show_version();

    printf("\n"
           "Usage: cellwired [arguments]\n"
           "\n"
           "Arguments:\n"
           "   -v                   Show version\n"
           "   -h                   Show help\n"
           "   -d                   Start as daemon\n"
           "   -f                   Set configuration file name\n"
           "   -l log_path          Fork log daemon with file path to be logged to\n"
           "\n"
           );
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

            threads_stop();

            return 1;
        }
        case SIGHUP:
            break;
        default:
        {
            d_error("Unknown Signal Number = %d\n", signum);
            break;
        }
            
    }
    return 0;
}

void logger_signal(int signum)
{
    switch (signum)
    {
        case SIGTERM:
        case SIGINT:
            logger_stop();
            break;
        case SIGHUP:
            break;
        default:
            break;
    }
}

void s6a_fd_hss_signal(int signum)
{
    fprintf(stderr, "asdfsadfsadfsdafasdfsadf = %d\n", signum);
    switch (signum)
    {
        case SIGTERM:
        case SIGINT:
            s6a_fd_hss_final();
            break;
        case SIGHUP:
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[])
{
    int opt;
    int opt_daemon = 0;
    int opt_logger = 0;

    char *log_path = NULL;

    /**************************************************************************
     * Starting up process.
     *
     * Keep the order of starting-up
     */

    while (1)
    {
        opt = getopt (argc, argv, "vhdf:l:");
        if (opt == -1)
            break;

        switch (opt)
        {
            case 'v':
                show_version();
                return EXIT_SUCCESS;
            case 'h':
                show_help();
                return EXIT_SUCCESS;
            case 'd':
                opt_daemon = 1;
                break;
            case 'f':
                config_path = optarg;
                break;
            case 'l':
                opt_logger = 1;
                log_path = optarg;
                break;
            default:
                show_help();
                return EXIT_FAILURE;
        }
    }

    if (opt_daemon)
    {
        pid_t pid;
        pid = fork();

        d_assert(pid != -1, return EXIT_FAILURE, "fork() failed");

        if (pid != 0)
        {
            /* Parent */
            return EXIT_SUCCESS;
        }
        /* Child */

        setsid();
        umask(027);
    }

    core_initialize();

    if (opt_logger)
    {
        pid_t pid;
        pid = fork();

        d_assert(pid != -1, return EXIT_FAILURE, "fork() failed");

        if (pid == 0)
        {
            /* Child */
            umask(027);

            core_signal(SIGINT, logger_signal);
            core_signal(SIGTERM, logger_signal);
            core_signal(SIGHUP, logger_signal);

            logger_start(log_path);

            return EXIT_SUCCESS;
        }
        /* Parent */
    }

    {
        pid_t pid;
        pid = fork();

        d_assert(pid != -1, return EXIT_FAILURE, "fork() failed");

        if (pid == 0)
        {
            /* Child */
            umask(027);

            core_signal(SIGINT, s6a_fd_hss_signal);
            core_signal(SIGTERM, s6a_fd_hss_signal);
            core_signal(SIGHUP, s6a_fd_hss_signal);

            s6a_fd_hss_init();

            return EXIT_SUCCESS;
        }

        /* Parent */
    }

    {
        extern int _mme_sm;
        extern int _enb_s1_sm;
        extern int _s1ap_path;

        d_trace_level(&_mme_sm, 100);
        d_trace_level(&_enb_s1_sm, 100);
        d_trace_level(&_s1ap_path, 100);
    }

    signal_init();

    if (cellwire_initialize(config_path) != CORE_OK)
    {
        d_fatal("CellWire initialization failed. Aborted");
        return EXIT_FAILURE;
    }
    
    show_version();
    d_info("CellWire daemon start");

    threads_start();

    signal_thread(check_signal);

    d_info("CellWire daemon terminating...");

    cellwire_terminate();

    core_terminate();

    return EXIT_SUCCESS;
}
