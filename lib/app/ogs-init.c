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
    } optarg;

    ogs_core_initialize();
    ogs_app_setup_log();

    ogs_app_context_init();
    ogs_app()->version = version;

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
    ogs_pkbuf_default_create(&ogs_app()->pool.defconfig);

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

    /**************************************************************************
     * Stage 5 : Setup Database Module
     */
    if (ogs_env_get("DB_URI"))
        ogs_app()->db_uri = ogs_env_get("DB_URI");

    /**************************************************************************
     * Stage 6 : Print Banner
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
     * Stage 7 : Queue, Timer and Poll
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
                ogs_error("Scanner error - %s at line %zu, column %zu"
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
                ogs_error("Parser error - %s at line %zu, column %zu"
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

static void recalculate_pool_size(void)
{
    ogs_app()->pool.packet = ogs_app()->max.ue * OGS_MAX_NUM_OF_PACKET_BUFFER;

#define MAX_NUM_OF_TUNNEL       3   /* Num of Tunnel per Bearer */
    ogs_app()->pool.sess = ogs_app()->max.ue * OGS_MAX_NUM_OF_SESS;
    ogs_app()->pool.bearer = ogs_app()->pool.sess * OGS_MAX_NUM_OF_BEARER;
    ogs_app()->pool.tunnel = ogs_app()->pool.bearer * MAX_NUM_OF_TUNNEL;

#define POOL_NUM_PER_UE 16
    ogs_app()->pool.timer = ogs_app()->max.ue * POOL_NUM_PER_UE;
    ogs_app()->pool.message = ogs_app()->max.ue * POOL_NUM_PER_UE;
    ogs_app()->pool.event = ogs_app()->max.ue * POOL_NUM_PER_UE;
    ogs_app()->pool.socket = ogs_app()->max.ue * POOL_NUM_PER_UE;
    ogs_app()->pool.xact = ogs_app()->max.ue * POOL_NUM_PER_UE;
    ogs_app()->pool.stream = ogs_app()->max.ue * POOL_NUM_PER_UE;

    ogs_app()->pool.nf = ogs_app()->max.peer;
#define NF_SERVICE_PER_NF_INSTANCE 16
    ogs_app()->pool.nf_service = ogs_app()->pool.nf * NF_SERVICE_PER_NF_INSTANCE;
    ogs_app()->pool.subscription = ogs_app()->pool.nf * NF_SERVICE_PER_NF_INSTANCE;

    ogs_app()->pool.gtp_node = ogs_app()->pool.nf;
    if (ogs_app()->max.gtp_peer)
        ogs_app()->pool.gtp_node = ogs_app()->max.gtp_peer;

    /* Num of TAI-LAI Mapping Table */
    ogs_app()->pool.csmap = ogs_app()->pool.nf;

#define MAX_NUM_OF_IMPU         8
    ogs_app()->pool.impi = ogs_app()->max.ue;
    ogs_app()->pool.impu = ogs_app()->pool.impi * MAX_NUM_OF_IMPU;
}

static int context_prepare(void)
{
#define USRSCTP_LOCAL_UDP_PORT      9899
    ogs_app()->usrsctp.udp_port = USRSCTP_LOCAL_UDP_PORT;

    ogs_app()->sockopt.no_delay = true;

#define MAX_NUM_OF_UE               1024    /* Num of UEs */
#define MAX_NUM_OF_PEER             64      /* Num of Peer */

    ogs_app()->max.ue = MAX_NUM_OF_UE;
    ogs_app()->max.peer = MAX_NUM_OF_PEER;

    ogs_pkbuf_default_init(&ogs_app()->pool.defconfig);

    recalculate_pool_size();

    return OGS_OK;
}

