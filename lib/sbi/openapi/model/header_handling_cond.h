/*
 * header_handling_cond.h
 *
 * 
 */

#ifndef _OpenAPI_header_handling_cond_H_
#define _OpenAPI_header_handling_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_header_handling_cond_NULL = 0, OpenAPI_header_handling_cond_EVERY_MATCH, OpenAPI_header_handling_cond_FIRST_MATCH } OpenAPI_header_handling_cond_e;

char* OpenAPI_header_handling_cond_ToString(OpenAPI_header_handling_cond_e header_handling_cond);

OpenAPI_header_handling_cond_e OpenAPI_header_handling_cond_FromString(char* header_handling_cond);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_header_handling_cond_H_ */

