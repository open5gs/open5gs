/*
 * api_ie_mapping.h
 *
 * API URI to IE mapping on which the protection policy needs to be applied
 */

#ifndef _OpenAPI_api_ie_mapping_H_
#define _OpenAPI_api_ie_mapping_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "api_signature.h"
#include "http_method.h"
#include "ie_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_api_ie_mapping_s OpenAPI_api_ie_mapping_t;
typedef struct OpenAPI_api_ie_mapping_s {
    struct OpenAPI_api_signature_s *api_signature;
    OpenAPI_http_method_e api_method;
    OpenAPI_list_t *ie_list;
} OpenAPI_api_ie_mapping_t;

OpenAPI_api_ie_mapping_t *OpenAPI_api_ie_mapping_create(
    OpenAPI_api_signature_t *api_signature,
    OpenAPI_http_method_e api_method,
    OpenAPI_list_t *ie_list
);
void OpenAPI_api_ie_mapping_free(OpenAPI_api_ie_mapping_t *api_ie_mapping);
OpenAPI_api_ie_mapping_t *OpenAPI_api_ie_mapping_parseFromJSON(cJSON *api_ie_mappingJSON);
cJSON *OpenAPI_api_ie_mapping_convertToJSON(OpenAPI_api_ie_mapping_t *api_ie_mapping);
OpenAPI_api_ie_mapping_t *OpenAPI_api_ie_mapping_copy(OpenAPI_api_ie_mapping_t *dst, OpenAPI_api_ie_mapping_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_api_ie_mapping_H_ */

