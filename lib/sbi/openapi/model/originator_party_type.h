/*
 * originator_party_type.h
 *
 * 
 */

#ifndef _OpenAPI_originator_party_type_H_
#define _OpenAPI_originator_party_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_originator_party_type_NULL = 0, OpenAPI_originator_party_type_CALLING, OpenAPI_originator_party_type_CALLED } OpenAPI_originator_party_type_e;

char* OpenAPI_originator_party_type_ToString(OpenAPI_originator_party_type_e originator_party_type);

OpenAPI_originator_party_type_e OpenAPI_originator_party_type_FromString(char* originator_party_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_originator_party_type_H_ */

