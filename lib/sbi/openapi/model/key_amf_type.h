/*
 * key_amf_type.h
 *
 * 
 */

#ifndef _OpenAPI_key_amf_type_H_
#define _OpenAPI_key_amf_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_key_amf_type_NULL = 0, OpenAPI_key_amf_type_KAMF, OpenAPI_key_amf_type_KPRIMEAMF } OpenAPI_key_amf_type_e;

char* OpenAPI_key_amf_type_ToString(OpenAPI_key_amf_type_e key_amf_type);

OpenAPI_key_amf_type_e OpenAPI_key_amf_type_FromString(char* key_amf_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_key_amf_type_H_ */

