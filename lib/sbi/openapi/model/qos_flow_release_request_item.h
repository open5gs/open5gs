/*
 * qos_flow_release_request_item.h
 *
 * 
 */

#ifndef _OpenAPI_qos_flow_release_request_item_H_
#define _OpenAPI_qos_flow_release_request_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_flow_release_request_item_s OpenAPI_qos_flow_release_request_item_t;
typedef struct OpenAPI_qos_flow_release_request_item_s {
    int qfi;
    char qos_rules;
    char qos_flow_description;
} OpenAPI_qos_flow_release_request_item_t;

OpenAPI_qos_flow_release_request_item_t *OpenAPI_qos_flow_release_request_item_create(
    int qfi,
    char qos_rules,
    char qos_flow_description
);
void OpenAPI_qos_flow_release_request_item_free(OpenAPI_qos_flow_release_request_item_t *qos_flow_release_request_item);
OpenAPI_qos_flow_release_request_item_t *OpenAPI_qos_flow_release_request_item_parseFromJSON(cJSON *qos_flow_release_request_itemJSON);
cJSON *OpenAPI_qos_flow_release_request_item_convertToJSON(OpenAPI_qos_flow_release_request_item_t *qos_flow_release_request_item);
OpenAPI_qos_flow_release_request_item_t *OpenAPI_qos_flow_release_request_item_copy(OpenAPI_qos_flow_release_request_item_t *dst, OpenAPI_qos_flow_release_request_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_release_request_item_H_ */

