/*
 * metering_method.h
 *
 * Possible values are - DURATION: Indicates that the duration of the service data flow traffic shall be metered. - VOLUME: Indicates that volume of the service data flow traffic shall be metered. - DURATION_VOLUME: Indicates that the duration and the volume of the service data flow traffic shall be metered. - EVENT: Indicates that events of the service data flow traffic shall be metered.
 */

#ifndef _OpenAPI_metering_method_H_
#define _OpenAPI_metering_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "null_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_metering_method_s OpenAPI_metering_method_t;
typedef struct OpenAPI_metering_method_s {
} OpenAPI_metering_method_t;

OpenAPI_metering_method_t *OpenAPI_metering_method_create(
    );
void OpenAPI_metering_method_free(OpenAPI_metering_method_t *metering_method);
OpenAPI_metering_method_t *OpenAPI_metering_method_parseFromJSON(cJSON *metering_methodJSON);
cJSON *OpenAPI_metering_method_convertToJSON(OpenAPI_metering_method_t *metering_method);
OpenAPI_metering_method_t *OpenAPI_metering_method_copy(OpenAPI_metering_method_t *dst, OpenAPI_metering_method_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_metering_method_H_ */

