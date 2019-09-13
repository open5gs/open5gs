#include "ogs-sctp.h"
#include "ogs-app.h"

static ogs_thread_t *pcrf_thread = NULL;
static ogs_thread_t *pgw_thread = NULL;
static ogs_thread_t *sgw_thread = NULL;
static ogs_thread_t *hss_thread = NULL;

#define MAX_CHILD_PROCESS 8
static ogs_proc_t process[MAX_CHILD_PROCESS];
static int process_num = 0;

static void child_main(void *data)
{
    char **commandLine = data;
    ogs_proc_t *current = NULL;
    FILE *out = NULL;
    char buf[OGS_HUGE_LEN];
    int ret = 0, out_return_code = 0;;

    current = &process[process_num++];
    ret = ogs_proc_create((const char *const*)commandLine,
            ogs_proc_option_combined_stdout_stderr|
            ogs_proc_option_inherit_environment,
            current);
    ogs_assert(ret == 0);
    out = ogs_proc_stdout(current);
    ogs_assert(out);

    while(fgets(buf, OGS_HUGE_LEN, out)) {
        printf("%s", buf);
    }

    ret = ogs_proc_join(current, &out_return_code);
    ogs_assert(ret == 0);
    ogs_assert(out_return_code == 0);

    ret = ogs_proc_destroy(current);
    ogs_assert(ret == 0);
}

ogs_thread_t *epc_child_create(char *name, char **argv)
{
    ogs_thread_t *child = NULL;
#define OGS_ARG_MAX                     256
    char *commandLine[OGS_ARG_MAX];
    int i = 0;
    char directory[OGS_MAX_FILEPATH_LEN];
    char command[OGS_MAX_FILEPATH_LEN];

    while(argv[i] && i < 32) {
        commandLine[i] = argv[i];
        i++;
    }
    commandLine[i] = NULL;

    ogs_path_remove_last_component(directory, argv[0]);
    if (strstr(directory, ".libs"))
        ogs_path_remove_last_component(directory, directory);
    ogs_snprintf(command, sizeof command, "%s/%s", directory, name);
    commandLine[0] = command;

    child = ogs_thread_create(child_main, (void*)commandLine);
    ogs_msleep(50);

    return child;
}

void epc_child_terminate(void)
{
    int i;
    ogs_proc_t *current = NULL;
    for (i = 0; i < process_num; i++) {
        current = &process[i];
        ogs_proc_terminate(current);
    }
}

int app_initialize(char **argv)
{
    int rv;

    if (ogs_config()->parameter.no_pcrf == 0)
        pcrf_thread = epc_child_create("nextepc-pcrfd", argv);
    if (ogs_config()->parameter.no_pgw == 0)
        pgw_thread = epc_child_create("nextepc-pgwd", argv);
    if (ogs_config()->parameter.no_sgw == 0)
        sgw_thread = epc_child_create("nextepc-sgwd", argv);
    if (ogs_config()->parameter.no_hss == 0)
        hss_thread = epc_child_create("nextepc-hssd", argv);

    ogs_sctp_init(ogs_config()->usrsctp.udp_port);

    rv = mme_initialize();
    ogs_assert(rv == OGS_OK);
    ogs_info("MME initialize...done");

    return OGS_OK;;
}

void app_terminate(void)
{
    mme_terminate();

    ogs_sctp_final();
    ogs_info("MME terminate...done");

    if (hss_thread) ogs_thread_destroy(hss_thread);
    if (sgw_thread) ogs_thread_destroy(sgw_thread);
    if (pgw_thread) ogs_thread_destroy(pgw_thread);
    if (pcrf_thread) ogs_thread_destroy(pcrf_thread);
}
