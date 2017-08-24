#ifndef __FD_LIB_H__
#define __FD_LIB_H__

#include "core_errno.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#include "fd_message.h"
#include "fd_logger.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FD_DIAMETER_ERROR_USER_UNKNOWN                  5001

CORE_DECLARE(int) fd_init(int mode, const char *conffile);
CORE_DECLARE(void) fd_final(void);

int fd_avp_search_avp ( struct avp * groupedavp, 
        struct dict_object * what, struct avp ** avp );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __FD_LIB_H__ */
