/*
 * search_result_info.h
 *
 * Contains additional information to the search result
 */

#ifndef _OpenAPI_search_result_info_H_
#define _OpenAPI_search_result_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_search_result_info_s OpenAPI_search_result_info_t;
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_search_result_info_s {
    OpenAPI_list_t *unsatisfied_tai_list;
};

OpenAPI_search_result_info_t *OpenAPI_search_result_info_create(
    OpenAPI_list_t *unsatisfied_tai_list
);
void OpenAPI_search_result_info_free(OpenAPI_search_result_info_t *search_result_info);
OpenAPI_search_result_info_t *OpenAPI_search_result_info_parseFromJSON(cJSON *search_result_infoJSON);
cJSON *OpenAPI_search_result_info_convertToJSON(OpenAPI_search_result_info_t *search_result_info);
OpenAPI_search_result_info_t *OpenAPI_search_result_info_copy(OpenAPI_search_result_info_t *dst, OpenAPI_search_result_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_search_result_info_H_ */

