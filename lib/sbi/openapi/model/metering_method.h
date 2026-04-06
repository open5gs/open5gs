/*
 * metering_method.h
 *
 * Indicates the metering method.   Possible values are: - DURATION: Indicates that the duration of the service data flow traffic shall be metered. - VOLUME: Indicates that volume of the service data flow traffic shall be metered. - DURATION_VOLUME: Indicates that the duration and the volume of the service data flow traffic shall be metered. - EVENT: Indicates that events of the service data flow traffic shall be metered. 
 */

#ifndef _OpenAPI_metering_method_H_
#define _OpenAPI_metering_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_metering_method_NULL = 0, OpenAPI_metering_method_DURATION, OpenAPI_metering_method_VOLUME, OpenAPI_metering_method_DURATION_VOLUME, OpenAPI_metering_method_EVENT } OpenAPI_metering_method_e;

char* OpenAPI_metering_method_ToString(OpenAPI_metering_method_e metering_method);

OpenAPI_metering_method_e OpenAPI_metering_method_FromString(char* metering_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_metering_method_H_ */

