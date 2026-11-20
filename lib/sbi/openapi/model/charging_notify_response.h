/*
 * charging_notify_response.h
 *
 * 
 */

#ifndef _OpenAPI_charging_notify_response_H_
#define _OpenAPI_charging_notify_response_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_charging_notify_response_s OpenAPI_charging_notify_response_t;
#include "invocation_result.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_charging_notify_response_s {
    struct OpenAPI_invocation_result_s *invocation_result;
};

OpenAPI_charging_notify_response_t *OpenAPI_charging_notify_response_create(
    OpenAPI_invocation_result_t *invocation_result
);
void OpenAPI_charging_notify_response_free(OpenAPI_charging_notify_response_t *charging_notify_response);
OpenAPI_charging_notify_response_t *OpenAPI_charging_notify_response_parseFromJSON(cJSON *charging_notify_responseJSON);
cJSON *OpenAPI_charging_notify_response_convertToJSON(OpenAPI_charging_notify_response_t *charging_notify_response);
OpenAPI_charging_notify_response_t *OpenAPI_charging_notify_response_copy(OpenAPI_charging_notify_response_t *dst, OpenAPI_charging_notify_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_notify_response_H_ */

