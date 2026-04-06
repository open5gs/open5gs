/*
 * ecs_addr_data.h
 *
 * Represents ECS Address Data.
 */

#ifndef _OpenAPI_ecs_addr_data_H_
#define _OpenAPI_ecs_addr_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ecs_addr_data_s OpenAPI_ecs_addr_data_t;
#include "ecs_auth_method.h"
#include "ecs_server_addr.h"
#include "snssai.h"
#include "spatial_validity_cond_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ecs_addr_data_s {
    char *self;
    struct OpenAPI_ecs_server_addr_s *ecs_server_addr;
    struct OpenAPI_spatial_validity_cond_1_s *spatial_validity_cond;
    OpenAPI_list_t *ecs_auth_methods;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    char *supp_feat;
};

OpenAPI_ecs_addr_data_t *OpenAPI_ecs_addr_data_create(
    char *self,
    OpenAPI_ecs_server_addr_t *ecs_server_addr,
    OpenAPI_spatial_validity_cond_1_t *spatial_validity_cond,
    OpenAPI_list_t *ecs_auth_methods,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *supp_feat
);
void OpenAPI_ecs_addr_data_free(OpenAPI_ecs_addr_data_t *ecs_addr_data);
OpenAPI_ecs_addr_data_t *OpenAPI_ecs_addr_data_parseFromJSON(cJSON *ecs_addr_dataJSON);
cJSON *OpenAPI_ecs_addr_data_convertToJSON(OpenAPI_ecs_addr_data_t *ecs_addr_data);
OpenAPI_ecs_addr_data_t *OpenAPI_ecs_addr_data_copy(OpenAPI_ecs_addr_data_t *dst, OpenAPI_ecs_addr_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ecs_addr_data_H_ */

