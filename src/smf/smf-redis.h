#ifndef SMF_REDIS_H
#define SMF_REDIS_H

#include "ogs-core.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct smf_redis_context_s {
    ogs_sockaddr_t *addr;
    void *redis;  /* hiredis context */
    bool is_connected;
    ogs_timer_t *t_reconnect;
} smf_redis_context_t;

int smf_redis_init(void);
void smf_redis_final(void);

int smf_redis_connect(void);
void smf_redis_disconnect(void);
void smf_redis_reconnect(void);


#ifdef __cplusplus
}
#endif

#endif /* SMF_REDIS_H */