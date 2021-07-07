/*
 * rsn.h
 *
 * 
 */

#ifndef _OpenAPI_rsn_H_
#define _OpenAPI_rsn_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_rsn_NULL = 0, OpenAPI_rsn_V1, OpenAPI_rsn_V2 } OpenAPI_rsn_e;

char* OpenAPI_rsn_ToString(OpenAPI_rsn_e rsn);

OpenAPI_rsn_e OpenAPI_rsn_FromString(char* rsn);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rsn_H_ */

