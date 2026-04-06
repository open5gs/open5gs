/*
 * am_policy_info_container.h
 *
 * AM Policy Information Container
 */

#ifndef _OpenAPI_am_policy_info_container_H_
#define _OpenAPI_am_policy_info_container_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_am_policy_info_container_s OpenAPI_am_policy_info_container_t;
#include "slice_usage_control_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_am_policy_info_container_s {
    OpenAPI_list_t* slice_usg_ctrl_info_sets;
};

OpenAPI_am_policy_info_container_t *OpenAPI_am_policy_info_container_create(
    OpenAPI_list_t* slice_usg_ctrl_info_sets
);
void OpenAPI_am_policy_info_container_free(OpenAPI_am_policy_info_container_t *am_policy_info_container);
OpenAPI_am_policy_info_container_t *OpenAPI_am_policy_info_container_parseFromJSON(cJSON *am_policy_info_containerJSON);
cJSON *OpenAPI_am_policy_info_container_convertToJSON(OpenAPI_am_policy_info_container_t *am_policy_info_container);
OpenAPI_am_policy_info_container_t *OpenAPI_am_policy_info_container_copy(OpenAPI_am_policy_info_container_t *dst, OpenAPI_am_policy_info_container_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_am_policy_info_container_H_ */

