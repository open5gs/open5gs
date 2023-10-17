/*
 * ecs_addr_config_info.h
 *
 * 
 */

#ifndef _OpenAPI_ecs_addr_config_info_H_
#define _OpenAPI_ecs_addr_config_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ecs_server_addr.h"
#include "spatial_validity_cond.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ecs_addr_config_info_s OpenAPI_ecs_addr_config_info_t;
typedef struct OpenAPI_ecs_addr_config_info_s {
    struct OpenAPI_ecs_server_addr_s *ecs_server_addr;
    struct OpenAPI_spatial_validity_cond_s *spatial_validity_cond;
} OpenAPI_ecs_addr_config_info_t;

OpenAPI_ecs_addr_config_info_t *OpenAPI_ecs_addr_config_info_create(
    OpenAPI_ecs_server_addr_t *ecs_server_addr,
    OpenAPI_spatial_validity_cond_t *spatial_validity_cond
);
void OpenAPI_ecs_addr_config_info_free(OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info);
OpenAPI_ecs_addr_config_info_t *OpenAPI_ecs_addr_config_info_parseFromJSON(cJSON *ecs_addr_config_infoJSON);
cJSON *OpenAPI_ecs_addr_config_info_convertToJSON(OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info);
OpenAPI_ecs_addr_config_info_t *OpenAPI_ecs_addr_config_info_copy(OpenAPI_ecs_addr_config_info_t *dst, OpenAPI_ecs_addr_config_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ecs_addr_config_info_H_ */

