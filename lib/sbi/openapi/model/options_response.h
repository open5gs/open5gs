/*
 * options_response.h
 *
 * Communication options of the NRF sent in response payload of OPTIONS method
 */

#ifndef _OpenAPI_options_response_H_
#define _OpenAPI_options_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_options_response_s OpenAPI_options_response_t;
typedef struct OpenAPI_options_response_s {
    char *supported_features;
} OpenAPI_options_response_t;

OpenAPI_options_response_t *OpenAPI_options_response_create(
    char *supported_features
);
void OpenAPI_options_response_free(OpenAPI_options_response_t *options_response);
OpenAPI_options_response_t *OpenAPI_options_response_parseFromJSON(cJSON *options_responseJSON);
cJSON *OpenAPI_options_response_convertToJSON(OpenAPI_options_response_t *options_response);
OpenAPI_options_response_t *OpenAPI_options_response_copy(OpenAPI_options_response_t *dst, OpenAPI_options_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_options_response_H_ */

