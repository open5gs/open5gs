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
    const char *path;
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
            int app;
            int s1ap;
            int nas;
            int gtpv2;
            int gtp;
            int diameter;
        } trace;
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

} context_t;

CORE_DECLARE(status_t)      context_init(void);
CORE_DECLARE(status_t)      context_final(void);
CORE_DECLARE(context_t*)    context_self(void);

CORE_DECLARE(status_t)      context_read_file(void);
CORE_DECLARE(status_t)      context_parse_config(void);
CORE_DECLARE(status_t)      context_setup_trace_module(void);

CORE_DECLARE(status_t)      context_db_init(const char *db_uri);
CORE_DECLARE(status_t)      context_db_final(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CONTEXT_H__ */
