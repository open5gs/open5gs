/*
 * scp_capability.h
 *
 * 
 */

#ifndef _OpenAPI_scp_capability_H_
#define _OpenAPI_scp_capability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_scp_capability_NULL = 0, OpenAPI_scp_capability_INDIRECT_COM_WITH_DELEG_DISC } OpenAPI_scp_capability_e;

char* OpenAPI_scp_capability_ToString(OpenAPI_scp_capability_e scp_capability);

OpenAPI_scp_capability_e OpenAPI_scp_capability_FromString(char* scp_capability);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scp_capability_H_ */

