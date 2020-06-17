/*
 * problem_details.h
 *
 *
 */

#ifndef _OpenAPI_problem_details_H_
#define _OpenAPI_problem_details_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "invalid_param.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_problem_details_s OpenAPI_problem_details_t;
typedef struct OpenAPI_problem_details_s {
    char *type;
    char *title;
    int status;
    char *detail;
    char *instance;
    char *cause;
    OpenAPI_list_t *invalid_params;
    char *supported_features;
} OpenAPI_problem_details_t;

OpenAPI_problem_details_t *OpenAPI_problem_details_create(
    char *type,
    char *title,
    int status,
    char *detail,
    char *instance,
    char *cause,
    OpenAPI_list_t *invalid_params,
    char *supported_features
    );
void OpenAPI_problem_details_free(OpenAPI_problem_details_t *problem_details);
OpenAPI_problem_details_t *OpenAPI_problem_details_parseFromJSON(cJSON *problem_detailsJSON);
cJSON *OpenAPI_problem_details_convertToJSON(OpenAPI_problem_details_t *problem_details);
OpenAPI_problem_details_t *OpenAPI_problem_details_copy(OpenAPI_problem_details_t *dst, OpenAPI_problem_details_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_problem_details_H_ */

