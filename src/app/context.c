#define TRACE_MODULE _context

#include "core_file.h"
#include "core_debug.h"
#include "core_lib.h"
#include "core_pkbuf.h"

#include <mongoc.h>
#include <yaml.h>
#include "yaml_helper.h"

#include "context.h"

#define DEFAULT_SCTP_STREAMS        30

static context_t self;

static int context_initialized = 0;

status_t context_init()
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "Context already has been context_initialized");

    memset(&self, 0, sizeof(context_t));

    context_initialized = 1;

    return CORE_OK;
}

status_t context_final()
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "Context already has been finalized");

    if (self.config.document)
    {
        yaml_document_delete(self.config.document);
        CORE_FREE(self.config.document);
    }

    context_initialized = 0;

    return CORE_OK;
}

context_t* context_self()
{
    return &self;
}

status_t context_read_file()
{
    config_t *config = &self.config;
    FILE *file;
    yaml_parser_t parser;
    yaml_document_t *document = NULL;

    d_assert(config->path, return CORE_ERROR,);

    file = fopen(config->path, "rb");
    d_assert(file, return CORE_ERROR,
            "Failed to read configuration file `%s`", config->path);

    d_assert(yaml_parser_initialize(&parser), return CORE_ERROR,);
    yaml_parser_set_input_file(&parser, file);

    document = core_calloc(1, sizeof(yaml_document_t));
    if (!yaml_parser_load(&parser, document))
    {
        d_fatal("Failed to parse configuration file '%s'", config->path);
        switch (parser.error)
        {
            case YAML_MEMORY_ERROR:
                d_error("Memory error: Not enough memory for parsing");
                break;
            case YAML_READER_ERROR:
                if (parser.problem_value != -1)
                    d_error("Reader error - %s: #%X at %zd", parser.problem,
                        parser.problem_value, parser.problem_offset);
                else
                    d_error("Reader error - %s at %zd", parser.problem,
                        parser.problem_offset);
                break;
            case YAML_SCANNER_ERROR:
                if (parser.context)
                    d_error("Scanner error - %s at line %lu, column %lu\n"
                            "%s at line %lu, column %lu", parser.context,
                            parser.context_mark.line+1,
                            parser.context_mark.column+1,
                            parser.problem, parser.problem_mark.line+1,
                            parser.problem_mark.column+1);
                else
                    d_error("Scanner error - %s at line %lu, column %lu",
                            parser.problem, parser.problem_mark.line+1,
                            parser.problem_mark.column+1);
                break;
            case YAML_PARSER_ERROR:
                if (parser.context)
                    d_error("Parser error - %s at line %lu, column %lu\n"
                            "%s at line %lu, column %lu", parser.context,
                            parser.context_mark.line+1,
                            parser.context_mark.column+1,
                            parser.problem, parser.problem_mark.line+1,
                            parser.problem_mark.column+1);
                else
                    d_error("Parser error - %s at line %lu, column %lu",
                            parser.problem, parser.problem_mark.line+1,
                            parser.problem_mark.column+1);
                break;
            default:
                /* Couldn't happen. */
                d_assert(0,, "Internal error");
                break;
        }

        CORE_FREE(document);
        yaml_parser_delete(&parser);
        d_assert(!fclose(file),,);
        return CORE_ERROR;
    }

    config->document = document;

    yaml_parser_delete(&parser);
    d_assert(!fclose(file),,);

    return CORE_OK;
}

status_t context_setup_trace_module()
{
    int app = context_self()->logger.trace.app;

    if (app)
    {
        extern int _mutex;
        d_trace_level(&_mutex, app);
        extern int _pkbuf;
        d_trace_level(&_pkbuf, app);
        extern int _timer;
        d_trace_level(&_timer, app);

        extern int _context;
        d_trace_level(&_context, app);
    }

    return CORE_OK;
}

static status_t context_prepare()
{
    self.logger.console = -1;
    self.parameter.sctp_streams = DEFAULT_SCTP_STREAMS;

    return CORE_OK;
}

