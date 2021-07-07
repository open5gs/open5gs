/*
 * requested_rule_data_type.h
 *
 * 
 */

#ifndef _OpenAPI_requested_rule_data_type_H_
#define _OpenAPI_requested_rule_data_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_requested_rule_data_type_NULL = 0, OpenAPI_requested_rule_data_type_CH_ID, OpenAPI_requested_rule_data_type_MS_TIME_ZONE, OpenAPI_requested_rule_data_type_USER_LOC_INFO, OpenAPI_requested_rule_data_type_RES_RELEASE, OpenAPI_requested_rule_data_type_SUCC_RES_ALLO, OpenAPI_requested_rule_data_type_EPS_FALLBACK } OpenAPI_requested_rule_data_type_e;

char* OpenAPI_requested_rule_data_type_ToString(OpenAPI_requested_rule_data_type_e requested_rule_data_type);

OpenAPI_requested_rule_data_type_e OpenAPI_requested_rule_data_type_FromString(char* requested_rule_data_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_requested_rule_data_type_H_ */

