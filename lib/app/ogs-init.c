/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

static int read_config(void);
static int parse_config(void);

int ogs_app_initialize(
        const char *version, const char *default_config,
        const char *const argv[])
{
    int rv, opt;
    ogs_getopt_t options;
    struct {
        char *config_file;
        char *log_file;
        char *log_level;
        char *domain_mask;
        char *config_section_id;
    } optarg;

    ogs_core_initialize();
    ogs_app_setup_log();

    ogs_app_context_init();
    ogs_app_config_init();
    ogs_app()->version = version;

    /**************************************************************************
     * Stage 1 : Command Line Options
     */
    memset(&optarg, 0, sizeof(optarg));

    ogs_getopt_init(&options, (char**)argv);
    while ((opt = ogs_getopt(&options, "c:l:e:m:k:")) != -1) {
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
        case 'k':
            optarg.config_section_id = options.optarg;
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
        ogs_app()->file = optarg.config_file;
    else
        ogs_app()->file = default_config;

    rv = read_config();
    if (rv != OGS_OK) return rv;

    rv = parse_config();
    if (rv != OGS_OK) return rv;

    /**************************************************************************
     * Stage 3 : Initialize Default Memory Pool
     */
    ogs_pkbuf_default_create(&ogs_global_conf()->pkbuf_config);

    /**************************************************************************
     * Stage 4 : Setup LOG Module
     */
    if (optarg.log_file)
        ogs_app()->logger.file = optarg.log_file;

    if (ogs_app()->logger.file) {
        if (ogs_log_add_file(ogs_app()->logger.file) == NULL) {
            ogs_fatal("cannot open log file : %s", 
                    ogs_app()->logger.file);
            return OGS_ERROR;
        }
    }

    if (optarg.domain_mask)
        ogs_app()->logger.domain = optarg.domain_mask;

    if (optarg.log_level) 
        ogs_app()->logger.level = optarg.log_level;

    rv = ogs_log_config_domain(
            ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    ogs_log_set_timestamp(ogs_app()->logger_default.timestamp,
                          ogs_app()->logger.timestamp);

    /**************************************************************************
     * Stage 5 : Setup Database Module
     */
    if (ogs_env_get("DB_URI"))
        ogs_app()->db_uri = ogs_env_get("DB_URI");

    /**************************************************************************
     * Stage 6 : Setup configuration section ID for running multiple NF from
     * same config file
     */
    if (optarg.config_section_id)
        ogs_app()->config_section_id = atoi(optarg.config_section_id);

    /**************************************************************************
     * Stage 7 : Print Banner
     */
    if (ogs_app()->version) {
        ogs_log_print(OGS_LOG_INFO,
                "Open5GS daemon %s\n\n", ogs_app()->version);

        ogs_info("Configuration: '%s'", ogs_app()->file);

        if (ogs_app()->logger.file) {
            ogs_info("File Logging: '%s'", ogs_app()->logger.file);

            if (ogs_app()->logger.level)
                ogs_info("LOG-LEVEL: '%s'", ogs_app()->logger.level);

            if (ogs_app()->logger.domain)
                ogs_info("LOG-DOMAIN: '%s'", ogs_app()->logger.domain);
        }
    }

    /**************************************************************************
     * Stage 8 : Queue, Timer and Poll
     */
    ogs_app()->queue = ogs_queue_create(ogs_app()->pool.event);
    ogs_assert(ogs_app()->queue);
    ogs_app()->timer_mgr = ogs_timer_mgr_create(ogs_app()->pool.timer);
    ogs_assert(ogs_app()->timer_mgr);
    ogs_app()->pollset = ogs_pollset_create(ogs_app()->pool.socket);
    ogs_assert(ogs_app()->pollset);

    return rv;
}

void ogs_app_terminate(void)
{
    ogs_app_config_final();
    ogs_app_context_final();

    ogs_pkbuf_default_destroy();

    ogs_core_terminate();
}

static int read_config(void)
{
    FILE *file;
    yaml_parser_t parser;
    yaml_document_t *document = NULL;

    ogs_assert(ogs_app()->file);

    file = fopen(ogs_app()->file, "rb");
    if (!file) {
        ogs_fatal("cannot open file `%s`", ogs_app()->file);
        return OGS_ERROR;
    }

    ogs_assert(yaml_parser_initialize(&parser));
    yaml_parser_set_input_file(&parser, file);

    document = calloc(1, sizeof(yaml_document_t));
    if (!yaml_parser_load(&parser, document)) {
        ogs_fatal("Failed to parse configuration file '%s'", ogs_app()->file);
        switch (parser.error) {
        case YAML_MEMORY_ERROR:
            ogs_error("Memory error: Not enough memory for parsing");
            break;
        case YAML_READER_ERROR:
            if (parser.problem_value != -1)
                ogs_error("Reader error - %s: #%X at %zd", parser.problem,
                    parser.problem_value, parser.problem_offset);
            else
                ogs_error("Reader error - %s at %zd", parser.problem,
                    parser.problem_offset);
            break;
        case YAML_SCANNER_ERROR:
            if (parser.context)
                ogs_error("Scanner error - %s at line %zu, column %zu "
                        "%s at line %zu, column %zu", parser.context,
                        parser.context_mark.line+1,
                        parser.context_mark.column+1,
                        parser.problem, parser.problem_mark.line+1,
                        parser.problem_mark.column+1);
            else
                ogs_error("Scanner error - %s at line %zu, column %zu",
                        parser.problem, parser.problem_mark.line+1,
                        parser.problem_mark.column+1);
            break;
        case YAML_PARSER_ERROR:
            if (parser.context)
                ogs_error("Parser error - %s at line %zu, column %zu "
                        "%s at line %zu, column %zu", parser.context,
                        parser.context_mark.line+1,
                        parser.context_mark.column+1,
                        parser.problem, parser.problem_mark.line+1,
                        parser.problem_mark.column+1);
            else
                ogs_error("Parser error - %s at line %zu, column %zu",
                        parser.problem, parser.problem_mark.line+1,
                        parser.problem_mark.column+1);
            break;
        default:
            /* Couldn't happen. */
            ogs_assert_if_reached();
            break;
        }

        free(document);
        yaml_parser_delete(&parser);
        ogs_assert(!fclose(file));
        return OGS_ERROR;
    }

    ogs_app()->document = document;

    yaml_parser_delete(&parser);
    ogs_assert(!fclose(file));

    return OGS_OK;
}

static int context_prepare(void)
{
    int rv;

#define USRSCTP_LOCAL_UDP_PORT      9899
    ogs_app()->usrsctp.udp_port = USRSCTP_LOCAL_UDP_PORT;

    rv = ogs_app_global_conf_prepare();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

static int context_validation(void)
{
    return OGS_OK;
}

static void parse_config_logger_file(ogs_yaml_iter_t *logger_iter,
                                     const char *logger_key)
{
    ogs_yaml_iter_t iter;

    /* Legacy format:
     *   logger:
     *     file: /var/log/open5gs/mme.log */
    if (!strcmp(logger_key, "file") && ogs_yaml_iter_has_value(logger_iter)) {
        ogs_app()->logger.file = ogs_yaml_iter_value(logger_iter);

        ogs_warn("Please change the configuration file as below.");
        ogs_log_print(OGS_LOG_WARN, "\n<OLD Format>\n");
        ogs_log_print(OGS_LOG_WARN, "logger:\n");
        ogs_log_print(OGS_LOG_WARN, "  file: %s\n", ogs_app()->logger.file);
        ogs_log_print(OGS_LOG_WARN, "\n<NEW Format>\n");
        ogs_log_print(OGS_LOG_WARN, "logger:\n");
        ogs_log_print(OGS_LOG_WARN, "  file:\n");
        ogs_log_print(OGS_LOG_WARN, "    path: %s\n", ogs_app()->logger.file);
        ogs_log_print(OGS_LOG_WARN, "\n\n\n");
        return;
    }

    /* Current format:
     *   logger:
     *     default:
     *       timestamp: false
     *     file:
     *       path: /var/log/open5gs/mme.log
     *       timestamp: true */
    ogs_yaml_iter_recurse(logger_iter, &iter);
    while (ogs_yaml_iter_next(&iter)) {
        const char *key = ogs_yaml_iter_key(&iter);
        ogs_assert(key);
        if (!strcmp(key, "timestamp")) {
            ogs_log_ts_e ts = ogs_yaml_iter_bool(&iter)
                              ? OGS_LOG_TS_ENABLED
                              : OGS_LOG_TS_DISABLED;
            if (!strcmp(logger_key, "default")) {
                ogs_app()->logger_default.timestamp = ts;
            } else if (!strcmp(logger_key, "file")) {
                ogs_app()->logger.timestamp = ts;
            }
        } else if (!strcmp(key, "path")) {
            if (!strcmp(logger_key, "file")) {
                ogs_app()->logger.file = ogs_yaml_iter_value(&iter);
            }
        }
    }
}

static int parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "db_uri")) {
            ogs_app()->db_uri = ogs_yaml_iter_value(&root_iter);
        } else if (!strcmp(root_key, "logger")) {
            ogs_yaml_iter_t logger_iter;
            ogs_yaml_iter_recurse(&root_iter, &logger_iter);
            while (ogs_yaml_iter_next(&logger_iter)) {
                const char *logger_key = ogs_yaml_iter_key(&logger_iter);
                ogs_assert(logger_key);
                parse_config_logger_file(&logger_iter, logger_key);
                if (!strcmp(logger_key, "level")) {
                    ogs_app()->logger.level =
                        ogs_yaml_iter_value(&logger_iter);
                } else if (!strcmp(logger_key, "domain")) {
                    ogs_app()->logger.domain =
                        ogs_yaml_iter_value(&logger_iter);
                }
            }
        } else if (!strcmp(root_key, "global")) {
            rv = ogs_app_parse_global_conf(&root_iter);
            if (rv != OGS_OK) {
                ogs_error("ogs_global_conf_parse_config() failed");
                return rv;
            }
        } else {
            rv = ogs_app_count_nf_conf_sections(root_key);
            if (rv != OGS_OK) {
                ogs_error("ogs_app_count_nf_conf_sections() failed");
                return rv;
            }
        }
    }

    rv = context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

void ogs_app_setup_log(void)
{
    ogs_log_install_domain(&__ogs_app_domain, "app", ogs_core()->log.level);
}
