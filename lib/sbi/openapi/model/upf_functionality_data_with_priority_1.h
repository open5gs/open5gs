/*
 * upf_functionality_data_with_priority_1.h
 *
 * UPF Functionality Data with an optional priority
 */

#ifndef _OpenAPI_upf_functionality_data_with_priority_1_H_
#define _OpenAPI_upf_functionality_data_with_priority_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_upf_functionality_data_with_priority_1_s OpenAPI_upf_functionality_data_with_priority_1_t;
#include "upf_functionality_data_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_upf_functionality_data_with_priority_1_s {
    bool is_priority;
    int priority;
    struct OpenAPI_upf_functionality_data_1_s *upf_functionality;
};

OpenAPI_upf_functionality_data_with_priority_1_t *OpenAPI_upf_functionality_data_with_priority_1_create(
    bool is_priority,
    int priority,
    OpenAPI_upf_functionality_data_1_t *upf_functionality
);
void OpenAPI_upf_functionality_data_with_priority_1_free(OpenAPI_upf_functionality_data_with_priority_1_t *upf_functionality_data_with_priority_1);
OpenAPI_upf_functionality_data_with_priority_1_t *OpenAPI_upf_functionality_data_with_priority_1_parseFromJSON(cJSON *upf_functionality_data_with_priority_1JSON);
cJSON *OpenAPI_upf_functionality_data_with_priority_1_convertToJSON(OpenAPI_upf_functionality_data_with_priority_1_t *upf_functionality_data_with_priority_1);
OpenAPI_upf_functionality_data_with_priority_1_t *OpenAPI_upf_functionality_data_with_priority_1_copy(OpenAPI_upf_functionality_data_with_priority_1_t *dst, OpenAPI_upf_functionality_data_with_priority_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_upf_functionality_data_with_priority_1_H_ */

