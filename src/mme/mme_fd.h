#ifndef __MME_FD_H__
#define __MME_FD_H__

#include "core_errno.h"

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(int) mme_fd_init(void);
CORE_DECLARE(void) mme_fd_final(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_FD_H__ */

