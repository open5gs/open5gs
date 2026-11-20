/*
 * prose_functionality.h
 *
 * 
 */

#ifndef _OpenAPI_prose_functionality_H_
#define _OpenAPI_prose_functionality_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_prose_functionality_NULL = 0, OpenAPI_prose_functionality_DIRECT_DISCOVERY, OpenAPI_prose_functionality_DIRECT_COMMUNICATION } OpenAPI_prose_functionality_e;

char* OpenAPI_prose_functionality_ToString(OpenAPI_prose_functionality_e prose_functionality);

OpenAPI_prose_functionality_e OpenAPI_prose_functionality_FromString(char* prose_functionality);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_prose_functionality_H_ */

