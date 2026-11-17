/*
 * class_identifier.h
 *
 * 
 */

#ifndef _OpenAPI_class_identifier_H_
#define _OpenAPI_class_identifier_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_class_identifier_NULL = 0, OpenAPI_class_identifier_PERSONAL, OpenAPI_class_identifier_ADVERTISEMENT, OpenAPI_class_identifier_INFORMATIONAL, OpenAPI_class_identifier__AUTO } OpenAPI_class_identifier_e;

char* OpenAPI_class_identifier_ToString(OpenAPI_class_identifier_e class_identifier);

OpenAPI_class_identifier_e OpenAPI_class_identifier_FromString(char* class_identifier);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_class_identifier_H_ */

