/*
 * sm_context_type.h
 *
 * 
 */

#ifndef _OpenAPI_sm_context_type_H_
#define _OpenAPI_sm_context_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sm_context_type_NULL = 0, OpenAPI_sm_context_type_EPS_PDN_CONNECTION, OpenAPI_sm_context_type_SM_CONTEXT, OpenAPI_sm_context_type_AF_COORDINATION_INFO } OpenAPI_sm_context_type_e;

char* OpenAPI_sm_context_type_ToString(OpenAPI_sm_context_type_e sm_context_type);

OpenAPI_sm_context_type_e OpenAPI_sm_context_type_FromString(char* sm_context_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_context_type_H_ */

