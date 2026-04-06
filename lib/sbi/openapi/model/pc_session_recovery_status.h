/*
 * pc_session_recovery_status.h
 *
 * 
 */

#ifndef _OpenAPI_pc_session_recovery_status_H_
#define _OpenAPI_pc_session_recovery_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_pc_session_recovery_status_NULL = 0, OpenAPI_pc_session_recovery_status_REST_REQUEST, OpenAPI_pc_session_recovery_status_REST_TRIGGERED, OpenAPI_pc_session_recovery_status_REST_NOT_TRIGGERED, OpenAPI_pc_session_recovery_status_NOT_FOUND } OpenAPI_pc_session_recovery_status_e;

char* OpenAPI_pc_session_recovery_status_ToString(OpenAPI_pc_session_recovery_status_e pc_session_recovery_status);

OpenAPI_pc_session_recovery_status_e OpenAPI_pc_session_recovery_status_FromString(char* pc_session_recovery_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pc_session_recovery_status_H_ */

