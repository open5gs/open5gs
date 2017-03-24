#ifndef __3GPP_CONV_H__
#define __3GPP_CONV_H__

#include "3gpp_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void *) s1ap_plmn_id_to_buffer(
        plmn_id_t *plmn_id, c_uint8_t *buf);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __3GPP_CONV_H__ */

