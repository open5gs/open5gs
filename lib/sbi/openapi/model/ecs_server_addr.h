/*
 * ecs_server_addr.h
 *
 * Contains the Edge Configuration Server Address Configuration Information as defined in clause 5.2.3.6.1 of 3GPP TS 23.502. 
 */

#ifndef _OpenAPI_ecs_server_addr_H_
#define _OpenAPI_ecs_server_addr_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ecs_server_addr_s OpenAPI_ecs_server_addr_t;
typedef struct OpenAPI_ecs_server_addr_s {
    OpenAPI_list_t *ecs_fqdn_list;
    OpenAPI_list_t *ecs_ip_address_list;
    OpenAPI_list_t *ecs_uri_list;
    char *ecs_provider_id;
} OpenAPI_ecs_server_addr_t;

OpenAPI_ecs_server_addr_t *OpenAPI_ecs_server_addr_create(
    OpenAPI_list_t *ecs_fqdn_list,
    OpenAPI_list_t *ecs_ip_address_list,
    OpenAPI_list_t *ecs_uri_list,
    char *ecs_provider_id
);
void OpenAPI_ecs_server_addr_free(OpenAPI_ecs_server_addr_t *ecs_server_addr);
OpenAPI_ecs_server_addr_t *OpenAPI_ecs_server_addr_parseFromJSON(cJSON *ecs_server_addrJSON);
cJSON *OpenAPI_ecs_server_addr_convertToJSON(OpenAPI_ecs_server_addr_t *ecs_server_addr);
OpenAPI_ecs_server_addr_t *OpenAPI_ecs_server_addr_copy(OpenAPI_ecs_server_addr_t *dst, OpenAPI_ecs_server_addr_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ecs_server_addr_H_ */

