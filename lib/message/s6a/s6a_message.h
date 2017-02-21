#ifndef _S6A_MESSAGE_H__
#define _S6A_MESSAGE_H__

#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S6A_SDU_SIZE 2048

typedef struct _s6a_conf_t {
    char *identity;
    char *realm;
} s6a_conf_t;

CORE_DECLARE(status_t) s6a_initialize();
CORE_DECLARE(status_t) s6a_conf_handle(s6a_conf_t *conf);

CORE_DECLARE(status_t) s6a_thread_start();
CORE_DECLARE(void) s6a_thread_stop();

CORE_DECLARE(void) s6a_conf_show();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

