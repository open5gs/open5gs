#ifndef MME_REDIS_H
#define MME_REDIS_H

#include "ogs-core.h"
#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mme_redis_context_s {
    ogs_sockaddr_t *addr;
    void *redis;  /* hiredis context */
    bool is_connected;
    ogs_timer_t *t_reconnect;
} mme_redis_context_t;

int mme_redis_init(void);
void mme_redis_final(void);

int mme_redis_connect(void);
void mme_redis_disconnect(void);
void mme_redis_reconnect(void);

int mme_redis_update_ue(mme_ue_t *mme_ue);
int mme_redis_remove_ue(mme_ue_t *mme_ue);

#ifdef __cplusplus
}
#endif

#endif /* MME_REDIS_H */ 