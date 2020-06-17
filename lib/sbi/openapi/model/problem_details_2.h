/*
 * problem_details_2.h
 *
 *
 */

#ifndef _OpenAPI_problem_details_2_H_
#define _OpenAPI_problem_details_2_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "invalid_param.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_problem_details_2_s OpenAPI_problem_details_2_t;
typedef struct OpenAPI_problem_details_2_s {
    char *type;
    char *title;
    int status;
    char *detail;
    char *instance;
    char *cause;
    OpenAPI_list_t *invalid_params;
    char *supported_features;
} OpenAPI_problem_details_2_t;

OpenAPI_problem_details_2_t *OpenAPI_problem_details_2_create(
    char *type,
    char *title,
    int status,
    char *detail,
    char *instance,
    char *cause,
    OpenAPI_list_t *invalid_params,
    char *supported_features
    );
void OpenAPI_problem_details_2_free(OpenAPI_problem_details_2_t *problem_details_2);
OpenAPI_problem_details_2_t *OpenAPI_problem_details_2_parseFromJSON(cJSON *problem_details_2JSON);
cJSON *OpenAPI_problem_details_2_convertToJSON(OpenAPI_problem_details_2_t *problem_details_2);
OpenAPI_problem_details_2_t *OpenAPI_problem_details_2_copy(OpenAPI_problem_details_2_t *dst, OpenAPI_problem_details_2_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_problem_details_2_H_ */

