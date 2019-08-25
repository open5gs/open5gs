/**
 * @file main.c
 */

#include "ogs-core.h"
#include "base/context.h"

/* Server */
#include "app/application.h"
#include "app-init.h"

void show_version()
{
    printf("%s\n\n", app_version());
}

void show_help(const char *name)
{
    printf("Usage: %s [options]\n"
        "Options:\n"
       "   -c filename    : set configuration file\n"
       "   -l filename    : set logging file\n"
       "   -e level       : set global log-level (default:info)\n"
       "   -m domain      : set log-domain (e.g. mme:sgw:gtp)\n"
       "   -d             : print lots of debugging information\n"
       "   -t             : print tracing information for developer\n"
       "   -D             : start as a daemon\n"
       "   -v             : show version number and exit\n"
       "   -h             : show this message and exit\n"
       "\n", name);
}

static int check_signal(int signum)
{
    switch (signum) {
    case SIGTERM:
    case SIGINT:
        ogs_info("%s received", 
                signum == SIGTERM ? "SIGTERM" : "SIGINT");

        return 1;
    case SIGHUP:
        ogs_info("SIGHUP received");
        app_logger_restart();
        break;
    default:
        ogs_error("Signal-NUM[%d] received (%s)",
                signum, ogs_signal_description_get(signum));
        break;
            
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
    int rv, i, opt;
    ogs_getopt_t options;
    struct {
        char *config_file;
        char *log_file;
        char *log_level;
        char *domain_mask;

        bool enable_debug;
        bool enable_trace;
    } optarg;
    char *argv_out[argc];

    memset(&optarg, 0, sizeof(optarg));

    ogs_getopt_init(&options, argv);
    while ((opt = ogs_getopt(&options, "vhDc:l:e:m:dt")) != -1) {
        switch (opt) {
        case 'v':
            show_version();
            return OGS_OK;
        case 'h':
            show_help(argv[0]);
            return OGS_OK;
        case 'D':
#if !defined(_WIN32)
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
        }
#else
            printf("%s: Not Support in WINDOWS", argv[0]);
#endif
            break;
        case 'c':
            optarg.config_file = options.optarg;
            break;
        case 'l':
            optarg.log_file = options.optarg;
            break;
        case 'e':
            optarg.log_level = options.optarg;
            break;
        case 'm':
            optarg.domain_mask = options.optarg;
            break;
        case 'd':
            optarg.enable_debug = true;
            break;
        case 't':
            optarg.enable_trace = true;
            break;
        case '?':
            fprintf(stderr, "%s: %s\n", argv[0], options.errmsg);
            show_help(argv[0]);
            return OGS_ERROR;
        default:
            fprintf(stderr, "%s: should not be reached\n", OGS_FUNC);
            return OGS_ERROR;
        }
    }

    if (optarg.enable_debug) optarg.log_level = "debug";
    if (optarg.enable_trace) optarg.log_level = "trace";

    i = 0;
    argv_out[i++] = argv[0];

    if (optarg.config_file) {
        argv_out[i++] = "-c";
        argv_out[i++] = optarg.config_file;
    }
    if (optarg.log_file) {
        argv_out[i++] = "-l";
        argv_out[i++] = optarg.log_file;
    }
    if (optarg.log_level) {
        argv_out[i++] = "-e";
        argv_out[i++] = optarg.log_level;
    }
    if (optarg.domain_mask) {
        argv_out[i++] = "-m";
        argv_out[i++] = optarg.domain_mask;
    }

    argv_out[i] = NULL;

    ogs_core_initialize();
    ogs_setup_signal_thread();
    base_initialize();

    rv = app_initialize(argv_out);
    if (rv != OGS_OK) {
        if (rv == OGS_RETRY)
            return EXIT_SUCCESS;

        ogs_fatal("NextEPC initialization failed. Aborted");
        return OGS_ERROR;
    }
    atexit(terminate);
    ogs_signal_thread(check_signal);

    ogs_info("NextEPC daemon terminating...");

    return OGS_OK;
}

