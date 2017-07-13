#define TRACE_MODULE _context

#include "core_file.h"

#include "context.h"

#define DEFAULT_CONFIG_FILE_PATH SYSCONFDIR PACKAGE ".conf"

static context_t self;

static int context_initialized = 0;

status_t context_init()
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "Context already has been context_initialized");

    /* Initialize context */
    memset(&self, 0, sizeof(context_t));

    context_initialized = 1;

    return CORE_OK;
}

status_t context_final()
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    context_initialized = 0;

    return CORE_OK;
}

context_t* context_self()
{
    return &self;
}

status_t context_read_file(char *file_path)
{
    config_t *config = &self.config;
    status_t rv;
    file_t *file;

    char *path = file_path;
    if (path == NULL) path = DEFAULT_CONFIG_FILE_PATH;

    rv = file_open(&file, path, FILE_READ, FILE_OS_DEFAULT);
    if (rv != CORE_OK) 
    {
        d_fatal("Can't open configuration file '%s' (errno = %d, %s)", 
            path, rv, core_strerror(rv, config->json, MAX_CONFIG_FILE_SIZE));
        return rv;
    }

    config->json_len = MAX_CONFIG_FILE_SIZE;
    rv = file_read(file, config->json, &config->json_len);
    if (rv != CORE_OK) 
    {
        d_fatal("Can't read configuration file '%s' (errno = %d, %s)", 
                path, rv, core_strerror(rv, config->json, MAX_CONFIG_FILE_SIZE));
        return rv;
    }
    file_close(file);

    jsmn_init(&config->parser);
    config->num_of_token = jsmn_parse(
        &config->parser, config->json, strlen(config->json), 
        config->token, sizeof(config->token)/sizeof(config->token[0]));
    if (config->num_of_token < 0) 
    {
        d_fatal("Failed to parse configuration '%s'(jsmnerr = %d)", 
                config->json, config->num_of_token);
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t context_parse_config()
{
    config_t *config = &self.config;
    int i;

    for (i = 0; i < config->num_of_token; i++) 
    {
        if (json_token_streq(config->json, &config->token[i], 
            "DB_URI") == 0)
        {
            self.db_uri = 
                json_token_tostr(config->json, &config->token[i+1]);
            i++;
        }
        if (json_token_streq(config->json, &config->token[i],
            "LOG_PATH") == 0)
        {
            self.log_path = 
                json_token_tostr(config->json, &config->token[i+1]);
            i++;
        }
    }

    printf("%s, %s\n", self.db_uri, self.log_path);

    return CORE_OK;
}
