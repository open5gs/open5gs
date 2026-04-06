/*
 * activation_status.h
 *
 * 
 */

#ifndef _OpenAPI_activation_status_H_
#define _OpenAPI_activation_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_activation_status_NULL = 0, OpenAPI_activation_status_ACTIVE, OpenAPI_activation_status_NOT_ACTIVE } OpenAPI_activation_status_e;

char* OpenAPI_activation_status_ToString(OpenAPI_activation_status_e activation_status);

OpenAPI_activation_status_e OpenAPI_activation_status_FromString(char* activation_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_activation_status_H_ */

