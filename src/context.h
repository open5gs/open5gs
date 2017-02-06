#ifndef __CELLWIRE_CONTEXT_H__
#define __CELLWIRE_CONTEXT_H__

#include "core_list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_PLMN_ID                 6
#define GRP_PER_MME                 256    /* According to spec it is 65535 */
#define CODE_PER_MME                256    /* According to spec it is 256*/

typedef struct _plmn_id_t {
    c_uint8_t       num_mnc_digits;
    c_uint8_t       mcc[3];
    c_uint8_t       mnc[3];
} plmn_id_t;

typedef struct _served_gummei {
    c_uint32_t      num_of_plmn;
    plmn_id_t       plmn_id[MAX_PLMN_ID];

    c_uint32_t      num_of_grp_id;
    c_uint16_t      grp_id[GRP_PER_MME];
    c_uint32_t      num_of_code;
    c_uint8_t       code[CODE_PER_MME];
} srvd_gummei_t;

/**
 * This structure represents HypcerCell */
typedef struct _mme_ctx_t {
    plmn_id_t       plmn_id;

    c_uint8_t       mme_capacity;

    srvd_gummei_t   srvd_gummei;
} mme_ctx_t;

CORE_DECLARE(status_t)      context_init(void);
CORE_DECLARE(status_t)      context_final(void);

CORE_DECLARE(status_t)      context_read_lock(void);
CORE_DECLARE(status_t)      context_write_lock(void);
CORE_DECLARE(status_t)      context_unlock(void);

CORE_DECLARE(void)          context_post_cpath(void);
CORE_DECLARE(int)           context_fetch_cpath(void);

CORE_DECLARE(mme_ctx_t*)    mme_self(void);
#define self() mme_self()

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__CELLWIRE_CONTEXT_H__ */
