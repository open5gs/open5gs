/*
 * problem_details_add_info.h
 *
 * 
 */

#ifndef _OpenAPI_problem_details_add_info_H_
#define _OpenAPI_problem_details_add_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_problem_details_add_info_s OpenAPI_problem_details_add_info_t;
typedef struct OpenAPI_problem_details_add_info_s {
    bool is_remote_error;
    int remote_error;
} OpenAPI_problem_details_add_info_t;

OpenAPI_problem_details_add_info_t *OpenAPI_problem_details_add_info_create(
    bool is_remote_error,
    int remote_error
);
void OpenAPI_problem_details_add_info_free(OpenAPI_problem_details_add_info_t *problem_details_add_info);
OpenAPI_problem_details_add_info_t *OpenAPI_problem_details_add_info_parseFromJSON(cJSON *problem_details_add_infoJSON);
cJSON *OpenAPI_problem_details_add_info_convertToJSON(OpenAPI_problem_details_add_info_t *problem_details_add_info);
OpenAPI_problem_details_add_info_t *OpenAPI_problem_details_add_info_copy(OpenAPI_problem_details_add_info_t *dst, OpenAPI_problem_details_add_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_problem_details_add_info_H_ */

