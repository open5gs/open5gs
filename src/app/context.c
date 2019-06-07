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

#include <mongoc.h>
#include <yaml.h>

#include "context.h"

#define DEFAULT_SCTP_STREAMS        30

static context_t self;

static int context_initialized = 0;

int context_init()
{
    ogs_assert(context_initialized == 0);

    memset(&self, 0, sizeof(context_t));

    context_initialized = 1;

    return OGS_OK;
}

int context_final()
{
    ogs_assert(context_initialized == 1);

    if (self.config.document) {
        yaml_document_delete(self.config.document);
        ogs_free(self.config.document);
    }

    context_initialized = 0;

    return OGS_OK;
}

context_t* context_self()
{
    return &self;
}

int context_read_file()
{
    config_t *config = &self.config;
    FILE *file;
    yaml_parser_t parser;
    yaml_document_t *document = NULL;

    ogs_assert(config->path);

    file = fopen(config->path, "rb");
    if (!file) {
        ogs_fatal("cannot open file `%s`", config->path);
        ogs_assert_if_reached();
    }

    ogs_assert(yaml_parser_initialize(&parser));
    yaml_parser_set_input_file(&parser, file);

    document = ogs_calloc(1, sizeof(yaml_document_t));
    if (!yaml_parser_load(&parser, document)) {
        ogs_fatal("Failed to parse configuration file '%s'", config->path);
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

        ogs_free(document);
        yaml_parser_delete(&parser);
        ogs_assert(!fclose(file));
        return OGS_ERROR;
    }

    config->document = document;

    yaml_parser_delete(&parser);
    ogs_assert(!fclose(file));

    return OGS_OK;
}

int context_setup_log_module()
{
    if (context_self()->log.level || context_self()->log.domain)
        ogs_log_set_mask_level(context_self()->log.domain,
                context_self()->log.level);

    return OGS_OK;
}

static int context_prepare()
{
#define USRSCTP_LOCAL_UDP_PORT      9899
    self.config.usrsctp.udp_port = USRSCTP_LOCAL_UDP_PORT;

#define MAX_NUM_OF_ENB              32  /* Num of eNodeB per MME */
#define MAX_NUM_OF_UE               128 /* Num of UE per eNodeB */
#define MAX_NUM_OF_SESS             4   /* Num of APN(Session) per UE */
#define MAX_NUM_OF_BEARER           4   /* Num of Bearer per APN(Session) */
#define MAX_NUM_OF_TUNNEL           3   /* Num of Tunnel per Bearer */
#define MAX_NUM_OF_PF               16  /* Num of Packet Filter per Bearer */
#define MAX_NUM_OF_PACKET_BUFFER    512 /* Num of Buffer when S1-U Deactivated */
#define MAX_NUM_OF_PACKET_POOL      131072 /* Num of 8192-packet pool in SGW */
    self.config.max.enb = MAX_NUM_OF_ENB;
    self.config.max.ue = MAX_NUM_OF_UE;
    self.config.max.sess = MAX_NUM_OF_SESS;
    self.config.max.bearer = MAX_NUM_OF_BEARER;
    self.config.max.tunnel = MAX_NUM_OF_TUNNEL;
    self.config.max.pf = MAX_NUM_OF_PF;
    self.config.max.packet.buffer = MAX_NUM_OF_PACKET_BUFFER;
    self.config.max.packet.pool = MAX_NUM_OF_PACKET_POOL;

    return OGS_OK;
}

