/*
 * pc5_qos_mapping.h
 *
 *
 */

#ifndef _OpenAPI_pc5_qos_mapping_H_
#define _OpenAPI_pc5_qos_mapping_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pc5_qos_flow_item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pc5_qos_mapping_s OpenAPI_pc5_qos_mapping_t;
typedef struct OpenAPI_pc5_qos_mapping_s {
    OpenAPI_list_t *ser_ids;
    struct OpenAPI_pc5_qos_flow_item_s *pc5_qo_s_flow_param;
} OpenAPI_pc5_qos_mapping_t;

OpenAPI_pc5_qos_mapping_t *OpenAPI_pc5_qos_mapping_create(
    OpenAPI_list_t *ser_ids,
    OpenAPI_pc5_qos_flow_item_t *pc5_qo_s_flow_param
    );
void OpenAPI_pc5_qos_mapping_free(OpenAPI_pc5_qos_mapping_t *pc5_qos_mapping);
OpenAPI_pc5_qos_mapping_t *OpenAPI_pc5_qos_mapping_parseFromJSON(cJSON *pc5_qos_mappingJSON);
cJSON *OpenAPI_pc5_qos_mapping_convertToJSON(OpenAPI_pc5_qos_mapping_t *pc5_qos_mapping);
OpenAPI_pc5_qos_mapping_t *OpenAPI_pc5_qos_mapping_copy(OpenAPI_pc5_qos_mapping_t *dst, OpenAPI_pc5_qos_mapping_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pc5_qos_mapping_H_ */

