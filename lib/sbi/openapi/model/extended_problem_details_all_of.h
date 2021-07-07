/*
 * extended_problem_details_all_of.h
 *
 * 
 */

#ifndef _OpenAPI_extended_problem_details_all_of_H_
#define _OpenAPI_extended_problem_details_all_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "acceptable_service_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_extended_problem_details_all_of_s OpenAPI_extended_problem_details_all_of_t;
typedef struct OpenAPI_extended_problem_details_all_of_s {
    struct OpenAPI_acceptable_service_info_s *acceptable_serv_info;
} OpenAPI_extended_problem_details_all_of_t;

OpenAPI_extended_problem_details_all_of_t *OpenAPI_extended_problem_details_all_of_create(
    OpenAPI_acceptable_service_info_t *acceptable_serv_info
);
void OpenAPI_extended_problem_details_all_of_free(OpenAPI_extended_problem_details_all_of_t *extended_problem_details_all_of);
OpenAPI_extended_problem_details_all_of_t *OpenAPI_extended_problem_details_all_of_parseFromJSON(cJSON *extended_problem_details_all_ofJSON);
cJSON *OpenAPI_extended_problem_details_all_of_convertToJSON(OpenAPI_extended_problem_details_all_of_t *extended_problem_details_all_of);
OpenAPI_extended_problem_details_all_of_t *OpenAPI_extended_problem_details_all_of_copy(OpenAPI_extended_problem_details_all_of_t *dst, OpenAPI_extended_problem_details_all_of_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_extended_problem_details_all_of_H_ */

