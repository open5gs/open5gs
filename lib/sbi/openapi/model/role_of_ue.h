/*
 * role_of_ue.h
 *
 * 
 */

#ifndef _OpenAPI_role_of_ue_H_
#define _OpenAPI_role_of_ue_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_role_of_ue_NULL = 0, OpenAPI_role_of_ue_ANNOUNCING_UE, OpenAPI_role_of_ue_MONITORING_UE, OpenAPI_role_of_ue_REQUESTOR_UE, OpenAPI_role_of_ue_REQUESTED_UE } OpenAPI_role_of_ue_e;

char* OpenAPI_role_of_ue_ToString(OpenAPI_role_of_ue_e role_of_ue);

OpenAPI_role_of_ue_e OpenAPI_role_of_ue_FromString(char* role_of_ue);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_role_of_ue_H_ */

