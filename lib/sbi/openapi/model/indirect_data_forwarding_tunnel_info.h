/*
 * indirect_data_forwarding_tunnel_info.h
 *
 * Indirect Data Forwarding Tunnel Information
 */

#ifndef _OpenAPI_indirect_data_forwarding_tunnel_info_H_
#define _OpenAPI_indirect_data_forwarding_tunnel_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_indirect_data_forwarding_tunnel_info_s OpenAPI_indirect_data_forwarding_tunnel_info_t;
typedef struct OpenAPI_indirect_data_forwarding_tunnel_info_s {
    char *ipv4_addr;
    char *ipv6_addr;
    char *gtp_teid;
    bool is_drb_id;
    int drb_id;
    bool is_additional_tnl_nb;
    int additional_tnl_nb;
} OpenAPI_indirect_data_forwarding_tunnel_info_t;

OpenAPI_indirect_data_forwarding_tunnel_info_t *OpenAPI_indirect_data_forwarding_tunnel_info_create(
    char *ipv4_addr,
    char *ipv6_addr,
    char *gtp_teid,
    bool is_drb_id,
    int drb_id,
    bool is_additional_tnl_nb,
    int additional_tnl_nb
);
void OpenAPI_indirect_data_forwarding_tunnel_info_free(OpenAPI_indirect_data_forwarding_tunnel_info_t *indirect_data_forwarding_tunnel_info);
OpenAPI_indirect_data_forwarding_tunnel_info_t *OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON(cJSON *indirect_data_forwarding_tunnel_infoJSON);
cJSON *OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON(OpenAPI_indirect_data_forwarding_tunnel_info_t *indirect_data_forwarding_tunnel_info);
OpenAPI_indirect_data_forwarding_tunnel_info_t *OpenAPI_indirect_data_forwarding_tunnel_info_copy(OpenAPI_indirect_data_forwarding_tunnel_info_t *dst, OpenAPI_indirect_data_forwarding_tunnel_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_indirect_data_forwarding_tunnel_info_H_ */

