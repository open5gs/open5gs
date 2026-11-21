/*
 * nni_relationship_mode.h
 *
 * 
 */

#ifndef _OpenAPI_nni_relationship_mode_H_
#define _OpenAPI_nni_relationship_mode_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nni_relationship_mode_NULL = 0, OpenAPI_nni_relationship_mode_TRUSTED, OpenAPI_nni_relationship_mode_NON_TRUSTED } OpenAPI_nni_relationship_mode_e;

char* OpenAPI_nni_relationship_mode_ToString(OpenAPI_nni_relationship_mode_e nni_relationship_mode);

OpenAPI_nni_relationship_mode_e OpenAPI_nni_relationship_mode_FromString(char* nni_relationship_mode);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nni_relationship_mode_H_ */

