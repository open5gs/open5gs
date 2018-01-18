#ifndef __PCSCF_FD_PATH_H__
#define __PCSCF_FD_PATH_H__

#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) pcscf_fd_init(void);
CORE_DECLARE(void) pcscf_fd_final(void);

CORE_DECLARE(void) pcscf_rx_send_aar(c_uint8_t **rx_sid, const char *ip,
        int qos_type, int flow_presence);
CORE_DECLARE(void) pcscf_rx_send_str(c_uint8_t *rx_sid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PCSCF_FD_PATH_H__ */

