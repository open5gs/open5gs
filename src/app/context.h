#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "ogs-core.h"
#include "ogs-yaml.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_CONFIG_FILE_SIZE    2048
#define MAX_NUM_OF_CONFIG_TOKEN 256

typedef struct _config_t {
    const char *path;
    void *document;

    const char *db_uri;
    struct {
        const char *file;
        const char *level;
        const char *domain;
    } logger;

    struct {
        /* Element */
        int no_hss;
        int no_sgw;
        int no_pgw;
        int no_pcrf;

        /* Network */
        int sctp_streams;
        int no_ipv4;
        int no_ipv6;
        int prefer_ipv4;
        int multicast;
        int no_slaac;
    } parameter;

} config_t;

#define MAX_DB_URI_LEN          256

typedef struct _context_t {
    config_t config;

    struct {
        bool initialized;
        const char *name;
        void *uri;
        void *client;
        void *database;
    } db;

    struct {
        const char *path;
        ogs_log_level_e level;
        const char *domain;
    } log;

} context_t;

int context_init(void);
int context_final(void);
context_t *context_self(void);

int context_read_file(void);
int context_parse_config(void);
int context_setup_log_module(void);

int context_db_init(const char *db_uri);
int context_db_final(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CONTEXT_H__ */
