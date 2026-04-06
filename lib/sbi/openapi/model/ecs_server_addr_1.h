/*
 * ecs_server_addr_1.h
 *
 * Contains the Edge Configuration Server Address Configuration Information as defined in clause 5.2.3.6.1 of 3GPP TS 23.502. 
 */

#ifndef _OpenAPI_ecs_server_addr_1_H_
#define _OpenAPI_ecs_server_addr_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ecs_server_addr_1_s OpenAPI_ecs_server_addr_1_t;
#include "ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ecs_server_addr_1_s {
    OpenAPI_list_t *ecs_fqdn_list;
    OpenAPI_list_t *ecs_ip_address_list;
    OpenAPI_list_t *ecs_uri_list;
    char *ecs_provider_id;
};

OpenAPI_ecs_server_addr_1_t *OpenAPI_ecs_server_addr_1_create(
    OpenAPI_list_t *ecs_fqdn_list,
    OpenAPI_list_t *ecs_ip_address_list,
    OpenAPI_list_t *ecs_uri_list,
    char *ecs_provider_id
);
void OpenAPI_ecs_server_addr_1_free(OpenAPI_ecs_server_addr_1_t *ecs_server_addr_1);
OpenAPI_ecs_server_addr_1_t *OpenAPI_ecs_server_addr_1_parseFromJSON(cJSON *ecs_server_addr_1JSON);
cJSON *OpenAPI_ecs_server_addr_1_convertToJSON(OpenAPI_ecs_server_addr_1_t *ecs_server_addr_1);
OpenAPI_ecs_server_addr_1_t *OpenAPI_ecs_server_addr_1_copy(OpenAPI_ecs_server_addr_1_t *dst, OpenAPI_ecs_server_addr_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ecs_server_addr_1_H_ */

