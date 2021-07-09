/*
 * binding_level.h
 *
 * 
 */

#ifndef _OpenAPI_binding_level_H_
#define _OpenAPI_binding_level_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_binding_level_NULL = 0, OpenAPI_binding_level_NF_SET, OpenAPI_binding_level_NF_INSTANCE } OpenAPI_binding_level_e;

char* OpenAPI_binding_level_ToString(OpenAPI_binding_level_e binding_level);

OpenAPI_binding_level_e OpenAPI_binding_level_FromString(char* binding_level);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_binding_level_H_ */

