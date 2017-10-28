#define TRACE_MODULE _context

#include "core_file.h"
#include "core_debug.h"
#include "core_lib.h"
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

    context_initialized = 0;

    return CORE_OK;
}

context_t* context_self()
{
    return &self;
}

status_t context_read_file()
{
    char buf[MAX_ERROR_STRING_LEN];
    config_t *config = &self.config;
    status_t rv;
    file_t *file;

    bson_error_t error;
    size_t json_len;
    jsmn_parser parser;
    int result;

    d_assert(config->path, return CORE_ERROR,);

    rv = file_open(&file, config->path, FILE_READ, FILE_OS_DEFAULT);
    if (rv != CORE_OK) 
    {
        d_fatal("Can't open configuration file '%s' (errno = %d, %s)", 
            config->path, rv, core_strerror(rv, buf, MAX_ERROR_STRING_LEN));
        return rv;
    }

    json_len = MAX_CONFIG_FILE_SIZE;
    rv = file_read(file, config->json, &json_len);
    if (rv != CORE_OK) 
    {
        d_fatal("Can't read configuration file '%s' (errno = %d, %s)", 
            config->path, rv, core_strerror(rv, buf, MAX_ERROR_STRING_LEN));
        return rv;
    }
    file_close(file);

    jsmn_init(&parser);
    result = jsmn_parse(&parser, config->json, strlen(config->json), 
        config->token, sizeof(config->token)/sizeof(config->token[0]));
    if (result < 0) 
    {
        d_fatal("Failed to parse configuration file '%s' (jsmnerr = %d)", 
                config->path, result);
        return CORE_ERROR;
    }

    if (result < 1 || config->token[0].type != JSMN_OBJECT) 
    {
        d_fatal("Failed to parse configuration file '%s' (OBJECT expected)",
                config->path);
        return CORE_ERROR;
    }

    config->bson = bson_new_from_json((const uint8_t *)config->json, -1, &error);;
    if (config->bson == NULL)
    {
        d_fatal("Failed to parse configuration file '%s'", config->path);
        return CORE_ERROR;
    }

    d_print("  Config '%s'\n", config->path);

    return CORE_OK;
}

status_t context_parse_config()
{
    config_t *config = &self.config;
    bson_iter_t iter, child1_iter, child2_iter;
    c_uint32_t length = 0;

    d_assert(config, return CORE_ERROR, );

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
            bson_iter_recurse(&iter, &child1_iter);
            while(bson_iter_next(&child1_iter))
            {
                const char *child1_key = bson_iter_key(&child1_iter);
                if (!strcmp(child1_key, "FILE") &&
                    BSON_ITER_HOLDS_UTF8(&child1_iter))
                {
                    self.log.file = bson_iter_utf8(&child1_iter, &length);
                }
                else if (!strcmp(child1_key, "SOCKET") &&
                        BSON_ITER_HOLDS_DOCUMENT(&iter))
                {
                    bson_iter_recurse(&child1_iter, &child2_iter);
                    while(bson_iter_next(&child2_iter))
                    {
                        const char *child2_key = bson_iter_key(&child2_iter);
                        if (!strcmp(child2_key, "FILE") &&
                            BSON_ITER_HOLDS_UTF8(&child2_iter))
                        {
                            self.log.socket.file =
                                bson_iter_utf8(&child2_iter, &length);
                        }
                        else if (!strcmp(child2_key, "UNIX_DOMAIN") &&
                            BSON_ITER_HOLDS_UTF8(&child2_iter))
                        {
                            self.log.socket.unix_domain =
                                bson_iter_utf8(&child2_iter, &length);
                        }
                    }

                }
            }
        }
        else if (!strcmp(key, "TRACE") && BSON_ITER_HOLDS_DOCUMENT(&iter))
        {
            bson_iter_recurse(&iter, &child1_iter);
            while(bson_iter_next(&child1_iter))
            {
                const char *child1_key = bson_iter_key(&child1_iter);
                if (!strcmp(child1_key, "S1AP") &&
                    BSON_ITER_HOLDS_INT32(&child1_iter))
                {
                    self.trace_level.s1ap = bson_iter_int32(&child1_iter);
                }
                else if (!strcmp(child1_key, "NAS") &&
                    BSON_ITER_HOLDS_INT32(&child1_iter))
                {
                    self.trace_level.nas = bson_iter_int32(&child1_iter);
                }
                else if (!strcmp(child1_key, "FD") &&
                    BSON_ITER_HOLDS_INT32(&child1_iter))
                {
                    self.trace_level.fd = bson_iter_int32(&child1_iter);
                }
                else if (!strcmp(child1_key, "GTP") &&
                    BSON_ITER_HOLDS_INT32(&child1_iter))
                {
                    self.trace_level.gtp = bson_iter_int32(&child1_iter);
                }
                else if (!strcmp(child1_key, "OTHERS") &&
                    BSON_ITER_HOLDS_INT32(&child1_iter))
                {
                    self.trace_level.others = bson_iter_int32(&child1_iter);
                }
            }
        }
        else if (!strcmp(key, "NODE") && BSON_ITER_HOLDS_DOCUMENT(&iter))
        {
            bson_iter_recurse(&iter, &child1_iter);
            while(bson_iter_next(&child1_iter))
            {
                const char *child1_key = bson_iter_key(&child1_iter);
                if (!strcmp(child1_key, "DISABLE_HSS") &&
                    BSON_ITER_HOLDS_INT32(&child1_iter))
                {
                    self.node.disable_hss = bson_iter_int32(&child1_iter);
                }
                else if (!strcmp(child1_key, "DISABLE_SGW") &&
                    BSON_ITER_HOLDS_INT32(&child1_iter))
                {
                    self.node.disable_sgw = bson_iter_int32(&child1_iter);
                }
                else if (!strcmp(child1_key, "DISABLE_PGW") &&
                    BSON_ITER_HOLDS_INT32(&child1_iter))
                {
                    self.node.disable_pgw = bson_iter_int32(&child1_iter);
                }
                else if (!strcmp(child1_key, "DISABLE_PCRF") &&
                    BSON_ITER_HOLDS_INT32(&child1_iter))
                {
                    self.node.disable_pcrf = bson_iter_int32(&child1_iter);
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
        return CORE_ERROR;
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
