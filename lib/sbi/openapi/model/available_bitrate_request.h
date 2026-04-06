/*
 * available_bitrate_request.h
 *
 * 
 */

#ifndef _OpenAPI_available_bitrate_request_H_
#define _OpenAPI_available_bitrate_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_available_bitrate_request_NULL = 0, OpenAPI_available_bitrate_request_UL, OpenAPI_available_bitrate_request_DL, OpenAPI_available_bitrate_request_BOTH, OpenAPI_available_bitrate_request_STOP } OpenAPI_available_bitrate_request_e;

char* OpenAPI_available_bitrate_request_ToString(OpenAPI_available_bitrate_request_e available_bitrate_request);

OpenAPI_available_bitrate_request_e OpenAPI_available_bitrate_request_FromString(char* available_bitrate_request);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_available_bitrate_request_H_ */

