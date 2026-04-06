/*
 * connection_capabilities.h
 *
 * 
 */

#ifndef _OpenAPI_connection_capabilities_H_
#define _OpenAPI_connection_capabilities_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_connection_capabilities_NULL = 0, OpenAPI_connection_capabilities_IMS, OpenAPI_connection_capabilities_MMS, OpenAPI_connection_capabilities_SUPL, OpenAPI_connection_capabilities_INTERNET, OpenAPI_connection_capabilities_IOT_DELAY_TOLERANT, OpenAPI_connection_capabilities_IOT_NON_DELAY_TOLERANT, OpenAPI_connection_capabilities_DL_STREAMING, OpenAPI_connection_capabilities_UL_STREAMING, OpenAPI_connection_capabilities_VEHIC_COMM, OpenAPI_connection_capabilities_REAL_TIME_INTERACTIVE, OpenAPI_connection_capabilities_UNIFIED_COMM, OpenAPI_connection_capabilities_BACKGROUND, OpenAPI_connection_capabilities_MISS_CRITICAL, OpenAPI_connection_capabilities_TIME_CRITICAL, OpenAPI_connection_capabilities_LOW_LAT_LOSS_TOL_UNACK } OpenAPI_connection_capabilities_e;

char* OpenAPI_connection_capabilities_ToString(OpenAPI_connection_capabilities_e connection_capabilities);

OpenAPI_connection_capabilities_e OpenAPI_connection_capabilities_FromString(char* connection_capabilities);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_connection_capabilities_H_ */

