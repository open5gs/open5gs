/*
 * query_parameter.h
 *
 * Contains the name and value of a query parameter
 */

#ifndef _OpenAPI_query_parameter_H_
#define _OpenAPI_query_parameter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_query_parameter_s OpenAPI_query_parameter_t;
typedef struct OpenAPI_query_parameter_s {
    char *name;
    char *value;
} OpenAPI_query_parameter_t;

OpenAPI_query_parameter_t *OpenAPI_query_parameter_create(
    char *name,
    char *value
);
void OpenAPI_query_parameter_free(OpenAPI_query_parameter_t *query_parameter);
OpenAPI_query_parameter_t *OpenAPI_query_parameter_parseFromJSON(cJSON *query_parameterJSON);
cJSON *OpenAPI_query_parameter_convertToJSON(OpenAPI_query_parameter_t *query_parameter);
OpenAPI_query_parameter_t *OpenAPI_query_parameter_copy(OpenAPI_query_parameter_t *dst, OpenAPI_query_parameter_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_query_parameter_H_ */

