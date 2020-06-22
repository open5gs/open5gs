/*
 * qos_flow_add_modify_request_item.h
 *
 *
 */

#ifndef _OpenAPI_qos_flow_add_modify_request_item_H_
#define _OpenAPI_qos_flow_add_modify_request_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "qos_flow_access_type.h"
#include "qos_flow_profile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_flow_add_modify_request_item_s OpenAPI_qos_flow_add_modify_request_item_t;
typedef struct OpenAPI_qos_flow_add_modify_request_item_s {
    int qfi;
    int ebi;
    char qos_rules;
    char qos_flow_description;
    struct OpenAPI_qos_flow_profile_s *qos_flow_profile;
    OpenAPI_qos_flow_access_type_e associated_an_type;
} OpenAPI_qos_flow_add_modify_request_item_t;

OpenAPI_qos_flow_add_modify_request_item_t *OpenAPI_qos_flow_add_modify_request_item_create(
    int qfi,
    int ebi,
    char qos_rules,
    char qos_flow_description,
    OpenAPI_qos_flow_profile_t *qos_flow_profile,
    OpenAPI_qos_flow_access_type_e associated_an_type
    );
void OpenAPI_qos_flow_add_modify_request_item_free(OpenAPI_qos_flow_add_modify_request_item_t *qos_flow_add_modify_request_item);
OpenAPI_qos_flow_add_modify_request_item_t *OpenAPI_qos_flow_add_modify_request_item_parseFromJSON(cJSON *qos_flow_add_modify_request_itemJSON);
cJSON *OpenAPI_qos_flow_add_modify_request_item_convertToJSON(OpenAPI_qos_flow_add_modify_request_item_t *qos_flow_add_modify_request_item);
OpenAPI_qos_flow_add_modify_request_item_t *OpenAPI_qos_flow_add_modify_request_item_copy(OpenAPI_qos_flow_add_modify_request_item_t *dst, OpenAPI_qos_flow_add_modify_request_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_add_modify_request_item_H_ */

