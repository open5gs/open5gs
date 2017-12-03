#ifndef __GTP_CONV_H__
#define __GTP_CONV_H__

#include "core_errno.h"

typedef struct _tlv_bearer_context_t tlv_bearer_context_t;
typedef struct _gtp_create_indirect_data_forwarding_tunnel_request_t gtp_create_indirect_data_forwarding_tunnel_request_t;
typedef struct _gtp_create_indirect_data_forwarding_tunnel_response_t gtp_create_indirect_data_forwarding_tunnel_response_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTP_MAX_NUM_OF_INDIRECT_TUNNEL      11

/* Create Indirect Data Forwarding Tunnel Request/Response */
CORE_DECLARE(void) gtp_bearers_in_create_indirect_tunnel_request(
        tlv_bearer_context_t *bearers[][GTP_MAX_NUM_OF_INDIRECT_TUNNEL],
        gtp_create_indirect_data_forwarding_tunnel_request_t *req);
CORE_DECLARE(void) gtp_bearers_in_create_indirect_tunnel_response(
        tlv_bearer_context_t *bearers[][GTP_MAX_NUM_OF_INDIRECT_TUNNEL],
        gtp_create_indirect_data_forwarding_tunnel_response_t *rsp);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_CONV_H__ */

