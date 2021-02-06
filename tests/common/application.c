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
    const char *argv_out[argc+4], *new_argv[argc+4];
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

    test_context_init();

    rv = test_context_parse_config();
    ogs_assert(rv == OGS_OK);
}

#define MAX_CHILD_PROCESS               16
#define OGS_ARG_MAX                     256

static ogs_proc_t process[MAX_CHILD_PROCESS];
static int process_num = 0;

static void child_main(void *data)
{
    const char **commandLine = data;
    ogs_proc_t *current = NULL;
    FILE *out = NULL;
    char buf[OGS_HUGE_LEN];
    int ret = 0, out_return_code = 0;

    current = &process[process_num++];
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

ogs_thread_t *test_child_create(const char *name, const char *const argv[])
{
    ogs_thread_t *child = NULL;
    const char *commandLine[OGS_ARG_MAX];
    int i = 0;
    char command[OGS_MAX_FILEPATH_LEN];

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

    child = ogs_thread_create(child_main, commandLine);
    ogs_msleep(50);

    return child;
}

void test_child_terminate(void)
{
    int i;
    ogs_proc_t *current = NULL;
    for (i = 0; i < process_num; i++) {
        current = &process[i];
        ogs_proc_terminate(current);
    }
}
