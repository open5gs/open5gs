#ifndef __FD_LIB_H__
#define __FD_LIB_H__

#include "core_errno.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#include "fd_context.h"
#include "fd_logger.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FD_REALM        "localdomain"       /* Default Relam */
#define MME_IDENTITY    "mme.localdomain"   /* Default MME Identity */
#define HSS_IDENTITY    "hss.localdomain"   /* Default HSS Identity */

#define AVP_CODE_CONTEXT_IDENTIFIER         (1423)
#define AVP_CODE_ALL_APN_CONFIG_INC_IND     (1428)
#define AVP_CODE_APN_CONFIGURATION          (1430)

CORE_DECLARE(int) fd_init(const char *conffile);
CORE_DECLARE(void) fd_final(void);

int fd_avp_search_avp ( struct avp * groupedavp, 
        struct dict_object * what, struct avp ** avp );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __FD_LIB_H__ */
