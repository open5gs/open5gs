#define TRACE_MODULE _context

#include "core_file.h"
#include "core_debug.h"
#include "core_lib.h"
#include "core_pkbuf.h"

#include <yaml.h>
#include <mongoc.h>

#include "context.h"

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

    if (self.config.bson)
        bson_destroy(self.config.bson);
    if (self.config.document)
    {
        yaml_document_delete(self.config.document);
        core_free(self.config.document);
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
    d_assert(file, return CORE_ERROR,);

    d_assert(yaml_parser_initialize(&parser), return CORE_ERROR,);
    yaml_parser_set_input_file(&parser, file);

    document = core_calloc(1, sizeof(yaml_document_t));
    if (!yaml_parser_load(&parser, document))
    {
        d_fatal("Failed to parse configuration file '%s'", config->path);
        core_free(document);
        return CORE_ERROR;
    }

    config->document = document;

    yaml_parser_delete(&parser);
    d_assert(!fclose(file),,);

    return CORE_OK;
}

status_t context_read_old_file()
{
    char buf[MAX_ERROR_STRING_LEN];
    config_t *config = &self.config;
    status_t rv;
    file_t *file;

    bson_error_t error;
    size_t json_len;

    d_assert(config->old_path, return CORE_ERROR,);

    rv = file_open(&file, config->old_path, FILE_READ, FILE_OS_DEFAULT);
    if (rv != CORE_OK) 
    {
        d_fatal("Can't open configuration file '%s' (errno = %d, %s)", 
            config->old_path, rv, core_strerror(rv, buf, MAX_ERROR_STRING_LEN));
        return rv;
    }

    json_len = MAX_CONFIG_FILE_SIZE;
    rv = file_read(file, config->json, &json_len);
    if (rv != CORE_OK) 
    {
        d_fatal("Can't read configuration file '%s' (errno = %d, %s)", 
            config->old_path, rv, core_strerror(rv, buf, MAX_ERROR_STRING_LEN));
        return rv;
    }
    file_close(file);

    config->bson = bson_new_from_json((const uint8_t *)config->json, -1, &error);;
    if (config->bson == NULL)
    {
        d_fatal("Failed to parse configuration file '%s'", config->old_path);
        return CORE_ERROR;
    }

    d_print("  Config '%s'\n", config->old_path);

    return CORE_OK;
}

static status_t context_prepare()
{
    self.log.console = -1;

    return CORE_OK;
}

status_t context_parse_config()
{
    config_t *config = &self.config;
    yaml_document_t *document = NULL;
    yaml_node_t *node = NULL;

    d_assert(config, return CORE_ERROR,);
    document = config->document;
    d_assert(document, return CORE_ERROR,);

    for (node = document->nodes.start; node < document->nodes.top; node++)
    {
#if 0
        switch (node->type)
        {
            case YAML_SCALAR_NODE:
                printf("SCALAR: %s\n", node->data.scalar.value);
                break;
            case YAML_SEQUENCE_NODE:
                printf("SEQUENCE\n");
                break;
            case YAML_MAPPING_NODE:
                printf("MAPPIGNG\n");
                break;
            default:
                d_assert(0, return CORE_ERROR,);
                break;
        }
#endif
    }

    return CORE_OK;
}

