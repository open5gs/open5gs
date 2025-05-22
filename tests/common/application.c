/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "test-config-private.h"
#include "test-common.h"

static void run(int argc, const char *const argv[],
        const char *name, void (*init)(const char * const argv[]))
{
    int rv;
    bool user_config;

    /* '-f sample-XXXX.conf -e error' is always added */
    const char *argv_out[argc+5], *new_argv[argc+5];
    int argc_out;

    char conf_file[OGS_MAX_FILEPATH_LEN];
    
    user_config = false;
    for (argc_out = 0; argc_out < argc; argc_out++) {
        if (strcmp("-c", argv[argc_out]) == 0) {
            user_config = true; 
        }
        argv_out[argc_out] = argv[argc_out];
    }
    argv_out[argc_out] = NULL;

    if (!user_config) {
        /* buildroot/configs/XXX-conf.yaml */
        ogs_snprintf(conf_file, sizeof conf_file, "%s%s",
            MESON_BUILD_ROOT OGS_DIR_SEPARATOR_S
            "configs" OGS_DIR_SEPARATOR_S, name);
        argv_out[argc_out++] = "-c";
        argv_out[argc_out++] = conf_file;
        argv_out[argc_out] = NULL;
    }

    /* buildroot/src/open5gs-main */
    argv_out[0] = MESON_BUILD_ROOT OGS_DIR_SEPARATOR_S 
            "src" OGS_DIR_SEPARATOR_S "open5gs-main";

    rv = abts_main(argc_out, argv_out, new_argv);
    ogs_assert(rv == OGS_OK);

    (*init)(new_argv);
}

void test_app_run(int argc, const char *const argv[],
        const char *name, void (*init)(const char * const argv[]))
{
    int rv;

    run(argc, argv, name, init);

    rv = ogs_app_parse_local_conf("test");
    ogs_assert(rv == OGS_OK);

    test_context_init();

    rv = test_context_parse_config();
    ogs_assert(rv == OGS_OK);
}

#define MAX_CHILD_PROCESS               32
#define OGS_ARG_MAX                     256

static ogs_proc_t process[MAX_CHILD_PROCESS];
static int process_num = 0;

static void child_main(void *data)
{
    thread_data_t *thread_data = data;
    const char **commandLine = thread_data->commandLine;
    ogs_proc_t *current = NULL;
    FILE *out = NULL;
    char buf[OGS_HUGE_LEN];
    int ret = 0, out_return_code = 0;

    current = &process[process_num++];

    // get name and index of NF into the process.
    current->nf_name =  ogs_strdup(thread_data->nf_name);
    current->index = thread_data->index;

    if (process_num > MAX_CHILD_PROCESS) {
        ogs_fatal("Process limit reached");
        ogs_assert_if_reached();
    }

    ret = ogs_proc_create(commandLine,
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

ogs_thread_t *test_child_create(const char *name, int index, const char *const argv[])
{
    ogs_thread_t *child = NULL;
    thread_data_t thread_data;

    const char *commandLine[OGS_ARG_MAX];
    int i = 0;
    char command[OGS_MAX_FILEPATH_LEN];

    memset(&thread_data, 0, sizeof(thread_data));

    while(argv[i] && i < 32) {
        commandLine[i] = argv[i];
        i++;
    }
    commandLine[i] = NULL;

    /* buildroot/src/mme/open5gs-mmed */
    ogs_snprintf(command, sizeof command, "%s%s%s%sd",
            MESON_BUILD_ROOT OGS_DIR_SEPARATOR_S "src" OGS_DIR_SEPARATOR_S,
            name, OGS_DIR_SEPARATOR_S "open5gs-", name);
    commandLine[0] = command;

    thread_data.commandLine = commandLine;
    thread_data.nf_name = name;
    thread_data.index = index;

    child = ogs_thread_create(child_main, &thread_data);

    ogs_msleep(50);

    return child;
}

void test_child_terminate(void)
{
    int i;
    ogs_proc_t *current = NULL;
    for (i = 0; i < process_num; i++) {
        current = &process[i];

        if (current->stdin_file != 0 &&  current->child != 0) {
            ogs_proc_terminate(current);
        }
    }
}

void test_child_terminate_with_name(char *name, int index)
{
    int i;
    ogs_proc_t *current = NULL;
    for (i = 0; i < process_num; i++) {
        current = &process[i];

        if (!strcmp(current->nf_name, name) &&
                current->index == index &&
                current->stdin_file != 0 &&
                current->child != 0) {
                ogs_proc_terminate(current);
                break;
        }
    }
}