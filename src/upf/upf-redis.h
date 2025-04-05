#ifndef UPF_REDIS_H
#define UPF_REDIS_H

#include "ogs-core.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct upf_redis_context_s {
    ogs_sockaddr_t *addr;
    void *redis;  /* hiredis context */
    bool is_connected;
    ogs_timer_t *t_reconnect;
} upf_redis_context_t;

int upf_redis_init(void);
void upf_redis_final(void);

int upf_redis_connect(void);
void upf_redis_disconnect(void);
void upf_redis_reconnect(void);


#ifdef __cplusplus
}
#endif

#endif /* UPF_REDIS_H */