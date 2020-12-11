/*
 * restriction_type.h
 *
 *
 */

#ifndef _OpenAPI_restriction_type_H_
#define _OpenAPI_restriction_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_restriction_type_NULL = 0, OpenAPI_restriction_type_ALLOWED_AREAS, OpenAPI_restriction_type_NOT_ALLOWED_AREAS } OpenAPI_restriction_type_e;

char* OpenAPI_restriction_type_ToString(OpenAPI_restriction_type_e restriction_type);

OpenAPI_restriction_type_e OpenAPI_restriction_type_FromString(char* restriction_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_restriction_type_H_ */

