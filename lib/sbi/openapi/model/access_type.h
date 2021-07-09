/*
 * access_type.h
 *
 * 
 */

#ifndef _OpenAPI_access_type_H_
#define _OpenAPI_access_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_access_type_NULL = 0, OpenAPI_access_type_3GPP_ACCESS, OpenAPI_access_type_NON_3GPP_ACCESS } OpenAPI_access_type_e;

char* OpenAPI_access_type_ToString(OpenAPI_access_type_e access_type);

OpenAPI_access_type_e OpenAPI_access_type_FromString(char* access_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_type_H_ */

