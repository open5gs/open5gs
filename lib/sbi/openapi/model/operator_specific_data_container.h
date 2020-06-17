/*
 * operator_specific_data_container.h
 *
 *
 */

#ifndef _OpenAPI_operator_specific_data_container_H_
#define _OpenAPI_operator_specific_data_container_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_operator_specific_data_container_s OpenAPI_operator_specific_data_container_t;
typedef enum { OpenAPI_operator_specific_data_container_DATATYPE_NULL = 0, OpenAPI_operator_specific_data_container_DATATYPE_string, OpenAPI_operator_specific_data_container_DATATYPE_integer, OpenAPI_operator_specific_data_container_DATATYPE_number, OpenAPI_operator_specific_data_container_DATATYPE_boolean, OpenAPI_operator_specific_data_container_DATATYPE_object } OpenAPI_operator_specific_data_container_data_type_e;

char* OpenAPI_operator_specific_data_container_data_type_ToString(OpenAPI_operator_specific_data_container_data_type_e data_type);

OpenAPI_operator_specific_data_container_data_type_e OpenAPI_operator_specific_data_container_data_type_FromString(char* data_type);
typedef struct OpenAPI_operator_specific_data_container_s {
    OpenAPI_operator_specific_data_container_data_type_e data_type;
    char *data_type_definition;
    char *value;
    char *supported_features;
} OpenAPI_operator_specific_data_container_t;

OpenAPI_operator_specific_data_container_t *OpenAPI_operator_specific_data_container_create(
    OpenAPI_operator_specific_data_container_data_type_e data_type,
    char *data_type_definition,
    char *value,
    char *supported_features
    );
void OpenAPI_operator_specific_data_container_free(OpenAPI_operator_specific_data_container_t *operator_specific_data_container);
OpenAPI_operator_specific_data_container_t *OpenAPI_operator_specific_data_container_parseFromJSON(cJSON *operator_specific_data_containerJSON);
cJSON *OpenAPI_operator_specific_data_container_convertToJSON(OpenAPI_operator_specific_data_container_t *operator_specific_data_container);
OpenAPI_operator_specific_data_container_t *OpenAPI_operator_specific_data_container_copy(OpenAPI_operator_specific_data_container_t *dst, OpenAPI_operator_specific_data_container_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_operator_specific_data_container_H_ */

