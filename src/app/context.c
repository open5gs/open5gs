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
        switch (parser.error)
        {
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
        }
        else if (!strcmp(root_key, "logger")) {
            ogs_yaml_iter_t logger_iter;
            ogs_yaml_iter_recurse(&root_iter, &logger_iter);
            while (ogs_yaml_iter_next(&logger_iter)) {
                const char *logger_key = ogs_yaml_iter_key(&logger_iter);
                ogs_assert(logger_key);
                if (!strcmp(logger_key, "file")) {
                    self.config.logger.file = ogs_yaml_iter_value(&logger_iter);
                }
                else if (!strcmp(logger_key, "level")) {
                    self.config.logger.level =
                        ogs_yaml_iter_value(&logger_iter);
                }
                else if (!strcmp(logger_key, "domain")) {
                    self.config.logger.domain =
                        ogs_yaml_iter_value(&logger_iter);
                }
            }
        }
        else if (!strcmp(root_key, "parameter"))
        {
            ogs_yaml_iter_t parameter_iter;
            ogs_yaml_iter_recurse(&root_iter, &parameter_iter);
            while (ogs_yaml_iter_next(&parameter_iter)) {
                const char *parameter_key = ogs_yaml_iter_key(&parameter_iter);
                ogs_assert(parameter_key);
                if (!strcmp(parameter_key, "no_hss"))
                {
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
                } else if (!strcmp(parameter_key, "sctp_streams")) {
                    const char *v = ogs_yaml_iter_value(&parameter_iter);
                    if (v) self.config.parameter.sctp_streams = atoi(v);
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
