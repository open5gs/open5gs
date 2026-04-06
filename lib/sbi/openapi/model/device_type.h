/*
 * device_type.h
 *
 * 
 */

#ifndef _OpenAPI_device_type_H_
#define _OpenAPI_device_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_device_type_NULL = 0, OpenAPI_device_type_MOBILE_PHONE, OpenAPI_device_type_SMART_PHONE, OpenAPI_device_type_TABLET, OpenAPI_device_type_DONGLE, OpenAPI_device_type_MODEM, OpenAPI_device_type_WLAN_ROUTER, OpenAPI_device_type_IOT_DEVICE, OpenAPI_device_type_WEARABLE, OpenAPI_device_type_MOBILE_TEST_PLATFORM, OpenAPI_device_type_UNDEFINED } OpenAPI_device_type_e;

char* OpenAPI_device_type_ToString(OpenAPI_device_type_e device_type);

OpenAPI_device_type_e OpenAPI_device_type_FromString(char* device_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_device_type_H_ */

