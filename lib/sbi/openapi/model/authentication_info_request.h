/*
 * authentication_info_request.h
 *
 * 
 */

#ifndef _OpenAPI_authentication_info_request_H_
#define _OpenAPI_authentication_info_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "resynchronization_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_authentication_info_request_s OpenAPI_authentication_info_request_t;
typedef struct OpenAPI_authentication_info_request_s {
    char *supported_features;
    char *serving_network_name;
    struct OpenAPI_resynchronization_info_s *resynchronization_info;
    char *ausf_instance_id;
    OpenAPI_list_t *cell_cag_info;
    bool is_n5gc_ind;
    int n5gc_ind;
} OpenAPI_authentication_info_request_t;

OpenAPI_authentication_info_request_t *OpenAPI_authentication_info_request_create(
    char *supported_features,
    char *serving_network_name,
    OpenAPI_resynchronization_info_t *resynchronization_info,
    char *ausf_instance_id,
    OpenAPI_list_t *cell_cag_info,
    bool is_n5gc_ind,
    int n5gc_ind
);
void OpenAPI_authentication_info_request_free(OpenAPI_authentication_info_request_t *authentication_info_request);
OpenAPI_authentication_info_request_t *OpenAPI_authentication_info_request_parseFromJSON(cJSON *authentication_info_requestJSON);
cJSON *OpenAPI_authentication_info_request_convertToJSON(OpenAPI_authentication_info_request_t *authentication_info_request);
OpenAPI_authentication_info_request_t *OpenAPI_authentication_info_request_copy(OpenAPI_authentication_info_request_t *dst, OpenAPI_authentication_info_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_authentication_info_request_H_ */

