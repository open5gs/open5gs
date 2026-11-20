/*
 * direct_discovery_model.h
 *
 * 
 */

#ifndef _OpenAPI_direct_discovery_model_H_
#define _OpenAPI_direct_discovery_model_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_direct_discovery_model_NULL = 0, OpenAPI_direct_discovery_model_MODEL_A, OpenAPI_direct_discovery_model_MODEL_B } OpenAPI_direct_discovery_model_e;

char* OpenAPI_direct_discovery_model_ToString(OpenAPI_direct_discovery_model_e direct_discovery_model);

OpenAPI_direct_discovery_model_e OpenAPI_direct_discovery_model_FromString(char* direct_discovery_model);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_direct_discovery_model_H_ */

