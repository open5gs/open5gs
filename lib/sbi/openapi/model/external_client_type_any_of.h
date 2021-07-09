/*
 * external_client_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_external_client_type_any_of_H_
#define _OpenAPI_external_client_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_external_client_type_any_of_NULL = 0, OpenAPI_external_client_type_any_of_EMERGENCY_SERVICES, OpenAPI_external_client_type_any_of_VALUE_ADDED_SERVICES, OpenAPI_external_client_type_any_of_PLMN_OPERATOR_SERVICES, OpenAPI_external_client_type_any_of_LAWFUL_INTERCEPT_SERVICES, OpenAPI_external_client_type_any_of_PLMN_OPERATOR_BROADCAST_SERVICES, OpenAPI_external_client_type_any_of_PLMN_OPERATOR_OM, OpenAPI_external_client_type_any_of_PLMN_OPERATOR_ANONYMOUS_STATISTICS, OpenAPI_external_client_type_any_of_PLMN_OPERATOR_TARGET_MS_SERVICE_SUPPORT } OpenAPI_external_client_type_any_of_e;

char* OpenAPI_external_client_type_any_of_ToString(OpenAPI_external_client_type_any_of_e external_client_type_any_of);

OpenAPI_external_client_type_any_of_e OpenAPI_external_client_type_any_of_FromString(char* external_client_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_external_client_type_any_of_H_ */

