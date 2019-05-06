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

#include "ogs-core.h"
#include "core/abts.h"

#include "fd/fd_lib.h"

#include "app/application.h"
#include "app/context.h"

#include "app_init.h"
#include "mme/mme_context.h"

abts_suite *test_s1ap_message(abts_suite *suite);
abts_suite *test_nas_message(abts_suite *suite);
abts_suite *test_gtp_message(abts_suite *suite);
abts_suite *test_security(abts_suite *suite);
abts_suite *test_sctp(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {test_s1ap_message},
    {test_nas_message},
    {test_gtp_message},
    {test_security},
#if !defined(HAVE_USRSCTP)
    {test_sctp},
#endif
    {NULL},
};

void test_terminate(void)
{
    app_will_terminate();
    mme_context_final();
    app_did_terminate();

    base_finalize();
    ogs_core_finalize();
}

int test_initialize(app_param_t *param, int argc, const char *const argv[])
{
    int rv;

    atexit(test_terminate);

    ogs_core_initialize();
    base_initialize();

    param->logfile_disabled = true;
    param->db_disabled = true;
    rv = app_will_initialize(param);
    if (rv != OGS_OK)
    {
        ogs_error("app_will_initialize() failed");
        return OGS_ERROR;
    }

    mme_context_init();
    app_did_initialize();

    return rv;
}

int main(int argc, const char **argv)
{
    int i;
    app_param_t param;
    const char *debug_mask = NULL;
    const char *trace_mask = NULL;
    char config_dir[MAX_FILEPATH_LEN/2];
    char config_path[MAX_FILEPATH_LEN];
    abts_suite *suite = NULL;

    abts_init(argc, argv);

    memset(&param, 0, sizeof(param));
    for (i = 1; i < argc; i++) {
        /* abts_init(argc, argv) handles the following options */
        if (!strcmp(argv[i], "-v")) continue;
        if (!strcmp(argv[i], "-x")) continue;
        if (!strcmp(argv[i], "-l")) continue;
        if (!strcmp(argv[i], "-q")) continue;

        if (!strcmp(argv[i], "-d")) {
            param.log_level = OGS_LOG_DEBUG;
            param.log_domain = argv[++i];
            continue;
        }
        if (!strcmp(argv[i], "-t")) {
            param.log_level = OGS_LOG_TRACE;
            param.log_domain = argv[++i];
            continue;
        }
        if (!strcmp(argv[i], "-f")) {
            param.config_path = argv[++i];
            continue;
        }

        if (argv[i][0] == '-') {
            fprintf(stderr, "Invalid option: `%s'\n", argv[i]);
            exit(1);
        }
    }

    if (!param.config_path)
    {
        ogs_path_remove_last_component(config_dir, argv[0]);
        if (strstr(config_dir, ".libs"))
            ogs_path_remove_last_component(config_dir, config_dir);
        ogs_snprintf(config_path, sizeof config_path,
                "%s/sample.conf", config_dir);
        param.config_path = config_path;
    }

    if (param.log_level)
        ogs_core()->log.level = OGS_LOG_DEFAULT;
    else
        ogs_core()->log.level = OGS_LOG_ERROR;
    test_initialize(&param, argc, argv);

    for (i = 0; alltests[i].func; i++)
    {
        suite = alltests[i].func(suite);
    }

    return abts_report(suite);
}
