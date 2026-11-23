/*
 * nni_session_direction.h
 *
 * 
 */

#ifndef _OpenAPI_nni_session_direction_H_
#define _OpenAPI_nni_session_direction_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nni_session_direction_NULL = 0, OpenAPI_nni_session_direction_INBOUND, OpenAPI_nni_session_direction_OUTBOUND } OpenAPI_nni_session_direction_e;

char* OpenAPI_nni_session_direction_ToString(OpenAPI_nni_session_direction_e nni_session_direction);

OpenAPI_nni_session_direction_e OpenAPI_nni_session_direction_FromString(char* nni_session_direction);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nni_session_direction_H_ */

