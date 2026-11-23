/*
 * nni_type.h
 *
 * 
 */

#ifndef _OpenAPI_nni_type_H_
#define _OpenAPI_nni_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nni_type_NULL = 0, OpenAPI_nni_type_NON_ROAMING, OpenAPI_nni_type_ROAMING_NO_LOOPBACK, OpenAPI_nni_type_ROAMING_LOOPBACK } OpenAPI_nni_type_e;

char* OpenAPI_nni_type_ToString(OpenAPI_nni_type_e nni_type);

OpenAPI_nni_type_e OpenAPI_nni_type_FromString(char* nni_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nni_type_H_ */

