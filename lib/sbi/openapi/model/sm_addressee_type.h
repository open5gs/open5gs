/*
 * sm_addressee_type.h
 *
 * 
 */

#ifndef _OpenAPI_sm_addressee_type_H_
#define _OpenAPI_sm_addressee_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sm_addressee_type_NULL = 0, OpenAPI_sm_addressee_type_TO, OpenAPI_sm_addressee_type_CC, OpenAPI_sm_addressee_type_BCC } OpenAPI_sm_addressee_type_e;

char* OpenAPI_sm_addressee_type_ToString(OpenAPI_sm_addressee_type_e sm_addressee_type);

OpenAPI_sm_addressee_type_e OpenAPI_sm_addressee_type_FromString(char* sm_addressee_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_addressee_type_H_ */

