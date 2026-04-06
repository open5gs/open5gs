/*
 * header_handling_action.h
 *
 * 
 */

#ifndef _OpenAPI_header_handling_action_H_
#define _OpenAPI_header_handling_action_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_header_handling_action_NULL = 0, OpenAPI_header_handling_action_DETECT, OpenAPI_header_handling_action__REMOVE, OpenAPI_header_handling_action_REPLACE, OpenAPI_header_handling_action_INSERT } OpenAPI_header_handling_action_e;

char* OpenAPI_header_handling_action_ToString(OpenAPI_header_handling_action_e header_handling_action);

OpenAPI_header_handling_action_e OpenAPI_header_handling_action_FromString(char* header_handling_action);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_header_handling_action_H_ */

