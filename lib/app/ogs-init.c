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

#include "ogs-app.h"

int __ogs_app_domain;

int ogs_app_initialize(const char *default_config, const char *const argv[])
{
    int rv, opt;
    ogs_getopt_t options;
    struct {
        char *config_file;
        char *log_file;
        char *log_level;
        char *domain_mask;

        bool enable_debug;
        bool enable_trace;
    } optarg;

    ogs_core_initialize();
    ogs_app_setup_log();

    ogs_config_init();

    /**************************************************************************
     * Stage 1 : Command Line Options
     */
    memset(&optarg, 0, sizeof(optarg));

    ogs_getopt_init(&options, (char**)argv);
    while ((opt = ogs_getopt(&options, "c:l:e:m:")) != -1) {
        switch (opt) {
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
        case '?':
        default:
            ogs_assert_if_reached();
            return OGS_ERROR;
        }
    }

    /**************************************************************************
     * Stage 2 : Load Configuration File
     */
    if (optarg.config_file)
        ogs_config()->file = optarg.config_file;
    else
        ogs_config()->file = default_config;

    rv = ogs_config_read();
    if (rv != OGS_OK) return rv;

    rv = ogs_config_parse();
    if (rv != OGS_OK) return rv;

    /**************************************************************************
     * Stage 3 : Initialize Default Memory Pool
     */
    ogs_pkbuf_default_create(&ogs_config()->pool.defconfig);

    /**************************************************************************
     * Stage 4 : Setup LOG Module
     */
    if (optarg.log_file)
        ogs_config()->logger.file = optarg.log_file;

    if (ogs_config()->logger.file) {
        if (ogs_log_add_file(ogs_config()->logger.file) == NULL) {
            ogs_fatal("cannot open log file : %s", 
                    ogs_config()->logger.file);
            return OGS_ERROR;
        }
    }

    if (optarg.domain_mask)
        ogs_config()->logger.domain = optarg.domain_mask;

    if (optarg.log_level) 
        ogs_config()->logger.level = optarg.log_level;

    rv = ogs_log_config_domain(
            ogs_config()->logger.domain, ogs_config()->logger.level);
    if (rv != OGS_OK) return rv;

    /**************************************************************************
     * Stage 5 : Setup Database Module
     */
    if (ogs_env_get("DB_URI"))
        ogs_config()->db_uri = ogs_env_get("DB_URI");

    return rv;
}

void ogs_app_terminate(void)
{
    ogs_config_final();

    ogs_pkbuf_default_destroy();

    ogs_core_terminate();
}

void ogs_app_setup_log(void)
{
    ogs_log_install_domain(&__ogs_app_domain, "app", ogs_core()->log.level);
}
