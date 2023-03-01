/*
 * get_nf_instances_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_get_nf_instances_200_response_H_
#define _OpenAPI_get_nf_instances_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "links_value_schema.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_get_nf_instances_200_response_s OpenAPI_get_nf_instances_200_response_t;
typedef struct OpenAPI_get_nf_instances_200_response_s {
    OpenAPI_list_t* _links;
} OpenAPI_get_nf_instances_200_response_t;

OpenAPI_get_nf_instances_200_response_t *OpenAPI_get_nf_instances_200_response_create(
    OpenAPI_list_t* _links
);
void OpenAPI_get_nf_instances_200_response_free(OpenAPI_get_nf_instances_200_response_t *get_nf_instances_200_response);
OpenAPI_get_nf_instances_200_response_t *OpenAPI_get_nf_instances_200_response_parseFromJSON(cJSON *get_nf_instances_200_responseJSON);
cJSON *OpenAPI_get_nf_instances_200_response_convertToJSON(OpenAPI_get_nf_instances_200_response_t *get_nf_instances_200_response);
OpenAPI_get_nf_instances_200_response_t *OpenAPI_get_nf_instances_200_response_copy(OpenAPI_get_nf_instances_200_response_t *dst, OpenAPI_get_nf_instances_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_get_nf_instances_200_response_H_ */

