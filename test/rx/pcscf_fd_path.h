#ifndef __PCSCF_FD_PATH_H__
#define __PCSCF_FD_PATH_H__

#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) pcscf_fd_init(void);
CORE_DECLARE(void) pcscf_fd_final(void);

CORE_DECLARE(void) pcscf_rx_send_aar(const char *ip);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PCSCF_FD_PATH_H__ */

