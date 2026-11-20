/*
 * media_initiator_flag.h
 *
 * 
 */

#ifndef _OpenAPI_media_initiator_flag_H_
#define _OpenAPI_media_initiator_flag_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_media_initiator_flag_NULL = 0, OpenAPI_media_initiator_flag_CALLED_PARTY, OpenAPI_media_initiator_flag_CALLING_PARTY, OpenAPI_media_initiator_flag_UNKNOWN } OpenAPI_media_initiator_flag_e;

char* OpenAPI_media_initiator_flag_ToString(OpenAPI_media_initiator_flag_e media_initiator_flag);

OpenAPI_media_initiator_flag_e OpenAPI_media_initiator_flag_FromString(char* media_initiator_flag);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_initiator_flag_H_ */

