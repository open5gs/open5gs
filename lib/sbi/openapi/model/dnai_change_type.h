/*
 * dnai_change_type.h
 *
 * 
 */

#ifndef _OpenAPI_dnai_change_type_H_
#define _OpenAPI_dnai_change_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_dnai_change_type_NULL = 0, OpenAPI_dnai_change_type_EARLY, OpenAPI_dnai_change_type_EARLY_LATE, OpenAPI_dnai_change_type_LATE } OpenAPI_dnai_change_type_e;

char* OpenAPI_dnai_change_type_ToString(OpenAPI_dnai_change_type_e dnai_change_type);

OpenAPI_dnai_change_type_e OpenAPI_dnai_change_type_FromString(char* dnai_change_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnai_change_type_H_ */