status_t context_parse_old_config()
{
    status_t rv;
    config_t *config = &self.config;
    bson_iter_t iter;
    c_uint32_t length = 0;

    d_assert(config, return CORE_ERROR, );

    rv = context_prepare();
    if (rv != CORE_OK) return rv;

    if (!bson_iter_init(&iter, config->bson))
    {
        d_error("bson_iter_init failed in this document");
        return CORE_ERROR;
    }

    while(bson_iter_next(&iter))
    {
        const char *key = bson_iter_key(&iter);
        if (!strcmp(key, "DB_URI") && BSON_ITER_HOLDS_UTF8(&iter))
        {
            self.db_uri = bson_iter_utf8(&iter, &length);
        }
        else if (!strcmp(key, "LOG") && BSON_ITER_HOLDS_DOCUMENT(&iter))
        {
            bson_iter_t log_iter;
            bson_iter_recurse(&iter, &log_iter);
            while(bson_iter_next(&log_iter))
            {
                const char *log_key = bson_iter_key(&log_iter);
                if (!strcmp(log_key, "CONSOLE") &&
                    BSON_ITER_HOLDS_INT32(&log_iter))
                {
                    self.log.console = bson_iter_int32(&log_iter);
                }
                else if (!strcmp(log_key, "SYSLOG") &&
                    BSON_ITER_HOLDS_UTF8(&log_iter))
                {
                    self.log.syslog = bson_iter_utf8(&log_iter, &length);
                }
                else if (!strcmp(log_key, "SOCKET") &&
                        BSON_ITER_HOLDS_DOCUMENT(&log_iter))
                {
                    bson_iter_t socket_iter;
                    bson_iter_recurse(&log_iter, &socket_iter);
                    while(bson_iter_next(&socket_iter))
                    {
                        const char *socket_key = bson_iter_key(&socket_iter);
                        if (!strcmp(socket_key, "FILE") &&
                            BSON_ITER_HOLDS_UTF8(&socket_iter))
                        {
                            self.log.socket.file =
                                bson_iter_utf8(&socket_iter, &length);
                        }
                        else if (!strcmp(socket_key, "UNIX_DOMAIN") &&
                            BSON_ITER_HOLDS_UTF8(&socket_iter))
                        {
                            self.log.socket.unix_domain =
                                bson_iter_utf8(&socket_iter, &length);
                        }
                    }
                }
                else if (!strcmp(log_key, "FILE") &&
                    BSON_ITER_HOLDS_UTF8(&log_iter))
                {
                    self.log.file = bson_iter_utf8(&log_iter, &length);
                }
            }
        }
        else if (!strcmp(key, "TRACE") && BSON_ITER_HOLDS_DOCUMENT(&iter))
        {
            bson_iter_t trace_iter;
            bson_iter_recurse(&iter, &trace_iter);
            while(bson_iter_next(&trace_iter))
            {
                const char *trace_key = bson_iter_key(&trace_iter);
                if (!strcmp(trace_key, "S1AP") &&
                    BSON_ITER_HOLDS_INT32(&trace_iter))
                {
                    self.trace_level.s1ap = bson_iter_int32(&trace_iter);
                }
                else if (!strcmp(trace_key, "NAS") &&
                    BSON_ITER_HOLDS_INT32(&trace_iter))
                {
                    self.trace_level.nas = bson_iter_int32(&trace_iter);
                }
                else if (!strcmp(trace_key, "FD") &&
                    BSON_ITER_HOLDS_INT32(&trace_iter))
                {
                    self.trace_level.fd = bson_iter_int32(&trace_iter);
                }
                else if (!strcmp(trace_key, "GTP") &&
                    BSON_ITER_HOLDS_INT32(&trace_iter))
                {
                    self.trace_level.gtp = bson_iter_int32(&trace_iter);
                }
                else if (!strcmp(trace_key, "OTHERS") &&
                    BSON_ITER_HOLDS_INT32(&trace_iter))
                {
                    self.trace_level.others = bson_iter_int32(&trace_iter);
                }
            }
        }
        else if (!strcmp(key, "NODE") && BSON_ITER_HOLDS_DOCUMENT(&iter))
        {
            bson_iter_t node_iter;
            bson_iter_recurse(&iter, &node_iter);
            while(bson_iter_next(&node_iter))
            {
                const char *node_key = bson_iter_key(&node_iter);
                if (!strcmp(node_key, "DISABLE_HSS") &&
                    BSON_ITER_HOLDS_INT32(&node_iter))
                {
                    self.node.disable_hss = bson_iter_int32(&node_iter);
                }
                else if (!strcmp(node_key, "DISABLE_SGW") &&
                    BSON_ITER_HOLDS_INT32(&node_iter))
                {
                    self.node.disable_sgw = bson_iter_int32(&node_iter);
                }
                else if (!strcmp(node_key, "DISABLE_PGW") &&
                    BSON_ITER_HOLDS_INT32(&node_iter))
                {
                    self.node.disable_pgw = bson_iter_int32(&node_iter);
                }
                else if (!strcmp(node_key, "DISABLE_PCRF") &&
                    BSON_ITER_HOLDS_INT32(&node_iter))
                {
                    self.node.disable_pcrf = bson_iter_int32(&node_iter);
                }
            }
        }
    }

    return CORE_OK;
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

    if (!mongoc_client_get_server_status(self.db_client, NULL, &reply, &error)) 
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
