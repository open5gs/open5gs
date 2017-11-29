#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "core_debug.h"
#include "core_param.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_CONFIG_FILE_SIZE    2048
#define MAX_NUM_OF_CONFIG_TOKEN 256

typedef struct _config_t {
    const char *old_path;
    const char *path;
    char json[MAX_CONFIG_FILE_SIZE+1];
    void *bson;
    void *document;
} config_t;

#define MAX_DB_URI_LEN          256

typedef struct _context_t {
    config_t config;

    const char *db_uri;
    void *db_client;
    char *db_name;
    void *database;

    struct {
        const char *path;
        int console;
        const char *syslog;
        struct {
            const char *unix_domain;
            const char *file;
        } network;
        const char *file;

        struct {
            int s1ap;
            int nas;
            int gtp;
            int fd;
            int others;
        } trace;
    } logger;

    struct {
        /* Element */
        int no_hss;
        int no_sgw;
        int no_pgw;
        int no_pcrf;

        /* Network */
        int no_ipv4;
        int no_ipv6;
        int prefer_ipv4;
    } parameter;

} context_t;

CORE_DECLARE(status_t)      context_init(void);
CORE_DECLARE(status_t)      context_final(void);
CORE_DECLARE(context_t*)    context_self(void);

CORE_DECLARE(status_t)      context_read_file(void);
CORE_DECLARE(status_t)      context_parse_config(void);

CORE_DECLARE(status_t)      context_read_old_file(void);

CORE_DECLARE(status_t)      context_db_init(const char *db_uri);
CORE_DECLARE(status_t)      context_db_final(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CONTEXT_H__ */
