/*
 * ims_session_priority.h
 *
 * 
 */

#ifndef _OpenAPI_ims_session_priority_H_
#define _OpenAPI_ims_session_priority_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ims_session_priority_NULL = 0, OpenAPI_ims_session_priority_PRIORITY_0, OpenAPI_ims_session_priority_PRIORITY_1, OpenAPI_ims_session_priority_PRIORITY_2, OpenAPI_ims_session_priority_PRIORITY_3, OpenAPI_ims_session_priority_PRIORITY_4 } OpenAPI_ims_session_priority_e;

char* OpenAPI_ims_session_priority_ToString(OpenAPI_ims_session_priority_e ims_session_priority);

OpenAPI_ims_session_priority_e OpenAPI_ims_session_priority_FromString(char* ims_session_priority);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ims_session_priority_H_ */

