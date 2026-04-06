/*
 * vfl_capability_type.h
 *
 * Type of Vertical Federated Learning Capability 
 */

#ifndef _OpenAPI_vfl_capability_type_H_
#define _OpenAPI_vfl_capability_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_vfl_capability_type_NULL = 0, OpenAPI_vfl_capability_type_VFL_SERVER, OpenAPI_vfl_capability_type_VFL_CLIENT, OpenAPI_vfl_capability_type_VFL_SERVER_AND_CLIENT } OpenAPI_vfl_capability_type_e;

char* OpenAPI_vfl_capability_type_ToString(OpenAPI_vfl_capability_type_e vfl_capability_type);

OpenAPI_vfl_capability_type_e OpenAPI_vfl_capability_type_FromString(char* vfl_capability_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vfl_capability_type_H_ */

