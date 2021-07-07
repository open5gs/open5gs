/*
 * sign.h
 *
 * 
 */

#ifndef _OpenAPI_sign_H_
#define _OpenAPI_sign_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sign_NULL = 0, OpenAPI_sign_POSITIVE, OpenAPI_sign_NEGATIVE } OpenAPI_sign_e;

char* OpenAPI_sign_ToString(OpenAPI_sign_e sign);

OpenAPI_sign_e OpenAPI_sign_FromString(char* sign);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sign_H_ */

