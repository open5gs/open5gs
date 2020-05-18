/*
 * change_type.h
 *
 *
 */

#ifndef _OpenAPI_change_type_H_
#define _OpenAPI_change_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_change_type_NULL = 0, OpenAPI_change_type_ADD, OpenAPI_change_type_MOVE, OpenAPI_change_type__REMOVE, OpenAPI_change_type_REPLACE } OpenAPI_change_type_e;

char* OpenAPI_change_type_ToString(OpenAPI_change_type_e change_type);

OpenAPI_change_type_e OpenAPI_change_type_FromString(char* change_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_change_type_H_ */

