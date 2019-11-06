/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-sctp.h"

#include "test-epc.h"
#include "test-config-private.h"

#define MAX_CHILD_PROCESS               8
#define OGS_ARG_MAX                     256

static ogs_thread_t *pcrf_thread = NULL;
static ogs_thread_t *pgw_thread = NULL;
static ogs_thread_t *sgw_thread = NULL;
static ogs_thread_t *hss_thread = NULL;

static ogs_proc_t process[MAX_CHILD_PROCESS];
static int process_num = 0;

static void child_main(void *data)
{
    const char **commandLine = data;
    ogs_proc_t *current = NULL;
    FILE *out = NULL;
    char buf[OGS_HUGE_LEN];
    int ret = 0, out_return_code = 0;;

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

int app_initialize(const char *const argv[])
{
    int rv;

    const char *argv_out[OGS_ARG_MAX];
    bool user_config = false;
    int i = 0;

    for (i = 0; argv[i]; i++) {
        if (strcmp("-c", argv[i]) == 0) {
            user_config = true; 
        }
        argv_out[i] = argv[i];
    }
    argv_out[i] = NULL;

    if (!user_config) {
        argv_out[i++] = "-c";
        argv_out[i++] = DEFAULT_CONFIG_FILENAME;
        argv_out[i] = NULL;
    }

    if (ogs_config()->parameter.no_pcrf == 0)
        pcrf_thread = test_child_create("pcrf", argv_out);
    if (ogs_config()->parameter.no_pgw == 0)
        pgw_thread = test_child_create("pgw", argv_out);
    if (ogs_config()->parameter.no_sgw == 0)
        sgw_thread = test_child_create("sgw", argv_out);
    if (ogs_config()->parameter.no_hss == 0)
        hss_thread = test_child_create("hss", argv_out);

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
