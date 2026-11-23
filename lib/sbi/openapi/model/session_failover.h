/*
 * session_failover.h
 *
 * 
 */

#ifndef _OpenAPI_session_failover_H_
#define _OpenAPI_session_failover_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_session_failover_NULL = 0, OpenAPI_session_failover_FAILOVER_NOT_SUPPORTED, OpenAPI_session_failover_FAILOVER_SUPPORTED } OpenAPI_session_failover_e;

char* OpenAPI_session_failover_ToString(OpenAPI_session_failover_e session_failover);

OpenAPI_session_failover_e OpenAPI_session_failover_FromString(char* session_failover);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_session_failover_H_ */

