/*
 * problem_details_1.h
 *
 * 
 */

#ifndef _OpenAPI_problem_details_1_H_
#define _OpenAPI_problem_details_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "invalid_param_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_problem_details_1_s OpenAPI_problem_details_1_t;
typedef struct OpenAPI_problem_details_1_s {
    char *type;
    char *title;
    bool is_status;
    int status;
    char *detail;
    char *instance;
    char *cause;
    OpenAPI_list_t *invalid_params;
} OpenAPI_problem_details_1_t;

OpenAPI_problem_details_1_t *OpenAPI_problem_details_1_create(
    char *type,
    char *title,
    bool is_status,
    int status,
    char *detail,
    char *instance,
    char *cause,
    OpenAPI_list_t *invalid_params
);
void OpenAPI_problem_details_1_free(OpenAPI_problem_details_1_t *problem_details_1);
OpenAPI_problem_details_1_t *OpenAPI_problem_details_1_parseFromJSON(cJSON *problem_details_1JSON);
cJSON *OpenAPI_problem_details_1_convertToJSON(OpenAPI_problem_details_1_t *problem_details_1);
OpenAPI_problem_details_1_t *OpenAPI_problem_details_1_copy(OpenAPI_problem_details_1_t *dst, OpenAPI_problem_details_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_problem_details_1_H_ */

