/*
 * operator_specific_data_container_value.h
 *
 * 
 */

#ifndef _OpenAPI_operator_specific_data_container_value_H_
#define _OpenAPI_operator_specific_data_container_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_operator_specific_data_container_value_s OpenAPI_operator_specific_data_container_value_t;
typedef struct OpenAPI_operator_specific_data_container_value_s {
} OpenAPI_operator_specific_data_container_value_t;

OpenAPI_operator_specific_data_container_value_t *OpenAPI_operator_specific_data_container_value_create(
);
void OpenAPI_operator_specific_data_container_value_free(OpenAPI_operator_specific_data_container_value_t *operator_specific_data_container_value);
OpenAPI_operator_specific_data_container_value_t *OpenAPI_operator_specific_data_container_value_parseFromJSON(cJSON *operator_specific_data_container_valueJSON);
cJSON *OpenAPI_operator_specific_data_container_value_convertToJSON(OpenAPI_operator_specific_data_container_value_t *operator_specific_data_container_value);
OpenAPI_operator_specific_data_container_value_t *OpenAPI_operator_specific_data_container_value_copy(OpenAPI_operator_specific_data_container_value_t *dst, OpenAPI_operator_specific_data_container_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_operator_specific_data_container_value_H_ */

