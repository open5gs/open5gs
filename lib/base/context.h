#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "core_debug.h"
#include "core_param.h"
#include "core_jsmn.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_CONFIG_FILE_SIZE    2048
#define MAX_NUM_OF_CONFIG_TOKEN 256

typedef struct _config_t {
    char *path;
    char json[MAX_CONFIG_FILE_SIZE+1];
    jsmntok_t token[MAX_NUM_OF_CONFIG_TOKEN];
} config_t;

#define MAX_DB_URI_LEN          256

typedef struct _context_t {
    config_t config;

    char *log_path;

    char *db_uri;
    void *db_client;
    char *db_name;
    void *database;

    struct {
        int context;
        int sm;
        int s1ap;
        int nas;
        int gtp;
        int s6a;
    } trace_level;
} context_t;

CORE_DECLARE(status_t)      context_init(void);
CORE_DECLARE(status_t)      context_final(void);
CORE_DECLARE(context_t*)    context_self(void);

CORE_DECLARE(status_t)      context_read_file(char *file_path);
CORE_DECLARE(status_t)      context_parse_config(void);
CORE_DECLARE(status_t)      context_setup_trace_module(void);

CORE_DECLARE(status_t)      context_db_init(char *db_uri);
CORE_DECLARE(status_t)      context_db_final(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CONTEXT_H__ */
