/*
 * response_time.h
 *
 * Indicates acceptable delay of location request.
 */

#ifndef _OpenAPI_response_time_H_
#define _OpenAPI_response_time_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "response_time_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_response_time_s OpenAPI_response_time_t;
typedef struct OpenAPI_response_time_s {
} OpenAPI_response_time_t;

OpenAPI_response_time_t *OpenAPI_response_time_create(void);
void OpenAPI_response_time_free(OpenAPI_response_time_t *response_time);
OpenAPI_response_time_t *OpenAPI_response_time_parseFromJSON(cJSON *response_timeJSON);
cJSON *OpenAPI_response_time_convertToJSON(OpenAPI_response_time_t *response_time);
OpenAPI_response_time_t *OpenAPI_response_time_copy(OpenAPI_response_time_t *dst, OpenAPI_response_time_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_response_time_H_ */

