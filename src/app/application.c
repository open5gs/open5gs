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

#include "base/types.h"

#include "context.h"
#include "application.h"

#include "base/context.h"

#define DEFAULT_CONFIG_FILENAME SYSCONF_DIR "nextepc/nextepc.conf"

static char *version = "NextEPC daemon v" PACKAGE_VERSION;

char *app_version()
{
    return version;
}

int app_will_initialize(char **argv)
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

    context_init();

    /**************************************************************************
     * Stage 1 : Command Line Options
     */
    memset(&optarg, 0, sizeof(optarg));

    ogs_getopt_init(&options, argv);
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
        context_self()->config.file = optarg.config_file;
    else
        context_self()->config.file = DEFAULT_CONFIG_FILENAME;

    rv = context_read_file();
    if (rv != OGS_OK) return rv;

    rv = context_parse_config();
    if (rv != OGS_OK) return rv;

    /**************************************************************************
     * Stage 3 : Initialize Default Memory Pool
     */
    ogs_pkbuf_default_create(&context_self()->config.pool.defconfig);

    /**************************************************************************
     * Stage 4 : Setup LOG Module
     */
    if (optarg.log_file)
        context_self()->config.logger.file = optarg.log_file;

    if (context_self()->config.logger.file) {
        if (ogs_log_add_file(context_self()->config.logger.file) == NULL) {
            ogs_fatal("cannot open log file : %s", 
                    context_self()->config.logger.file);
            return OGS_ERROR;
        }
    }

    if (optarg.domain_mask)
        context_self()->config.logger.domain = optarg.domain_mask;

    if (optarg.log_level) 
        context_self()->config.logger.level = optarg.log_level;

    rv = ogs_log_config_domain(
            context_self()->config.logger.domain,
            context_self()->config.logger.level);
    if (rv != OGS_OK) return rv;


    /**************************************************************************
     * Stage 5 : Setup DATABASE
     */
    if (ogs_env_get("DB_URI"))
        context_self()->config.db_uri = ogs_env_get("DB_URI");

    if (context_self()->config.db_uri) {
        rv = context_db_init(context_self()->config.db_uri);
        if (rv != OGS_OK) return rv;
    }

    /**************************************************************************
     * Stage 6 : Print Result
     */
    ogs_log_print(OGS_LOG_INFO, "%s\n\n", version);

    ogs_log_print(OGS_LOG_INFO, "Configuration: '%s'\n",
            context_self()->config.file);

    if (context_self()->config.logger.file) {
        ogs_log_print(OGS_LOG_INFO, 
                "File Logging: '%s'\n", context_self()->config.logger.file);

        if (context_self()->config.logger.level)
            ogs_log_print(OGS_LOG_INFO, 
                    "LOG-LEVEL: '%s'\n", context_self()->config.logger.level);

        if (context_self()->config.logger.domain)
            ogs_log_print(OGS_LOG_INFO, 
                    "LOG-DOMAIN: '%s'\n", context_self()->config.logger.domain);
    }

    if (context_self()->config.db_uri) {
        ogs_log_print(OGS_LOG_INFO, "MongoDB URI: '%s'\n",
                context_self()->config.db_uri);
    }

    return rv;
}

int app_did_initialize(void)
{
    return OGS_OK;
}

void app_will_terminate(void)
{
}

void app_did_terminate(void)
{
    context_db_final();
    context_final();
}

int app_logger_restart()
{
    ogs_log_cycle();

    return OGS_OK;
}

