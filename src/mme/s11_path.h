#ifndef __MME_S11_PATH_H__
#define __MME_S11_PATH_H__

#include "gtp_tlv.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) mme_s11_listen();
CORE_DECLARE(status_t) mme_s11_close();

CORE_DECLARE(status_t) mme_s11_send_to_sgw(
        void *sgw, c_uint8_t type, gtp_message_t *gtp_message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_S11_PATH_H__ */
