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

static ogs_config_t self;

static int initialized = 0;

int ogs_config_init()
{
    ogs_assert(initialized == 0);

    memset(&self, 0, sizeof(ogs_config_t));

    initialized = 1;

    return OGS_OK;
}

int ogs_config_final()
{
    ogs_assert(initialized == 1);

    if (self.document) {
        yaml_document_delete(self.document);
        free(self.document);
    }

    initialized = 0;

    return OGS_OK;
}

ogs_config_t *ogs_config()
{
    return &self;
}

int ogs_config_read()
{
    ogs_config_t *config = &self;
    FILE *file;
    yaml_parser_t parser;
    yaml_document_t *document = NULL;

    ogs_assert(config->file);

    file = fopen(config->file, "rb");
    if (!file) {
        ogs_fatal("cannot open file `%s`", config->file);
        return OGS_ERROR;
    }

    ogs_assert(yaml_parser_initialize(&parser));
    yaml_parser_set_input_file(&parser, file);

    document = calloc(1, sizeof(yaml_document_t));
    if (!yaml_parser_load(&parser, document)) {
        ogs_fatal("Failed to parse configuration file '%s'", config->file);
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

    config->document = document;

    yaml_parser_delete(&parser);
    ogs_assert(!fclose(file));

    return OGS_OK;
}

static void recalculate_pool_size(void)
{
#define MAX_NUM_OF_BEARER       4   /* Num of Bearer per APN(Session) */
#define MAX_NUM_OF_TUNNEL       3   /* Num of Tunnel per Bearer */
#define MAX_NUM_OF_PF           16  /* Num of PacketFilter per Bearer */

    self.pool.ue = self.max.ue * self.max.enb;
    self.pool.pfcp = ogs_max(self.max.smf, self.max.upf);
    self.pool.sess = self.pool.ue * OGS_MAX_NUM_OF_SESS;
    self.pool.bearer = self.pool.sess * MAX_NUM_OF_BEARER;
    self.pool.tunnel = self.pool.bearer * MAX_NUM_OF_TUNNEL;
    self.pool.pf = self.pool.bearer * MAX_NUM_OF_PF;
}

static int config_prepare(void)
{
#define USRSCTP_LOCAL_UDP_PORT      9899
    self.usrsctp.udp_port = USRSCTP_LOCAL_UDP_PORT;

#define MAX_NUM_OF_SGW              32  /* Num of SGW per MME */
#define MAX_NUM_OF_PGW              32  /* Num of PGW per MME */
#define MAX_NUM_OF_VLR              32  /* Num of VLR per MME */
#define MAX_NUM_OF_CSMAP            128 /* Num of TAI-LAI MAP per MME */
#define MAX_NUM_OF_ENB              32  /* Num of eNodeB per MME */
#define MAX_NUM_OF_UE               128 /* Num of UE per eNodeB */
#define MAX_NUM_OF_SMF              32  /* Num of SMF per AMF */
#define MAX_NUM_OF_UPF              32  /* Num of PGW per AMF */
    self.max.sgw = MAX_NUM_OF_SGW;
    self.max.pgw = MAX_NUM_OF_PGW;
    self.max.vlr = MAX_NUM_OF_VLR;
    self.max.csmap = MAX_NUM_OF_CSMAP;
    self.max.enb = MAX_NUM_OF_ENB;
    self.max.ue = MAX_NUM_OF_UE;
    self.max.smf = MAX_NUM_OF_SMF;
    self.max.upf = MAX_NUM_OF_UPF;

#define MAX_NUM_OF_PACKET_POOL      65536
    self.pool.packet = MAX_NUM_OF_PACKET_POOL;

    ogs_pkbuf_default_init(&self.pool.defconfig);

    recalculate_pool_size();

    return OGS_OK;
}

static int ogs_app_ctx_validation(void)
{
    if (self.parameter.no_ipv4 == 1 &&
        self.parameter.no_ipv6 == 1) {
        ogs_error("Both `no_ipv4` and `no_ipv6` set to `true` in `%s`",
                self.file);
        return OGS_ERROR;
    }

    return OGS_OK;
}
int ogs_config_parse()
{
    int rv;
    ogs_config_t *config = &self;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    ogs_assert(config);
    document = config->document;
    ogs_assert(document);

    rv = config_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "db_uri")) {
            self.db_uri = ogs_yaml_iter_value(&root_iter);
        } else if (!strcmp(root_key, "logger")) {
            ogs_yaml_iter_t logger_iter;
            ogs_yaml_iter_recurse(&root_iter, &logger_iter);
            while (ogs_yaml_iter_next(&logger_iter)) {
                const char *logger_key = ogs_yaml_iter_key(&logger_iter);
                ogs_assert(logger_key);
                if (!strcmp(logger_key, "file")) {
                    self.logger.file = ogs_yaml_iter_value(&logger_iter);
                } else if (!strcmp(logger_key, "level")) {
                    self.logger.level =
                        ogs_yaml_iter_value(&logger_iter);
                } else if (!strcmp(logger_key, "domain")) {
                    self.logger.domain =
                        ogs_yaml_iter_value(&logger_iter);
                }
            }
        } else if (!strcmp(root_key, "parameter")) {
            ogs_yaml_iter_t parameter_iter;
            ogs_yaml_iter_recurse(&root_iter, &parameter_iter);
            while (ogs_yaml_iter_next(&parameter_iter)) {
                const char *parameter_key = ogs_yaml_iter_key(&parameter_iter);
                ogs_assert(parameter_key);
                if (!strcmp(parameter_key, "no_hss")) {
                    self.parameter.no_hss =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_sgw")) {
                    self.parameter.no_sgw =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_pgw")) {
                    self.parameter.no_pgw =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_pcrf")) {
                    self.parameter.no_pcrf =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_ipv4")) {
                    self.parameter.no_ipv4 =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_ipv6")) {
                    self.parameter.no_ipv6 =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "prefer_ipv4")) {
                    self.parameter.prefer_ipv4 =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "multicast")) {
                    self.parameter.multicast =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_slaac")) {
                    self.parameter.no_slaac =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else
                    ogs_warn("unknown key `%s`", parameter_key);
            }
        } else if (!strcmp(root_key, "sctp")) {
            ogs_yaml_iter_t sctp_iter;
            ogs_yaml_iter_recurse(&root_iter, &sctp_iter);
            while (ogs_yaml_iter_next(&sctp_iter)) {
                const char *sctp_key = ogs_yaml_iter_key(&sctp_iter);
                ogs_assert(sctp_key);
                if (!strcmp(sctp_key, "heartbit_interval")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) self.sockopt.sctp.heartbit_interval = atoi(v);
                } else if (!strcmp(sctp_key, "rto_initial")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) self.sockopt.sctp.rto_initial = atoi(v);
                } else if (!strcmp(sctp_key, "rto_min")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) self.sockopt.sctp.rto_min = atoi(v);
                } else if (!strcmp(sctp_key, "rto_max")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) self.sockopt.sctp.rto_max = atoi(v);
                } else if (!strcmp(sctp_key, "max_num_of_ostreams")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v)
                        self.sockopt.sctp.max_num_of_ostreams = atoi(v);
                } else if (!strcmp(sctp_key, "max_num_of_istreams")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v)
                        self.sockopt.sctp.max_num_of_istreams = atoi(v);
                } else if (!strcmp(sctp_key, "max_attempts")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) self.sockopt.sctp.max_attempts = atoi(v);
                } else if (!strcmp(sctp_key, "max_initial_timeout")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v)
                        self.sockopt.sctp.max_initial_timeout = atoi(v);
                } else if (!strcmp(sctp_key, "usrsctp_udp_port")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) self.usrsctp.udp_port = atoi(v);
                } else
                    ogs_warn("unknown key `%s`", sctp_key);
            }
        } else if (!strcmp(root_key, "max")) {
            ogs_yaml_iter_t max_iter;
            ogs_yaml_iter_recurse(&root_iter, &max_iter);
            while (ogs_yaml_iter_next(&max_iter)) {
                const char *max_key = ogs_yaml_iter_key(&max_iter);
                ogs_assert(max_key);
                if (!strcmp(max_key, "ue")) {
                    const char *v = ogs_yaml_iter_value(&max_iter);
                    if (v) self.max.ue = atoi(v);
                } else if (!strcmp(max_key, "enb")) {
                    const char *v = ogs_yaml_iter_value(&max_iter);
                    if (v) self.max.enb = atoi(v);
                } else
                    ogs_warn("unknown key `%s`", max_key);
            }

            recalculate_pool_size();
        } else if (!strcmp(root_key, "pool")) {
            ogs_yaml_iter_t pool_iter;
            ogs_yaml_iter_recurse(&root_iter, &pool_iter);
            while (ogs_yaml_iter_next(&pool_iter)) {
                const char *pool_key = ogs_yaml_iter_key(&pool_iter);
                ogs_assert(pool_key);
                if (!strcmp(pool_key, "128")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v)
                        self.pool.defconfig.cluster_128_pool = atoi(v);
                } else if (!strcmp(pool_key, "256")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v)
                        self.pool.defconfig.cluster_256_pool = atoi(v);
                } else if (!strcmp(pool_key, "512")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v)
                        self.pool.defconfig.cluster_512_pool = atoi(v);
                } else if (!strcmp(pool_key, "1024")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v)
                        self.pool.defconfig.cluster_1024_pool = atoi(v);
                } else if (!strcmp(pool_key, "2048")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v)
                        self.pool.defconfig.cluster_2048_pool = atoi(v);
                } else if (!strcmp(pool_key, "8192")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v)
                        self.pool.defconfig.cluster_8192_pool = atoi(v);
                } else if (!strcmp(pool_key, "big")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v)
                        self.pool.defconfig.cluster_big_pool = atoi(v);
                } else if (!strcmp(pool_key, "packet")) {
                    const char *v = ogs_yaml_iter_value(&pool_iter);
                    if (v)
                        self.pool.packet = atoi(v);
                } else
                    ogs_warn("unknown key `%s`", pool_key);
            }
        }
            
    }

    rv = ogs_app_ctx_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}
