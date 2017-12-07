#ifndef __PCRF_CONTEXT_H__
#define __PCRF_CONTEXT_H__

#include "core_errno.h"
#include "core_mutex.h"

#include "3gpp_types.h"
#include "gx_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _pcrf_context_t {
    const char  *fd_conf_path;          /* PCRF freeDiameter conf path */

    void        *subscriberCollection;
    mutex_id    db_lock;
} pcrf_context_t;

CORE_DECLARE(status_t) pcrf_context_init(void);
CORE_DECLARE(status_t) pcrf_context_final(void);
CORE_DECLARE(pcrf_context_t*) pcrf_self(void);

CORE_DECLARE(status_t) pcrf_context_parse_config(void);
CORE_DECLARE(status_t) pcrf_context_setup_trace_module(void);

CORE_DECLARE(status_t) pcrf_db_init(void);
CORE_DECLARE(status_t) pcrf_db_final(void);

CORE_DECLARE(status_t) pcrf_db_pdn_data(
        c_int8_t *imsi_bcd, c_int8_t *apn, gx_cca_message_t *cca_message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PCRF_CONTEXT_H__ */
