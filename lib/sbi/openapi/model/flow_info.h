/*
 * flow_info.h
 *
 * 
 */

#ifndef _OpenAPI_flow_info_H_
#define _OpenAPI_flow_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_flow_info_s OpenAPI_flow_info_t;
typedef struct OpenAPI_flow_info_s {
    int flow_id;
    OpenAPI_list_t *flow_descriptions;
} OpenAPI_flow_info_t;

OpenAPI_flow_info_t *OpenAPI_flow_info_create(
    int flow_id,
    OpenAPI_list_t *flow_descriptions
);
void OpenAPI_flow_info_free(OpenAPI_flow_info_t *flow_info);
OpenAPI_flow_info_t *OpenAPI_flow_info_parseFromJSON(cJSON *flow_infoJSON);
cJSON *OpenAPI_flow_info_convertToJSON(OpenAPI_flow_info_t *flow_info);
OpenAPI_flow_info_t *OpenAPI_flow_info_copy(OpenAPI_flow_info_t *dst, OpenAPI_flow_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_flow_info_H_ */

