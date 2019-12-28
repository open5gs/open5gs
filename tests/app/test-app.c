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

#include "test-app.h"
#include "test-config-private.h"

static int connected_count = 0;
static void test_diam_logger_handler(enum fd_hook_type type, struct msg * msg, 
    struct peer_hdr * peer, void * other, struct fd_hook_permsgdata *pmd, 
    void * regdata)
{
    if (type == HOOK_PEER_CONNECT_SUCCESS) {
        connected_count++;
    }
}

void test_app_run(int argc, const char *const argv[],
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

    ogs_diam_logger_register(test_diam_logger_handler);

    (*init)(new_argv);

    while(1) {
        if (connected_count == 1) break;
        ogs_msleep(50);
    }

    ogs_msleep(500); /* Wait for listening all sockets */
}

void test_app_init(void)
{
    ogs_log_install_domain(&__ogs_sctp_domain, "sctp", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_s1ap_domain, "s1ap", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_diam_domain, "diam", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_dbi_domain, "dbi", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_nas_domain, "nas", OGS_LOG_ERROR);

    ogs_assert(ogs_mongoc_init(ogs_config()->db_uri) == OGS_OK);
}