static status_t context_validation()
{
    if (self.parameter.no_ipv4 == 1 && self.parameter.no_ipv6 == 1)
    {
        d_error("Both `no_ipv4` and `no_ipv6` set to `true` in `%s`",
                context_self()->config.path);
        return CORE_ERROR;
    }

    return CORE_OK;
}
status_t context_parse_config()
{
    status_t rv;
    config_t *config = &self.config;
    yaml_document_t *document = NULL;
    yaml_iter_t root_iter;

    d_assert(config, return CORE_ERROR,);
    document = config->document;
    d_assert(document, return CORE_ERROR,);

    rv = context_prepare();
    if (rv != CORE_OK) return rv;

    yaml_iter_init(&root_iter, document);
    while(yaml_iter_next(&root_iter))
    {
        const char *root_key = yaml_iter_key(&root_iter);
        d_assert(root_key, return CORE_ERROR,);
        if (!strcmp(root_key, "db_uri"))
        {
            self.db_uri = yaml_iter_value(&root_iter);
        }
        else if (!strcmp(root_key, "logger"))
        {
            yaml_iter_t logger_iter;
            yaml_iter_recurse(&root_iter, &logger_iter);
            while(yaml_iter_next(&logger_iter))
            {
                const char *logger_key = yaml_iter_key(&logger_iter);
                d_assert(logger_key, return CORE_ERROR,);
                if (!strcmp(logger_key, "file"))
                {
                    self.logger.file = yaml_iter_value(&logger_iter);
                }
                else if (!strcmp(logger_key, "console"))
                {
                    const char *v = yaml_iter_value(&logger_iter);
                    if (v) self.logger.console = atoi(v);
                }
                else if (!strcmp(logger_key, "syslog"))
                {
                    self.logger.syslog = yaml_iter_value(&logger_iter);
                }
                else if (!strcmp(logger_key, "network"))
                {
                    yaml_iter_t network_iter;
                    yaml_iter_recurse(&logger_iter, &network_iter);
                    while(yaml_iter_next(&network_iter))
                    {
                        const char *network_key = yaml_iter_key(&network_iter);
                        d_assert(network_key, return CORE_ERROR,);
                        if (!strcmp(network_key, "file"))
                        {
                            self.logger.network.file = 
                                yaml_iter_value(&network_iter);
                        }
                        else if (!strcmp(network_key, "unixDomain"))
                        {
                            self.logger.network.unix_domain = 
                                yaml_iter_value(&network_iter);
                        }
                        else
                            d_warn("unknown key `%s`", network_key);
                    }
                }
                else if (!strcmp(logger_key, "trace"))
                {
                    yaml_iter_t trace_iter;
                    yaml_iter_recurse(&logger_iter, &trace_iter);
                    while(yaml_iter_next(&trace_iter))
                    {
                        const char *trace_key = yaml_iter_key(&trace_iter);
                        d_assert(trace_key, return CORE_ERROR,);
                        if (!strcmp(trace_key, "app"))
                        {
                            const char *v = yaml_iter_value(&trace_iter);
                            if (v) self.logger.trace.app = atoi(v);
                        }
                        else if (!strcmp(trace_key, "s1ap"))
                        {
                            const char *v = yaml_iter_value(&trace_iter);
                            if (v) self.logger.trace.s1ap = atoi(v);
                        }
                        else if (!strcmp(trace_key, "nas"))
                        {
                            const char *v = yaml_iter_value(&trace_iter);
                            if (v) self.logger.trace.nas = atoi(v);
                        }
                        else if (!strcmp(trace_key, "diameter"))
                        {
                            const char *v = yaml_iter_value(&trace_iter);
                            if (v) self.logger.trace.diameter = atoi(v);
                        }
                        else if (!strcmp(trace_key, "gtpv2"))
                        {
                            const char *v = yaml_iter_value(&trace_iter);
                            if (v) self.logger.trace.gtpv2 = atoi(v);
                        }
                        else if (!strcmp(trace_key, "gtp"))
                        {
                            const char *v = yaml_iter_value(&trace_iter);
                            if (v) self.logger.trace.gtp = atoi(v);
                        }
                        else
                            d_warn("unknown key `%s`", trace_key);
                    }
                }
            }
        }
        else if (!strcmp(root_key, "parameter"))
        {
            yaml_iter_t parameter_iter;
            yaml_iter_recurse(&root_iter, &parameter_iter);
            while(yaml_iter_next(&parameter_iter))
            {
                const char *parameter_key = yaml_iter_key(&parameter_iter);
                d_assert(parameter_key, return CORE_ERROR,);
                if (!strcmp(parameter_key, "no_hss"))
                {
                    self.parameter.no_hss =
                        yaml_iter_bool(&parameter_iter);
                }
                else if (!strcmp(parameter_key, "no_sgw"))
                {
                    self.parameter.no_sgw =
                        yaml_iter_bool(&parameter_iter);
                }
                else if (!strcmp(parameter_key, "no_pgw"))
                {
                    self.parameter.no_pgw =
                        yaml_iter_bool(&parameter_iter);
                }
                else if (!strcmp(parameter_key, "no_pcrf"))
                {
                    self.parameter.no_pcrf =
                        yaml_iter_bool(&parameter_iter);
                }
                else if (!strcmp(parameter_key, "sctp_streams"))
                {
                    const char *v = yaml_iter_value(&parameter_iter);
                    if (v) self.parameter.sctp_streams = atoi(v);
                }
                else if (!strcmp(parameter_key, "no_ipv4"))
                {
                    self.parameter.no_ipv4 =
                        yaml_iter_bool(&parameter_iter);
                }
                else if (!strcmp(parameter_key, "no_ipv6"))
                {
                    self.parameter.no_ipv6 =
                        yaml_iter_bool(&parameter_iter);
                }
                else if (!strcmp(parameter_key, "prefer_ipv4"))
                {
                    self.parameter.prefer_ipv4 =
                        yaml_iter_bool(&parameter_iter);
                }
                else if (!strcmp(parameter_key, "multicast"))
                {
                    self.parameter.multicast =
                        yaml_iter_bool(&parameter_iter);
                }
                else if (!strcmp(parameter_key, "no_slaac"))
                {
                    self.parameter.no_slaac =
                        yaml_iter_bool(&parameter_iter);
                }
                else
                    d_warn("unknown key `%s`", parameter_key);
            }
        }
    }

    rv = context_validation();
    if (rv != CORE_OK) return rv;

    return CORE_OK;
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

status_t context_db_init(const char *db_uri)
{
    bson_t reply;
    bson_error_t error;
    bson_iter_t iter;

    const mongoc_uri_t *uri;

    mongoc_init();

    self.db_client = mongoc_client_new(db_uri);
    if (!self.db_client)
    {
        d_error("Failed to parse DB URI [%s]", db_uri);
        return CORE_ERROR;
    }

#if MONGOC_MAJOR_VERSION >= 1 && MONGOC_MINOR_VERSION >= 4
    mongoc_client_set_error_api(self.db_client, 2);
#endif

    uri = mongoc_client_get_uri(self.db_client);
    d_assert(uri, context_db_final(); return CORE_ERROR,
            "Database is not defined in DB_URI [%s]", db_uri);

    self.db_name = (char *)mongoc_uri_get_database(uri);
    d_assert(self.db_name, context_db_final(); return CORE_ERROR,
            "Database is not defined in DB_URI [%s]", db_uri);

    self.database = mongoc_client_get_database(self.db_client, self.db_name);
    d_assert(self.database, context_db_final(); return CORE_ERROR,
            "Database is not defined in DB_URI [%s]", db_uri);

    if (!context_mongoc_client_get_server_status(
                self.db_client, NULL, &reply, &error)) 
    {
        d_error("Failed to conect to server [%s]", db_uri);
        return CORE_EAGAIN;
    }

    d_assert(bson_iter_init_find(&iter, &reply, "ok"), 
            bson_destroy(&reply); context_db_final(); return CORE_ERROR,
            "Invalid reply for server status [%s]", db_uri);

    bson_destroy(&reply);
    return CORE_OK;
}

status_t context_db_final()
{
    if (self.database)
    {
        mongoc_database_destroy(self.database);
        self.database = NULL;
    }
    if (self.db_client)
    {
        mongoc_client_destroy(self.db_client);
        self.db_client = NULL;
    }

    mongoc_cleanup();

    return CORE_OK;
}
