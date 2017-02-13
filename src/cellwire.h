#ifndef __CELLWIRE_H__
#define __CELLWIRE_H__

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S1_SCTP_PORT                36412
#define GTP_C_UDP_PORT              2123
#define GTP_U_UDP_PORT              2152

CORE_DECLARE(status_t) cellwire_initialize(char *config_path);
CORE_DECLARE_NONSTD(void) cellwire_terminate(void);

CORE_DECLARE(void) threads_start(void);
CORE_DECLARE(void) threads_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__CELLWIRE_H__ */
