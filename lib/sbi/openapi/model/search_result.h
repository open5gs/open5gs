/*
 * search_result.h
 *
 *
 */

#ifndef _OpenAPI_search_result_H_
#define _OpenAPI_search_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_profile.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_search_result_s OpenAPI_search_result_t;
typedef struct OpenAPI_search_result_s {
    int validity_period;
    OpenAPI_list_t *nf_instances;
    char *search_id;
    int num_nf_inst_complete;
    char *nrf_supported_features;
} OpenAPI_search_result_t;

OpenAPI_search_result_t *OpenAPI_search_result_create(
    int validity_period,
    OpenAPI_list_t *nf_instances,
    char *search_id,
    int num_nf_inst_complete,
    char *nrf_supported_features
    );
void OpenAPI_search_result_free(OpenAPI_search_result_t *search_result);
OpenAPI_search_result_t *OpenAPI_search_result_parseFromJSON(cJSON *search_resultJSON);
cJSON *OpenAPI_search_result_convertToJSON(OpenAPI_search_result_t *search_result);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_search_result_H_ */

