#ifndef __GTP_CONV_H__
#define __GTP_CONV_H__

#include "core_errno.h"

typedef struct _tlv_bearer_context_t tlv_bearer_context_t;
typedef struct _gtp_create_indirect_data_forwarding_tunnel_request_t gtp_create_indirect_data_forwarding_tunnel_request_t;
typedef struct _gtp_create_indirect_data_forwarding_tunnel_response_t gtp_create_indirect_data_forwarding_tunnel_response_t;
typedef struct c_sockaddr_t c_sockaddr_t;
typedef struct _gtp_f_teid_t gtp_f_teid_t;
typedef struct _ip_t ip_t;

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

CORE_DECLARE(status_t) gtp_f_teid_to_sockaddr(
    gtp_f_teid_t *f_teid, c_uint16_t port, c_sockaddr_t **list);
CORE_DECLARE(status_t) gtp_sockaddr_to_f_teid(
    c_sockaddr_t *addr, c_sockaddr_t *addr6, gtp_f_teid_t *f_teid, int *len);
CORE_DECLARE(status_t) gtp_f_teid_to_ip(gtp_f_teid_t *f_teid, ip_t *ip);
CORE_DECLARE(status_t) gtp_ip_to_f_teid(
        ip_t *ip, gtp_f_teid_t *f_teid, int *len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_CONV_H__ */

