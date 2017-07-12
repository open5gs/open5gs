#define TRACE_MODULE _config

#include "core_file.h"
#include "core_debug.h"
#include "core_lib.h"
#include "core_jsmn.h"

#include "app.h"

#define MAX_CONFIG_FILE_SIZE 1024
#define MAX_JSMN_TOKEN_SIZE 128
#define DEFAULT_CONFIG_PATH SYSCONFDIR PACKAGE ".conf"
#define MAX_PATH_LEN 256

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) 
{
    if (tok->type == JSMN_PRIMITIVE && 
        (int) strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) 
    {
        return 0;
    }

    return -1;
}

status_t config_initialize(char *config_path)
{
    status_t rv;
    size_t nbytes = MAX_CONFIG_FILE_SIZE;
    char str[MAX_CONFIG_FILE_SIZE+1];
    file_t *file;

    char db_uri[MAX_PATH_LEN];
    char log_path[MAX_PATH_LEN];

    int i;
    int num_of_tok;
    jsmn_parser parser;
    jsmntok_t tok[128];

    char *path = config_path;
    if (path == NULL) path = DEFAULT_CONFIG_PATH;

    rv = file_open(&file, path, FILE_READ, FILE_OS_DEFAULT);
    if (rv != CORE_OK) 
    {
        d_fatal("Can't open configuration file '%s' (errno = %d, %s)", 
                path, rv, core_strerror(rv, str, MAX_CONFIG_FILE_SIZE));
        return rv;
    }

    rv = file_read(file, str, &nbytes);
    if (rv != CORE_OK) 
    {
        d_fatal("Can't read configuration file '%s' (errno = %d, %s)", 
                path, rv, core_strerror(rv, str, MAX_CONFIG_FILE_SIZE));
        return rv;
    }
    str[nbytes-1] = 0;

    file_close(file);

    jsmn_init(&parser);
    num_of_tok = jsmn_parse(
        &parser, str, strlen(str), tok, sizeof(tok)/sizeof(tok[0]));
    if (num_of_tok < 0) 
    {
        d_fatal("Failed to parse configuration '%s'(jsmnerr = %d)", 
                str, num_of_tok);
        return CORE_ERROR;
    }

    for (i = 0; i < num_of_tok; i++) 
    {
        if (jsoneq(str, &tok[i], "DB_URI") == 0) 
        {
            core_cpystrn(db_uri, 
                str + tok[i+1].start, tok[i+1].end-tok[i+1].start+1);
            i++;
        } 
        else if (jsoneq(str, &tok[i], "LOG_PATH") == 0) 
        {
            core_cpystrn(log_path, 
                str + tok[i+1].start, tok[i+1].end-tok[i+1].start+1);
            i++;
        }
    }

    return CORE_OK;
}

void config_terminate(void)
{
}
