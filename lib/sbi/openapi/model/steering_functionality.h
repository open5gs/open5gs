/*
 * steering_functionality.h
 *
 * Possible values are   - MPTCP: Indicates that PCF authorizes the MPTCP functionality to support traffic steering, switching and splitting.   - ATSSS_LL: Indicates that PCF authorizes the ATSSS-LL functionality to support traffic steering, switching and splitting.
 */

#ifndef _OpenAPI_steering_functionality_H_
#define _OpenAPI_steering_functionality_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_steering_functionality_s OpenAPI_steering_functionality_t;
typedef struct OpenAPI_steering_functionality_s {
} OpenAPI_steering_functionality_t;

OpenAPI_steering_functionality_t *OpenAPI_steering_functionality_create(
    );
void OpenAPI_steering_functionality_free(OpenAPI_steering_functionality_t *steering_functionality);
OpenAPI_steering_functionality_t *OpenAPI_steering_functionality_parseFromJSON(cJSON *steering_functionalityJSON);
cJSON *OpenAPI_steering_functionality_convertToJSON(OpenAPI_steering_functionality_t *steering_functionality);
OpenAPI_steering_functionality_t *OpenAPI_steering_functionality_copy(OpenAPI_steering_functionality_t *dst, OpenAPI_steering_functionality_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_steering_functionality_H_ */

