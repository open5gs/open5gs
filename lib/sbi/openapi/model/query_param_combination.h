/*
 * query_param_combination.h
 *
 * Contains a list of Query Parameters
 */

#ifndef _OpenAPI_query_param_combination_H_
#define _OpenAPI_query_param_combination_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "query_parameter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_query_param_combination_s OpenAPI_query_param_combination_t;
typedef struct OpenAPI_query_param_combination_s {
    OpenAPI_list_t *query_params;
} OpenAPI_query_param_combination_t;

OpenAPI_query_param_combination_t *OpenAPI_query_param_combination_create(
    OpenAPI_list_t *query_params
);
void OpenAPI_query_param_combination_free(OpenAPI_query_param_combination_t *query_param_combination);
OpenAPI_query_param_combination_t *OpenAPI_query_param_combination_parseFromJSON(cJSON *query_param_combinationJSON);
cJSON *OpenAPI_query_param_combination_convertToJSON(OpenAPI_query_param_combination_t *query_param_combination);
OpenAPI_query_param_combination_t *OpenAPI_query_param_combination_copy(OpenAPI_query_param_combination_t *dst, OpenAPI_query_param_combination_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_query_param_combination_H_ */

