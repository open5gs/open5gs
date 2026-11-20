/*
 * sdp_type.h
 *
 * 
 */

#ifndef _OpenAPI_sdp_type_H_
#define _OpenAPI_sdp_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sdp_type_NULL = 0, OpenAPI_sdp_type_OFFER, OpenAPI_sdp_type_ANSWER } OpenAPI_sdp_type_e;

char* OpenAPI_sdp_type_ToString(OpenAPI_sdp_type_e sdp_type);

OpenAPI_sdp_type_e OpenAPI_sdp_type_FromString(char* sdp_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sdp_type_H_ */