static int context_validation()
{
    if (self.config.parameter.no_ipv4 == 1 &&
        self.config.parameter.no_ipv6 == 1) {
        ogs_error("Both `no_ipv4` and `no_ipv6` set to `true` in `%s`",
                context_self()->config.path);
        return OGS_ERROR;
    }

    self.pool.ue = self.config.max.ue * self.config.max.enb;
    self.pool.sess = self.pool.ue *  self.config.max.sess;
    self.pool.bearer = self.pool.sess * self.config.max.bearer;
    self.pool.tunnel = self.pool.bearer * self.config.max.tunnel;
    self.pool.pf = self.pool.bearer * self.config.max.pf;
    self.pool.diameter_sess = self.pool.ue * self.config.max.sess;

    return OGS_OK;
}
int context_parse_config()
{
    int rv;
    config_t *config = &self.config;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;

    ogs_assert(config);
    document = config->document;
    ogs_assert(document);

    rv = context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "db_uri")) {
            self.config.db_uri = ogs_yaml_iter_value(&root_iter);
        } else if (!strcmp(root_key, "logger")) {
            ogs_yaml_iter_t logger_iter;
            ogs_yaml_iter_recurse(&root_iter, &logger_iter);
            while (ogs_yaml_iter_next(&logger_iter)) {
                const char *logger_key = ogs_yaml_iter_key(&logger_iter);
                ogs_assert(logger_key);
                if (!strcmp(logger_key, "file")) {
                    self.config.logger.file = ogs_yaml_iter_value(&logger_iter);
                } else if (!strcmp(logger_key, "level")) {
                    self.config.logger.level =
                        ogs_yaml_iter_value(&logger_iter);
                } else if (!strcmp(logger_key, "domain")) {
                    self.config.logger.domain =
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
                    self.config.parameter.no_hss =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_sgw")) {
                    self.config.parameter.no_sgw =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_pgw")) {
                    self.config.parameter.no_pgw =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_pcrf")) {
                    self.config.parameter.no_pcrf =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_ipv4")) {
                    self.config.parameter.no_ipv4 =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_ipv6")) {
                    self.config.parameter.no_ipv6 =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "prefer_ipv4")) {
                    self.config.parameter.prefer_ipv4 =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "multicast")) {
                    self.config.parameter.multicast =
                        ogs_yaml_iter_bool(&parameter_iter);
                } else if (!strcmp(parameter_key, "no_slaac")) {
                    self.config.parameter.no_slaac =
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
                    if (v) self.config.sockopt.sctp.heartbit_interval = atoi(v);
                } else if (!strcmp(sctp_key, "rto_initial")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) self.config.sockopt.sctp.rto_initial = atoi(v);
                } else if (!strcmp(sctp_key, "rto_min")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) self.config.sockopt.sctp.rto_min = atoi(v);
                } else if (!strcmp(sctp_key, "rto_max")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) self.config.sockopt.sctp.rto_max = atoi(v);
                } else if (!strcmp(sctp_key, "max_num_of_ostreams")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v)
                        self.config.sockopt.sctp.max_num_of_ostreams = atoi(v);
                } else if (!strcmp(sctp_key, "max_num_of_istreams")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v)
                        self.config.sockopt.sctp.max_num_of_istreams = atoi(v);
                } else if (!strcmp(sctp_key, "max_attempts")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) self.config.sockopt.sctp.max_attempts = atoi(v);
                } else if (!strcmp(sctp_key, "max_initial_timeout")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v)
                        self.config.sockopt.sctp.max_initial_timeout = atoi(v);
                } else if (!strcmp(sctp_key, "usrsctp_udp_port")) {
                    const char *v = ogs_yaml_iter_value(&sctp_iter);
                    if (v) self.config.usrsctp.udp_port = atoi(v);
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
                    if (v) self.config.max.ue = atoi(v);
                } else if (!strcmp(max_key, "enb")) {
                    const char *v = ogs_yaml_iter_value(&max_iter);
                    if (v) self.config.max.enb = atoi(v);
                } else if (!strcmp(max_key, "sess")) {
                    const char *v = ogs_yaml_iter_value(&max_iter);
                    if (v) self.config.max.sess = atoi(v);
                } else if (!strcmp(max_key, "bearer")) {
                    const char *v = ogs_yaml_iter_value(&max_iter);
                    if (v) self.config.max.bearer = atoi(v);
                } else if (!strcmp(max_key, "tunnel")) {
                    const char *v = ogs_yaml_iter_value(&max_iter);
                    if (v) self.config.max.tunnel = atoi(v);
                } else if (!strcmp(max_key, "pf")) {
                    const char *v = ogs_yaml_iter_value(&max_iter);
                    if (v) self.config.max.pf = atoi(v);
                } else if (!strcmp(max_key, "packet")) {
                    ogs_yaml_iter_t packet_iter;
                    ogs_yaml_iter_recurse(&max_iter, &packet_iter);
                    while (ogs_yaml_iter_next(&packet_iter)) {
                        const char *packet_key = ogs_yaml_iter_key(&packet_iter);
                        ogs_assert(packet_key);
                        if (!strcmp(packet_key, "buffer")) {
                            const char *v = ogs_yaml_iter_value(&packet_iter);
                            if (v) self.config.max.packet.buffer = atoi(v);
                        } else if (!strcmp(packet_key, "pool")) {
                            const char *v = ogs_yaml_iter_value(&packet_iter);
                            if (v) self.config.max.packet.pool = atoi(v);
                        } else
                            ogs_warn("unknown key `%s`", packet_key);
                    }
                } else
                    ogs_warn("unknown key `%s`", max_key);
            }
        }
            
    }

    rv = context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}

/*
 * We've added it 
 * Because the following function is deprecated in the mongo-c-driver
 */
static bool
context_mongoc_client_get_server_status (mongoc_client_t *client, /* IN */
                                 mongoc_read_prefs_t *read_prefs, /* IN */
                                 bson_t *reply,                   /* OUT */
                                 bson_error_t *error)             /* OUT */
{
   bson_t cmd = BSON_INITIALIZER;
   bool ret = false;

   BSON_ASSERT (client);

   BSON_APPEND_INT32 (&cmd, "serverStatus", 1);
   ret = mongoc_client_command_simple (
      client, "admin", &cmd, read_prefs, reply, error);
   bson_destroy (&cmd);

   return ret;
}

int context_db_init(const char *db_uri)
{
    bson_t reply;
    bson_error_t error;
    bson_iter_t iter;

    const mongoc_uri_t *uri;

    memset(&self.db, 0, sizeof self.db);

    mongoc_init();
    self.db.initialized = true;

    self.db.client = mongoc_client_new(db_uri);
    if (!self.db.client) {
        ogs_error("Failed to parse DB URI [%s]", db_uri);
        return OGS_ERROR;
    }

#if MONGOC_MAJOR_VERSION >= 1 && MONGOC_MINOR_VERSION >= 4
    mongoc_client_set_error_api(self.db.client, 2);
#endif

    uri = mongoc_client_get_uri(self.db.client);
    ogs_assert(uri);

    self.db.name = mongoc_uri_get_database(uri);
    ogs_assert(self.db.name);

    self.db.database = mongoc_client_get_database(self.db.client, self.db.name);
    ogs_assert(self.db.database);

    if (!context_mongoc_client_get_server_status(
                self.db.client, NULL, &reply, &error)) {
        ogs_error("Failed to connect to server [%s]", db_uri);
        return OGS_RETRY;
    }

    ogs_assert(bson_iter_init_find(&iter, &reply, "ok"));

    bson_destroy(&reply);
    return OGS_OK;
}

int context_db_final()
{
    if (self.db.database) {
        mongoc_database_destroy(self.db.database);
        self.db.database = NULL;
    }
    if (self.db.client) {
        mongoc_client_destroy(self.db.client);
        self.db.client = NULL;
    }

    if (self.db.initialized) {
        mongoc_cleanup();
        self.db.initialized = false;
    }

    return OGS_OK;
}
