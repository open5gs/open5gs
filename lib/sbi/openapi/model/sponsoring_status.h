/*
 * sponsoring_status.h
 *
 * 
 */

#ifndef _OpenAPI_sponsoring_status_H_
#define _OpenAPI_sponsoring_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sponsoring_status_NULL = 0, OpenAPI_sponsoring_status_SPONSOR_DISABLED, OpenAPI_sponsoring_status_SPONSOR_ENABLED } OpenAPI_sponsoring_status_e;

char* OpenAPI_sponsoring_status_ToString(OpenAPI_sponsoring_status_e sponsoring_status);

OpenAPI_sponsoring_status_e OpenAPI_sponsoring_status_FromString(char* sponsoring_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sponsoring_status_H_ */

