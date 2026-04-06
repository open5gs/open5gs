/*
 * upf_functionality_data_with_priority.h
 *
 * UPF Functionality Data with an optional priority
 */

#ifndef _OpenAPI_upf_functionality_data_with_priority_H_
#define _OpenAPI_upf_functionality_data_with_priority_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_upf_functionality_data_with_priority_s OpenAPI_upf_functionality_data_with_priority_t;
#include "upf_functionality_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_upf_functionality_data_with_priority_s {
    bool is_priority;
    int priority;
    struct OpenAPI_upf_functionality_data_s *upf_functionality;
};

OpenAPI_upf_functionality_data_with_priority_t *OpenAPI_upf_functionality_data_with_priority_create(
    bool is_priority,
    int priority,
    OpenAPI_upf_functionality_data_t *upf_functionality
);
void OpenAPI_upf_functionality_data_with_priority_free(OpenAPI_upf_functionality_data_with_priority_t *upf_functionality_data_with_priority);
OpenAPI_upf_functionality_data_with_priority_t *OpenAPI_upf_functionality_data_with_priority_parseFromJSON(cJSON *upf_functionality_data_with_priorityJSON);
cJSON *OpenAPI_upf_functionality_data_with_priority_convertToJSON(OpenAPI_upf_functionality_data_with_priority_t *upf_functionality_data_with_priority);
OpenAPI_upf_functionality_data_with_priority_t *OpenAPI_upf_functionality_data_with_priority_copy(OpenAPI_upf_functionality_data_with_priority_t *dst, OpenAPI_upf_functionality_data_with_priority_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upf_functionality_data_with_priority_H_ */

