/*
 * tunnel_info.h
 *
 * Tunnel Information
 */

#ifndef _OpenAPI_tunnel_info_H_
#define _OpenAPI_tunnel_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tunnel_info_s OpenAPI_tunnel_info_t;
typedef struct OpenAPI_tunnel_info_s {
    char *ipv4_addr;
    char *ipv6_addr;
    char *gtp_teid;
    OpenAPI_access_type_e an_type;
} OpenAPI_tunnel_info_t;

OpenAPI_tunnel_info_t *OpenAPI_tunnel_info_create(
    char *ipv4_addr,
    char *ipv6_addr,
    char *gtp_teid,
    OpenAPI_access_type_e an_type
);
void OpenAPI_tunnel_info_free(OpenAPI_tunnel_info_t *tunnel_info);
OpenAPI_tunnel_info_t *OpenAPI_tunnel_info_parseFromJSON(cJSON *tunnel_infoJSON);
cJSON *OpenAPI_tunnel_info_convertToJSON(OpenAPI_tunnel_info_t *tunnel_info);
OpenAPI_tunnel_info_t *OpenAPI_tunnel_info_copy(OpenAPI_tunnel_info_t *dst, OpenAPI_tunnel_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tunnel_info_H_ */

