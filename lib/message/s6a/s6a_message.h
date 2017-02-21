#ifndef _S6A_MESSAGE_H__
#define _S6A_MESSAGE_H__

#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S6A_SDU_SIZE 2048

CORE_DECLARE(status_t) s6a_initialize();

CORE_DECLARE(status_t) s6a_thread_start();
CORE_DECLARE(void) s6a_thread_stop();

CORE_DECLARE(status_t) s6a_conf_set_identity(char *identity);
CORE_DECLARE(status_t) s6a_conf_set_realm(char *realm);
CORE_DECLARE(void) s6a_conf_show();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

