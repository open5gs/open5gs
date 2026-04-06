/*
 * external_client_type.h
 *
 * Indicates types of External Clients.
 */

#ifndef _OpenAPI_external_client_type_H_
#define _OpenAPI_external_client_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_external_client_type_NULL = 0, OpenAPI_external_client_type_EMERGENCY_SERVICES, OpenAPI_external_client_type_VALUE_ADDED_SERVICES, OpenAPI_external_client_type_PLMN_OPERATOR_SERVICES, OpenAPI_external_client_type_LAWFUL_INTERCEPT_SERVICES, OpenAPI_external_client_type_PLMN_OPERATOR_BROADCAST_SERVICES, OpenAPI_external_client_type_PLMN_OPERATOR_OM, OpenAPI_external_client_type_PLMN_OPERATOR_ANONYMOUS_STATISTICS, OpenAPI_external_client_type_PLMN_OPERATOR_TARGET_MS_SERVICE_SUPPORT, OpenAPI_external_client_type_SL_POS } OpenAPI_external_client_type_e;

char* OpenAPI_external_client_type_ToString(OpenAPI_external_client_type_e external_client_type);

OpenAPI_external_client_type_e OpenAPI_external_client_type_FromString(char* external_client_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_external_client_type_H_ */

