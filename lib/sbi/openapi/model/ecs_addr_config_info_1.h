/*
 * ecs_addr_config_info_1.h
 *
 * 
 */

#ifndef _OpenAPI_ecs_addr_config_info_1_H_
#define _OpenAPI_ecs_addr_config_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ecs_server_addr.h"
#include "spatial_validity_cond_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ecs_addr_config_info_1_s OpenAPI_ecs_addr_config_info_1_t;
typedef struct OpenAPI_ecs_addr_config_info_1_s {
    struct OpenAPI_ecs_server_addr_s *ecs_server_addr;
    struct OpenAPI_spatial_validity_cond_1_s *spatial_validity_cond;
} OpenAPI_ecs_addr_config_info_1_t;

OpenAPI_ecs_addr_config_info_1_t *OpenAPI_ecs_addr_config_info_1_create(
    OpenAPI_ecs_server_addr_t *ecs_server_addr,
    OpenAPI_spatial_validity_cond_1_t *spatial_validity_cond
);
void OpenAPI_ecs_addr_config_info_1_free(OpenAPI_ecs_addr_config_info_1_t *ecs_addr_config_info_1);
OpenAPI_ecs_addr_config_info_1_t *OpenAPI_ecs_addr_config_info_1_parseFromJSON(cJSON *ecs_addr_config_info_1JSON);
cJSON *OpenAPI_ecs_addr_config_info_1_convertToJSON(OpenAPI_ecs_addr_config_info_1_t *ecs_addr_config_info_1);
OpenAPI_ecs_addr_config_info_1_t *OpenAPI_ecs_addr_config_info_1_copy(OpenAPI_ecs_addr_config_info_1_t *dst, OpenAPI_ecs_addr_config_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ecs_addr_config_info_1_H_ */

