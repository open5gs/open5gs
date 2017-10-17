#define TRACE_MODULE _context

#include "core_file.h"
#include "core_debug.h"
#include <mongoc.h>

#include "context.h"

#define DEFAULT_CONFIG_FILE_PATH SYSCONFDIR PACKAGE ".conf"

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

    context_initialized = 0;

    return CORE_OK;
}

context_t* context_self()
{
    return &self;
}

status_t context_read_file(char *file_path)
{
    char buf[MAX_ERROR_STRING_LEN];
    config_t *config = &self.config;
    status_t rv;
    file_t *file;

    jsmn_parser parser;
    size_t json_len;
    int result;

    config->path = file_path;
    if (config->path == NULL) config->path = DEFAULT_CONFIG_FILE_PATH;

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

    d_print("  Config '%s'\n", config->path);

    return CORE_OK;
}

status_t context_parse_config()
{
    config_t *config = &self.config;

    char *json = config->json;
    jsmntok_t *token = config->token;

    typedef enum { START, ROOT, SKIP, STOP } parse_state;
    parse_state state = START;

    size_t root_tokens = 0;
    size_t skip_tokens = 0;
    size_t i, j, m, n;

    for (i = 0, j = 1; j > 0; i++, j--)
    {
        jsmntok_t *t = &token[i];

        j += t->size;

        switch (state)
        {
            case START:
            {
                state = ROOT;
                root_tokens = t->size;

                break;
            }
            case ROOT:
            {
                if (jsmntok_equal(json, t, "DB_URI") == 0)
                {
                    self.db_uri = jsmntok_to_string(json, t+1);
                }
                else if (jsmntok_equal(json, t, "LOG_PATH") == 0)
                {
                    self.log_path = jsmntok_to_string(json, t+1);
                }
                else if (jsmntok_equal(json, t, "TRACE") == 0)
                {
                    for (m = 1, n = 1; n > 0; m++, n--)
                    {
                        n += (t+m)->size;

                        char *v = jsmntok_to_string(json, t+m+1);
                        if (jsmntok_equal(json, t+m, "S1AP") == 0)
                        {
                            if (v) self.trace_level.s1ap = atoi(v);
                        }
                        else if (jsmntok_equal(json, t+m, "NAS") == 0)
                        {
                            if (v) self.trace_level.nas = atoi(v);
                        }
                        else if (jsmntok_equal(json, t+m, "FD") == 0)
                        {
                            if (v) self.trace_level.fd = atoi(v);
                        }
                        else if (jsmntok_equal(json, t+m, "GTP") == 0)
                        {
                            if (v) self.trace_level.gtp = atoi(v);
                        }
                        else if (jsmntok_equal(json, t+m, "OTHERS") == 0)
                        {
                            if (v) self.trace_level.others = atoi(v);
                        }
                    }
                }
                else if (jsmntok_equal(json, t, "NODE") == 0)
                {
                    for (m = 1, n = 1; n > 0; m++, n--)
                    {
                        n += (t+m)->size;

                        char *v = jsmntok_to_string(json, t+m+1);
                        if (jsmntok_equal(json, t+m, "DISABLE_HSS") == 0)
                        {
                            if (v) self.hidden.disable_hss = atoi(v);
                        }
                        else if (jsmntok_equal(json, t+m, "DISABLE_SGW") == 0)
                        {
                            if (v) self.hidden.disable_sgw = atoi(v);
                        }
                        else if (jsmntok_equal(json, t+m, "DISABLE_PGW") == 0)
                        {
                            if (v) self.hidden.disable_pgw = atoi(v);
                        }
                        else if (jsmntok_equal(json, t+m, "DISABLE_PCRF") == 0)
                        {
                            if (v) self.hidden.disable_pcrf = atoi(v);
                        }
                    }
                }

                state = SKIP;
                skip_tokens = t->size;

                root_tokens--;
                if (root_tokens == 0) state = STOP;

                break;
            }
            case SKIP:
            {
                skip_tokens += t->size;

                skip_tokens--;
                if (skip_tokens == 0) state = ROOT;

                break;
            }
            case STOP:
            {
                break;
            }
            default:
            {
                d_error("Failed to parse configuration in the state(%u)", 
                        state);
                break;
            }

        }
    }

    return CORE_OK;
}

status_t context_db_init(char *db_uri)
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
