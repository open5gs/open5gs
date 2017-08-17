#ifndef __FD_LOGGER_H__
#define __FD_LOGGER_H__

#include "core_errno.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct fd_logger_t {

#define FD_MODE_SERVER   0x1
#define FD_MODE_CLIENT   0x2
    int mode;        /* default FD_MODE_SERVER | FD_MODE_CLIENT */
    
    int duration; /* default 10 */
    struct fd_stats {
        unsigned long long nb_echoed; /* server */
        unsigned long long nb_sent;   /* client */
        unsigned long long nb_recv;   /* client */
        unsigned long long nb_errs;   /* client */
        unsigned long shortest;  /* fastest answer, in microseconds */
        unsigned long longest;   /* slowest answer, in microseconds */
        unsigned long avg;       /* average answer time, in microseconds */
    } stats;

    pthread_mutex_t stats_lock;
};

CORE_DECLARE(int) fd_logger_init(int mode);
CORE_DECLARE(void) fd_logger_final();

CORE_DECLARE(struct fd_logger_t*) fd_logger_self();

CORE_DECLARE(int) fd_logger_stats_start();

typedef void (*fd_logger_user_handler)(
    enum fd_hook_type type, struct msg *msg, struct peer_hdr *peer, 
    void *other, struct fd_hook_permsgdata *pmd, void *regdata);

CORE_DECLARE(void) fd_logger_register(fd_logger_user_handler instance);
CORE_DECLARE(void) fd_logger_unregister();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __FD_LOGGER_H__ */