static int context_validation(void)
{
    return OGS_OK;
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
                if (!strcmp(logger_key, "file")) {
                    ogs_app()->logger.file = ogs_yaml_iter_value(&logger_iter);
                } else if (!strcmp(logger_key, "level")) {
                    ogs_app()->logger.level =
                        ogs_yaml_iter_value(&logger_iter);
                } else if (!strcmp(logger_key, "domain")) {
                    ogs_app()->logger.domain =
                        ogs_yaml_iter_value(&logger_iter);
                }
            }
        } else if (!strcmp(root_key, "global")) {
            ogs_yaml_iter_t global_iter;
            ogs_yaml_iter_recurse(&root_iter, &global_iter);
            while (ogs_yaml_iter_next(&global_iter)) {
                const char *global_key = ogs_yaml_iter_key(&global_iter);
                ogs_assert(global_key);
                if (!strcmp(global_key, "parameter")) {
                    ogs_yaml_iter_t parameter_iter;
                    ogs_yaml_iter_recurse(&global_iter, &parameter_iter);
                    while (ogs_yaml_iter_next(&parameter_iter)) {
                        const char *parameter_key =
                            ogs_yaml_iter_key(&parameter_iter);
                        ogs_assert(parameter_key);
                        if (!strcmp(parameter_key, "no_hss")) {
                            ogs_app()->parameter.no_hss =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_mme")) {
                            ogs_app()->parameter.no_mme =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_sgwu")) {
                            ogs_app()->parameter.no_sgwu =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_sgwc")) {
                            ogs_app()->parameter.no_sgwc =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_sgw")) {
                            ogs_app()->parameter.no_sgw =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_pgw")) {
                            ogs_app()->parameter.no_pgw =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_pcrf")) {
                            ogs_app()->parameter.no_pcrf =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_nrf")) {
                            ogs_app()->parameter.no_nrf =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_scp")) {
                            ogs_app()->parameter.no_scp =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_sepp")) {
                            ogs_app()->parameter.no_sepp =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_amf")) {
                            ogs_app()->parameter.no_amf =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_smf")) {
                            ogs_app()->parameter.no_smf =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_upf")) {
                            ogs_app()->parameter.no_upf =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_ausf")) {
                            ogs_app()->parameter.no_ausf =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_udm")) {
                            ogs_app()->parameter.no_udm =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_pcf")) {
                            ogs_app()->parameter.no_pcf =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_nssf")) {
                            ogs_app()->parameter.no_nssf =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_bsf")) {
                            ogs_app()->parameter.no_bsf =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_udr")) {
                            ogs_app()->parameter.no_udr =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_ipv4")) {
                            ogs_app()->parameter.no_ipv4 =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "no_ipv6")) {
                            ogs_app()->parameter.no_ipv6 =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "prefer_ipv4")) {
                            ogs_app()->parameter.prefer_ipv4 =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "multicast")) {
                            ogs_app()->parameter.multicast =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key, "use_openair")) {
                            ogs_app()->parameter.use_openair =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key,
                                    "no_ipv4v6_local_addr_in_packet_filter")) {
                            ogs_app()->parameter.
                                no_ipv4v6_local_addr_in_packet_filter =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else if (!strcmp(parameter_key,
                                    "no_pfcp_rr_select")) {
                            ogs_app()->parameter.no_pfcp_rr_select =
                                ogs_yaml_iter_bool(&parameter_iter);
                        } else
                            ogs_warn("unknown key `%s`", parameter_key);
                    }
                } else if (!strcmp(global_key, "sockopt")) {
                    ogs_yaml_iter_t sockopt_iter;
                    ogs_yaml_iter_recurse(&global_iter, &sockopt_iter);
                    while (ogs_yaml_iter_next(&sockopt_iter)) {
                        const char *sockopt_key =
                            ogs_yaml_iter_key(&sockopt_iter);
                        ogs_assert(sockopt_key);
                        if (!strcmp(sockopt_key, "no_delay")) {
                            ogs_app()->sockopt.no_delay =
                                ogs_yaml_iter_bool(&sockopt_iter);
                        } else if (!strcmp(sockopt_key, "linger")) {
                            const char *v = ogs_yaml_iter_value(&sockopt_iter);
                            if (v) ogs_app()->sockopt.l_linger = atoi(v);
                            ogs_app()->sockopt.l_onoff = true;
                        } else
                            ogs_warn("unknown key `%s`", sockopt_key);
                    }
                } else if (!strcmp(global_key, "max")) {
                    ogs_yaml_iter_t max_iter;
                    ogs_yaml_iter_recurse(&global_iter, &max_iter);
                    while (ogs_yaml_iter_next(&max_iter)) {
                        const char *max_key = ogs_yaml_iter_key(&max_iter);
                        ogs_assert(max_key);
                        if (!strcmp(max_key, "ue")) {
                            const char *v = ogs_yaml_iter_value(&max_iter);
                            if (v) ogs_app()->max.ue = atoi(v);
                        } else if (!strcmp(max_key, "peer") ||
                                    !strcmp(max_key, "enb")) {
                            const char *v = ogs_yaml_iter_value(&max_iter);
                            if (v) ogs_app()->max.peer = atoi(v);
                        } else if (!strcmp(max_key, "gtp_peer") ||
                                    !strcmp(max_key, "enb")) {
                            const char *v = ogs_yaml_iter_value(&max_iter);
                            if (v) ogs_app()->max.gtp_peer = atoi(v);
                        } else
                            ogs_warn("unknown key `%s`", max_key);
                    }

                    recalculate_pool_size();
                } else if (!strcmp(global_key, "pool")) {
                    ogs_yaml_iter_t pool_iter;
                    ogs_yaml_iter_recurse(&global_iter, &pool_iter);
                    while (ogs_yaml_iter_next(&pool_iter)) {
                        const char *pool_key = ogs_yaml_iter_key(&pool_iter);
                        ogs_assert(pool_key);
                        if (!strcmp(pool_key, "128")) {
                            const char *v = ogs_yaml_iter_value(&pool_iter);
                            if (v)
                                ogs_app()->pool.defconfig.cluster_128_pool =
                                    atoi(v);
                        } else if (!strcmp(pool_key, "256")) {
                            const char *v = ogs_yaml_iter_value(&pool_iter);
                            if (v)
                                ogs_app()->pool.defconfig.cluster_256_pool =
                                    atoi(v);
                        } else if (!strcmp(pool_key, "512")) {
                            const char *v = ogs_yaml_iter_value(&pool_iter);
                            if (v)
                                ogs_app()->pool.defconfig.cluster_512_pool =
                                    atoi(v);
                        } else if (!strcmp(pool_key, "1024")) {
                            const char *v = ogs_yaml_iter_value(&pool_iter);
                            if (v)
                                ogs_app()->pool.defconfig.cluster_1024_pool =
                                    atoi(v);
                        } else if (!strcmp(pool_key, "2048")) {
                            const char *v = ogs_yaml_iter_value(&pool_iter);
                            if (v)
                                ogs_app()->pool.defconfig.cluster_2048_pool =
                                    atoi(v);
                        } else if (!strcmp(pool_key, "8192")) {
                            const char *v = ogs_yaml_iter_value(&pool_iter);
                            if (v)
                                ogs_app()->pool.defconfig.cluster_8192_pool =
                                    atoi(v);
                        } else if (!strcmp(pool_key, "32768")) {
                            const char *v = ogs_yaml_iter_value(&pool_iter);
                            if (v)
                                ogs_app()->pool.defconfig.cluster_32768_pool =
                                    atoi(v);
                        } else if (!strcmp(pool_key, "big")) {
                            const char *v = ogs_yaml_iter_value(&pool_iter);
                            if (v)
                                ogs_app()->pool.defconfig.cluster_big_pool =
                                    atoi(v);
                        } else
                            ogs_warn("unknown key `%s`", pool_key);
                    }
                }
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
