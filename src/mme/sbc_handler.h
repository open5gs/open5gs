#ifndef __SBC_HANDLER_H__
#define __SBC_HANDLER_H__

#include "sbc_message.h"

/* SBc-AP handles */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) sbc_handle_write_replace_warning_request(sbc_pws_data_t *sbc_pws);
CORE_DECLARE(void) sbc_handle_stop_warning_request(sbc_pws_data_t *sbc_pws);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SBC_HANDLER_H__ */
