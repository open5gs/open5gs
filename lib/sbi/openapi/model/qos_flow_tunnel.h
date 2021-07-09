/*
 * qos_flow_tunnel.h
 *
 * Tunnel Information per QoS Flow
 */

#ifndef _OpenAPI_qos_flow_tunnel_H_
#define _OpenAPI_qos_flow_tunnel_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "tunnel_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_flow_tunnel_s OpenAPI_qos_flow_tunnel_t;
typedef struct OpenAPI_qos_flow_tunnel_s {
    OpenAPI_list_t *qfi_list;
    struct OpenAPI_tunnel_info_s *tunnel_info;
} OpenAPI_qos_flow_tunnel_t;

OpenAPI_qos_flow_tunnel_t *OpenAPI_qos_flow_tunnel_create(
    OpenAPI_list_t *qfi_list,
    OpenAPI_tunnel_info_t *tunnel_info
);
void OpenAPI_qos_flow_tunnel_free(OpenAPI_qos_flow_tunnel_t *qos_flow_tunnel);
OpenAPI_qos_flow_tunnel_t *OpenAPI_qos_flow_tunnel_parseFromJSON(cJSON *qos_flow_tunnelJSON);
cJSON *OpenAPI_qos_flow_tunnel_convertToJSON(OpenAPI_qos_flow_tunnel_t *qos_flow_tunnel);
OpenAPI_qos_flow_tunnel_t *OpenAPI_qos_flow_tunnel_copy(OpenAPI_qos_flow_tunnel_t *dst, OpenAPI_qos_flow_tunnel_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_tunnel_H_ */

