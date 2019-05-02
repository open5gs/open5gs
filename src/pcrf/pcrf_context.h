#ifndef __PCRF_CONTEXT_H__
#define __PCRF_CONTEXT_H__

#include "fd/gx/gx_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern int __pcrf_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __pcrf_log_domain

typedef struct _fd_config_t fd_config_t;
struct session;

typedef struct _pcrf_context_t {
    const char      *fd_conf_path;  /* PCRF freeDiameter conf path */
    fd_config_t     *fd_config;     /* PCRF freeDiameter config */

    void            *subscriberCollection;
    ogs_thread_mutex_t db_lock;

    ogs_hash_t      *ip_hash; /* hash table for Gx Frame IPv4/IPv6 */
    ogs_thread_mutex_t hash_lock;
} pcrf_context_t;

int pcrf_context_init(void);
int pcrf_context_final(void);
pcrf_context_t *pcrf_self(void);

int pcrf_context_parse_config(void);

int pcrf_db_init(void);
int pcrf_db_final(void);

int pcrf_db_qos_data(char *imsi_bcd, char *apn, gx_message_t *gx_message);

int pcrf_sess_set_ipv4(const void *key, uint8_t *sid);
int pcrf_sess_set_ipv6(const void *key, uint8_t *sid);
uint8_t *pcrf_sess_find_by_ipv4(const void *key);
uint8_t *pcrf_sess_find_by_ipv6(const void *key);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PCRF_CONTEXT_H__ */
