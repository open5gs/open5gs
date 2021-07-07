/*
 * sc_type.h
 *
 * 
 */

#ifndef _OpenAPI_sc_type_H_
#define _OpenAPI_sc_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sc_type_NULL = 0, OpenAPI_sc_type_NATIVE, OpenAPI_sc_type_MAPPED } OpenAPI_sc_type_e;

char* OpenAPI_sc_type_ToString(OpenAPI_sc_type_e sc_type);

OpenAPI_sc_type_e OpenAPI_sc_type_FromString(char* sc_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sc_type_H_ */

