/*
 * stored_search_result.h
 *
 *
 */

#ifndef _OpenAPI_stored_search_result_H_
#define _OpenAPI_stored_search_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_profile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_stored_search_result_s OpenAPI_stored_search_result_t;
typedef struct OpenAPI_stored_search_result_s {
    OpenAPI_list_t *nf_instances;
} OpenAPI_stored_search_result_t;

OpenAPI_stored_search_result_t *OpenAPI_stored_search_result_create(
    OpenAPI_list_t *nf_instances
    );
void OpenAPI_stored_search_result_free(OpenAPI_stored_search_result_t *stored_search_result);
OpenAPI_stored_search_result_t *OpenAPI_stored_search_result_parseFromJSON(cJSON *stored_search_resultJSON);
cJSON *OpenAPI_stored_search_result_convertToJSON(OpenAPI_stored_search_result_t *stored_search_result);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_stored_search_result_H_ */

