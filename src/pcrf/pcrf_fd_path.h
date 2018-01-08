#ifndef __PCRF_FD_PATH_H__
#define __PCRF_FD_PATH_H__

#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(int) pcrf_fd_init(void);
CORE_DECLARE(void) pcrf_fd_final(void);

CORE_DECLARE(int) pcrf_gx_init(void);
CORE_DECLARE(void) pcrf_gx_final(void);
CORE_DECLARE(int) pcrf_rx_init(void);
CORE_DECLARE(void) pcrf_rx_final(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PCRF_FD_PATH_H__ */

