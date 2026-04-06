/*
 * up_connection_status.h
 *
 * UP Connection Status.
 */

#ifndef _OpenAPI_up_connection_status_H_
#define _OpenAPI_up_connection_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_up_connection_status_NULL = 0, OpenAPI_up_connection_status_ESTABLISHED, OpenAPI_up_connection_status_RELEASED, OpenAPI_up_connection_status_MOVE } OpenAPI_up_connection_status_e;

char* OpenAPI_up_connection_status_ToString(OpenAPI_up_connection_status_e up_connection_status);

OpenAPI_up_connection_status_e OpenAPI_up_connection_status_FromString(char* up_connection_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_connection_status_H_ */

