#ifndef __MME_S11_BUILD_H__
#define __MME_S11_BUILD_H__

#include "gtp/gtp_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int mme_s11_build_create_session_request(
        ogs_pkbuf_t **pkbuf, uint8_t type, mme_sess_t *sess);
int mme_s11_build_modify_bearer_request(ogs_pkbuf_t **pkbuf,
        uint8_t type, mme_bearer_t *bearer, int uli_presense);
int mme_s11_build_delete_session_request(
        ogs_pkbuf_t **pkbuf, uint8_t type, mme_sess_t *sess);
int mme_s11_build_create_bearer_response(
        ogs_pkbuf_t **pkbuf, uint8_t type, mme_bearer_t *bearer);
int mme_s11_build_update_bearer_response(
        ogs_pkbuf_t **pkbuf, uint8_t type, mme_bearer_t *bearer);
int mme_s11_build_delete_bearer_response(
        ogs_pkbuf_t **pkbuf, uint8_t type, mme_bearer_t *bearer);
int mme_s11_build_release_access_bearers_request(
        ogs_pkbuf_t **pkbuf, uint8_t type);
int mme_s11_build_downlink_data_notification_ack(
        ogs_pkbuf_t **pkbuf, uint8_t type);
int mme_s11_build_create_indirect_data_forwarding_tunnel_request(
        ogs_pkbuf_t **pkbuf, uint8_t type, mme_ue_t *mme_ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_S11_BUILD_H__ */
