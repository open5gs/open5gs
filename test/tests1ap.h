#ifndef __TESTS1AP_H__
#define __TESTS1AP_H__

#include "core_net.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(net_sock_t) *tests1ap_enb_connect(void);
CORE_DECLARE(status_t) tests1ap_enb_close(net_sock_t *sock);
CORE_DECLARE(int) tests1ap_enb_send(net_sock_t *sock, pkbuf_t *sendbuf);
CORE_DECLARE(int) tests1ap_enb_read(net_sock_t *sock, pkbuf_t *recvbuf);

CORE_DECLARE(status_t) tests1ap_build_setup_req(
        pkbuf_t **pkbuf, c_uint32_t enb_id);
CORE_DECLARE(status_t) tests1ap_build_initial_ue_msg(pkbuf_t **pkbuf, int i);
CORE_DECLARE(status_t) tests1ap_build_authentication_response(
        pkbuf_t **pkbuf, int i);
CORE_DECLARE(status_t) tests1ap_build_security_mode_complete(
        pkbuf_t **pkbuf, int i);
CORE_DECLARE(status_t) tests1ap_build_esm_information_response(
        pkbuf_t **pkbuf, int i);
CORE_DECLARE(status_t) tests1ap_build_ue_capability_info_indication(
        pkbuf_t **pkbuf, int i);
CORE_DECLARE(status_t) tests1ap_build_initial_context_setup_response(
        pkbuf_t **pkbuf, int i);
CORE_DECLARE(status_t) tests1ap_build_attach_complete(pkbuf_t **pkbuf, int i);
CORE_DECLARE(status_t) tests1ap_build_emm_status(pkbuf_t **pkbuf, int i);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TESTS1AP_H__ */
