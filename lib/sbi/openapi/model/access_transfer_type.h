/*
 * access_transfer_type.h
 *
 * 
 */

#ifndef _OpenAPI_access_transfer_type_H_
#define _OpenAPI_access_transfer_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_access_transfer_type_NULL = 0, OpenAPI_access_transfer_type_PS_TO_CS, OpenAPI_access_transfer_type_CS_TO_PS, OpenAPI_access_transfer_type_PS_TO_PS, OpenAPI_access_transfer_type_CS_TO_CS } OpenAPI_access_transfer_type_e;

char* OpenAPI_access_transfer_type_ToString(OpenAPI_access_transfer_type_e access_transfer_type);

OpenAPI_access_transfer_type_e OpenAPI_access_transfer_type_FromString(char* access_transfer_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_transfer_type_H_ */

