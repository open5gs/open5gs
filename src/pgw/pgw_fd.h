#ifndef __PGW_FD_H__
#define __PGW_FD_H__

#include "core_errno.h"

#include "pgw_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(int) pgw_fd_init(void);
CORE_DECLARE(void) pgw_fd_final(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PGW_FD_H__ */

